#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2023-01-12 15:39:17 trottar"
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
if len(sys.argv)-1!=1:
    print("!!!!! ERROR !!!!!\n Expected 3 arguments\n Usage is with - ROOTfilePrefix RunNumber MaxEvents \n!!!!! ERROR !!!!!")
    sys.exit(1)

##################################################################################################################################################

# Input params - run number and max number of events
runNum = sys.argv[1]
MaxEvent = "-1"

################################################################################################################################################
'''
ltsep package import and pathing definitions
'''

# Import package for cuts
from ltsep import Root

lt=Root(os.path.realpath(__file__))

# Add this to all files for more dynamic pathing
USER=lt.USER # Grab user info for file finding
HOST=lt.HOST
REPLAYPATH=lt.REPLAYPATH
UTILPATH=lt.UTILPATH
SIMCPATH=lt.SIMCPATH
ANATYPE=lt.ANATYPE
OUTPATH=lt.OUTPATH

##############################################################################################################################################

ROOTPrefix = "%s_coin_replay_production" % ANATYPE

##############################################################################################################################################
'''
Define and set up cuts
'''

particle = "kaon"

cut_f = '/DB/CUTS/run_type/coin_prod.cuts'

# defining Cuts
if particle == "kaon":
    cuts = ["coin_ek_cut_all_noRF","coin_ek_cut_prompt_noRF","coin_ek_cut_rand_noRF","coin_ek_cut_all_RF","coin_ek_cut_prompt_RF","coin_ek_cut_rand_RF"]
if particle == "pion":
    cuts = ["coin_epi_cut_all_noRF","coin_epi_cut_prompt_noRF","coin_epi_cut_rand_noRF","coin_epi_cut_all_RF","coin_epi_cut_prompt_RF","coin_epi_cut_rand_RF"]

lt=Root(os.path.realpath(__file__),"Prod",ROOTPrefix,runNum,MaxEvent,cut_f,cuts)

# Add this to all files for more dynamic pathing
USER=lt.USER # Grab user info for file finding
HOST=lt.HOST
REPLAYPATH=lt.REPLAYPATH
UTILPATH=lt.UTILPATH
SIMCPATH=lt.SIMCPATH
ANATYPE=lt.ANATYPE
OUTPATH=lt.OUTPATH

proc_root = lt.setup_ana()
c = proc_root[0] # Cut object
tree = proc_root[1] # Dictionary of branches
strDict = proc_root[2] # Dictionary of cuts as strings

#################################################################################################################################################################

def coin_kaons():

    # Define the array of arrays containing the relevant HMS and SHMS info                              

    NoCut_COIN_Kaons = [tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]]

    Uncut_COIN_Kaons = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*NoCut_COIN_Kaons)
        ]

    # Create array of arrays of pions after cuts, all events, prompt and random          

    Cut_COIN_Kaons_tmp = NoCut_COIN_Kaons
    coin_ek_cut_all_noRF_tmp = []
    coin_ek_cut_prompt_noRF_tmp = []
    coin_ek_cut_rand_noRF_tmp = []
    coin_ek_cut_all_RF_tmp = []
    coin_ek_cut_prompt_RF_tmp = []
    coin_ek_cut_rand_RF_tmp = []

    for arr in Cut_COIN_Kaons_tmp:
        coin_ek_cut_all_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_all_noRF"))
        coin_ek_cut_prompt_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_prompt_noRF"))
        coin_ek_cut_rand_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_rand_noRF"))
        coin_ek_cut_all_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_all_RF"))
        coin_ek_cut_prompt_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_prompt_RF"))
        coin_ek_cut_rand_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_rand_RF"))

    Cut_Kaon_Events_all_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_all_noRF_tmp)]

    Cut_Kaon_Events_prompt_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_prompt_noRF_tmp)]

    Cut_Kaon_Events_rand_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_rand_noRF_tmp)]    

    Cut_Kaon_Events_all_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_all_RF_tmp)]

    Cut_Kaon_Events_prompt_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_prompt_RF_tmp)]

    Cut_Kaon_Events_rand_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_rand_RF_tmp)]    
    
    COIN_Kaons = {
        "Uncut_Kaon_Events" : Uncut_COIN_Kaons,
        "Cut_Kaon_Events_all_noRF" : Cut_Kaon_Events_all_noRF,
        "Cut_Kaon_Events_prompt_noRF" : Cut_Kaon_Events_prompt_noRF,
        "Cut_Kaon_Events_rand_noRF" : Cut_Kaon_Events_rand_noRF,
        "Cut_Kaon_Events_all_RF" : Cut_Kaon_Events_all_RF,
        "Cut_Kaon_Events_prompt_RF" : Cut_Kaon_Events_prompt_RF,
        "Cut_Kaon_Events_rand_RF" : Cut_Kaon_Events_rand_RF,
        }

    return COIN_Kaons

