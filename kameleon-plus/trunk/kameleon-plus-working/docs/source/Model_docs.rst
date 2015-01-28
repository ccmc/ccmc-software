Model Classes
=============
One of the core functions of Kameleon is to provide an interface to models hosted at the CCMC. The most direct way of working with the models is to access them through the C++ interface. Much of this functionality has also been made available through kameleon's wrapper libraries (see :ref:`wrappers`).

.. _Model_classes:

Model-Independent Classes
-------------------------
Attribute
.........
.. doxygenclass:: ccmc::Attribute
	:members:
	:undoc-members:

FileReader
..........
.. doxygenclass:: ccmc::FileReader
	:members:
	:protected-members:
	:undoc-members:

CDFFileReader
.............
.. doxygenclass:: ccmc::CDFFileReader
	:members:
	:protected-members:
	:undoc-members:

HDF5FileReader
..............
.. doxygenclass:: ccmc::HDF5FileReader
	:members:
	:protected-members:
	:undoc-members:

GeneralFileReader
.................
.. doxygenclass:: ccmc::GeneralFileReader
	:members: 
	:protected-members:
	:undoc-members:
	:no-link:

Model class
...........
.. doxygenclass:: ccmc::Model
	:members:
	:protected-members:
	:undoc-members:


Time
....
.. doxygenclass:: ccmc::Time
	:members:
	:undoc-members:

.. _Kameleon_Class:

Kameleon Class
..............
Depending on the application, one may use the more general Kameleon class to access any of the :ref:`Supported-Models`. 
This promotes code reuse when comparing results from different models. See :ref:`Kameleon_Class_Example` for a complete example.

.. doxygenclass:: ccmc::Kameleon
    :members:

Model-Dependent Classes
-----------------------
Adapt3D
.......
.. doxygenclass:: ccmc::Adapt3D
    :members:


BATSRUS
.......
.. doxygenclass:: ccmc::BATSRUS
	:members:

ENLIL
.....
.. doxygenclass:: ccmc::ENLIL
	:members:


LFM
...
.. doxygenclass:: ccmc::LFM
	:members:

MAS
...
.. doxygenclass:: ccmc::MAS
	:members:

OpenGGCM
.........
.. doxygenclass:: ccmc::OpenGGCM
	:members:


SWMF Ionosphere
...............
.. doxygenclass:: ccmc::SWMFIono
	:members:



