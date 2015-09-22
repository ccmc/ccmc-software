import sys, time, argparse
sys.path.append('@CCMC_DIR@')
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/')
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import numpy as np
from pyreaders.ARMS import readARMS
import _CCMC as ccmc

def main(argv):

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

	output_options = parser.add_argument_group(title = 'output options', description = 'saving and visualization')
	output_options.add_argument("-vis", "--visualize", type = bool, metavar = 'visualization_flag', default = False, help = 'visualize with matplotlib')

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
			print 'point input not implemented'
		else:
			if args.verbose: print 'no point specified'

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
				variables = armsreader.map((c0.ravel(),c1.ravel(),c2.ravel()), variables = args.variables, input_coordinates = args.input_coordinates)
			else:
				raise IOError('input coordinate system' + args.input_coordinates + 'not supported. Exiting')
				exit()

			# rr,th,pp = readARMS.cartesian_to_spherical_exponential(xx,yy,zz)
			# variables = armsreader.map((rr,th,pp), input_coordinates = 'ARMS')

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
				visualize(c0,c1,c2,variables)
			else:
				print variables
		else:
			if args.verbose: print 'no grid resolution set'
	else:
		if args.verbose: print 'no variables listed'
	

def module_exists(module_name):
    try:
        __import__(module_name)
    except ImportError:
        return False
    else:
        return True
def visualize(c0,c1,c2, variables):
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