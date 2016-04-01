.. _Coordinate_Transformations:

Coordinate Transformations
==========================

Kameleon now allows users to work in their preferred coordinate system
without requiring intimate knowledge of the underlying models. This
document gives some examples illustrating the new capabilities,
primarily using the kameleon python API.

Setup
-----

Make sure the \_CCMC module is compiled and placed in
lib/ccmc/python/CCMC.

Detailed instructions may be found
`here <https://github.com/ccmc/ccmc-software/blob/Coordinate_Transformations/kameleon-plus/trunk/kameleon-plus-working/Kameleon-Anaconda.ipynb>`__
(anaconda users) and
`here <http://ccmc.gsfc.nasa.gov/downloads/kameleon_instructions/html/Quick_start.html#quick-start>`__
(main Kameleon site).

Load the ccmc python api into a python environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once compiled, replace the path variable with your
/path/to/lib/ccmc/python/CCMC/

.. code:: python

    path = '/Users/apembrok/git/ccmc/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/lib/ccmc/python/CCMC/'
    import sys
    sys.path.append(path)
    import _CCMC as ccmc
    
Choose a kameleon-compatible file to work with
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kameleon-compatible model files may be obtained through our
`Runs-on-Request <http://ccmc.gsfc.nasa.gov/requests/requests.php>`__
system, by searching the database or requesting a run. Requested model
output will be automatically converted to a format the CCMC library
understands.

.. code:: python

    filename = '/Users/apembrok/Work/New_Horizons/nh_mhd/3D_CDF/nh3d_mhd.enlil.0348.cdf' #sample enlil
Working with kameleon objects
-----------------------------

Create a kameleon object that opens and interrogates the file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kameleon objects allow access to model attributes without needing to
know the particular structure of a given model. ### Global attributes

.. code:: python

    kameleon = ccmc.Kameleon()
    kameleon.open(filename)
    
    for i in range(kameleon.getNumberOfGlobalAttributes()):
        gname = kameleon.getGlobalAttributeName(i)
        gattr = kameleon.getGlobalAttribute(gname)
        if gname != 'README':
            print gname, gattr.toString()

