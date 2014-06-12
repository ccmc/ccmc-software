      program generalfilereader_compat_f
 
c     Three functions used to interpolate 
c     data from a specified batsrus cdf file
 
      external f_generalfilereader_create, f_generalfilereader_open, f_generalfilereader_delete
      external f_generalfilereader_close, f_kameleon_create_c_string
 
      character*300 cdf_file_path 
      character*301 cdf_file_path_c
      integer status,fid
      

      call getarg(1, cdf_file_path)
      call f_kameleon_create_c_string(cdf_file_path, cdf_file_path_c)
c      cdf_file_path='example.cdf '
      
      write(*,*) 'Processing file: ', cdf_file_path
 
c     Open the cdf file     
 
      fid=0
      call f_generalfilereader_create(fid)
      call f_generalfilereader_open(fid,cdf_file_path_c,status)
      call f_generalfilereader_close(fid)
      call f_generalfilereader_delete(fid)
 
      end 
      
