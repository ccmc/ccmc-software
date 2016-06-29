import sys, time, argparse
@CCMC_SRC_PATH_STR@
@CCMC_MODULE_PATH_STR@
@CCMC_LIB_PATH_STR@
import numpy as np
from pyreaders.ARMS import readARMS
import _CCMC as ccmc
import json
import base64
from collections import namedtuple
import numpy.ma as ma

def main(argv):
	# raise Exception("TODO: YOU NEED TO FIX THE SET_ROOT_RANGES FUNCTION SO THAT IT RETURNS THE MIN AND MAX ARRAYS FOR ALL ROOT BOUNDING BOXES")

	parser = argparse.ArgumentParser(description="Direct test of ARMs reader and interpolator")
	parser.add_argument("-v", "--verbose", action="count", default=0, help = 'verbosity of output')
	parser.add_argument("input_file", metavar = '/path/to/ARMS/config/file/ARMs.ini', type=str, 
		help="ARMs config file contains paths to ARMS header and flicks data")
	parser.add_argument("-ginfo","--global-info", action='store_true', help = 'print global attributes')
	parser.add_argument("-db", "--debug", action = 'store_true', help = 'debugging flag')

	arms_options = parser.add_argument_group(title = "ARMS-specific options", description = 'Options specific to ARMS code')
	arms_options.add_argument("-in_coord", "--input-coordinates", type = str, default = 'Preferred', metavar = '<coordinate type>', 
				help = """sets coordinates of input grid. Options: 
				Preferred (default) - query positions coordinates are determined by ARMS.ini PreferredCoordinates attribute,
				ARMS - query positions are in the coordinate system of ARMS),
				CART - cartesian,
				SPHEXP - spherical exponential""")
	arms_options.add_argument("-tol", "--tolerance", type = float, default = None, metavar = '<normalized tolerance>',
		help = 'sets interpolation tolerance for ARMS scaled to block sizes. Default is .0001')

	# requested variables
	var_options = parser.add_argument_group(title = 'variable options', description = 'List and interrogate variables. Not all variables will have interpolator support.')
	var_options.add_argument("-lvar", "--list-vars",action = 'store_true', help = 'list variables in the file (use -v to print all variable attributes)')
	var_options.add_argument("-vinfo","--variable-info", metavar = 'var', type = str, help = 'print attributes for given variable')
	var_options.add_argument("-vars", "--variables", type=str, nargs='+',metavar = ('var1','var2',), help='list of variables to be interpolated')

	# single point interpolation
	point_options = parser.add_argument_group(title = 'point options', description = 'interpolation options for a single point')
	point_options.add_argument("-p","--point", type=float, nargs=3, metavar=("c0", "c1", "c2"), help = 'point at which to interpolate variables')


	# grid options
	grid_options = parser.add_argument_group(title = 'grid options', description = 'interpolation options for a grid of points in model')
	grid_options.add_argument("-copy_leaf", "--copy_leaf_ranges", action = 'store_true', help = 'use the ranges of the last leaf (see --leaf_key option)')
	grid_options.add_argument("-c0", "--c0-range", type = float, nargs = 2, metavar = ('c0min','c0max'), help = "range of c0")
	grid_options.add_argument("-c1", "--c1-range", type = float, nargs = 2, metavar = ('c1min','c1max'), help = "range of c1")
	grid_options.add_argument("-c2", "--c2-range", type = float, nargs = 2, metavar = ('c2min','c2max'), help = "range of c2")
	# grid_options.add_argument("-b", "--box", type = float, nargs = 6, metavar=('xmin','xmax', 'ymin','ymax', 'zmin', 'zmax'), help = "min and max corners of the grid")
	grid_options.add_argument("-res", "--resolution", type = int, nargs = '+', metavar=('n0','n1',), help = "resolution of the grid along each axis")
	grid_options.add_argument("-c0int","--c0-intercept", type = float, default= 0, nargs =1, metavar = 'c0int', help = 'fixes c0 for line or plane (ignores x-range)' )
	grid_options.add_argument("-c1int","--c1-intercept", type = float, default= 0, nargs =1, metavar = 'c1int', help = 'fixes c1 for line or plane' )
	grid_options.add_argument("-c2int","--c2-intercept", type = float, default= 0, nargs =1, metavar = 'c2int', help = 'fixes c2 for line or plane' )
	# grid_options.add_argument("-ind", "--indexing", type = str, default = 'ij', metavar= 'index_type', 
		# help = 'sets indexing of grid to be row major (matrix-style ij) or column major (cartesian-style xy). \nOptions: \'ij\' (default) or \'xy\'')
	grid_options.add_argument("-order", "--ordering", type = str, default = 'C', metavar = 'ordering',
		help = 'sets ordering of output arrays. options: \'C\' (default - C-style row major) or \'F\' (FORTRAN-style column major)')
	grid_options.add_argument("-t", "--transform", type = float, nargs = 3, help = 'transformation matrix to apply to grid before interpolating (not implemented yet)')


	# vis options
	vis_options = parser.add_argument_group(title = 'Vis Options', description = 'options for visualization')
	vis_options.add_argument("-vis", "--visualize", action = 'store_true', help = 'visualize with matplotlib')
	vis_options.add_argument("-rows", "--vis_rows", type = int, metavar = 'plot rows', help = 'number of rows to plot')
	vis_options.add_argument("-cols", "--vis_columns", type = int, metavar = 'plot columns', help = 'number of columns to plot')
	vis_options.add_argument("-slice0", "--slice_0", type = int, metavar = 'c0 index', help = 'slice along c0 for visualization')
	vis_options.add_argument("-slice1", "--slice_1", type = int, metavar = 'c1 index', help = 'slice along c1 for visualization')
	vis_options.add_argument("-slice2", "--slice_2", type = int, metavar = 'c2 index', help = 'slice along c2 for visualization')
	vis_options.add_argument("-pcoords", "--plot_coordinates", type = int, nargs = 2, default = (0,1), metavar = '<x-axis> <y-axis>', 
			help = 'components to use for x-axis. Default: 0 1 for X, Y when --input-coordinates = CART and R, T for --input-coordinates = SPHEXP')
	vis_options.add_argument("-levels", "--contour_levels", type = int, default = 20, metavar = 'number contours', help = 'number of contours to use in vis')
	vis_options.add_argument("-cvals", "--contour_values", type = float, nargs='+', metavar = ('var1_min','var1_max'), help = 'ranges for contour values')
	vis_options.add_argument("-leaf", "--leaf_key", type = int, nargs = '?', default = None, const = [-1, -1], metavar = ('key0'), help = 'key tuple for leaf to plot. default: plot last leaf')
	vis_options.add_argument("-lslice0", "--leaf_slice0", type = int, default = None, metavar = 'index0', 
		help = 'plots slice along 0th dimension of leaf defined by -leaf parameter. ex.: "-lslice0 0" for (0,:,:)')
	vis_options.add_argument("-lslice1", "--leaf_slice1", type = int, default = None, metavar = 'index0', 
		help = 'plots slice along 0th dimension of leaf defined by -leaf parameter. ex.: "-lslice1 -1" for (:,<last>,:)')
	vis_options.add_argument("-lslice2", "--leaf_slice2", type = int, default = None, metavar = 'index0', 
		help = 'plots slice along 0th dimension of leaf defined by -leaf parameter. ex.: "-lslice2 1" for (:,:,1)')
	vis_options.add_argument("-plv", "--plot_leaf_values", action = 'store_true', help = "make a line plot to see leaf ordering")
	vis_options.add_argument("-pgv", "--plot_grid_values", action = 'store_true', help = "make a line plot of grid values v index")
	vis_options.add_argument("-cmaps", "--color_maps", type = str, nargs = '+', default = None, metavar = 'cmap_name', help = 'matplotlib colormap ie bone rainbow ..')

	#output options
	output_options = parser.add_argument_group(title = 'ouput options', description = 'where to store results of interpolation')
	output_options.add_argument("-o", "--output_file", type = str, metavar = 'path/to/output_file', help = 'output file name and location')
	output_options.add_argument("-f", "--format", default = '12.3f', type = str, metavar = '<flags><width><.precision><length>specifier', help = 'c-sytle format of output variables (e.g. 12.3f)')
	output_options.add_argument("-d", "--delimiter", default = ' ', type = str, metavar = ("\' \'"), help = 'delimiter for ascii output (default is \' \')')
	output_options.add_argument('-ff', '--file_format', default = 'txt', type = str, nargs = '+', metavar = ('fits', 'md'),
		help = 'File format for output. default: \'txt\' for ASCII. Use \'fits\' for binary IDL fits file (requires astropy), or \'json\'')
	output_options.add_argument("-pout", "--positions_out_flag", type = bool, metavar = 'positions_output_flag', default = True,
		help = 'pass interpolating positions to output')


	args = parser.parse_args()


	if args.visualize:
		if module_exists("matplotlib"):
			import matplotlib.pyplot as plt
			import matplotlib.ticker as ticker
		else:
			raise ImportError('Need matplotlib to visualize, please install: pip install matplotlib')

	# readARMS can be initialized with or without a config file
	armsreader = readARMS(args.input_file)
	if args.debug: armsreader.debug = True
	if args.verbose:
		print 'ARMS reader initialized with config file'
		print 'ARMS config file:', armsreader.header_file_name


	# must open with config file
	armsreader.open(args.input_file)

	if args.input_coordinates == 'Preferred': # this is the default
		if args.verbose: 
			print "using ARMS.ini PreferredCoordinates: ",
			print armsreader.preferred_coordinates
		# Preferred could indicate ARMS, CART, or SPHEXP depending on what's defined in ARMS.ini
		args.input_coordinates = armsreader.preferred_coordinates
	else: #ignore the default in ARMS.ini and use input_coordinates instead
		armsreader.preferred_coordinates = args.input_coordinates

	if args.global_info:
		print "Global Attributes:"
		for i in range(armsreader.getNumberOfGlobalAttributes()):
			# print 'retrieving attribute name'
			attr_name = armsreader.getGlobalAttributeName(i)
			attr = armsreader.getGlobalAttribute(attr_name)
			print '\t',attr_name, ':', getAttributeValue(attr)

	if args.list_vars:
		nvar = armsreader.getNumberOfVariables()
		if args.verbose: print 'number of variables in file:', nvar		
		for i in range(nvar):
			var_name = armsreader.getVariableName(i)
			print var_name
			if args.verbose:
				for j in range(armsreader.getNumberOfVariableAttributes()):
					attr_name = armsreader.getVariableAttributeName(j)
					attr = armsreader.getVariableAttribute(var_name, attr_name)
					print '\t',attr_name, ':', getAttributeValue(attr)
	if args.variable_info:
		var_name = args.variable_info
		if args.verbose: print 'retrieving information for', args.variable_info, '...'
		if armsreader.doesVariableExist(args.variable_info):
			print var_name
			for j in range(armsreader.getNumberOfVariableAttributes()):
				attr_name = armsreader.getVariableAttributeName(j)
				attr = armsreader.getVariableAttribute(var_name, attr_name)
				print '\t',attr_name, ':', getAttributeValue(attr)

	if args.variables:
		if args.tolerance:
			if args.verbose: print 'setting interpolation tolerance to', args.tolerance
			armsreader.set_tolerance(args.tolerance)
			
		if args.verbose: 
			print 'loading', len(args.variables), "desired variables:", args.variables
		for var_name in args.variables:
			if armsreader.doesVariableExist(var_name):
				armsreader.getVariable(var_name)			
			else:
				print var_name, 'does not exist!'
				exit()

		if args.verbose > 1:
			armsreader._print_tree_info()

		if args.point:
			c0, c1, c2 = args.point
			if args.verbose: 
				print 'interpolating at {0}:'.format(args.point), args.input_coordinates
			result = []
			if args.positions_out_flag:
				result = result + args.point
				if args.debug: print 'result', result
			for varname in args.variables:
				result_ = armsreader.interpolate(varname,c0, c1, c2) #done in preferred coordinate system
				result.append(result_)
			print result


		def get_rows_columns(n):
			if args.vis_rows != None:
				rows = args.vis_rows
			else:
				rows = int(np.floor(np.sqrt(n)))
			if args.vis_columns != None:
				columns = args.vis_columns
			else:
				columns = int(np.ceil(float(n)/rows))
			if args.verbose: print 'plotting rows, columns:', rows, columns
			return rows, columns

		def get_input_coordinates():
			'''get component names of input positions'''
			if args.input_coordinates == 'ARMS':
				attr = armsreader.getGlobalAttribute('grid_system_1')
				component_names = getAttributeValue(attr)
				return component_names.split('[')[1].split(']')[0].split(',')
			elif args.input_coordinates == 'CART':
				return ['X','Y','Z']
			elif args.input_coordinates == 'SPHEXP':
				return ['logR','T','P']
			else:
				print 'input coordinates not supported. Exiting'
				exit()

		def index_where_equal(L, value, equal = True, else_value = -1):
			if equal: 
				return next((i for i, v in enumerate(L) if v == value), else_value)
			else: 
				return next((i for i, v in enumerate(L) if v != value), else_value)

		def plot_slice(	grid, resolution, variables, slices = (None,None, None), contour_values = None, contour_levels = 20, point = None, title = None, cmaps = None):
			"""Plot slice of data. Could be interpolated or raw data.

				Args: 
					grid (tuple or list of 3D arrays): c0, c1, c2 arrays
					resolution (int tuple): resolution of the input grid.
					variables (named tuple of arrays): variable arrays matching grid
					slices (optional int tuple): slice indices. Defaults to (None, None, None).
						The first non-None slice will be used
					contour_values (optional): 2 x Nvar ranges for variable colormap
					contour_levels: number of contours in each plot
					point (optional): a point to plot
			"""			
			PlotTuple = namedtuple('PlotTuple', ['x','y','var'])

			input_coordinates = get_input_coordinates()
			missing_value = armsreader.getMissingValue()
			if args.verbose: print 'missing value is ', missing_value

			rows, columns = get_rows_columns(len(variables))
			
			fig, axs = plt.subplots(rows,columns)
			fig.subplots_adjust(hspace=0.34, wspace = .34, left = .1)
			axs = axs.ravel()

			slice_ = [slice(None), slice(None), slice(None)]

			#see if grid resolution has a dimension of size 1
			slice_dim = index_where_equal(resolution, value = 1)
			if slice_dim != -1: # a 2D grid
				slice_num = 0 
			else: # a 3D grid. see whether a slice was specified
				slice_dim = index_where_equal(slices, value = None, equal = False)
				if slice_dim != -1:
					slice_num = slices[slice_dim]
				else:
					if args.verbose: print 'no slice specified.'
					slice_dim, slice_num = 2, 0

			if args.verbose: print 'slicing dim', slice_dim,'at', slice_num
			slice_[slice_dim] = slice_num

			x_label,y_label = [input_coordinates[i] for i in args.plot_coordinates] 
			plot_coordinates = [grid[i][slice_].squeeze() for i in args.plot_coordinates]
			if point != None:
				plot_point = [point[i] for i in args.plot_coordinates] 

			for var_index, var_name in enumerate(variables._fields):
				# if var_name =='Mass_Density':
				# 	variable = np.log10(variables[var_index])
				# 	log_str = 'log10 '
				# else:
				# 	log_str = ''
				# 	variable = variables[var_index]
				variable = variables[var_index][slice_].squeeze()
				if args.verbose: print var_name, variables[var_index].shape, 'sliced:', variable.shape
				variable = ma.masked_values(variable,armsreader.getMissingValue())
				plot_tuple = PlotTuple(*plot_coordinates,var=variable)

				# set this variable's contours
				if contour_values is None:
					if args.verbose: print '\tmin,max:', variable.min(), variable.max()
					levels = np.linspace(variable.min(),variable.max(),contour_levels)
				elif len(contour_values)/2 == len(args.variables): #use levels from input:	
					levels = np.linspace(contour_values[var_index*2], contour_values[var_index*2+1], contour_levels)
				else:
					levels = np.linspace(variable.min(),variable.max(),contour_levels)

				# set this variable's colormap
				if cmaps is None:
					cmap = None
				elif len(cmaps) == 1:
					cmap = cmaps[0]
				elif len(cmaps) == len(args.variables):
					cmap = cmaps[var_index]
				else:
					cmap = None
				try:
					if args.verbose > 1:
						print 'plot_tuple:'
						for i, field in enumerate(plot_tuple._fields): 
							print '\t',field, plot_tuple[i].shape, 'range:', plot_tuple[i].min(), plot_tuple[i].max()
					cs = axs[var_index].contourf(*plot_tuple, levels = levels, extend = 'both', cmap=cmap)
					if args.contour_values != None:
						if args.verbose: print 'plotting contours'
						axs[var_index].contour(*plot_tuple, levels = [levels[0], levels[-1]])
				except TypeError:
					for i, field in enumerate(plot_tuple._fields): print field, plot_tuple[i].shape
					print 'slice_, slice_dim', slice_, slice_dim
					print 'grid:'
					for i, g in enumerate(grid): 
						print '\t', i, g.shape, 'sliced:', g[slice_].shape
					print 'plot_coordinates:'
					for c in plot_coordinates: print '\t', c.shape
					raise

				axs[var_index].set_title(var_name)
				axs[var_index].set_xlabel(x_label)
				axs[var_index].set_ylabel(y_label)
				cbar = fig.colorbar(cs, ax=axs[var_index], shrink=0.9, format=ticker.FuncFormatter(fmt))
			
				units = armsreader.getVariableAttribute(var_name, 'units').getAttributeString()
				cbar.ax.set_ylabel(var_name + ' [' + units +  ']')
				if point != None:
					if args.verbose: print '\tplotting point', plot_point
					axs[var_index].plot(plot_point[0],plot_point[1],'o')

			if title!= None:
				fig.text(0.5, .95, title, ha = 'center', fontsize = 18)

		def get_leaf_key(leaf_key):
			if leaf_key == [-1, -1]:
				if args.verbose: print 'getting last leaf found'
				leaf_key = armsreader.last_key
			else: #use input key
				leaf_key = args.leaf_key
			return leaf_key

		def get_leaf_axes(leaf_key):
			bbx = armsreader.tree_data[leaf_key].bbx
			ni,nj,nk = armsreader.leaf_resolution
			c0 = np.linspace(bbx.r_min, bbx.r_max, ni)
			c1 = np.linspace(bbx.theta_min, bbx.theta_max, nj)
			c2 = np.linspace(bbx.phi_min, bbx.phi_max, nk)
			return c0, c1, c2

		def plot_variable_values(variables, rows_columns, title = '', names = None):
				fig, axs = plt.subplots(*rows_columns)
				axs = axs.ravel()
				fig.text(.5, .95, title, ha = 'center', fontsize = 18)
				if names != None:
					for var_index, var_name in enumerate(names):
						axs[var_index].plot(variables[var_index].ravel(order = args.ordering))
						axs[var_index].set_title(var_name)
				else:
					for var_index, variable in enumerate(variables):
						axs[var_index].plot(variable.ravel(order = args.ordering))

		def plot_leaf_slice(leaf_key, point = None, cmaps = None):	
			leaf_key = get_leaf_key(leaf_key)		
			if args.verbose: print 'leaf key: ', leaf_key, 'parent key:', armsreader.tree_data[leaf_key].parent_key
			c0_, c1_, c2_ = get_leaf_axes(leaf_key)
			leaf_grid = np.meshgrid(c0_,c1_,c2_, indexing = 'ij') # or 'ij'? in ARMS coordinates

			if args.verbose == 1: 
				print 'leaf grid from ARMS:'
				try:
					print 'type:',type(armsreader.leaf_data[leaf_key])
				except:
					pass

				for lc_index, lc in enumerate(leaf_grid):
					print '\t', lc_index, lc.shape, 'range', lc.min(), lc.max()
				print 'converting leaf_grid to', args.input_coordinates

			leaf_grid = armsreader.convert_positions_from_ARMS(leaf_grid, args.input_coordinates)

			if args.verbose == 1:
				print 'leaf grid in ' + args.input_coordinates + ':'
				for lc_index, lc in enumerate(leaf_grid):
					print '\t', lc_index, lc.shape, 'range', lc.min(), lc.max()

			leaf_slices = args.leaf_slice0, args.leaf_slice1, args.leaf_slice2
			leaf_tuple = namedtuple("leaf_data", args.variables)
			leaf_variables = [armsreader.leaf_data[leaf_key][var_name] for var_name in args.variables]
			lt = leaf_tuple(*leaf_variables)

			if args.plot_leaf_values:
				plot_variable_values(lt, get_rows_columns(len(lt)), 'values for leaf ' + str(leaf_key), lt._fields)
				plot_variable_values(leaf_grid, (3,1), 'leaf positions', ['c0', 'c1', 'c2'])
				
			if args.debug:
				print 'variables tuple:'
				for i, f in enumerate(lt._fields):
					print '\t',f, lt[i].shape

			plot_slice(	leaf_grid, armsreader.leaf_resolution, leaf_tuple(*leaf_variables), 
						leaf_slices, point = point, contour_levels = 20,
						title = 'leaf variables at ' + str(leaf_slices),
						cmaps = cmaps)

		#plot variables on a leaf
		if args.leaf_key == None:
			if args.verbose: print 'no leaf specified'
		else:
			plot_leaf_slice(args.leaf_key, args.point, cmap = args.cmap)

		# grid interpolation	
		if args.resolution:
			if args.verbose: 
				print 'requested output resolution:', args.resolution

			if args.copy_leaf_ranges:
				if args.leaf_key:
					leaf_key = get_leaf_key(args.leaf_key)
					bbx = armsreader.tree_data[leaf_key].bbx
					c0_ = np.linspace(bbx.r_min, bbx.r_max, args.resolution[0])
					c1_ = np.linspace(bbx.theta_min, bbx.theta_max, args.resolution[1])
					c2_ = np.linspace(bbx.phi_min, bbx.phi_max, args.resolution[2])
					c0, c1, c2 = np.meshgrid(c0_,c1_,c2_, indexing = 'ij')
					c0, c1, c2 = armsreader.convert_positions_from_ARMS((c0,c1,c2), args.input_coordinates)
				else:
					if args.verbose: print '-leaf option not specified. Cannot use leaf ranges.'
					exit()

			else:
				if args.c0_range:
					c0_ = np.linspace(args.c0_range[0], args.c0_range[1], args.resolution[0])
					if args.verbose: 
						print 'c0-range:', args.c0_range
						if args.verbose > 1:
							print '\tvalues:', c0_
				else:
					c0_ = np.zeros(1) + args.c0_intercept

				if args.c1_range:
					c1_ = np.linspace(args.c1_range[0], args.c1_range[1], args.resolution[1])
					if args.verbose: 
						print 'c1-range:', args.c1_range
						if args.verbose > 1:
							print '\tvalues:', c1_
				else:
					c1_ = np.zeros(1) + args.c1_intercept

				if args.c2_range:
					c2_ = np.linspace(args.c2_range[0], args.c2_range[1], args.resolution[2])
					if args.verbose: 
						print 'c2-range:', args.c2_range
						if args.verbose > 1:
							print '\tvalues:', c2_
				else:
					c2_ = np.zeros(1) + args.c2_intercept

				c0, c1, c2 = np.meshgrid(c0_,c1_,c2_, indexing = 'ij')

			"""ARMS uses spherical coordinates: natural log for r, theta in [-pi, pi], phi in [-pi to pi].
					You can pass your positions to the map function in ARMS or CART or SPHEXP"""

			t0 = time.clock()
			if (args.input_coordinates == 'ARMS') | (args.input_coordinates == 'CART') | (args.input_coordinates == 'SPHEXP'):
				if args.verbose: print 'input coordinates:', args.input_coordinates
				results = armsreader.map((c0,c1,c2), variables = args.variables, input_coordinates = args.input_coordinates)

				if args.plot_grid_values:
					plot_variable_values([c0,c1,c2], (3,1), 'grid positions', ['c0', 'c1', 'c2'])
					plot_variable_values(results, get_rows_columns(len(results)), 'grid values', args.variables)
			else:
				raise IOError('input coordinate system' + args.input_coordinates + 'not supported. Exiting')


			# B_x, B_y, B_z = readARMS.spherical_to_cartesian_field(rr, th, pp, 
			# 				variables.Magnetic_Field_R, 
			# 				variables.Magnetic_Field_T, 
			# 				variables.Magnetic_Field_P)

			if args.output_file:
				var_format, ljust, var_names = get_variable_format(armsreader, args, args.positions_out_flag)

				results_ = [r.ravel() for r in results]
				if args.positions_out_flag:
					if args.verbose: print 'including positions in output'
					results_ = [c0.ravel(), c1.ravel(), c2.ravel()] + results_
					variables_tuple = namedtuple('Variables', get_position_components(armsreader, args) + args.variables)
				else:
					if args.verbose: print 'exclusing positions from output'
					variables_tuple = namedtuple('Variables', args.variables)
 
				try:				
					results_ = variables_tuple(*results_)
				except:
					print 'variables tuple:', variables_tuple._fields
					raise


				if 'txt' in args.file_format:
					if args.verbose: 
						print 'writing ASCII to', args.output_file + '.txt'
					with open(args.output_file + '.txt', 'w') as f:
						f.write(var_names)
						f.write('\n')
						for variables in zip(*results_):
							try:
								f.write(var_format.format(*variables))
							except:
								print var_format
								print variables
								raise
							f.write('\n')
					f.close()

				if 'md' in args.file_format:
					if args.verbose: print 'writting ASCII markdown table to', args.output_file + '.md'
					args.delimiter = ' | '
					var_format, ljust, var_names = get_variable_format(armsreader, args, args.positions_out_flag, False)
					with open(args.output_file + '.md', 'w') as f:
						f.write(var_names)
						f.write('\n')
						header_names = var_names.split(args.delimiter)
						for i, var_name in enumerate(header_names):
							f.write('---' + args.delimiter*((i+1)!=len(header_names)))
						f.write('\n')
						for variables in zip(*results):
							f.write(var_format.format(*variables))
							f.write('\n')
					f.close()
					pass

				if 'json' in args.file_format:
					import json
					if args.verbose:
						print 'writing json to', args.output_file + '.json'
					
					with open(args.output_file + '.json', 'w') as f:
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
					nglobal = armsreader.getNumberOfGlobalAttributes()
					#global attributes
					for i in range(nglobal):
						attr_name = armsreader.getGlobalAttributeName(i)
						attr = armsreader.getGlobalAttribute(attr_name)
						primary_header[str(i)] = attr_name + ': ' + str(getAttributeValue(attr))
					#variable attributes
					for varname in args.variables:
						for j in range(armsreader.getNumberOfVariableAttributes()):
							attr_name = armsreader.getVariableAttributeName(nglobal+j)
							attr = armsreader.getVariableAttribute(varname, attr_name)
							primary_header[varname + str(j)] = attr_name + ': '	+ str(getAttributeValue(attr))		
			
					primary_hdu = fits.PrimaryHDU(header = primary_header)

					# generate columns
					columns = []
					for var_name, var_array in results._asdict().items():
						columns.append(fits.Column(name = var_name, format = 'E', array = var_array))
					cols = fits.ColDefs(columns)
					hdutable = fits.BinTableHDU.from_columns(cols)
					
					thdulist = fits.HDUList([primary_hdu,hdutable])
					thdulist.writeto(args.output_file + '.fits')

			elapsed = time.clock()-t0
			seconds_per_interpolation = elapsed/c0.size
			if args.verbose: 
				print '\tmap time:', elapsed, 'seconds'
				print '\tseconds per interpolation:', seconds_per_interpolation
			if args.visualize:
				#plot variables on user-defined grid slice
				arg_slices = args.slice_0, args.slice_1, args.slice_2
				plot_slice([c0,c1,c2], args.resolution, results, arg_slices, 
							args.contour_values, args.contour_levels, args.point, 
							title = 'Variables on Grid',
							cmaps = args.color_maps)
		else:
			if args.verbose: 
				print 'no grid resolution set for --resolution option'
			
		

		#render
		if args.visualize:
			plt.show()
		
	else:
		if args.verbose: print 'no variables listed'



