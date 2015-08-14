import sys
from CCMC import _CCMC as ccmc

def main(argv):
	if (len(argv) == 5):
	    filename = argv[0]
	    variable = argv[1]

	    c0 = float(argv[2])
	    c1 = float(argv[3])
	    c2 = float(argv[4])

	    pyModel = ccmc.PythonModel() #config file

	    pyModel.open(filename) 

	    interpolator = pyModel.createNewInterpolator()
	    result = interpolator.interpolate(variable,c0, c1, c2)
	    unit = pyModel.getNativeUnit(variable)
	    print result, unit
	else:
		print argv
		print 'Usage: <filename> <variable> x, y, z \n python pyModel_test /path/to/config/file.ini Mass_Density 1.1 0 0' 

if __name__ == '__main__':
    main(sys.argv[1:])
