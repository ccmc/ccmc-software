#1/usr/bin/python
""" Given an input file name, looks for a setup file, 
	then calls the python reader appropriate for the model"""
import sys
sys.path.append('build')
sys.path.append('..')
import pyReader
from pyreaders import *

# subclasses = pyReader.FileReader.__subclasses__()


class General_PyReader(pyReader.FileReader):
	def __init__(self):
		# This line is required to call the base class constructor
		super(testReader.pyFileReader, self).__init__()

		for classes in itersubclasses(pyReader.FileReader):
			print classes.__name__

		self = getClass('testPyFileReader').__init__()

	def openFile(self, filename, readonly = True):
		print 'looking for file'


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


def getClass(reader):
	for subcls in itersubclasses(pyReader.FileReader):
		if subcls.__name__ == reader:
			print 'selecting', reader,'class'
			return subcls


testGeneral = General_PyReader()
testGeneral.open('xymid.dat')
