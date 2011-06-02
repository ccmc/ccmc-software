c-----------------------------------------------------------
      subroutine end0(r,m)
c-----------------------------------------------------------
      character*80 r
      n=min0(m,len(r))
      do 100 i=1,n
      m=i-1
      if(r(i:i).eq.' ') return
100   continue
      m=n
      return
      end
