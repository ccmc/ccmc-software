#1/usr/bin/python

from numpy import *
import sys, getopt
sys.path.append("/usr/local/lib/python2.7/site-packages/kameleon_plus")
import time
import CCMC as ccmc

def main(argv):
    print 'Hello from minmax!'
    filename = argv[0]
    positionsfile = argv[1]
    
    klobb = ccmc.Kameleon()

    print "Opening file ", filename
    klobb.open(filename)    

#   Initializing Tracer    
    tracer = ccmc.Tracer(klobb)
    tracer.setMaxIterations(20000)
    tracer.setDn(.2)   
    
#   Set up seed positions 
    print "Loading positions from", positionsfile
    positions = loadpositions(positionsfile)
    print "There are ", len(positions), "points in the file"

    for i, p in enumerate(positions):
        print 'Bidirectional trace at seed position:', p.component1, p.component2, p.component3
        f1 = tracer.bidirectionalTrace('b',p.component1,p.component2,p.component3)
        f2 = ccmc.Fieldline()
        interp = klobb.createNewInterpolator()
        for j in range(f1.size()):
            pnt = f1.getPosition(j)
            binv = interp.interpolate('b', pnt.component1,pnt.component2,pnt.component3)
            binv = 1.0/binv
            f2.insertPointData(pnt,binv)
        
        print "fieldline has size: ", f2.size()
        f2.minmax()
 
        print "fieldline has", f2.mincount, "minima,", f2.maxcount, "maxima \n\n"
    
    
#    Done with input file 
    klobb.close()

    print 'goodbye'




def loadpositions(file):
    file = open(file, 'r')
    positions = list()
    for line in file:
        pstr = line.split()
        p = ccmc.Point3f(float(pstr[0]), float(pstr[1]), float(pstr[2]))
        positions.append(p)
#        print "positions[-1] =", positions[-1].component1, positions[-1].component2, positions[-1].component3

    file.close()

    return positions

    
if __name__ =="__main__":
    main(sys.argv[1:])
