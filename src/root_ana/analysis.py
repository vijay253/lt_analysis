#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-09-27 14:04:23 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
import cppyy
import sys,os

################################################################################################################################################
'''
User Inputs
'''

#eff_file = sys.argv[1]
#off_file = sys.argv[2]

eff_file = "list.settings.omega"
off_file = "offset.dat"

################################################################################################################################################
'''
ltsep package import and pathing definitions


# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__),"<runType>",ROOTPrefix,runNum,MaxEvent,cut_f,cuts)

# Add this to all files for more dynamic pathing
VOLATILEPATH=lt.VOLATILEPATH
ANALYSISPATH=lt.ANALYSISPATH
HCANAPATH=lt.HCANAPATH
REPLAYPATH=lt.REPLAYPATH
UTILPATH=lt.UTILPATH
PACKAGEPATH=lt.PACKAGEPATH
OUTPATH=lt.OUTPATH
ROOTPATH=lt.ROOTPATH
REPORTPATH=lt.REPORTPATH
CUTPATH=lt.CUTPATH
PARAMPATH=lt.PARAMPATH
SCRIPTPATH=lt.SCRIPTPATH
SIMCPATH=lt.SIMCPATH
ANATYPE=lt.ANATYPE
USER=lt.USER
HOST=lt.HOST

cut_f = '/DB/CUTS/run_type/<cut>.cuts'

cuts = []

proc_root = lt.setup_ana()
c = proc_root[0] # Cut object
tree = proc_root[1] # Dictionary of branches
strDict = proc_root[2] # Dictionary of cuts as strings

'''
################################################################################################################################################
from cppfiles import read_setting, general_utility

#cppyy.include('general_utility.h')

def Analysis(eff_struc):

    eff_ana = eff_struc

    Init()

def Init():

    cppyy.cppdef(general_utility())
    cppyy.cppdef(read_setting())
    inp_f = cppyy.gbl.ReadFile(eff_file, off_file)
    print(inp_f)
    kin_ana = rff.kin_pro
