.. _Developers:

Developers
==========

This page is for developers who wish to compile the Kameleon libraries. 

Anaconda
--------
The easiest way to get dependencies and build Kameleon is to use anaconda. We recommend light-weight version of anaconda (miniconda) found `here <http://conda.pydata.org/miniconda.html>`_. 

.. highlight::console

Download the appropriate bash installer for your system and run it, following the prompts::

    bash Miniconda-latest-MacOSX-x86_64.sh

Download requirements::

    conda install cmake swig hdf5=1.8.15.1

Install the anaconda client to obtain our c++11-compatible dependencies.

    conda install anaconda-client bzip2=1.0

    conda install --override-channels -c asherp boost cdf36

On linux::

    conda install cmake gcc

On Mac OS::

    conda install clang

Download
--------

Kameleon-plus is hosted on `github <https://github.com/ccmc/ccmc-software>`_

You may download an archived version of the code here: :download:`Kameleon-plus-6.0_source <../download/Kameleon-plus-6.0_source.tar.bz2>`

.. highlight::console

Once downloaded, create a directory to store the source, unzip and unpack into it::

    mkdir Kameleon-plus-source
    bunzip2 Kameleon-plus-6.0_source.tar.bz2
    tar -xf Kameleon-plus-6.0_source.tar --directory Kameleon-plus-source

Requirements
------------ 
* `cmake <http://www.cmake.org/>`_ - minimum 2.8
* `cdf <http://cdaweb.gsfc.nasa.gov/pub/software/cdf/dist/cdf35_0_2/>`_ - 3.5
* `boost <http://www.boost.org/>`_ - 1.54.0
* `hdf5 <http://www.hdfgroup.org/HDF5/release/obtain5.html>`_ (optional)
* `SWIG <http://www.swig.org/>`_ minimum version 3.0.0 (optional - for python and java wrapper support)

Build
-----
.. highlight:: console

To minimize clutter, Kameleon-plus now uses an out-of-source system-independent build structure.
To build the library and example executables, make a fresh build directory::

    mkdir ~/Kameleon-plus-build
    cd ~/Kameleon-plus-build
    cmake /path/to/Kameleon-plus-source
    make -j8

This populates the build directory with Makefiles for Linux and Mac (or Visual Studio Files for Windows). 
The last line compiles the ccmc libraries and executables that make use of them.


.. _executables-table:

Executables
...........
The table below shows all executables created by the build process and a brief description of their function.

Note: The executable paths will be /path/to/Kameleon-plus-source/bin/examples/<language>.


+------------------+------------------------------+----------------------------------------------------------+
| language         |     executable               |   Description                                            |
+==================+==============================+==========================================================+
|     c++/         | kameleon_prog                | opens and samples kameleon supported Model               |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | time_interp                  | tests interpolation between time steps *                 |
+                  +------------------------------+----------------------------------------------------------+
|                  | tracer_prog                  | tests the field line tracer on supported models          |
+                  +------------------------------+----------------------------------------------------------+
|                  | integrator_prog              | tests field line integrator on supported models          |
+                  +------------------------------+----------------------------------------------------------+
|                  |coordinate_transformation_test| tests cxform's coordinate transformations *              |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | lfm_test                     | tests the reader and interpolator for LFM                |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | enlil_test                   | tests the reader/interpolator for Enlil                  |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | adapt3d_test                 | tests the reader/interpolator for Adapt3d                |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | open_ggcm_test               | tests the reader/interpolator for open-GGCM              |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | mas_test                     | tests the reader/interpolator for MAS                    |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | swmf_iono_test               | tests the reader/interpolator for SWMF ionosphere model  |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | CDFReader                    | tests the reader CDF file reader                         |
+------------------+------------------------------+----------------------------------------------------------+
|      c/          |filereader_compat_test        | tests c wrapper for filereader                           |
+                  +------------------------------+----------------------------------------------------------+
|                  | kameleon_compat_test         | tests c wrapper for kameleon                             |
+                  +------------------------------+----------------------------------------------------------+
|                  | time_interp_c                | tests c wrapper for time interpolator                    |
+                  +------------------------------+----------------------------------------------------------+
|                  | tracer_c                     | tests c wrapper for tracer                               |
+------------------+------------------------------+----------------------------------------------------------+
|     fortran/     | generalfilereader_compat_f   | tests FORTRAN wrapper for general file reader            |
+                  +------------------------------+----------------------------------------------------------+
|                  | kameleon_compat_f            | tests FORTRAN access to models through Kameleon          |
+                  +------------------------------+----------------------------------------------------------+
|                  | timeinterp_compat_f          | tests FORTRAN compatibility for time interpolator        |
+                  +------------------------------+----------------------------------------------------------+
|                  | tracer_compat_f              | tests FORTRAN compatibility for field line tracer        |
|                  +------------------------------+----------------------------------------------------------+
|                  | time_series_test             |saves time series of multiple variables to an output file |
+------------------+------------------------------+----------------------------------------------------------+
|     python/      | kameleon_test.py             | tests python access to kameleon objects                  |
+                  +------------------------------+----------------------------------------------------------+
|                  | tracer.py                    | tests python access to field line tracer                 |
+------------------+------------------------------+----------------------------------------------------------+

    \* - indicates the example needs work!

.. _libraries-table:

Libraries
.........

The build will also generate the following platform-dependent libraries (names corresponding to a Mac build shown here). The example programs automatically link to these. The library paths will begin with /path/to/Kameleon-plus-source/lib

+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+
|  library path          |        library name                        |   Description                                                               |
+========================+============================================+=============================================================================+
|      ccmc/             |     libccmc.a                              |   main ccmc library containing model readers, interpolators, and tools      |
+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+
|      ccmc/c/           |   libccmc_wrapper_c.a                      |  c wrapper for ccmc library                                                 |
+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+
|      ccmc/fortran/     | libccmc_wrapper_fortran.a                  |  fortran wrapper for ccmc library                                           |
+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+
|      ccmc/java/        | libkameleon_plus-wrapper-java-6.0.0.jnilib |java wrapper for ccmc library (lib<name>.so for linux,<name>.dll for windows)|
+                        +--------------------------------------------+-----------------------------------------------------------------------------+
|                        | kameleon_plus-native-Darwin-6.0.0.jar      |  platform-specific java library (Darwin on mac)                             |
+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+
|      ccmc/python/CCMC  | CCMC.py, _CCMC.so                          |python module - "make install" will install to system's python libraries     |
+------------------------+--------------------------------------------+-----------------------------------------------------------------------------+


Build Flags
...........

To control the build process, the following flags may be invoked when running cmake from your build directory::

    cmake -D<flag_1>=ON -D<flag_2>=OFF -D<flag_3> /path/to/Kameleon-plus-source

+------------------+--------+--------------------------------------------------------------------------+
|    <flag>        | default|   Description                                                            |
+------------------+--------+--------------------------------------------------------------------------+
| BUILD_SHARED_LIBS|   ON   | build shared ccmc library  (libccmc.dylib, libccmc.so, or ccmc.dll)      |
+------------------+--------+--------------------------------------------------------------------------+
| BUILD_STATIC_LIBS|   ON   | build static ccmc library  (libccmc_static.a)                            | 
+------------------+--------+--------------------------------------------------------------------------+
| USE_STATIC_LIBS  |   OFF  | link against the static version of any required libraries when building  |
+------------------+--------+--------------------------------------------------------------------------+
| BUILD_HDF5       |   ON   |  Build with HDF5 library - (will ignore if HDF5 is not found)            |
+------------------+--------+--------------------------------------------------------------------------+
|CMAKE_MACOSX_RPATH|   OFF  | On Mac, build shared libs without @rpath                                 |
+------------------+--------+--------------------------------------------------------------------------+                     

These flags will be saved in your build directory in the file CMakeCache.txt. If you edit this file, the new values will be used next time you run cmake. See :ref:`trouble_shooting` for potential conflicts between these flags.


Install
-------
Libraries
.........
.. highlight:: sh

To install the compiled libraries on your platform (/usr/local/lib/ccmc on Mac)::

    cd path/to/Kameleon-plus-build
    make install

.. _python_module:

Python Module
.............
.. highlight:: python

You can load the CCMC python library by placing the following at the top of your script::
    
    import sys
    sys.path.append('path/to/Kameleon-plus-source/lib/ccmc/python/CCMC')
    import CCMC as ccmc

.. highlight:: sh

Alternatively, you can install the CCMC module into your system's python extensions::

    cd path/to/Kameleon-plus-source/lib/ccmc/python
    sudo python setup.py install

Test that install worked::

    python -c "from CCMC import _CCMC as ccmc"

If this command segfaults, try this :ref:`python_module_segFaults`.


Tests
-----
.. highlight:: sh

Kameleon has been tested successfully on the following platforms:

+------------+-----------------+--------------------------------------+------------------------------------------------------------------------+
| platform   |    OS           |  Compilers                           |   Notes                                                                |
+============+=================+======================================+========================================================================+
|  mac OSX   |  Darwin-12.5.0  | Clang 5.1.0 (C,C++), gfortran        |                                                                        |
+------------+-----------------+--------------------------------------+------------------------------------------------------------------------+
|  linux     |scientific linux | gnu                                  |                                                                        |
+------------+-----------------+--------------------------------------+------------------------------------------------------------------------+
|  windows   |  cygwin         | gnu                                  | apt-cyg for requirements. export PATH=/usr/local/bin/:/usr/bin/:/bin/  |
+------------+-----------------+--------------------------------------+------------------------------------------------------------------------+


Automated testing will be available soon. In the meantime, try running one of the basic c++ examples: 

The following commands read from a cdf file, interpolate and print the z-component of the magnetic field at the coordinates -30 0 0 in the model's coordinate system::

    cd path/to/kameleon-plus-source/bin/examples/c++
    ./kameleon_prog /path/to/ccmc-converted/file.cdf bz -30 0 0

