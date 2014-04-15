#1/usr/bin/python

from numpy import *
import sys, getopt
sys.path.append("/usr/local/lib/python2.7/site-packages/kameleon_plus")
import time
import CCMC as ccmc

def main(argv):
    filename = argv[0]
    variable = argv[1]
    c0 = float(argv[2])
    c1 = float(argv[3])
    c2 = float(argv[4])
    
    print 'Hello from integrator! This is the python version of integrator in example/c++/'        
    print 'Input file is "', filename
    print 'seed position:', c0, c1, c2
#    
    kameleon = ccmc.Kameleon()
#
    kameleon.open(filename)
    kameleon.loadVariable(variable)
    tracer = ccmc.Tracer(kameleon)
    tracer.setMaxIterations(20000)
    tracer.setDn(.2)   
    
    print 'Testing bidirectional trace at seed position:', c0, c1, c2
    start = time.clock()
    f1 = tracer.bidirectionalTrace(variable,c0,c1,c2)
    finish = time.clock()    
    elapsed = finish - start
    print 'Bidirectional trace completed. Elapsed time: ', elapsed, 'seconds. (Compare with C++ version)'
    
    interpolator = kameleon.createNewInterpolator()
    
    f2 = ccmc.Fieldline()
    
    for f in range(f1.size()):
        p = f1.getPosition(f)
        f2.insertPointData(p,interpolator.interpolate(variable, p.component1,p.component2,p.component3))
     
    kameleon.close()
    
    print 'total points: ', f2.size()
    print 'points in f1:', f1.size()
 
    elengths = f2.getDs()
    length = f2.measure()

    
    for i in range(f2.size()):
        if ((i % 100 == 0 )|(i==f2.size()-1)):
            if (i>0):
                print "data at point ", i, "= ", f2.getData()[i], \
                    "; ds vector = ", f2.getElements()[i].component1, \
                    f2.getElements()[i].component2, f2.getElements()[i].component3,\
                    "element length: ", f2.getDs()[i], \
                    " total length: ", f2.measure()[i], " integral: ", f2.integrate()[i]
            else:
                print "data at point ", i, "= ", f2.getData()[i]
                
    f3 = f2.interpolate(1, 100)
    
    for i in range(f3.size()):
        p = f3.getPositions()[i]
        nearest = f3.getNearest()[i]
        print "New point [", i,"]: ", p.component1, p.component2, p.component3, \
            " data at new vertex: ", f3.getData()[i], "nearest lower vertex: ", nearest, \
            " data around nearest: [", f2.getData()[nearest], ",", 
        if (i == f3.size()-1):
            print f3.getData()[i], "]"
        else: print f2.getData()[nearest+1], "]"
        
                
    print 'goodbye'
    
if __name__ =="__main__":
    main(sys.argv[1:])




