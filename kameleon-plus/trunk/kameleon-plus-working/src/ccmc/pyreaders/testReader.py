#1/usr/bin/python
import unittest
import sys, os
sys.path.append('build')
sys.path.append('.')
sys.path.append('../')
import pyKameleon
from pyKameleon import FileReader, Attribute
# import random #only used for testing
import ConfigParser
import inspect


class FileReaderFactory(object):
	def __init__(self, config_file = None):
		if config_file == None:
			Config = ConfigParser.ConfigParser()
			Config.add_section('Reader')
			Config.set('Reader','pyreadermodule','testReader')
			Config.set('Reader','pyfilereaderclass','pyFileReader')
			self.Config = Config
		else:
			self.Config = getConfig(config_file)

	def createPyReader(self):
		pyReader_module_name = ConfigSectionMap(self.Config,"Reader")['pyReaderModule'.lower()]
		pyReader_class_name = ConfigSectionMap(self.Config, "Reader")['pyFileReaderClass'.lower()]
		if pyReader_module_name == "testReader":
			# print "using base pyFileReader"
			return pyFileReader()
		else:
			print "\tattempting to return subclass", pyReader_class_name
			try: 
				#TODO: check path/to/pyReader.py case, if so add path to pythonpath
				pyReader_module = __import__(pyReader_module_name)
				print "\timported", pyReader_module_name

				#TODO: check that pyReader_class is a subclass of FileReader
				pyReader = getattr(pyReader_module, pyReader_class_name)()
				assert issubclass(pyReader.__class__, FileReader)				
				pyReader.Config = self.Config
				return pyReader

			except:
				raise ImportError("could not import " + pyReader_module_name)


class pyFileReader(FileReader):
	""" General class for exposing python reader to kameleon.
		
	pyFileReader is a subclass of the Kameleon FileReader c++ class

	The following static variables of the FileReader class are available:
	ATTRIBUTE_DOES_NOT_EXIST 
	FILE_DOES_NOT_EXIST      
	LOAD_FAILED              
	MODEL_NOT_SUPPORTED      
	NOT_A_VALID_KAMELEON_FILE
	OK                       
	OPEN_ERROR               
	UNABLE_TO_ALLOCATE_MEMORY
	VARIABLE_DOES_NOT_EXIST  
	VARIABLE_NOT_IN_MEMORY   
	"""

	def __init__(self):
		"""default pyFileReader constructor

		This initializer calls the base class initializer.
		The variables dictionary is initialized here and is expected to be populated by the subclassing reader.
		"""
		super(pyFileReader, self).__init__()
		self.variables = {} #created in subclass


	def openFile(self, filename, readonly = True):
		""" Opens file, sets filename. To be overriden in subclass"""
		# print 'opening', filename, 'now'
		# print datapath + ' --> ' + filename
		self.current_filename = filename

		return FileReader.OK

	def getVariableInt(self, variable):
		if self.variables.has_key(variable):
			# print '\tpyFileReader.getVariableInt: already loaded variable ', variable, ' returning stored ', type(self.variables[variable])
			return self.variables[variable]
		else:
			print "\tpyFileReader.getVariableInt: variable", variable, "not yet loaded, loading.."
			if (type(variable) == int)|(type(variable) == long):
				return self.getVariable(self.getKey(variable))

			# vector = pyKameleon.vectorInt() #of type std::vector<int>



	def getVariableIntAtIndex(self, variable, index):
		variable = self.getKey(variable)
		if index < len(self.variables[self.getKey(variable)]):
			return self.variables[variable][index]
		else:
			raise IndexError('Index beyond size of variable!')

	def getVariable(self, variable, startIndex = None, count = 15):
		if self.variables.has_key(variable):
			# print '\tpyFileReader.getVariable: already loaded variable ', variable, ' returning stored ', type(self.variables[variable])
			return self.variables[variable]
		elif (type(variable) == int)|(type(variable) == long):
				return self.getVariable(self.getKey(variable), startIndex, count)

		else:
			raise KeyError("pyFileReader.getVariable: variable", variable, "not loaded")


		#find out what startIndex and count are for..
		if startIndex != None:
			print "\tpyFileReader.getVariable: startIndex = ", startIndex
		if count != None:
			print "\tpyFileReader.getVariable: count = ", count


	def getKey(self, variable):
		if self.variables.has_key(variable):
			return variable
		elif (type(variable) == int)|(type(variable) == long):
			if (variable < len(self.variables.keys())) & (variable >= 0):
				return self.variables.keys()[variable]
			else:
				raise KeyError("Index out of range of loaded Variables")
		else:
			raise KeyError("no such variable " + str(variable)+ " found")

	def getVariableAtIndex(self, variable, index):
		variable = self.getKey(variable)
		if index < len(self.variables[self.getKey(variable)]):
			return self.variables[variable][index]
		else:
			raise IndexError('Index beyond size of variable!')


