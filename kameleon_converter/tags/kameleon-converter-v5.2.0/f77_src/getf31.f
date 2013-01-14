c-----------------------------------------------------------
      subroutine getf31(iu,a1,nx,ny,nz,l1,l2,it,mlen)
c-----------------------------------------------------------
      real a1(*)
      character*80 rec
      character*80 l1,l2
c      m=len(l1)
      m=mlen
c      open(unit=iu, file="3df_file",status="old",form="formatted")
c      write(*,*) 'len(l1): ',len(l1)
c      print *, "form getf31 iu =", iu
c      print *, "l1=", l1
c      print *, "calling end0"
      call end0(l1,m)
c      write(*,*) 'len(l1): ',m
      isany=0
      if(l1(1:m).eq.'any') isany=1
100   continue
c      print *, 'isany=',isany
c      print *, 'calling 1st read'
      read(iu,1000,end=190) rec
      if(rec(1:10).eq.'FIELD-3D-1') then
c      print *, "calling 2nd read because line = FIELD-3D-1"
      read(iu,1000,end=190) rec
      if((isany.eq.0).and.l1(1:m).ne.rec(1:m)) goto 100
c      print *, "comparing l1 & rec", l1(1:2), rec(1:2)
c      if((isany.eq.0).and.l1(1:3).ne.rec(1:3)) goto 100
      l1=rec
c      print *, "calling 3rd read"
      read(iu,1000,end=190) l2
c      print *, "calling 4th read"
      read(iu,*,end=190) it,nx,ny,nz
      call rdn2(iu,a1,nn,rec,it,rid)
      if(nn.lt.0)nx=nn
      return
      endif
      goto 100
190   continue
      nx=-1
1000  format(a)
      return
      end