.. highlight:: python


Example python program::

    import CCMC
    kameleon = CCMC.Kameleon()
    kameleon.open("/path/to/ccmc-converted/file.cdf")
    kameleon.loadVariable("bz")
    interpolator = kameleon.createNewInterpolator()
    bz = interpolator.interpolate("bz",25,0,0)
    print "bz:", bz
    kameleon.close()


Much of Kameleon's functionality is illustrated in several examples accross multiple languages. See :ref:`Full_Examples` to get started. 

.. _trouble_shooting:

Trouble-shooting
----------------
Depending on the platform, you may run into issues during the configuration or build process. In almost all cases, you will need to rebuild from a fresh build directory (or at least delete the CMakeCache.txt in your current one). 

CMake Error: Permission denied to make directories in /usr/local
................................................................
When running cmake, an attempt will be made to create directories in /usr/local. If you do not have write access there, you can specify an alternate install location where you do have write permissions::

    cmake -DCMAKE_INSTALL_PREFIX=/path/to/alternate


CMake Error: Libraries not found
................................
Cmake may be unable to find the required libraries if they are installed in unusual places. If so, you can set the paths to required libraries manually::

    cmake -DBOOST_ROOT=/path/to/boost -DCDF_PATH=/path/to/CDF -DHDF5_ROOT=/path/to/HDF5 /path/to/Kameleon-plus-source

Build Error: -fPIC errors
.........................
This error can occur when linking. This is due to the static libraries not being compiled with the -fPIC flag. You can either rebuild using::

    cmake -DBUILD_STATIC_LIBS=OFF /path/to/Kameleon-plus-source

or manually set the -fPIC flags::

    cmake -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC /path/to/kameleon/source


Build Error: duplicate symbols related to zlib
..............................................
This appears to be due to conflicts between the static HDF5 and CDF libraries both using zlib.
As a work-around, turn off the HDF5 static library::

    cmake -DHDF5_USE_STATIC_LIBRARIES=OFF /path/to/kameleon/source

.. _python_module_segFaults:

Python Error: ccmc module segFaults on load
...........................................
After installing the :ref:`python_module` and testing::

    python -c "from CCMC import _CCMC as ccmc"

If this command segfaults, it means the ccmc module is linked against a different python library from the command line python. 
To fix this, follow these steps:

1. Run the following command to determine which library your system's python is linked to::
    
    otool -L /path/to/system/python (mac)
    ldd /path/to/system/python (linux)

This will print /path/to/system/python/library/Python

2. Rebuild Kameleon-plus::

    cd path/to/Kameleon-plus-build
    cmake -DPYTHON_LIBRARY=/path/to/system/python/library/Python path/to/Kameleon-plus-source
    make -j8

3. Reinstall

_CCMC module build error: cannot find python.swig, typemaps.i
.............................................................
You may see this error if you installed swig with macports. 

Solution: install swig-python::
    
    sudo port install swig-python


MacOS rpath issues after linking
................................
By default, cmake should build without rpath. Solution: make this behavior explicit by setting the cmake flag::

    cmake -DCMAKE_MACOSX_RPATH=OFF

See documentation for CMAKE_MACOSX_RPATH `here <http://www.cmake.org/cmake/help/v3.0/variable/CMAKE_MACOSX_RPATH.html>`_  and  more on rpath `here <http://www.cmake.org/Wiki/CMake_RPATH_handling>`_.



Building with Homebrew
----------------------
You may have problems getting cmake to recognize homebrew's python. If so, consider tapping Nikolaus Demmel's cmake, which patches FindPythonLibs::

    brew install nikolausdemmel/devel/cmake

His trick is to first find the python interpreter, then call distutils within python to find the corresponding headers and libs.



Building Programs with Kameleon-Plus
------------------------------------
You can use CMake to build on top of the Kameleon libraries.

1. Write your program using the :ref:`Full_Examples` as a guide.
2. Copy your program's source code to the appropriate example language directory::

    cp my_program.cpp /path/to/Kameleon-plus-source/src/example/c++

.. highlight::cmake

3. Edit the corresponding CMakeLists file (i.e. src/example/c++/CMakeLists.txt) and add the lines::

    add_executable(my_program my_program.cpp)
    include_directories(/path/to/custom/includes)
    target_link_libraries(my_program ccmc)

For a custom fortran program, the process is the same except for the link line::
    
    target_link_libraries(my_program ccmc_wrapper_fortran)

.. highlight::console

4. Make a fresh build directory, then rebuild Kameleon-plus::

    cd path/to/Kameleon-plus-build
    cmake path/to/Kameleon-plus-source
    make -j8

Your custom programs should now appear in Kameleon-plus-source/bin/examples/<language>

IDE support
-----------
When running cmake from your build directory, use the -G flag, e.g.::

    -G "Sublime Text 2 - Unix Makefiles"
    -G "Xcode"

Run cmake -help for a list of project generators (eclipse, ninja, etc.)