.. parsed-literal::

    model_type STRING: model_type: Heliosphere
    grid_system_count STRING: grid_system_count: 1
    model_name STRING: model_name: enlil
    output_type STRING: output_type: Heliosphere
    grid_system_1 STRING: grid_system_1: [r,theta,phi]
    grid_1_type STRING: grid_1_type: HNM
    run_type STRING: run_type: stationary_Solar_Wind
    standard_grid_target STRING: standard_grid_target: HNM
    original_output_file_name STRING: original_output_file_name: /tim.0348.nc
    run_registration_number STRING: run_registration_number: nh3d_mhd
    terms_of_usage STRING: terms_of_usage: ***For tracking purposes of our government sponsors, we ask that you notify the CCMC 
    whenever you use CCMC results in scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov
    tim_type STRING: tim_type: TIM
    tim_title STRING: tim_title: Values at the given time level
    tim_program STRING: tim_program: enlil
    tim_version STRING: tim_version: 2.8
    tim_project STRING: tim_project: nh3d
    tim_code STRING: tim_code: reg40low1.256-mcp1umn1cd-1
    tim_model STRING: tim_model: mhd ethe splitted upwind llf minmod nototh difb
    tim_geometry STRING: tim_geometry: spherical
    tim_grid STRING: tim_grid: X1=0.1-40.1/uniform X2=50-130/uniform X3=0-360/uniform
    tim_coordinates STRING: tim_coordinates: HEEQ+180
    tim_rotation STRING: tim_rotation: sidereal
    tim_case STRING: tim_case: reg40low1.dtb-a6b1r-donki500cme6-sa1.20150601
    tim_cordata STRING: tim_cordata: wsadu
    tim_observatory STRING: tim_observatory: gongb
    tim_corona STRING: tim_corona: WSA_V2.2
    tim_crpos STRING: tim_crpos: 2164_219
    tim_shift_deg FLOAT: : 0
    tim_boundary STRING: tim_boundary: LLVV=3./1./700./0. RRtilt=25./0./0. NTxcld=4./1./1. NTrcav=1./4./0.
    tim_run STRING: tim_run: reg40low1.dtb-a6b1r-donki500cme6-sa1.256-mcp1umn1cd-1.g53q5.20150601
    tim_parameters STRING: tim_parameters: g=1.6666667 q=0.05/1 xa=0.05 rot=sidereal cfl=0.4 difb=0.2/
    tim_boundary_old STRING: tim_boundary_old: B=350./4./0./1 D=125./2. T=1.5/0 V=700./0./75./200. S=9. A=0.05
    tim_obsdate_mjd FLOAT: : 0
    tim_obsdate_cal STRING: tim_obsdate_cal: null
    tim_crstart_mjd FLOAT: : 0
    tim_crstart_cal STRING: tim_crstart_cal: 2015-05-21T09:13:14
    tim_rundate_mjd FLOAT: : 0
    tim_rundate_cal STRING: tim_rundate_cal: 2015-06-01
    tim_rbnd FLOAT: : 0
    tim_gamma STRING: : 
    tim_xalpha STRING: : 
    tim_mevo INT: tim_mevo: 71029
    tim_mfld INT: tim_mfld: 0
    tim_mslc INT: tim_mslc: 0
    tim_mtim INT: tim_mtim: 349
    tim_creation STRING: tim_creation: 2015-07-08T18:41:34
    grid_system_1_dimension_1_size INT: grid_system_1_dimension_1_size: 6400
    grid_system_1_dimension_2_size INT: grid_system_1_dimension_2_size: 20
    grid_system_1_dimension_3_size INT: grid_system_1_dimension_3_size: 90
    grid_system_1_number_of_dimensions INT: grid_system_1_number_of_dimensions: 3
    time_physical_time FLOAT: time_physical_time: 3758578.5
    time_physical_time_step FLOAT: time_physical_time_step: 274.08963
    time_numerical_time_step INT: time_numerical_time_step: 71028
    Conversion Time STRING: Conversion Time: 2015/07/09 16:37:34


Viewing variable attributes
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    for i in range(kameleon.getNumberOfVariables()):
        varname  = kameleon.getVariableName(i)
        min_attr = kameleon.getVariableAttribute(varname, 'actual_min').getAttributeFloat()
        max_attr = kameleon.getVariableAttribute(varname, 'actual_max').getAttributeFloat()
        units = kameleon.getVisUnit(varname)
        units2 = kameleon.getNativeUnit(varname)
        print varname, '\t', min_attr,'\t', max_attr, units, units2

.. parsed-literal::

    r 	15427500032.0 	5.99849225421e+12 m m
    theta 	0.907571196556 	2.23402142525 radian radian
    phi 	0.0349065847695 	6.24827861786 radian radian
    rho 	5.06677902223e-26 	1.75479584072e-18 kg/m3 kg/m3
    T 	121.367195129 	1424361.375 K 
    ur 	250993.953125 	717503.3125 km/s m/s
    utheta 	-29017.0996094 	73542.9765625 km/s m/s
    uphi 	-68833.5078125 	39320.1835938 km/s m/s
    br 	-1.32109281847e-10 	3.1329065564e-07 nT T
    btheta 	-3.10669090453e-09 	3.30375415913e-09 nT T
    bphi 	-5.49196386146e-08 	1.08497091519e-11 nT T
    dp 	0.0 	0.00549638364464 kg/m^3 kg/m^3
    bp 	-0.952782809734 	0.954018414021 1000000000000.0 1000000000000.0
    b1r 	-3.1329065564e-07 	1.32109281847e-10 nT nT
    b1theta 	-3.30375415913e-09 	3.10669090453e-09 nT nT
    b1phi 	-1.08497091519e-11 	5.49196386146e-08 nT nT


