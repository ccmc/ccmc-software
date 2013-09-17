c.---------------------------------------
      subroutine wrn2(iu,a,n,cid,it,rid)
c.---------------------------------------
c character encoding of real array into 2 bytes
      real a(n)
      character*8 cid
      real a1(0:63)
      integer i1(0:63),i2(0:63),i3(0:63)
      zmin=1.e33
      zmax=1.e-33
      do 100 i=1,n
      b=abs(a(i))
      zmin=amin1(zmin,b)
      zmax=amax1(zmax,b)
100   continue
1000  format(a,i8,3e14.7,i8,a)
      zmin=amax1(1.e-33,zmin)
      zmax=amin1( 1.e33,zmax)
      z2=alog(zmax)
      z1=-76.
      if(zmin.ne.0.)z1=alog(zmin)
      z1=amax1(z1,z2-37.)
      if(abs(z2-z1).le.1.e-5) then
      z1=z1-1.0
      z2=z2+1.0
      endif
      z0=exp(z1)
      dz=float(4410)/(z2-z1)
      write(iu,1000)'WRN2',n,z1,z2,rid,it,cid
      do 200 k=1,n,64
      nk=min0(63,n-k)
      do 210 i=0,nk
      a1(i)=amin1(1.e33,abs(a(i+k)))
      a1(i)=amax1(z0,a1(i))
      a1(i)=dz*(alog(a1(i))-z1)+0.5
      i3(i)=a1(i)
      i3(i)=max0(0,i3(i))
      i3(i)=min0(4414,i3(i))
      i1(i)=i3(i)/94
      i2(i)=i3(i)-94*i1(i)
      if(a(i+k).lt.0.)i1(i)=i1(i)+47
      i1(i)=i1(i)+33
      i2(i)=i2(i)+33
210   continue
      call wrnenc(iu,i1,nk)
      call wrnenc(iu,i2,nk)
200   continue
      return
      end
