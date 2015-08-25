# Notes #

All python examples in src/example/python should include the following lines at the top:

```python
import sys
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import _CCMC as ccmc
```

This allows cmake to insert the user's path to the CCMC module (_CCMC.so).
The resulting script will be placed in the /bin/examples/python directory.
