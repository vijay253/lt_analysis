      program take_averages

c     This program takes averages of W and Q2 over different theta_pq
c     settings, then different polarities.
c
c     Input:  kindata/kindata.*.dat
c     Output: averages/averages.*.dat

!     call average_k(Q2,epslow,epshigh)
      call average_k(5.5,0.1838,0.5291)
      print*,  "-------------------------------------------------"
!      call average_k(4.4,0.4805,0.7148)
      print*,  "-------------------------------------------------"
!     call average_k(3.3,0.3935,0.6668)
      print*,  "-------------------------------------------------"
!     call average_k(3.3,0.5736,0.8791)
      print*,  "-------------------------------------------------"
!     call average_k(2.115,0.2477,0.7864)
      print*,  "-------------------------------------------------"
!     call average_k(0.5,0.4515,0.6979)

      stop
      end

*-----------------------------------------------------------------------

      subroutine average_k(q2_set,eps_lo_set,eps_hi_set)

c     Average W and Q2 over theta_pq settings, then over low and high epsilon
c     settings, then over neg. and pos. settings,
c     and save result in averages/aver.* .

      parameter (nu=3)

      real aveW(nu),errW(nu),aveQ2(nu),errQ2(nu)
      real avW(nu,2),erW(nu,2),avQ2(nu,2),erQ2(nu,2)
      real aW(nu,2,2),eW(nu,2,2),aQ2(nu,2,2),eQ2(nu,2,2)

      real avett(nu),    errtt(nu)    
      real avtt(nu,2),   ertt(nu,2)    
      real att(nu,2,2),  ett(nu,2,2)    


c      real thetacm_neg(nu),thetacm_pos(nu)
      real thetacm_only(nu)

      real eps_lo(nu),eps_hi(nu)

      real eps_set(2)

      integer pol_set(2)

      character*40 fn
      character*2 pol

      eps_set(1)=eps_lo_set
      eps_set(2)=eps_hi_set

      pol_set(1)=+1
      pol_set(2)=-1

      do it=1,nu

         aveW(it)=0.
         errW(it)=0.
         aveQ2(it)=0.
         errQ2(it)=0.

         avett(it)=0.
         errtt(it)=0.

         do ip=1,1

            avW(it,ip)=0.
            erW(it,ip)=0.
            avQ2(it,ip)=0.
            erQ2(it,ip)=0.
 
            avtt(it,ip)=0.
            ertt(it,ip)=0.

            do lh=1,2
               aW(it,lh,ip)=0.
               eW(it,lh,ip)=0.
               aQ2(it,lh,ip)=0.
               eQ2(it,lh,ip)=0.

               att(it,lh,ip)=0.
               ett(it,lh,ip)=0.

            end do

         end do

      end do

c     Get low, high eps. and neg., pos. polarity data.

      do ip=1,1

         do lh=1,2

            nset=0
            open(55,file='./root_ana/list.settings')
            do while(.true.)

               read(55,*,end=9) ipol,q2,eps,th_pq,tmn,tmx,nbt
               if(ipol.eq.pol_set(ip).and.q2.eq.q2_set.and.
     &              eps.eq.eps_set(lh)) then

                  if(ipol.eq.-1) then
                     pol='mn'
                  elseif(ipol.eq.+1) then
                     pol='pl'
                  else
                     stop '*** aver: wrong pol ***'
                  endif


                  

                  write(fn,'(''kindata/kindata.'',a2,''_'',i2.2,''_'',i2.2,
     *                 ''_'',SP,i5.4,S,''.dat'')')
     *                 pol,nint(q2_set*10.),nint(eps_set(lh)*100.),
     *                 nint(th_pq*1000.)
                  print*,'fn=',fn
c                 pause


c                 print*, 'aaaaaaaaaaaaaaaaaaaa '

                  open(66,file=fn)
!                  read(66,*) one


                  do it=1,nbt
 !                    read(66,*) W,dW,Q2,dQ2,tt,dtt
                     print*,W,dW,Q2,dQ2,it
                     if(dW.gt.0.) then
                        aW(it,lh,ip)=aW(it,lh,ip)+W/dW**2
                        eW(it,lh,ip)=eW(it,lh,ip)+1./dW**2
                     end if
                     if(dQ2.gt.0.) then
                        aQ2(it,lh,ip)=aQ2(it,lh,ip)+Q2/dQ2**2
                        eQ2(it,lh,ip)=eQ2(it,lh,ip)+1./dQ2**2
                     end if
                     if(dtt.gt.0.) then
                        att(it,lh,ip)=att(it,lh,ip)+tt/dtt**2
                        ett(it,lh,ip)=ett(it,lh,ip)+1./dtt**2
                     end if



                  end do
                  close(66)

                  tmin=tmn
                  tmax=tmx
                  ntbins=nbt

                  nset=nset+1

               end if           !ipol=pol_set & q2=q2_set & eps=eps_set

            end do              !while not eof.

 9          continue
            close(55)

            print*,'nset=',nset

         end do                 !lh=1,2

      end do                    !ip=1,2

      pause

      do ip=1,1
         do lh=1,2
            do it=1,ntbins
               if (eW(it,lh,ip).gt.0.) then
                  aW(it,lh,ip)=aW(it,lh,ip)/eW(it,lh,ip)
                  eW(it,lh,ip)=1./sqrt(eW(it,lh,ip))
