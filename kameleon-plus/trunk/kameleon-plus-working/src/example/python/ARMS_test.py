import sys, time, argparse
sys.path.append('@CCMC_DIR@')
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/')
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import numpy as np
from pyreaders.ARMS import readARMS
import _CCMC as ccmc

def main(argv):
	# raise Exception("TODO: YOU NEED TO FIX THE SET_ROOT_RANGES FUNCTION SO THAT IT RETURNS THE MIN AND MAX ARRAYS FOR ALL ROOT BOUNDING BOXES")

	parser = argparse.ArgumentParser(description="Direct test of ARMs reader and interpolator")
	parser.add_argument("-v", "--verbose", action="count", default=0, help = 'verbosity of output')
	parser.add_argument("input_file", metavar = '/path/to/ARMS/config/file/ARMs.ini', type=str, 
		help="ARMs config file contains paths to ARMS header and flicks data")
	parser.add_argument("-ginfo","--global-info", action='store_true', help = 'print global attributes')
	parser.add_argument("-db", "--debug", action = 'store_true', help = 'debugging flag')

	arms_options = parser.add_argument_group(title = "ARMS-specific options", description = 'Options specific to ARMS code')
	arms_options.add_argument("-in_coord", "--input-coordinates", type = str, default = 'ARMS', metavar = '<coordinate type>',
		help = 'sets coordinates of input grid. Options: ARMS (default) or CART (cartesian) or SPHEXP (spherical exponential)')

	# requested variables
	var_options = parser.add_argument_group(title = 'variable options', description = 'List and interrogate variables. Not all variables will have interpolator support.')
	var_options.add_argument("-lvar", "--list-vars",action = 'store_true', help = 'list variables in the file (use -v to print all variable attributes)')
	var_options.add_argument("-vinfo","--variable-info", metavar = 'var', type = str, help = 'print attributes for given variable')
	var_options.add_argument("-vars", "--variables", type=str, nargs='+',metavar = ('var1','var2',), help='list of variables to be interpolated')
	var_options.add_argument("-pout", "--positions_out_flag", type = bool, metavar = 'positions_output_flag', default = True,
		help = 'pass interpolating positions to output')

	# single point interpolation
	point_options = parser.add_argument_group(title = 'point options', description = 'interpolation options for a single point')
	point_options.add_argument("-p","--point", type=float, nargs=3, metavar=("c0", "c1", "c2"), help = 'point at which to interpolate variables')

	# grid options
	grid_options = parser.add_argument_group(title = 'grid options', description = 'interpolation options for a grid of points in model')
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
	vis_options.add_argument("-levels", "--contour_levels", type = int, default = 10, metavar = 'number contours', help = 'number of contours to use in vis')
	
	args = parser.parse_args()

	# readARMS can be initialized with or without a config file
	armsreader = readARMS(args.input_file)
	if args.debug: armsreader.debug = True
	if args.verbose:
		print 'ARMS reader initialized with config file'
		print 'ARMS config file:', armsreader.header_file_name


	# must open with config file
	armsreader.open(args.input_file)

	if args.global_info:
		print "Global Attributes:"
		for i in range(armsreader.getNumberOfGlobalAttributes()):
			print 'retrieving attribute name'
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

	if args.variables:
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
				print 'interpolating at {0}:'.format(args.point)

			result = []
			if args.positions_out_flag:
				result = result + args.point
				if args.debug: print 'result', result
			for varname in args.variables:
				result_ = armsreader.interpolate(varname,c0, c1, c2)
				result.append(result_)
			print result

		# xx,zz = np.mgrid[1.003:1.06:50j,-.0125:0.0125:50j]
		# yy = np.zeros(xx.shape)	
		if args.resolution:
			if args.verbose: 
				print 'requested output resolution:', args.resolution
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

			# c0, c1, c2 = np.meshgrid(c0_,c1_,c2_, indexing = args.indexing)
			c0, c1, c2 = np.meshgrid(c0_,c1_,c2_, indexing = 'ij')

			"""Note: ARMS uses spherical coordinates: log10 for r, theta in [-pi, pi], phi in [-pi to pi].
					You can pass your positions to the map function in ARMS or CART or SPHEXP"""

			t0 = time.clock()
			if (args.input_coordinates == 'ARMS') | (args.input_coordinates == 'CART') | (args.input_coordinates == 'SPHEXP'):
				if args.verbose: print 'input coordinates:', args.input_coordinates
				variables = armsreader.map((c0,c1,c2), variables = args.variables, input_coordinates = args.input_coordinates)
			else:
				raise IOError('input coordinate system' + args.input_coordinates + 'not supported. Exiting')
				exit()


			# B_x, B_y, B_z = readARMS.spherical_to_cartesian_field(rr, th, pp, 
			# 				variables.Magnetic_Field_R, 
			# 				variables.Magnetic_Field_T, 
			# 				variables.Magnetic_Field_P)

			elapsed = time.clock()-t0
			seconds_per_interpolation = elapsed/c0.size
			if args.verbose: 
				print 'map time:', elapsed, 'seconds'
				print 'seconds per interpolation:', seconds_per_interpolation


			if args.visualize:
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

				input_coordinates = get_input_coordinates()

				if module_exists("matplotlib"):
					import matplotlib.pyplot as plt
					from collections import namedtuple
					import matplotlib.ticker as ticker
					import numpy.ma as ma

					missing_value = armsreader.getMissingValue()
					if args.verbose: print 'missing value is ', missing_value

					rows, columns = get_rows_columns(len(variables))
					PlotTuple = namedtuple('PlotTuple', ['x','y','var'])
					fig, axs = plt.subplots(rows,columns)
					axs = axs.ravel()

					slice_ = [None, None, None]
					#see if grid resolution has a dimension of size 1
					try:
						slice_dex = args.resolution.index(1)
					except ValueError:
						if args.verbose: print 'checking slices'
						if args.slice_0 != None:
							slice_dex = 0
							slice_[slice_dex] = args.slice_0
						elif args.slice_1 != None:
							slice_dex = 1
							slice_[slice_dex] = args.slice_1
						elif args.slice_2 != None:
							slice_dex = 2
							slice_[slice_dex] = args.slice_2
						else:
							print 'Must specify slice to visualize: -slice0 <slice#>'
							exit()

					x_label,y_label = [var for i, var in enumerate(input_coordinates) if i != slice_dex]
					plot_coordinates = [s[slice_].squeeze() for i, s in enumerate([c0,c1,c2]) if i != slice_dex]
					
		
					for i, var_name in enumerate(args.variables):
						# if var_name =='Mass_Density':
						# 	variable = np.log10(variables[i])
						# 	log_str = 'log10 '
						# else:
						# 	log_str = ''
						# 	variable = variables[i]
						variable = variables[i].squeeze()
						variable = ma.masked_values(variable,armsreader.getMissingValue())
						levels = np.linspace(variable.min(),variable.max(),args.contour_levels)
						plot_tuple = PlotTuple(*plot_coordinates,var=variables[i][slice_].squeeze())
						try:
							cs = axs[i].contourf(*plot_tuple, levels = levels, extend = 'both')
						except TypeError:
							print plot_tuple
							print slice_, slice_dex
							raise

						axs[i].set_title(var_name)
						axs[i].set_xlabel(x_label)
						axs[i].set_ylabel(y_label)
						cbar = fig.colorbar(cs, ax=axs[i], shrink=0.9, format=ticker.FuncFormatter(fmt))
					
						units = armsreader.getVariableAttribute(var_name, 'units').getAttributeString()
						cbar.ax.set_ylabel(var_name + ' [' + units +  ']')
					
					plt.show()

				else:
					print 'cannot visualize without matplotlib. please run\n\tpip install matplotlib'
				

				# visualize(armsreader, c0,c1,c2,variables)
			
		else:
			if args.verbose: print 'no grid resolution set'
	else:
		if args.verbose: print 'no variables listed'



