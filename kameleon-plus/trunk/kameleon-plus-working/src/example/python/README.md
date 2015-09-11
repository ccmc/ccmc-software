# Developer Note #

All python examples in src/example/python should include the following lines at the top:

```python
import sys
sys.path.append('@KAMELEON_LIB_DIR@/ccmc/python/CCMC/')
import _CCMC as ccmc
```

This allows cmake to insert the user's path to the CCMC module (_CCMC.so).
The resulting script will be placed in the /bin/examples/python directory.

# Running the new command-line interface #
All Python examples will eventually have a full-fledged command line interface to make it easier for users to leverage the powerful post-processing tools offered by Kameleon. The grid.py example is a good start. Once cmake is run and the _CCMC module is built, navigate to python examples

```console
cd ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/bin/examples/python
```

## Interrogating a file ##
If you want to see what global information is stored in the file, run
```console
python grid.py /path/to/ccmc/output/file.cdf -ginfo
```
This will yield all the global parameters: model name, run type, run time, etc.

To list all variables in the file, type:
```console
python grid.py /path/to/ccmc/output/file.cdf -lvar
```
To get information on a specific variable, type:
```console
python grid.py /path/to/ccmc/output/file.cdf -vinfo variable-name
```
where ```variable-name``` could be ```rho``` for density:
```console
rho [ amu/cm^3 ]
	valid_min : 0.0
	valid_max : 9.99999995904e+11
	units : amu/cm^3
	grid_system : grid_system_1
	mask : -1.09951162778e+12
	description : atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8
	is_vector_component : 0
	position_grid_system : grid_system_1
	data_grid_system : grid_system_1
	actual_min : 0.0992666035891
	actual_max : 25.3303718567
```

## Choosing variables for interpolation ##
To set which variables are to be interpolated, use
```console
python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3
```
where you could use ```rho p bx by bz``` in place of ```var1 var2 var3```
You will get an error if you try to choose a variable that does not exist.

## Setting interpolation positions ##
The positions will depend on the coordinate system of the underlying model. For magnetospheric runs, this usually means SM coordinates. **Unfortunately, knowing which coordinate system a model is in is not well documented!**

There are three ways to set your interpolation positions:
* Specifying a single position at command line
* Specifying a rectilinear cartesian grid
* Specifying an input file in columnar x,y,z format

### Specifying a single position ###
This is useful for verifying that you are in the right coordinate system and the results match what you would expect for your domain.
```console
python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3 -p -30 0 0
```
This returns the following table (actual values/variables depend on input):
```console
rho[amu/cm^3]        p[nPa]        bx[nT]        by[nT]        bz[nT]
       0.121        0.060        8.351       -0.824       -0.655
```
Interpolating outside the bounds of the model will result in junk data, most likely a large negative value.
You can also format the output of this using c-style syntax:

```console
python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3 -d '\t' -f "12.3f" -p -30 0 0 
```
Results in:
```console
rho[amu/cm^3]	       p[nPa]	       bx[nT]	       by[nT]	       bz[nT]
       0.121	       0.060	       8.351	      -0.824	      -0.655
```

### Specifying a grid ###
A grid may be generated in cartesian (or spherical?) coordinates. The grid resolution must be specified (ni,nj,nk). The resulting positions will be 'ij'.


## More Options ##
Run the python code with -h or --help
```console
python grid.py --help
```

The code will spit out a bunch of useful info:

```
usage: grid.py [-h] [-v] [-ginfo] [-lvar] [-vinfo var] [-vars var1 [var2 ...]]
               [-pfile /path/to/input/positions.txt] [-p px py pz]
               [-x xmin xmax] [-y ymin ymax] [-z zmin zmax] [-res nx [ny ...]]
               [-xint xint] [-yint yint] [-zint zint]
               [-t TRANSFORM TRANSFORM TRANSFORM] [-o path/to/output_file]
               [-f <flags><width><.precision><length>specifier] [-d ' ']
               [-ff fits [json ...]]
               full/path/to/input_file.cdf

Interpolates variables onto grid.

positional arguments:
  full/path/to/input_file.cdf
                        kameleon-compatible file

optional arguments:
  -h, --help            show this help message and exit
  -v, --verbose         verbosity of output
  -ginfo, --global-info
                        print global attributes

variable options:
  List and interrogate variables. Not all variables will have interpolator
  support.

  -lvar, --list-vars    list variables in the file (use -v to print all
                        variable attributes)
  -vinfo var, --variable-info var
                        print attributes for given variable
  -vars var1 [var2 ...], --variables var1 [var2 ...]
                        list of variables to be interpolated

input positions file options:
  File containing positions for interpolation

  -pfile /path/to/input/positions.txt, --positions_file /path/to/input/positions.txt
                        file containing column positions x, y, z. Valid
                        separators are ' ', <tab>, ','

point options:
  interpolation options for a single point

  -p px py pz, --point px py pz
                        point at which to interpolate variables

grid options:
  interpolation options for a grid of points

  -x xmin xmax, --x-range xmin xmax
                        range of x
  -y ymin ymax, --y-range ymin ymax
                        range of y
  -z zmin zmax, --z-range zmin zmax
                        range of z
  -res nx [ny ...], --resolution nx [ny ...]
                        resolution of the grid along each axis
  -xint xint, --x-intercept xint
                        fixes x for line or plane (ignores x-range)
  -yint yint, --y-intercept yint
                        fixes y for line or plane
  -zint zint, --z-intercept zint
                        fixes z for line or plane
  -t TRANSFORM TRANSFORM TRANSFORM, --transform TRANSFORM TRANSFORM TRANSFORM
                        transformation matrix to apply to grid before
                        interpolating

ouput options:
  where to store results of interpolation

  -o path/to/output_file, --output_file path/to/output_file
                        output file name and location
  -f <flags><width><.precision><length>specifier, --format <flags><width><.precision><length>specifier
                        c-sytle format of output variables (e.g. 12.3f)
  -d ' ', --delimiter ' '
                        delimiter for ascii output (default is ' ')
  -ff fits [json ...], --file_format fits [json ...]
                        File format for output. default: 'txt' for ASCII. Use
                        'fits' for binary IDL fits file (requires astropy), or
                        'json'
```
