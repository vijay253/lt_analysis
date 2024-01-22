      program xfit_in_t
      
c      Fit, sigL,sigT,sigTT,sigLT in t.

c      character*2 prv_it
c      common/prv_it/prv_it
      
c       Get number of the previous iteration.
      
c      if(iargc().ne.1) then
c         print*,'*** usage: xfit_in_t prv_it ***'
c         stop
c      end if
c      call getarg(1,prv_it)
      
c      call mninit(5,6,7)
      
c      call mnseti('Fit t dependence of D-/D+ cross-sections')
      
      call xfit_sigL('pl','375',1,8)
      
c      call xfit_sigT('pl','245',1,6)
      
c      call xfit_sigLT('pl','245',1,6)

c      call xfit_sigTT('pl','245',1,6)
      
      stop
      end
