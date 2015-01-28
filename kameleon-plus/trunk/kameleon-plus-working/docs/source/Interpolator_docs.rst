Interpolator Classes
====================


Model-Independent Classes
-------------------------
Interpolator
............
.. doxygenclass:: ccmc::Interpolator
	:members:

KameleonInterpolator
....................
.. doxygenclass:: ccmc::KameleonInterpolator
	:members:

TimeInterpolator
................
.. doxygenclass:: ccmc::TimeInterpolator
	:members:
	:undoc-members:

Point classes
.............
Point and Point3f are two different implementations of a point class. Point uses templating and leaves the operator implementation to the Vector subclass. Point3f is for floats only, but includes the operators. Point3f is used extensively by the :ref:`Fieldline_class` class.

.. doxygenclass:: ccmc::Point
	:members:
	:undoc-members:

.. doxygenclass:: ccmc::Point3f
	:members:
	:undoc-members:

Vector
......
.. doxygenclass:: ccmc::Vector
	:members:
	:undoc-members:

Polyhedron
..........
This class implements `Spherical Barycentric Coordinates <http://dl.acm.org/citation.cfm?id=1281968>`_ for interpolation. In the limit that a query point approaches one of the faces, the scheme converges to `Mean Value Coordinates <http://dl.acm.org/citation.cfm?id=775496>`_.

.. doxygenclass:: ccmc::Polyhedron
	:members:

Nanoflann Classes
.................
The `Nanoflann <https://github.com/jlblancoc/nanoflann>`_ header-only library is used for aiding in search queries. 

.. doxygenstruct:: PointCloud
	:members:
	:undoc-members:

.. doxygenclass:: nanoflann::KNNResultSet
	:members:
	:undoc-members:

.. doxygenclass:: NanoKdTree
	:members:
	:undoc-members:

.. _Fieldline_class:

Fieldline
.........
.. doxygenclass:: ccmc::Fieldline
	:members:
	:undoc-members:



Model-specific Interpolators
----------------------------
All Kameleon Interpolators inherit from the Interpolator class

Adapt3D
.......
.. doxygenclass:: ccmc::Adapt3DInterpolator
	:members:

BATSRUS
.......
.. doxygenclass:: ccmc::BATSRUSInterpolator
	:members:

.. _LFM_Interpolator:

LFM
...
.. doxygenclass:: ccmc::LFMInterpolator
	:members:

ENLIL
.....
.. doxygenclass:: ccmc::ENLIL
	:members:


MAS
...
.. doxygenclass:: ccmc::MASInterpolator
	:members:

OpenGGCM
........
.. doxygenclass:: ccmc::OpenGGCMInterpolator
	:members:

SWMF Ionosphere
...............
.. doxygenclass:: ccmc::SWMFIonoInterpolator
	:members:



