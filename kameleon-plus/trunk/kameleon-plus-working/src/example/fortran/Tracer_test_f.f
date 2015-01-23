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
      character*100 c0,c1,c2
      real rc0, rc1, rc2
      integer argc

c      character*50 var_to_read

c     --- set your actual path name here ---
      argc = iargc()
      if (argc.ne.5) then
        print *,"tracer_compat_f <filename> variable c0 c1 c2"
        print *,"    Adapt3D, OpenGGCM, BATSRUS, LFM: x y z"
        print *,"    ENLIL, MAS: r theta(latitude) phi(longitude)"
        print *,"    SWMF Iono: 0 theta(latitude) phi(MLT)"
       call exit(1)
      endif

      call getarg(1, cdf_file_path)
      call getarg(2, variable)
      call getarg(3, c0)
      call getarg(4, c1)
      call getarg(5, c2)

      read(c0,*) rc0
      read(c1,*) rc1
      read(c2,*) rc2

      write(*,*) 'Processing file: ', cdf_file_path

c     Open the cdf file


      kid=0
      tid=0
      status=0
      actual_steps=0
      call f_kameleon_create_c_string(cdf_file_path, cdf_file_path_c)
      call f_kameleon_create_c_string(variable, variable_c)
      call f_kameleon_create(kid)
      call f_kameleon_open(kid,cdf_file_path_c,status)
      call f_kameleon_load_vector_variable(kid,variable_c)
      call f_tracer_create(tid,kid)

      print *, 'c0: ', rc0, ' c1: ', rc1, ' c2: ', rc2
      print *, 'passing variables:', 'tid: ', tid, 'variable ', variable

c     make sure the step size used is appropriate and the target arrays have 2 times
c     the step_max so the arrays have enough for two directions.
      call f_Tracer_bidirectionaltrace(tid,variable,rc0,rc1,rc2,10000, 
     &       .2, actual_steps,x_array,y_array,z_array)
      write(*,*) 'finished tracing'
      write(*,*) 'actual_steps: ',actual_steps
      call f_tracer_delete(tid);
      call f_kameleon_close(kid) 
      call f_kameleon_delete(kid, status)
      end
