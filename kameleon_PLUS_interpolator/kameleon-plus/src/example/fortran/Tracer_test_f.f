      program tracer_f
 
c     Five functions to create a Kameleon Object, open the cdf file, load the variable, and close the file
 
      external f_kameleon_create, f_kameleon_open, f_kameleon_delete
      external f_kameleon_load_vector_variable, f_kameleon_close
      
c     Three Tracer functions
      external f_tracer_create, f_tracer_bidirectionaltrace, f_tracer_delete
 
c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path 
      character*50 variable
c      real*8 x,y,z
c      real*8 interpolated_value
      integer status,kid,tid
      real*8 actual_steps
      real x_array(1000)
      real y_array(1000)
      real z_array(1000)
      
c      character*50 var_to_read
 
c     --- set your actual path name here ---

      call getarg(1, cdf_file_path)
      call getarg(2, variable)
c      cdf_file_path='example.cdf '
      
      write(*,*) 'Processing file: ', cdf_file_path
      write(*,*) 'Note:  The above referenced file name is 
     1hard coded in the examples/f2c_interp_batsrus.f file.
     2either create this file/link or modify the cdf_file_path
     3variable '
 
c     Open the cdf file     
 

      kid=0
      tid=0
      status=0
      call f_Kameleon_create(kid)
c      call f_Kameleon_open(kid,cdf_file_path,status)
c      call f_Kameleon_load_vector_variable(kid,variable)
c      call f_Tracer_create(tid,kid)
c      call f_Tracer_bidirectionaltrace(tid,variable,-10.,0.,0.,1000,.2, 
c     1actual_steps,x_array,y_array,z_array)
c      call f_Tracer_delete(tid);
c      call f_Kameleon_close(kid)
c      call f_Kameleon_delete(kid)

      end 