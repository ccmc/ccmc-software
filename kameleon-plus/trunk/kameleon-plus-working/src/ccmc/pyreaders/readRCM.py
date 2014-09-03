import testReader
import pyKameleon
import unittest
from fortranfile import FortranFile
import numpy
import struct

class RCM_reader(testReader.pyFileReader):
	"""Reader for Rice Convection Model"""

	def __init__(self, config_file = None):
		# This line is required to call the base class constructor
		super(testReader.pyFileReader, self).__init__()

		if (config_file != None):
			self.Config = testReader.getConfig(config_file)


		self.variables = {}


	def openFile(self, filename, readonly = True):
		print 'opening binary RCM file', filename
		self.current_filename = filename
		# self.variables['eeta'] = read_binary_file("eeta")

		f = open(filename, 'rb')
		
		f.close()

		return pyKameleon.FileReader.OK

def read_binary_file(variable_file):
	""" Reads an rcm binary file. All RCM data files include a label for each record before the variable data
		This is RCM's definition of the label structure (bytes sum to 240):
		    TYPE :: label_def
		       INTEGER (iprec)   :: intg (20)
		       REAL (rprec)      :: real (20)
		       CHARACTER(LEN=80) :: char  
		       """


	# dt = numpy.dtype([('name', np.str_, 16), ('grades', np.float64, (2,))])

	f = open()
	x = numpy.fromfile()

	vector = pyKameleon.vectorFloat()

	return vector

	'''

class RcmFortranFile(FortranFile):
	def _set_header_prec(self, prec):
		self._header_prec = prec

    def readReals(self, prec='f'):
        """Read in an array of real numbers.
        
        Parameters
        ----------
        prec : character, optional
            Specify the precision of the array using character codes from
            Python's struct module.  Possible values are 'd' and 'f'.
            
        """
        
        _numpy_precisions = {'d': numpy.float64,
                             'f': numpy.float32
                            }

        if prec not in self._real_precisions:
            raise ValueError('Not an appropriate precision')
            
        data_str = self.readRecord()
        num = len(data_str)/struct.calcsize(prec)
        numbers =struct.unpack(self.ENDIAN+str(num)+prec,data_str) 
        return numpy.array(numbers, dtype=_numpy_precisions[prec])

        '''


class Test_readRCM(unittest.TestCase):
	def test_open_check(self):
		test = RCM_reader('RCM.ini')
		directory = '/Users/apembrok/Downloads/RCM_data/hanna.ccmc.gsfc.nasa.gov/pub/out/ccmc/Lutz_Rastaetter_111813_IM_1/RCM_OUTPUT'
		filename = directory+'/rcmv'
		self.assertEqual(test.open(filename), pyKameleon.FileReader.OK)
		self.assertEqual(test.current_filename, filename)


def main():
	unittest.main()

if __name__ == '__main__':
	main()
