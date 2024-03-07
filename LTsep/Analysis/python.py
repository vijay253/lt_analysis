import matplotlib.pyplot as plt
import numpy as np
import sys as sys
 
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/RunListHMSCalEff', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_highe', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375highe', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375mide', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375lowe', 'r') as file:
with open("/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19", 'r') as file:
    #with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Heep_Coin_2p7', 'r') as file:
    #with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Heep_Coin_3p6', 'r') as file:
    #with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Heep_Coin_4p5', 'r') as file:
    file_names = file.read().splitlines()

Run_list      = []  
Eff_list      = []  
Eff_err_list  = []

for file_name in file_names:
    Run, Eff, err = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/'+file_name+'.dat', unpack=True)  
    Run_list.append(Run)
    Eff_list.append(Eff)
    Eff_err_list.append(err)

col = []            
for i in range(len(file_names)):
#    if Run_list[i] <= 8547.0:      #Production data at Q1 
    if Run_list[i] <= 8549.0:       #Heep Coin data at Q2
        col.append('blue')  
#    elif Run_list[i] <= 8653.0:
    elif Run_list[i] <= 8571.0:
        col.append('green') 
        
    else:
        col.append('red') 

fig, ax = plt.subplots()
for i in range(len(file_names)):

    ax.errorbar(Run_list[i], Eff_list[i], yerr=Eff_err_list[i], fmt='s', markersize=2, c = col[i])

#ax.set_xticks()
ax.set_ylim(0.995, 1.0)
#ax.set_xlim(file_names[0], file_names[5])
#ax.xaxis.set_major_locator(plt.MaxNLocator(3))
#ax.set_xticks(Run_list)
#ax.set_xticklabels(Run_list, rotation=0)
ax.set_xlabel('Run Number')
ax.set_ylabel('HMS Cal Efficiency')
#ax.set_title('HMS Cal Eff vs Run')
#plt.legend()
#plt.show()
#plt.savefig('OUTPUT/Cal0p7Cut.png')
#plt.savefig('OUTPUT/Cal0p2Cut.png')

plt.savefig('OUTPUT/HMS_CAL_EFF_Q1.png')

#plt.savefig('OUTPUT/lowe.png')
#plt.savefig('OUTPUT/HeepCoin_lowe.png')
#plt.savefig('OUTPUT/mide.png')
#plt.savefig('OUTPUT/HeepCoin_mide.png')
#plt.savefig('OUTPUT/highe.png')
#plt.savefig('OUTPUT/HeepCoin_highe.png')
