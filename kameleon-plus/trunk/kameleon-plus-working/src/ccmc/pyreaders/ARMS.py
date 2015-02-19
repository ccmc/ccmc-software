#1/usr/bin/python
import unittest
import sys, os
sys.path.append('build')
sys.path.append('.')
# sys.path.append('../')
import pyKameleon, testReader
from Attribute import Attribute
# import random #only used for testing
import ConfigParser
from scipy.io import FortranFile
from fortranfile import FortranFile as FF
import numpy as np
from collections import defaultdict
from operator import itemgetter, attrgetter


"""Creating reader for ARMS code. The idea is to treat ARMS like a batsrus file. We first need to understand what has to be in a batsrus file,
	which lets us read in the ARMS results and interpolate with the BATSRUS interpolator.

	BATSRUS variables:
		block_x_min_ (float)
		block_x_max_
		block_y_min_
		block_y_max_
		block_z_min_
		block_z_max_	

		block_child_count_ (variable int)
		block_x_center (float)
		block_y_center
		block_z_center

		block_child_id_1_ (variable integers)
		block_child_id_2_
		block_child_id_3_
		block_child_id_4_
		block_child_id_5_
		block_child_id_6_
		block_child_id_7_
		block_child_id_8_

		block_at_amr_level_ (variable int)


	 """

