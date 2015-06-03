import os
import glob

__all__ = []
for f in glob.glob(os.path.dirname(__file__)+"/*.py"):
	if not (os.path.basename(f).startswith('_') | os.path.basename(f).startswith('generate') 
		| os.path.basename(f).startswith('GeneralPyReader.py')):
		if os.path.isfile(f):
			__all__.append(os.path.basename(f)[:-3])

# print __all__