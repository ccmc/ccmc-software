import sys, time
sys.path.append('../../../lib/ccmc/')
sys.path.append('../../ccmc/')
sys.path.append('.')



def main(argv):
	if len(sys.argv) != 2:
		raise Exception("Need to specify config file:\n\tpython ARMS.py /path/to/ARMS/config/file/<config.ini>")

	if module_exists("numpy"):
		import numpy as np
		from pyreaders.ARMS import readARMS
	else:
		"need numpy to use ARMS"

	filename = sys.argv[1]
	print filename
	armsreader = readARMS(filename)
	print 'ARMS reader initialized'
	armsreader.open(filename)

	# armsreader._print_tree_info()

	
	xx,zz = np.mgrid[1.003:1.06:50j,-.0125:0.0125:50j]
	yy = np.zeros(xx.shape)	

	"""Note: ARMS uses spherical coordinates: log10 for r, theta in [-pi, pi], phi in [-pi to pi].
			You can pass your positions to the map function in ARMS or Cartesian"""

	t0 = time.clock()
	rr,th,pp = readARMS.cartesian_to_ARMS(xx,yy,zz)
	variables = armsreader.map((rr,th,pp), input_coordinates = 'ARMS')

	B_x, B_y, B_z = readARMS.spherical_to_cartesian_field(rr, th, pp, 
					variables.Magnetic_Field_R, 
					variables.Magnetic_Field_T, 
					variables.Magnetic_Field_P)

	elapsed = time.clock()-t0
	seconds_per_interpolation = elapsed/xx.size
	print 'map time:', elapsed, 'seconds'
	print 'seconds per interpolation:', seconds_per_interpolation


	if module_exists("matplotlib"):
		import matplotlib.pyplot as plt

		contours = 40
		br_contours = np.linspace(-4.1,35.0,contours)
		bth_contours = np.linspace(-17,17,contours)
		rho_contours = np.linspace(.105, .210, contours)

		fig = plt.figure()

		ax0 = fig.add_subplot(221)
		ax0.contourf(xx,zz,variables.Magnetic_Field_R,levels = br_contours)
		armsreader.plot_visited_leaf_midpoints(ax0)
		plt.title("Magnetic_Field_R")

		ax1 = fig.add_subplot(222,sharex = ax0, sharey = ax0)
		ax1.contourf(xx,zz,variables.Mass_Density, levels = rho_contours)
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
		cm = ax.contourf(xx,zz,variables.Mass_Density,levels = rho_contours)
		plt.colorbar(cm)
		ax.streamplot(xx[:,0],zz[0,:],B_x.T,B_z.T, color='k', density = 1)

		plt.show()


	

def module_exists(module_name):
    try:
        __import__(module_name)
    except ImportError:
        return False
    else:
        return True


if __name__ =="__main__":
    main(sys.argv[1:])