class readARMS(testReader.pyFileReader):
	"""Reader for the Adaptively Refined MHD Solver (ARMS) """
	def __init__(self, config_file = None):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)

		if (config_file != None):
			self.Config = testReader.getConfig(config_file)

		self.tree_data = {}
		self.leaf_data = {}
		self.roots = [] # sorted by R,T,P bounding box minima

	def read_ARMS_header(self, header_filename):
		"""Read gloabl attributes from file """
		header_file = open(header_filename, 'r')

		# get time step
		line = header_file.readline().split()
		time = line[0]
		self.globalAttributes['timestep_time'] = Attribute('timestep_time', '2000-01-01T'+time+'.000Z')

		# get grid type
		line = header_file.readline().split()
		self.grid_type = line[0]
		self.globalAttributes['grid_type'] = Attribute('grid_type', self.grid_type)

		if self.grid_type == 'Spherical_Exponential':
			components = ['R', 'T', 'P']
		else:
			components = ['x', 'y', 'z'] # just guessing here...

		# get Block sizes
		for i in range(3):
			dim_size, dim_name = header_file.readline().split()
			self.globalAttributes[dim_name] = Attribute(dim_name, int(dim_size))


		#initialize variables
		var_num = 0
		
		line = ' '
		while len(line) > 0:
			line = header_file.readline().split()
			if len(line) == 0:
				break
			else:
				number_components, var_base_name = line
			
			if int(number_components) == 1:
				self.addVariableName(var_base_name, var_num)

				# get variable units
				val, attr = header_file.readline().split() 
				self.variableAttributes[var_base_name]['units'] = Attribute('units', val)

				# get variable scale factor
				val, attr = header_file.readline().split()
				self.variableAttributes[var_base_name]['scale_factor'] = Attribute('scale_factor', float(val))

				# get min and max values (what are the valid_min valid_max values???)
				val, attr = header_file.readline().split()
				self.variableAttributes[var_base_name]['actual_min'] = Attribute('actual_min', float(val))

				val, attr = header_file.readline().split()
				self.variableAttributes[var_base_name]['actual_max'] = Attribute('actual_max', float(val))

				var_num += 1
			else:
				# get variable units
				units, attr = header_file.readline().split() 

				# get variable scale factor
				scale_factor, attr = header_file.readline().split()


				for i in range(int(number_components)+1):
					if i == int(number_components):
						val, attr = header_file.readline().split()
						self.globalAttributes[var_base_name+'_mag_min'] = Attribute(var_base_name+'_mag_min', float(val))

						val, attr = header_file.readline().split()
						self.globalAttributes[var_base_name+'_mag_max'] = Attribute(var_base_name+'_mag_max', float(val))

						
					else:
						var_name = var_base_name + '_' + components[i]

						self.addVariableName(var_name, var_num)
						self.variableAttributes[var_name]['units'] = Attribute('units', units)
						self.variableAttributes[var_name]['scale_factor'] = Attribute('scale_factor', float(scale_factor))

						# get min and max values (what are the valid_min valid_max values???)
						val, attr = header_file.readline().split()
						self.variableAttributes[var_name]['actual_min'] = Attribute('actual_min', float(val))

						val, attr = header_file.readline().split()
						self.variableAttributes[var_name]['actual_max'] = Attribute('actual_max', float(val))

						var_num += 1

	def read_ARMS_data(self, filename):
		"""Reads in the arms data files """

		f = file(filename)
		s = f.read()

		endian = '>'
		offset = [0]
		header_dtype = np.dtype(endian+'u4')

		model_name_length = np.frombuffer(s, dtype=header_dtype, offset = offset[0], count=1)[0]
		if model_name_length > 100:
			print 'header length too long, switching endian'
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
		model_name = read_record(s,'S'+str(model_name_length), offset=offset)[0]
		self.globalAttributes['model_name'] = Attribute('model_name', model_name)


		#set model time
		model_time = read_record(s, endian+'f1', offset = offset)[0]
		self.globalAttributes['sim_time'] = Attribute('sim_time', float(model_time))

		# get number of total blocks, leaf blocks, and new_grid_flag
		
		num_total_blocks, num_leaf_blocks, new_grid_flag = read_record(s, endian+'i4', 3, offset = offset)
		print 'offset:', offset
		print 'total blocks, leaf blocks, new grid:', num_total_blocks, num_leaf_blocks, new_grid_flag

		self.globalAttributes['num_total_blocks'] = Attribute('num_total_blocks', int(num_total_blocks))
		self.globalAttributes['num_leaf_blocks'] = Attribute('num_leaf_blocks', int(num_leaf_blocks))
		self.globalAttributes['new_grid_flag'] = Attribute('new_grid_flag', int(new_grid_flag))

		ni = self.getGlobalAttribute('RBlockSize').getAttributeValue()
		nj = self.getGlobalAttribute('TBlockSize').getAttributeValue()
		nk = self.getGlobalAttribute('PBlockSize').getAttributeValue()
		print 'number total blocks!', num_total_blocks
		ancestors = 0
		print 'dimensions', ni,nj,nk
		print 'points per leaf:', ni*nj*nk

		def create_block_datatype():
			dtype_list = []
			dtype_list.append(('tree_header', endian+'i4'))
			dtype_list.append(('block_loc', endian +'2i4'))
			dtype_list.append(('block_type', endian +'i4'))
			dtype_list.append(('parent_loc', endian +'2i4'))
			dtype_list.append(('child_loc', endian +'2,8i4'))
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
			
			
			self.tree_data[block_key] = [block_type, parent_key, child_key, bndbox]

			# 1 = leaf, 2 = parent, 3 = grand-parent, etc
			if block_type == 1: 
				self.leaf_data[block_key] = np.frombuffer(s, dtype=variable_datatype, count = ni*nj*nk, offset = offset[0])
				offset[0] += variable_datatype.itemsize*ni*nj*nk
			
			if parent_key == (-1, -1):
				self.roots.append((
					block_key[0],
					block_key[1],
				 	bndbox[0],
				 	bndbox[2],
					bndbox[4])
				)


		f.close()

		self.sort_roots()
		self.set_root_resolution()

	def sort_roots(self):
		print 'number of roots:', len(self.roots)
		self.roots.sort(key=itemgetter(4))
		self.roots.sort(key=itemgetter(3))
		self.roots.sort(key=itemgetter(2))

	def set_root_resolution(self):
		r, theta, phi = [self.roots[0][2]], [self.roots[0][3]], [self.roots[0][4]]
		for k in range(1,len(self.roots)):
			phi.append(self.roots[k][4])
			if phi[k] < phi[k-1]:
				phi.pop()
				break

		nk = len(phi)

		for j in range(1,len(self.roots)/nk):
			theta.append(self.roots[j*nk][3])
			if theta[j] < theta[j-1]:
				theta.pop()
				break

		nj = len(theta)
		ni = len(self.roots)/(nj*nk)
		print 'ni,nj,nk=', ni,nj,nk

		for i in range(1,ni):
			r.append(self.roots[i*nj*nk][2])	

		self.root_coord = (r,theta,phi)	

		print theta
		print phi
		# self.plot_root_coord()
		r_mid = (r[2]+r[1])/2
		theta_mid = (theta[1]+theta[0])/2
		phi_mid = (phi[1]+phi[0])/2
		mid_loc = self.find_point(np.array([r_mid,theta_mid,phi_mid]))
		print 'cell is:', mid_loc, mid_loc[0]*nj*nk+mid_loc[1]*nk+mid_loc[2]


	def plot_root_coord(self, index = 2):
		from mpl_toolkits.mplot3d import axes3d
		import matplotlib.pyplot as plt
		import numpy as np
		# 
		r, theta, phi = self.root_coord

		fig = plt.figure()
		ax = fig.add_subplot(111, projection='3d')

		if index == 2:
			rr,pp = np.meshgrid(r,phi, sparse=True)
			xx = rr*np.cos(pp)
			yy = rr*np.sin(pp)
			zz = 0*xx

		ax.plot_wireframe(rr,pp,yy)

		plt.show()

	def find_point(self,point):
		print point
		r, theta, phi = self.root_coord
		print theta
		i = np.searchsorted(r,point[0])-1
		j = np.searchsorted(theta,point[1])-1
		k = np.searchsorted(phi,point[2])-1
		return i,j,k


	def openFile(self, filename, readonly = True):
		print 'opening binary ARMS file', filename
		self.current_filename = filename
		# self.variables['eeta'] = read_binary_file("eeta")
		self.read_ARMS_header('/Users/apembrok/Work/DeVore/ARMs/kameleon.0447339')
		self.read_ARMS_data(filename)
		self.initializeVariableAttributeIDs()
		# self.initializeGlobalAttributeIDs()
		# for key, val in self.globalAttributes.items():
		# 	print key, val
		return pyKameleon.FileReader.OK



