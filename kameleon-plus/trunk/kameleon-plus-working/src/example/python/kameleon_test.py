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

	    kameleon = ccmc.Kameleon()
	    kameleon.open(filename)
	    kameleon.loadVariable(variable)

	    interpolator = kameleon.createNewInterpolator()
	    var = interpolator.interpolate(variable,c0, c1, c2)

	    print variable, var

	    kameleon.close()
	else:
		print 'Usage: <filename> <variable> x, y, z \n python kameleon_test rho -40 0 0' 

if __name__ == '__main__':
    main(sys.argv[1:])
