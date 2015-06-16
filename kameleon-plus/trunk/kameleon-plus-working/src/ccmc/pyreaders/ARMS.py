#1/usr/bin/python
import unittest
import sys, os
sys.path.append('../../../lib/ccmc/')
sys.path.append('.')
# sys.path.append('../')
import pyKameleon, testReader
from Attribute import Attribute
# import random #only used for testing
import ConfigParser
# from scipy.io import FortranFile
# from fortranfile import FortranFile as FF
import numpy as np
from collections import defaultdict
from operator import itemgetter, attrgetter
import collections
import matplotlib.pyplot as plt
import math


"""Reader for the Adaptively Refined MHD Solver (ARMS) """
class readARMS(testReader.pyFileReader):
	_bbx_tuple = collections.namedtuple('boundingBox', 'r_min r_max theta_min theta_max phi_min phi_max')
	_tree_data_tuple = collections.namedtuple('block_info','block_type parent_key child_key bbx')

	def __init__(self, config_file = None):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)

		if config_file != None:
			self._config = testReader.getConfig(config_file)
			self.set_file_names()

		self.tree_data = {}
		self.leaf_data = {}
		self.roots = [] # sorted by R,T,P bounding box minima
		self.last_key = None
		self.visited = {}
		self.missing_value = -256.*-256.*-256.*-256.*-256.
		self.leaf_iterations = 0


	def read_ARMS_header(self, header_filename):
		"""Read gloabl attributes from file """
		self.header_file = open(header_filename, 'r')

		# get time step
		line = self.header_file.readline().split()
		time = line[0]

		#can get date from config
		date = str(testReader.get_config_value(self._config,'MetaData', 'Date'))
		self.globalAttributes['timestep_time'] = Attribute('timestep_time', date+'T'+time+'.000Z')

		# get grid type
		line = self.header_file.readline().split()
		self.grid_type = line[0]
		self.globalAttributes['grid_type'] = Attribute('grid_type', self.grid_type)

		if self.grid_type == 'Spherical_Exponential':
			components = ['R', 'T', 'P']
		else:
			print self.grid_type, 'not supported!'
			raise Exception("grid type not supported!")
			components = ['x', 'y', 'z'] # just guessing here...

		# get Block sizes
		for i in range(3):
			dim_size, dim_name = self.header_file.readline().split()
			self.globalAttributes[dim_name] = Attribute(dim_name, int(dim_size))


		#initialize variables
		var_num = 0
		
		line = ' '
		while len(line) > 0:
			line = self.header_file.readline().split()
			if len(line) == 0:
				break
			else:
				number_components, var_base_name = line
			
			if int(number_components) == 1:
				self.addVariableName(var_base_name, var_num)

				# get variable units
				val, attr = self.header_file.readline().split() 
				self.variableAttributes[var_base_name]['units'] = Attribute('units', val)

				# get variable scale factor
				val, attr = self.header_file.readline().split()
				self.variableAttributes[var_base_name]['scale_factor'] = Attribute('scale_factor', float(val))

				# get min and max values (what are the valid_min valid_max values???)
				val, attr = self.header_file.readline().split()
				self.variableAttributes[var_base_name]['actual_min'] = Attribute('actual_min', float(val))

				val, attr = self.header_file.readline().split()
				self.variableAttributes[var_base_name]['actual_max'] = Attribute('actual_max', float(val))

				var_num += 1
			else:
				# get variable units
				units, attr = self.header_file.readline().split() 

				# get variable scale factor
				scale_factor, attr = self.header_file.readline().split()


				for i in range(int(number_components)+1):
					if i == int(number_components):
						val, attr = self.header_file.readline().split()
						self.globalAttributes[var_base_name+'_mag_min'] = Attribute(var_base_name+'_mag_min', float(val))

						val, attr = self.header_file.readline().split()
						self.globalAttributes[var_base_name+'_mag_max'] = Attribute(var_base_name+'_mag_max', float(val))

						
					else:
						var_name = var_base_name + '_' + components[i]

						self.addVariableName(var_name, var_num)
						self.variableAttributes[var_name]['units'] = Attribute('units', units)
						self.variableAttributes[var_name]['scale_factor'] = Attribute('scale_factor', float(scale_factor))

						# get min and max values (what are the valid_min valid_max values???)
						val, attr = self.header_file.readline().split()
						self.variableAttributes[var_name]['actual_min'] = Attribute('actual_min', float(val))

						val, attr = self.header_file.readline().split()
						self.variableAttributes[var_name]['actual_max'] = Attribute('actual_max', float(val))

						var_num += 1


	def read_ARMS_data(self, filename):
		"""Reads in the arms data files """
		# print 'read_arms_data', filename
		self.data_file = file(filename)
		s = self.data_file.read()

		endian = '<'
		offset = [0]
		header_dtype = np.dtype(endian+'u4')

		model_name_length = np.frombuffer(s, dtype=header_dtype, offset = offset[0], count=1)[0]
		if model_name_length > 100: #header length too long, switch endian
			endian = '>'
			header_dtype = np.dtype(endian+'u4')
			model_name_length = np.frombuffer(s, dtype=endian+'u4', offset = offset[0], count=1)[0]


		def read_record(s, dtype, count=1, offset=None):
			"""offset is a mutable list"""
			# print 'input offset', offset[0]
			if offset == None:
				offset = [0]
			else:
				# print 'rec length:', np.frombuffer(s, dtype = header_dtype, count =1, offset =offset[0])[0], 'bytes'
				offset[0] += header_dtype.itemsize #record header
				result = np.frombuffer(s, dtype=dtype, count=count, offset=offset[0])
				offset[0] += result.itemsize*count + header_dtype.itemsize #footer
			return result

		#set model name
		python_model_name = read_record(s,'S'+str(model_name_length), offset=offset)[0]
		self.globalAttributes['python_model_name'] = Attribute('python_model_name', str(python_model_name))


		#set model time
		model_time = read_record(s, endian+'f1', offset = offset)[0]
		self.globalAttributes['sim_time'] = Attribute('sim_time', float(model_time))

		# get number of total blocks, leaf blocks, and new_grid_flag
		
		num_total_blocks, num_leaf_blocks, new_grid_flag = read_record(s, endian+'i4', 3, offset = offset)
		
		# print 'total blocks, leaf blocks, new grid:', num_total_blocks, num_leaf_blocks, new_grid_flag

		self.globalAttributes['num_total_blocks'] = Attribute('num_total_blocks', int(num_total_blocks))
		self.globalAttributes['num_leaf_blocks'] = Attribute('num_leaf_blocks', int(num_leaf_blocks))
		self.globalAttributes['new_grid_flag'] = Attribute('new_grid_flag', int(new_grid_flag))

		ni = self.getGlobalAttribute('RBlockSize').getAttributeValue()
		nj = self.getGlobalAttribute('TBlockSize').getAttributeValue()
		nk = self.getGlobalAttribute('PBlockSize').getAttributeValue()
		self.leaf_resolution = (ni,nj,nk)


		def create_block_datatype():
			dtype_list = []
			dtype_list.append(('tree_header', endian+'i4'))
			dtype_list.append(('block_loc', endian +'2i4'))
			dtype_list.append(('block_type', endian +'i4'))
			dtype_list.append(('parent_loc', endian +'2i4'))
			dtype_list.append(('child_loc', endian +'2,8i4')) # 8 (block#, processor#) pairs 
			dtype_list.append(('tree_footer', endian+'i4'))
			dtype_list.append(('bndbox_header', endian+'i4'))
			dtype_list.append(('bndbox', endian+'6f4'))
			dtype_list.append(('bndbox_footer', endian+'i4'))
			return np.dtype(dtype_list)

		block_dtype = create_block_datatype()

		# variable_names = ['Mass_density','Velocity_R','Velocity_T','Velocity_P','Magnetic_Field_R','Magnetic_Field_T','Magnetic_Field_P']

		def create_variable_datatype(self):
			"""creates a custom datatype to view variable data"""
			# note: this assumes order of variables in header matches data file, s.t. variableNames was initialized in the proper order.
			variable_names = self.variableNames.values()
			dtype_list = [('header', endian+'i4')]
			for var_name in variable_names: dtype_list.append((var_name, endian + 'f4'))
			dtype_list.append(('footer', endian+'i4'))
			return np.dtype(dtype_list)
		
		variable_datatype = create_variable_datatype(self)
		

		for block_number in range(num_total_blocks): #num_total_blocks
			block_data = np.frombuffer(s, dtype = block_dtype, count=1, offset = offset[0])
			offset[0] += block_dtype.itemsize

			block_key = tuple(block_data['block_loc'].flatten())

			block_type = block_data['block_type']
			parent_key = tuple(block_data['parent_loc'].flatten())
			child_key = tuple(block_data['child_loc'].flatten())
			bndbox = list(block_data['bndbox'][0].flatten())

			if self.grid_type == 'Spherical_Exponential':
				bndbox[2:4] = bndbox[3],bndbox[2]
			
			
			self.tree_data[block_key] = self._tree_data_tuple(block_type, parent_key, child_key, self._bbx_tuple(*bndbox))

			# 1 = leaf, 2 = parent, 3 = grand-parent, etc
			if block_type == 1: #::-1 flips second index
				self.leaf_data[block_key] = np.frombuffer(s, dtype=variable_datatype, count = ni*nj*nk, offset = offset[0]).reshape((ni,nj,nk))[:,::-1,:]
				offset[0] += variable_datatype.itemsize*ni*nj*nk
			
			if parent_key == (-1, -1):
				self.roots.append( (block_key,bndbox) )


		self.data_file.close()

		self.sort_roots()
		self.set_root_ranges()

	def sort_roots(self):
		# print 'number of roots:', len(self.roots)
		self.roots.sort(key= lambda x: itemgetter(0,2,4)(x[1]))

	def _print_tree_info(self):
		for key,attr in self.globalAttributes.items():
			print key, attr.getAttributeValue()


	def set_root_ranges(self):
		def get_root_range(self, min_getter, max_getter, stride = 1):
			get_root_bbx = itemgetter(1)
			root_range = [min_getter(get_root_bbx(self.roots[0]))] #initialize range
			for root_id in range(stride,len(self.roots),stride):
				root_bbx = get_root_bbx(self.roots[root_id])
				root_range.append(min_getter(root_bbx))
				if root_range[-1] < root_range[-2]:
					root_range.pop()
					root_range.append(max_getter(get_root_bbx(self.roots[root_id-1]))) #add max
					break
			return root_range

		get_r_min, get_r_max = itemgetter(0), itemgetter(1)
		get_theta_min, get_theta_max = itemgetter(2), itemgetter(3)
		get_phi_min, get_phi_max = itemgetter(4), itemgetter(5)

		phi = get_root_range(self,get_phi_min, get_phi_max)
		nk = len(phi)-1
		
		theta = get_root_range(self,get_theta_min,get_theta_max,nk)
		nj = len(theta)-1

		r = get_root_range(self,get_r_min,get_r_max,nj*nk)
		ni = len(r)-1

		self.root_coord = (r,theta,phi)
		self.root_resolution = (ni,nj,nk)

	def plot_root_coord(self, ax = None, index = 2):
		from mpl_toolkits.mplot3d import axes3d
		import matplotlib.pyplot as plt
		import numpy as np
		# 
		r, theta, phi = self.root_coord

		if ax == None:
			fig = plt.figure()
			ax = fig.add_subplot(111)

		if index == 2:
			rr,pp = np.meshgrid(r,phi, sparse=True)
			xx = np.exp(rr)*np.cos(pp)
			yy = np.exp(rr)*np.sin(pp)
			zz = 0*xx

		ax.plot_wireframe(xx,yy,zz)
		plt.show()
		return ax

	def find_leaf(self,point,start_key=None):
		"""gets the cell for the query point"""
		self.leaf_iterations += 1
		# print '\tfind_leaf called with key', start_key, 'looking for', point

		get_type = itemgetter(0)
		get_parent = itemgetter(1)
		get_children = itemgetter(2)
		get_bbx =itemgetter(3)

		r_min, r_max, r_range = itemgetter(0), itemgetter(1), itemgetter(0,1)
		theta_min, theta_max, theta_range = itemgetter(2), itemgetter(3), itemgetter(2,3)
		phi_min, phi_max, phi_range = itemgetter(4), itemgetter(5), itemgetter(4,5)
		min_range, max_range = itemgetter(0,2,4),itemgetter(1,3,5)
		r, theta, phi = itemgetter(0), itemgetter(1), itemgetter(2)

		if start_key == None:
			root_key = self.find_root(point)
			if root_key != -1:
				start_key = root_key
				block = self.tree_data[start_key]
				if self.in_block(block,point):
					pass
					# print self.leaf_iterations, 'found point among roots', point, get_bbx(block)
				else:
					raise ArithmeticError("Point not actually in root!")
			else: #out of simulation domain
				return -1
		else:
			# see if point is in this block's range
			block = self.tree_data[start_key]
			if self.in_block(block,point):
				# print self.leaf_iterations, 'point was in start_key block', point, get_bbx(block)
				pass
			else:
				parent_key = get_parent(block)
				if parent_key == (-1,-1):
					# print '\t\tlooking among roots'
					root_key = self.find_root(point)
					if root_key != -1:
						start_key = root_key
						block = self.tree_data[start_key]
						if self.in_block(block,point):
							pass
						else:
							raise Exception('point not in found root!')
					else: #out of simulation bounds
						# print '\t\tout of bounds!'
						return -1
				else:
					# print '\t\tlooking at parent'
					return self.find_leaf(point,parent_key)

		
		if get_type(block) == 1: # found leaf - block has no children
			# print self.leaf_iterations, 'found point in leaf', start_key
			return start_key
		else:
			if self.in_block(block,point):
				# print self.leaf_iterations, '\tpoint among children', point, get_bbx(block)
				pass
			else:
				raise AssertionError("point is not in block, something wrong")

			# print '\t\tlooking at children',
			children = get_children(block)

			#normalization
			point_norm = self.point_norm(point, get_bbx(block))
			# print self.leaf_iterations,'\tpoint_norm:', point_norm 
			if np.less(point_norm,0).any() | np.greater(point_norm,1).any():
				raise ArithmeticError("normalized point is not in [0,1]!")

			r_index = int(r(point_norm) > .5)#.5
			theta_index = int(theta(point_norm) <= .5)
			phi_index = int(phi(point_norm) > .5)

			# the child's index will be in [0,7]
			child_index = r_index+ 2*theta_index + 4*phi_index
			child_key = (children[2*child_index], children[2*child_index+1])

			# numpy interpolate will adjust the point slightly so that it lies in the child box	 		
			child_bbx = get_bbx(self.tree_data[child_key])
			child_r = self.x_shift(r(point), *r_range(child_bbx))
			child_theta = self.x_shift(theta(point),*theta_range(child_bbx))
			child_phi = self.x_shift(phi(point),*phi_range(child_bbx))
			child_point = child_r,child_theta,child_phi

			return self.find_leaf(child_point, child_key)
	
	def x_shift(self, x, xmin, xmax):
		if x < xmin:
			return xmin
		elif x > xmax:
			return xmax
		else:
			return x

	def point_norm(self,point,bbx):
		"""shifts the point and normalizes to bounding box"""
		r_min, r_max = itemgetter(0), itemgetter(1)
		theta_min, theta_max = itemgetter(2), itemgetter(3)
		phi_min, phi_max = itemgetter(4), itemgetter(5)
		r, theta, phi = itemgetter(0), itemgetter(1), itemgetter(2)

		r_norm = (r(point)-r_min(bbx))/(r_max(bbx)-r_min(bbx))
		theta_norm = (theta(point)-theta_min(bbx))/(theta_max(bbx)-theta_min(bbx))
		phi_norm = (phi(point)-phi_min(bbx))/(phi_max(bbx)- phi_min(bbx))
		return r_norm, theta_norm, phi_norm

	def print_child_order(self):
		print """min ranges for child blocks are in this order:\n\
			child 0: 0.0 0.5 0.0         (0,1,1)______________ (1,1,1) \n\
			child 1: 0.5 0.5 0.0              / |___4__|___5__|\n\
			child 2: 0.0 0.0 0.0             / /|______|______|\n\
			child 3: 0.5 0.0 0.0    (0,0,1) /|//   0  /  1   /\n\
			child 4: 0.0 0.5 0.5           |/|/______/______/ (1,.5,0)\n\
			child 5: 0.5 0.5 0.5           |//   2  /  3   /\n\
			child 6: 0.0 0.0 0.5    (0,0,0)|/______/______/(1,0,0)\n\
			child 7: 0.5 0.0 0.5     \n\
		"""
		pass

	def in_block(self,block,point):
		#checks if point in (min_ranges, max_ranges]
		get_bndbox = itemgetter(3)
		r, theta, phi = itemgetter(0), itemgetter(1), itemgetter(2)
		r_min, r_max, theta_min, theta_max,phi_min,phi_max = (itemgetter(0), itemgetter(1),
															itemgetter(2), itemgetter(3),
															itemgetter(4),itemgetter(5))
		bndbox = get_bndbox(block)
		inside = (r_min(bndbox) < r(point) <= r_max(bndbox)) and\
				(theta_min(bndbox) < theta(point) <= theta_max(bndbox)) and\
				(phi_min(bndbox) < phi(point) <= phi_max(bndbox))
		return inside

	def find_root(self,point):
		#finds a point in (min_range, max_range]
		get_root_key = itemgetter(0)
		r,theta,phi = itemgetter(0), itemgetter(1), itemgetter(2)
		r_root, theta_root, phi_root = self.root_coord
		ni,nj,nk = self.root_resolution
		i = np.searchsorted(r_root,r(point))-1
		j = np.searchsorted(theta_root,theta(point))-1
		k = np.searchsorted(phi_root,phi(point))-1
		if (i == -1) | (i == len(r_root)-1) | (j == -1) | (j == len(theta_root)-1) | (k == -1) | (k == len(phi_root)-1):
			return -1
		else:
			root_id = i*nj*nk+j*nk+k
			root_key = get_root_key(self.roots[root_id])
			return root_key

	def bbx_mid(self,bbx):
		r_mid = (bbx.r_min + bbx.r_max)/2
		th_mid = (bbx.theta_min + bbx.theta_max)/2
		phi_mid = (bbx.phi_min + bbx.phi_max)/2
		return r_mid,th_mid,phi_mid

	def interpolate(self,variable, point):
		if type(variable) != str:
			variable = self.variableNames[variable]
			
		"""interpolates data at point r,theta,phi

		Note: r may be stored in log space. If so, it is up to the caller to make the conversion

			Vxyz = 	V000 (1 - x) (1 - y) (1 - z) +	//    	   Left:    		ARMS needs:		
					V100 x (1 - y) (1 - z) +		//   V011 6----7 V111     V001 4----5 V101
					V010 (1 - x) y (1 - z) +		//    	 /|   /|              /|   /|
					V001 (1 - x) (1 - y) z +		// V001 4----5 V101     V011 6----7 V111 
					V101 x (1 - y) z +				//      | |  | |             | |  | |
					V011 (1 - x) y z +				//   V010 2--|-3 V110     V000 0--|-1 V100
					V110 x y (1 - z) +				//    	|/   |/              |/   |/
					V111 x y z 						// V000 0----1 V100     V010 2----3 V110

			r_index = int(r(point_norm) > .5) 
					= int(r(point_norm)*(N-1)) for N = 3
			theta_index = int(theta(point_norm) <= .5) 
						= int(1-theta(point_norm)*(N-1)) for N = 3 
			phi_index = int(phi(point_norm) > .5)

			# the child's index will be in [0,7]
			child_index = r_index+ 2*theta_index + 4*phi_index


		"""
		r, theta, phi = itemgetter(0), itemgetter(1), itemgetter(2)
		ni,nj,nk = self.leaf_resolution

		leaf_key = self.find_leaf(point, self.last_key)
		self.last_key = leaf_key
		bbx = self.tree_data[leaf_key].bbx

		if leaf_key != -1:
			if self.visited.has_key(leaf_key):
				pass
			else:
				self.visited[leaf_key] = self.bbx_mid(bbx)
			
			try:	
				var_data = self.leaf_data[leaf_key][variable].T
			except ValueError:
				print 'available variables:', 
				for var in self.variableNames.values(): print var,
				raise


			""" 
				0    1      2     3 ni = 4
				|----+------+-----|
			 	|--------------x--| x = .8
			 	|-----------+--|--| x*(ni-1) = 2.4

			"""
			#normalize relative to leaf bbx
			# bbx = get_bbx(self.tree_data[leaf_key])
			point_norm = self.point_norm(point,bbx)
			# print point_norm, point
			# get min cell corner
	
			pi = r(point_norm)*(ni-1)
			pj = (theta(point_norm))*(nj-1) 
			pk = phi(point_norm)*(nk-1)

			# print pi,pj,pk
			#get min cell corner
			i0,j0,k0 = int(pi),int(pj),int(pk)
			
			#see if point is on leaf boundary
			if i0==ni-1: i0 -= 1
			if j0==nj-1: j0 -= 1
			if k0==nk-1: k0 -= 1

			i1,j1,k1 = i0+1,j0+1,k0+1 

			#shift to cell, coordinates in [0,1)
			p = pi-i0, pj-j0, pk-k0


			return self.tri_linear(var_data,i0,i1,j0,j1,k0,k1,p)

		else:
			return self.missing_value

	"""Interpolate variable at position along with local resolution """
	def interpolate_dc(self,variable,point):
		val = self.interpolate(variable,point)
		if self.last_key != -1:		
			bbx = self.tree_data[self.last_key]
			ni,nj,nk = self.leaf_resolution
			dr = (bbx.r_max - bbx.r_min)/ni
			dth = (bbx.theta_max - bbx.theta_min)/nj
			dphi = (bbx.phi_max - bbx.phi_min)/nk
			return val, dr, dth, dphi 
		else:
			return self.missing_value, self.missing_value, self.missing_value, self.missing_value

	def tri_linear(self,var_data,i0,i1,j0,j1,k0,k1,p):
		r, theta, phi = itemgetter(0), itemgetter(1), itemgetter(2)
		try:
			c00 = var_data[i0,j0,k0]*(1-r(p)) + var_data[i1,j0,k0]*r(p)
			c10 = var_data[i0,j1,k0]*(1-r(p)) + var_data[i1,j1,k0]*r(p)
			c01 = var_data[i0,j0,k1]*(1-r(p)) + var_data[i1,j0,k1]*r(p)
			c11 = var_data[i0,j1,k1]*(1-r(p)) + var_data[i1,j1,k1]*r(p)

			c0 = c00*(1-theta(p)) + c10*theta(p)
			c1 = c01*(1-theta(p)) + c11*theta(p)

			var_out = c0*(1-phi(p)) + c1*phi(p)

			## this code is not stable!!
			# var_out = 	var_data[i0,j0,k0]*(1-r(p))	*(1-theta(p))	*(1-phi(p)) +\
			# 			var_data[i1,j0,k0]*r(p)		*(1-theta(p))	*(1-phi(p)) +\
			# 			var_data[i0,j1,k0]*(1-r(p))	*theta(p)		*(1-phi(p)) +\
			# 			var_data[i0,j0,k1]*(1-r(p))	*(1-theta(p))	*phi(p) +\
			# 			var_data[i1,j0,k1]*r(p)		*(1-theta(p)	*phi(p)) +\
			# 			var_data[i0,j1,k1]*(1-r(p))	*theta(p)		*phi(p) +\
			# 			var_data[i1,j1,k0]*r(p)		*theta(p)		*(1-phi(p)) +\
			# 			var_data[i1,j1,k1]*r(p)		*theta(p)		*phi(p)
		except IndexError:
			print 'problem at index', "(i0,j0,k0)", i0,j0,k0, "(i1,j1,k1),", i1,j1,k1
			print 'query point', p
			print 'leaf resolution', self.leaf_resolution
			print 'var_data shape:', var_data.shape
			raise
		return var_out


	def set_file_names(self):
		self.header_file_name = testReader.get_config_value(self._config, 'Files', 'HeaderFile')
		self.data_file_name = testReader.get_config_value(self._config, 'Files', 'DataFile')

	def closeFile(self):
		# print 'closing', self.header_file_name
		if not self.header_file.closed:
			self.header_file.close()

		# print 'closing', self.data_file_name
		if not self.data_file.closed:
			self.data_file.close()

		return pyKameleon.FileReader.OK

	def openFile(self, config_file, readonly = True):
		# print 'loading config file'
		if config_file != None:
			try:
				self._config = testReader.getConfig(config_file)
				self.set_file_names()
			except:
				raise

		# print 'setting current filename'
		self.current_filename = self.data_file_name
		self.read_ARMS_header(self.header_file_name)
		self.read_ARMS_data(self.data_file_name)
		self.initializeVariableIDs()
		self.initializeVariableAttributeIDs()
		variables = self.variableNames.values()
		readARMS.variables_tuple = collections.namedtuple('Variables', variables) #so interpolate_variables can access it
		# self.initializeGlobalAttributeIDs()
		# for key, val in self.globalAttributes.items():
		# 	print key, val
		return pyKameleon.FileReader.OK
		
	def get_non_leaf(self):
		# find a root that is not a leaf
		for key,bndbox in self.roots:
			root_block = self.tree_data[key]
			root_type = root_block[0]
			if root_type !=1:
				break

		return key, bndbox


	"""Interpolates set of variables onto input positions"""
	@staticmethod
	@np.vectorize
	def interpolate_variables(r, th, phi, func = lambda var,(r,th,phi): r**(-3)*(1+3*np.sin(th))**.5):
		"""returns a named tuple of interpolated variables"""
		results = []
		for variable in readARMS.variables_tuple._fields:	
			results.append(func(variable,(r,th,phi)))
		return readARMS.variables_tuple(*results)

	def interpolate_bbx(self,var,point):
		get_bbx =itemgetter(3)
		leaf_key = self.find_leaf(point, self.last_key)
		if leaf_key != -1:
			bbx = self.bbx_tuple(*get_bbx(self.tree_data[leaf_key]))._asdict()
			return bbx[var]
		else:
			print "leaf key invalid!"

	def get_leaf_slice(self, leaf_key, var, slice_obj = slice(1)):
		return self.leaf_data[leaf_key][var][slice_obj].squeeze()

	

	def plot_leaf_slice(self,ax,leaf_key,variable, slice_obj=slice(1), cartesian = True,
			**contourf_kwargs):
		bbx = self.tree_data[leaf_key].bbx
		ni,nj,nk = self.leaf_resolution
		ppk,ppj,ppi = np.mgrid[bbx.phi_min:bbx.phi_max:np.complex(nk),
										bbx.theta_min:bbx.theta_max:np.complex(nj),
										bbx.r_min:bbx.r_max:np.complex(ni)
										]

		if cartesian:
			ppk,ppj,ppi = readARMS.ARMS_to_cartesian(ppi,ppj,ppk)


		sliced = readARMS.get_slice(self.leaf_data[leaf_key][variable],slice_obj)
		try:
			ax.contourf(readARMS.get_slice(ppk,slice_obj),readARMS.get_slice(ppi,slice_obj), sliced,
							**contourf_kwargs)
		except TypeError:
			print "Can't plot slice with shape", sliced.shape
			raise


	def plot_visited_leaf_midpoints(self, ax, components = 'xz',marker='o',c='w',s=5, **scatterargs):
		for key,(mid_r,mid_th,mid_ph) in self.visited.items():
			mid_x,mid_y,mid_z = readARMS.ARMS_to_cartesian(mid_r,mid_th,mid_ph)
			ax.scatter(mid_x,mid_z,marker=marker,c=c,s=s,**scatterargs)

	@staticmethod
	def get_slice(array,slice_obj = slice(1)): 
		return array[slice_obj].squeeze()

	"""Obtain the leaf containing a point """
	def plot_mgrid(self, point, fig = None, cmap1 = None, cmap2 = None, show = True):
		#refactor so that only one variable is plotted instead of two, with var and ranges specified
		if fig == None: fig = plt.figure()

		get_bbx = itemgetter(3)
		leaf_key = self.find_leaf(point, self.last_key)
		if leaf_key != -1:
			bbx = self.bbx_tuple(*get_bbx(self.tree_data[leaf_key]))
			ni,nj,nk = self.leaf_resolution

			pp,th,rr = np.mgrid[bbx.phi_min:bbx.phi_max:np.complex(nk),
								bbx.theta_min:bbx.theta_max:np.complex(nj),
								bbx.r_min:bbx.r_max:np.complex(ni)
								]
			mid = lambda x1,x2: (x1+x2)/2
			r_mid = mid(bbx.r_min,bbx.r_max)
			th_mid = mid(bbx.theta_min,bbx.theta_max)
			phi_mid = mid(bbx.phi_min,bbx.phi_max)
			def insert_subplot((rows,columns,num),(x,y,var),contours = 20, colormap = None, (vmin,vmax) = (None,None)):
				ax = fig.add_subplot(rows, columns,num) #refactor to outside of mgrid
				cm = ax.contourf(x, y, var, contours, vmin = vmin, vmax = vmax)
				ax.scatter(r_mid,th_mid,marker='o',c='w',s=5)
				# ax.scatter(point[0],point[1],marker='o',c='k',s=2)
				if colormap != None: plt.colorbar(cm)
				return cm
			k=0
			rr_ = rr[k,:,:]
			th_ = th[k,:,:]
			r_min,r_max = -.01,8
			th_min, th_max = -np.pi/20,np.pi/20
			bth_min, bth_max = -17, 17
			br_min, br_max = -4.1, 35.0
			contours = 40

			def get_var(var): return self.leaf_data[leaf_key][var]
			B_r = get_var('Magnetic_Field_R')[k,:,:]
			B_th = get_var('Magnetic_Field_T')[k,:,:]
			# insert_subplot((2,2,1),(rr_,th_,rr_),np.linspace(r_min,r_max,contours), None, (r_min,r_max))
			# cm = insert_subplot((1,1,1),(rr_,th_,th_),np.linspace(th_min,th_max,contours),None, (th_min,th_max))
			insert_subplot((2,2,3),(rr_,th_,B_r),np.linspace(br_min,br_max,contours),cmap1, (br_min,br_max))
			insert_subplot((2,2,4),(rr_,th_,B_th),np.linspace(bth_min,bth_max,contours),cmap2,(bth_min,bth_max))

			if show: plt.show()

			return fig,(cmap1,cmap2), (rr,th,pp,get_var('Magnetic_Field_R'),get_var('Magnetic_Field_T'),get_var('Magnetic_Field_P'))
		else:
			raise Exception("Could not find leaf key for this point")

	@np.vectorize
	def cartesian_to_ARMS(x,y,z):
			r = math.sqrt((x**2 + y**2 + z**2))
			theta = math.asin(z/r) #in colatitude
			phi = math.atan2(y,x)
			return math.log10(r),theta,phi


	@np.vectorize
	def ARMS_to_cartesian(r_, th_, ph_):
		xx = (10**r_)*math.cos(th_)*math.cos(ph_)
		yy = (10**r_)*math.cos(th_)*math.sin(ph_)
		zz = (10**r_)*math.sin(th_)
		return xx, yy, zz

	@np.vectorize
	def spherical_to_cartesian_field(r,th,ph,A_r,A_th,A_ph,transpose=False):
		#set transpose = True to convert spherical to cartesian
		sin, cos = math.sin, math.cos
		r,th = 10**r, math.pi/2 - th #incoming positions in ARMS coordinates
		A_x = A_r*sin(th)*cos(ph)+A_th*cos(th)*cos(ph)-A_ph*sin(ph)
		A_y = A_r*sin(th)*sin(ph)+A_th*cos(th)*sin(ph)+A_ph*cos(ph)
		A_z = A_r*cos(th)-A_th*sin(th)
		return A_x, A_y, A_z

	"""Maps variables onto tuple containing points. Points can be a tuple of positions in the form (x,y,z) where
		each coordinated is a list or a numpy array of arbitrary dimension"""
	def map(self, points, variables = None, input_coordinates = 'ARMS'):
		if input_coordinates != 'ARMS':
			rr,th,pp = readARMS.cartesian_to_ARMS(*points)
		else:
			rr,th,pp = points

		if variables != None:
			readARMS.variables_tuple = collections.namedtuple('Variables', variables)

		return readARMS.variables_tuple(*readARMS.interpolate_variables(rr, th, pp, self.interpolate))



	def plotVariable(self, xx,yy,zz, variables = None):
		rr,th,pp = readARMS.cartesian_to_ARMS(xx,yy,zz)
		
		if variables != None:
			readARMS.variables_tuple = collections.namedtuple('Variables', variables)

		fig = plt.figure()
		ax = fig.add_subplot(221)

		results = readARMS.variables_tuple(*readARMS.interpolate_variables(rr, th, pp, self.interpolate))
		print "leaf iterations:", self.leaf_iterations

		def plot_visited(self,ax):
			for key,(p_r,p_th,p_ph) in self.visited.items():
				x, y, z = readARMS.ARMS_to_cartesian(p_r,p_th,p_ph)
				ax.scatter(x,z, marker = 'o',c='w',s=5)

		# readARMS.variables_tuple = readARMS.bbx_tuple
		# results = readARMS.variables_tuple(*readARMS.interpolate_variables(rr, th, pp, self.interpolate_bbx))

		
		B_rho = results.Magnetic_Field_R
		B_phi = results.Magnetic_Field_P
		B_theta = results.Magnetic_Field_T
		B = np.sqrt(B_rho**2 + B_theta**2 + B_phi**2)

		B_x, B_y, B_z = readARMS.spherical_to_cartesian_field(rr, th, pp, B_rho, B_theta, B_phi)

		# mask_log = lambda data: np.log10(np.ma.masked_less_equal(np.abs(data),0))
		bth_min, bth_max = -17, 17
		br_min, br_max = -4.1, 35.0
		cm1 = ax.contourf(xx, zz, B_rho, np.linspace(br_min,br_max,40), vmin=br_min, vmax = br_max)
		ax.streamplot(xx[:,0],zz[0,:],B_x.T,B_z.T, color='k', density = 1)
		ax.contour(xx,zz,B_rho,[0], colors = 'w',linewidths=(2,))
		ax.contour(xx,zz,B,[.125], colors = 'k',linewidths=(2,))
		# ax.scatter(xx,zz, marker = '.',c='k',s=1)
		# Q = plt.quiver(xx, zz, B_x/B, B_z/B)
		plot_visited(self,ax)
		
		# plt.colorbar(cs)
		plt.title("B_r")

		ax = fig.add_subplot(222)
		cm2 = ax.contourf(xx, zz, B_theta, 40, vmin = bth_min, vmax = bth_max)
		# Q2 = plt.quiver(xx, zz, B_x/B, B_z/B)
		ax.contour(xx,zz,B_rho,[0], colors = 'w',linewidths=(2,))
		ax.contour(xx,zz,B,[.125], colors = 'k',linewidths=(2,))
		# ax.streamplot(xx[:,0],zz[0,:],B_x.T,B_z.T, color='k', density = 1)
		# plt.colorbar(cs)
		plt.title("B_th")
		plot_visited(self,ax)

		return fig,(cm1,cm2)

