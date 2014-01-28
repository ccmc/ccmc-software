      program timeinterp_compat_f
c     My attempt at a fortran program. This is an example program
c     mainly to test the time interpolator
 
      external f_timeinterp_create
      external f_timeinterp_addtimestep
      external f_timeinterp_managemem,f_timeinterp_interpolate
      external f_timeinterp_timetoepoch
      external f_timeinterp_timestrtoepoch
      external f_timeinterp_closeandcleanupmemory

c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path1
      character*300 cdf_file_path2
      character*25 time
      character*100 c0,c1,c2
      character*100 variable

c
      real interpolated_value
      real rc0,rc1,rc2
      real time_epoch
      integer status,tid
      integer argc,zero
      zero = 0
      time_epoch = 0.
c      character*50 var_to_read
 
c     --- set your actual path name here ---
      argc = iargc()

      if (argc.ne.6) then
        print *,". <filename1> <filename2> <time> c0 c1 c2"
        print *,"    Adapt3D, OpenGGCM, BATSRUS: x y z"
        print *,"    ENLIL, MAS: r theta(latitude) phi(longitude)"
        print *,"    SWMF Iono: 0 theta(latitude) phi(MLT)"
       call exit(1)
      endif

      call getarg(1, cdf_file_path1)
      call getarg(2, cdf_file_path2)
      call getarg(3, time)
      call getarg(4, c0)
      call getarg(5, c1)
      call getarg(6, c2)

      read(c0,*) rc0
      read(c1,*) rc1
      read(c2,*) rc2

      print *, 'c0: ', rc0, ' c1: ', rc1, ' c2: ', rc2
c      cdf_file_path='example.cdf '

c     this id will be set when calling f_timeinterp_create. just
c     initialize it, since I have no idea if you need to initialize
c     variables in fortran
      tid=0
      print *,'before create'
      call f_timeinterp_create(tid)
      print *,'before timestrtoepoch'
      call f_timeinterp_timestrtoepoch(time, time_epoch)
      call f_timeinterp_addtimestep(tid, cdf_file_path1)
      call f_timeinterp_addtimestep(tid, cdf_file_path2)
      variable = 'ep'
      call f_timeinterp_managemem(tid, time_epoch, variable);
      call f_timeinterp_interpolate(tid, time_epoch, variable, 0.0,
     &89.0, 12.0, interpolated_value)
      print *, 'interpolated value: ', interpolated_value;
      call f_timeinterp_closeandcleanupmemory(tid)
      print *, 'finished'

        stop
      end 
      
