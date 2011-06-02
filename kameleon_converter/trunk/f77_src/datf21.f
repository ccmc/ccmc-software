c-----------------------------------------------------------
      subroutine datf21(iu,a1,nx,ny,l1,l2,it)
c-----------------------------------------------------------
      character*80 l1,l2
      real a1(*)
      write(iu,'(a)')'FIELD-2D-1'
      write(iu,'(a)') l1
      write(iu,'(a)') l2
      write(iu,*)it,nx,ny
      call wrn2(iu,a1,nx*ny,'FUNC-2-1',it,float(ny))
      return
      end
