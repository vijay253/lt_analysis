#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-07-26 17:23:26 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#

###################################################################################################################################################

# Import relevant packages
import uproot as up
import numpy as np
import root_numpy as rnp
import pandas as pd
import root_pandas as rpd
import ROOT
import scipy
import scipy.integrate as integrate
import matplotlib.pyplot as plt
import sys, math, os, subprocess

##################################################################################################################################################

# Check the number of arguments provided to the script
if len(sys.argv)-1!=2:
    print("!!!!! ERROR !!!!!\n Expected 2 arguments\n Usage is with - ROOTfilePrefix RunNumber MaxEvents \n!!!!! ERROR !!!!!")
    sys.exit(1)

##################################################################################################################################################

# Input params - run number and max number of events
runNum = sys.argv[1]
MaxEvent = "-1"
spec = sys.argv[2]

################################################################################################################################################
'''
ltsep package import and pathing definitions
'''

# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__),"SimcSing_%s" % spec)

# Add this to all files for more dynamic pathing
USER=lt.USER # Grab user info for file finding
HOST=lt.HOST
REPLAYPATH=lt.REPLAYPATH
UTILPATH=lt.UTILPATH
SIMCPATH=lt.SIMCPATH
ANATYPE=lt.ANATYPE
OUTPATH=lt.OUTPATH

##############################################################################################################################################

ROOTPrefix = "%s_%s_replay_production" % (ANATYPE,spec)

##############################################################################################################################################
'''
Define and set up cuts
'''

cut_f = '/DB/CUTS/run_type/simc_sing_heep.cuts'

# defining Cuts
if spec == 'HMS':
    cuts = ["ecut_pid"]
else:
    cuts = ["pcut_pid"]

lt=Root(os.path.realpath(__file__),"SimcSing_%s" % spec,ROOTPrefix,runNum,MaxEvent,cut_f,cuts)

proc_root = lt.setup_ana()
c = proc_root[0] # Cut object
tree = proc_root[1] # Dictionary of branches
strDict = proc_root[2] # Dictionary of cuts as strings

#################################################################################################################################################################

def shms_protons():

    # Define the array of arrays containing the relevant SHMS and SSHMS info                              

    NoCut_SHMS_Protons = [tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMp"], tree["P_RF_Dist"], tree["W"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"]]

    Uncut_SHMS_Protons = [(tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMp"], tree["P_RF_Dist"], tree["W"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"]) for (tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMp"], tree["P_RF_Dist"], tree["W"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"]) in zip(*NoCut_SHMS_Protons)]

    # Create array of arrays of pions after cuts, all events, prompt and random          

    Cut_SHMS_Protons_tmp = NoCut_SHMS_Protons
    Cut_SHMS_Protons_all_tmp = []

    for arr in Cut_SHMS_Protons_tmp:
        Cut_SHMS_Protons_all_tmp.append(c.add_cut(arr, "pcut_pid"))

    Cut_SHMS_Protons_all = [(tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMp"], tree["P_RF_Dist"], tree["W"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"]) for (tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMp"], tree["P_RF_Dist"], tree["W"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"]) in zip(*Cut_SHMS_Protons_all_tmp)]

    SHMS_Protons = {
        "Uncut_Proton_Events" : Uncut_SHMS_Protons,
        "Cut_Proton_Events_All" : Cut_SHMS_Protons_all,
        }

    return SHMS_Protons

def hms_protons():

    # Define the array of arrays containing the relevant HMS and SHMS info                              

    NoCut_HMS_Protons = [tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"],tree["H_RF_Dist"], tree["H_W"]]

    Uncut_HMS_Protons = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"],tree["H_RF_Dist"], tree["H_W"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"],tree["H_RF_Dist"], tree["H_W"]) in zip(*NoCut_HMS_Protons)]

    # Create array of arrays of pions after cuts, all events, prompt and random          

    Cut_HMS_Protons_tmp = NoCut_HMS_Protons
    Cut_HMS_Protons_all_tmp = []

    for arr in Cut_HMS_Protons_tmp:
        Cut_HMS_Protons_all_tmp.append(c.add_cut(arr, "ecut_pid"))

    Cut_HMS_Protons_all = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"],tree["H_RF_Dist"], tree["H_W"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"],tree["H_RF_Dist"], tree["H_W"]) in zip(*Cut_HMS_Protons_all_tmp)]

    HMS_Protons = {
        "Uncut_Proton_Events" : Uncut_HMS_Protons,
        "Cut_Proton_Events_All" : Cut_HMS_Protons_all,
        }

    return HMS_Protons

##################################################################################################################################################################

def main():
    
    # This is just the list of branches we use from the initial root file for each dict
    # I don't like re-defining this here as it's very prone to errors if you included (or removed something) earlier but didn't modify it here
    # Should base the branches to include based on some list and just repeat the list here (or call it again directly below)

    if "SHMS" in spec:
        Sing_Proton_Data = shms_protons()
        Sing_Proton_Data_Header = ["ssyptar","ssxptar","ssypfp","ssxpfp","ssyfp","ssxfp","P_dc_InsideDipoleExit", "P_gtr_eta", "P_gtr_p", "ssdelta", "P_hod_goodscinhit", "P_hod_goodstarttime", "P_cal_etotnorm", "P_cal_etottracknorm", "P_aero_npeSum", "P_aero_xAtAero", "P_aero_yAtAero", "P_hgcer_npeSum", "P_hgcer_xAtCer", "P_hgcer_yAtCer", "MMp","P_RF_Dist", "W", "emiss", "pmiss", "pmx", "pmy", "pmz"]
    elif "HMS" in spec:
        Sing_Proton_Data = hms_protons()
        Sing_Proton_Data_Header = ["hsyptar","hsxptar","hsypfp","hsxpfp","hsyfp","hsxfp","H_dc_InsideDipoleExit","H_gtr_eta", "hsdelta", "H_gtr_p", "H_hod_goodscinhit", "H_hod_goodstarttime", "H_cal_etotnorm", "H_cal_etottracknorm", "H_cer_npeSum","H_RF_Dist", "W"]
    else:
        print("Error: %s invalid spectrometer" % SPEC)
        sys.exit(0)
        
    # Need to create a dict for all the branches we grab                                                
    data = {}
    data.update(Sing_Proton_Data)
    data_keys = list(data.keys()) # Create a list of all the keys in all dicts added above, each is an array of data

    for i in range (0, len(data_keys)):
        if("Proton" in data_keys[i]):
            DFHeader=list(Sing_Proton_Data_Header)
        else:
            continue
            # Uncomment the line below if you want .csv file output, WARNING the files can be very large and take a long time to process!                                                                      
            #pd.DataFrame(data.get(data_keys[i])).to_csv("%s/%s_%s.csv" % (OUTPATH, data_keys[i], runNum), header=DFHeader, index=False) # Convert array to panda dataframe and write to csv with correct header                                                                                                      
        if (i == 0):
            pd.DataFrame(data.get(data_keys[i]), columns = DFHeader, index = None).to_root("%s/%s_%s_%s_Raw_Data.root" % (OUTPATH, runNum, MaxEvent, spec), key ="%s" % data_keys[i])
        elif (i != 0):
            pd.DataFrame(data.get(data_keys[i]), columns = DFHeader, index = None).to_root("%s/%s_%s_%s_Raw_Data.root" % (OUTPATH, runNum, MaxEvent, spec), key ="%s" % data_keys[i], mode ='a')

if __name__ == '__main__':
    main()
print ("Processing Complete")
