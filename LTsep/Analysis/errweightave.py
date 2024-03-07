#This script is made to get error weighted average, VK, July 17, 2023
import matplotlib.pyplot as plt
import numpy as np
import math as ma

with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19', 'r') as file:
    
    file_names = file.read().splitlines()

#Assign lists

xaxis_list     = [1]
errweigtave_list     = []
err_errweigtave_list = []

#Assign variables

ErroWeightAveNum  = 0.0
ErroWeightAveDen  = 0.0
Err_ErroWeightAve = 0.0

for file_name in file_names:
    Run, Eff, err = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/'+file_name+'.dat', unpack=True)  
    
    ErroWeightAveNum  += Eff/(err*err) 
    ErroWeightAveDen  += 1.0/(err*err) 

errweightave = ErroWeightAveNum/ErroWeightAveDen
err_errweigtave = 1.0/(ma.sqrt(ErroWeightAveDen))

#save value in a list
errweigtave_list.append(errweightave)
err_errweigtave_list.append(err_errweigtave)

print(errweightave)
print(err_errweigtave)

fig, ax = plt.subplots()
ax.errorbar(xaxis_list, errweigtave_list, yerr=err_errweigtave_list, fmt='s', markersize=5, c = 'red')

#ax.set_xticks()
ax.set_ylim(0.997, 0.999)
#ax.set_xlim(file_names[0], file_names[5])
#ax.xaxis.set_major_locator(plt.MaxNLocator(3))
#ax.set_xticks(Run_list)
#ax.set_xticklabels(Run_list, rotation=0)
#ax.set_xlabel('Run Number')
#ax.set_ylabel('HMS Cal Efficiency')
plt.savefig('OUTPUT/Final_HMS_Cal_Eff.png')

