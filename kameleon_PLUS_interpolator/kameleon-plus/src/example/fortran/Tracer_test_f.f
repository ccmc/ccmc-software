      program tracer_f

c     Five functions to create a Kameleon Object, open the cdf file, load the variable, and close the file

      external f_kameleon_create, f_kameleon_open, f_kameleon_delete
      external f_kameleon_load_vector_variable, f_kameleon_close
      external f_kameleon_create_c_string

c     Three Tracer functions
      external f_tracer_create, f_tracer_bidirectionaltrace, f_tracer_delete

c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path
      character*301 cdf_file_path_c
      character*50 variable
      character*50 variable_c
c      real*8 x,y,z
c      real*8 interpolated_value
      integer status,kid,tid,s_length,actual_steps
      real x_array(1000)
      real y_array(1000)
      real z_array(1000)

c      character*50 var_to_read

c     --- set your actual path name here ---

      call getarg(1, cdf_file_path)
      call getarg(2, variable)
c      cdf_file_path='/Users/dberrios/Desktop/test.cdf'//CHAR(0)
c      trim(filename)
      s_length=len(cdf_file_path)
      write(*,*) 's_length for cdf_file_path: ',s_length
c      cdf_file_path(s_length:s_length)=CHAR(0)

c      s_length=len(variable)
c      write(*,*) 's_length for variable: ',s_length
c      variable(s_length:s_length)=CHAR(0)

      write(*,*) 'Processing file: ', cdf_file_path
c      write(*,*) 'Note:  The above referenced file name is
c     1hard coded in the examples/f2c_interp_batsrus.f file.
c     2either create this file/link or modify the cdf_file_path
c     3variable '

c     Open the cdf file


      kid=0
      tid=0
      status=0
      actual_steps=0
      call f_kameleon_create_c_string(cdf_file_path, cdf_file_path_c)
      call f_kameleon_create_c_string(variable, variable_c)
      call f_kameleon_create(kid)
      call f_Kameleon_open(kid,cdf_file_path_c,status)
      call f_Kameleon_load_vector_variable(kid,variable_c)
      call f_Tracer_create(tid,kid)

c     make sure the step size used is appropriate and the target arrays have 2 times
c     the step_max so the arrays have enough for two directions.
      call f_Tracer_bidirectionaltrace(tid,variable,-10.,0.,0.,500,.2,
     1actual_steps,x_array,y_array,z_array)
      write(*,*) 'actual_steps: ',actual_steps
      call f_Tracer_delete(tid);
      call f_Kameleon_close(kid)
      call f_Kameleon_delete(kid)
      end
