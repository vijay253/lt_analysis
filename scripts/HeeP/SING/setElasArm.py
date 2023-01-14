#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-07-28 11:08:33 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import sys,os,math

KIN = sys.argv[1]
SPEC = sys.argv[2]
ebeam_elas = "{0:.3f}".format(float(sys.argv[3]))
eTh_elas = "{0:.3f}".format(float(sys.argv[4]))
InputSIMC = sys.argv[5]
inp_table = sys.argv[6]

################################################################################################################################################
'''
ltsep package import and pathing definitions
'''
# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__))

# Add this to all files for more dynamic pathing
UTILPATH=lt.UTILPATH
SIMCPATH=lt.SIMCPATH

################################################################################################################################################

# Open inp_f file to grab prescale values and tracking efficiency
inp_f = SIMCPATH+"/input/%s.inp" % InputSIMC

with open(inp_f, 'r') as f:
    f_data = f.read()

with open(inp_f, 'r') as f:
    # Search for keywords, then save as value in dictionary
    for line in f:
        data = line.split('=')
        if 'Ebeam' in data[0]:
            if not 'dEbeam' in data[0]:
                ebeam = data[1].split(";")[0]
        if 'spec%e%P' in data[0]:
            eP = data[1].split(";")[0]
        if 'spec%e%theta' in data[0]:
            eTh = data[1].split(";")[0]
        if 'spec%p%P' in data[0]:
            pP = data[1].split(";")[0]
        if 'spec%p%theta' in data[0]:
            pTh = data[1].split(";")[0]

inpDict = {
    "ebeam" : ebeam,
    "eP" : eP,
    "eTh" : eTh,
    "pP" : pP,
    "pTh" : pTh,
}

data_str = inp_table.split("Sig_p (fm^2/sr)")[1]
data = data_str.split("\n")
for i,l in enumerate(data):
    data[i] = l.split('    ')
    data[i] = [j.strip(' ').strip('\r') for j in data[i]]

for d in data:
    if eTh_elas in d[0]:
        eP_elas = d[2]
        pTh_elas = d[3]
        pP_elas = d[4]
        print("For ebeam = %s, eTh = %s the elastic settings are...\n" % (ebeam_elas,eTh_elas),d)

try:
    pTh_elas
    pP_elas
except NameError:
    print("Error: eTh = %s not found in elastic table..." % eTh_elas)
    sys.exit(0)
    
outDict = inpDict.copy()

for key,val in inpDict.items():
    if key == "ebeam":
        outDict[key] = " {0:.2f}\t\t".format(float(ebeam))
    if key == "eP":
        outDict[key] = " {0:.1f}\t\t".format(float(eP))
    if key == "eTh":
        outDict[key] = " {0:.3f}\t\t".format(float(eTh))
    if key == "pP":
        outDict[key] = " {0:.1f}\t\t".format(float(pP_elas)*1000) # Converts to MeV
    if key == "pTh":
        outDict[key] = " {0:.3f}\t\t".format(abs(float(pTh_elas))) # Removes minus sign

print("Original Values...\n",sorted(inpDict.items()))
print("New Values...\n",sorted(outDict.items()))

f_data = f_data.replace(inpDict['ebeam'],outDict['ebeam'])
f_data = f_data.replace(inpDict['eP'],outDict['eP'])
f_data = f_data.replace(inpDict['eTh'],outDict['eTh'])
f_data = f_data.replace(inpDict['pP'],outDict['pP'])
f_data = f_data.replace(inpDict['pTh'],outDict['pTh'])

with open(inp_f, 'w') as f:
    f.write(f_data)
