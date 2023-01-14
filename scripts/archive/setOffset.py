#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-09-08 04:49:50 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import sys,os,math

InputSIMC = sys.argv[1]
offset = sys.argv[2]

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

# Open inp_f file to grab energy, momentum, and angles 
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

outDict = inpDict.copy()

offset = offset.split(",")

print("Given offsets...\n",offset,"\n")

for off in offset:
    for key,val in inpDict.items():
        val = float(val)
        if key in off:
            offsetPct = float(off.replace(key,"").replace("=",""))
            if "Th" in key:
                offsetValue = offsetPct*(180/(1000*math.pi))
            else:
                offsetValue = val*(offsetPct/1000)
            if key == "ebeam":
                outDict[key] = " {0:.2f}  \t\t".format(val+offsetValue)
            if key == "eP":
                outDict[key] = " {0:.1f}\t\t".format(val+offsetValue)
            if key == "eTh":
                outDict[key] = " {0:.3f}\t\t".format(val+offsetValue)
            if key == "pP":
                outDict[key] = " {0:.1f}\t\t".format(val+offsetValue)
            if key == "pTh":
                outDict[key] = " {0:.3f}   \t".format(val+offsetValue)

print("Original Values...\n",sorted(inpDict.items()))
print("Offset Values...\n",sorted(outDict.items()))

f_data = f_data.replace(inpDict['ebeam'],outDict['ebeam'])
f_data = f_data.replace(inpDict['eP'],outDict['eP'])
f_data = f_data.replace(inpDict['eTh'],outDict['eTh'])
f_data = f_data.replace(inpDict['pP'],outDict['pP'])
f_data = f_data.replace(inpDict['pTh'],outDict['pTh'])

with open(inp_f, 'w') as f:
    f.write(f_data)
