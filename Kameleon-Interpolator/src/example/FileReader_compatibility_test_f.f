      program FileReader_compat_f
 
c     Three functions used to interpolate 
c     data from a specified batsrus cdf file
 
      external f_FileReader_create, f_FileReader_open, f_FileReader_delete
      external f_FileReader_close
 
c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path 
c      real*8 x,y,z
c      real*8 interpolated_value
      integer status
c      character*50 var_to_read
 
c     --- set your actual path name here ---

      call getarg(1, cdf_file_path)
c      cdf_file_path='batsrus.cdf '
      
      write(*,*) 'Processing file: ', cdf_file_path
      write(*,*) 'Note:  The above referenced file name is 
     1hard coded in the examples/f2c_interp_batsrus.f file.
     2either create this file/link or modify the cdf_file_path
     3variable '
 
c     Open the cdf file     
 
      integer id
      id=0
      f_FileReader_create(id)
      f_FileReader_open(id,cdf_file_path)
      f_FileReader_close(id)
      f_FileReader_delete(id)
 
      end 
      
