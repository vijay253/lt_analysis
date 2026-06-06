      REAL theta_hms(5),theta_sos(5),degrad
      parameter (degrad=0.0174533)

c gh 12.10.09
c th_hms values consistent w/SIMC
c no HMS in-plane (yptar) offset applied in SIMC or elsewhere
      data theta_hms /10.54,12.20,10.615,13.61,16.61/

c gh 12.10.09 
c th_sos values consistent w/SIMC
c no SOS in-plane angle (yptar) offset applied in SIMC or elsewhere
c      data theta_sos /2*51.48,3*29.43/

c see kinoffsets.inc for hms,sos z position offsets

c      real p_hms(5),p_sos(5)
c      real Ebeam(5)
      real Ebeam(10)


c WL 16.04.16
c values consistent w/SIMC
c there are no beam energy offsets applied elsewhere
	data Ebeam/2*3.77861, 3*4.70897, 2*4.21027, 3*5.24684/


c gh 12.10.09
c values consistent w/SIMC
c there are no beam energy offsets applied elsewhere
c	data Ebeam/2*4.21027,3*5.24684/

c gh 12.10.09
c p_hms values consistent w/SIMC
c there are no momentum offsets applied elsewhere
c      data p_hms/5*3.33170/

c gh 12.10.09
c p_sos values consistent w/SIMC
c there are no momentum offsets applied elsewhere
c      data p_sos/2*0.77100,3*1.71840/
