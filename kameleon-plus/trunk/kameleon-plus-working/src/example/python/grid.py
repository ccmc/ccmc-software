# Warning: expect weird results if your python interpreter library doesn't match the python library used to build _CCMC.so
import sys, argparse
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import _CCMC as ccmc
import numpy as np


def main(argv):
	parser = argparse.ArgumentParser(description="Interpolates variables onto grid.")
	parser.add_argument("-v", "--verbose", action="count", default=0, help = 'verbosity of output')
	parser.add_argument("input_file", metavar = 'full/path/to/input_file.cdf', type=str, help="kameleon-compatible file")
	parser.add_argument("-ginfo","--global-info", action='store_true', help = 'print global attributes')
	var_options = parser.add_argument_group(title = 'variable options', description = 'List and interrogate variables. Not all variables will have interpolator support.')
	var_options.add_argument("-lvar", "--list-vars",action = 'store_true', help = 'list variables in the file (use -v to print all variable attributes)')
	var_options.add_argument("-vinfo","--variable-info", metavar = 'var', type = str, help = 'print attributes for given variable')
	var_options.add_argument("-vars", "--variables", type=str, nargs='+',metavar = ('var1','var2',), help='list of variables to be interpolated')
	var_options.add_argument("-f", "--format", default = '8.3f', type = str, help = 'format of output variables')
	var_options.add_argument("-d", "--delimiter", default = ' ', type = str, help = 'delimiter for output (default is \' \')')

	point_options = parser.add_argument_group(title = 'point options', description = 'interpolation options for a single point')
	point_options.add_argument("-p","--point", type=float, nargs=3, metavar=("px", "py", "pz"), help = 'point at which to interpolate variables')

	grid_options = parser.add_argument_group(title = 'grid options', description = 'interpolation options for a grid of points')
	grid_options.add_argument("-x", "--x-range", type = float, nargs = 2, metavar = ('xmin','xmax'), help = "range of x")
	grid_options.add_argument("-y", "--y-range", type = float, nargs = 2, metavar = ('ymin','ymax'), help = "range of y")
	grid_options.add_argument("-z", "--z-range", type = float, nargs = 2, metavar = ('zmin','zmax'), help = "range of z")
	grid_options.add_argument("-b", "--box", type = float, nargs = 6, metavar=('xmin','xmax', 'ymin','ymax', 'zmin', 'zmax'), help = "min and max corners of the grid")
	grid_options.add_argument("-res", "--resolution", type = int, nargs = 3, metavar=('nx','ny','nz'), help = "resolution of the grid along each axis")
	args = parser.parse_args()

	
	kameleon = ccmc.Kameleon() 
	kameleon.open(args.input_file)

	if args.verbose:
		print "Running grid interpolator."

	if args.global_info:
		print "Global Attributes:"
		for i in range(kameleon.getNumberOfGlobalAttributes()):
			attr_name = kameleon.getGlobalAttributeName(i)
			attr = kameleon.getGlobalAttribute(attr_name)
			print '\t',attr_name, ':', getAttributeValue(attr)

	if args.list_vars:
		nvar = kameleon.getNumberOfVariables()
		nglobal = kameleon.getNumberOfGlobalAttributes() #variable attribute ids come after global ones
		if args.verbose: print 'number of variables in file:', nvar		
		for i in range(nvar):
			var_name = kameleon.getVariableName(i)
			native_unit = kameleon.getNativeUnit(var_name)
			print var_name, '[', native_unit ,']'
			if args.verbose:
				for j in range(kameleon.getNumberOfVariableAttributes()):
					attr_name = kameleon.getVariableAttributeName(nglobal+j)
					attr = kameleon.getVariableAttribute(var_name, attr_name)
					print '\t',attr_name, ':', getAttributeValue(attr)

	if args.variable_info:
		var_name = args.variable_info
		if args.verbose: print 'retrieving information for', args.variable_info, '...'
		if kameleon.doesVariableExist(args.variable_info):
			nglobal = kameleon.getNumberOfGlobalAttributes() #variable attribute ids come after global ones
			native_unit = kameleon.getNativeUnit(var_name)
			print var_name, '[', native_unit ,']'	
			for j in range(kameleon.getNumberOfVariableAttributes()):
				attr_name = kameleon.getVariableAttributeName(nglobal+j)
				attr = kameleon.getVariableAttribute(var_name, attr_name)
				print '\t',attr_name, ':', getAttributeValue(attr)
	

	interpolator = kameleon.createNewInterpolator()
	if args.variables:
		var_format = '{0:'+args.format + '}' + args.delimiter 
		if args.verbose: print 'loading', len(args.variables), "desired variables:"
		for varname in args.variables:
			kameleon.loadVariable(varname)


		if args.point:
			c0, c1, c2 = args.point
			if args.verbose: print 'interpolating at {0}:'.format(args.point)
			for varname in args.variables:
				if kameleon.doesVariableExist(varname):
					unit = kameleon.getNativeUnit(varname)
					print ('{0}[{1}]'+args.delimiter).format(varname, unit), 
				else:
					print varname, 'does not exist!'
					exit()
			print ''
			for varname in args.variables:
				result, dc0, dc1, dc2 = interpolator.interpolate_dc(varname,c0, c1, c2)
				print var_format.format(result),
			print ''


def getAttributeValue(attribute):
	if attribute.getAttributeType() == ccmc.Attribute.STRING:
		return attribute.getAttributeString()
	elif attribute.getAttributeType() == ccmc.Attribute.FLOAT:
		return attribute.getAttributeFloat()
	elif attribute.getAttributeType() == ccmc.Attribute.INT:
		return attribute.getAttributeInt()

if __name__ == '__main__':
    main(sys.argv[1:])

