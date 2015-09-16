# Warning: expect weird results if your python interpreter library doesn't match the python library used to build _CCMC.so
import sys, argparse
# sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
sys.path.append('/Users/apembrok/git/ccmc/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/python/CCMC/')
import _CCMC as ccmc
import numpy as np
import collections
import json
import base64


def main(argv):
	parser = argparse.ArgumentParser(description="Interpolates variables onto grid.")
	parser.add_argument("-v", "--verbose", action="count", default=0, help = 'verbosity of output')
	parser.add_argument("input_file", metavar = 'full/path/to/input_file.cdf', type=str, help="kameleon-compatible file")
	parser.add_argument("-ginfo","--global-info", action='store_true', help = 'print global attributes')

	# requested variables
	var_options = parser.add_argument_group(title = 'variable options', description = 'List and interrogate variables. Not all variables will have interpolator support.')
	var_options.add_argument("-lvar", "--list-vars",action = 'store_true', help = 'list variables in the file (use -v to print all variable attributes)')
	var_options.add_argument("-vinfo","--variable-info", metavar = 'var', type = str, help = 'print attributes for given variable')
	var_options.add_argument("-vars", "--variables", type=str, nargs='+',metavar = ('var1','var2',), help='list of variables to be interpolated')
	var_options.add_argument("-pout", "--positions_out_flag", type = bool, metavar = 'positions_output_flag', default = True,
		help = 'pass interpolating positions to output')

	# positions file options
	in_positions_options = parser.add_argument_group(title = 'input positions file options', description = 'File containing positions for interpolation')
	in_positions_options.add_argument('-pfile', '--positions_file', type = str, metavar = '/path/to/input/positions.txt', 
		help = 'file containing column positions x, y, z. Optional separators: \' \' (default), <tab>, \',\' ')

	# single point options
	point_options = parser.add_argument_group(title = 'point options', description = 'interpolation options for a single point')
	point_options.add_argument("-p","--point", type=float, nargs=3, metavar=("px", "py", "pz"), help = 'point at which to interpolate variables')

	# grid options
	grid_options = parser.add_argument_group(title = 'grid options', description = 'interpolation options for a grid of points')
	grid_options.add_argument("-x", "--x-range", type = float, nargs = 2, metavar = ('xmin','xmax'), help = "range of x")
	grid_options.add_argument("-y", "--y-range", type = float, nargs = 2, metavar = ('ymin','ymax'), help = "range of y")
	grid_options.add_argument("-z", "--z-range", type = float, nargs = 2, metavar = ('zmin','zmax'), help = "range of z")
	# grid_options.add_argument("-b", "--box", type = float, nargs = 6, metavar=('xmin','xmax', 'ymin','ymax', 'zmin', 'zmax'), help = "min and max corners of the grid")
	grid_options.add_argument("-res", "--resolution", type = int, nargs = '+', metavar=('nx','ny',), help = "resolution of the grid along each axis")
	grid_options.add_argument("-xint","--x-intercept", type = float, default= 0, nargs =1, metavar = 'xint', help = 'fixes x for line or plane (ignores x-range)' )
	grid_options.add_argument("-yint","--y-intercept", type = float, default= 0, nargs =1, metavar = 'yint', help = 'fixes y for line or plane' )
	grid_options.add_argument("-zint","--z-intercept", type = float, default= 0, nargs =1, metavar = 'zint', help = 'fixes z for line or plane' )
	# grid_options.add_argument("-ind", "--indexing", type = str, default = 'ij', metavar= 'index_type', 
		# help = 'sets indexing of grid to be row major (matrix-style ij) or column major (cartesian-style xy). \nOptions: \'ij\' (default) or \'xy\'')
	grid_options.add_argument("-order", "--ordering", type = str, default = 'C', metavar = 'ordering',
		help = 'sets ordering of output arrays. options: \'C\' (default - C-style row major) or \'F\' (FORTRAN-style column major)')
	grid_options.add_argument("-t", "--transform", type = float, nargs = 3, help = 'transformation matrix to apply to grid before interpolating (not implemented yet)')

	#output options
	output_options = parser.add_argument_group(title = 'ouput options', description = 'where to store results of interpolation')
	output_options.add_argument("-o", "--output_file", type = str, metavar = 'path/to/output_file', help = 'output file name and location')
	output_options.add_argument("-f", "--format", default = '12.3f', type = str, metavar = '<flags><width><.precision><length>specifier', help = 'c-sytle format of output variables (e.g. 12.3f)')
	output_options.add_argument("-d", "--delimiter", default = ' ', type = str, metavar = "\' \'", help = 'delimiter for ascii output (default is \' \')')
	output_options.add_argument('-ff', '--file_format', default = 'txt', type = str, nargs = '+', metavar = ('fits', 'json'),
		help = 'File format for output. default: \'txt\' for ASCII. Use \'fits\' for binary IDL fits file (requires astropy), or \'json\'')

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

	def get_variable_format(positions_out_flag):
		ljust = len(('{0:'+args.format+'}').format(0))
		var_format = ''
		var_names = ''
		if positions_out_flag:
			for i, pos_name in enumerate(['x','y','z']):
				var_names += pos_name.rjust(ljust)+args.delimiter
				var_format += '{' + str(i) + ':'+ args.format +'}' + args.delimiter
		for i,varname in enumerate(args.variables):
			var_format += '{' + str(i+3*positions_out_flag) + ':'+ args.format +'}' + args.delimiter*((i+1)!=len(args.variables))
			unit = kameleon.getNativeUnit(varname)
			var_names += ('{0}[{1}]'.format(varname, unit)).rjust(ljust)+args.delimiter*((i+1)!=len(args.variables))
		return var_format, ljust, var_names

	if args.variables:		
		if args.verbose: 
			print 'loading', len(args.variables), "desired variables:", args.variables
		for varname in args.variables:
			if kameleon.doesVariableExist(varname):
				kameleon.loadVariable(varname)			
			else:
				print varname, 'does not exist!'
				exit()

		if args.positions_out_flag:
			variables_tuple = collections.namedtuple('Variables', ['x', 'y', 'z'] +args.variables)
		else:
			variables_tuple = collections.namedtuple('Variables', args.variables)

		var_format, ljust, var_names = get_variable_format(args.positions_out_flag)

		if args.point:
			c0, c1, c2 = args.point
			if args.verbose: 
				print 'interpolating at {0}:'.format(args.point)

			print var_names
			result = []
			if args.positions_out_flag:
				result = result + args.point
			for varname in args.variables:
				result_, dc0, dc1, dc2 = interpolator.interpolate_dc(varname,c0, c1, c2)
				result.append(result_)

			print var_format.format(*result)
			
		if args.resolution:
			if args.verbose: 
				print 'requested output resolution:', args.resolution

			if args.x_range:
				x_ = np.linspace(args.x_range[0], args.x_range[1], args.resolution[0])
				if args.verbose: 
					print 'x-range:', args.x_range
					if args.verbose > 1:
						print '\tvalues:', x_
			else:
				x_ = np.zeros(1) + args.x_intercept

			if args.y_range:
				y_ = np.linspace(args.y_range[0], args.y_range[1], args.resolution[1])
				if args.verbose: 
					print 'y-range:', args.y_range
					if args.verbose > 1:
						print '\tvalues:', y_
			else:
				y_ = np.zeros(1) + args.y_intercept

			if args.z_range:
				z_ = np.linspace(args.z_range[0], args.z_range[1], args.resolution[2])
				if args.verbose: 
					print 'z-range:', args.z_range
					if args.verbose > 1:
						print '\tvalues:', z_
			else:
				z_ = np.zeros(1) + args.z_intercept

			# x, y, z = np.meshgrid(x_,y_,z_, indexing = args.indexing)
			x, y, z = np.meshgrid(x_,y_,z_, indexing = 'ij')

		elif args.positions_file:
			with open(args.positions_file, 'r') as f:
				line0 = f.readline().strip()
				splitter = ' '
				if ',' in line0:
					splitter = ','
				elif '\t' in line0:
					splitter = '\t'
				positions = np.array(line0.split(splitter),dtype='float')
				for line in f:
					line = line.strip()
					pnext = np.array(line.split(splitter), dtype='float')
					positions = np.vstack((positions, pnext))

			x = positions[:,0]
			y = positions[:,1]
			z = positions[:,2]

		if args.transform:
			raise NotImplementedError('Grid transformations not implemented yet')

		if (args.positions_file != None) | (args.resolution != None):
			results = interpolate_variables(interpolator, variables_tuple, 
				x.ravel(order = args.ordering), y.ravel(order = args.ordering), z.ravel(order = args.ordering), 
				args.positions_out_flag)	
			results = variables_tuple(*results)
			if args.verbose:
				print 'output resolution:', x.shape
				if args.verbose > 1:
					for var_name, var_array in results._asdict().items():
						print '\t', var_name, var_array.shape
			
			if args.output_file:
				if 'txt' in args.file_format:
					if args.verbose: 
						print 'writing ASCII to', args.output_file
					with open(args.output_file, 'w') as f:
						f.write(var_names)
						f.write('\n')
						for variables in zip(*results):
							f.write(var_format.format(*variables))
							f.write('\n')
					f.close()
				if 'json' in args.file_format:
					import json
					if args.verbose:
						print 'writing json to', args.output_file
					
					with open(args.output_file, 'w') as f:
						json.dump(results._asdict(),f, cls = NumpyEncoder)
					f.close()
					if args.verbose > 1:
						print json.dumps(results._asdict(), cls = NumpyEncoder, indent=4, separators=(',',': '))

				if 'fits' in args.file_format:
					from astropy.io import fits
					if args.verbose: 
						print 'writing to fits file (IDL)'
					# generate header info
					primary_header = fits.Header()
					nglobal = kameleon.getNumberOfGlobalAttributes()
					#global attributes
					for i in range(nglobal):
						attr_name = kameleon.getGlobalAttributeName(i)
						attr = kameleon.getGlobalAttribute(attr_name)
						primary_header[str(i)] = attr_name + ': ' + str(getAttributeValue(attr))
					#variable attributes
					for varname in args.variables:
						for j in range(kameleon.getNumberOfVariableAttributes()):
							attr_name = kameleon.getVariableAttributeName(nglobal+j)
							attr = kameleon.getVariableAttribute(varname, attr_name)
							primary_header[varname + str(j)] = attr_name + ': '	+ str(getAttributeValue(attr))		
			
					primary_hdu = fits.PrimaryHDU(header = primary_header)

					# generate columns
					columns = []
					for var_name, var_array in results._asdict().items():
						columns.append(fits.Column(name = var_name, format = 'E', array = var_array))
					cols = fits.ColDefs(columns)
					hdutable = fits.BinTableHDU.from_columns(cols)
					
					thdulist = fits.HDUList([primary_hdu,hdutable])
					thdulist.writeto(args.output_file)
			else:
				print var_names
				for variables in zip(*results):
					print var_format.format(*variables)



			
