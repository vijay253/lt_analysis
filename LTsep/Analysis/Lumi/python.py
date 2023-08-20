import matplotlib.pyplot as plt
import numpy as np
import math as m
from scipy.optimize import curve_fit
from scipy.stats import linregress

def Line(x, intercept, slope):

    y = intercept + slope * x
    return y 

HMScaleff     = 0.9979
HMScalefferr  = 4.004e-05

#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/tmp', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/TLT_plot', 'r') as file:
with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/Pion_Carbon_Lumi_2p7_plot', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/CTLT_plot', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/Pion_Lumi_2p7_plot', 'r') as file:
    file_names = file.read().splitlines()

list_x      = []  
list_y      = []  
list_y_err  = []

for file_name in file_names:
    Run, TLT, errTLT, Yield, Rerr, Yieldtr, Rerrtr, HMStreff, I = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/OUTPUT/'+file_name+'.dat', unpack=True)  
    '''
    list_x.append(Run)
    list_y.append(TLT)
    list_y_err.append(errTLT)
    '''
    list_x.append(I)

#    list_y.append((Yield)/(TLT*HMStreff*7081710.28397946))
#    list_y_err.append(Rerrtr)

#C tracl study
    list_y.append(Yieldtr/(TLT*HMStreff*4066365.937032089))
    list_y_err.append(Rerrtr)

#LH2 track study
#    list_y.append((Yieldtr/(TLT*HMStreff)-(4.30864/(0.9979*0.944964*20.367))*I)/12818303.18107453)
#    list_y_err.append(m.sqrt(Rerrtr*errTLT))
#    list_y_err.append(Rerrtr)
    
fig, ax = plt.subplots()
#for i in range(len(file_names)):
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='EDTM Live Time')
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS Carbon-12 Non-track F.N. Yield')
ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='2', markersize=18, color='red',label='HMS Carbon-12 track F.N. Yield')

#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS LH2 Non-track F.N. Yield')
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='2', markersize=18, color='red',label='HMS LH2 track F.N. Yield')
#ax.errorbar(list_x, list_y, fmt='s', markersize=2, color='red')

test = linregress(list_x, list_y)

parameter, covariance = curve_fit(Line, list_x, list_y)
parameters, covariance = curve_fit(Line, list_x, list_y, p0 =parameter, sigma=list_y_err, absolute_sigma=True)
inter = parameters[0]
slope = parameters[1]
d_inter = np.sqrt(covariance[0][0])
d_slope = np.sqrt(covariance[1][1])

print(slope)
print(d_slope)
print(inter)
print(d_inter)

print(test[0])
print(test[1])

ax.text(16, 1.05, 'm\u2080 ='+str("%.7f" %slope)+', \u03B5m='+str("%.7f" %d_slope), fontsize=8, bbox=dict(facecolor='red', alpha=0.5))   # For C
#ax.text(12, 1.05, 'm\u2080 ='+str("%.7f" %slope)+', \u03B5m\u2080='+str("%.7f" %d_slope), fontsize=8, bbox=dict(facecolor='red', alpha=0.5))    # For H2
fit_y = Line(np.array(list_x), inter, slope)
ax.plot(list_x, fit_y, '-', label='y = c + m\u2080*x')   #"%.7f" %slope
ax.legend()

#print(np.array(list_x))

#ax.set_xticks()
#ax.set_ylim(0.90, 1.10)
#ax.set_ylim(0.85, 1.10)    #for CTLT
#ax.set_xlim(0.0, 60.0)
#ax.xaxis.set_major_locator(plt.MaxNLocator(3))

#ax.set_xticks(list_x)
#ax.set_xticklabels(list_x, rotation=45)

ax.set_ylim(0.90, 1.10)
ax.set_xlabel('BCM1 Current [uA]')
ax.set_ylabel('Fractional Normalized Yield')
'''
ax.set_xlabel('Run Number')
ax.set_ylabel('EDTM Live Time')
plt.savefig('OUTPUT/CTLT.png')
'''
#plt.savefig('OUTPUT/Cnontrack.png')
plt.savefig('OUTPUT/Ctrack.png')

#plt.savefig('OUTPUT/Hnontrack.png')
#plt.savefig('OUTPUT/Htrack.png')

#plt.savefig('OUTPUT/tmp.png')