Loading variables
~~~~~~~~~~~~~~~~~

Usually you will need to load the variables you are interested in prior
to interpolation. There are two ways to confirm if a variable exists:

1. the doesVariableExist method
2. return value of getVariableID

.. code:: python

    # Enlil only stores br,btheta,bphi
    if kameleon.doesVariableExist('bx'): 
        print 'bx exists!?!?'
        
    br_id = kameleon.getVariableID('br')
    if br_id != -1:
        print 'variable br exists, loading'
        kameleon.loadVariable('br')

.. parsed-literal::

    variable br exists, loading


One can also load multiple components of vector variables in one call

.. code:: python

    kameleon.loadVectorVariable('b') #automatically loads br,btheta,bphi
    kameleon.loadVariable('p')
    kameleon.loadVariable('rho') 
    
    #Note: a bug causes loadVariable to always return true
    if kameleon.loadVariable('non-existent_variable'):
        print 'non-existent_variable should not be loaded' 

.. parsed-literal::

    non-existent_variable should not be loaded


Interpolation
-------------

Native Interpolation
~~~~~~~~~~~~~~~~~~~~

Native interpolation is performed by default, meaning that kameleon
assumes the user knows what coordinate system the underlying model is in
and will make no effort to convert query points. If you have used
kameleon before, this behavior should match what you're used to.

.. code:: python

    model_name = kameleon.getGlobalAttribute('model_name').getAttributeString()
    coordinates = kameleon.getGlobalAttribute('grid_1_type').getAttributeString()
    
    # HNM (radius[AU], longitude, latitude).
    print model_name, 'uses', coordinates
    
    #when intializing the interpolator with no arguments, we assume native interpolation
    interpolator = kameleon.createNewInterpolator() 
    
    # The new horizons run extends past pluto.
    point = 10.0,0,0  
    if br_id != -1:
        result = interpolator.interpolate(br_id, *point)
        print 'br',point,result,kameleon.getVisUnit('br')

.. parsed-literal::

    enlil uses HNM
    br (10.0, 0, 0) 0.0602672547102 nT


Preferred Coordinate Interpolation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To enable conversion between coordinate systems, create a coordinate
interpolator object.

-  If initialized without argument, native interpolation is assumed.
-  If initialized with the name of a registered coordinate system, user
   query points will be converted from that coordinate system to that of
   the underlying model prior to interpolation.
-  If initialized with an unregistered coordinate system, an error
   message will print and native interpolation will be used.

.. code:: python

    coordinate_interpolator = kameleon.createCoordinateInterpolator() #no arguments assumes native
Model Epoch Time
~~~~~~~~~~~~~~~~

The model epoch time will be used to convert between coordinate systems.

.. code:: python

    print model_name, 'epoch time:', coordinate_interpolator.getEphemTime(), 'seconds'

.. parsed-literal::

    enlil epoch time: 489230172 seconds


one may also change the epoch time if it is not set properly in the
file:

.. code:: python

    coordinate_interpolator.setEphemTime(200)
    print model_name, 'epoch time:', coordinate_interpolator.getEphemTime(), 'seconds'

.. parsed-literal::

    enlil epoch time: 200 seconds


Available transformations
-------------------------

Coordinate transformations may be performed from the following cartesian
geocentric and heliocentric coordinate systems. Note that because of the
length scales, it is helpful to think of geocenctric coordinates in R\_E
while Heliocentric are in km.

.. code:: python

    geo_coords = [  "J2000", #geocentric are in R_E
                    "GEI",
                    "GEO",
                    "MAG",
                    "GSE",
                    "GSM",
                    "SM",
                    "RTN",
                    "GSEQ",]
    
    helio_coords = [ "HEE", #heliocentric are in km
                    "HAE",
                    "HEEQ"] 
