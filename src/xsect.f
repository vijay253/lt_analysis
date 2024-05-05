      program calc_xsect


      implicit none

c This script computes the experimental cross section using the ratio
c DATA/MC(Yields * CS(MC)

c      character*2 prv_it
c      common prv_it

c      integer q2_bin
c      integer u_bin, phi_bin
c      common u_bin, phi_bin
      
c     Get number of the previous iteration.
      
c      if(iargc().ne.1) then
c         print*,'*** usage: calc_xsect prv_it ***'
c         stop
c      end if
c      call getarg(1,prv_it)

c     Calculate unseparated cross-sections. Now settings are for the piplus data (+)

c      stop

      call xsect(+1,0.375,0.286)
      call xsect(+1,0.375,0.629)
      call xsect(+1,0.375,0.781)

      stop
      end

*=======================================================================

      subroutine xsect(npol_set,q2_set,eps_set)

      implicit none

      integer npol_set
      real q2_set,eps_set

      integer kset,nbin

      character*80 r_fn, kin_fn, xunsep_fn
      character*2 pol

      integer it,ip
      real Eb,eps

      real one

      integer nt,nphi
c      parameter (nt=3,nphi=8)
c      parameter (nt=2,nphi=10)

      real r,dr,w,dw,q2,dq2,tt,dtt,th_pos,th_cm
      real tm,tmn,tmx
      real um
      real eps_mod,th_mod,x_mod
      real x_real,dx_real

      integer ipol
      real th_pq

      real phi

      real, Dimension(9) :: t_bin_boundary

      real q2_bin

      integer u_bin, phi_bin
c      common  u_bin, phi_bin

      character*80:: line

      integer j
      real*8 pi

      parameter (pi=3.14159265) 

      include './kin_xyz.inc'

c   /*--------------------------------------------------*/
c   Read the u and phi bins 

      open (unit = 22, file = "t_bin_interval", action='read')
      read (22,*) q2_bin, u_bin, phi_bin
      nt = u_bin
      nphi = phi_bin 

c      read (22, '(A)') line  
c      read (22, '(A)') line  
c      print*,  line


c      read (line, end=20) 

c      print*,  trim(line)

c      read(line, *) u_bin_boundary(0), u_bin_boundary(1), u_bin_boundary(2)

c      read(line, *) (u_bin_boundary(j), j = 1, 3)

c      print*, u_bin_boundary(1)
c       read (22,*) 
c       read (22,*) q2_bin, u_bin, phi_bin
c       read (22,*) 
c 
c       print*,  u_bin, phi_bin

c      do j = 1, 3
c         print*, u_bin_boundary(j)
c      end do


      if(q2_set.eq.0.375) then

         read (22, '(A)') line  
         read(line, *) (t_bin_boundary(j), j = 1, u_bin + 1)

c        u_bin_boundary = (/ 0.0, 0.12,  0.20, 0.40/)
c        u_bin_boundary = (/0.0, 0.10, 0.17, 0.32/)
 
      elseif(q2_set.eq.2.45) then
 
c        u_bin_boundary = (/ 0.0, 0.212, 0.33, 0.60/)
c        u_bin_boundary = (/0.0, 0.19, 0.30, 0.50/)
 
         read (22,*) 
         read (22,*) 
         read (22, '(A)') line  
         read(line, *) (t_bin_boundary(j), j = 1, u_bin + 1)

      endif

c      stop

      close(22)

      ipol=0
      q2=0.
      eps=0.
      tmn=0.
      tmx=0.
      kset=0
c      open(55,file='./list.settings.omega')
      open(55,file='./list.settings.pion19')
      do while(ipol.ne.npol_set.or.q2.ne.q2_set.or.eps.ne.eps_set)
         read(55,*) ipol,q2,eps,th_pq,tmn,tmx,nbin,kset
c         write(6,2)ipol,q2,eps,th_pq,tmn,tmx,nbin,kset
c 2       format(i5,5f10.5,2i5)
      end do
      close(55)
      write(6,3)tmn,tmx,kset
 3    format(' tmn, tmx: ',2f10.5,'  for kset=',i5)
      if(tmn.eq.0..or.tmx.eq.0.) 
     *     stop '*** setting is not found in list.settings.fpi2'

      open(57,file='./Eb_pion19.dat')
      do while(.true.)
         read(57,*) Eb,q2,eps
         write(*,*) Eb,q2,eps
         if(q2.eq.q2_set.and.eps.eq.eps_set) go to 5
      end do
 5    close(57)
      Eb=Eb/1000.

c      Eb=Ebeam(kset)

      if(npol_set.lt.0) then
         pol='mn'
      else
         pol='pl'
      end if
c
c      write(6,4)Eb,q2,eps,pol
c 4    format(' xsect: Eb=',f8.5,'   at Q2=',f7.4,
c     *     '  eps=',f6.4,'  pol=',a2)

c     construct ratio data file name.

      write(r_fn,10) pol,nint(q2*1000),nint(eps*1000)
 10   format('averages/aver.',a2,'_',i3.3,'_',i3.3,'.dat')
      print*,'xsect: r_fn=',r_fn

      open(51,file=r_fn)

c     construct kinematics data file name.

      write(kin_fn,20) nint(q2*1000)
 20   format('averages/avek.',i3.3,'.dat')
      print*,'xsect: kin_fn=',kin_fn

      open(52,file=kin_fn)

*     construct output file name.
      write(xunsep_fn,30) pol,nint(q2_set*1000),nint(eps_set*1000)
 30   format('xsects/x_unsep.',a2,'_',i3.3,'_',i3.3)
      print*,'xsect: xunsep_fn=',xunsep_fn
c      pause

      open(61,file=xunsep_fn,status='replace')

      
      print*, ""
      print*, ""
      print*, ""
      print*, "Check for the reading file "
      print*, ""

      nbin = u_bin

      do it=1,nbin

c         tm=tmn+(it-0.5)*(tmx-tmn)/nbin


         tm = (t_bin_boundary(it) + t_bin_boundary(it+1)) / 2

         print *, "11112222  " , nbin, tm 

c         print *, nbin, u_bin_boundary(it+2), u_bin_boundary(nbin+1),  

c         stop


         read(52,*) w,dw,q2,dq2,tt,dtt,th_pos
         write(6,32) w,dw,q2,dq2,tt,dtt,th_pos
 32      format('xsect: ',7f10.4)


         th_cm=th_pos


         tm = tt

c         print *,  w,dw,q2,dq2,tt,dtt,th_pos

c        stop

         th_cm=th_cm*3.14159D0/180.D0
         

         print*, " "
         print*, " ", th_cm
         print*, " "

         do ip=1,nphi

            phi=(ip-0.5)*2.*3.14159/nphi
  
c            phi=(ip-1)*2.*3.14159/nphi
            read(51,*) r,dr

c            print *, "ratio checkaaaaaaa: ", r, dr


c            stop

            
c             print *, ip, r, dr, it, nphi, nbin
c             print *, "~~ ", w, dw, q2, dq2, th_pos
c             print *, "== ", tm, phi
c             print *, "-- ", npol_set, Eb, q2_set, w, q2, tm, phi



c            print*, q2_set, um

c            stop

 
            call xmodel(npol_set,Eb,q2_set,w,q2,tm,phi,
     *           eps_mod,th_mod,x_mod)

c            call xmodel_ghm_two_model(npol_set,Eb,q2_set,w,q2,tm,phi,
c     *           eps_mod,th_mod,x_mod)
            

c           stop


c            if (q2_set.eq.1.6) then
c                
c               print*, "Q2=1.6 parameterization is used! "
c
c                call xmodel_ghm_160(npol_set,Eb,q2_set,w,q2,um,tm,phi,
c     *           eps_mod,th_mod,x_mod)
c
cc                stop
c
c            else if (q2_set.eq.2.45) then
c
c               print*, "Q2=2.45 parameterization is used! "
c
c                call xmodel_ghm_245(npol_set,Eb,q2_set,w,q2,um,tm,phi,
c     *           eps_mod,th_mod,x_mod)
c
c            else
c                print*, "No parameterization is aviliable for Q2=", q2_set
c                stop
c            endif
            
c            print*,"testtest" , q2_set 
c            stop

c            call xmodel_ghm(npol_set,Eb,q2_set,w,q2,um,tm,phi,
c     *           eps_mod,th_mod,x_mod)


c            call xmodel_ghm(npol_set,Eb,q2_set,w,q2,um,tm,phi,
c     *           eps_mod,th_mod,x_mod)



c            stop


cc /*--------------------------------------------------*/
cc angle check
cc angle check is taken off temporarily 
cc WL 31/March/2016


c           if (abs(th_mod-th_cm).gt.1.e-4) then
c              write(6,*)' Angle error ',th_mod,th_cm
c              stop
c           endif
c
c

c /*--------------------------------------------------*/
c cross-section
c ratio is data/simc - see GH logbook, p.55

            if (r.gt.2.0) then
               r=0.0
               dr=0.0
            endif

             x_real=x_mod*r
c             x_real=x_mod
c             dx_real=x_mod*dr/r
             dx_real=x_mod*dr
c             dx_real=x_mod*2/100
c             dx_real=2.823

             if (x_real.eq.0.0) then
                dx_real = -1000
c                x_real = 0
c                dx_real = 0
             endif
            


c             dx_real=x_mod*dr

c            x_real=x_mod
c            dx_real=0.01




            print*, "JJJJJJJJ  ratio check ", r, x_mod, x_real 



cc ratio is simc/data - see GH logbook, p.55
c            if (r.ne.0) then
c               x_real=x_mod/r
c               dx_real=x_mod*dr/r**2
c            else
c               x_real=0.
c               dx_real=0.
c            endif


c           print*, "ratio check ", r, x_mod, x_real 


c   /*--------------------------------------------------
c   This is where data output to file happens


            
            print*, "kkkkkk  ", x_mod, eps_mod, th_mod 

            write(61,40) x_real,dx_real,x_mod,eps_mod,
     *           th_mod*180./pi,phi*180./pi,tm,w,q2
 40         format(3G15.5,f8.5,2f7.2,4f8.5)

         end do                 !phi


c         stop 
         
c         print*, x_real,dx_real,x_mod,eps_mod, 80./3.14159,phi*180./3.14159,tm,um,w,q2

c         stop

c        Write out kinematics for Henk.
c         if(npol_set.gt.0) write(99,'(5f8.3,2x,2f6.2)')
c     *   w,q2,eps_mod,th_mod*180./3.14159,tm,eps_set,q2_set
      end do                    !t

      close(51)
      close(52)
      close(61)
      print*,' '

      end

*=======================================================================

      subroutine xmodel(npol_set,Eb,q2_set,w,q2,tm,phi,
     *     eps_mod,th_mod,x_mod)

      integer npol_set
      real Eb,q2_set,w,q2,tm,eps_mod,th_mod, thetacm, x_mod

      real*8 sig
      real*8 t_gev,tprime_gev,q2_gev,w_gev,eps,tp
      real*8 lambda0_sq,lambdapi_sq,alphapi_t,alphapi_0
      real*8 a,b,c,d,e,f,fpi,fpi235, tav, ftav, ft
      real*8 m_pi0sq
      real*8 phicm, phi pi, mp

c     Fit parameters (V.K.) 
      integer npar,ipar
      parameter (npar=12) 
      real*8 fitpar(npar),par,par_er
      save fitpar
c      open(88,file='parameters/it8/par.pl')      
c      open(88,file='simc_gfortran/old_parameters/start_par/par.pl')      
      open(88,file='simc_gfortran/par.pl',status='old')      
      do while(.true.)
         read(88,*,end=99) par,par_er,ipar
         fitpar(ipar)=par
      end do
 99   close(88)
      

      pi = 3.1415926
      mp=0.93827231             !mp

      call eps_n_theta(npol_set, Eb, w, q2, tm, thetacm, eps_mod)
      
c       print*, "check   ", thetacm
c 
c       stop

c      print*,"*****", npol_set, Eb, w, q2, tm, thetacm, eps_mod


      phicm = phi

      tprime_gev = tm

      tp = abs(tm)      ! just to make sure it's positive

c      if (abs(t_gev)<tp) then
c         write(6,*)' invalid -t>-tprime error',abs(t_gev),tp
c         stop
c      endif

c      m_pi0sq= Mpi02/1.e6
      m_pi0sq= (0.13957018**2)/1.e6

* Normally, this would equal m_rho**2, but Laget has adjusted the value
* to reproduce the real photon data.
      lambda0_sq= 0.462         !GeV^2

* Pion saturating Regge trajectory.
* For t>(m_pi**2), the trajectory takes the usual form, 0.7(t-m_pi**2).
* As t --> -infty, the trajectory goes asymptotically to -1.
* Laget does not mention which function he uses for the asymptotic
* behavior, but I find that taking the hyperbolic tangent of the usual
* trajectory expression gives a curve which resembles the one in
* Laget's paper.
      if (t_gev>m_pi0sq) then
         alphapi_t=0.7*(t_gev-m_pi0sq)
      else
         alphapi_t=tanh(0.7*(t_gev-m_pi0sq))
      endif

* Use these instead of the usual Q^2 scaling of the response functions.
      alphapi_0=0.7*(0.-m_pi0sq) 
      lambdapi_sq=lambda0_sq*((1.+alphapi_t)/(1.+alphapi_0))**2
c      fpi    = 1./(1.+q2_gev/lambdapi_sq)
      fpi    = 1./(1.+q2/lambdapi_sq)
c      fpi235 = 1./(1.+2.35/lambdapi_sq)
      fpi375 = 1./(1.+0.375/lambdapi_sq)

* Fit parameters to t-dependence of Laget's p(e,e'p)omega response
* functions at Q^2=2.35, W=2.47 [ub/GeV^2].  Before fitting, I first
* divided the response functions by (fpi/fpi235)^2 and any sin(thetacm)
* factors.

c===========================
c hh Vijay
c===========================
c it0
c      do ipar=1,npar
c         print*,fitpar(ipar),ipar
c      end do      
      
c      a =  0.25961E+02
c      b = -0.10000E+02  
c      c = -0.15838E+02
c      d =  0.00000E+00
c it1
c      a = 24.5738
c      b = -11.2538 
c      c = -15.8380
c      d = -11.6768
c it2
c      a = 8.3738
c      b = -1.4386 
c      c = -15.8380
c      d = -17.3067
c test
      a = 20.7692
      b = -4.7067 
      c = -15.8380
      d = -13.7974

c      sigL = ((a+b*log(q2))*exp((c+d*log(q2))*(tp-0.2)))
c      sigL = ((fitpar(5)+fitpar(6)*log(q2))
c     1     *exp((fitpar(7)+fitpar(8)*log(q2))*(tp-0.2)))
c      sigL = ((fitpar(5)+fitpar(6)*log(q2))
c     1     *exp((fitpar(7)+fitpar(8)*log(q2))*(tp)))
c      sigL = ((fitpar(5)+fitpar(6)*log(q2))
c     1     *exp((fitpar(7)+fitpar(8)*log(q2))*tp))

      sigl = fitpar(5)*exp(fitpar(6)*(tp))
c it0
c      a =  0.46859E+02
c      b = -0.30000E+02 
c      c = -0.33572E+01
c      d =  0.00000E+00
c it1
c      a = 46.85900
c      b = -75.5521
c      c = 1.8041
c      d = -5.2755
c it2
c      a = 46.85900
c      b = -100.0000
c      c = 28.5507
c      d = -32.5449

c test
      a = 46.85900
      b = -96.4655
      c = -0.5225
      d = -2.8901

      tav = (0.0735+0.028*log(q2_set))*q2_set
      ftav = (abs(tp) - tav)/tav
      sigT = fitpar(1)+fitpar(2)*ftav
c      sigT=fitpar(1)+fitpar(2)*log(q2)+(fitpar(3)+fitpar(4)*log(q2))
c     1     *ftav
c      sigT=fitpar(1)*(fitpar(2)*tp)*exp(fitpar(3)*tp)

c it0      
      a =  0.10000E+00
      b = -0.28000E+02
      c =  0.35000E+01
c it1
c      a = -347.0861
c      b = -99.4037
c      c = -3.2874
c it2
c      a = 3078.3235
c      b = -36.5664
c      c = -37.2869

c test
c      a = -347.0861
c      b = -99.4037
c      c = -3.2874
   
c      sigLT = ((a*exp(b*(-tp))+c/(tp))*sin(thetacm))
      sigLT = ((fitpar(9)*exp(fitpar(10)*(tp))+fitpar(11)/(tp))
     1     *sin(thetacm))

c      sigLT = 0
c      print*,"vija", thetacm
c it0
      a = -0.67276E+02
c it1
c      a = -130.8499
c it2
c      a = -150.9493
c test
c      a = -130.8499

      ft = tp/(abs(tp) + 0.139570**2)**2
c      sigTT = ((a/q2*exp(-q2))*ft*sin(thetacm)**2)
      sigTT = ((fitpar(12)/q2*exp(-q2))*ft*sin(thetacm)**2)
c      sigLT = ((fitpar(9)/q2*exp(-q2))*ft*sin(thetacm))

c      sigTT =0
      print*, sigL, sigT, sigLT, sigTT
c      print*, sigLT

c===========================

* Since I have nothing better to go on, for now I assume W scales as
* 1/(W^2-mp^2)^2.
c      wfactor=(2.2002**2-mp**2)**2/(w**2-mp**2)**2
      wfactor= 1.0/(w**2-mp**2)**2

      sig = sigT + eps_mod*sigL+(eps_mod*cos(2.*phicm)*sigTT)
     *     +sqrt(2.*eps_mod*(1.+eps_mod))*(cos(phicm))*sigLT
     
c      sig = sig/2./pi/1.d+06      !dsig/dtdphicm in microbarns/MeV^2/rad

c      sig = sig*wfactor
c      sig = sig
c      sig = sig/2./pi/1.d+06           !dsig/dtdphicm in microbarns/GeV^2/rad
c      sig = sig/1.d+06           !dsig/dtdphicm in microbarns/GeV^2/rad

      x_mod = sig     
      th_mod=thetacm

c      if (phi.lt.0.3) then
c         write(6,102) eps_mod,tm,sigL,sigT,sigTT,sigLT, x_mod
c 102     format( ('xmodel: eps=',f5.3,' t=',f5.3,' sigL=',f7.2,' sigT=',f6.2,
c     1        ' sigTT=',f5.2,' sigLT=',f5.2,' x_mod=',f10.6) )
c     endif
      end
      include 'eps_n_theta.f'

