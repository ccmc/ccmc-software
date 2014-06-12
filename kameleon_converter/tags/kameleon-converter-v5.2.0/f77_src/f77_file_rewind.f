      subroutine f77_file_rewind( file_unit, return_status )
      integer file_unit, ios, return_status
c      print *, "file unit:", file_unit
c      print *, "return_status before close call", return_status
      rewind( unit=file_unit, iostat=ios )
      return_status = ios
c      print *, "return_status after rewind call", return_status
      end