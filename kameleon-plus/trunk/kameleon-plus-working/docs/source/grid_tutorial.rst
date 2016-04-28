.. _grid_tutorial:

Grid Tutorial
=============

This document details usage of the grid.py command line tool.

All Python examples will eventually have a full-fledged command line
interface to make it easier for users to leverage the powerful
post-processing tools offered by Kameleon. The grid.py example is a good
start. Navigate to python examples

.. code:: console

    cd <path_to_kameleon>/bin/ccmc/examples/python

Interrogating global metadata from a file
-----------------------------------------

If you want to see what global information is stored in the file, run

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -ginfo

This will yield all the global parameters: model name, run type, run
time, etc.

To list all variables in the file, type:

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -lvar -v

The -v verbose flag will also print variable metadata. You can use -vv
for "very verbose" to get even more info.

Querying variable metadata
--------------------------

To get information on a specific variable, type:

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -vinfo variable-name

where ``variable-name`` could be ``rho`` for density:

.. code:: console

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

Choosing variables for interpolation
------------------------------------

To set which variables are to be interpolated, use

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3

where you could use ``rho p bx by bz`` in place of ``var1 var2 var3``
You will get an error if you try to choose a variable that does not
exist.

Setting interpolation positions
-------------------------------

The positions will depend on the coordinate system of the underlying
model. For magnetospheric runs, this usually means SM coordinates.
**Unfortunately, finding out which coordinate system is not yet
documented!**

There are three ways to set your interpolation positions: \* Specifying
a single position at command line \* Specifying a rectilinear cartesian
grid \* Specifying an input file in columnar x,y,z format

Specifying a single position
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is useful for verifying that you are in the right coordinate system
and the results match what you would expect for your domain.

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3 -p -30 0 0

This returns the following table (actual values/variables depend on
input):

.. code:: console

    rho[amu/cm^3]        p[nPa]        bx[nT]        by[nT]        bz[nT]
           0.121        0.060        8.351       -0.824       -0.655

Interpolating outside the bounds of the model will result in junk data,
most likely a large negative value. You can also format the output of
this using c-style syntax:

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf --variables var1 var2 var3 -d '\t' -f "12.3f" -p -30 0 0 

Results in:

.. code:: console

    rho[amu/cm^3]          p[nPa]          bx[nT]          by[nT]          bz[nT]
           0.121           0.060           8.351          -0.824          -0.655

Specifying a grid
~~~~~~~~~~~~~~~~~

A grid may be generated in cartesian (or spherical?) coordinates.
Parameters for specifying the grid are as follows: \*
``-res <ni> <nj> <nk>`` Grid resolution in each dimension \*
``-x <xmin> <xmax>.`` Inclusive range of x. If not specified, a constant
value of x-intercept = 0 will be used. \* ``-xint <x-intercept>``
overriedes default x-intercept. Ignored if ``-x`` is set. \*
``-y <xmin> <xmax> -yint <y-intercept> -z <zmin> <zmax> -zint <z-intercept>``
same as above. \* ``-order 'F'`` for Fortran-style column major.
Positions will be in row major by default.

Example 1 - Planar output
^^^^^^^^^^^^^^^^^^^^^^^^^

This command computes variables on a 2 x 3 plane at x = -30 with fortran
ordering:

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -xint -30 -y -10 10 -z -10 10 -order 'F' -res 1 2 3 -vars rho p bx by bz

Returns:

.. code:: console

               x            y            z rho[amu/cm^3]       p[nPa]       bx[nT]       by[nT]       bz[nT]
         -30.000      -10.000      -10.000        0.463        0.005      -14.188       -3.648       -1.768
         -30.000       10.000      -10.000        0.130        0.001      -14.626        0.292       -2.098
         -30.000      -10.000        0.000        0.522        0.055        9.560        1.758        0.508
         -30.000       10.000        0.000        0.397        0.043       10.131       -3.109        1.361
         -30.000      -10.000       10.000        0.504        0.006       14.860       -0.025       -0.062
         -30.000       10.000       10.000        0.191        0.003       15.025       -2.743        1.391

Example 2 - Volume output
^^^^^^^^^^^^^^^^^^^^^^^^^

Computing variables on a 2 x 2 x 2 grid in the tail with C ordering:

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -x -10 -50 -y -10 10 -z -10 10 -res 2 2 2 -vars rho p bx by bz

Returns:

