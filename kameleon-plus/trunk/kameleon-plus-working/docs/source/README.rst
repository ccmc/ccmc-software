Overview
========
Kameleon is a software suite that is being developed at the CCMC to address the difficulty in analyzing and disseminating the varying output formats of space weather model data. Through the employment of a comprehensive data format standardization methodology, Kameleon allows heterogeneous model output to be stored uniformly in a common science data format. The converted files contain both the original model output as well as additional metadata elements to create platform independent and self-descriptive data files. To facilitate model data dissemination, data reuse, and code reuse – the Kameleon access and interpolation library provides direct access to both the model data as well as the embedded metadata.

In a nutshell, the Kameleon converter reads model output data and re-writes the data into a standard format. The Kameleon access/interpolation library reads data already converted by Kameleon providing a high level interface to the data. 

To get started right away, go to the :ref:`Quick_start` page. 


Features
--------

- Model Readers and Interpolators for models hosted at the CCMC. For a list of supported models, see :ref:`Supported-Models`.  
- Model-specific field line integration
- Wrappers for C, Fortran, Python, and Java


Coming Soon
-----------
- Model plug-ins: Python-embedded readers and interpolators
- Support for Rice Convection Model
- Separatrix Analysis
- New SWMF Ionospheric interpolator
- Python Visualization tools
- ParaView and MayaVi support
- Dimensional analysis for Unit conversions and derived variables
- Simplicial Complex analysis on top of CGAL


Installation
------------

See :ref:`Quick_start` for cross-platform installation.
    

Contribute
----------
The Kameleon-plus source code is hosted on Google code: https://code.google.com/p/ccmc-software/

.. highlight:: console

To contribute, check out the CCMC source code::

	git clone https://code.google.com/p/ccmc-software/

The Kameleon-plus source is in the subdirectory **/kameleon-plus/trunk/kameleon-plus-working**


Support
-------

If you are having issues, please let us know.

Issue Tracker: https://code.google.com/p/ccmc-software/issues

CCMC Software Developers Google Group: https://groups.google.com/d/forum/ccmc-software

CCMC Software Developers Google Group Mail List: ccmc-software@googlegroups.com 

Lead developer: `Asher Pembroke <http://ccmc.gsfc.nasa.gov/staff/asher.php>`_

