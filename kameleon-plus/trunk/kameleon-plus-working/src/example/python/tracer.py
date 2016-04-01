#1/usr/bin/python
"""
This program takes as input a kameleon-compatible cdf or hdf5 file, a variable, and a seed position.
It then computes a fieldline and interpolates the variable onto it, plotting the resulting fieldline in 3d
and a graph of the variable as a function of arc length.

todo: 
"""
import numpy as np
import sys, getopt
import time
@CCMC_MODULE_PATH_STR@
import _CCMC as ccmc


def main(argv):
    if (len(argv) == 5):
        filename = argv[0]
        variable = argv[1]
        c0 = float(argv[2])
        c1 = float(argv[3])
        c2 = float(argv[4])

        print 'Input file is "', filename
        print 'seed position:', c0, c1, c2
    
        kameleon = ccmc.Kameleon()
    
        kameleon.open(filename)
        kameleon.loadVariable("b")
        kameleon.loadVariable(variable)
        tracer = ccmc.Tracer(kameleon)
        tracer.setMaxIterations(20000)
        tracer.setDn(.2)   
        
        print 'Testing bidirectional trace at seed position:', c0, c1, c2
        start = time.clock()
        fieldline1 = tracer.bidirectionalTrace('b',c0,c1,c2)
        finish = time.clock()    
        elapsed = finish - start
        print 'Bidirectional trace completed. Elapsed time: ', elapsed, 'seconds.'



        interpolator = kameleon.createNewInterpolator()

        if module_exists("matplotlib"):
            import matplotlib.pyplot as plt
            from mpl_toolkits.mplot3d import axes3d
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')

            npts = fieldline1.size()
            x = np.zeros(npts); y = np.zeros(npts); z = np.zeros(npts)
            data = np.zeros(npts); arcLength = np.zeros(npts)

            for i in range(npts):
                p = fieldline1.getPosition(i)
                (x[i], y[i], z[i]) = (p.component1, p.component2, p.component3)
                data[i] = interpolator.interpolate(variable, x[i], y[i], z[i])
                arcLength[i] = fieldline1.getLength(i)

            def plotExtrema(extrema, color = 'r', marker = 'o'):
            #plot min and max values along fieldline
                for i in range(extrema.size()):
                    p  = fieldline1.getPosition(extrema[i])
                    ax.scatter(p.component1, p.component2, p.component3, c = color, marker  = marker)

            fieldline1.minmax()
            plotExtrema(fieldline1.minima)
            plotExtrema(fieldline1.maxima, 'b', '^')

            ax.plot(x, y, z, label='Magnetic field line')
            ax.legend()

            fig = plt.figure()
            plt.plot(arcLength, data, label='parametric data')
            plt.xlabel('arc length [' + kameleon.getVisUnit('x') + ']')
            plt.ylabel(variable + ' [' + kameleon.getVisUnit(variable) + ']')



            plt.show()

        else:
            print 'matplotlib not installed.. no 3d view supported.'

    else: print 'Usage: <filename> <variable> x, y, z \n python kameleon_test rho -40 0 0'
        


        
def module_exists(module_name):
    try:
        __import__(module_name)
    except ImportError:
        return False
    else:
        return True
    
if __name__ =="__main__":
    main(sys.argv[1:])




