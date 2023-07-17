#This script is made to get error weighted average, VK, July 17, 2023
import matplotlib.pyplot as plt
import numpy as np
import math as ma

with open('/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19', 'r') as file:
    
    file_names = file.read().splitlines()

#Assign lists

errweigtave_list     = []
err_errweigtave_list = []

#Assign variables

ErroWeightAveNum  = 0.0
ErroWeightAveDen  = 0.0
Err_ErroWeightAve = 0.0

for file_name in file_names:
    Run, Eff, err = np.loadtxt('/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/'+file_name+'.dat', unpack=True)  
    
    ErroWeightAveNum  += Eff/(pow(err,2)) 
    ErroWeightAveDen  += 1.0/pow(err,2) 
    Err_ErroWeightAve += 1.0/(ma.sqrt(1.0/pow(err, 2)))

errweightave = ErroWeightAveNum/ErroWeightAveDen
err_errweigtave = Err_ErroWeightAve
#print(test)
#save value in a list
errweigtave_list.append(errweightave)
err_errweigtave_list.append(err_errweigtave)

print(errweightave)
print(err_errweigtave)