def fmt(x, pos):
    a, b = '{:.2e}'.format(x).split('e')
    b = int(b)
    return r'$'+str(a)+r' \times 10^{'+str(b)+r'}$'

    # return r' \times 10^{{{}}}$'.format(a, b)	


def module_exists(module_name):
    try:
        __import__(module_name)
    except ImportError:
        return False
    else:
        return True

def get_position_components(kameleon, args, default_names = ['x','y','z']):
	if kameleon.doesAttributeExist('grid_system_1'):
		attr = kameleon.getGlobalAttribute('grid_system_1')
		component_names = getAttributeValue(attr)
		component_names = component_names.split('[')[1].split(']')[0].split(',')
		component_names = [component.replace("'", "").replace(" ", "") for component in component_names]
		if args.debug: print 'component names:', component_names
		return component_names
	else:
		return default_names

def get_variable_format(kameleon, args, positions_out_flag, justification = True):
	if justification:
		just = len(('{0:'+args.format+'}').format(0))
	else:
		just = 0
	var_format = ''
	var_names = ''
	if positions_out_flag:
		for i, pos_name in enumerate(get_position_components(kameleon, args)):
			var_names += pos_name.rjust(just)+args.delimiter
			var_format += '{' + str(i) + ':'+ args.format +'}' + args.delimiter
	for i,var_name in enumerate(args.variables):
		var_format += '{' + str(i+3*positions_out_flag) + ':'+ args.format +'}' + args.delimiter*((i+1)!=len(args.variables))
		vis_unit = kameleon.getVariableAttribute(var_name, 'units').getAttributeString()
		var_names += ('{0}[{1}]'.format(var_name, vis_unit)).rjust(just)+args.delimiter*((i+1)!=len(args.variables))
	if args.debug: 
		print 'variable names for output:', var_names
		print 'variable formats:', var_format
	return var_format, just, var_names

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

def getAttributeValue(attribute):
	if attribute.getAttributeType() == ccmc.Attribute.STRING:
		return attribute.getAttributeString()
	elif attribute.getAttributeType() == ccmc.Attribute.FLOAT:
		return attribute.getAttributeFloat()
	elif attribute.getAttributeType() == ccmc.Attribute.INT:
		return attribute.getAttributeInt()

if __name__ =="__main__":
    main(sys.argv[1:])
