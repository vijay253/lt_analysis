      program average_ratios

c     Taken exp. and MC yields at each theta_pq setting, this program
c     calculates exp/simc ratios for polarity-Q2-eps setting. The idea
c     comes from Jochen.
c
c     Input: yields/yields.*.dat
c     Output: averages/aver.*.dat

      call average_r(+1,0.375,0.286)
      call average_r(+1,0.375,0.629)
      call average_r(+1,0.375,0.781)

      stop
      end

*-----------------------------------------------------------------------

      subroutine average_r(pol_set,q2_set,eps_set)

      implicit none

      integer pol_set
      real q2_set,eps_set

c     Aquire yields over theta_pq settings, calculate ratio, save result
c     in averages/aver.* .

      integer nt,nphi,it,ip
      integer nset,ipol
      integer nbin
      real q2,eps,th_pq,tmn,tmx,r,e,er
      real one
      real yd

      parameter (nt=8,nphi=16)
      real yrd(nphi,nt),drd(nphi,nt)
      real ydm(nphi,nt),ddm(nphi,nt)
      real ymc(nphi,nt),dmc(nphi,nt)

      character*80 fn
      character*2 pol

      do it=1,nt
         do ip=1,nphi
            yrd(ip,it)=0.
            drd(ip,it)=0.
            ydm(ip,it)=0.
            ddm(ip,it)=0.
            ymc(ip,it)=0.
            dmc(ip,it)=0.
         end do
      end do

      nset=0
      open(55,file='./list.settings.pion19')
c      open(55,file='./test.setting.pion19')
      do while(.true.)

         read(55,*,end=9) ipol,q2,eps,th_pq,tmn,tmx,nbin
         if(ipol.eq.pol_set.and.q2.eq.q2_set.and.eps.eq.eps_set) then

            if(ipol.eq.-1) then
               pol='mn'
            elseif(ipol.eq.+1) then
               pol='pl'
            else
               stop '*** aver: wrong pol ***'
            endif

            write(*,*)'We are averaging the following kinematics:'
            write(*,*)ipol,q2,eps,th_pq,pol

c     Read real data.
            write(fn,'(''yields/yields.'',
     *      a2,''_'',i3.3,''_'',i3.3,''_'',SP,i5.4,S,''.target'')')
     *      pol,nint(q2_set*1000.),nint(eps_set*1000.),nint(th_pq*1000.)
            print*,'fn=',fn
c            pause
               
            open(66,file=fn)
            read(66,*) one
            do it=1,nt
               do ip=1,nphi
                  read(66,*) yd,er
                  print*,yd,er
                  yrd(ip,it)=yrd(ip,it)+yd
                  drd(ip,it)=drd(ip,it)+er**2
               end do
            end do
            close(66)

c     Read dummy data.
            write(fn,'(''yields/yields.'',
     *      a2,''_'',i3.3,''_'',i3.3,''_'',SP,i5.4,S,''.dummy'')')
     *      pol,nint(q2_set*1000.),nint(eps_set*1000.),nint(th_pq*1000.)
            print*,'fn=',fn
c            pause

            open(66,file=fn)
            read(66,*) one
            do it=1,nt
               do ip=1,nphi
                  read(66,*) yd,er
                  print*,yd,er
                  ydm(ip,it)=ydm(ip,it)+yd
                  ddm(ip,it)=ddm(ip,it)+er**2
               end do
            end do
            close(66)

c     Read simc data.
            write(fn,'(''yields/yields.'',a2,''_'',i3.3,''_'',i3.3,
     *      ''_'',SP,i5.4,S,''.simc'')')
     *      pol,nint(q2_set*1000.),nint(eps_set*1000.),nint(th_pq*1000.)
            print*,'fn=',fn
c            pause

            open(66,file=fn)
            read(66,*) one
            do it=1,nt
               do ip=1,nphi
                  read(66,*) yd,er
                  print*,yd,er
                  ymc(ip,it)=ymc(ip,it)+yd
                  dmc(ip,it)=dmc(ip,it)+er**2
               end do
            end do
            close(66)

c            do it=1,nt
c               do ip=1,nphi
c                  write(*,*) it,ip
c                  write(*,*)'exp_yield=',yrd(ip,it),drd(ip,it)
c                  write(*,*)'dummy_yield=',ydm(ip,it),ddm(ip,it)
c                  write(*,*)'MC_yield=',ymc(ip,it),dmc(ip,it)
c               enddo
c            enddo

            nset=nset+1

         end if                 !ipol=pol_set & q2=q2_set & eps=eps_set

      end do                    !while not eof.

 9    continue
      close(55)

      print*,'nset=',nset

c      pause

      write(fn,'(''averages/aver.'',a2,''_'',i3.3,''_'',i3.3,''.dat'')'
     *     )pol,nint(q2_set*1000.),nint(eps_set*1000.)
      print*,'fn=',fn
      print*

      open(77,file=fn,status='replace')
      do it=1,nt
         do ip=1,nphi
            r=0.
            e=0.
c ratio is data/simc - see GH logbook, p.55
            if(ymc(ip,it).ne.0.) then
              r=(yrd(ip,it)-ydm(ip,it))/ymc(ip,it)
              e=e+(drd(ip,it)+ddm(ip,it))/ymc(ip,it)**2
              e=e+((r/ymc(ip,it))**2)*dmc(ip,it)
              e=sqrt(e)
              write(*,*)'     the data/simc ratio is:'
              write(*,'(2f15.5,2i3)')r,e,ip,it
              write(*,*)'--------------------------'
            end if
c ratio is simc/data - see GH logbook, p.55
c            if ((yrd(ip,it)-ydm(ip,it)).ne.0) then
c              r=ymc(ip,it)/(yrd(ip,it)-ydm(ip,it))
c              e=e+(drd(ip,it)+ddm(ip,it))*(r/(yrd(ip,it)-ydm(ip,it)))**2
c              e=e+dmc(ip,it)/(yrd(ip,it)-ydm(ip,it))**2
c              e=sqrt(e)
c              write(*,*)'     the simc/data ratio is:'
c              write(*,'(2f15.5,2i3)')r,e,ip,it
c              write(*,*)'--------------------------'
c            end if
            write(77,'(2f15.5,2i3)') r,e,ip,it
         end do
      end do
      close(77)

      end
