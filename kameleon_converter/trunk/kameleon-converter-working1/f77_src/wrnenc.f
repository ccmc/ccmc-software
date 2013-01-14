c.---------------------------------------
      subroutine wrnenc(iu,i1,n)
c.---------------------------------------
      integer i1(0:63)
      integer i2(-1:63)
      character c*72
      ick=i1(0)
      ir=i1(0)
      ic=1
      k=-1
      do 100 i=1,n
      ick=ick+i1(i)
      if(i1(i).eq.ir) then
      ic=ic+1
      else
      if(ic.eq.1) then
      k=k+1
      i2(k)=ir
      else
      k=k+1
      i2(k)=ic+170
      k=k+1
      i2(k)=ir
      endif
      ic=1
      ir=i1(i)
      endif
100   continue
      if(ic.eq.1) then
      k=k+1
      i2(k)=ir
      else
      k=k+1
      i2(k)=ic+170
      k=k+1
      i2(k)=ir
      endif
      i2(-1)=33+mod(ick,92)
      j=0
      do 200 i=-1,k
      j=j+1
      c(j:j)=char(i2(i))
200   continue
      write(iu,'(a)')c(1:j)
      return
      end