class Test_ARMS_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Testing ARMS attributes'
		self.armsreader = readARMS()
		print 'ARMS reader initialized'
		self.armsreader.open('/Users/apembrok/Work/DeVore/ARMs/ARMS.ini')
		print 'File opened'
		self.armsreader.close()
		print 'File closed'

	def test_variables_created(self):
		self.assertTrue(self.armsreader.doesVariableExist('Mass_Density'))
		with self.assertRaises(NameError): self.armsreader.getVariable('missing_variable')
		for key, variable_name in self.armsreader.variableNames.items():
			print 'key:', key, 'variable name:', variable_name
			var_min = self.armsreader.getVariableAttribute(variable_name,'actual_min').getAttributeValue()
			var_max = self.armsreader.getVariableAttribute(variable_name,'actual_max').getAttributeValue()
			print 'var min, max', variable_name, var_min, var_max

			for attr_id, attr_name in self.armsreader.variableAttributeNames.items():
				attr = self.armsreader.variableAttributes[variable_name][attr_name]
				print '\t', attr_id, attr.getAttributeName(), attr.getAttributeValue(), attr.getAttributeType()

		print '\n'
		print '\tchecking that model_name attributes exist:', 
		self.assertTrue(self.armsreader.doesAttributeExist('model_name'))
		print self.armsreader.globalAttributes['model_name'].getAttributeValue()
		print '\tchecking that grid_type attributes exists:',
		self.assertTrue(self.armsreader.doesAttributeExist('grid_type'))
		print self.armsreader.globalAttributes['grid_type'].getAttributeValue()

		print '\tglobal attributes:'
		for key, attr in self.armsreader.globalAttributes.items():
			print '\t ', attr.getAttributeName(),':', attr.getAttributeValue(), attr.getAttributeType()


		

def main():
	print "Performing tests for ARMS reader"

	unittest.main()

if __name__ == '__main__':
	main()

