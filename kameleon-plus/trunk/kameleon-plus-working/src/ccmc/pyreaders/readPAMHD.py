#1/usr/bin/python
import testReader
from Attribute import Attribute
import pyKameleon
import unittest
import PAMHD
import numpy as np
from scipy.interpolate import RegularGridInterpolator

class readPAMHD(testReader.pyFileReader):
	def __init__(self):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)
		self.variables = {}
		self.interpolators = {}
		self.missing_value = 30

	def openFile(self, filename, readonly = True):
		"""
			PAMHD.load returns simulation_params as a named tuple
			It takes a dictionary as input and fills it with cell_id, mhd_data pairs
			each mhd_data is a named tuple
			"""
		print 'openFile by readPAMHD'
		self.current_filename = filename
		# self.setCurrentFilename(filename) # alternatively
		
		self.mhd_dict = {}
		self.sim_params = PAMHD.load(filename, self.mhd_dict)
		self.set_global_attributes()

		self.set_variables()

		self.set_variable_names([	'c0','c1','c2', 
									'mass_density',
									'px','py','pz', 
									'total_energy_density', 
									'bx','by','bz', 
									'jx','jy','jz', 
									'electric_resistivity'
									])
		self.set_variable_attributes()

		self.set_positions()

		self.set_resolution()
		self.set_interpolators()

		return pyKameleon.FileReader.OK


	def set_global_attributes(self):
		# self.globalAttributes['timestep_time'] = Attribute('timestep_time', date+'T'+str(self.sim_params[0])+'.000Z')
		for i, name in enumerate(self.sim_params._fields):
			self.globalAttributes[name] = Attribute(name, float(self.sim_params[i]))

		self.initializeGlobalAttributeIDs()

	def set_variable_names(self,variable_names):
		for i, name in enumerate(variable_names):
			self.addVariableName(name, i)

	def set_variables(self):
		mhd_data = []
		for cell_id, mhd_array in self.mhd_dict.items():
			for mhd in mhd_array:
				mhd_data.append(tuple(list(mhd)+[cell_id]))

		mhd_dtype = np.dtype([
								('c0', float),
								('c1', float), 
								('c2',float),
								('mass_density', float),
								('px',float),
								('py',float),
								('pz',float),
								('total_energy_density',float),
								('bx',float),
								('by',float),
								('bz',float),
								('jx',float),
								('jy',float),
								('jz',float),
								('cell_type',int),
								('mpi_rank',int),
								('electric_resistivity',float),
								('cell_id',int)
								])
		mhd_data = np.array(mhd_data, dtype=mhd_dtype)

		self.mhd_data = mhd_data.view(np.recarray)

	def set_variable_attributes(self):
		# Need to add units
		for key in self.variableNames.values():
			min_ = self.mhd_data[key].min()
			max_ = self.mhd_data[key].max()
			self.variableAttributes[key]['actual_min'] = Attribute('actual_min', float(min_))
			self.variableAttributes[key]['actual_max'] = Attribute('actual_max', float(max_))


	def set_positions(self):
		np.sort(self.mhd_data, order=['c0','c1','c2'])
		self.unique_points = np.unique(self.mhd_data.c0), np.unique(self.mhd_data.c1), np.unique(self.mhd_data.c2)
		self.res_3D = tuple([len(u) for u in self.unique_points]) # (n0,n1,n2)

		self._select = tuple([i for i in range(3) if self.res_3D[i] > 1]) # e.g. (0,2) if n1 = 1 
		
		if self.res_3D[0]*self.res_3D[1]*self.res_3D[2] == len(self.mhd_data):
			self.resolution = tuple([self.res_3D[s] for s in self._select]) # e.g. (n0,n2) if n1 = 1
			self.sparse_grid = tuple([self.unique_points[s] for s in self._select])
			self.mhd_data = self.mhd_data.reshape(self.resolution)
		else:
			raise ArithmeticError('Could not construct regular grid')

	def set_interpolators(self):
		'''Create interpolators for each variable'''
		for i in range(self.getNumberOfVariables()):
			var_name = self.getVariableName(i)
			self.interpolators[var_name] = RegularGridInterpolator(self.sparse_grid, self.mhd_data[var_name], fill_value = self.missing_value)

	def set_resolution(self):
		'''Compute gradients for coordinates with size > 1. Store interpolators.'''
		dc = tuple([np.gradient(g) for g in self.sparse_grid])
		dc_ = np.meshgrid(*dc, indexing = 'ij')
		self.dc_names = ['dc0', 'dc1', 'dc2']
		names = [self.dc_names[s] for s in self._select]

		for i,name in enumerate(names):
			self.interpolators[name] = RegularGridInterpolator(self.sparse_grid, dc_[i])

	def interpolate(self,variable, *point):
		'''Interpolates for arbitrary 1D, 2D and 3D regular grids'''
		rgi = self.interpolators[variable]
		point_ = [point[s] for s in self._select]
		result = rgi([point_])[0]
		return result

	def interpolate_dc(self,variable,*point):
		value = self.interpolate(variable,*point)
		point_ = [point[s] for s in self._select]
		dc = [0,0,0]
		for i in range(3):
			if i in self._select:
				rgi = self.interpolators[self.dc_names[i]]
				dc[i] = rgi([point_])[0]
			else:
				pass

		return value, dc[0], dc[1], dc[2]

class Test_pyReader(unittest.TestCase):
	def test_open_check(self):
		test = readPAMHD()

		filename = '/Users/apembrok/Work/Honkonen/Python_program_for_reading_MHD_output/mhd_1.000e+00_s.dc'
		print '\tchecking that pyFileReader.open() returns FileReader.OK'
		self.assertEqual(test.open(filename), pyKameleon.FileReader.OK)
		self.assertEqual(test.current_filename, filename)

		print 'global Attributes:'
		for i in range(test.getNumberOfGlobalAttributes()):
			name = test.getGlobalAttributeName(i)
			glob_attr = test.getGlobalAttribute(name)
			print '\t', name, glob_attr.getAttributeValue()

		print 'variable Attributes:'
		for i in range(test.getNumberOfVariables()):
			name = test.getVariableName(i)
			min_ = test.getVariableAttribute(name,'actual_min').getAttributeValue()
			max_ = test.getVariableAttribute(name,'actual_max').getAttributeValue()
			print '\t', name, min_, max_

		import matplotlib.pyplot as plt
		plt.plot(test.grid[0], test.mhd_data.by)
		point = (10., 0., 0.)
		result = test.interpolate('bx', *point)
		print result
		print 'dimensions:', len(test.grid)
		plt.show()


def main():
	unittest.main()

	print issubclass(readPAMHD,testReader.pyFileReader)
	print issubclass(readPAMHD,pyKameleon.FileReader)




if __name__ == '__main__':
	main()
