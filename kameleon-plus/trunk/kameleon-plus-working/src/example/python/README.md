# Overview #

We provide a python api for the kameleon library via swig. Tutorials for python examples may be found in ipython notebooks contained in this directory.

# Developer Note #
In order to load the correct CCMC module, all python examples in src/example/python should include the following lines at the top:

```python
import sys
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import _CCMC as ccmc
```
This allows cmake to insert the user's path to the CCMC module (_CCMC.so).
The resulting script will be placed in the /bin/examples/python directory and will look like this:
```python
import sys
sys.path.append('/path/to/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/python/CCMC')
import _CCMC as ccmc
```
