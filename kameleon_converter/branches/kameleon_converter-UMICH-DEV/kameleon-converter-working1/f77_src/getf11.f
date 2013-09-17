c-----------------------------------------------------------
      subroutine getf11(iu,a1,nx,l1,l2,it,mlen)
c-----------------------------------------------------------
      real a1(*)
      character*80 rec
      character*80 l1,l2
c      m=len(l1)
      m=mlen
c      print *, "the lenght m of l1 =", m
c      print *, "l1 is actually ", l1 
c      open(unit=iu, file="grid_file",status="old",form="formatted")
c      open(unit=iu, file="grid_file",status="old")
c      print *, "from getf11 iu=",iu
c      print *, "calling end0"
      call end0(l1,m)
      isany=0
      if(l1(1:m).eq.'any') isany=1
100   continue
c      print *, "the value of isany", isany
c      print *, "calling 1st read"
      read(iu,1000,end=190) rec
c      print *, "rec=", rec
      if(rec(1:10).eq.'FIELD-1D-1') then
c      print *, "calling 2nd read"
      read(iu,1000,end=190) rec
c      print *, "2nd rec=", rec
c      print *, "comparing", l1, "to", rec
c      if((isany.eq.0).and.l1(1:5).ne.rec(1:5)) goto 100
      if((isany.eq.0).and.l1(1:m).ne.rec(1:m)) goto 100
      l1=rec
c      print *, "calling 3rd read"
      read(iu,1000,end=190) l2
c      print *, "calling 4th read"
      read(iu,*,end=190)it,nx
c      print *, "calling rdn2"
      call rdn2(iu,a1,nx,rec,it,rid)
      return
      endif
      goto 100
190   continue
      nx=-1
1000  format(a)
      return
      end
