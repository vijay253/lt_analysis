#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-12-22 13:27:45 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import pandas as pd
import sys,os

runNum = sys.argv[1]
efficiency_table = sys.argv[2]
table_val = sys.argv[3]

###############################################################################################################################################

'''
ltsep package import and pathing definitions
'''

# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__))

# Add this to all files for more dynamic pathing
UTILPATH=lt.UTILPATH

if table_val == "efficiency":
    ################################################################################################################################################
    # Grab and calculate efficiency 

    from getDataTable import calculate_efficiency

    tot_efficiency = calculate_efficiency(runNum,efficiency_table)

    ################################################################################################################################################

    print(tot_efficiency)
    
if table_val == "ebeam":
    ################################################################################################################################################
    # Grab beam energy

    from getDataTable import get_ebeam

    ebeam_val = get_ebeam(runNum,efficiency_table)

    ################################################################################################################################################

    print(ebeam_val)

if table_val == "pTheta":
    ################################################################################################################################################
    # Grab pTheta

    from getDataTable import get_pTheta

    pTheta_val = get_pTheta(runNum,efficiency_table)

    ################################################################################################################################################

    print(pTheta_val)
    