def fmt(x, pos):
    a, b = '{:.2e}'.format(x).split('e')
    b = int(b)
    return r'$'+str(a)+r' \times 10^{'+str(b)+r'}$'

    # return r'${} \times 10^{{{}}}$'.format(a, b)	


def module_exists(module_name):
    try:
        __import__(module_name)
    except ImportError:
        return False
    else:
        return True
def visualize(armsreader, c0,c1,c2, variables):
	if module_exists("matplotlib"):
		import matplotlib.pyplot as plt

		contours = 40
		br_contours = np.linspace(-4.1,35.0,contours)
		bth_contours = np.linspace(-17,17,contours)
		rho_contours = np.linspace(.105, .210, contours)

		fig = plt.figure()

		ax0 = fig.add_subplot(221)
		ax0.contourf(c0,c2,variables.Magnetic_Field_R,levels = br_contours)
		armsreader.plot_visited_leaf_midpoints(ax0)
		plt.title("Magnetic_Field_R")

		ax1 = fig.add_subplot(222,sharex = ax0, sharey = ax0)
		ax1.contourf(c0,c2,variables.Mass_Density, levels = rho_contours)
		armsreader.plot_visited_leaf_midpoints(ax1)
		plt.title("Magnetic_Field_T")


		# plot leaf slice along phi directly (No interpolation)
		ax2 = fig.add_subplot(223,sharex = ax0, sharey = ax0)
		for leaf_key, (p_r,p_th,p_ph) in armsreader.visited.items():
			armsreader.plot_leaf_slice(ax2, leaf_key, 'Magnetic_Field_R', levels = br_contours, slice_obj=slice(-1,None), cartesian = True)
		armsreader.plot_visited_leaf_midpoints(ax2)
			
		ax3 = fig.add_subplot(224,sharex = ax0, sharey = ax0)
		for leaf_key, (p_r,p_th,p_ph) in armsreader.visited.items():
			armsreader.plot_leaf_slice(ax3, leaf_key, 'Mass_Density', levels = rho_contours, slice_obj=slice(-1,None), cartesian = True)
		armsreader.plot_visited_leaf_midpoints(ax3)

		fig2 = plt.figure()
		ax = fig2.add_subplot(111)
		cm = ax.contourf(c0,c2,variables.Mass_Density,levels = rho_contours)
		plt.colorbar(cm)
		# ax.streamplot(c0[:,0],c2[0,:],B_x.T,B_z.T, color='k', density = 1)

		plt.show()
	else:
		print 'please install matplotlib to visualize'



def getAttributeValue(attribute):
	if attribute.getAttributeType() == ccmc.Attribute.STRING:
		return attribute.getAttributeString()
	elif attribute.getAttributeType() == ccmc.Attribute.FLOAT:
		return attribute.getAttributeFloat()
	elif attribute.getAttributeType() == ccmc.Attribute.INT:
		return attribute.getAttributeInt()

if __name__ =="__main__":
    main(sys.argv[1:])