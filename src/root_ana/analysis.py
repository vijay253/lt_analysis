#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-09-27 14:17:12 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#
from cppyy import include
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

include('analysis.h')

def Analysis(eff_struc):

    eff_ana = eff_struc

    Init()

def Init():

    cppyy.cppdef(general_utility())
    cppyy.cppdef(read_setting())
    inp_f = cppyy.gbl.ReadFile(eff_file, off_file)
    print(inp_f)
    kin_ana = inp_f.kin_pro
    print(kin_ana)
    cen_ana = inp_f.cen_pro
    print(cen_ana)
    list_file = inp_f.Get_List_File()
    print(list_file)
    data_file_dir = "data/"
    Para_Init(kin_ana)

def Para_Init(kin_ana):
    
    Q_2		= kin_ana.Q2set[0]
    kset	= kin_ana.kset[0]
    t_min	= kin_ana.tmnset[0]
    t_max	= kin_ana.tmxset[0]

    t_bin_set = kin_ana.NBtset[0]
    if t_bin_set > 0.0:
        t_width   = ( t_max - t_min ) / t_bin_set
    else:
        t_width   = 0.0
        
        

    phi_bin_num = 16

    pstp = 360./phi_bin_num
    pmn  = pstp/2.
    pmx  = 360. - pstp/2.

    acccc_temp = 0.0
    errrr_temp = 0.0
    charge_tot = 0.0


    yield_vec.resize(t_bin_set*phi_bin_num)
    yield_err_vec.resize(t_bin_set*phi_bin_num)
    phi_vec.resize(t_bin_set*phi_bin_num)
    tb_vec.resize(t_bin_set*phi_bin_num)
