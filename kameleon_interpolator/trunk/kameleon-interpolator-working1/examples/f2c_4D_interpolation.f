      program f2c_4D_interpolation
 
c     Three functions used for temporal 
c     interpolation given a particular data set
 
      integer f2c_init_time, f2c_time_interpolate, f2c_close_cdf
      external f2c_init_time, f2c_time_interpolate, f2c_close_cdf
 
c     Variables to be used for interpolation and data extraction

      character*150 cdf_directory_path 
      real*8 x,y,z
      real*8 interpolated_value
      integer status
      character*50 var_to_read

c    declare time variables to be used for time interpolation
c    the start_time & end_time variables will be set during 
c    f2c_init_time() call.  The values will represent the time
c    range for all data located in the specified cdf_directory_path

      real*8 start_time, end_time, target_time

c     --- set your position values in GSE ---
 
      x=-55.0
      y=12.0
      z=20.0
 
c     --- set name of variable of interest ---
 
      var_to_read='bx '

c     --- set directory path containing data set ---

      cdf_directory_path='your_test_data_set '
      
      write(*,*) 'Processing directory: ', cdf_directory_path
      write(*,*) 'Note:  The above referenced directory name is 
     1hard coded in the examples/f2c_4D_interpolation.f file.
     2either create this file/link or modify the cdf_directory_path
     3variable '

c     Call the f2c_init_time routine to prepare data set and register
c     available times with program 

      status=f2c_init_time( cdf_directory_path, start_time, end_time )
      
      write( *, * ) 'cdf_directory_path ', cdf_directory_path
      write( *, * ) 'start_time: ', start_time
      write( *,*  ) 'end_time:   ', end_time

c     Time is currently set to CDF EPOCH ( milliseconds since 1.1.000 )
c     We already know the time range via the start_time & end_time variables
c     so we can increment the start_time variable by 90000 milliseconds = 2.5 minutes
c     NOTE:  For loop can be added to interate over range at any specified time interval
c     where start_time <= target_time <= end_time

      target_time = start_time + ( 90000. )

      status=f2c_time_interpolate( target_time, x, y, z, var_to_read, 
     1interpolated_value )
c      write(*,*) var_1_to_read, '@', target_time '=' interp_val_1	  
      print*, var_to_read, '@ ', target_time, '=', interpolated_value
c     status=f2c_close_cdf(0)

      end

