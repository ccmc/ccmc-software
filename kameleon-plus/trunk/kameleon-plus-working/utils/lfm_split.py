#!/opt/local/bin/python2.7
# import h5py # for hdf5 files
import sys
from pyhdf.SD import SD, SDC
import math
import numpy as np
import matplotlib.pyplot as plt

# http://www.hdfgroup.org/h4toh5/
# http://docs.h5py.org/en/latest/build.html#install

# HDF-4 files: 
# sudo python setup.py install -i /opt/local/include -l /opt/local/lib (assuming hdf4 is installed in /opt/local)
# Install: See http://cdx.jpl.nasa.gov/documents/technical-design/accessing-hdf-data-from-python-on-mac-os-x
# Usage: See http://hdfdap.hdfgroup.uiuc.edu/software/pyhdf.php

# The goal of this program is to 
# 1 - import an MLFM data file with concatonated timesteps,
# 2 - split it into multiple timesteps
# 3 - for each timestep, compute the grid corners
# 4 - save in hdf4 format


def main(argv):

	# Export a timestep(s)
	# load_standard_lfm_hdf('/Users/apembrok/Work/LTR-para-runs/LTR-2_2_0/IMFBy/Asher_Pembroke_112513_1_mhd_2000-01-01T07-07-00Z.hdf')
	export_multi_fluid_LFM(argv)

	#import and plot a timestep
	# filename = '/Users/apembrok/Work/Garcia/total/total_S-IMF-time_step_1640000.hdf'
	# sd = SD(filename, SDC.READ)
	# plot_slice(sd, 'p_', sliceIndex =10, slice_type= 'j')



def export_multi_fluid_LFM(argv):
	if (len(argv) >= 2):
		input_filename = argv[0]
		output_filename = argv[1]
		print input_filename
		sd = SD(input_filename, SDC.READ)

		grid = get_corners(sd)

		timesteps = 0

		# step = 1640000

		for key in sd.datasets().keys():
			shift = key.find('time_step')
			if shift == 0:
				if len(argv) == 3:
					step = argv[2]
					if key == 'time_step_'+str(step):
						export_timestep(sd, output_filename, key, grid)
				else:
					export_timestep(sd, output_filename, key, grid)
				timesteps += 1

		print 'timesteps found in file:', timesteps


	else:
		print 'usage: python lfm_split.py input_multi_timestep_hdf output_filename_prefix step(optional)'



"""Function computes grid corners by rotating x2,y2 along phi """
def get_corners(f):
	# corners have one extra step than the LFM cells

	# extract X,Y,PHI
	x2 = f.select('grid_x2')[:]
	y2 = f.select('grid_y2')[:]
	phi =f.select('grid_phi')[:]
	print 'phi datatype:', phi.dtype, 'phi min,max =', phi.min(), phi.max()

	dtype = phi.dtype


	(njp1,nip1) = x2[:].shape #column major
	nkp1 = phi[:].shape[0]
	# print 'nip1,njp1,nkp1=', nip1, njp1, nkp1
	print 'ni,nj,nk=', nip1-1, njp1-1, nkp1-1

	x = np.zeros([nkp1,njp1,nip1],dtype)
	y = np.zeros([nkp1,njp1,nip1],dtype)
	z = np.zeros([nkp1,njp1,nip1],dtype)

	R_e = 6.378e8 # radius of earth in cm

	x2_f = x2.flatten() # column major
	y2_f = y2.flatten()

	# for i in range(nip1):
	# 	print x2_f[i]/R_e

	x[:,:,:] = np.tile(x2_f,nkp1).reshape([nkp1,njp1,nip1]) #switch to row major

	y[:,:,:] = np.tile(y2_f,nkp1).reshape([nkp1,njp1,nip1]) #row major

	phi_tile = np.tile(phi,nip1*njp1).reshape([nip1,njp1,nkp1]).T

	z = y*np.sin(phi_tile)
	y = y*np.cos(phi_tile)

	

	return x,y,z 