@np.vectorize
def interpolate_variables(interpolator, variables_tuple, c0,c1,c2, positions_out_flag):
	"""returns a named tuple of interpolated variables"""
	results = []
	if positions_out_flag:
		results = results + [c0,c1,c2]
	for i, variable in enumerate(variables_tuple._fields):
		if positions_out_flag:
			if i > 2:	
				results.append(interpolator.interpolate(variable,c0,c1,c2))
		else:
			results.append(interpolator.interpolate(variable,c0,c1,c2))
	return variables_tuple(*results)


def getAttributeValue(attribute):
	if attribute.getAttributeType() == ccmc.Attribute.STRING:
		return attribute.getAttributeString()
	elif attribute.getAttributeType() == ccmc.Attribute.FLOAT:
		return attribute.getAttributeFloat()
	elif attribute.getAttributeType() == ccmc.Attribute.INT:
		return attribute.getAttributeInt()

class NumpyEncoder(json.JSONEncoder):
    def default(self, obj):
        """
        if input object is a ndarray it will be converted into a dict holding dtype, shape and the data base64 encoded
        """
        if isinstance(obj, np.ndarray):
            data_b64 = base64.b64encode(obj.data)
            return dict(__ndarray__=data_b64,
                        dtype=str(obj.dtype),
                        shape=obj.shape)
        # Let the base class default method raise the TypeError
        return json.JSONEncoder(self, obj)


def json_numpy_obj_hook(dct):
    """
    Decodes a previously encoded numpy ndarray
    with proper shape and dtype
    :param dct: (dict) json encoded ndarray
    :return: (ndarray) if input was an encoded ndarray
    """
    if isinstance(dct, dict) and '__ndarray__' in dct:
        data = base64.b64decode(dct['__ndarray__'])
        return np.frombuffer(data, dct['dtype']).reshape(dct['shape'])
    return dct

expected = np.arange(100, dtype=np.float)
dumped = json.dumps(expected, cls=NumpyEncoder)
result = json.loads(dumped, object_hook=json_numpy_obj_hook)

if __name__ == '__main__':
    main(sys.argv[1:])

