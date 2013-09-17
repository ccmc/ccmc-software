c.---------------------------------------
      subroutine wrndec(iu,i1,n)
c.---------------------------------------
      integer i1(0:63)
C      common /wrdqq9/i2(-2:67)
      integer i2(-2:67)
      character c*72
      nsa=n
      c=' '
      read(iu,'(a)',end=900,err=900) c
      i=-2
      j=0
100   continue
      j=j+1
      if(j.gt.67) then
      write(0,*)'error:wrndec: cant find end of encoded record'
      goto 910
      endif
      ir=ichar(c(j:j))
      if(ir.lt.0)ir=ir+256
      if(ir.eq.32) goto 190
      if(ir.gt.127) then
      ic=ir-170
      j=j+1
      ir=ichar(c(j:j))
      if(ir.lt.0)ir=ir+256
      do 110 l=1,ic
      i=i+1
      i2(i)=ir
110   continue
      else
      i=i+1
      i2(i)=ir
      endif
      goto 100
190   continue
      ick=0
      n=i
      if(n.gt.63) then
      write(0,*)'error:wrndec: n gt 63, n=',n
c      n=-5
c      write(0,'(a)')'rec:'
c      write(0,'(a)')c
      goto 910
      endif
      do 200 i=0,n
      i1(i)=i2(i)
      ick=ick+i1(i)
200   continue
      if(i2(-1).ne.33+mod(ick,92)) then
      write(0,*)'error:wrndec: checksum error '
c      write(0,'(a,a)')'rec:',c
c      write(0,*)i2(-1),33+mod(ick,92)
c      n=-5
      goto 910
      endif
      return
910   continue
      do 911 i=0,63
      i1(i)=34
911   continue
      n=nsa
      return
900   continue
      write(0,*)' wrndec eof/err '
      n=-5
      return
      end
