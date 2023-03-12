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

c      write(r_fn,10) pol,nint(q2*1000),nint(eps*1000)
c 10   format('averages/aver.',a2,'_',i3.3,'_',i3.3,'.dat')
c      print*,'xsect: r_fn=',r_fn

c      open(51,file=r_fn)

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


c         tm = tt


c         print *,  w,dw,q2,dq2,tt,dtt,th_pos

c        stop

         th_cm=th_cm*3.14159D0/180.D0
         

         print*, " "
         print*, " ", th_cm
         print*, " "

         do ip=1,nphi

            phi=(ip-0.5)*2.*3.14159/nphi
c            phi=(ip)*22.5*pi/180
            print*, "DDDDDDDDDD ", phi
  
c            phi=(ip-1)*2.*3.14159/nphi
c            read(51,*) r,dr

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

c ratio is data/simc - see GH logbook, p.55

c             x_real=x_mod*r
             x_real=x_mod
c             dx_real=x_mod*dr/r
             dx_real=x_mod

             if (x_real.eq.0.0) then
                dx_real = -1000
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
      real*8 a,b,c,d,e,f,fpi,fpi235
      real*8 m_pi0sq
      real*8 phicm, phi pi

      pi = 3.1415926

      call eps_n_theta(npol_set, Eb, w, q2, tm, thetacm, eps_mod)
      
c       print*, "check   ", thetacm
c 
c       stop

c      print*,"*****", npol_set, Eb, w, q2, tm, thetacm, eps_mod


      phicm = phi

      tprime_gev = tm

      tp = abs(tprime_gev)      ! just to make sure it's positive

c      if (abs(t_gev)<tp) then
c         write(6,*)' invalid -t>-tprime error',abs(t_gev),tp
c         stop
c      endif

      m_pi0sq= Mpi02/1.e6

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
      fpi235 = 1./(1.+2.35/lambdapi_sq)
      
* Fit parameters to t-dependence of Laget's p(e,e'p)omega response
* functions at Q^2=2.35, W=2.47 [ub/GeV^2].  Before fitting, I first
* divided the response functions by (fpi/fpi235)^2 and any sin(thetacm)
* factors.
      a = 0.16675 
      b = 0.89524 
      c = 3.5991
      d = 6.4562
      e = -9.6199
      f = 5.8319
      sigL = a*exp(-b*tp)+c*exp(-d*(tp**0.5))+e*exp(-f*(tp**0.25))
c...this is a new model
c......................
      t0  =    -14.33725
      t1  =      -0.1419
      t2  =       0.2082
      t3  =       0.0000
      l0  =      11.5206
      l1  =      -0.1996
      l2  =       0.0749
      l3  =       0.0000
      lt0 =      -0.0209
      lt1 =       0.3192
      lt2 =       0.0000
      lt3 =       0.0000
      tt0 =       0.0864
      tt1 =      -1.7213
      tt2 =       0.0000
      tt3 =       0.0000
      
      sigl = l0 * (tp + l1)*(tp + l1) + l2
      sigt = t0 * (tp + t1)*(tp + t1) + t2 
      siglt= (lt0+lt1*tp+lt2*log(q2)+lt3*tp*log(q2))*sin(thetacm)
      sigtt=(tt0+tt1*tp+tt2*log(q2)+tt3*tp*log(q2))*sin(thetacm)**2
      wfactor=((2.47**2-m_p**2)**2)/((w**2-m_p**2)**2)
      sigl = sigl*wfactor
      sigt = sigt*wfactor
      siglt= siglt*wfactor
      sigtt= sigtt*wfactor
c      sig = sigt + eps_mod*sigl +eps_mod*cos(2.*phicm)*sigtt
c     *     + sqrt(2.*eps_mod*(1.+eps_mod))*cos(phicm)*siglt

c.........................................................
c      print*, fpi, fpi235

c      sigL = sigL *(fpi/fpi235)**2
      sigL = sigL

c      print*,"asdasd", sigL

      a = -0.12286
      b = 0.56383
      c = 1.4673
      d = 2.1988
      e = 0.65170
      f = 18.501
      sigT = a*exp(-b*tp)+c*exp(-d*(tp**0.5))+e*exp(-f*(tp**2))
c      sigT = sigT *(fpi/fpi235)**2
      sigT = sigT

      a = 0.46397
      b = 4.9179 
      c = 3.3023
      d = 3.1741
      sigLT = a*exp(-b*tp)+c*exp(-d*(tp**0.5))
c      sigLT = sigLT*(fpi/fpi235)**2*sin(thetacm)
      sigLT = sigLT*sin(thetacm)
* Laget uses -sqrt(e(1+e)) instead of +sqrt(2e(1+e))
      sigLT = -sigLT/sqrt(2.)

      a = -0.26497
      b = 2.7655 
      c = 2.8034
      d = 3.8586
      sigTT = a*exp(-b*tp)+c*exp(-d*(tp**0.5))
c      sigTT= sigTT*(fpi/fpi235)**2*(sin(thetacm))**2
      sigTT= sigTT*(sin(thetacm))**2

* Since I have nothing better to go on, for now I assume W scales as
* 1/(W^2-mp^2)^2.
      wfactor=(2.47**2-mp**2)**2/(w**2-mp**2)**2
c      wfactor=1./(w**2-mp**2)**2

      sigL = sigL*wfactor
      sigT = sigT*wfactor
      sigLT= sigLT*wfactor
      sigTT= sigTT*wfactor

      sig = sigT + eps_mod*sigL + eps_mod*cos(2.*phicm)*sigTT
     *     +sqrt(2.*eps_mod*(1.+eps_mod))*cos(phicm)*sigLT
     
      sig = sig/2./pi/1.d+06      !dsig/dtdphicm in microbarns/MeV^2/rad

c      sig = sig/2./pi      !dsig/dtdphicm in microbarns/GeV^2/rad

      x_mod = sig     
      th_mod=thetacm

c      if (phi.lt.0.3) then
c         write(6,102) eps_mod,tm,sigL,sigT,sigTT,sigLT, x_mod
c 102     format( ('xmodel: eps=',f5.3,' t=',f5.3,' sigL=',f7.2,' sigT=',f6.2,
c     1        ' sigTT=',f5.2,' sigLT=',f5.2,' x_mod=',f10.6) )
c     endif
      end
      include 'eps_n_theta.f'

