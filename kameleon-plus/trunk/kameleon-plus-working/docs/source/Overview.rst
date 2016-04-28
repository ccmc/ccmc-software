Overview
========
Kameleon is a software suite that is being developed at the CCMC to address the difficulty in analyzing and disseminating the varying output formats of space weather model data. Through the employment of a comprehensive data format standardization methodology, Kameleon allows heterogeneous model output to be stored uniformly in a common science data format. The converted files contain both the original model output as well as additional metadata elements to create platform independent and self-descriptive data files. To facilitate model data dissemination, data reuse, and code reuse – the Kameleon access and interpolation library provides direct access to both the model data as well as the embedded metadata.

In a nutshell, the Kameleon converter reads model output data and re-writes the data into a standard format. The Kameleon access/interpolation library reads data already converted by Kameleon providing a high level interface to the data. 

To get started right away, go to the :ref:`Quick_start` page. 


Kameleon Software Suite
-----------------------
The Kameleon Software Suite began life as two packages written in C. The Kameleon Converter Package and the Kameleon Access/Interpolation Library. Both converter and interpolator packages evolved, and were eventually re-written with successor distributions now in JAVA and C++ respectively.

The suite now consists of:

- Original `Kameleon Converter ( C ) <https://github.com/ccmc/ccmc-software/tree/master/kameleon_converter/tags/kameleon-converter-v5.2.0>`_
- Generation 2 `KameleonJ/Kameleon6 Converter ( JAVA ) <http://ccmc.gsfc.nasa.gov/downloads/KameleonConverter6.tgz>`_
- Original `Kameleon Interpolator ( C ) <https://github.com/ccmc/ccmc-software/tree/master/kameleon_interpolator/trunk/kameleon-interpolator-working1>`_
- Generation 2 `Kameleon+ Interpolator ( C++ ) <http://ccmc.gsfc.nasa.gov/downloads/Kameleon/Quick_start.html>`_


For Developers
..............

If you are a developer and want to add new model conversion functionality to the Kameleon Converter, you can use either the C or JAVA version depending on what language you are most comfortable with. The CCMC is investing primarily in KameleonJ/Kameleon6 development, but we are not opposed to new external development activities/branches that extend the utility, features, functionality of the original version.
If you have model(s) and/or data and are interested in writing directly to the Kameleon supported format, please make sure all standard global and variable meta-data is created and compatible data structures are used. Please note that a formal specification document is currently under development for this...
If you want to add new access/interpolation support to Kameleon, we recommend development using the Kameleon+ (C++) version of the library as a baseline. The Kameleon+ version is expected to receive the bulk of both CCMC and community support.

For Users
.........

If you already have Kameleon compatible data and want to read it, you should first verify that the KameleonJ/Kameleon6 version supports the model output you have. If it does, we recommend you use the KameleonJ/Kameleon6 version. However, if the original Kameleon access/interpolation library is the only version that supports the data you have and/or you are already comfortable with the original version, please feel free to use the original distribution.
* Please note that the CCMC is investing primarily in the new generations of KameleonJ/Kameleon6 and Kameleon+

Detailed Kameleon Information
.............................
Printable 2-page summary: 
`Kameleon Software <http://ccmc.gsfc.nasa.gov/downloads/kameleon.pdf>`_

Primary Features
----------------

- Model Readers and Interpolators for models hosted at the CCMC. For a list of supported models, see :ref:`Supported-Models`.  
- Model-specific field line integration
- Wrappers for C, Fortran, Python, and Java


News
----
- Model plug-ins: Python-embedded readers and interpolators
- Python Visualization tools
- Coordinate Transformation

Coming Soon
-----------
- Separatrix Analysis
- Dimensional analysis for Unit conversions and derived variables

Installation
------------

See :ref:`Quick_start` for cross-platform installation.
    
Tutorials
---------

The new :ref:`Coordinate_Transformations` tutorial illustrates coordinate-aware interpolation. Users may choose from a set of standard Heilospheric or Magnetospheric coordinate systems. Kameleon will automatically transform interpolation positions to the coordinate system of the underlying model.

The new :ref:`grid_tutorial` illustrates usage of the command-line tool grid.py, which interpolates variables onto a user-specified grid.

Contribute
----------
The Kameleon-plus source code is hosted on github: https://github.com/ccmc/ccmc-software

.. highlight:: console

To contribute, check out the CCMC source code::

	git clone https://github.com/ccmc/ccmc-software

The Kameleon-plus source is in the subdirectory **/kameleon-plus/trunk/kameleon-plus-working**


Support
-------

If you are having issues, please let us know.

Issue Tracker: https://github.com/ccmc/ccmc-software/issues

Lead developer: `Asher Pembroke <http://ccmc.gsfc.nasa.gov/staff/asher.php>`_

