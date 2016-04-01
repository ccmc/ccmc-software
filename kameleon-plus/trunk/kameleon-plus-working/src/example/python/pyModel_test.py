# Warning: expect weird results if your python interpreter library doesn't match the python library used to build _CCMC.so
import sys
@CCMC_MODULE_PATH_STR@
import _CCMC as ccmc


def main(argv):
	if (len(argv) == 5):
	    filename = argv[0]
	    variable = argv[1]

	    c0 = float(argv[2])
	    c1 = float(argv[3])
	    c2 = float(argv[4])

	    pyModel = ccmc.PythonModel() #config file

	    pyModel.open(filename) 
	    # no variable loading necessary
	    interpolator = pyModel.createNewInterpolator()
	    result, dc0, dc1, dc2 = interpolator.interpolate_dc(variable,c0, c1, c2)
	    unit = pyModel.getNativeUnit(variable)
	    print variable,(c0,c1,c2),':\n\t', result, unit
	    print 'resolution', dc0, dc1, dc2

	    result = interpolator.interpolate(variable,c0,c1,c2)
	    print 'result:', result, unit


	else:
		print argv
		print 'Usage: \n<filename> <variable> x, y, z: \n\tpython pyModel_test /path/to/config/file.ini Mass_Density 1.1 0 0'

if __name__ == '__main__':
    main(sys.argv[1:])