.. code:: console

               x            y            z rho[amu/cm^3]       p[nPa]       bx[nT]       by[nT]       bz[nT]
         -10.000      -10.000      -10.000        0.956        0.020      -26.503      -12.704       -3.907
         -10.000      -10.000       10.000        0.651        0.011       29.393        8.146       -4.921
         -10.000       10.000      -10.000        0.366        0.007      -28.422        9.572       -6.825
         -10.000       10.000       10.000        0.361        0.010       29.279      -10.665        0.849
         -50.000      -10.000      -10.000        0.444        0.004      -10.964       -2.785       -2.127
         -50.000      -10.000       10.000        0.723        0.007       11.192       -0.474        0.179
         -50.000       10.000      -10.000        0.134        0.001      -11.449       -0.635       -2.087
         -50.000       10.000       10.000        0.243        0.003       11.774       -2.202        0.056

Output options
--------------

In the above examples, results were printed to console by default.
Alternatively, you may specify an output file in which to store the
results:

Exporting to ASCII
~~~~~~~~~~~~~~~~~~

.. code:: console

      python grid.py /path/to/ccmc/output/file.cdf -x -10 -50 -y -10 10 -z -10 10 -res 2 2 2 -vars rho p bx by bz -o /tmp/output_file.txt

By default, the results are stored as column ascii data using the same
format and delimitters as above.

Exporting to json
~~~~~~~~~~~~~~~~~

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -p -30 0 0 -vars rho -o /tmp/results.json -ff json

Exporting to IDL (`fits <http://idlastro.gsfc.nasa.gov/fitsio.html>`__ format)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: console

    python grid.py /path/to/ccmc/output/file.cdf -x -10 -50 -y -10 10 -z -10 10 -res 2 2 2 -o /tmp/fits_out -ff fits -vars rho p bx by bz

The results may be read into IDL. To print the global and variable
metadata from the cdf file:

.. code:: console

    IDL> results = MRDFITS('fits_out', 0, header)
    IDL> print, header

To extract the results of the interpolation:

.. code:: console

    IDL> results = MRDFITS('fits_out', 1, header)
    MRDFITS: Binary table.  8 columns by  8 rows.

    IDL> help, results,/str
    ** Structure <131f5238>, 8 tags, length=32, data length=32, refs=1:
       X               FLOAT          -10.0000
       Y               FLOAT          -10.0000
       Z               FLOAT          -10.0000
       RHO             FLOAT          0.955738
       P               FLOAT         0.0203208
       BX              FLOAT          -26.5025
       BY              FLOAT          -12.7045
       BZ              FLOAT          -3.90678

    IDL> print, results
    {     -10.0000     -10.0000     -10.0000     0.955738    0.0203208     -26.5025     -12.7045     -3.90678}{     -10.0000     -10.0000      10.0000     0.650893    0.0111245      29.3928
          8.14576     -4.92121}{     -10.0000      10.0000     -10.0000     0.365616   0.00686792     -28.4215      9.57153     -6.82492}{     -10.0000      10.0000      10.0000     0.361352
       0.00967156      29.2788     -10.6648     0.849321}{     -50.0000     -10.0000     -10.0000     0.444090   0.00394465     -10.9637     -2.78490     -2.12730}{     -50.0000     -10.0000
          10.0000     0.722521   0.00713427      11.1925    -0.474207     0.179418}{     -50.0000      10.0000     -10.0000     0.133836   0.00134037     -11.4489    -0.634802     -2.08698}{
         -50.0000      10.0000      10.0000     0.242857   0.00283294      11.7740     -2.20208    0.0556372}

More Options
------------

Run the python code with -h or --help

.. code:: console

    python grid.py --help

The code will spit out a bunch of useful info:

::

    usage: grid.py [-h] [-v] [-ginfo] [-lvar] [-vinfo var] [-vars var1 [var2 ...]]
                   [-pout positions_output_flag]
                   [-pfile /path/to/input/positions.txt] [-p px py pz]
                   [-x xmin xmax] [-y ymin ymax] [-z zmin zmax] [-res nx [ny ...]]
                   [-xint xint] [-yint yint] [-zint zint] [-order ordering]
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
      -pout positions_output_flag, --positions_out_flag positions_output_flag
                            pass interpolating positions to output

    input positions file options:
      File containing positions for interpolation

      -pfile /path/to/input/positions.txt, --positions_file /path/to/input/positions.txt
                            file containing column positions x, y, z. Optional
                            separators: ' ' (default), <tab>, ','

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
      -order ordering, --ordering ordering
                            sets ordering of output arrays. options: 'C' (default
                            - C-style row major) or 'F' (FORTRAN-style column
                            major)
      -t TRANSFORM TRANSFORM TRANSFORM, --transform TRANSFORM TRANSFORM TRANSFORM
                            transformation matrix to apply to grid before
                            interpolating (not implemented yet)

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
