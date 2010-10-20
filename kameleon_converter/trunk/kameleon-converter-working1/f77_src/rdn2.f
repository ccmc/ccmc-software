c.---------------------------------------
      subroutine rdn2(iu,a,n,cid,it,rid)
c.---------------------------------------
c character decoding
      real a(*)
      character*8 cid
      character*4 did
      real a1(0:63)
      integer i1(0:63),i2(0:63),i3(0:63)
100   continue
      read(iu,1000,err=100,end=900)did,n,zmin,zmax,rid,it,cid
1000  format(a,i8,3e14.7,i8,a)
      if(did.ne.'WRN2') goto 100
c      write(0,*)' zmin,zmax,n ',zmin,zmax,n
      if(zmin.eq.zmax) then
      do 200 i=1,n
      a(i)=zmin
200   continue
      return
      endif
      l=0
      do 300 k=1,n,64
      nk=min0(63,n-k)
      call wrndec(iu,i1,nn)
      if(nn.ne.nk) then
c      write(0,*)'rdn2: nn .ne. nk ',nn,nk,n,k
      n=-2
      return
      endif
      call wrndec(iu,i2,nn)
      if(nn.ne.nk) then
      write(0,*)'rdn2: nn .ne. nk ',nn,nk
      n=-2
      return
      endif
      dzi=(zmax-zmin)/float(4410)
      do 330 i=0,nk
      i1(i)=i1(i)-33
      i2(i)=i2(i)-33
      sig=1.
      if(i1(i).ge.47) then
      sig=-1.
      i1(i)=i1(i)-47
      endif
      i3(i)=i2(i)+94*i1(i)
      a1(i)=i3(i)
      a1(i)=dzi*a1(i)+zmin
      a1(i)=sig*exp(a1(i))
      l=l+1
      a(l)=a1(i)
330   continue
300   continue
      return
900   continue
      n=-1
      return
      end