"""function calculates positions and saves to new datafile """
def export_timestep(sd_in, filename, timestep, (x,y,z)):

	shape_np1 = (nkp1,njp1,nip1) = x.shape
	(nk,nj,ni) = (shape_np1[0]-1, shape_np1[1]-1,shape_np1[2]-1) 

	print 'exporting timestep', timestep, 'broadcasting to shape', shape_np1
	total = sd_in.select(timestep)
	resolution = (ni,nj,nk)
	rho = get_variable(resolution, total[:], 0) #hydrogen? [gm/cc]
	vx = get_variable(resolution, total[:],1,)
	vy = get_variable(resolution, total[:],2,)
	vz = get_variable(resolution, total[:],3,)
	c_s = get_variable(resolution, total[:],4,)
	ei = get_variable(resolution, variable_array = sd_in.select('ei_'+timestep)[:], variable_resolution = (ni,njp1,nkp1))
	ej = get_variable(resolution, variable_array = sd_in.select('ej_'+timestep)[:], variable_resolution = (nip1,nj,nkp1))
	ek = get_variable(resolution, variable_array = sd_in.select('ek_'+timestep)[:], variable_resolution = (nip1,njp1,nk))

	pressure = rho*c_s**2

	# create an HDF file
	sd_out = SD(filename + timestep+'.hdf', SDC.WRITE| SDC.CREATE)

	bx, by, bz = get_bfield(sd_in, timestep, resolution)

	dataType = total.info()[3]
	add_variable(sd_out, rho, "rho_", dataType, shape_np1, total.attributes(True)) #2nd index varies fastest
	add_variable(sd_out, vx, "vx_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, vy, "vy_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, vz, "vz_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, c_s, "c_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, bx, "bx_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, by, "by_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, bz, "bz_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, ei, "ei_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, ej, "ej_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, ek, "ek_", dataType, shape_np1, total.attributes(True))
	add_variable(sd_out, x, "X_grid", dataType, x.shape, total.attributes(True))
	add_variable(sd_out, y, "Y_grid", dataType, x.shape, total.attributes(True))
	add_variable(sd_out, z, "Z_grid", dataType, x.shape, total.attributes(True))
	add_variable(sd_out, pressure, "p_", dataType, shape_np1, total.attributes(True))

	sd_out.end()

def get_variable(resolution, total_array = None, variable_index = None, variable_array = None, variable_resolution = None):
	if variable_resolution == None:
		variable_resolution = resolution

	(ni,nj,nk) = resolution
	(var_ni,var_nj,var_nk) = variable_resolution

	if total_array != None:
		data = np.zeros([nk+1,nj+1,ni+1],total_array.dtype) #row major
		data[:var_nk,:var_nj,:var_ni] = total_array[variable_index,:,:,:]
	elif variable_array != None:
		data = np.zeros([nk+1,nj+1,ni+1],variable_array.dtype) #row major
		data[:var_nk,:var_nj,:var_ni] = variable_array
	return data
	

"""Adds variable to open file """
def add_variable(sd, data, name, data_type, shape, attributes = {}, fillvalue = 0, verbose = False):
	if verbose:
		print 'adding variable ', name, 'min,max,avg:', data.min(),data.max(), data.mean()
	# Create a dataset
	sds = sd.create(name, data_type, shape)
	sds.setfillvalue(fillvalue)

	# #set dimension names
	# dim1 = sds.dim(0)
	# dim1.setname("row")
	# dim2 = sds.dim(1)
	# dim2.setname("column")

	#assign attributes
	for attr_key in attributes.keys():
		(value, index, attr_type, length) = attributes[attr_key]
		attribute = sds.attr(attr_key)
		attribute.set(attr_type,value) 

	#Write data
	sds[:] = data

	# close the dataset
	sds.endaccess()

def load_standard_lfm_hdf(filename):
	""" Load the standard formated hdf which we want to emulate"""
	f = SD(filename, SDC.READ)
	X_grid = f.select('X_grid')
	Y_grid = f.select('Y_grid')
	Z_grid = f.select('Z_grid')

	# x_grid is size nkp1,njp1,nip1
	(nkp1,njp1,nip1) = X_grid[:].shape
	# The LFM reader expects i to vary fastest, then j, then k
	# However, the LFM pre-converted files store positions with k varying fastest (column-major)
	# Recommend saving in column-major format. If it fails, we can always switch.

	
	# i = 0; j = 0; k = 0
	# print 'printing standard first row'
	# for i in range(nip1):
	# 	print X_grid[k,j,i]/R_e

	# print 'printing j sweep'
	# i = 0; j = 0; k = 0;
	# for j in range(njp1):
	# 	print X_grid[k,j,i]/R_e

	# print 'printing k sweep'
	# i = 0; j = 0; k = 0;
	# for k in range(nkp1):
	# 	print X_grid[k,j,i]/R_e


	print 'standard nip1,njp1,nkp1 =', nip1,njp1,nkp1
	ni = nip1-1
	nj = njp1-1
	nk = nkp1-1
	print 'standard ni,nj,nk =', ni,nj,nk

"""Calculate bfield from fluxes """
def get_bfield(sd, timestep, resolution):
	(ni,nj,nk) = resolution
	(nip1, njp1, nkp1) = (ni+1, nj+1,nk+1)

	# shape_np1 = (nkp1,njp1,nip1) = x.shape
	# (nk,nj,ni) = (shape_np1[0]-1, shape_np1[1]-1,shape_np1[2]-1) 

	# 0th-order magnetic field
	bzeros = sd.select('bzeros')[:]
	print 'bzeros shape:', bzeros.shape

	total = sd.select(timestep)

	resolution = (ni, nj, nk) = (nip1-1, njp1-1, nkp1-1)
	bx_1 = total[:][5,:,:,:].flatten() #the perturbation field
	by_1 = total[:][6,:,:,:].flatten()
	bz_1 = total[:][7,:,:,:].flatten()

	print 'size of perturbation field:', bx_1.shape, by_1.shape, bz_1.shape

	def vijk1(v,i,j,k):
		return i+ni*(j+nj*(k+nk*v))

	def vijk(i,j,k): 
		return i+ni*(j+nj*k)


	def ijk(i,j,k): 
		return i+ni*(j+nj*k)

	fp_bnqfi = 0 #points to start of bzeros  

	fp_bnqfk = fp_bnqfi + 8*nip1*nj*nk +3*nip1*njp1*nk + 8*ni*njp1*nk + 3*ni*njp1*nkp1;
	fp_bsqqk = fp_bnqfk + ni*nj*nkp1;
	fp_bzqnk = fp_bsqqk + ni*nj*nkp1;
	fp_bxqnk = fp_bzqnk + ni*nj*nkp1;
	fp_byqnk = fp_bxqnk + ni*nj*nkp1;
	fp_bzqfk = fp_byqnk + ni*nj*nkp1;

	fpBXqnK = fp_bxqnk # an integer offset into bzeros array
	fpBYqnK = fp_byqnk;
	fpBZqnK = fp_bzqfk;

	bx = np.zeros([ni*nj*nk]).flatten() 
	by = np.zeros([ni*nj*nk]).flatten() 
	bz = np.zeros([ni*nj*nk]).flatten() 

	for k in range(nk): 
	    for j in range(nj):
	      for i in range(ni): 
			bx0_k = bzeros[fpBXqnK+ijk(i,j,k)]
			bx0_kp1 = bzeros[fpBXqnK+ijk(i,j,k+1)] 
			bx[vijk(i,j,k)] =(bx_1[vijk1(0,i,j,k)] + (bx0_k+bx0_kp1)*0.5) #gauss

			by0_k = bzeros[fpBYqnK+ijk(i,j,k)]
			by0_kp1 = bzeros[fpBYqnK+ijk(i,j,k+1)] 
			by[vijk(i,j,k)] =(by_1[vijk1(0,i,j,k)] + (by0_k+by0_kp1)*0.5)

			bz0_k = bzeros[fpBZqnK+ijk(i,j,k)]
			bz0_kp1 = bzeros[fpBZqnK+ijk(i,j,k+1)] 
			bz[vijk(i,j,k)] =(bz_1[vijk1(0,i,j,k)] + (bz0_k+bz0_kp1)*0.5)

	#field needs to be put into arrays of size nip1,njp1,nkp1
	bx_out = np.zeros([nk+1,nj+1,ni+1],total[:].dtype)
	bx_out[:nk,:nj,:ni] = bx.reshape([nk,nj,ni])
	by_out = np.zeros([nk+1,nj+1,ni+1],total[:].dtype)
	by_out[:nk,:nj,:ni] = by.reshape([nk,nj,ni])
	bz_out = np.zeros([nk+1,nj+1,ni+1],total[:].dtype)
	bz_out[:nk,:nj,:ni] = bz.reshape([nk,nj,ni])
	
	return bx_out, by_out, bz_out

""" plots slice of lfm along given axis"""
def plot_slice(sd, variable, sliceIndex = 0, slice_type = 'k', norm = 6.378e8):
	from mpl_toolkits.mplot3d import Axes3D
	import matplotlib.pyplot as plt
	import numpy as np

	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')

	# norm = 6.378e8 # radius of earth in cm
	# get first slice
	if slice_type == 'k':
		x = sd.select('X_grid')[sliceIndex,:,:]/norm	
		y = sd.select('Y_grid')[sliceIndex,:,:]/norm
		z = sd.select('Z_grid')[sliceIndex,:,:]/norm
		data = sd.select(variable)[sliceIndex,:,:]
		ax.plot_surface(x, y, z, color='b', cstride=5)
		# plt.contourf(x,y,data)
	elif slice_type == 'j':
		x = sd.select('X_grid')[:,sliceIndex,:]/norm	
		y = sd.select('Y_grid')[:,sliceIndex,:]/norm	
		z = sd.select('Z_grid')[:,sliceIndex,:]/norm
		data = sd.select(variable)[:,sliceIndex,:]
		ax.plot_surface(x, y, z, color='b', cstride=5)
		# plt.contourf(y,z,data)
	elif slice_type =='i':
		x = sd.select('X_grid')[:,:,sliceIndex]/norm	
		y = sd.select('Y_grid')[:,:,sliceIndex]/norm
		z = sd.select('Z_grid')[:,:,sliceIndex]/norm
		data = sd.select(variable)[:,:,sliceIndex]
		ax.plot_surface(x, y, z, color='b', cstride=5)
		# plt.contourf(y,z,data)

	
	# plt.colorbar()
	plt.title(variable)
	plt.show()




if __name__ =="__main__":
    main(sys.argv[1:])