Example: Interpolating from SM coordinates:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    coordinate_interpolator = kameleon.createCoordinateInterpolator("SM")
    var = 'rho' #slightly slower, but interpolator may be called with string instead of variable ID
    print var,  coordinate_interpolator.getPreferredCoordinates(), point, 
    print coordinate_interpolator.interpolate(var, point[0],point[1],point[2]), 
    print kameleon.getVisUnit(var)

.. parsed-literal::

    rho SM (10.0, 0, 0) 1.83079636822e-17 kg/m3


note: this assumes that the variable is a scalar field! For example, if
you ask for br, there will be no attempt to rotate the result into your
preferred coordinate system - **the result will be the radial component
of Enlil's field in Enlil's coordinate system.**

If you want to check the conversion to model coordinates, you can use
the convertCoordinates method

.. code:: python

    coordinate_interpolator.setPreferredCoordinates("SM")
    point = 10, 0, 0 # SM cartesian [Re]
    
    query_point = ccmc.Position()
    query_point.c0, query_point.c1, query_point.c2 = point
    
    model_point = ccmc.Position()
    coordinate_interpolator.convertCoordinates(query_point,model_point)
    print coordinate_interpolator.get_model_coords(), 
    print model_point.c0, model_point.c1, model_point.c2 # HNM R[AU], latitude [deg], longitude [deg]

.. parsed-literal::

    HNM 1.01669931412 -3.13182067871 360.0


Example: Interpolating between multiple coordinate frames
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    from pandas import DataFrame as df
    import numpy as np
    from collections import defaultdict
    
    def get_coordinate_table(kameleon, coordinate_list, variable_names, point):
        if type(variable_names) != list: 
            variable_names = [variable_names]
        query_point = ccmc.Position()
        query_point.c0, query_point.c1, query_point.c2 = point
        coordinate_interpolator = kameleon.createCoordinateInterpolator()
        model_coordinates = coordinate_interpolator.get_model_coords()
        model_coordinates = str(point) + " ---> " + model_coordinates + '[AU, lat, lon]'
        missing_value = kameleon.getMissingValue()
        model_positions = []
        results = []
        table = defaultdict(list)
        for coord in coordinate_list:
            coordinate_interpolator.setPreferredCoordinates(coord)
            coordinate_interpolator.convertCoordinates(query_point,model_point)
            table[model_coordinates].append(np.array([model_point.c0, model_point.c1, model_point.c2]))
            for variable_name in variable_names:
                column_name = variable_name + '[' + kameleon.getVisUnit(variable_name) + ']'
                result = coordinate_interpolator.interpolate(variable_name, *point)
                if result == missing_value: 
                    table[column_name].append(np.NAN)
                else:
                    table[column_name].append(result)
        table = df(table) #make a pretty pandas table
        table.index = coordinate_list
        return table
The above code makes a nice looking pandas data table from results of
the interpolation in each of the available coordinate systems. Here are
the results for geocentric coordinates.

.. code:: python

    geo_table = get_coordinate_table(kameleon,geo_coords,['br', 'rho', 'p'], (10, 0, 0))
    geo_table