##################################################################################################################################################################

def coin_kaons():

    # Define the array of arrays containing the relevant HMS and SHMS info                              

    NoCut_COIN_Kaons = [tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]]

    Uncut_COIN_Kaons = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*NoCut_COIN_Kaons)
        ]

    # Create array of arrays of pions after cuts, all events, prompt and random          

    Cut_COIN_Kaons_tmp = NoCut_COIN_Kaons
    coin_ek_cut_all_noRF_tmp = []
    coin_ek_cut_prompt_noRF_tmp = []
    coin_ek_cut_rand_noRF_tmp = []
    coin_ek_cut_all_RF_tmp = []
    coin_ek_cut_prompt_RF_tmp = []
    coin_ek_cut_rand_RF_tmp = []

    for arr in Cut_COIN_Kaons_tmp:
        coin_ek_cut_all_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_all_noRF"))
        coin_ek_cut_prompt_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_prompt_noRF"))
        coin_ek_cut_rand_noRF_tmp.append(c.add_cut(arr, "coin_ek_cut_rand_noRF"))
        coin_ek_cut_all_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_all_RF"))
        coin_ek_cut_prompt_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_prompt_RF"))
        coin_ek_cut_rand_RF_tmp.append(c.add_cut(arr, "coin_ek_cut_rand_RF"))

    Cut_Kaon_Events_all_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_all_noRF_tmp)]

    Cut_Kaon_Events_prompt_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_prompt_noRF_tmp)]

    Cut_Kaon_Events_rand_noRF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_rand_noRF_tmp)]    

    Cut_Kaon_Events_all_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_all_RF_tmp)]

    Cut_Kaon_Events_prompt_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_prompt_RF_tmp)]

    Cut_Kaon_Events_rand_RF = [(tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) for (tree["H_gtr_yp"],tree["H_gtr_xp"],tree["H_dc_yp_fp"],tree["H_dc_xp_fp"],tree["H_dc_y_fp"],tree["H_dc_x_fp"],tree["P_gtr_yp"],tree["P_gtr_xp"],tree["P_dc_yp_fp"],tree["P_dc_xp_fp"],tree["P_dc_y_fp"],tree["P_dc_x_fp"],tree["P_dc_InsideDipoleExit"],tree["H_dc_InsideDipoleExit"],tree["H_gtr_beta"],  tree["H_gtr_dp"], tree["H_gtr_p"], tree["H_hod_goodscinhit"], tree["H_hod_goodstarttime"], tree["H_cal_etotnorm"], tree["H_cal_etottracknorm"], tree["H_cer_npeSum"], tree["CTime_eKCoinTime_ROC1"], tree["P_gtr_beta"],  tree["P_gtr_p"], tree["P_gtr_dp"], tree["P_hod_goodscinhit"], tree["P_hod_goodstarttime"], tree["P_cal_etotnorm"], tree["P_cal_etottracknorm"], tree["P_aero_npeSum"], tree["P_aero_xAtAero"], tree["P_aero_yAtAero"], tree["P_hgcer_npeSum"], tree["P_hgcer_xAtCer"], tree["P_hgcer_yAtCer"], tree["MMK"], tree["H_RF_Dist"],tree["P_RF_Dist"], tree["Q2"], tree["W"], tree["epsilon"], tree["ph_q"],tree["ph_recoil"],tree["th_q"],tree["th_recoil"], tree["MandelT"], tree["emiss"],tree["pmiss"], tree["pmiss_x"], tree["pmiss_y"], tree["pmiss_z"],tree["Erecoil"],tree["emiss_nuc"],tree["Mrecoil"],tree["raster_x"],tree["raster_y"],tree["raster_z"]) in zip(*coin_ek_cut_rand_RF_tmp)]    
    
    COIN_Kaons = {
        "Uncut_Kaon_Events" : Uncut_COIN_Kaons,
        "Cut_Kaon_Events_all_noRF" : Cut_Kaon_Events_all_noRF,
        "Cut_Kaon_Events_prompt_noRF" : Cut_Kaon_Events_prompt_noRF,
        "Cut_Kaon_Events_rand_noRF" : Cut_Kaon_Events_rand_noRF,
        "Cut_Kaon_Events_all_RF" : Cut_Kaon_Events_all_RF,
        "Cut_Kaon_Events_prompt_RF" : Cut_Kaon_Events_prompt_RF,
        "Cut_Kaon_Events_rand_RF" : Cut_Kaon_Events_rand_RF,
        }

    return COIN_Kaons