class Test_pyReader(unittest.TestCase):
	def setUp(self):
		self.testReader = pyFileReader()
		self.rho = fillRandom()
		self.topology = fillRandom('int')
		self.testReader.variables['rho'] = self.rho
		self.testReader.variables['topology'] = self.topology

	def test_open_check(self):
		print '\tchecking that pyFileReader.open() returns FileReader.OK'
		self.assertEqual(self.testReader.open('SomeDataFile.dat'), FileReader.OK)
		self.assertEqual(self.testReader.current_filename, 'SomeDataFile.dat')

	def test_getVariable(self):
		self.assertEqual(self.testReader.getVariable('rho'), self.rho)
		self.assertEqual(self.testReader.getVariable('topology'), self.topology)
	
	def test_getVariableAtIndex(self):
		self.assertEqual(self.testReader.getVariableAtIndex('rho', -1), self.rho[-1])
		self.assertEqual(self.testReader.getVariableIntAtIndex('topology',-1), self.topology[-1])

	def test_getVariableAtIndex_Fail(self):
		self.assertRaises(IndexError, self.testReader.getVariableAtIndex,'rho', len(self.rho))

	def test_pyReader_Factory(self):
		factory = FileReaderFactory()
		reader = factory.createPyReader()
		self.assertEqual(reader.__class__,pyFileReader)

	def test_pyReader_Facotry_with_custom(self):
		factory = FileReaderFactory('kameleon.ini')
		reader = factory.createPyReader()
		self.assertIs(issubclass(reader.__class__, FileReader), True)

		factory = FileReaderFactory('RCM.ini')
		reader = factory.createPyReader()
		self.assertIs(issubclass(reader.__class__, FileReader), True)

 
def main():
	print "Performing tests for pyReader"
	unittest.main()

def fillRandom(type = 'float', count = 30):
	import random
	if type == 'float':
		vector = pyKameleon.vectorFloat()
		for i in range(count):
			vector.append(random.random())
		return vector
	if type == 'int':
		vector = pyKameleon.vectorInt()
		for i in range(count):
			vector.append(int(5*random.random()))
		return vector

def getConfig(config_file):
	Config = ConfigParser.ConfigParser()
	Config.read(config_file)
	Config.sections()
	return Config

def ConfigSectionMap(Config, section):
    dict1 = {}
    options = Config.options(section)
    for option in options:
        try:
            dict1[option] = Config.get(section, option)
            if dict1[option] == -1:
                print("skip: %s" % option)
        except:
            print("exception on %s!" % option)
            dict1[option] = None
    return dict1



def itersubclasses(cls, _seen=None):
    """
    itersubclasses(cls)

    Generator over all subclasses of a given class, in depth first order.

    >>> list(itersubclasses(int)) == [bool]
    True
    >>> class A(object): pass
    >>> class B(A): pass
    >>> class C(A): pass
    >>> class D(B,C): pass
    >>> class E(D): pass
    >>> 
    >>> for cls in itersubclasses(A):
    ...     print(cls.__name__)
    B
    D
    E
    C
    >>> # get ALL (new-style) classes currently defined
    >>> [cls.__name__ for cls in itersubclasses(object)] #doctest: +ELLIPSIS
    ['type', ...'tuple', ...]
    """
    
    if not isinstance(cls, type):
        raise TypeError('itersubclasses must be called with '
                        'new-style classes, not %.100r' % cls)
    if _seen is None: _seen = set()
    try:
        subs = cls.__subclasses__()
    except TypeError: # fails only when cls is type
        subs = cls.__subclasses__(cls)
    for sub in subs:
        if sub not in _seen:
            _seen.add(sub)
            yield sub
            for sub in itersubclasses(sub, _seen):
                yield sub


if __name__ == '__main__':
	main()

