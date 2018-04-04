      program f2c_interp_msfc_tvm
 
c     Three functions used to interpolate 
c     data from a specified msfc_tvm cdf file
 
      external f2c_open_cdf, f2c_close_cdf, f2c_interp_msfc_tvm_cdf
 
c     Variables to be used for interpolation and data extraction

      character*150 cdf_file_path 
      real*8 x,y,z
      real*8 interpolated_value
      integer status
      character*50 var_to_read
 
c     --- set your actual path name here ---

      cdf_file_path='msfc_tvm.cdf '

      write(*,*) 'Processing file: ', cdf_file_path
      write(*,*) 'Note:  The above referenced file name is 
     1hard coded in the examples/f2c_interp_msfc_tvm.f file.
     2either create this file/link or modify the cdf_file_path
     3variable '
 
c     Open the cdf file     
 
      status=f2c_open_cdf(cdf_file_path)
 
c     --- set your position values in GEO ---
 
      x=.077294615
      y=.00001
      z=0
 
c     --- set name of variable of interest ---
 
      var_to_read='b_l '
 
c     --- call the interpolation routine ---
 
      status=
     /f2c_interp_msfc_tvm_cdf(x,y,z,interpolated_value, var_to_read )
 
c     --- close the currently open cdf file
 
      status=f2c_close_cdf(0)
 
      write(*,*) var_to_read, interpolated_value

      end

