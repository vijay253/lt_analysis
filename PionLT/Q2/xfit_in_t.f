
      program xfit_in_t

      implicit none

      call xfit_sigL('pl','375',1,8)

      stop 
      end
c =====================================================================

      subroutine xfit_sigL(pol,q2set,t1,t2)
      
c Fit sigL in t 
      
      implicit double precision (a-h,o-z)
     
      character*2 pol
      character*3 q2set
      integer t1,t2
      
      external fcn_sigL
      
c      character*2 prv_it
c      common/prv_it/prv_it
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
c      dimension prv_par(12)
      data prv_par/12*0.D0/
      
      pol_set=pol
      q2_set=q2set
      tlow=t1
      thi=t2
      ntot=thi-tlow+1
      
c Read previous fit parameters.
c      if(prv_it.ne.'N') then
c         open(77,file='fit_params/it'//prv_it//'/par.'//pol//'_060')
      open(77,file='parameters/itt_0/par.pl_375')
c         open(77,file='parameters/itt_0/par.'//pol//'_'//q2set//
c     1        '/par.'//pol//'_'//q2set)
         do while(.true.)
            read(77,*,end=1) p,e,i
            print*,p,e,i
            prv_par(i)=p
         end do
 1         close(77)
c      endif
      
      call mncomd(fcn_sigL,'clear',icond,0)
      
c Start from the previous fit parameters.

      call mnparm(1,'sigL P1',prv_par(1),0.1D0,0.D0,0.D0,icond)
      call mnparm(2,'sigL P2',prv_par(2),0.1D0,0.D0,0.D0,icond)
      call mnparm(3,'sigL P3',prv_par(3),0.1D0,0.D0,0.D0,icond)
      call mnparm(4,'sigL P4',prv_par(4),0.1D0,0.D0,0.D0,icond)
      call mnparm(5,'sigT P5',prv_par(5),0.1D0,0.D0,0.D0,icond)
      call mnparm(6,'sigT P6',prv_par(6),0.1D0,0.D0,0.D0,icond)
      call mnparm(7,'sigT P7',prv_par(7),0.1D0,0.D0,0.D0,icond)
      call mnparm(8,'sigT P8',prv_par(8),0.1D0,0.D0,0.D0,icond)
      call mnparm(9,'sigLT P9',prv_par(9),0.1D0,0.D0,0.D0,icond)
      call mnparm(10,'sigLT P10',prv_par(10),0.1D0,0.D0,0.D0,icond)
      call mnparm(11,'sigLT P11',prv_par(11),0.1D0,0.D0,0.D0,icond)
      call mnparm(12,'sigTT P12',prv_par(12),0.1D0,0.D0,0.D0,icond)

c       pause
      
*      call mncomd(fcn_sigL,'set pri -1',icond,0)

      call mncomd(fcn_sigL,'set pri 1',icond,0)
      
c Fit sigL.
      
      call mncomd(fcn_sigL,'rel 1',icond,0)
      call mncomd(fcn_sigL,'fix 2',icond,0)
      call mncomd(fcn_sigL,'rel 3',icond,0)
      call mncomd(fcn_sigL,'fix 4',icond,0)
      call mncomd(fcn_sigL,'fix 5',icond,0)
      call mncomd(fcn_sigL,'fix 6',icond,0)
      call mncomd(fcn_sigL,'fix 7',icond,0)
      call mncomd(fcn_sigL,'fix 8',icond,0)
      call mncomd(fcn_sigL,'fix 9',icond,0)
      call mncomd(fcn_sigL,'fix 10',icond,0)
      call mncomd(fcn_sigL,'fix 11',icond,0)
      call mncomd(fcn_sigL,'fix 12',icond,0)

c      pause
      
      call mncomd(fcn_sigL,'cali 1',icond,0)
c      pause
      call mncomd(fcn_sigL,'set gra',icond,0)
c      pause
      call mncomd(fcn_sigL,'migrad',icond,0)
c      pause
      call mncomd(fcn_sigL,'minos',icond,0)
c      pause
      call mncomd(fcn_sigL,'return',icond,0)
c      pause
      
*      call mnhelp

      end

*--------------------------------------------------------------------------

      subroutine fcn_sigL(npar,grad,fval,xval,iflag,futil)
      implicit double precision (a-h,o-z)

c Fit sigL
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
      dimension grad(12),xval(12)

      parameter (targ=938.27231) ! for piplus channel

      real WW,QQ2
c      dimension tm(6),x(6),dx(6),W(6),Q2(6)
      dimension tm(8),x(8),dx(8),W(8),Q2(8)
      save tm,x,dx,W,Q2

      character*40 fn

c      real weight(8)
      real weight(4)
      save weight

      g(Wi)=1./(Wi**2-(targ/1.D+3)**2)**2

c      print*,'=== in FCN: npar=',npar,'   iflag=',iflag
c      pause
      
      if(iflag.eq.1) then
         
c         fn='xsects/x_sep.'//pol_set//'_'//q2_set
         fn='xsects/x_sep.pl_375'
         print*,fn
         open(31,file=fn)
         
c         fn='averages/avek.'//q2_set//'.dat'

         fn='averages/avek.375.dat'
         print*,fn
         open(32,file=fn)
         
         itot=0
         do i=1,thi
            read(31,*) sigT,dsigT,sigL,dsigL,
     1           sigTT,dsigTT,sigLT,dsigLT,x2,t
            read(32,*) WW,dW,QQ2,dQ2
            if (i.ge.tlow) then
               itot=itot+1
               x(itot)=sigL
               dx(itot)=dsigL
               tm(itot)=t
               W(itot)=WW
               Q2(itot)=QQ2
c               weight(itot)=weigh(i,pol_set,q2_set)
c               weight(itot)=weigh(i,pol_set,q2_set)

               write(*,'(5f7.3,i4)')x(itot),dx(itot),
     1              W(itot),Q2(itot),tm(itot),itot

            endif
         end do

         close(31)
         close(32)
                   
      end if                  !iflag=1
      
      fval=0.d0
      if(iflag.eq.2) then
         do i=1,12
            grad(i)=0.d0
         end do
      end if
      
      p1=xval(1)
      p2=xval(2)
      p3=xval(3)
      p4=xval(4)
      
      do i=1,ntot
         
         if(iflag.eq.3.and.(i-1+ntot)/ntot*ntot.eq.i-1+ntot) then
            j=(i-1+ntot)/ntot
            fn='xsects/sigl_fit.'//pol_set//'_'//q2_set
            open(77,file=fn,status='replace')
         end if
         
         xs=x(i)
         er=dx(i)
         qsq=Q2(i)
         
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c         x_tw=exp((p3+p4*log(qsq))*abs(tm(i)))*g(W(i))
c	 sigL=(p1+p2*log(qsq))*x_tw

* Revised for IT22, 12.11.06
         x_tw=exp((p3+p4*log(qsq))*(abs(tm(i))-0.2))*g(W(i))
	 sigL=(p1+p2*log(qsq))*x_tw

         prv_sigL=(prv_par(1)+prv_par(2)*log(qsq))
     *        *exp((prv_par(3)+prv_par(4)*log(qsq))*(abs(tm(i))-0.2))
     *        *g(W(i))

c Save fit cross-section, bin by bin.
         if(iflag.eq.3) write(77,'(E12.5,f8.5,E13.5)') sigL,tm(i),prv_sigL
         if(i/ntot*ntot.eq.i) close(77)
         
c difference between sigL from xfit_in_phi and previous iteration model
         dif=sigL-xs
	 if (iflag.eq.1 .or. iflag.eq.3) print*,' L',i,sigL,xs,dif
         
c         term=dif/er*weight(i)
c         fval=fval+term*term
         fval=fval
         
         if(iflag.eq.2) then

* Vector of first derivatives corresponding to
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c            grad(1)=grad(1)+2.*term/er*x_tw 
c            grad(2)=grad(2)+2.*term/er*log(qsq)*x_tw 
c            grad(3)=grad(3)+2.*term/er*sigL*(abs(tm(i))) 
c            grad(4)=grad(4)+2.*term/er*sigL*log(qsq)*(abs(tm(i))) 

* Revised for IT22, 12.11.06
            grad(1)=grad(1)+2.*term/er*x_tw 
            grad(2)=grad(2)+2.*term/er*log(qsq)*x_tw 
            grad(3)=grad(3)+2.*term/er*sigL*(abs(tm(i))-0.2) 
            grad(4)=grad(4)+2.*term/er*sigL*log(qsq)*(abs(tm(i))-0.2) 

         endif
         
      end do
      
      if(iflag.eq.3) then
         
c Save separated fit parameters.
         
         chi2=0.
         ndf=ntot-npar
         if(ndf.ne.0) chi2=fval/ndf

         print*,' chi2=',chi2,' ndf =',ndf
         print*,' ntot',ntot,' npar =',npar
         
         call mnerrs(1,epl,emn,dp1,glcc)
         call mnerrs(2,epl,emn,dp2,glcc)
         call mnerrs(3,epl,emn,dp3,glcc)
         call mnerrs(4,epl,emn,dp4,glcc)
         
         fn='fit_params/par.'//pol_set//'_'//q2_set
         stat=system('rm '//fn)
         print*,'Status of remove:',stat
         open(77,file=fn,status='replace')

         write(77,10) p1,dp1,1,chi2
         write(77,10) p2,dp2,2,chi2
         write(77,10) p3,dp3,3,chi2
         write(77,10) p4,dp4,4,chi2
         
         close(77)
         
      end if                  !iflag=3
      
 10   format(2(E12.5,1x),1x,i2,f15.1)
      end

*==========================================================================

      subroutine xfit_sigT(pol,q2set,t1,t2)
      
c Fit sigT in t
      
      implicit double precision (a-h,o-z)
      
      character*2 pol
      character*3 q2set
      integer t1,t2
      
      external fcn_sigT
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
      pol_set=pol
      q2_set=q2set
      tlow=t1
      thi=t2
      ntot=thi-tlow+1

      call mncomd(fcn_sigT,'set pri 1',icond,0)
      
c Fix sigL fit parameters, release sigT fit parameters.
      
      call mncomd(fcn_sigT,'fix 1',icond,0)
      call mncomd(fcn_sigT,'fix 2',icond,0)
      call mncomd(fcn_sigT,'fix 3',icond,0)
      call mncomd(fcn_sigT,'fix 4',icond,0)
      call mncomd(fcn_sigT,'rel 5',icond,0)
      call mncomd(fcn_sigT,'fix 6',icond,0)
      call mncomd(fcn_sigT,'rel 7',icond,0)
      call mncomd(fcn_sigT,'fix 8',icond,0)
      call mncomd(fcn_sigT,'fix 9',icond,0)
      call mncomd(fcn_sigT,'fix 10',icond,0)
      call mncomd(fcn_sigT,'fix 11',icond,0)
      call mncomd(fcn_sigT,'fix 12',icond,0)

c      pause
      
      call mncomd(fcn_sigT,'cali 1',icond,0)
c      pause
      call mncomd(fcn_sigT,'set gra',icond,0)
c      pause
      call mncomd(fcn_sigT,'migrad',icond,0)
c      pause
      call mncomd(fcn_sigT,'minos',icond,0)
c      pause
      call mncomd(fcn_sigT,'return',icond,0)
c      pause
      
*      call mnhelp
      
      end

*--------------------------------------------------------------------------

      subroutine fcn_sigT(npar,grad,fval,xval,iflag,futil)
      implicit double precision (a-h,o-z)
      
c Fit sigT in t
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      real q2_gev
      
      dimension grad(12),xval(12)
      
      parameter (targ=938.27231) ! for piplus channel

      real WW,QQ2
      dimension tm(6),x(6),dx(6),W(6),Q2(6),th_pq(6),xsigl(6)
      save tm,x,dx,W,Q2,th_pq,xsigl

      character*40 fn

      real weight(6)
      save weight

c      f(t)=abs(t)/(abs(t)+0.139570**2)**2
      g(Wi)=1./(Wi**2-(targ/1.D+3)**2)**2

c      print*,'=== in FCN: npar=',npar,'   iflag=',iflag
c      pause

      if(iflag.eq.1) then
         
         fn='xsects/x_sep.'//pol_set//'_'//q2_set
         print*,fn
         open(31,file=fn)
         
         fn='averages/avek.'//q2_set//'.dat'
         print*,fn
         open(32,file=fn)
         
         itot=0
         do i=1,thi
            read(31,*)sigT,dsigT,sigL,dsigL,
     1           sigTT,dsigTT,sigLT,dsigLT,x2,t
            read(32,*) WW,dW,QQ2,dQ2,th_neg,th_pos
            if (i.ge.tlow) then
               itot=itot+1
               x(itot)=sigT
               dx(itot)=dsigT
               tm(itot)=t
               W(itot)=WW
               Q2(itot)=QQ2
c               weight(itot)=weigh(i,pol_set,q2_set)

               if(pol_set.eq.'mn') then
                  th_pq(itot)=th_neg
               elseif(pol_set.eq.'pl') then
                  th_pq(itot)=th_pos
               else
                  stop '*** Wrong pol_set ***'
               end if
               th_pq(itot)=th_pq(itot)*3.14159D0/180.D0

               write(*,'(6f7.3,i4)')x(itot),dx(itot),
     1              W(itot),Q2(itot),th_pq(itot),tm(itot),itot

            endif
         end do
         
         close(31)
         close(32)
         
c         pause
      end if                  !iflag=1
      
      fval=0.d0
      if(iflag.eq.2) then
         do i=1,12
            grad(i)=0.d0
         end do
      end if
      
      p5=xval(5)
      p6=xval(6)
      p7=xval(7)
      p8=xval(8)
      
      do i=1,ntot
         
         if(iflag.eq.3.and.(i-1+ntot)/ntot*ntot.eq.i-1+ntot) then
            j=(i-1+ntot)/ntot
            fn='xsects/sigt_fit.'//pol_set//'_'//q2_set
            open(77,file=fn,status='replace')
         end if
         
         xs=x(i)
         er=dx(i)         
         qsq=Q2(i)
c         ft=f(tm(i))
         
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c         tav=(0.0735+0.028*log(qsq))*qsq

* Revised for IT22, 12.11.06
	 read(q2_set,'(f3.0)')q2_gev
         q2_gev=q2_gev/100.
         tav=(0.0735+0.028*log(q2_gev))*q2_gev
         f_tav=(tm(i)-tav)/tav
         sigT=(p5+p6*log(qsq)+(p7+p8*log(qsq))*f_tav)*g(W(i))

         prv_sigT=(prv_par(5)+prv_par(6)*log(qsq)
     *        +(prv_par(7)+prv_par(8)*log(qsq))*f_tav)*g(W(i))
         
c Save fit cross-section, bin by bin.
         if(iflag.eq.3) write(77,'(E12.5,f8.5,E13.5)') sigT,tm(i),prv_sigT
         if(i/ntot*ntot.eq.i) close(77)
         
c difference between sigT from xfit_in_phi and previous iteration model
         dif=sigT-xs
	 if (iflag.eq.1 .or. iflag.eq.3) print*,' T',i,sigT,xs,dif
         
c         term=dif/er*weight(i)
c         fval=fval+term*term
         fval=fval
         
         if(iflag.eq.2) then

* Vector of first derivatives corresponding to
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
            grad(5)=grad(5)+2.*term/er*g(W(i))
            grad(6)=grad(6)+2.*term/er*log(qsq)*g(W(i))
            grad(7)=grad(7)+2.*term/er*f_tav*g(W(i))
            grad(8)=grad(8)+2.*term/er*log(qsq)*f_tav*g(W(i))

         endif
         
c       print*,'fval=',fval,'  x_tw=',x_tw,'  sigT=',sigT,
c      1      '  grad:',grad(2),grad(3),grad(4)
         
      end do
      
      if(iflag.eq.3) then
         
c Save separated fit parameters.
         
         chi2=0.
         ndf=ntot-npar
         if(ndf.ne.0) chi2=fval/ndf

         print*,' chi2=',chi2,' ndf =',ndf
         print*,' ntot',ntot,' npar =',npar
         
         call mnerrs(5,epl,emn,dp5,glcc)
         call mnerrs(6,epl,emn,dp6,glcc)
         call mnerrs(7,epl,emn,dp7,glcc)
         call mnerrs(8,epl,emn,dp8,glcc)

         fn='fit_params/par.'//pol_set//'_'//q2_set
         open(77,file=fn,access='append')

         write(77,10) p5,dp5,5,chi2
         write(77,10) p6,dp6,6,chi2
         write(77,10) p7,dp7,7,chi2
         write(77,10) p8,dp8,8,chi2

         close(77)
         
      end if                  !iflag=3
      
 10   format(2(E12.5,1x),1x,i2,f15.1)
      end
      
*==========================================================================

      subroutine xfit_sigLT(pol,q2set,t1,t2)
      
c Fit sigLT in t
      
      implicit double precision (a-h,o-z)
      
      character*2 pol
      character*3 q2set
      integer t1,t2
      
      external fcn_sigLT
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot

      pol_set=pol
      q2_set=q2set
      tlow=t1
      thi=t2
      ntot=thi-tlow+1
      
      call mncomd(fcn_sigLT,'set pri 1',icond,0)
      
c Fix sigT fit parameters, release sigLT fit parameters.

      call mncomd(fcn_sigLT,'fix 1',icond,0)
      call mncomd(fcn_sigLT,'fix 2',icond,0)
      call mncomd(fcn_sigLT,'fix 3',icond,0)
      call mncomd(fcn_sigLT,'fix 4',icond,0)
      call mncomd(fcn_sigLT,'fix 5',icond,0)
      call mncomd(fcn_sigLT,'fix 6',icond,0)
      call mncomd(fcn_sigLT,'fix 7',icond,0)
      call mncomd(fcn_sigLT,'fix 8',icond,0)
      call mncomd(fcn_sigLT,'fix 9',icond,0)
      call mncomd(fcn_sigLT,'fix 10',icond,0)
      call mncomd(fcn_sigLT,'fix 11',icond,0)
      call mncomd(fcn_sigLT,'fix 12',icond,0)

*      pause
      
      call mncomd(fcn_sigLT,'cali 1',icond,0)
c      pause
      call mncomd(fcn_sigLT,'set gra',icond,0)
c      pause
      call mncomd(fcn_sigLT,'migrad',icond,0)
c      pause
      call mncomd(fcn_sigLT,'minos',icond,0)
c      pause
      call mncomd(fcn_sigLT,'return',icond,0)
c      pause
      
*      call mnhelp
      
      end

*--------------------------------------------------------------------------

      subroutine fcn_sigLT(npar,grad,fval,xval,iflag,futil)
      implicit double precision (a-h,o-z)
      
c Fit sigLT
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
      dimension grad(12),xval(12)
      
      parameter (targ=938.27231) ! for piplus channel
      
      real WW,QQ2
      dimension tm(6),x(6),dx(6),W(6),Q2(6),th_pq(6)
      save tm,x,dx,W,Q2,th_pq
      
      character*40 fn
      
      real weight(6)
      save weight
      
      g(Wi)=1./(Wi**2-(targ/1.D+3)**2)**2
      
c      print*,'=== in FCN: npar=',npar,'   iflag=',iflag
c      pause
      
      if(iflag.eq.1) then
         
         itot=0
         
         fn='xsects/x_sep.'//pol_set//'_'//q2_set
         print*,fn
         open(31,file=fn)
         
         fn='averages/avek.'//q2_set//'.dat'
         print*,fn
         open(32,file=fn)
         
         itot=0
         do i=1,thi
            read(31,*)sigT,dsigT,sigL,dsigL,
     1           sigTT,dsigTT,sigLT,dsigLT,x2,t
            read(32,*) WW,dW,QQ2,dQ2,th_neg,th_pos
            if (i.ge.tlow) then
               itot=itot+1
               x(itot)=sigLT
               dx(itot)=dsigLT
               tm(itot)=t
               W(itot)=WW
               Q2(itot)=QQ2
c               weight(itot)=weigh(i,pol_set,q2_set)

               if(pol_set.eq.'mn') then
                  th_pq(itot)=th_neg
               elseif(pol_set.eq.'pl') then
                  th_pq(itot)=th_pos
               else
                  stop '*** Wrong pol_set ***'
               end if
               th_pq(itot)=th_pq(itot)*3.14159D0/180.D0
               
               write(*,'(6f7.3,i4)')x(itot),dx(itot),
     1              W(itot),Q2(itot),th_pq(itot),tm(itot),itot
               
            endif

         end do
         
         close(31)
         close(32)
         
c       pause
      end if                  !iflag=1
      
      fval=0.d0
      if(iflag.eq.2) then
         do i=1,12
            grad(i)=0.d0
         end do
      end if
      
      p9=xval(9)
      p10=xval(10)
      p11=xval(11)
      
      do i=1,ntot
         
         if(iflag.eq.3.and.(i-1+ntot)/ntot*ntot.eq.i-1+ntot) then
            j=(i-1+ntot)/ntot
            fn='xsects/siglt_fit.'//pol_set//'_'//q2_set
            open(77,file=fn,status='replace')
         end if
         
         xs=x(i)
         er=dx(i)
         qsq=Q2(i)

* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c         x_tw=exp(p10*abs(tm(i)))*sin(th_pq(i))*g(W(i))
c         sigLT=p9*x_tw

* Revised for IT26, 12.11.09
         x_tw=exp(p10*abs(tm(i)))*sin(th_pq(i))*g(W(i))
         sigLT_1=p9*x_tw
         sigLT=sigLT_1 + p11/abs(tm(i))*sin(th_pq(i))*g(W(i))

         prv_sigLT=(prv_par(9)*exp(prv_par(10)*abs(tm(i)))
     *        +prv_par(11)/abs(tm(i)))*sin(th_pq(i))*g(W(i))

c Save fit cross-section, bin by bin.
         if(iflag.eq.3) write(77,'(E12.5,f8.5,E13.5)') sigLT,tm(i),prv_sigLT
         if(i/ntot*ntot.eq.i) close(77)
         
c difference between sigLT from xfit_in_phi and previous iteration model
         dif=sigLT-xs
	 if (iflag.eq.1 .or. iflag.eq.3) print*,'LT',i,sigLT,xs,dif
         
c         term=dif/er*weight(i)
c         fval=fval+term*term
         fval=fval
         
         if(iflag.eq.2) then

* Vector of first derivatives corresponding to
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c            grad(9)=grad(9)+2.*term/er*x_tw
c            grad(10)=grad(10)+2.*term/er*abs(tm(i))*sigLT 

c Revised for IT26, 12.11.09
            grad(9)=grad(9)+2.*term/er*x_tw
            grad(10)=grad(10)+2.*term/er*abs(tm(i))*sigLT_1
            grad(11)=grad(11)+2*term/er/abs(tm(i))*sin(th_pq(i))*g(W(i))

         endif
         
c       print*,'fval=',fval,'  sigLT=',sigLT,
c      1      '  grad:',grad(7),grad(8),grad(9),grad(10),
c      1          grad(13),grad(14)
         
      end do
      
      if(iflag.eq.3) then
         
c Save separated fit parameters.
         
         chi2=0.
         ndf=ntot-npar
         if(ndf.ne.0) chi2=fval/ndf

         print*,' chi2=',chi2,' ndf =',ndf
         print*,' ntot',ntot,' npar =',npar
                  
         call mnerrs(9,epl,emn,dp9,glcc)
         call mnerrs(10,epl,emn,dp10,glcc)
         call mnerrs(11,epl,emn,dp11,glcc)

         fn='fit_params/par.'//pol_set//'_'//q2_set
         open(77,file=fn,access='append')

         write(77,10) p9,dp9,9,chi2
         write(77,10) p10,dp10,10,chi2
         write(77,10) p11,dp11,11,chi2

         close(77)
         
      end if                  !iflag=3
      
 10   format(2(E12.5,1x),1x,i2,f15.1)
      end
      
*========================================================================= 

      subroutine xfit_sigTT(pol,q2set,t1,t2)
      
c Fit sigTT in t
      
      implicit double precision (a-h,o-z)
      
      character*2 pol
      character*3 q2set
      integer t1,t2
      
      external fcn_sigTT
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
      pol_set=pol
      q2_set=q2set
      tlow=t1
      thi=t2
      ntot=thi-tlow+1

      call mncomd(fcn_sigTT,'set pri 1',icond,0)
      
c Fix sigT fit parameters, release sigTT fit parameters.

      call mncomd(fcn_sigTT,'fix 1',icond,0)
      call mncomd(fcn_sigTT,'fix 2',icond,0)
      call mncomd(fcn_sigTT,'fix 3',icond,0)
      call mncomd(fcn_sigTT,'fix 4',icond,0)
      call mncomd(fcn_sigTT,'fix 5',icond,0)
      call mncomd(fcn_sigTT,'fix 6',icond,0)
      call mncomd(fcn_sigTT,'fix 7',icond,0)
      call mncomd(fcn_sigTT,'fix 8',icond,0)      
      call mncomd(fcn_sigTT,'fix 9',icond,0)
      call mncomd(fcn_sigTT,'fix 10',icond,0)
      call mncomd(fcn_sigTT,'fix 11',icond,0)
      call mncomd(fcn_sigTT,'rel 12',icond,0)

c      pause
      
      call mncomd(fcn_sigTT,'cali 1',icond,0)
c      pause
      call mncomd(fcn_sigTT,'set gra',icond,0)
c      pause
      call mncomd(fcn_sigTT,'migrad',icond,0)
c      pause
      call mncomd(fcn_sigTT,'minos',icond,0)
c      pause
      call mncomd(fcn_sigTT,'return',icond,0)
c      pause
      
*      call mnhelp
      
      end

*--------------------------------------------------------------------------

      subroutine fcn_sigTT(npar,grad,fval,xval,iflag,futil)
      implicit double precision (a-h,o-z)
      
c Fit sigTT
      
      common/pol_set/pol_set,q2_set,tlow,thi,ntot,prv_par(12)
      character*2 pol_set
      character*3 q2_set
      integer tlow,thi,ntot
      
      dimension grad(12),xval(12)

      parameter (targ=938.27231) ! for piplus channel
      
      real WW,QQ2
      dimension tm(6),x(6),dx(6),W(6),Q2(6),th_pq(6)
      save tm,x,dx,W,Q2,th_pq
      
      character*40 fn
      
      real weight(6)
      save weight
      
      f(t)=abs(t)/(abs(t)+0.139570**2)**2
      g(Wi)=1./(Wi**2-(targ/1.D+3)**2)**2
      
c      print*,'=== in FCN: npar=',npar,'   iflag=',iflag
c      pause
      
      if(iflag.eq.1) then
                  
         fn='xsects/x_sep.'//pol_set//'_'//q2_set
         print*,fn
         open(31,file=fn)
         
         fn='averages/avek.'//q2_set//'.dat'
         print*,fn
         open(32,file=fn)
         
         itot=0
         do i=1,thi
            read(31,*) sigT,dsigT,sigL,dsigL,
     1           sigTT,dsigTT,sigLT,dsigLT,x2,t
            read(32,*) WW,dW,QQ2,dQ2,th_neg,th_pos
            if (i.ge.tlow) then
               itot=itot+1
               x(itot)=sigTT
               dx(itot)=dsigTT
               tm(itot)=t
               W(itot)=WW
               Q2(itot)=QQ2
c               weight(itot)=weigh(i,pol_set,q2_set)

               if(pol_set.eq.'mn') then
                  th_pq(itot)=th_neg
               elseif(pol_set.eq.'pl') then
                  th_pq(itot)=th_pos
               else
                  stop '*** Wrong pol_set ***'
               end if
               th_pq(itot)=th_pq(itot)*3.14159D0/180.D0
               
               write(*,'(6f7.3,i4)')x(itot),dx(itot),
     1              W(itot),Q2(itot),th_pq(itot),tm(itot),itot
               
            endif
            
         end do
         
         close(31)
         close(32)
         
c       pause
      end if                  !iflag=1
      
      fval=0.d0
      if(iflag.eq.2) then
         do i=1,12
            grad(i)=0.d0
         end do
      end if
      
c      p11=xval(11)
      p12=xval(12)
      
      do i=1,ntot
         
         if(iflag.eq.3.and.(i-1+ntot)/ntot*ntot.eq.i-1+ntot) then
            j=(i-1+ntot)/ntot
            fn='xsects/sigtt_fit.'//pol_set//'_'//q2_set
            open(77,file=fn,status='replace')
         end if
         
         xs=x(i)
         er=dx(i)
         qsq=Q2(i)
         ft=f(tm(i))

* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c         x_tw=ft*(sin(th_pq(i))**2)*g(W(i))
c         sigTT=(p11*qsq*exp(-qsq)+p12/qsq**2)*x_tw

c         prv_sigTT=(prv_par(11)*qsq*exp(-qsq)+prv_par(12)/qsq**2)
c     *        *ft*(sin(th_pq(i))**2)*g(W(i))

* Revised for IT26, 12.11.09
         x_tw=ft*(sin(th_pq(i))**2)*g(W(i))
         sigTT=(p12*qsq*exp(-qsq))*x_tw

         prv_sigTT=(prv_par(12)*qsq*exp(-qsq))*ft*(sin(th_pq(i))**2)
     *     *g(W(i))

c Save fit cross-section, bin by bin.
         if(iflag.eq.3) write(77,'(E12.5,f8.5,E13.5)') sigTT,tm(i),prv_sigTT
         if(i/ntot*ntot.eq.i) close(77)
         
c difference between sigTT from xfit_in_phi and previous iteration model
         dif=sigTT-xs
	 if (iflag.eq.1 .or. iflag.eq.3) print*,'TT',i,sigL,xs,dif
         
c         term=dif/er*weight(i)
c         fval=fval+term*term
         fval=fval
         
         if(iflag.eq.2) then

* Vector of first derivatives corresponding to
* Parameterization based upon Fpi-1 pi+ IT25, 12.04.18
c            grad(11)=grad(11)+2.*term/er*qsq*exp(-qsq)*x_tw
c            grad(12)=grad(12)+2.*term/er/qsq**2*x_tw

* Revised for IT26, 12.11.09
            grad(12)=grad(12)+2.*term/er*qsq*exp(-qsq)*x_tw

         endif
         
c       print*,'fval=',fval,'  x_tw=',x_tw,'  sigTT=',sigTT,
c      1      '  grad:',grad(5),grad(6)
         
      end do
      
      if(iflag.eq.3) then
         
c Save separated fit parameters.
         
         chi2=0.
         ndf=ntot-npar
         if(ndf.ne.0) chi2=fval/ndf

         print*,' chi2=',chi2,' ndf =',ndf
         print*,' ntot',ntot,' npar =',npar         
         
c         call mnerrs(11,epl,emn,dp11,glcc)
         call mnerrs(12,epl,emn,dp12,glcc)
         
*         do is=1,1
         fn='fit_params/par.'//pol_set//'_'//q2_set
         open(77,file=fn,access='append')
c         write(77,10) p11,dp11,11,chi2
         write(77,10) p12,dp12,12,chi2
         close(77)
*        end do
         
      end if                  !iflag=3
      
 10   format(2(E12.5,1x),1x,i2,f15.1)
      end

*==========================================================================
      
      function weigh(it,pol,q2)
      
      character*2 pol
      character*3 q2
      
      real weigh=1.
      
*      if(pol.ne.'pl') return
***    if(q2.ne.'060'.and.q2.ne.'100') return

c      if(it.le.3) return
c      weigh=0.

      end
