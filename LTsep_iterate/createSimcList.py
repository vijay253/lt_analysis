#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2023-01-16 19:20:17 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import sys, os

##################################################################################################################################################

# Check the number of arguments provided to the script
if len(sys.argv)-1!=7:
    print("!!!!! ERROR !!!!!\n Expected 7 arguments\n Usage is with - Q2 POL EPSVAL runNumRight runNumLeft runNumCenter kinematics\n!!!!! ERROR !!!!!")
    sys.exit(1)

################################################################################################################################################
'''
User Inputs
'''

Q2 = sys.argv[1].replace("p",".")
POL = sys.argv[2]
EPSVAL = sys.argv[3]

runNumRight = sys.argv[4].split(" ")
runNumLeft = sys.argv[5].split(" ")
runNumCenter = sys.argv[6].split(" ")

kinematics = sys.argv[7].split("_")

InSIMCFilenameRight = "Prod_Coin_{}.root".format(kinematics[0]+"right_"+kinematics[1])
InSIMCFilenameLeft = "Prod_Coin_{}.root".format(kinematics[0]+"left_"+kinematics[1])
InSIMCFilenameCenter = "Prod_Coin_{}.root".format(kinematics[0]+"center_"+kinematics[1])

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
ANATYPE=lt.ANATYPE
OUTPATH=lt.OUTPATH

###############################################################################################################################################

# Grabs simc number of events and normalizaton factor
simc_right_hist = "%s/OUTPUT/Analysis/%sLT/%s" % (LTANAPATH,ANATYPE,InSIMCFilenameRight.replace('.root','.hist'))
try:
    f_simc_right = open(simc_right_hist)
    angle_flag = False
    for line in f_simc_right:
        #print(line)
        if "Ebeam" in line:
            val = line.split("=")
            EbeamValRight = float(val[1].replace("MeV\n",""))/1000
        if "angle" in line and angle_flag == False:
            angle_flag = True
            val = line.split("=")
            pThetaValRight = float(val[1].replace("deg\n","").split("          ")[1])
        if "Ngen" in line:
            val = line.split("=")
            simc_right_nevents = int(val[1])
        if "normfac" in line:
            val = line.split("=")
            simc_right_normfactor = float(val[1])
    if 'simc_right_nevents' and 'simc_right_normfactor' in locals():
        print('\n\nsimc_right_nevents = ',simc_right_nevents,'\nsimc_right_normfactor = ',simc_right_normfactor,'\n\n')
    else:
        print("ERROR: Invalid simc right hist file %s" % simc_right_hist)
        #sys.exit(1)
    f_simc_right.close()
except FileNotFoundError:
    print("No right setting...")
    
simc_left_hist = "%s/OUTPUT/Analysis/%sLT/%s" % (LTANAPATH,ANATYPE,InSIMCFilenameLeft.replace('.root','.hist'))
f_simc_left = open(simc_left_hist)
angle_flag = False
for line in f_simc_left:
    #print(line)
    if "Ebeam" in line:
        val = line.split("=")
        EbeamValLeft = float(val[1].replace("MeV\n",""))/1000
    if "angle" in line and angle_flag == False:
        angle_flag = True
        val = line.split("=")
        pThetaValLeft = float(val[1].replace("deg\n","").split("          ")[1])
    if "Ngen" in line:
        val = line.split("=")
        simc_left_nevents = int(val[1])
    if "normfac" in line:
        val = line.split("=")
        simc_left_normfactor = float(val[1])
if 'simc_left_nevents' and 'simc_left_normfactor' in locals():
    print('\n\nsimc_left_nevents = ',simc_left_nevents,'\nsimc_left_normfactor = ',simc_left_normfactor,'\n\n')
else:
    print("ERROR: Invalid simc left hist file %s" % simc_left_hist)
    #sys.exit(1)
f_simc_left.close()

simc_center_hist = "%s/OUTPUT/Analysis/%sLT/%s" % (LTANAPATH,ANATYPE,InSIMCFilenameCenter.replace('.root','.hist'))
f_simc_center = open(simc_center_hist)
angle_flag = False
for line in f_simc_center:
    #print(line)
    if "Ebeam" in line:
        val = line.split("=")
        EbeamValCenter = float(val[1].replace("MeV\n",""))/1000
    if "angle" in line and angle_flag == False:
        angle_flag = True
        val = line.split("=")
        pThetaValCenter = float(val[1].replace("deg\n","").split("          ")[1])
    if "Ngen" in line:
        val = line.split("=")
        simc_center_nevents = int(val[1])
    if "normfac" in line:
        val = line.split("=")
        simc_center_normfactor = float(val[1])
if 'simc_center_nevents' and 'simc_center_normfactor' in locals():
    print('\n\nsimc_center_nevents = ',simc_center_nevents,'\nsimc_center_normfactor = ',simc_center_normfactor,'\n\n')
else:
    print("ERROR: Invalid simc center hist file %s" % simc_center_hist)
    #sys.exit(1)
f_simc_center.close()

################################################################################################################################################

def write_to_file(f_out,line):
    # Open a file in append mode
    with open(f_out, 'a') as f:
        # Write the value of the variable to the file
        f.write(line)

################################################################################################################################################

# Define thpq vector relative to middle setting
try:
    thpq_right = abs(float(pThetaValCenter)-float(pThetaValRight))
except NameError:
    print("")
thpq_left = abs(float(pThetaValCenter)-float(pThetaValLeft))
thpq_center = 0.000

f_list = '{}/src/root_ana/list.simc'.format(LTANAPATH)

if not os.path.exists(f_list):
    open(f_list, "w").close()    
# Open a file in write mode
with open(f_list, 'r') as f:
    lines = f.readlines()
    try:
        if int(runNumRight[0]) == 0:
            # Write the value of the variable to the file
            check_line = "{} {:.5f} {} -{:.3f} {} {}\n".format(Q2,float(EbeamValRight),EPSVAL,thpq_right,simc_right_normfactor,simc_right_nevents)
            # Check if the line already exists
            if check_line not in lines:
                write_to_file(f_list,check_line)
    except NameError:
        print("")                
    if int(runNumLeft[0]) == 1:
        check_line = "{} {:.5f} {} +{:.3f} {} {}\n".format(Q2,float(EbeamValLeft),EPSVAL,thpq_left,simc_left_normfactor,simc_left_nevents)
        if check_line not in lines:
            write_to_file(f_list,check_line)
    if int(runNumCenter[0]) == 2:
        check_line = "{} {:.5f} {} {:.3f} {} {}\n".format(Q2,float(EbeamValCenter),EPSVAL,thpq_center,simc_center_normfactor,simc_center_nevents)
        if check_line not in lines:
            write_to_file(f_list,check_line)
