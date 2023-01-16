#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2023-01-16 00:41:20 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import sys, os

##################################################################################################################################################

# Check the number of arguments provided to the script
if len(sys.argv)-1!=29:
    print("!!!!! ERROR !!!!!\n Expected 29 arguments\n Usage is with - Q2 POL EPSVAL TMIN TMAX NumtBins Kset runNumRight runNumLeft runNumCenter pThetaValRight pThetaValLeft pThetaValCenter EbeamValRight EbeamValLeft EbeamValCenter EffValRight EffValLeft EffValCenter EffErrRight EffErrLeft EffErrCenter ChargeValRight ChargeValLeft ChargeValCenter ChargeErrRight ChargeErrLeft ChargeErrCenter \n!!!!! ERROR !!!!!")
    sys.exit(1)

################################################################################################################################################
'''
User Inputs
'''

Q2 = sys.argv[1].replace("p",".")
POL = sys.argv[2]
EPSVAL = sys.argv[3]
TMIN = sys.argv[4]
TMAX = sys.argv[5]
NumtBins = sys.argv[6]
Kset = sys.argv[7]

runNumRight = sys.argv[8].split(" ")
runNumLeft = sys.argv[9].split(" ")
runNumCenter = sys.argv[10].split(" ")

pThetaValRight = sys.argv[11].split(" ")
pThetaValLeft = sys.argv[12].split(" ")
pThetaValCenter = sys.argv[13].split(" ")

EbeamValRight = sys.argv[14].split(" ")
EbeamValLeft = sys.argv[15].split(" ")
EbeamValCenter = sys.argv[16].split(" ")

EffValRight = sys.argv[17].split(" ")
EffValLeft = sys.argv[18].split(" ")
EffValCenter = sys.argv[19].split(" ")
EffErrRight = sys.argv[20].split(" ")
EffErrLeft = sys.argv[21].split(" ")
EffErrCenter = sys.argv[22].split(" ")

ChargeValRight = sys.argv[23].split(" ")
ChargeValLeft = sys.argv[24].split(" ")
ChargeValCenter = sys.argv[25].split(" ")
ChargeErrRight = sys.argv[26].split(" ")
ChargeErrLeft = sys.argv[27].split(" ")
ChargeErrCenter = sys.argv[28].split(" ")

TargetType = sys.argv[29]

################################################################################################################################################
'''
ltsep package import and pathing definitions
'''
# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__))

# Add this to all files for more dynamic pathing
UTILPATH=lt.UTILPATH
LTANAPATH=lt.LTANAPATH

################################################################################################################################################

def write_to_file(f_out,line):
    # Open a file in append mode
    with open(f_out, 'a') as f:
        # Write the value of the variable to the file
        f.write(line)

################################################################################################################################################

# Define thpq vector relative to middle setting
thpq_right = abs(float(pThetaValCenter[0])-float(pThetaValRight[0]))
thpq_left = abs(float(pThetaValCenter[0])-float(pThetaValLeft[0]))
thpq_center = 0.000

f_list_settings = '{}/src/root_ana/list.settings'.format(LTANAPATH)
if not os.path.exists(f_list_settings):
    open(f_list_settings, "w").close()
# First check if line exists
with open(f_list_settings, 'r') as f:
    lines = f.readlines()
    if float(runNumRight[0]) != 0.0:
        check_line = "{} {} {} -{:.3f} {} {} {} {}\n".format(POL,Q2,EPSVAL,thpq_right,TMIN,TMAX,NumtBins,Kset)
        # Check if the line already exists
        if check_line not in lines:
            write_to_file(f_list_settings,check_line)
    if float(runNumLeft[0]) != 0.0:
        check_line = "{} {} {} +{:.3f} {} {} {} {}\n".format(POL,Q2,EPSVAL,thpq_left,TMIN,TMAX,NumtBins,Kset)
        if check_line not in lines:
            write_to_file(f_list_settings,check_line)
    if float(runNumCenter[0]) != 0.0:
        check_line = "{} {} {} {:.3f} {} {} {} {}\n".format(POL,Q2,EPSVAL,thpq_center,TMIN,TMAX,NumtBins,Kset)
        if check_line not in lines:
            write_to_file(f_list_settings,check_line)

if TargetType == "dummy":
    f_list = '{}/src/lists/list.dummy_{}_{:.0f}'.format(LTANAPATH,Q2.replace(".",""),float(EPSVAL)*100)
else:
    f_list = '{}/src/lists/list.{}_{:.0f}'.format(LTANAPATH,Q2.replace(".",""),float(EPSVAL)*100)

if not os.path.exists(f_list):
    open(f_list, "w").close()    
# Open a file in write mode
with open(f_list, 'r') as f:
    lines = f.readlines()
    if float(runNumRight[0]) != 0.0:
        # Write the value of the variable to the file
        for i,thpq in enumerate(EbeamValRight):
            # convert uC to C (10^-6C=1uC)
            check_line = "{} {} {} {} {} {:.5f} {} {} -{:.3f}\n" \
                    .format(runNumRight[i],Q2,EbeamValRight[i],float(ChargeValRight[i])/1000000,ChargeErrRight[i], \
                            float(EffValRight[i]),EffErrRight[i],EPSVAL,thpq_right)
            # Check if the line already exists
            if check_line not in lines:
                write_to_file(f_list,check_line)
    if float(runNumLeft[0]) != 0.0:
        for i,thpq in enumerate(EbeamValLeft):
            check_line = "{} {} {} {} {} {:.5f} {} {} +{:.3f}\n" \
                    .format(runNumLeft[i],Q2,EbeamValLeft[i],float(ChargeValLeft[i])/1000000,ChargeErrLeft[i], \
                            float(EffValLeft[i]),EffErrLeft[i],EPSVAL,thpq_left)
            if check_line not in lines:
                write_to_file(f_list,check_line)
    if float(runNumCenter[0]) != 0.0:
        for i,thpq in enumerate(EbeamValCenter):
            check_line = "{} {} {} {} {} {:.5f} {} {} {:.3f}\n" \
                    .format(runNumCenter[i],Q2,EbeamValCenter[i],float(ChargeValCenter[i])/1000000,ChargeErrCenter[i], \
                            float(EffValCenter[i]),EffErrCenter[i],EPSVAL,thpq_center)
            if check_line not in lines:
                write_to_file(f_list,check_line)
