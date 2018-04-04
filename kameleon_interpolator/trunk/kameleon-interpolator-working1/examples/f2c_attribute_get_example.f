      program f2c_attribute_get_example

c     Functions used for various attribute get 
c     operations - function name is formated
c     as f2c_[g|v]attribute_[char|float|int]_get
c     where g - global attribute
c           v - variable attribute
c     int/float/get - attribute type
       
      integer f2c_open_cdf, f2c_close_cdf
      external f2c_open_cdf, f2c_close_cdf
      integer f2c_gattribute_char_get
      external f2c_gattribute_char_get
      integer f2c_gattribute_int_get
      external f2c_gattribute_int_get
      integer f2c_gattribute_float_get
      external f2c_gattribute_float_get
      integer f2c_vattribute_char_get
      external f2c_vattribute_char_get
      integer f2c_vattribute_float_get
      external f2c_vattribute_float_get
      integer f2c_vattribute_int_get
      external f2c_vattribute_int_get

c     Variables to be used as arguments to attribute get
c     routines

      character*250 cdf_file_path 
      character*50 variable_of_interest
      integer status, string_length

c     --- variables to hold attribute values returned by
c     --- attribute get routines 

      integer gattr_int_val, vattr_int_val
      real*4 gattr_float_val, vattr_float_val
      character*250 gattr_char_val, vattr_char_val

c     --- variables to hold the names of the attributes of interest      

      character*500 gattr_name_4int, gattr_name_4float
      character*500 gattr_name_4char, vattr_name_4int
      character*500 vattr_name_4float, vattr_name_4char

c     --- set your actual path name here ---      

      cdf_file_path='your_test_data_file '

c     --- set the names of of attributes of interest ---      

      gattr_name_4char="model_name "
      gattr_name_4int='grid_system_count '
      gattr_name_4float='elapsed_time_in_seconds '

      variable_of_interest='bx '

      vattr_name_4char='description '
      vattr_name_4int='is_vector_component '
      vattr_name_4float='mask '

c     --- begin function calls ---	  

      write(*,*) 'Processing file: ', cdf_file_path
      write(*,*) 'Note:  The above referenced file name is 
     1hard coded in the examples/f2c_attribute_get.f file.
     2either create this file/link or modify the cdf_file_path
     3variable '

      status=f2c_open_cdf(cdf_file_path)

c     --- call the Global attribute "gets" ---	  
      string_length = 
     1f2c_gattribute_char_get(gattr_name_4char,gattr_char_val)
      status = f2c_gattribute_int_get( gattr_name_4int, gattr_int_val )
      status =
     1f2c_gattribute_float_get( gattr_name_4float, gattr_float_val )

c	  print*, 'gattr_char_val length = ', string_length

      print*, 'Extracted Global Attribute: ', 
     1gattr_name_4char(1:30), '=', gattr_char_val

      print*, 'Extracted Global Attribute: ',
     1gattr_name_4int(1:30), '=', gattr_int_val

      print*, 'Extracted Global Attribute: ',
     1gattr_name_4float(1:30), '=', gattr_float_val

c     --- call the Variable attribute "gets" ---	  	  

      string_length =
     1f2c_vattribute_char_get( variable_of_interest, 
     2vattr_name_4char,vattr_char_val)
c 	  print*, 'vattr_char_val length = ', string_length     

      status =
     1f2c_vattribute_float_get( variable_of_interest,
     2vattr_name_4float, vattr_float_val )
      status =
     1f2c_vattribute_int_get( variable_of_interest,
     2vattr_name_4int, vattr_int_val )
      print*, 'Variable Attributes for ', variable_of_interest 
      print*, 'Extracted Variable Attribute: ',
     1vattr_name_4char(1:30), '=', vattr_char_val
      print*, 'Extracted Variable Attribute: ',
     1vattr_name_4int(1:30), '=', vattr_int_val

      print*, 'Extracted Variable Attribute: ',
     1vattr_name_4float(1:30), '=', vattr_float_val

      status=f2c_close_cdf(0)

      end

