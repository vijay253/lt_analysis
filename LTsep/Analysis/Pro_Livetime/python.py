import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np
import math as m
from scipy.optimize import curve_fit
from scipy.stats import linregress

def Line(x, intercept, slope):

    y = intercept + slope * x
    return y 

with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1Q2', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_EDTM_live_Q1Q2', 'r') as file:
    file_names = file.read().splitlines()

Run_list     = []
list_x       = []  
list_y1      = []  
list_y_err1  = []

list_y2      = []  
list_y_err2  = []

tbc_list      = []
tbc_list_err  = []

shms3y4_list   = []
shmshod1_list  = []
shmshod2_list  = []
shmshod3_list  = []
shmshod4_list  = []

for file_name in file_names:
    Run,TLT,errTLT,CPULIVE,CPULIVEERR,Coin_trig_rate,I,SHMS3Y4,SHMSHOD1,SHMSHOD2,SHMSHOD3,SHMSHOD4 = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/'+file_name+'.dat', unpack=True)  
    list_x.append(Coin_trig_rate)

    if TLT <= 0:    # check if EDTM was off    
        list_y1.append(CPULIVE)    
        list_y_err1.append(CPULIVEERR)
    else: 
        list_y1.append(TLT)        
        list_y_err1.append(errTLT)

    Run_list.append(Run)        
    list_y2.append(CPULIVE)
    list_y_err2.append(CPULIVEERR)

    tbc_list.append(1-0.0003382*I)
    tbc_list_err.append(m.sqrt(I**2*(0.0000773)**2+(0.0003382)**2*(0.01*I)**2))   # 1% uncer in I

    shms3y4_list.append(SHMS3Y4)    
    shmshod1_list.append(SHMSHOD1)
    shmshod2_list.append(SHMSHOD2)
    shmshod3_list.append(SHMSHOD3)
    shmshod4_list.append(SHMSHOD4)

fig, ax = plt.subplots(2,2)
#fig, ax = plt.subplots(2)
#for i in range(len(file_names)):

# Coin rate and Run number vs EDTM live time  plot
'''
ax[0].errorbar(list_x, list_y1, yerr=list_y_err1, fmt='s', markersize=2, color='red',label='EDTM total live time')
ax[0].set_xlabel('Coin Trigger Rate [kHz]')
ax[0].set_ylabel('EDTM Live Time')

ax[1].errorbar(Run_list, list_y1, yerr=list_y_err1, fmt='s', markersize=2, color='green',label='EDTM total live time')
ax[1].set_xlabel('Run Number')
ax[1].set_ylabel('EDTM Live Time')
'''
# Coin trigg rate vs CPU live time  plot

'''
ax.errorbar(Run_list, list_y2, yerr=list_y_err2, fmt='s', markersize=2, color='green',label='CPU live time')
ax.set_xlabel('Coin Trigger Rate [kHz]')
ax.set_ylabel('CPU Live Time')
'''
# TBoiling plot
'''
ax.errorbar(Run_list, tbc_list, yerr=tbc_list_err, fmt='s', markersize=2, color='green',label='Target Boiling Factor')
ax.set_xlabel('Run Number')
ax.set_ylabel('Target Boiling Factor')
'''
# Run Number vs SHMS3Y4 efficiency plot
'''
ax[0].errorbar(list_x, shms3y4_list, fmt='s', markersize=2, color='red',label='SHMS3Y4 Efficiency')
ax[0].set_xlabel('Coin Trigger Rate [kHz]')
ax[0].set_ylabel('SHMS 3/4 Efficiency')

ax[1].errorbar(Run_list, shms3y4_list, fmt='s', markersize=2, color='green',label='SHMS3Y4 Efficiency')
ax[1].set_xlabel('Run Number')
ax[1].set_ylabel('SHMS 3/4 Efficiency')
'''
# Run Number vs SHMS Individiual hodo efficiency plot

ax[0,0].errorbar(Run_list, shmshod1_list, fmt='s', markersize=2, color='red',label='SHMSHOD1 Efficiency')
ax[0,0].set_xlabel('Run Number')
ax[0,0].set_ylabel('SHMS HODO 1 Eff')

ax[0,1].errorbar(Run_list, shmshod2_list, fmt='s', markersize=2, color='red',label='SHMSHOD2 Efficiency')
ax[0,1].set_xlabel('Run Number')
ax[0,1].set_ylabel('SHMS HODO 2 Eff')

ax[1,0].errorbar(Run_list, shmshod3_list, fmt='s', markersize=2, color='red',label='SHMSHOD3 Efficiency')
ax[1,0].set_xlabel('Run Number')
ax[1,0].set_ylabel('SHMS HODO 3 Eff')

ax[1,1].errorbar(Run_list, shmshod4_list, fmt='s', markersize=2, color='red',label='SHMSHOD4 Efficiency')
ax[1,1].set_xlabel('Run Number')
ax[1,1].set_ylabel('SHMS HODO 4 Eff')

#plot Run vs Coin trigger rate
'''
fig, ax = plt.subplots()
ax.scatter(Run_list,list_x, s=15, marker='^', color='blue' )
ax.set_xlabel('Run Number')
ax.set_ylabel('Coin Trigger Rate [kHz]')
'''
fig.tight_layout()


#ax.set_xticks()
#ax.set_ylim(0.90, 1.04)   # For target boiling plot
#ax.set_ylim(0.80, 1.10)
#ax.set_ylim(0.85, 1.10)   
#ax.set_xlim(0.0, 60.0)
#ax.xaxis.set_major_locator(plt.MaxNLocator(3))

#ax.set_xlabel('Coin Trigger Rate [kHz]')
#ax.set_ylabel('EDTM Live Time')

#plt.savefig('OUTPUT/predtmlive.png')
#plt.savefig('OUTPUT/tbcf.png')
#plt.savefig('OUTPUT/SHMS3Y4.png')
plt.savefig('OUTPUT/SHMSHODO.png')
#plt.savefig('OUTPUT/prcpulive.png')
#plt.savefig('OUTPUT/Run_coin_rate.png')
