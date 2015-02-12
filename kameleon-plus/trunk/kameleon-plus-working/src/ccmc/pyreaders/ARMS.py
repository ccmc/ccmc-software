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


	def read_ARMS_header(self, header_filename):
		"""Read gloabl attributes from file """
		header_file = open(header_filename, 'r')

		# get time step
		line = header_file.readline().split()
		time = line[0]
		self.globalAttributes['timestep_time'] = Attribute('timestep_time', '2000-01-01T'+time+'.000Z')

		# get grid type
		line = header_file.readline().split()
		grid_type = line[0]
		self.globalAttributes['grid_type'] = Attribute('grid_type', grid_type)

		if grid_type == 'Spherical_Exponential':
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
						var_name = var_base_name + '_mag'
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

		header_dtype = np.dtype('>i4')
		f = FortranFile(filename, 'r', header_dtype = header_dtype)
		
		n_chars = f._read_size()[0]
		print 'size:', n_chars
		char_dtype = np.dtype('a'+str(n_chars))

		model_name = f.read_record(char_dtype)
		print model_name

		endian = '<'
		# dt = np.dtype(endian+'a'+str(size))
		ff = FF(filename, endian)
		try:
			model_name = ff.readRecord()
		except IOError:
			ff.close()
			endian = '>'
			ff = FF(filename, endian)
			model_name = ff.readRecord()

		print 'file endian: ', endian
		print 'model name is:', model_name

		#set model name
		self.globalAttributes['model_name'] = Attribute('model_name', model_name)

		# print 'header length:', f._header_length
		# #try to read sim_time using numpy
		# # dt = np.dtype([int, long])
           
		# # time = np.fromfile(f,dtype=dt)

		# #set model simultation time
		# time = f.readReals()[0]
		# self.globalAttributes['sim_time'] = Attribute('sim_time', float(time))

		# num_total_blocks, num_leaf_blocks, new_grid_flag = f.readInts()
		# self.globalAttributes['num_total_blocks'] = Attribute('num_total_blocks', int(num_total_blocks))
		# self.globalAttributes['num_leaf_blocks'] = Attribute('num_leaf_blocks', int(num_leaf_blocks))
		# self.globalAttributes['new_grid_flag'] = Attribute('new_grid_flag', int(new_grid_flag))


		# ni = self.getGlobalAttribute('RBlockSize').getAttributeValue()
		# nj = self.getGlobalAttribute('TBlockSize').getAttributeValue()
		# nk = self.getGlobalAttribute('PBlockSize').getAttributeValue()
		# print 'number total blocks!', num_total_blocks
		# ancestors = 0



		# for block_number in range(num_total_blocks):
		# 	line = f.readInts() #lbpe(2), type, prnt(2), child(2,8)
		# 	f.readReals() # bndbox(2,3)
		# 	leaf_type = line[2] # 1 = leaf, 2 = parent, 3 = grand-parent, etc
		# 	# use seek offset to skip the data
		# 	if leaf_type == 1:
		# 		for i in range(ni):
		# 			for j in range(nj):
		# 				for k in range(nk):
		# 					f.readReals() # vars(N)
		# 	else:
		# 		if leaf_type == 5:
		# 			print '-',
		# 		elif leaf_type == 4: 
		# 			print 'G',
		# 		elif leaf_type == 3: 
		# 			print 'g',
		# 		elif leaf_type == 2: 
		# 			print '.',
		# 		else:
		# 			print '^',

		# 		ancestors += 1
		# 		if ancestors%70 == 0: print '\n',

		# print '\n'
		# print 'ancestors counted:', ancestors
		f.close()







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
		for key, variable_name in self.armsreader.variableNames.items():
			print 'key:', key, 'variable name:', variable_name

			for attr_id, attr_name in self.armsreader.variableAttributeNames.items():
				attr = self.armsreader.variableAttributes[variable_name][attr_name]
				print '\t', attr_id, attr.getAttributeName(), attr.getAttributeValue(), attr.getAttributeType()

	# def test_attributes_loaded(self):
		print '\tchecking that model_name attributes exist'
		self.assertTrue(self.armsreader.doesAttributeExist('model_name'))
		print '\tchecking that grid_type attributes exist'
		self.assertTrue(self.armsreader.doesAttributeExist('grid_type'))
		print '\tglobal attributes:'
		for key, attr in self.armsreader.globalAttributes.items():
			print '\t ', attr.getAttributeName(),':', attr.getAttributeValue(), attr.getAttributeType()


def main():
	print "Performing tests for ARMS reader"
	unittest.main()

if __name__ == '__main__':
	main()

