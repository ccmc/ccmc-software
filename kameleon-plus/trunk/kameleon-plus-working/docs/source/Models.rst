.. _Supported-Models:

Supported Models
================
As of Version |version|, the following models are supported.


+-----------------------+------------+---------------+----------------------------------+
| Model                 | Reader     | Supported Variables                              |
+=======================+============+===============+==================================+
| :ref:`Adapt3D`        | Supported  | Supported                                        |
+-----------------------+------------+--------------------------------------------------+ 
| :ref:`BATS_R_US`      | Supported  | Supported                                        |
+-----------------------+------------+--------------------------------------------------+ 
| :ref:`ENLIL`          | Supported  | Supported                                        |
+-----------------------+------------+--------------------------------------------------+ 
| :ref:`LFM`            | Supported  | p,rho,ux,uy,uz,bx,by,bz,ex,ey,ez                 |
+-----------------------+------------+--------------------------------------------------+ 
| :ref:`Open_GGCM`      | Supported  | Supported                                        |
+-----------------------+------------+--------------------------------------------------+ 
| :ref:`MAS`            | Supported  | Supported                                        |
+-----------------------+------------+--------------------------------------------------+ 
|:ref:`SWMF_Ionosphere` | beta       | beta                                             |
+-----------------------+------------+--------------------------------------------------+ 



Accessing the Models
--------------------
The :ref:`Kameleon_Class` provides access to supported models through a common interface.

.. highlight:: python

For example, the following python code is model-independent (e.g. it may be used to access the BATS-R-US model *or* the LFM)::

    import CCMC
    kameleon = CCMC.Kameleon()
    kameleon.open("/path/to/ccmc-converted/file.cdf")
    kameleon.loadVariable("bz")
    interpolator = kameleon.createNewInterpolator()
    bz = interpolator.interpolate("bz",25,0,0)
    print "bz:", bz
    kameleon.close()

This encourages code re-use when comparing results from different models. See :ref:`Kameleon_Class_Example` for a complete example.

Alternatively, you may invoke a model's reader directly::

	import CCMC
	lfm = CCMC.LFM()
	lfm.open("/path/to/converted/lfm_file.cdf") #could also be "/path/to/converted/lfm_file.H5"
	lfm.loadVariable('bz')
	interpolator = lfm.createNewInterpolator()
	bz, dx, dy, dz = interpolator.interpolate_dc('bz',-30,0,0) # dx,dy,dz will store the local resolution
	print bz
	lfm.close()

This is useful for accessing model-specific variables and routines.

* :ref:`Models-at-a-glance` - broad descriptions of accessible models. 
* :ref:`Full_Examples` - more examples in C++,Python,Fortran.
* :ref:`Model_classes` - API and wrappers.