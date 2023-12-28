import matplotlib.pyplot as plt
from array import array
import numpy as np
import math as m
from scipy.optimize import curve_fit
from scipy.stats import linregress
from sklearn.linear_model import LinearRegression

def Line(x, intercept, slope):

    y = intercept + slope * x
#    y = intercept + slope1 * x
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
    Run, TLT, errTLT, Yield, Rerr, Yieldtr, Rerrtr, HMStreff, I, HMStrefferr = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/OUTPUT/'+file_name+'.dat', unpack=True)  
    '''
    list_x.append(Run)
    list_y.append(TLT)
    list_y_err.append(errTLT)
    '''
    list_x.append(I)

#    list_y.append((Yield)/(TLT*HMStreff*7081710.28397946))
#    list_y_err.append(Rerrtr)

#C tracl study
#    list_y.append(Yieldtr/(TLT**HMStreff*4669632.764768297))
    list_y.append(Yieldtr/(TLT*4669632.764768297))
#    list_y_err.append(m.sqrt((Rerrtr)**2+(errTLT)**2))
    list_y_err.append(m.sqrt((Rerrtr)**2+(0.02)**2+(HMStrefferr)**2+(0.01)**2))   #2% (EDTM), 1% (bcm 1 charge) fraction errors

#LH2 track study
#    list_y.append((Yieldtr/(TLT*HMStreff*0.9979)-(2.96471e+06/(0.9979*0.935592*20.367*4.8579))*I)/8563239.106804244)
#    list_y.append((Yieldtr/(TLT*HMStreff*0.9979)-(2.96471e+06/(0.9979*0.935592*20.367*4.8579))*I)/8211344.64983696)
#    list_y.append((Yieldtr/(TLT*HMStreff*0.9979)-(2.96471e+06/(0.9979*0.935592*20.367*5.21))*I)/8211344.64983696)
#    list_y_err.append(m.sqrt((Rerrtr)**2 + (TLT*0.02)**2))
#    list_y_err.append(Rerrtr*errTLT)
    
fig, ax = plt.subplots()
#for i in range(len(file_names)):
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='EDTM Live Time')
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS Carbon-12 Non-track F.N. Yield')
ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS Carbon-12 track F.N. Yield')

#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS LH2 Non-track F.N. Yield')
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='HMS LH2 track F.N. Yield')
#ax.errorbar(list_x, list_y, fmt='s', markersize=2, color='red')

slope1, intercept1, r, p, se = linregress(list_x, list_y)

params, covariance = curve_fit(Line, list_x, list_y, sigma=list_y_err)
intercept_fit, slope_fit = params
intercept_error, slope_error = np.sqrt(np.diag(covariance))

print(intercept_fit)
print(intercept_error)
print(slope_fit)
print(slope_error)

#parameter, covariance = curve_fit(Line, list_x, list_y)
#parameters, covariance = curve_fit(Line, list_x, list_y, p0 =params, sigma=list_y_err, absolute_sigma=True)

ax.text(16, 1.05, 'm\u2080 ='+str("%.7f" %slope_fit)+', \u03B5m='+str("%.7f" %slope_error), fontsize=8, bbox=dict(facecolor='red', alpha=0.5))   # For C
#ax.text(12, 1.05, 'm\u2080 ='+str("%.7f" %slope_fit)+', \u03B5m\u2080='+str("%.7f" %slope_error), fontsize=8, bbox=dict(facecolor='red', alpha=0.5))    # For H2
fit_y = Line(np.array(list_x), intercept_fit, slope_fit)
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
ax.set_xlabel('Avg BCM1 Current [uA]')
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
