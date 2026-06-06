import uproot as up
import numpy as np
import root_numpy as rnp
import pandas as pd
import root_pandas as rpd
import ROOT
import scipy
import scipy.integrate as integrate
import matplotlib.pyplot as plt
import sys, math, os, subprocess
import csv

sys.path.insert(0, 'python/')
# Check the number of arguments provided to the script
#if len(sys.argv)-1!=1:
#   print("!!!!! ERROR !!!!!\n Expected 1 arguments\n  RunNumber \n!!!!! ERROR !!!!!")
#  sys.exit(1)
# Input params - run number and max number of events
runNumfilename = sys.argv[1]

#Physics summer 2019 Q2 data

RunNums = []

with open('/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/%s' % (runNumfilename), 'r') as file:

    for line in file:
        try:
            RunNum = int(line.strip())
            RunNums.append(RunNum)
        except ValueError:
            print(f"Skipping invalid line: {line}")
print(RunNums)

sum = 0.0

for i in RunNums:     
    with open('/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/%s.dat' % (i), 'r') as f:
        #  for line in f:   
        line1 = f.readlines()
        line2 = line1[0].split()
        sum = float(sum)+float(line2[1])

with open('/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_%s.csv' % (runNumfilename), 'w') as file:

    file.write(str(sum))

print("The effective charge is saved in 'CalEffCharge' directory")    
