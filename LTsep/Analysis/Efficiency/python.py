import matplotlib.pyplot as plt
import numpy as np
import math as m
from scipy.optimize import curve_fit
from scipy.stats import linregress

def Line(x, intercept, slope):

    y = intercept + slope * x
    return y 

with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/EffRunList', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1', 'r') as file:
    file_names = file.read().splitlines()

list_x      = []  
list_y      = []  
list_y_err  = []

for file_name in file_names:
    Run, SHMStreff, HMStreff = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/OUTPUT/'+file_name+'.dat', unpack=True)  
    list_x.append(Run)
    list_y.append(SHMStreff)
#    list_y_err.append(errTLT)
    
fig, ax = plt.subplots()
#for i in range(len(file_names)):
#ax.errorbar(list_x, list_y, yerr=list_y_err, fmt='s', markersize=8, color='red',label='EDTM Live Time')
ax.errorbar(list_x, list_y, fmt='s', markersize=2, color='red')

#ax.set_xticks()
ax.set_ylim(0.925, 1.075)
#ax.set_xlim(0.0, 60.0)
#ax.xaxis.set_major_locator(plt.MaxNLocator(3))

#ax.set_xticks(list_x)
#ax.set_xticklabels(list_x, rotation=45)

ax.set_xlabel('Run Number')
ax.set_ylabel('Tracking Efficiency')
plt.savefig('OUTPUT/TrkEff.png')
