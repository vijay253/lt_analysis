! elas_kin.f
! written by Dave Gaskell
! cross sections are based on parameterization of Walker data

      program elas_kin

      implicit none
      
      real*8 mn,pi,ei
      real*8 theta,scat,q2,ep,eps,nu,tau,mott,recoil
      real*8 alpha,barn
      
      real*4 GEp,GEn,GMp,GMn,qsq
      real*8 w1p,w2p,csp
      
      real*8 s2,t2,q3sq,q3,thetaq
      
      integer iff,i
      
      data mn/0.938272/
c     data mn/1.875612/
      
c     nucleon mass = 0.938272 GeV/c**2
      data pi/3.14159/
C     alpha = 1/137, barn:(1 GeV)**-2 = 0.389e-3 barn
      data alpha,barn/7.2993e-3,0.389e-3/
      
      write(6,*) 'Input electron beam energy in GeV.'
      read(5,*) ei
c     type *,'Input scattering angle (degrees).'
c     accept *,theta
c     theta =14.0
      write(6,*)' Electron beam energy: ',ei,' GeV'
      
      write(6,790)
 790  format(/,t3,'Th_e',t14,'Q^2',t24,'E_e',t32,'Th_p',t44,'P_p',
     1     t55,'Mott',t62,'Sig_p (fm^2/sr)')
      do i=0,1000
         theta = 7.*pi/180 + i/20.*pi/180.
         
C     Calculate Q**2
         q2 = (4.*(ei*sin(abs(theta)/2))**2)/
     $        (1+2.*ei*(sin(abs(theta)/2))**2/mn)
         qsq = q2
         
         ep = ei - q2/(2*mn)
         theta = 2*asin(sqrt(q2/4/ei/ep))
         scat = theta*180./pi
         
         eps = 1./(1.+2.*(1.+nu**2/q2)*tan(theta/2.)**2)
         
         s2=sin(abs(theta)/2.)**2
         
c     Calculate energy transfer
         nu = ei-ep
         
         tau = nu/(2.*mn)
         
c     Calculate Mott cross section in barn/(GeV-sr)
         mott = ((alpha * cos(abs(theta)/2.) / (2. * ei*s2) )**2)*barn
         mott = mott * 100	!(in fm**2/sr)
c     type *,'sigma_Mott = ',mott,' fm**2/sr'
         
c     Calculate recoil factor
         recoil = ep/ei
c     type *,'recoil = ',recoil
         
c     Estimate form factors for neutron and proton.
         
c     reference for form factor parametrization
cdg	write(6,100)
cdg  	format ('IG = 1 - DIPOLE WITH FORM FACTOR SCALING AND GEN=0.0',/,
cdg	1'     2 - IJL FIVE PARAMETER MODEL FIT                        ',/,
cdg	1'     3 - GEP AND GMP FROM IJL, GMN=DIPOLE, GEN=GALSTER       ',/,
cdg	1'     4 - BEST FIT EXCEPT GEN = 0.0                           ',/,
cdg	1'     5 - BLATNIK + ZOVKO VDM FIT                             ',/,
cdg	1'     6 - JANNSENS 1966 STANDARD FIT                          ',/,
cdg	1'     7 - DIPOLE + F1N = 0.0                                  ',/,
cdg	1'     8 - GES = 0.0,  GMS = 1.0                               ',/,
cdg	1'     9 - GES = 1.0,  GMS = 0.0                               ',/,
cdg	1'    10 - HOHLER1 - PROTON AND NEUTRON FROM FIT 8.2           ',/,
cdg	1'    11 - HOHLER2 - PROTON FROM FIT 5.3, NEUTRON FROM 8.2     ',/,
cdg	1'    12 - GARI + KRUMPELMANN, Z PHYS. A322,689(1985)          ',/,
cdg	1'    13 - KORNER + KURODA, PHYS. REV. D16,2165(1977)          ',/,
cdg	1'    14 - Gari+Kruempelmann & dipole for Gep                  ',/,
cdg	1'    15 - Best fit of Walker data (PRD 49, p. 5681)           ')
         
         iff =15
c         call nform(iff,qsq,GEp,GEn,GMp,GMn)
c        write(6, *) 'GEn:',GEn,'GMn:',GMn,'GEp:',GEp,'GMp:',GMp
         
         t2 = tan(abs(theta)/2.)**2
         
c     Calculate elastic cross sections for protons 
         w1p = GMp**2 * tau
         w2p = (GEp**2 + tau * GMp**2)/(1 + tau)
         csp = mott * recoil * (w2p + 2 * w1p * t2)
c        type *,'proton cross section = ',csp,' fm**2/sr'
c        type *,'nuW2 = ',nu*w2p
         
c        w1n = GMn**2 * tau
c        w2n = (GEn**2 + tau * GMn**2)/(1 + tau)
c        csn = mott * recoil * (w2n + 2 * w1n * t2)
c        type *,'neutron cross section = ',csn,' fm**2/sr'
         
         q3sq = q2 + nu**2
         q3 = sqrt(q3sq)
         thetaq = -asin(ep*sin(theta)/q3)
c        write(6,*) 'Q**2 = ',q2,'nu = ',nu
c        write(6,*) 'ef = ',ep,'q3 = ',q3
c        write(6,*) 'thetaq = ',thetaq*180/3.14159
         
         write(6,3) scat,q2,ep,thetaq*180./3.14159,q3,mott,csp
      enddo
      
 3    format(5(f7.3,4x),2(E10.3E2,4x))
      
      end
