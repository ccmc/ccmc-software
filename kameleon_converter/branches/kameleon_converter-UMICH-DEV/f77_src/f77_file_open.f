      subroutine f77_file_open( file_unit, file_name, file_name_len,
     *                file_status, file_status_len,  file_format, 
     *                file_format_len, return_status )
      character file_name*(*)
      character file_status*(*)
      character file_format*(*)
      character file_name_2*400, file_status_2*25, file_format_2*25
      integer file_unit, ios, return_status
      integer file_name_len, file_status_len, file_format_len

      file_name_2(1:400) = file_name(1:file_name_len)
      file_status_2(1:25) = file_status(1:file_status_len)
      file_format_2(1:25) = file_format(1:file_format_len)

c      print *, "file unit:", file_unit
c      print *, "file_name_2:", file_name_2
c      print *, "open_status_2:", file_status_2
c      print *, "open_format_2:", file_format_2
c      print *, "return_status", return_status
      open( unit=file_unit, file=file_name_2, status=file_status_2,
     *                form=file_format_2, iostat=ios )
      return_status = ios
c      print *, "return_status after open call", return_status
      end
      
