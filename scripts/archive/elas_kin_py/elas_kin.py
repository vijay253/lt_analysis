#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-06-01 21:11:03 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import numpy as np
from numpy import f2py
import pandas as pd
import os,sys

##################################################################################################################################################
# Fortran compiler to extract elastic kinematic data

with open("elas_kin.f") as srcf:
    srcc = srcf.read()
f2py.compile(srcc,modulename='elas_kin', verbose=False)

try:
    import elas_kin
except ModuleNotFoundError:
    print("ERROR: Module for %s not found..." % inp_f)
    sys.exit(0)

print("\n\nDoc for elas_kin.f...")    
print(elas_kin.__doc__)

##################################################################################################################################################

beam_energy = float(sys.argv[1])
theta_e = float(sys.argv[2])
theta_lim = 0.05

N = 1000
Th_e = np.zeros(N)
Q2 = np.zeros(N)
E_e = np.zeros(N)
Th_p = np.zeros(N)
P_p = np.zeros(N)
Mott = np.zeros(N)
Sig_p = np.zeros(N)

# Th_e,Q2,E_e,Th_p,P_p,Mott,Sig_p are will get redefined with the proper values from calculations
elas_kin.kin_table(beam_energy,Th_e,Q2,E_e,Th_p,P_p,Mott,Sig_p,N)

kinDict = {

    'Theta_e' : Th_e,
    'Q2' : Q2,
    'E_e' : E_e,
    'Theta_p' : Th_p,
    'P_p' : P_p,
    'Mott' : Mott,
    'Sig_p (fm^2/sr)' : Sig_p,
}

kin_data = {i : kinDict[i] for i in sorted(kinDict.keys())}

# Convert merged dictionary to a pandas dataframe then sort it
table  = pd.DataFrame(kin_data, columns=kin_data.keys())
table = table.reindex(sorted(table.columns), axis=1)

good_kin = table[table['Theta_e'].between(theta_e-theta_lim,theta_e+theta_lim,inclusive=True)]

print(good_kin)
