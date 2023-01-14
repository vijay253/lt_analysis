#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-12-05 23:41:10 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import pandas as pd
import re, sys, os

################################################################################################################################################
'''
User Inputs
'''
efficiency_table = sys.argv[1]
ROOTPrefix = sys.argv[2]
runNum = sys.argv[3]
MaxEvent=sys.argv[4]

################################################################################################################################################
'''
ltsep package import and pathing definitions
'''
# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__))

# Add this to all files for more dynamic pathing
UTILPATH=lt.UTILPATH

################################################################################################################################################
# Grab and calculate efficiency 

from getDataTable import calculate_efficiency

tot_efficiency = calculate_efficiency(runNum,efficiency_table)

################################################################################################################################################

# Open report file to grab prescale values and tracking efficiency
report = UTILPATH+"/REPORT_OUTPUT/Analysis/General/%s_%s_%s.report" % (ROOTPrefix,runNum,MaxEvent)
#report = UTILPATH+"/REPORT_OUTPUT/Analysis/HeeP/%s_%s_%s.report" % (ROOTPrefix,runNum,MaxEvent)

with open(report) as f:
    # Search for keywords, then save as value in dictionary
    for line in f:
        data = line.split(':')
        if 'BCM1_Beam_Cut_Charge' in data[0]:
            charge = int(re.sub("\D","","%s" % data[1]))
        if 'Total_Events' in data[0]:
            numevts = int(re.sub("\D","","%s" % data[1]))

effective_charge = float(charge/1000)*float(tot_efficiency)

# Need to convert to int value for bash to interpret correctly
print(int(1000*effective_charge))
