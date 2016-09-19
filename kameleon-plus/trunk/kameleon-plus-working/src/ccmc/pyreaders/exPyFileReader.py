# exPyFileReader.py
"""Illustrates basic use of pyFileReader class"""
import testReader
import pyKameleon
import random
import unittest


class customPyFileReader(testReader.pyFileReader):
	"""Basic use of pyFileReader class"""

	def __init__(self):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)
		
		#all variables will be stored here
		testReader.pyFileReader.variables = {}

	def openFile(self, filename, readonly = True):
		print 'custom open by exPyFileReader'
		self.current_filename = filename
		# self.setCurrentFilename(filename) # alternatively
		
		print 'data file:', filename, 'readonly:', readonly
		self.variables['sound_speed'] = readVariableFromFile(filename, 'sound_speed')

		return pyKameleon.FileReader.OK

class Test_pyReader(unittest.TestCase):
	def test_open_check(self):
		test = customPyFileReader()
		print '\tchecking that pyFileReader.open() returns FileReader.OK'
		self.assertEqual(test.open('SomeDataFile.dat'), pyKameleon.FileReader.OK)
		print '\topen worked..'
		self.assertEqual(test.current_filename, 'SomeDataFile.dat')


def main():
	unittest.main()


	print issubclass(customPyFileReader,testReader.pyFileReader)
	print issubclass(customPyFileReader,pyKameleon.FileReader)

def readVariableFromFile(filename, variable):
	vector = pyKameleon.vectorFloat()
	for i in range(30):
		vector.append(random.random())

	return vector


if __name__ == '__main__':
	main()


