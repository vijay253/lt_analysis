import matplotlib.pyplot as plt
import numpy as np

#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_highe', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375highe', 'r') as file:
#with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375mide', 'r') as file:
with open('/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375lowe', 'r') as file:
    file_names = file.read().splitlines()

Run_list      = []  
Eff_list      = []  
Eff_err_list  = []

for file_name in file_names:
    Run, Eff, err = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/'+file_name+'.dat', unpack=True)  
    Run_list.append(Run)
    Eff_list.append(Eff)
    Eff_err_list.append(err)

fig, ax = plt.subplots()
for i in range(len(file_names)):
    ax.errorbar(Run_list[i], Eff_list[i], yerr=Eff_err_list[i], fmt='s', markersize=2, color='red')

#ax.set_xticks()
#ax.set_ylim(0.97, 1.0)
#ax.set_xlim(file_names[0], file_names[5])
#ax.set_xticklabels(file_names, rotation=0)
ax.set_xlabel('Run')
ax.set_ylabel('Efficiency')
ax.set_title('HMS Cal Eff vs Run')
#plt.legend()
#plt.show()
plt.savefig('OUTPUT/CalEff.png')
