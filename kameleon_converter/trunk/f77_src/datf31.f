c-----------------------------------------------------------
      subroutine datf31(iu,a1,nx,ny,nz,l1,l2,it)
c-----------------------------------------------------------
      character*80 l1,l2
      real a1(*)
      write(iu,'(a)')'FIELD-3D-1'
      write(iu,'(a)') l1
      write(iu,'(a)') l2
      write(iu,*)it,nx,ny,nz
      call wrn2(iu,a1,nx*ny*nz,'FUNC-3-1',it,float(ny))
      return
      end
