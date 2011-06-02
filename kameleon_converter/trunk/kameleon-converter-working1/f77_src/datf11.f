c-----------------------------------------------------------
      subroutine datf11(iu,a1,nx,l1,l2,it)
c-----------------------------------------------------------
      character*80 l1,l2
      real a1(*)
      write(iu,'(a)')'FIELD-1D-1'
      write(iu,'(a)') l1
      write(iu,'(a)') l2
      write(iu,*) it,nx
      call wrn2(iu,a1,nx,'FUNC-1-1',it,float(it))
      return
      end