*****                  eW(it,lh,ip)=0.001
               end if
               if(eQ2(it,lh,ip).gt.0.) then
                  aQ2(it,lh,ip)=aQ2(it,lh,ip)/eQ2(it,lh,ip)
                  eQ2(it,lh,ip)=1./sqrt(eQ2(it,lh,ip))
*****                  eQ2(it,lh,ip)=0.001
               end if

               if(ett(it,lh,ip).gt.0.) then
                  att(it,lh,ip)=att(it,lh,ip)/ett(it,lh,ip)
                  ett(it,lh,ip)=1./sqrt(ett(it,lh,ip))
*****                  eQ2(it,lh,ip)=0.001
               end if




c               write(*,'(4f8.5,2i3)') aW(it,lh,ip),eW(it,lh,ip),
c               aQ2(it,lh,ip),eQ2(it,lh,ip),it,lh
            end do
         end do
      end do
c      pause

c     Average over low and high epsilon.

      do ip=1,1
         do it=1,ntbins
            do lh=1,2
               if(eW(it,lh,ip).gt.0.) then
                  avW(it,ip)=avW(it,ip)+aW(it,lh,ip)/eW(it,lh,ip)**2
                  erW(it,ip)=erW(it,ip)+1./eW(it,lh,ip)**2
               end if
               if(eQ2(it,lh,ip).gt.0.) then
                  avQ2(it,ip)=avQ2(it,ip)+aQ2(it,lh,ip)/eQ2(it,lh,ip)**2
                  erQ2(it,ip)=erQ2(it,ip)+1./eQ2(it,lh,ip)**2
               end if


               if(ett(it,lh,ip).gt.0.) then
                  avtt(it,ip)=avtt(it,ip)+att(it,lh,ip)/ett(it,lh,ip)**2
                  ertt(it,ip)=ertt(it,ip)+1./ett(it,lh,ip)**2
               end if

            end do
         end do
      end do

      do ip=1,1
         do it=1,ntbins
            if(erW(it,ip).gt.0.) then
               avW(it,ip)=avW(it,ip)/erW(it,ip)
               erW(it,ip)=1./sqrt(erW(it,ip))
            end if
            if(erQ2(it,ip).gt.0.) then
               avQ2(it,ip)=avQ2(it,ip)/erQ2(it,ip)
               erQ2(it,ip)=1./sqrt(erQ2(it,ip))
            end if

            if(ertt(it,ip).gt.0.) then
               avtt(it,ip)=avtt(it,ip)/ertt(it,ip)
               ertt(it,ip)=1./sqrt(ertt(it,ip))
            end if


         end do
      end do

c     Average over neg. and pos. settings.
      do it=1,ntbins
         do ip=1,1
            if(erW(it,ip).gt.0.) then
               aveW(it)=aveW(it)+avW(it,ip)/erW(it,ip)**2
               errW(it)=errW(it)+1./erW(it,ip)**2
            end if
            if(erQ2(it,ip).gt.0.) then
               aveQ2(it)=aveQ2(it)+avQ2(it,ip)/erQ2(it,ip)**2
               errQ2(it)=errQ2(it)+1./erQ2(it,ip)**2
            end if


            if(ertt(it,ip).gt.0.) then
               avett(it)=avett(it)+avtt(it,ip)/ertt(it,ip)**2
               errtt(it)=errtt(it)+1./ertt(it,ip)**2
            end if
 

        end do
      end do

      do it=1,ntbins
         aveW(it)=aveW(it)/errW(it)
         errW(it)=1./sqrt(errW(it))
         aveQ2(it)=aveQ2(it)/errQ2(it)
         errQ2(it)=1./sqrt(errQ2(it))

         avett(it)=avett(it)/errtt(it)
         errtt(it)=1./sqrt(errtt(it))

      end do
      
c     Thetacm for neg. and pos. settings. It's turned out the same for
c     low and high epsilons, but different for negatives and positives.
c     So calculate for high eps., neg.-s and pos.-s.

c     Get Beam energy at first.
      Eb=0.
      open(55,file='beam/Eb_KLT.dat')
