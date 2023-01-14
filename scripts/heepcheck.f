      program heepcheck
      dimension par(5),diff(4),derv(4,5)
      data am/938.30/,radfac/0.017453293/
      w(e,e1,th)=
     1 sqrt(2.0*am*(e-e1)+am**2-4.0*e*e1*sin(th*radfac/2.0)**2)
      write(6,600)
 600  format(" program heepcheck, version 31.5.97") 
   1  continue
      write(6,601)
 601  format(" enter electron energy(MeV) and angle; E0=0/:/")     
      read(5,*)e0, the0
      if(e0.lt.0.1)stop
      call heepkin(e0,the0,pe0,thq0,q0)
      w0=w(e0,pe0,the0)
      write(6,602)e0,the0,pe0,q0,thq0,w0
c
c  these are the real values corresponding to the given energy and angle
c  they will in general (at least for q, thq and W) NOT be the ones you
c  get in your analysis, since the 'real' values
c  give E_m=0 and p_m=0, while the cooresponding ones in your
c  analysis may be off
c
 602  format(/"Nominal (correct) values:"
     1/"    e_e     th_e'    e_e'     q     th_q     W"/6f8.1)
      write(6,603)
 603  format(/"variations in W, E_m and p_m for changes in parms",
     1/"     dE   dth_e'  dp_e'  dthp  dp_p |",
     2 "    dW    dE_m   dp_m(par)  dp_m(perp)"/)
      unity=1.0
c   variation of +0.1% in beam energy
      de=0.001*e0
      derv(1,1)=w(e0+de,pe0,the0)-w0
      derv(2,1)=de
      derv(3,1)=de*cos(thq0*radfac)
      derv(4,1)=de*sin(thq0*radfac)
      write(6,611)unity,(derv(i,1),i=1,4)
 611  format(f7.1,28x," |",4f8.2)
c   variation of +1 mrad in electron angle
      dth=0.001
      derv(1,2)=w(e0,pe0,the0+dth/radfac)-w0
      derv(2,2)=0.0
      derv(3,2)=pe0*dth*sin((the0+thq0)*radfac)
      derv(4,2)=-pe0*dth*cos((the0+thq0)*radfac)
      write(6,612)unity,(derv(i,2),i=1,4)
 612  format(7x,f7.1,21x," |",4f8.2)
c   variation of +0.1% in scattered electron energy
      dpe=0.001*pe0
      derv(1,3)=w(e0,pe0+dpe,the0)-w0
      derv(2,3)=-dpe
      derv(3,3)=-dpe*cos((the0+thq0)*radfac)
      derv(4,3)=-dpe*sin((the0+thq0)*radfac)
      write(6,613)unity,(derv(i,3),i=1,4)
 613  format(14x,f7.1,14x," |",4f8.2)
c   variation of +1 mrad in proton angle
      dthp=0.001
      derv(1,4)=0.0
      derv(2,4)=0.0
      derv(3,4)=0.0
      derv(4,4)=q0*dthp
      write(6,614)unity,(derv(i,4),i=1,4)
 614  format(21x,f7.1,7x," |",4f8.2)
c   variation of +0.1% in proton momentum
      dpp=0.001*q0
      derv(1,5)=0.0
      derv(2,5)=-q0/sqrt(am**2+q0**2)*dpp
      derv(3,5)=-dpp
      derv(4,5)=0.0
      write(6,615)unity,(derv(i,5),i=1,4)
 615  format(28x,f7.1," |",4f8.2)
      write(6,610)
 610  format(/" now give your own de0, dthe, dpe, dthp, dpp;",
     1 /" the units are 0.1% for momenta and 1 mrad for angles",
     2 /" the input can be repeated; back to E0 with de0>=9/")     
c
c  if you find a set of variations that yield E-m=0 and p_m=0, this means
c  that you have to apply them with reversed signs, e.g. your real energy
c  was what you entered in this program MINUS de0, since your original
c  estimates were the wrong ones and the final ones are the correct ones,
c  while the program works the other way
c
  10  read(5,*)par
      if(par(1).gt.8.0) goto 1
      do 15 i=1,4
      diff(i)=0.0
  15  continue
      do 20 i=1,4
      do 20 j=1,5
      diff(i)=diff(i)+par(j)*derv(i,j)
  20  continue
      write(6,620)par, diff
 620  format(5f7.1," |",4f8.2)
      goto 10
      end
      function side3(x,y,t)
      data radfac/0.017453293/
      arg=x**2+y**2-2.0*x*y*cos(t*radfac)
      if (arg.lt.0.0) arg = 0.0
      side3=sqrt(arg)
      end
      function angle(x,y,z)
      data radfac/0.017453293/
      arg=(y**2+z**2-x**2)/(2.0*y*z)
      if (arg.lt.-1.0) arg=-1.0
      if (arg.gt. 1.0) arg= 1.0
      angle=acos(arg)/radfac
      end
      subroutine heepkin(e,the,pe,thq,q)
      data am/938.30/, radfac/0.017453293/
      pe=e/(1.0+2.0*e/am*sin(the*radfac/2.0)**2)
      q=side3(e,pe,the)
      thq=angle(pe,q,e)
      end
