#1/usr/bin/python
import testReader
from Attribute import Attribute
import pyKameleon
import unittest
import PAMHD
import numpy as np
from scipy.interpolate import RegularGridInterpolator

class AnalyticModel(testReader.pyFileReader):
	def __init__(self):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)
		self.variables = {}
		self.missing_value = -100000000

	def openFile(self, filename, readonly = True):
		"""
			PAMHD.load returns simulation_params as a named tuple
			It takes a dictionary as input and fills it with cell_id, mhd_data pairs
			each mhd_data is a named tuple
			"""
		print 'openFile by AnalyticModel'
		self.current_filename = filename
		# self.setCurrentFilename(filename) # alternatively

		self.set_variable_names([	'x', 'y', 'z',
									'bx', 'by', 'bz',
									])

		# self.set_global_attribute()
		self.initializeGlobalAttributeIDs()
		self.set_positions()
		return pyKameleon.FileReader.OK


	def set_global_attribute(self, name, value):
		self.globalAttributes[name] = Attribute(name, float(value))

		

	def set_variable_names(self,variable_names):
		for i, name in enumerate(variable_names):
			self.addVariableName(name, i)

	
	def set_variable_attributes(self):
		# Need to add units
		for key in self.variableNames.values():
			min_ = self.variables[key].min()
			max_ = self.variables[key].max()
			self.variableAttributes[key]['actual_min'] = Attribute('actual_min', float(min_))
			self.variableAttributes[key]['actual_max'] = Attribute('actual_max', float(max_))


	def set_positions(self):
		nx = self.globalAttributes['nx'].getAttributeValue()
		ny = self.globalAttributes['ny'].getAttributeValue()
		nz = self.globalAttributes['nz'].getAttributeValue()
		print nx,ny,nz


	def interpolate(self,variable, *point):
		'''Interpolates for arbitrary 1D, 2D and 3D regular grids'''
		point = dict(x=point[0], y=point[1], z=point[2])
		expr = self.globalAttributes['expr'].getAttributeValue()
		result = eval(expr,point)
		return result

	def interpolate_dc(self,variable,*point):
		value = self.interpolate(variable,*point)
		expr = self.globalAttributes['res'].getAttributeValue()
		dc = eval(expr)
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
		test = AnalyticModel()

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

	print issubclass(AnalyticModel,testReader.pyFileReader)
	print issubclass(AnalyticModel,pyKameleon.FileReader)




if __name__ == '__main__':
	main()