##################################################################################################################################################################

def main():

    if particle == "kaon":
        COIN_Data = coin_kaons()
    if particle == "pion":
        COIN_Data = coin_pions()        

    # This is just the list of branches we use from the initial root file for each dict
    # I don't like re-defining this here as it's very prone to errors if you included (or removed something) earlier but didn't modify it here
    # Should base the branches to include based on some list and just repeat the list here (or call it again directly below)
    # RLT: I did what Stephen said not to so I can match naming scheme of simc root output

    COIN_Data_Header = ["hsyptar","hsxptar","hsypfp","hsxpfp","hsyfp","hsxfp","ssyptar","ssxptar","ssypfp","ssxpfp","ssyfp","ssxfp","P_dc_InsideDipoleExit","H_dc_InsideDipoleExit","H_gtr_beta", "hsdelta", "H_gtr_p", "H_hod_goodscinhit", "H_hod_goodstarttime", "H_cal_etotnorm", "H_cal_etottracknorm", "H_cer_npeSum", "CTime_ROC1", "P_gtr_beta", "P_gtr_p", "ssdelta", "P_hod_goodscinhit", "P_hod_goodstarttime", "P_cal_etotnorm", "P_cal_etottracknorm", "P_aero_npeSum", "P_aero_xAtAero", "P_aero_yAtAero", "P_hgcer_npeSum", "P_hgcer_xAtCer", "P_hgcer_yAtCer", "MM", "H_RF_Dist","P_RF_Dist", "Q2", "W", "epsilon", "ph_q", "ph_recoil", "th_q", "th_recoil", "MandelT", "emiss", "pmiss", "pmx", "pmy", "pmz","Erecoil","emiss_nuc","Mrecoil","raster_x","raster_y","raster_z"]
    
    # Need to create a dict for all the branches we grab                                                
    data = {}
    data.update(COIN_Data)
    data_keys = list(data.keys()) # Create a list of all the keys in all dicts added above, each is an array of data

    if particle == "kaon":
        term_search = "Kaon"
    if particle == "pion":
        term_search = "Pion"        
    
    for i in range (0, len(data_keys)):
        if(term_search in data_keys[i]):
            DFHeader=list(COIN_Data_Header)
        else:
            continue
            # Uncomment the line below if you want .csv file output, WARNING the files can be very large and take a long time to process!                                                                      
            #pd.DataFrame(data.get(data_keys[i])).to_csv("%s/%s_%s.csv" % (OUTPATH, data_keys[i], runNum), header=DFHeader, index=False) # Convert array to panda dataframe and write to csv with correct header                                                                                                      
        if (i == 0):
            pd.DataFrame(data.get(data_keys[i]), columns = DFHeader, index = None).to_root("%s/%s_%s_Raw_Data.root" % (OUTPATH, runNum, MaxEvent), key ="%s" % data_keys[i])
        elif (i != 0):
            pd.DataFrame(data.get(data_keys[i]), columns = DFHeader, index = None).to_root("%s/%s_%s_Raw_Data.root" % (OUTPATH, runNum, MaxEvent), key ="%s" % data_keys[i], mode ='a')

if __name__ == '__main__':
    main()
print ("Processing Complete")