!      open(55,file='Eb_fpi2.dat')
      do while(.true.)
         read(55,*) Eb,q2,eps
         write(*,*) Eb,q2,eps
         if(q2.eq.q2_set.and.eps.eq.eps_hi_set) go to 5
      end do
 5    close(55)
      Eb=Eb/1000.               !Mev -> Gev units.
      print*,'xsect: Eb=',Eb,'   at Q2=',q2,'  eps=',eps,'  pol=',pol
      pause

c      do it=1,ntbins
c         tm=tmin+(it-0.5)*(tmax-tmin)/ntbins
c         call eps_n_theta(-1,Eb,aveW(it),aveQ2(it),tm,th_mod,eps_mod)
c         thetacm_neg(it)=th_mod*180./3.14159
c         call eps_n_theta(+1,Eb,aveW(it),aveQ2(it),tm,th_mod,eps_mod)
c         thetacm_pos(it)=th_mod*180./3.14159
c      end do

      do it=1,ntbins
         tm=tmin+(it-0.5)*(tmax-tmin)/ntbins
c         call eps_n_theta(-1,Eb,aveW(it),aveQ2(it),tm,th_mod,eps_mod)
c         thetacm_neg(it)=th_mod*180./3.14159

         call eps_n_theta(+1,Eb,aveW(it),aveQ2(it),avett(it),th_mod, 
     *  eps_mod)

         thetacm_only(it)=th_mod*180./3.14159

      end do






c     Save data.

      write(fn,'(''averages/avek.'',i3.3,''.dat'')')
     *     nint(q2_set*10.)
      print*,'fn=',fn
      print*

c      open(77,file=fn)
c      do it=1,ntbins
c         write(77,'(6f8.5,2f10.5,i3)')
c     *        aveW(it),errW(it),aveQ2(it),errQ2(it),
c     *        avett(it), errtt(it),
c     *        thetacm_neg(it),thetacm_pos(it),it
c



      open(77,file=fn)
      do it=1,ntbins
         write(77,'(6f8.5,f10.5,i3)')
     *        aveW(it),errW(it),aveQ2(it),errQ2(it),
     *        avett(it), errtt(it), thetacm_only(it),it





c         write(*,'(4f8.5,i3)') aveW(it),errW(it),aveQ2(it),errQ2(it),it
      end do
      close(77)

      end










*-----------------------------------------------------------------------

      subroutine eps_n_theta(npol,Eb,w,q2,tm,thetacm,eps)

c     To calculate model theta_pq in CM and epsilon. This subroutine is largely
c     based on theta_cm.f function, which in turn is based Jochen's script.

      implicit none

      integer npol
      real Eb,w,q2,tm,thetacm,eps

      REAL s,omega,q,tmin
      REAL p1cm,p3cm,e1cm,e3cm,p1lab

      REAL m2,m3,m4
      REAL m12,m22,m32,m42

      real mp,mp2,mpi,mpi2,mn,mn2,mw,mw2
      parameter (mp=.93827231)   !mp
      parameter (mp2=.88035493)  !mp^2
      parameter (mpi=.13956995)   !mpi
      parameter (mpi2=.01947977)  !mpi^2
      parameter (mn=.93956563)   !mn
      parameter (mn2=.88278357)  !mn^2

      parameter (mw=.78265)      !mw
      parameter (mw2=.61254)     !mw2



      parameter (m3=mw)
      parameter (m32=mw2)

      m2=mp
      m22=mp2
      m4=mp
      m42=mp2


      s=w*w
      omega=(s+q2-m22)/(2*m2)
      q=sqrt(q2+omega**2)
*     m12=q2    !error?
      m12=-q2   !mass squared of virtual photon.

      e1cm=(s+m12-m22)/(2*w)
      e3cm=(s+m32-m42)/(2*w)
      p1lab=q
      p1cm=p1lab*m2/w

c      print*,  e3cm, e3cm*e3cm, m32

      p3cm=sqrt(e3cm*e3cm-m32)
      tmin=-((e1cm-e3cm)**2-(p1cm-p3cm)**2)

c      print*, tm, tmin
c      stop

      if (tm.ge.tmin) then
         thetacm=2*asin(sqrt((tm-tmin)/(4*p1cm*p3cm)))
      else
         thetacm=-1.
         print*, 'eps_n_theta: *** tm=',tm,' <  tmin=',tmin,' ! ***'
      endif

      eps=1.+2.*(q2+omega**2)/(4.*Eb*(Eb-omega)-q2)
      eps=1./eps

      print *, "!!!!!!!!!!!!  " , eps

c      write(*,'(a13,7(F8.5,1x))')
c     *     'eps_n_theta: ',w,q2,t,tmin,thetacm,eps,omega

      end
