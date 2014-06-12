      program kameleon_compat_f
c     My attempt at a fortran program. This is an example program
c     mainly to test the ionospheric output of SWMF
 
c     Three functions used to interpolate 
c     data from a specified batsrus cdf file
 
      external f_kameleon_create, f_kameleon_open, f_kameleon_delete
      external f_kameleon_load_variable, f_kameleon_close
      external f_interpolator_create
      external f_interpolator_delete
      external f_kameleon_interpolate
      external f_kameleon_interpolate_by_id
      external f_kameleon_get_variable_id
 
c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path
      character*100 c0,c1,c2
      character*100 variable

c     r, lat, mlt for iono data
      real rc0, rc1, rc2
      real d0,d1,d2
      real interpolated_value
      real latdelta,mltdelta
      integer status,kid,iid, vid
      integer argc,zero
      zero = 0
c      character*50 var_to_read
 
c     --- set your actual path name here ---
      argc = iargc()

      if (argc.ne.2) then
        print *,"Must supply the filename and variable"
       call exit(1)
      endif

      call getarg(1, cdf_file_path)
      call getarg(2, variable)
c      call getarg(3, c0)
c      call getarg(4, c1)
c      call getarg(5, c2)

c      read(c0,*) rc0
c      read(c1,*) rc1
c      read(c2,*) rc2

c      print *, 'c0: ', rc0, ' c1: ', rc1, ' c2: ', rc2
c      cdf_file_path='example.cdf '

c     this id will be set when calling f_kameleon_create. just
c     initialize it, since I have no idea if you need to initialize
c     variables in fortran
      kid=0
      vid=0
c      variable='ep '

c      lat=89.0
c      mlt=12.0
c      variable='ep '
      call f_kameleon_create(kid)
      call f_kameleon_open(kid,cdf_file_path,status)
      print *, 'done opening file'
      call f_kameleon_load_variable(kid,variable)
      print *, 'done loading variable ', variable
      call f_interpolator_create(kid, iid)
      print *,'created interpolator'
      call f_kameleon_get_variable_id(kid,variable, vid);
      latdelta = 180.0/(181.-1.)
      mltdelta = 24.0/(181.-1.)
      do j=0,180
        do i = 0, 180
          rc1 = 90 - i*latdelta
          rc2 = mod(12.+j*mltdelta, 24.)
          call f_kameleon_interpolate_by_id(iid, vid, rc0, rc1, rc2,
     &d0, d1, d2, interpolated_value)
          print *, ' lat: ', rc1, ' mlt: ', rc2,
     &' interpolated value: ', interpolated_value
        enddo
      enddo

      call f_interpolator_delete(iid);
      call f_kameleon_close(kid)
      call f_kameleon_delete(kid,status)
 
        stop
      end 
      
