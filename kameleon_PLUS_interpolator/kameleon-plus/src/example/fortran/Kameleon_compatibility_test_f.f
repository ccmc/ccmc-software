      program kameleon_compat_f
 
c     Three functions used to interpolate 
c     data from a specified batsrus cdf file
 
      external f_kameleon_create, f_kameleon_open, f_kameleon_delete
      external f_kameleon_load_variable, f_kameleon_close
      external f_interpolator_create
      external f_interpolator_delete
      external f_kameleon_interpolate
 
c     Variables to be used for interpolation and data extraction

      character*300 cdf_file_path
      character*3 variable
      real r,lat,mlt
      real dr,dlat,dmlt
      real interpolated_value
      integer status,kid,iid
      
c      character*50 var_to_read
 
c     --- set your actual path name here ---

      call getarg(1, cdf_file_path)
c      cdf_file_path='example.cdf '
      
      write(*,*) 'Processing file: ', cdf_file_path
      write(*,*) 'Note:  The above referenced file name is 
     1hard coded in the examples/f2c_interp_batsrus.f file.
     2either create this file/link or modify the cdf_file_path
     3 variable '
 
c     Open the cdf file     
 
      
      kid=0

C      lat=-1.*i+90
      lat=89.0
C      mlt=0.0
      mlt=12.0
      variable='ep '
      call f_kameleon_create(kid)
      call f_kameleon_open(kid,cdf_file_path,status)
      call f_kameleon_load_variable(kid,variable)
      call f_interpolator_create(kid, iid)
       print*,'kid iid lat mlt=',kid,iid,lat,mlt
c      do a for loop using the same interpolator id so we can reuse the object
c       do i=1,1
c      lat = -1.*i+90.
c      interpolated_value=-999.
      call f_kameleon_interpolate(iid, variable, r, lat, mlt, dr, dlat,
     &dmlt, interpolated_value)
       print*,'kid2=',kid,iid
       print*, 'lat=', lat, 'mlt=', mlt ,r,interpolated_value
c       enddo
c     I don't know how to output the interpolated value. Hopefully it is
c     stored in interpolated_value
      call f_interpolator_delete(iid);
      call f_kameleon_close(kid)
      call f_kameleon_delete(kid,status)
 
c       print*, 'lat2=', lat, 'lon2=', mlt 
c       print*,'UP TO HERE2'
        stop
      end 
      