class Test_ARMS_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Testing ARMS attributes'
		self.armsreader = readARMS()
		self.armsreader.open('/Users/apembrok/Work/DeVore/ARMs/F/flicks.0447339')

	def test_variables_created(self):
		self.assertTrue(self.armsreader.doesVariableExist('Mass_Density'))
		with self.assertRaises(NameError): self.armsreader.getVariable('missing_variable')
		# for key, variable_name in self.armsreader.variableNames.items():
		# 	print 'key:', key, 'variable name:', variable_name

		# 	for attr_id, attr_name in self.armsreader.variableAttributeNames.items():
		# 		attr = self.armsreader.variableAttributes[variable_name][attr_name]
		# 		print '\t', attr_id, attr.getAttributeName(), attr.getAttributeValue(), attr.getAttributeType()

		# print self.armsreader.variableAttributes
	# def test_attributes_loaded(self):
		print '\n'
		print '\tchecking that model_name attributes exist'
		self.assertTrue(self.armsreader.doesAttributeExist('model_name'))
		print '\tchecking that grid_type attributes exist'
		self.assertTrue(self.armsreader.doesAttributeExist('grid_type'))
		# print '\tglobal attributes:'
		# for key, attr in self.armsreader.globalAttributes.items():
		# 	print '\t ', attr.getAttributeName(),':', attr.getAttributeValue(), attr.getAttributeType()


def main():
	print "Performing tests for ARMS reader"

	unittest.main()

if __name__ == '__main__':
	main()

