.. _Quick_start:

Quick Start
===========

Kameleon Installers
-------------------
These platform-dependent installers include pre-built binaries and executeables.


================   ==================   ==============================
   Platform         Installer             `Sha256 Hash <https://en.wikipedia.org/wiki/File_verification>`_
----------------   ------------------   ------------------------------
  MacOSX x86_64     Mac_installer_      **f294e1a3b51e2aca2ef50abd11670285f7f31e38a2750c8ea62f6d86a0fc652f**
----------------   ------------------   ------------------------------
  Linux x86_64      Linux_installer_    **3686ae0460d366ef3791cdfdbee141cac975f51963aecf09e299c6fd07429d91**
================   ==================   ==============================

Run the installer with bash::
    
    $ bash kameleon-6.1.0-MacOSX-x86_64.sh 

Then follow the prompts::

    Welcome to kameleon 6.1.1

    kameleon will now be installed into this location:
    <your/home/directory>/kameleon

      - Press ENTER to confirm the location
      - Press CTRL-C to abort the installation
      - Or specify an different location below

    [<your/home/directory>/kameleon] >>> 

After chosing your install location preference, you'll see the following output::

    PREFIX=/Users/apembrok/kameleon
    installing: python-2.7.11-0 ...
    Python 2.7.11 :: Continuum Analytics, Inc.
    installing: boost-1.59.0-py27_0 ...
    installing: bzip2-1.0.6-0 ...
    installing: cdf36-3.6.1-0 ...
    installing: cmake-3.3.1-0 ...
    installing: hdf5-1.8.15.1-2 ...
    installing: kameleon-6.1.0-py27_0 ...
    installing: openssl-1.0.2g-0 ...
    installing: pcre-8.31-0 ...
    installing: pip-8.1.1-py27_0 ...
    installing: readline-6.2-2 ...
    installing: setuptools-20.3-py27_0 ...
    installing: sqlite-3.9.2-0 ...
    installing: swig-3.0.8-1 ...
    installing: tk-8.5.18-0 ...
    installing: wheel-0.29.0-py27_0 ...
    installing: zlib-1.2.8-0 ...
    installation finished.

Finally, choose whether to prepend the kameleon install location to your system path.::

    Do you wish the installer to prepend the kameleon install location
    to PATH in your /Users/apembrok/.bash_profile ? [yes|no]
    [yes] >>> no

    You may wish to edit your .bashrc or prepend the kameleon install location:

    $ export PATH=/Users/apembrok/kameleon/bin:$PATH

    Thank you for installing kameleon!

Note that because the kameleon installer includes its own version of python, chosing to prepend kameleon's bin directory will cause kameleon's python to take precedence over your system python.


.. _executables-table:

Executables
-----------
The table below shows all executables packaged with the installer.

Note: The executable paths will be path/to/kameleon/bin/ccmc/examples/<language>.


+------------------+------------------------------+----------------------------------------------------------+
| language         |     executable               |   Description                                            |
+==================+==============================+==========================================================+
|     c++/         | kameleon_prog                | opens and samples kameleon supported Model               |
+                  +------------------------------+----------------------------------------------------------+ 
|                  | time_interp                  | tests interpolation between time steps                   |
+                  +------------------------------+----------------------------------------------------------+
|                  | tracer_prog                  | tests the field line tracer on supported models          |
+                  +------------------------------+----------------------------------------------------------+
|                  | integrator_prog              | tests field line integrator on supported models          |
+                  +------------------------------+----------------------------------------------------------+
|                  |coordinate_transformation_test| tests cxform's coordinate transformations                |
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
+                  +------------------------------+----------------------------------------------------------+
|                  | ARMS_test.py                 | tests python access to ARMS reader (python extension)    |
+                  +------------------------------+----------------------------------------------------------+
|                  | grid.py                      | basic grid interpolation/cut plane visualization         |
+                  +------------------------------+----------------------------------------------------------+
|                  | pyModel_test.py              | test of embedded python readers/interpolators            |
+------------------+------------------------------+----------------------------------------------------------+


.. _libraries-table:

Libraries
---------

The installer includes the following platform-dependent libraries (names corresponding to a Mac build shown here). The example programs automatically link to these. The library paths will begin with /path/to/kameleon/lib/ccmc. Headers are in /path/to/kameleon/include.

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

Test Data
---------
Download sample kameleon test data here.

+----------------+----------------+--------------------------+
|   Model name   |  model output  |         run id           |
+================+================+==========================+
| Enlil          | Enlil_output_  | Ailsa_Prise_101414_SH_1  |
+----------------+----------------+--------------------------+
| LFM            | LFM_output_    |                          |
+----------------+----------------+--------------------------+
| MAS            | MAS_output_    |                          |
+----------------+----------------+--------------------------+
| SWMF           |  SWMF_output_  |             Zheng        |
+----------------+----------------+--------------------------+
| OpenGGCM       |OpenGGCM_output_| Alexa_Halford_062105_2   |
+----------------+----------------+--------------------------+

.. _Python: http://www.python.org/
.. _Enlil_output: http://ccmc.gsfc.nasa.gov/downloads/sample_data/ENLIL.tar.bz2
.. _LFM_output: http://ccmc.gsfc.nasa.gov/downloads/sample_data/LFM.tar.bz2
.. _MAS_output: http://ccmc.gsfc.nasa.gov/downloads/sample_data/CORHEL_MAS.tar.bz2
.. _SWMF_output: http://ccmc.gsfc.nasa.gov/downloads/sample_data/SWMF.tar.bz2
.. _OpenGGCM_output: http://ccmc.gsfc.nasa.gov/downloads/sample_data/open_ggcm.tar.bz2
.. _Mac_installer: http://ccmc.gsfc.nasa.gov/downloads/kameleon_installers/kameleon-6.1.1-MacOSX-x86_64.sh
.. _Linux_installer: http://ccmc.gsfc.nasa.gov/downloads/kameleon_installers/kameleon-6.1.1-Linux-x86_64.sh

Download, untar and unzip any of the above test data, e.g.::

    wget http://ccmc.gsfc.nasa.gov/downloads/sample_data/ENLIL.tar.bz2
    tar -vxjf ENLIL.tar.bz2
