#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-09-09 01:39:32 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import pandas as pd
import sys,os

efficiency_table = "coin_heep_HeePCoin_efficiency_data_2022_09_09.csv"

###############################################################################################################################################

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

from getDataTable import get_efficiencies

runNums = [4827,4828,4855,4856,4857,4858,4859,4860,4862,4863]
#runNums = [7974,7975,7976]
#runNums = [7866,7867]
#runNums = [6881,6882]
#runNums = [6634,6635]
for i,evt in enumerate(runNums):
    effDict = get_efficiencies(evt,efficiency_table)[0]
    for key,val in effDict.items():
        if val < 0.75:
            print(key,"\t",val)

################################################################################################################################################