.. raw:: html

    <div>
    <table border="1" class="dataframe">
      <thead>
        <tr style="text-align: right;">
          <th></th>
          <th>(10, 0, 0) ---&gt; HNM[AU, lat, lon]</th>
          <th>br[nT]</th>
          <th>p[nPa]</th>
          <th>rho[kg/m3]</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <th>J2000</th>
          <td>[1.01669943333, -3.13182067871, 3.67431243831e...</td>
          <td>7.144619</td>
          <td>896249.1250</td>
          <td>1.830797e-17</td>
        </tr>
        <tr>
          <th>GEI</th>
          <td>[1.01669943333, -3.13182067871, 3.67120128431e...</td>
          <td>7.144619</td>
          <td>896249.1250</td>
          <td>1.830797e-17</td>
        </tr>
        <tr>
          <th>GEO</th>
          <td>[1.01669943333, -3.13182830811, 360.0]</td>
          <td>7.144630</td>
          <td>896253.7500</td>
          <td>1.830796e-17</td>
        </tr>
        <tr>
          <th>MAG</th>
          <td>[1.01669943333, -3.13182067871, 9.96583594315e...</td>
          <td>7.144619</td>
          <td>896249.1250</td>
          <td>1.830797e-17</td>
        </tr>
        <tr>
          <th>GSE</th>
          <td>[1.01669931412, -3.13182067871, 360.0]</td>
          <td>7.144633</td>
          <td>896250.1250</td>
          <td>1.830796e-17</td>
        </tr>
        <tr>
          <th>GSM</th>
          <td>[1.01669931412, -3.13182067871, 360.0]</td>
          <td>7.144633</td>
          <td>896241.0625</td>
          <td>1.830796e-17</td>
        </tr>
        <tr>
          <th>SM</th>
          <td>[1.01669931412, -3.13182067871, 360.0]</td>
          <td>7.144633</td>
          <td>896250.7500</td>
          <td>1.830796e-17</td>
        </tr>
        <tr>
          <th>RTN</th>
          <td>[1.01669943333, -3.13182067871, 360.0]</td>
          <td>7.144630</td>
          <td>896250.1250</td>
          <td>1.830796e-17</td>
        </tr>
        <tr>
          <th>GSEQ</th>
          <td>[1.01669931412, -3.13182067871, 360.0]</td>
          <td>7.144633</td>
          <td>896250.8750</td>
          <td>1.830796e-17</td>
        </tr>
      </tbody>
    </table>
    </div>



The results are not so senstive to the change in coordinates because
ENLIL's scale is much larger than the differences in geocentric
coordinate systems. We see larger changes for heliocentric coordinate
interpolations:

.. code:: python

    helio_table = get_coordinate_table(kameleon,helio_coords,['br', 'rho', 'p'], (1.5e8, 0, 0))
    helio_table



.. raw:: html

    <div>
    <table border="1" class="dataframe">
      <thead>
        <tr style="text-align: right;">
          <th></th>
          <th>(150000000.0, 0, 0) ---&gt; HNM[AU, lat, lon]</th>
          <th>br[nT]</th>
          <th>p[nPa]</th>
          <th>rho[kg/m3]</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <th>HEE</th>
          <td>[1.00268721581, -3.13182067871, 360.0]</td>
          <td>7.523039</td>
          <td>1.208926e+24</td>
          <td>1.831418e-17</td>
        </tr>
        <tr>
          <th>HAE</th>
          <td>[1.00268721581, -7.03282165527, 78.6580734253]</td>
          <td>2.221460</td>
          <td>1.208926e+24</td>
          <td>7.569455e-18</td>
        </tr>
        <tr>
          <th>HEEQ</th>
          <td>[1.00268721581, 0.0, 0.0]</td>
          <td>7.432927</td>
          <td>1.208926e+24</td>
          <td>1.883335e-17</td>
        </tr>
      </tbody>
    </table>
    </div>



Tolerance considerations
------------------------

There is a significant loss of precision when converting between helio
and geocentric coordinates, due to the vastly different length scales.
As a result, if one attempts to convert a position from helio to
geocentric and back, you will incur an error of order 1. The code below
is meant to illustrate this.

.. code:: python

    from pandas import DataFrame as df
    import numpy as np
    from collections import defaultdict
    def test_conversion(start_coord, start_point, to_coords, time_et):
        table = defaultdict(list)
    
        to = ccmc.Position()
        back = ccmc.Position()
    
        start = ccmc.Position()
        start.c0, start.c1, start.c2 = start_point
    
        start_point = np.array([start.c0,start.c1,start.c2])
        print "converting", start_point, "from", start_coord, "to new point and back to", start_coord
        for t in to_coords:
            kameleon._cxform(start_coord,t, time_et, start,to)
            to_vec = np.array([to.c0,to.c1,to.c2])
            table["new_point"].append(to_vec)
    
            kameleon._cxform(t,start_coord, time_et, to, back)
            back_vec = np.array([back.c0,back.c1,back.c2])
            table["return_point in " + start_coord].append(back_vec)
            table["error"].append(np.linalg.norm(start_point - back_vec))
    
        table = df(table)
        table = table[["new_point","return_point in " + start_coord,"error"]]
    #     table = df([table["new_point"], table["return_point"], table["error"]], index = to_coords)
        table.index = to_coords
        return table  
.. code:: python

    test_conversion("GSE", start_point = (10, 0, 0), 
                    to_coords = geo_coords + helio_coords, 
                    time_et = 0)

.. parsed-literal::

    converting [ 10.   0.   0.] from GSE to new point and back to GSE




.. raw:: html

    <div>
    <table border="1" class="dataframe">
      <thead>
        <tr style="text-align: right;">
          <th></th>
          <th>new_point</th>
          <th>return_point in GSE</th>
          <th>error</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <th>J2000</th>
          <td>[1.80101644993, -9.02481365204, -3.91268110275]</td>
          <td>[10.0, 7.8837850026e-08, 3.2981541942e-09]</td>
          <td>7.890681e-08</td>
        </tr>
        <tr>
          <th>GEI</th>
          <td>[1.80101644993, -9.02481365204, -3.91268110275]</td>
          <td>[10.0, 7.8837850026e-08, 3.2981541942e-09]</td>
          <td>7.890681e-08</td>
        </tr>
        <tr>
          <th>GEO</th>
          <td>[9.20181369781, 0.132480040193, -3.91268110275]</td>
          <td>[10.0, 1.26621149121e-08, 1.06196843319e-07]</td>
          <td>1.069490e-07</td>
        </tr>
        <tr>
          <th>MAG</th>
          <td>[3.44704961777, 8.77182579041, -3.34259176254]</td>
          <td>[10.0, -9.95483517841e-08, 3.51740894189e-08]</td>
          <td>1.055798e-07</td>
        </tr>
        <tr>
          <th>GSE</th>
          <td>[10.0, 0.0, 0.0]</td>
          <td>[10.0, 0.0, 0.0]</td>
          <td>0.000000e+00</td>
        </tr>
        <tr>
          <th>GSM</th>
          <td>[10.0, 0.0, 0.0]</td>
          <td>[10.0, 0.0, 0.0]</td>
          <td>0.000000e+00</td>
        </tr>
        <tr>
          <th>SM</th>
          <td>[9.42481231689, 0.0, -3.34259176254]</td>
          <td>[10.0, -2.07942907338e-08, 1.94201689396e-07]</td>
          <td>1.953118e-07</td>
        </tr>
        <tr>
          <th>RTN</th>
          <td>[-10.0, 0.0, 0.0]</td>
          <td>[10.0, 0.0, 0.0]</td>
          <td>0.000000e+00</td>
        </tr>
        <tr>
          <th>GSEQ</th>
          <td>[10.0, 0.0, 0.0]</td>
          <td>[10.0, 0.0, 0.0]</td>
          <td>0.000000e+00</td>
        </tr>
        <tr>
          <th>HEE</th>
          <td>[147101264.0, -1.22464679965e-15, 0.0]</td>
          <td>[15.5077991486, -1.80147079476e-08, 0.0]</td>
          <td>5.507799e+00</td>
        </tr>
        <tr>
          <th>HAE</th>
          <td>[-26493180.0, 144695872.0, 0.0]</td>
          <td>[8.40400218964, 0.419026881456, 0.0]</td>
          <td>1.650089e+00</td>
        </tr>
        <tr>
          <th>HEEQ</th>
          <td>[146897904.0, 3.72529029846e-09, -7732361.0]</td>
          <td>[9.60768127441, 0.000253342703218, 0.002190539...</td>
          <td>3.923249e-01</td>
        </tr>
      </tbody>
    </table>
    </div>



.. code:: python

    test_conversion("HEEQ", start_point = (1, 0, 0), 
                    to_coords = geo_coords + helio_coords, 
                    time_et = 0)

.. parsed-literal::

    converting [ 1.  0.  0.] from HEEQ to new point and back to HEEQ




.. raw:: html

    <div>
    <table border="1" class="dataframe">
      <thead>
        <tr style="text-align: right;">
          <th></th>
          <th>new_point</th>
          <th>return_point in HEEQ</th>
          <th>error</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <th>J2000</th>
          <td>[26493182.0, -132756168.0, -57556040.0]</td>
          <td>[-0.404457479715, 0.185022413731, 1.50997173786]</td>
          <td>2.070447e+00</td>
        </tr>
        <tr>
          <th>GEI</th>
          <td>[26493182.0, -132756168.0, -57556040.0]</td>
          <td>[-0.404457479715, 0.185022413731, 1.50997173786]</td>
          <td>2.070447e+00</td>
        </tr>
        <tr>
          <th>GEO</th>
          <td>[135359856.0, 1948798.25, -57556040.0]</td>
          <td>[0.934973955154, 0.0493004247546, 0.860102057457]</td>
          <td>8.639644e-01</td>
        </tr>
        <tr>
          <th>MAG</th>
          <td>[50706540.0, 129034680.0, -49169952.0]</td>
          <td>[1.4498347044, 0.676015079021, 0.923388004303]</td>
          <td>1.229631e+00</td>
        </tr>
        <tr>
          <th>GSE</th>
          <td>[147101280.0, 0.00603915005922, 0.0522168129683]</td>
          <td>[-0.488757014275, -1.78320043176e-08, 0.078364...</td>
          <td>1.490818e+00</td>
        </tr>
        <tr>
          <th>GSM</th>
          <td>[147101280.0, 0.0115641998127, 0.0512770526111]</td>
          <td>[-0.488757014275, -1.77362000642e-08, 0.078364...</td>
          <td>1.490818e+00</td>
        </tr>
        <tr>
          <th>SM</th>
          <td>[138640192.0, 0.0115641998127, -49169952.0]</td>
          <td>[-1.86519682407, 0.499917596579, 2.37058091164]</td>
          <td>3.752189e+00</td>
        </tr>
        <tr>
          <th>RTN</th>
          <td>[-147101280.0, 7.31750478735e-05, 0.0518711544...</td>
          <td>[-0.488793492317, 9.47769137838e-07, 0.0776719...</td>
          <td>1.490818e+00</td>
        </tr>
        <tr>
          <th>GSEQ</th>
          <td>[147101280.0, 3.08773196593e-05, 0.0525648742914]</td>
          <td>[-0.488757014275, -1.78933579065e-08, 0.078364...</td>
          <td>1.490818e+00</td>
        </tr>
        <tr>
          <th>HEE</th>
          <td>[0.998617529869, -0.00603915005922, 0.05221681...</td>
          <td>[1.0, 6.34189090132e-11, -1.06054776072e-09]</td>
          <td>1.062442e-09</td>
        </tr>
        <tr>
          <th>HAE</th>
          <td>[-0.173912256956, 0.983375787735, 0.0522168129...</td>
          <td>[1.0, 1.08513087405e-09, -4.08465317125e-10]</td>
          <td>1.159462e-09</td>
        </tr>
        <tr>
          <th>HEEQ</th>
          <td>[1.0, 0.0, 0.0]</td>
          <td>[1.0, 0.0, 0.0]</td>
          <td>0.000000e+00</td>
        </tr>
      </tbody>
    </table>
    </div>



