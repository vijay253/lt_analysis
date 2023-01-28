#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2023-01-21 15:54:18 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#

##################################################################################################################################################

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
import array
from ROOT import TCanvas, TColor, TGaxis, TH1F, TH2F, TPad, TStyle, gStyle, gPad, TGaxis, TLine, TMath, TPaveText, TArc, TGraphPolar 
from ROOT import kBlack, kCyan, kRed, kGreen, kMagenta
from functools import reduce

##################################################################################################################################################
# Check the number of arguments provided to the script

if len(sys.argv)-1!=21:
    print("!!!!! ERROR !!!!!\n Expected 21 arguments\n Usage is with - KIN W Q2 EPSVAL OutDATAFilename OutFullAnalysisFilename tmin tmax NumtBins NumPhiBins runNumRight runNumLeft runNumCenter data_charge_right data_charge_left data_charge_center InData_efficiency_right InData_efficiency_left InData_efficiency_center efficiency_table target\n!!!!! ERROR !!!!!")
    sys.exit(1)

##################################################################################################################################################    

DEBUG = False # Flag for no cut plots

# Input params
kinematics = sys.argv[1].split("_")
W = sys.argv[2]
Q2 = sys.argv[3]
EPSVAL = sys.argv[4]
InDATAFilename = sys.argv[5]
#InDATAFilename = "Analysed_Data_Q0p375W2p2_lowe_Left1.root"
OutFilename = sys.argv[6]
tmin = float(sys.argv[7])
tmax = float(sys.argv[8])
NumtBins = int(sys.argv[9])
NumPhiBins = int(sys.argv[10])
runNumRight = sys.argv[11]
runNumLeft = sys.argv[12]
runNumCenter = sys.argv[13]
data_charge_right = int(sys.argv[14])/1000
data_charge_left = int(sys.argv[15])/1000
data_charge_center = int(sys.argv[16])/1000
InData_efficiency_right = sys.argv[17]
InData_efficiency_left = sys.argv[18]
InData_efficiency_center = sys.argv[19]
efficiency_table = sys.argv[20]
target = sys.argv[21]

particle = "kaon"

###############################################################################################################################################
ROOT.gROOT.SetBatch(ROOT.kTRUE) # Set ROOT to batch mode explicitly, does not splash anything to screen
###############################################################################################################################################

'''
ltsep package import and pathing definitions
'''

# Import package for cuts
from ltsep import Root
# Import package for progress bar
from ltsep import Misc

lt=Root(os.path.realpath(__file__),"Plot_Prod")

# Add this to all files for more dynamic pathing
USER=lt.USER # Grab user info for file finding
HOST=lt.HOST
REPLAYPATH=lt.REPLAYPATH
UTILPATH=lt.UTILPATH
#LTANAPATH=lt.LTANAPATH
LTANAPATH="/u/group/c-kaonlt/USERS/vijay/lt_analysis"
ANATYPE=lt.ANATYPE
OUTPATH=lt.OUTPATH

foutname = OUTPATH+"/" + OutFilename + ".root"
fouttxt  = OUTPATH+"/" + OutFilename + ".txt"
outputpdf  = OUTPATH+"/" + OutFilename + ".pdf"

################################################################################################################################################
'''
Importing diamond cut script
'''

from DiamondPlot_all_v1 import DiamondPlot
print("test")
print(kinematics[0])

################################################################################################################################################

def find_tbins():

    ################################################################################################################################################
    # Define root file trees of interest
    
    H_t_Right = []
    H_t_Left = []
    H_t_Center = []

    for val in ['Lef1', 'Left2', 'Center']:
    #for val in settingList:
        rootFile = OUTPATH+"/"+InDATAFilename+"_%s.root" % val
        if not os.path.isfile(rootFile):
            continue
        else:
            if val == 'Left1':
                InFile_LEFT1_DATA = ROOT.TFile.Open(rootFile, "OPEN")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Uncut_Kaon_Events")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_all_noRF")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_prompt_noRF")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_rand_noRF")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_all_RF")
                TBRANCH_LEFT1_DATA   = InFile_LEFT1_DATA.Get("Cut_Kaon_Events_prompt_noRF")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_prompt_RF")
                #TBRANCH_RIGHT_DATA  = InFile_RIGHT_DATA.Get("Cut_Kaon_Events_rand_RF")
                print("Creating right t-bin histogram...")
                # Grab t bin range
                for i,evt in enumerate(TBRANCH_LEFT1_DATA):
                    # Progress bar
                    Misc.progressBar(i, TBRANCH_LEFT1_DATA.GetEntries())
                    if (tmin <= -evt.MandelT <= tmax):
                        H_t_Right.append(-evt.MandelT)   
                #rbins,H_t_Right = np.histogram(H_t_Right,bins=200)
                
                InFile_LEFT1_DATA.Close()            
                
            if val == 'Left2':
                InFile_LEFT2_DATA = ROOT.TFile.Open(rootFile, "OPEN")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Uncut_Kaon_Events")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_all_noRF")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_prompt_noRF")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_rand_noRF")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_all_RF")
                TBRANCH_LEFT2_DATA   = InFile_LEFT2_DATA.Get("Cut_Kaon_Events_prompt_noRF")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_prompt_RF")
                #TBRANCH_LEFT_DATA  = InFile_LEFT_DATA.Get("Cut_Kaon_Events_rand_RF")
                print("\nCreating left t-bin histogram...")
                # Grab t bin range
                for i,evt in enumerate(TBRANCH_LEFT2_DATA):
                    # Progress bar
                    Misc.progressBar(i, TBRANCH_LEFT2_DATA.GetEntries())
                    if (tmin <= -evt.MandelT <= tmax):
                        H_t_Left.append(-evt.MandelT)
                #lbins,H_t_Left = np.histogram(H_t_Left,bins=200)
                InFile_LEFT2_DATA.Close()
                
            if val == 'Center':
                InFile_CENTER_DATA = ROOT.TFile.Open(rootFile, "OPEN")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Uncut_Kaon_Events")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_all_noRF")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_prompt_noRF")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_rand_noRF")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_all_RF")
                TBRANCH_CENTER_DATA    = InFile_CENTER_DATA.Get("Cut_Kaon_Events_prompt_noRF")
               #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_prompt_RF")
                #TBRANCH_CENTER_DATA  = InFile_CENTER_DATA.Get("Cut_Kaon_Events_rand_RF")
                print("\nCreating center t-bin histogram...")
                # Grab t bin range
                for i,evt in enumerate(TBRANCH_CENTER_DATA):
                    # Progress bar
                    Misc.progressBar(i, TBRANCH_CENTER_DATA.GetEntries())
                    if (tmin <= -evt.MandelT <= tmax):
                        H_t_Center.append(-evt.MandelT)
                #cbins,H_t_Center = np.histogram(H_t_Center,bins=200)
                InFile_CENTER_DATA.Close()        
                
    ################################################################################################################################################

    def histedges_equalN(x, nbin):
        npt = len(x)
        return np.interp(np.linspace(0, npt, nbin + 1),np.arange(npt),np.sort(x))

    H_t_BinTest = []
    for val in settingList:
        if val == "Left1":
            for r in H_t_Right:
                H_t_BinTest.append(r)
        if val == "Left2":
            for l in H_t_Left:
                H_t_BinTest.append(l)
        if val == "Center":
            for c in H_t_Center:
                H_t_BinTest.append(c)

    n, bins, patches = plt.hist(H_t_BinTest, histedges_equalN(H_t_BinTest, NumtBins))

    rn, rbins = np.histogram(H_t_Right, bins=bins)
    ln, lbins = np.histogram(H_t_Left, bins=bins)
    cn, cbins = np.histogram(H_t_Center, bins=bins)

    # Write t_bin_interval for lt_analysis scripts
    lines = []
    with open("{}/src/t_bin_interval_{}_{:.0f}".format(LTANAPATH,Q2.replace("p",""),float(EPSVAL)*100), "w") as file:
        file.write("{}\t{}\t{}\n".format(Q2,NumtBins,NumPhiBins))
        for i,t in enumerate(bins):
            lines.append("\t{:.2f}".format(float(t)))
        file.writelines(lines)
    
    return [n,bins]
    
def defineHists(phi_setting):

    ################################################################################################################################################
    # Call diamond cut script

    paramDict = DiamondPlot(particle,float(Q2.replace("p",".")),float(W.replace("p",".")),phi_setting,tmin,tmax,target)
    
    a1 = paramDict["a1"]
    b1 = paramDict["b1"]
    a2 = paramDict["a2"]
    b2 = paramDict["b2"]
    a3 = paramDict["a3"]
    b3 = paramDict["b3"]
    a4 = paramDict["a4"]
    b4 = paramDict["b4"]

    ################################################################################################################################################
    # Define simc root file trees of interest

    # Names don't match so need to do some string rearrangement
    InSIMCFilename = "Prod_Coin_{}.root".format(kinematics[0]+phi_setting.lower()+"_"+kinematics[1])
    rootFileSimc = OUTPATH+"/"+InSIMCFilename
    if not os.path.isfile(rootFileSimc):
        print("\n\nERROR: No simc file found called {}\n\n".format(rootFileSimc))
        #return {}

    InFile_SIMC = ROOT.TFile.Open(rootFileSimc, "OPEN")

    TBRANCH_SIMC  = InFile_SIMC.Get("h10")

    ###############################################################################################################################################

    # Grabs simc number of events and normalizaton factor
    simc_hist = rootFileSimc.replace('.root','.hist')
    f_simc = open(simc_hist)
    for line in f_simc:
        #print(line)
        if "Ngen" in line:
            val = line.split("=")
            simc_nevents = int(val[1])
        if "normfac" in line:
            val = line.split("=")
            simc_normfactor = float(val[1])
    if 'simc_nevents' and 'simc_normfactor' in locals():
        print('\n\nsimc_nevents = ',simc_nevents,'\nsimc_normfactor = ',simc_normfactor,'\n\n')
    else:
        print("\n\nERROR: Invalid simc hist file %s\n\n" % simc_hist)
        sys.exit(1)
    f_simc.close()    
    
    ################################################################################################################################################
    # Define data root file trees of interest

    rootFileData = OUTPATH+"/"+InDATAFilename+"_%s.root" % (phi_setting)
    if not os.path.isfile(rootFileData):
        print("\n\nERROR: No data file found called {}\n\n".format(rootFileData))
        return {}

    InFile_DATA = ROOT.TFile.Open(rootFileData, "OPEN")

    #TBRANCH_DATA  = InFile_DATA.Get("Uncut_Kaon_Events")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_all_noRF")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_prompt_noRF")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_rand_noRF")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_all_RF")
    TBRANCH_DATA   = InFile_DATA.Get("Cut_Kaon_Events_prompt_noRF")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_prompt_RF")
    #TBRANCH_DATA  = InFile_DATA.Get("Cut_Kaon_Events_rand_RF")
    
    ################################################################################################################################################
    # Grab and calculate efficiency

    
    if phi_setting == "Left1":
        runNums= runNumRight
        runNum = runNumRight.split(' ')[0]
        InData_efficiency = InData_efficiency_right
    if phi_setting == "Left2":
        runNums= runNumLeft
        runNum = runNumLeft.split(' ')[0]
        InData_efficiency = InData_efficiency_left
    if phi_setting == "Center":
        runNums= runNumCenter
        runNum = runNumCenter.split(' ')[0]
        InData_efficiency = InData_efficiency_center

    sys.path.append('../')
    from getDataTable import calculate_effError

    tot_effError_data = [calculate_effError(run,efficiency_table) for run in runNums.split(' ')]
    #print(InData_efficiency)
    #print(tot_effError_data)
    eff_errProp_data = sum(tot_effError_data) # Error propagation for addition

    print("\n\nTotal Data Efficiency Uncertainty =",eff_errProp_data)

    # Define total efficiency vs run number plots
    G_data_eff = ROOT.TGraphErrors(len(InData_efficiency.split(' ')), np.array([float(x) for x in runNums.split(' ')]),np.array([float(x) for x in InData_efficiency.split(' ')]),np.array([0]*len(tot_effError_data)),np.array(tot_effError_data)*np.array([float(x) for x in InData_efficiency.split(' ')]))

    ###############################################################################################################################################
    # Grab windows for random subtraction

    # Section for grabing Prompt/Random selection parameters from PARAM file
    PARAMPATH = "%s/DB/PARAM" % UTILPATH
    print("Running as %s on %s, hallc_replay_lt path assumed as %s" % (USER[1], HOST[1], LTANAPATH))
    TimingCutFile = "%s/Timing_Parameters.csv" % PARAMPATH # This should match the param file actually being used!
    TimingCutf = open(TimingCutFile)
    try:
        TimingCutFile
    except NameError:
        print("!!!!! ERRROR !!!!!\n One (or more) of the cut files not found!\n!!!!! ERRORR !!!!!")
        sys.exit(2)
    print("Reading timing cuts from %s" % TimingCutFile)
    PromptWindow = [0, 0]
    RandomWindows = [0, 0, 0, 0]
    linenum = 0 # Count line number we're on
    TempPar = -1 # To check later
    for line in TimingCutf: # Read all lines in the cut file
        linenum += 1 # Add one to line number at start of loop
        if(linenum > 1): # Skip first line
            line = line.partition('#')[0] # Treat anything after a # as a comment and ignore it
            line = line.rstrip()
            array = line.split(",") # Convert line into an array, anything after a comma is a new entry 
            if(int(runNum) in range (int(array[0]), int(array[1])+1)): # Check if run number for file is within any of the ranges specified in the cut file
                TempPar += 2 # If run number is in range, set to non -1 value
                BunchSpacing = float(array[2])
                CoinOffset = float(array[3]) # Coin offset value
                nSkip = float(array[4]) # Number of random windows skipped 
                nWindows = float(array[5]) # Total number of random windows
                PromptPeak = float(array[6]) # Pion CT prompt peak positon 
    TimingCutf.close() # After scanning all lines in file, close file

    if(TempPar == -1): # If value is still -1, run number provided din't match any ranges specified so exit 
        print("!!!!! ERROR !!!!!\n Run number specified does not fall within a set of runs for which cuts are defined in %s\n!!!!! ERROR !!!!!" % TimingCutFile)
        sys.exit(3)
    elif(TempPar > 1):
        print("!!! WARNING!!! Run number was found within the range of two (or more) line entries of %s !!! WARNING !!!" % TimingCutFile)
        print("The last matching entry will be treated as the input, you should ensure this is what you want")

    # From our values from the file, reconstruct our windows 
    PromptWindow[0] = PromptPeak - (BunchSpacing/2) - CoinOffset
    PromptWindow[1] = PromptPeak + (BunchSpacing/2) + CoinOffset
    RandomWindows[0] = PromptPeak - (BunchSpacing/2) - CoinOffset - (nSkip*BunchSpacing) - ((nWindows/2)*BunchSpacing)
    RandomWindows[1] = PromptPeak - (BunchSpacing/2) - CoinOffset - (nSkip*BunchSpacing)
    RandomWindows[2] = PromptPeak + (BunchSpacing/2) + CoinOffset + (nSkip*BunchSpacing)
    RandomWindows[3] = PromptPeak + (BunchSpacing/2) + CoinOffset + (nSkip*BunchSpacing) + ((nWindows/2)*BunchSpacing)

    ################################################################################################################################################
    # Plot definitions

    H_hsdelta_SIMC  = ROOT.TH1D("H_hsdelta_SIMC","HMS Delta", 200, -20.0, 20.0)
    H_hsxptar_SIMC  = ROOT.TH1D("H_hsxptar_SIMC","HMS xptar", 200, -0.1, 0.1)
    H_hsyptar_SIMC  = ROOT.TH1D("H_hsyptar_SIMC","HMS yptar", 200, -0.1, 0.1)
    H_ssxfp_SIMC    = ROOT.TH1D("H_ssxfp_SIMC","SHMS xfp", 200, -25.0, 25.0)
    H_ssyfp_SIMC    = ROOT.TH1D("H_ssyfp_SIMC","SHMS yfp", 200, -25.0, 25.0)
    H_ssxpfp_SIMC   = ROOT.TH1D("H_ssxpfp_SIMC","SHMS xpfp", 200, -0.09, 0.09)
    H_ssypfp_SIMC   = ROOT.TH1D("H_ssypfp_SIMC","SHMS ypfp", 200, -0.05, 0.04)
    H_hsxfp_SIMC    = ROOT.TH1D("H_hsxfp_SIMC","HMS xfp", 200, -40.0, 40.0)
    H_hsyfp_SIMC    = ROOT.TH1D("H_hsyfp_SIMC","HMS yfp", 200, -20.0, 20.0)
    H_hsxpfp_SIMC   = ROOT.TH1D("H_hsxpfp_SIMC","HMS xpfp", 200, -0.09, 0.05)
    H_hsypfp_SIMC   = ROOT.TH1D("H_hsypfp_SIMC","HMS ypfp", 200, -0.05, 0.04)
    H_ssdelta_SIMC  = ROOT.TH1D("H_ssdelta_SIMC","SHMS delta", 200, -20.0, 20.0)
    H_ssxptar_SIMC  = ROOT.TH1D("H_ssxptar_SIMC","SHMS xptar", 200, -0.1, 0.1)
    H_ssyptar_SIMC  = ROOT.TH1D("H_ssyptar_SIMC","SHMS yptar", 200, -0.04, 0.04)
    H_q_SIMC        = ROOT.TH1D("H_q_SIMC","q", 200, 0.0, 10.0)
    H_Q2_SIMC       = ROOT.TH1D("H_Q2_SIMC","Q2", 200, 0.0, 10.0)
    H_W_SIMC  = ROOT.TH1D("H_W_SIMC","W ", 200, 0.0, 10.0)
    H_t_SIMC       = ROOT.TH1D("H_t_SIMC","-t", 200, -1.0, 1.5)  
    H_epsilon_SIMC  = ROOT.TH1D("H_epsilon_SIMC","epsilon", 200, 0., 1.0)
    H_MM_SIMC  = ROOT.TH1D("H_MM_SIMC","MM_{K}", 200, 0.0, 2.0)
    H_th_SIMC  = ROOT.TH1D("H_th_SIMC","X' tar", 200, -0.1, 0.1)
    H_ph_SIMC  = ROOT.TH1D("H_ph_SIMC","Y' tar", 200, -0.1, 0.1)
    H_ph_q_SIMC  = ROOT.TH1D("H_ph_q_SIMC","Phi Detected (ph_xq)", 200, -10.0, 10.0)
    H_th_q_SIMC  = ROOT.TH1D("H_th_q_SIMC","Theta Detected (th_xq)", 200, -0.2, 0.2)
    H_ph_recoil_SIMC  = ROOT.TH1D("H_ph_recoil_SIMC","Phi Recoil (ph_bq)", 200, -10.0, 10.0)
    H_th_recoil_SIMC  = ROOT.TH1D("H_th_recoil_SIMC","Theta Recoil (th_bq)", 200, -10.0, 10.0)
    H_pmiss_SIMC  = ROOT.TH1D("H_pmiss_SIMC","pmiss", 200, 0.0, 10.0)
    H_emiss_SIMC  = ROOT.TH1D("H_emiss_SIMC","emiss", 200, 0.0, 10.0)
    H_pmx_SIMC  = ROOT.TH1D("H_pmx_SIMC","pmx", 200, -10.0, 10.0)
    H_pmy_SIMC  = ROOT.TH1D("H_pmy_SIMC","pmy ", 200, -10.0, 10.0)
    H_pmz_SIMC  = ROOT.TH1D("H_pmz_SIMC","pmz", 200, -10.0, 10.0)

    H_hsdelta_DATA  = ROOT.TH1D("H_hsdelta_DATA","HMS Delta", 200, -20.0, 20.0)
    H_hsxptar_DATA  = ROOT.TH1D("H_hsxptar_DATA","HMS xptar", 200, -0.1, 0.1)
    H_hsyptar_DATA  = ROOT.TH1D("H_hsyptar_DATA","HMS yptar", 200, -0.1, 0.1)
    H_ssxfp_DATA    = ROOT.TH1D("H_ssxfp_DATA","SHMS xfp", 200, -25.0, 25.0)
    H_ssyfp_DATA    = ROOT.TH1D("H_ssyfp_DATA","SHMS yfp", 200, -25.0, 25.0)
    H_ssxpfp_DATA   = ROOT.TH1D("H_ssxpfp_DATA","SHMS xpfp", 200, -0.09, 0.09)
    H_ssypfp_DATA   = ROOT.TH1D("H_ssypfp_DATA","SHMS ypfp", 200, -0.05, 0.04)
    H_hsxfp_DATA    = ROOT.TH1D("H_hsxfp_DATA","HMS xfp", 200, -40.0, 40.0)
    H_hsyfp_DATA    = ROOT.TH1D("H_hsyfp_DATA","HMS yfp", 200, -20.0, 20.0)
    H_hsxpfp_DATA   = ROOT.TH1D("H_hsxpfp_DATA","HMS xpfp", 200, -0.09, 0.05)
    H_hsypfp_DATA   = ROOT.TH1D("H_hsypfp_DATA","HMS ypfp", 200, -0.05, 0.04)
    H_ssdelta_DATA  = ROOT.TH1D("H_ssdelta_DATA","SHMS delta", 200, -20.0, 20.0)
    H_ssxptar_DATA  = ROOT.TH1D("H_ssxptar_DATA","SHMS xptar", 200, -0.1, 0.1)
    H_ssyptar_DATA  = ROOT.TH1D("H_ssyptar_DATA","SHMS yptar", 200, -0.04, 0.04)
    H_q_DATA        = ROOT.TH1D("H_q_DATA","q", 200, 0.0, 10.0)
    H_Q2_DATA       = ROOT.TH1D("H_Q2_DATA","Q2", 200, 0.0, 10.0)
    H_W_DATA  = ROOT.TH1D("H_W_DATA","W ", 200, 0.0, 10.0)
    H_t_DATA       = ROOT.TH1D("H_t_DATA","-t", 200, -1.0, 1.5)  
    H_epsilon_DATA  = ROOT.TH1D("H_epsilon_DATA","epsilon", 200, 0., 1.0)
    H_MM_DATA  = ROOT.TH1D("H_MM_DATA","MM_{K}", 200, 0.0, 2.0)
    H_th_DATA  = ROOT.TH1D("H_th_DATA","X' tar", 200, -0.1, 0.1)
    H_ph_DATA  = ROOT.TH1D("H_ph_DATA","Y' tar", 200, -0.1, 0.1)
    H_ph_q_DATA  = ROOT.TH1D("H_ph_q_DATA","Phi Detected (ph_xq)", 200, -10.0, 10.0)
    H_th_q_DATA  = ROOT.TH1D("H_th_q_DATA","Theta Detected (th_xq)", 200, -0.2, 0.2)
    H_ph_recoil_DATA  = ROOT.TH1D("H_ph_recoil_DATA","Phi Recoil (ph_bq)", 200, -10.0, 10.0)
    H_th_recoil_DATA  = ROOT.TH1D("H_th_recoil_DATA","Theta Recoil (th_bq)", 200, -10.0, 10.0)
    H_pmiss_DATA  = ROOT.TH1D("H_pmiss_DATA","pmiss", 200, 0.0, 10.0)
    H_emiss_DATA  = ROOT.TH1D("H_emiss_DATA","emiss", 200, 0.0, 10.0)
    H_pmx_DATA  = ROOT.TH1D("H_pmx_DATA","pmx", 200, -10.0, 10.0)
    H_pmy_DATA  = ROOT.TH1D("H_pmy_DATA","pmy ", 200, -10.0, 10.0)
    H_pmz_DATA  = ROOT.TH1D("H_pmz_DATA","pmz", 200, -10.0, 10.0)
    H_ct_ep_DATA = ROOT.TH1D("H_ct_ep_DATA", "Electron-Proton CTime", 200, -10, 10)
    H_cal_etottracknorm_DATA = ROOT.TH1D("H_cal_etottracknorm_DATA", "HMS Cal etottracknorm", 200, 0.2, 1.8)
    H_cer_npeSum_DATA = ROOT.TH1D("H_cer_npeSum_DATA", "HMS Cer Npe Sum", 200, 0, 30)
    P_cal_etottracknorm_DATA = ROOT.TH1D("P_cal_etottracknorm_DATA", "SHMS Cal etottracknorm", 200, 0, 1)
    P_hgcer_npeSum_DATA = ROOT.TH1D("P_hgcer_npeSum_DATA", "SHMS HGCer Npe Sum", 200, 0, 50)
    P_aero_npeSum_DATA = ROOT.TH1D("P_aero_npeSum_DATA", "SHMS Aero Npe Sum", 200, 0, 50)

    H_hsdelta_DATA_rand  = ROOT.TH1D("H_hsdelta_DATA_rand","HMS Delta", 200, -20.0, 20.0)
    H_hsxptar_DATA_rand  = ROOT.TH1D("H_hsxptar_DATA_rand","HMS xptar", 200, -0.1, 0.1)
    H_hsyptar_DATA_rand  = ROOT.TH1D("H_hsyptar_DATA_rand","HMS yptar", 200, -0.1, 0.1)
    H_ssxfp_DATA_rand    = ROOT.TH1D("H_ssxfp_DATA_rand","SHMS xfp", 200, -25.0, 25.0)
    H_ssyfp_DATA_rand    = ROOT.TH1D("H_ssyfp_DATA_rand","SHMS yfp", 200, -25.0, 25.0)
    H_ssxpfp_DATA_rand   = ROOT.TH1D("H_ssxpfp_DATA_rand","SHMS xpfp", 200, -0.09, 0.09)
    H_ssypfp_DATA_rand   = ROOT.TH1D("H_ssypfp_DATA_rand","SHMS ypfp", 200, -0.05, 0.04)
    H_hsxfp_DATA_rand    = ROOT.TH1D("H_hsxfp_DATA_rand","HMS xfp", 200, -40.0, 40.0)
    H_hsyfp_DATA_rand    = ROOT.TH1D("H_hsyfp_DATA_rand","HMS yfp", 200, -20.0, 20.0)
    H_hsxpfp_DATA_rand   = ROOT.TH1D("H_hsxpfp_DATA_rand","HMS xpfp", 200, -0.09, 0.05)
    H_hsypfp_DATA_rand   = ROOT.TH1D("H_hsypfp_DATA_rand","HMS ypfp", 200, -0.05, 0.04)
    H_ssdelta_DATA_rand  = ROOT.TH1D("H_ssdelta_DATA_rand","SHMS delta", 200, -20.0, 20.0)
    H_ssxptar_DATA_rand  = ROOT.TH1D("H_ssxptar_DATA_rand","SHMS xptar", 200, -0.1, 0.1)
    H_ssyptar_DATA_rand  = ROOT.TH1D("H_ssyptar_DATA_rand","SHMS yptar", 200, -0.04, 0.04)
    H_q_DATA_rand        = ROOT.TH1D("H_q_DATA_rand","q", 200, 0.0, 10.0)
    H_Q2_DATA_rand       = ROOT.TH1D("H_Q2_DATA_rand","Q2", 200, 0.0, 10.0)
    H_W_DATA_rand  = ROOT.TH1D("H_W_DATA_rand","W ", 200, 0.0, 10.0)
    H_t_DATA_rand       = ROOT.TH1D("H_t_DATA_rand","-t", 200, -1.0, 1.5)
    H_epsilon_DATA_rand  = ROOT.TH1D("H_epsilon_DATA_rand","epsilon", 200, 0., 1.0)
    H_MM_DATA_rand  = ROOT.TH1D("H_MM_DATA_rand","MM_{K}", 200, 0.0, 2.0)
    H_th_DATA_rand  = ROOT.TH1D("H_th_DATA_rand","X' tar", 200, -0.1, 0.1)
    H_ph_DATA_rand  = ROOT.TH1D("H_ph_DATA_rand","Y' tar", 200, -0.1, 0.1)
    H_ph_q_DATA_rand  = ROOT.TH1D("H_ph_q_DATA_rand","Phi Detected (ph_xq)", 200, -10.0, 10.0)
    H_th_q_DATA_rand  = ROOT.TH1D("H_th_q_DATA_rand","Theta Detected (th_xq)", 200, -0.2, 0.2)
    H_ph_recoil_DATA_rand  = ROOT.TH1D("H_ph_recoil_DATA_rand","Phi Recoil (ph_bq)", 200, -10.0, 10.0)
    H_th_recoil_DATA_rand  = ROOT.TH1D("H_th_recoil_DATA_rand","Theta Recoil (th_bq)", 200, -10.0, 10.0)
    H_pmiss_DATA_rand  = ROOT.TH1D("H_pmiss_DATA_rand","pmiss", 200, 0.0, 10.0)
    H_emiss_DATA_rand  = ROOT.TH1D("H_emiss_DATA_rand","emiss", 200, 0.0, 10.0)
    H_pmx_DATA_rand  = ROOT.TH1D("H_pmx_DATA_rand","pmx", 200, -10.0, 10.0)
    H_pmy_DATA_rand  = ROOT.TH1D("H_pmy_DATA_rand","pmy ", 200, -10.0, 10.0)
    H_pmz_DATA_rand  = ROOT.TH1D("H_pmz_DATA_rand","pmz", 200, -10.0, 10.0)
    H_ct_ep_DATA_rand = ROOT.TH1D("H_ct_ep_DATA_rand", "Electron-Proton CTime", 200, -10, 10)

    ################################################################################################################################################
    # 2D histograms

    MM_vs_CoinTime_DATA = ROOT.TH2D("MM_vs_CoinTime_DATA","Missing Mass vs CTime; MM; Coin_Time",100, 0, 2, 100, -2, 2)
    CoinTime_vs_beta_DATA = ROOT.TH2D("CoinTime_vs_beta_DATA", "CTime vs SHMS #beta; Coin_Time; SHMS_#beta", 100, -2, 2, 100, 0, 2)
    MM_vs_beta_DATA = ROOT.TH2D("MM_vs_beta_DATA", "Missing Mass vs SHMS #beta; MM; SHMS_#beta", 100, 0, 2, 200, 0, 2)
    phiq_vs_t_DATA = ROOT.TH2D("phiq_vs_t_DATA","; #phi ;t", 12, -3.14, 3.14, 24, tmin, tmax)
    Q2_vs_W_DATA = ROOT.TH2D("Q2_vs_W_DATA", "Q^{2} vs W; Q^{2}; W", 200, 0.0, 10.0, 200, 0.0, 10.0)

    ################################################################################################################################################
    # Fill histograms for various trees called above
    
    print("\nPlotting %s simc..." % phi_setting)
    for evt in TBRANCH_SIMC:

      # Define the acceptance cuts  

      # Select the cuts
      SHMS_Acceptance = (evt.ssdelta>=-10.0) & (evt.ssdelta<=20.0) & (evt.ssxptar>=-0.06) & (evt.ssxptar<=0.06) & (evt.ssyptar>=-0.04) & (evt.ssyptar<=0.04)
      HMS_Acceptance = (evt.hsdelta>=-8.0) & (evt.hsdelta<=8.0) & (evt.hsxptar>=-0.08) & (evt.hsxptar<=0.08) & (evt.hsyptar>=-0.045) & (evt.hsyptar<=0.045)
      Diamond = (evt.W/evt.Q2>a1+b1/evt.Q2) & (evt.W/evt.Q2<a2+b2/evt.Q2) & (evt.W/evt.Q2>a3+b3/evt.Q2) & (evt.W/evt.Q2<a4+b4/evt.Q2)
      #........................................

      #Fill SIMC events
      if(HMS_Acceptance & SHMS_Acceptance):

          H_ssxfp_SIMC.Fill(evt.ssxfp, evt.Weight)
          H_ssyfp_SIMC.Fill(evt.ssyfp, evt.Weight)
          H_ssxpfp_SIMC.Fill(evt.ssxpfp, evt.Weight)
          H_ssypfp_SIMC.Fill(evt.ssypfp, evt.Weight)
          H_hsxfp_SIMC.Fill(evt.hsxfp, evt.Weight)
          H_hsyfp_SIMC.Fill(evt.hsyfp, evt.Weight)
          H_hsxpfp_SIMC.Fill(evt.hsxpfp, evt.Weight)
          H_hsypfp_SIMC.Fill(evt.hsypfp, evt.Weight)
          H_ssdelta_SIMC.Fill(evt.ssdelta, evt.Weight) 
          H_hsdelta_SIMC.Fill(evt.hsdelta, evt.Weight)	
          H_ssxptar_SIMC.Fill(evt.ssxptar, evt.Weight)
          H_ssyptar_SIMC.Fill(evt.ssyptar, evt.Weight)
          H_hsxptar_SIMC.Fill(evt.hsxptar, evt.Weight)	
          H_hsyptar_SIMC.Fill(evt.hsyptar, evt.Weight)

          H_ph_q_SIMC.Fill(evt.phipq, evt.Weight)
          H_th_q_SIMC.Fill(evt.thetapq, evt.Weight)

          H_pmiss_SIMC.Fill(evt.Pm, evt.Weight)	
          H_emiss_SIMC.Fill(evt.Em, evt.Weight)	
          #H_pmx_SIMC.Fill(evt.Pmx, evt.Weight)
          #H_pmy_SIMC.Fill(evt.Pmy, evt.Weight)
          #H_pmz_SIMC.Fill(evt.Pmz, evt.Weight)
          H_Q2_SIMC.Fill(evt.Q2, evt.Weight)
          H_W_SIMC.Fill(evt.W, evt.Weight)
          H_t_DATA.Fill(evt.t)
          H_epsilon_SIMC.Fill(evt.epsilon, evt.Weight)
          H_MM_SIMC.Fill(np.sqrt(pow(evt.Em, 2) - pow(evt.Pm, 2)), evt.Weight)
    
    
    ################################################################################################################################################
    # Fill histograms for various trees called above

    
    ibin = 1
    print("\nPlotting %s data..." % phi_setting)
    for i,evt in enumerate(TBRANCH_DATA):

        # Progress bar
        Misc.progressBar(i, TBRANCH_DATA.GetEntries())
        
        #CUTs Definations 
        SHMS_FixCut = (evt.P_hod_goodstarttime == 1) & (evt.P_dc_InsideDipoleExit == 1) # & P_hod_betanotrack > 0.5 & P_hod_betanotrack < 1.4
        SHMS_Acceptance = (evt.ssdelta>=-10.0) & (evt.ssdelta<=20.0) & (evt.ssxptar>=-0.06) & (evt.ssxptar<=0.06) & (evt.ssyptar>=-0.04) & (evt.ssyptar<=0.04)

        HMS_FixCut = (evt.H_hod_goodstarttime == 1) & (evt.H_dc_InsideDipoleExit == 1)
        HMS_Acceptance = (evt.hsdelta>=-8.0) & (evt.hsdelta<=8.0) & (evt.hsxptar>=-0.08) & (evt.hsxptar<=0.08) & (evt.hsyptar>=-0.045) & (evt.hsyptar<=0.045)
        Diamond = (evt.W/evt.Q2>a1+b1/evt.Q2) & (evt.W/evt.Q2<a2+b2/evt.Q2) & (evt.W/evt.Q2>a3+b3/evt.Q2) & (evt.W/evt.Q2<a4+b4/evt.Q2)

        #........................................
                
        if(HMS_Acceptance & SHMS_Acceptance):

            MM_vs_CoinTime_DATA.Fill(evt.MM, evt.CTime_ROC1)
            CoinTime_vs_beta_DATA.Fill(evt.CTime_ROC1,evt.P_gtr_beta)
            MM_vs_beta_DATA.Fill(evt.MM,evt.P_gtr_beta)
            phiq_vs_t_DATA.Fill(evt.ph_q, -evt.MandelT)
            Q2_vs_W_DATA.Fill(evt.Q2, evt.W)
            
            H_ct_ep_DATA.Fill(evt.CTime_ROC1)

            H_ssxfp_DATA.Fill(evt.ssxfp)
            H_ssyfp_DATA.Fill(evt.ssyfp)
            H_ssxpfp_DATA.Fill(evt.ssxpfp)
            H_ssypfp_DATA.Fill(evt.ssypfp)
            H_ssdelta_DATA.Fill(evt.ssdelta)
            H_ssxptar_DATA.Fill(evt.ssxptar)
            H_ssyptar_DATA.Fill(evt.ssyptar)

            H_hsxfp_DATA.Fill(evt.hsxfp)
            H_hsyfp_DATA.Fill(evt.hsyfp)
            H_hsxpfp_DATA.Fill(evt.hsxpfp)
            H_hsypfp_DATA.Fill(evt.hsypfp)
            H_hsdelta_DATA.Fill(evt.hsdelta)
            H_hsxptar_DATA.Fill(evt.hsxptar)	
            H_hsyptar_DATA.Fill(evt.hsyptar)

            H_ph_q_DATA.Fill(evt.ph_q)
            H_th_q_DATA.Fill(evt.th_q)
            H_ph_recoil_DATA.Fill(evt.ph_recoil)
            H_th_recoil_DATA.Fill(evt.th_recoil)

            H_pmiss_DATA.Fill(evt.pmiss)	
            H_emiss_DATA.Fill(evt.emiss)	
            #H_emiss_DATA.Fill(evt.emiss_nuc)
            H_pmx_DATA.Fill(evt.pmx)
            H_pmy_DATA.Fill(evt.pmy)
            H_pmz_DATA.Fill(evt.pmz)
            H_Q2_DATA.Fill(evt.Q2)
            H_t_DATA.Fill(-evt.MandelT)
            H_W_DATA.Fill(evt.W)
            H_epsilon_DATA.Fill(evt.epsilon)
            H_MM_DATA.Fill(np.sqrt(pow(evt.emiss, 2) - pow(evt.pmiss, 2)))
            #H_MM_DATA.Fill(pow(evt.MM, 2))  
            #H_MM_DATA.Fill(evt.Mrecoil)
            
            ###################################################################################################################################################
            
            # Random subtraction
            H_ssxfp_DATA_rand.Fill(evt.ssxfp)
            H_ssyfp_DATA_rand.Fill(evt.ssyfp)
            H_ssxpfp_DATA_rand.Fill(evt.ssxpfp)
            H_ssypfp_DATA_rand.Fill(evt.ssypfp)
            H_ssdelta_DATA_rand.Fill(evt.ssdelta)
            H_ssxptar_DATA_rand.Fill(evt.ssxptar)
            H_ssyptar_DATA_rand.Fill(evt.ssyptar)

            H_hsxfp_DATA_rand.Fill(evt.hsxfp)
            H_hsyfp_DATA_rand.Fill(evt.hsyfp)
            H_hsxpfp_DATA_rand.Fill(evt.hsxpfp)
            H_hsypfp_DATA_rand.Fill(evt.hsypfp)
            H_hsdelta_DATA_rand.Fill(evt.hsdelta)
            H_hsxptar_DATA_rand.Fill(evt.hsxptar)	
            H_hsyptar_DATA_rand.Fill(evt.hsyptar)
            
            H_pmiss_DATA_rand.Fill(evt.pmiss)	
            H_emiss_DATA_rand.Fill(evt.emiss)	
            #H_emiss_DATA_rand.Fill(evt.emiss_nuc)
            H_pmx_DATA_rand.Fill(evt.pmx)
            H_pmy_DATA_rand.Fill(evt.pmy)
            H_pmz_DATA_rand.Fill(evt.pmz)
            H_Q2_DATA_rand.Fill(evt.Q2)
            H_t_DATA_rand.Fill(-evt.MandelT)
            H_W_DATA_rand.Fill(evt.W)
            H_epsilon_DATA_rand.Fill(evt.epsilon)
            H_MM_DATA_rand.Fill(np.sqrt(pow(evt.emiss, 2) - pow(evt.pmiss, 2))  )
            
            H_cal_etottracknorm_DATA.Fill(evt.H_cal_etottracknorm)
            H_cer_npeSum_DATA.Fill(evt.H_cer_npeSum)
            
            P_cal_etottracknorm_DATA.Fill(evt.P_cal_etottracknorm)
            P_hgcer_npeSum_DATA.Fill(evt.P_hgcer_npeSum)
            P_aero_npeSum_DATA.Fill(evt.P_aero_npeSum)
            
    ibin+=1
          
    ################################################################################################################################################
    # Normalize simc by normfactor/nevents
    # Normalize data by effective charge

    normfac_simc = (simc_normfactor)/(simc_nevents)
    H_ssxfp_SIMC.Scale(normfac_simc)
    H_ssyfp_SIMC.Scale(normfac_simc)
    H_ssxpfp_SIMC.Scale(normfac_simc)
    H_ssypfp_SIMC.Scale(normfac_simc)
    H_hsxfp_SIMC.Scale(normfac_simc)
    H_hsyfp_SIMC.Scale(normfac_simc)
    H_hsxpfp_SIMC.Scale(normfac_simc)
    H_hsypfp_SIMC.Scale(normfac_simc)
    H_ssxptar_SIMC.Scale(normfac_simc)
    H_ssyptar_SIMC.Scale(normfac_simc)
    H_hsxptar_SIMC.Scale(normfac_simc)
    H_hsyptar_SIMC.Scale(normfac_simc)
    H_ssdelta_SIMC.Scale(normfac_simc)
    H_hsdelta_SIMC.Scale(normfac_simc)
    H_Q2_SIMC.Scale(normfac_simc)
    H_t_SIMC.Scale(normfac_simc)
    H_epsilon_SIMC.Scale(normfac_simc)
    H_MM_SIMC.Scale(normfac_simc)
    H_ph_q_SIMC.Scale(normfac_simc)
    H_th_q_SIMC.Scale(normfac_simc)
    H_ph_recoil_SIMC.Scale(normfac_simc)
    H_th_recoil_SIMC.Scale(normfac_simc)
    H_pmiss_SIMC.Scale(normfac_simc)
    H_emiss_SIMC.Scale(normfac_simc)
    #H_pmx_SIMC.Scale(normfac_simc)
    #H_pmy_SIMC.Scale(normfac_simc)
    #H_pmz_SIMC.Scale(normfac_simc)
    H_W_SIMC.Scale(normfac_simc)
          
    if phi_setting == "Left1":
        normfac_data = 1/(data_charge_right)
    if phi_setting == "Left2":
        normfac_data = 1/(data_charge_left)
    if phi_setting == "Center":
        normfac_data = 1/(data_charge_center)        
    H_ssxfp_DATA.Scale(normfac_data)
    H_ssyfp_DATA.Scale(normfac_data)
    H_ssxpfp_DATA.Scale(normfac_data)
    H_ssypfp_DATA.Scale(normfac_data)
    H_hsxfp_DATA.Scale(normfac_data)
    H_hsyfp_DATA.Scale(normfac_data)
    H_hsxpfp_DATA.Scale(normfac_data)
    H_hsypfp_DATA.Scale(normfac_data)
    H_ssxptar_DATA.Scale(normfac_data)
    H_ssyptar_DATA.Scale(normfac_data)
    H_hsxptar_DATA.Scale(normfac_data)
    H_hsyptar_DATA.Scale(normfac_data)
    H_ssdelta_DATA.Scale(normfac_data)
    H_hsdelta_DATA.Scale(normfac_data)
    H_Q2_DATA.Scale(normfac_data)
    H_t_DATA.Scale(normfac_data)
    H_epsilon_DATA.Scale(normfac_data)
    H_MM_DATA.Scale(normfac_data)
    H_ph_q_DATA.Scale(normfac_data)
    H_th_q_DATA.Scale(normfac_data)
    H_ph_recoil_DATA.Scale(normfac_data)
    H_th_recoil_DATA.Scale(normfac_data)
    H_pmiss_DATA.Scale(normfac_data)
    H_emiss_DATA.Scale(normfac_data)
    H_pmx_DATA.Scale(normfac_data)
    H_pmy_DATA.Scale(normfac_data)
    H_pmz_DATA.Scale(normfac_data)
    H_W_DATA.Scale(normfac_data)
    H_ct_ep_DATA.Scale(normfac_data)

    # Data Random subtraction
    H_ssxfp_DATA_rand.Scale(normfac_data/nWindows)
    H_ssyfp_DATA_rand.Scale(normfac_data/nWindows)
    H_ssxpfp_DATA_rand.Scale(normfac_data/nWindows)
    H_ssypfp_DATA_rand.Scale(normfac_data/nWindows)
    H_hsxfp_DATA_rand.Scale(normfac_data/nWindows)
    H_hsyfp_DATA_rand.Scale(normfac_data/nWindows)
    H_hsxpfp_DATA_rand.Scale(normfac_data/nWindows)
    H_hsypfp_DATA_rand.Scale(normfac_data/nWindows)
    H_ssxptar_DATA_rand.Scale(normfac_data/nWindows)
    H_ssyptar_DATA_rand.Scale(normfac_data/nWindows)
    H_hsxptar_DATA_rand.Scale(normfac_data/nWindows)
    H_hsyptar_DATA_rand.Scale(normfac_data/nWindows)
    H_ssdelta_DATA_rand.Scale(normfac_data/nWindows)
    H_hsdelta_DATA_rand.Scale(normfac_data/nWindows)
    H_Q2_DATA_rand.Scale(normfac_data/nWindows)
    H_t_DATA_rand.Scale(normfac_data/nWindows)
    H_epsilon_DATA_rand.Scale(normfac_data/nWindows)
    H_MM_DATA_rand.Scale(normfac_data/nWindows)
    H_pmiss_DATA_rand.Scale(normfac_data/nWindows)
    H_emiss_DATA_rand.Scale(normfac_data/nWindows)
    H_pmx_DATA_rand.Scale(normfac_data/nWindows)
    H_pmy_DATA_rand.Scale(normfac_data/nWindows)
    H_pmz_DATA_rand.Scale(normfac_data/nWindows)
    H_W_DATA_rand.Scale(normfac_data/nWindows)
    #H_ct_ep_DATA_rand.Scale(normfac_data/nWindows)
    
    ###
    # Data Random subtraction
    H_ssxfp_DATA.Add(H_ssxfp_DATA_rand,-1)
    H_ssyfp_DATA.Add(H_ssyfp_DATA_rand,-1)
    H_ssxpfp_DATA.Add(H_ssxpfp_DATA_rand,-1)
    H_ssypfp_DATA.Add(H_ssypfp_DATA_rand,-1)
    H_hsxfp_DATA.Add(H_hsxfp_DATA_rand,-1)
    H_hsyfp_DATA.Add(H_hsyfp_DATA_rand,-1)
    H_hsxpfp_DATA.Add(H_hsxpfp_DATA_rand,-1)
    H_hsypfp_DATA.Add(H_hsypfp_DATA_rand,-1)
    H_ssxptar_DATA.Add(H_ssxptar_DATA_rand,-1)
    H_ssyptar_DATA.Add(H_ssyptar_DATA_rand,-1)
    H_hsxptar_DATA.Add(H_hsxptar_DATA_rand,-1)
    H_hsyptar_DATA.Add(H_hsyptar_DATA_rand,-1)
    H_ssdelta_DATA.Add(H_ssdelta_DATA_rand,-1)
    H_hsdelta_DATA.Add(H_hsdelta_DATA_rand,-1)
    H_Q2_DATA.Add(H_Q2_DATA_rand,-1)
    H_t_DATA.Add(H_t_DATA_rand,-1)
    H_epsilon_DATA.Add(H_epsilon_DATA_rand,-1)
    H_MM_DATA.Add(H_MM_DATA_rand,-1)
    H_pmiss_DATA.Add(H_pmiss_DATA_rand,-1)
    H_emiss_DATA.Add(H_emiss_DATA_rand,-1)
    H_pmx_DATA.Add(H_pmx_DATA_rand,-1)
    H_pmy_DATA.Add(H_pmy_DATA_rand,-1)
    H_pmz_DATA.Add(H_pmz_DATA_rand,-1)
    H_W_DATA.Add(H_W_DATA_rand,-1)
    H_ct_ep_DATA.Add(H_ct_ep_DATA_rand,-1)

    histDict = {
        "phi_setting" : phi_setting,
        "runNums" : runNums.split(' '),
        "InData_efficiency" : InData_efficiency.split(' '),
        "G_data_eff" : G_data_eff,
        "H_hsdelta_DATA" :     H_hsdelta_DATA,
        "H_hsxptar_DATA" :     H_hsxptar_DATA,
        "H_hsyptar_DATA" :     H_hsyptar_DATA,
        "H_ssxfp_DATA" :     H_ssxfp_DATA  ,
        "H_ssyfp_DATA" :     H_ssyfp_DATA  ,
        "H_ssxpfp_DATA" :     H_ssxpfp_DATA ,
        "H_ssypfp_DATA" :     H_ssypfp_DATA ,
        "H_hsxfp_DATA" :     H_hsxfp_DATA  ,
        "H_hsyfp_DATA" :     H_hsyfp_DATA  ,
        "H_hsxpfp_DATA" :     H_hsxpfp_DATA ,
        "H_hsypfp_DATA" :     H_hsypfp_DATA ,
        "H_ssdelta_DATA" :     H_ssdelta_DATA,
        "H_ssxptar_DATA" :     H_ssxptar_DATA,
        "H_ssyptar_DATA" :     H_ssyptar_DATA,
        "H_q_DATA" :     H_q_DATA      ,
        "H_Q2_DATA" :     H_Q2_DATA     ,
        "H_t_DATA" :     H_t_DATA     ,
        "H_epsilon_DATA" :     H_epsilon_DATA,
        "H_MM_DATA" :     H_MM_DATA,
        "H_th_DATA" :     H_th_DATA,
        "H_ph_DATA" :     H_ph_DATA,
        "H_ph_q_DATA" :     H_ph_q_DATA,
        "H_th_q_DATA" :     H_th_q_DATA,
        "H_ph_recoil_DATA" :     H_ph_recoil_DATA,
        "H_th_recoil_DATA" :     H_th_recoil_DATA,
        "H_pmiss_DATA" :     H_pmiss_DATA,
        "H_emiss_DATA" :     H_emiss_DATA,
        "H_pmx_DATA" :     H_pmx_DATA,
        "H_pmy_DATA" :     H_pmy_DATA,
        "H_pmz_DATA" :     H_pmz_DATA,
        "H_W_DATA" :     H_W_DATA,
        "H_hsdelta_SIMC" :     H_hsdelta_SIMC,
        "H_hsxptar_SIMC" :     H_hsxptar_SIMC,
        "H_hsyptar_SIMC" :     H_hsyptar_SIMC,
        "H_ssxfp_SIMC" :     H_ssxfp_SIMC  ,
        "H_ssyfp_SIMC" :     H_ssyfp_SIMC  ,
        "H_ssxpfp_SIMC" :     H_ssxpfp_SIMC ,
        "H_ssypfp_SIMC" :     H_ssypfp_SIMC ,
        "H_hsxfp_SIMC" :     H_hsxfp_SIMC  ,
        "H_hsyfp_SIMC" :     H_hsyfp_SIMC  ,
        "H_hsxpfp_SIMC" :     H_hsxpfp_SIMC ,
        "H_hsypfp_SIMC" :     H_hsypfp_SIMC ,
        "H_ssdelta_SIMC" :     H_ssdelta_SIMC,
        "H_ssxptar_SIMC" :     H_ssxptar_SIMC,
        "H_ssyptar_SIMC" :     H_ssyptar_SIMC,
        "H_q_SIMC" :     H_q_SIMC      ,
        "H_Q2_SIMC" :     H_Q2_SIMC     ,
        "H_t_SIMC" :     H_t_SIMC     ,
        "H_epsilon_SIMC" :     H_epsilon_SIMC,
        "H_MM_SIMC" :     H_MM_SIMC,
        "H_th_SIMC" :     H_th_SIMC,
        "H_ph_SIMC" :     H_ph_SIMC,
        "H_ph_q_SIMC" :     H_ph_q_SIMC,
        "H_th_q_SIMC" :     H_th_q_SIMC,
        "H_ph_recoil_SIMC" :     H_ph_recoil_SIMC,
        "H_th_recoil_SIMC" :     H_th_recoil_SIMC,
        "H_pmiss_SIMC" :     H_pmiss_SIMC,
        "H_emiss_SIMC" :     H_emiss_SIMC,
        "H_pmx_SIMC" :     H_pmx_SIMC,
        "H_pmy_SIMC" :     H_pmy_SIMC,
        "H_pmz_SIMC" :     H_pmz_SIMC,
        "H_W_SIMC" :     H_W_SIMC,        
        "H_ct_ep_DATA" :     H_ct_ep_DATA,
        "H_cal_etottracknorm_DATA" :     H_cal_etottracknorm_DATA,
        "H_cer_npeSum_DATA" :     H_cer_npeSum_DATA,
        "P_cal_etottracknorm_DATA" :     P_cal_etottracknorm_DATA,
        "P_hgcer_npeSum_DATA" :     P_hgcer_npeSum_DATA,
        "P_aero_npeSum_DATA" :     P_aero_npeSum_DATA,
        "MM_vs_CoinTime_DATA" : MM_vs_CoinTime_DATA,
        "CoinTime_vs_beta_DATA" : CoinTime_vs_beta_DATA,
        "MM_vs_beta_DATA" : MM_vs_beta_DATA,
        "phiq_vs_t_DATA" : phiq_vs_t_DATA,
        "Q2_vs_W_DATA" : Q2_vs_W_DATA,
        "InFile_DATA" : InFile_DATA,
    }

    return histDict

################################################################################################################################################
# Removes stat box
ROOT.gStyle.SetOptStat(0)
################################################################################################################################################

# Call histogram function above to define dictonaries for right, left, center settings
# Put these all into an array so that if we are missing a setting it is easier to remove
# Plus it makes the code below less repetitive
phisetlist = ["Left1","Left2","Center"]
histlist = []
for phiset in phisetlist:
    histlist.append(defineHists(phiset))

print("\n\n")

settingList = []
for i,hist in enumerate(histlist):
    if not bool(hist): # If hist is empty
        histlist.remove(hist)
    else:
        settingList.append(hist["phi_setting"])
        
eff_plt = TCanvas()
G_eff_plt = ROOT.TMultiGraph()
l_eff_plt = ROOT.TLegend(0.115,0.35,0.33,0.5)

eff_plt.SetGrid()

for i,hist in enumerate(histlist):
    hist["G_data_eff"].SetMarkerStyle(21)
    hist["G_data_eff"].SetMarkerSize(1)
    hist["G_data_eff"].SetMarkerColor(i+1)
    G_eff_plt.Add(hist["G_data_eff"])

G_eff_plt.Draw("AP")

G_eff_plt.SetTitle(" ;Run Numbers; Total Efficiency")

i=0
for i,hist in enumerate(histlist):
    while i <= G_eff_plt.GetXaxis().GetXmax():
        bin_ix = G_eff_plt.GetXaxis().FindBin(i)
        if str(i) in hist["runNums"]: 
            G_eff_plt.GetXaxis().SetBinLabel(bin_ix,"%d" % i)
        i+=1

G_eff_plt.GetYaxis().SetTitleOffset(1.5)
G_eff_plt.GetXaxis().SetTitleOffset(1.5)
G_eff_plt.GetXaxis().SetLabelSize(0.04)

for i,hist in enumerate(histlist):
    l_eff_plt.AddEntry(hist["G_data_eff"],hist["phi_setting"])

l_eff_plt.Draw()

eff_plt.Print(outputpdf + '(')
        
# Plot histograms
c_pid = TCanvas()

c_pid.Divide(2,3)

c_pid.cd(1)
gPad.SetLogy()

for i,hist in enumerate(histlist):
    hist["H_cal_etottracknorm_DATA"].SetLineColor(i+1)
    hist["H_cal_etottracknorm_DATA"].Draw("same, E1")

c_pid.cd(2)
gPad.SetLogy()

for i,hist in enumerate(histlist):
    hist["H_cer_npeSum_DATA"].SetLineColor(i+1)
    hist["H_cer_npeSum_DATA"].Draw("same, E1")

c_pid.cd(3)
gPad.SetLogy()
for i,hist in enumerate(histlist):
    hist["P_cal_etottracknorm_DATA"].SetLineColor(i+1)
    hist["P_cal_etottracknorm_DATA"].Draw("same, E1")

c_pid.cd(4)
gPad.SetLogy()
for i,hist in enumerate(histlist):
    hist["P_hgcer_npeSum_DATA"].SetLineColor(i+1)
    hist["P_hgcer_npeSum_DATA"].Draw("same, E1")

c_pid.cd(5)
gPad.SetLogy()
for i,hist in enumerate(histlist):
    hist["P_aero_npeSum_DATA"].SetLineColor(i+1)
    hist["P_aero_npeSum_DATA"].Draw("same, E1")
        
c_pid.Draw()

c_pid.Print(outputpdf)

ct_ep = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ct_ep_DATA"].SetLineColor(i+1)
    hist["H_ct_ep_DATA"].Draw("same, E1")

ct_ep.Print(outputpdf)


CQ2 = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_Q2_DATA"].SetLineColor(i+1)
    hist["H_Q2_DATA"].Draw("same, E1")
    hist["H_Q2_SIMC"].SetLineColor(40)
    hist["H_Q2_SIMC"].SetLineStyle(10-i)
    hist["H_Q2_SIMC"].Draw("same, E1")    

CQ2.Print(outputpdf)

CW = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_W_DATA"].SetLineColor(i+1)
    hist["H_W_DATA"].Draw("same, E1")
    hist["H_W_SIMC"].SetLineColor(40)
    hist["H_W_SIMC"].SetLineStyle(10-i)
    hist["H_W_SIMC"].Draw("same, E1")        
    
CW.Print(outputpdf)

Ct = TCanvas()
l_t = ROOT.TLegend(0.115,0.45,0.33,0.95)
l_t.SetTextSize(0.0235)

binned_t = find_tbins()
binmax = []
for i,hist in enumerate(histlist):
    hist["H_t_DATA"].SetLineColor(i+1)
    l_t.AddEntry(hist["H_t_DATA"],hist["phi_setting"])
    hist["H_t_DATA"].Draw("same, E1")
    hist["H_t_SIMC"].SetLineColor(40)
    hist["H_t_SIMC"].SetLineStyle(10-i)
    hist["H_t_SIMC"].Draw("same, E1")
    binmax.append(hist["H_t_DATA"].GetMaximum())
binmax = max(binmax)
    
tBin_line = TLine()
# binned_t[0] is missing a value for the final bin
# so adding the first element allows the zip to include all bins
# this is okay because the number of events per bin should be the same
binned_t0 = list(binned_t[0])
binned_t0.append(binned_t[0][0])
for i,(n,b) in enumerate(zip(binned_t0,binned_t[1])):
    tBin_line.SetLineColor(4)
    tBin_line.SetLineWidth(4)
    tBin_line.DrawLine(b,0,b,binmax)
    l_t.AddEntry(tBin_line,"Bin Edge %s" % i )
    l_t.AddEntry(tBin_line,"Evts = %.0f" % n)
    l_t.AddEntry(tBin_line,"BinCenter = %.2f" % b)

l_t.Draw()    

Ct.Print(outputpdf)

Cepsilon = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_epsilon_DATA"].SetLineColor(i+1)
    hist["H_epsilon_DATA"].Draw("same, E1")
    hist["H_epsilon_SIMC"].SetLineColor(40)
    hist["H_epsilon_SIMC"].SetLineStyle(10-i)
    hist["H_epsilon_SIMC"].Draw("same, E1")
    
Cepsilon.Print(outputpdf)

CMM = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_MM_DATA"].SetLineColor(i+1)
    hist["H_MM_DATA"].Draw("same, E1")
    hist["H_MM_SIMC"].SetLineColor(40)
    hist["H_MM_SIMC"].SetLineStyle(10-i)
    hist["H_MM_SIMC"].Draw("same, E1")
    
CMM.Print(outputpdf)

xfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssxfp_DATA"].SetLineColor(i+1)
    hist["H_ssxfp_DATA"].Draw("same, E1")
    hist["H_ssxfp_SIMC"].SetLineColor(40)
    hist["H_ssxfp_SIMC"].SetLineStyle(10-i)
    hist["H_ssxfp_SIMC"].Draw("same, E1")
    
xfp.Print(outputpdf)

yfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssyfp_DATA"].SetLineColor(i+1)
    hist["H_ssyfp_DATA"].Draw("same, E1")
    hist["H_ssyfp_SIMC"].SetLineColor(40)
    hist["H_ssyfp_SIMC"].SetLineStyle(10-i)
    hist["H_ssyfp_SIMC"].Draw("same, E1")
    
yfp.Print(outputpdf)

xpfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssxpfp_DATA"].SetLineColor(i+1)
    hist["H_ssxpfp_DATA"].Draw("same, E1")
    hist["H_ssxpfp_SIMC"].SetLineColor(40)
    hist["H_ssxpfp_SIMC"].SetLineStyle(10-i)
    hist["H_ssxpfp_SIMC"].Draw("same, E1")
    
xpfp.Print(outputpdf)

ypfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssxpfp_DATA"].SetLineColor(i+1)
    hist["H_ssxpfp_DATA"].Draw("same, E1")
    hist["H_ssxpfp_SIMC"].SetLineColor(40)
    hist["H_ssxpfp_SIMC"].SetLineStyle(10-i)
    hist["H_ssxpfp_SIMC"].Draw("same, E1")
    
ypfp.Print(outputpdf)

hxfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsxfp_DATA"].SetLineColor(i+1)
    hist["H_hsxfp_DATA"].Draw("same, E1")
    hist["H_hsxfp_SIMC"].SetLineColor(40)
    hist["H_hsxfp_SIMC"].SetLineStyle(10-i)
    hist["H_hsxfp_SIMC"].Draw("same, E1")
    
hxfp.Print(outputpdf)

hyfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsyfp_DATA"].SetLineColor(i+1)
    hist["H_hsyfp_DATA"].Draw("same, E1")
    hist["H_hsyfp_SIMC"].SetLineColor(40)
    hist["H_hsyfp_SIMC"].SetLineStyle(10-i)
    hist["H_hsyfp_SIMC"].Draw("same, E1")
    
hyfp.Print(outputpdf)

hxpfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsxpfp_DATA"].SetLineColor(i+1)
    hist["H_hsxpfp_DATA"].Draw("same, E1")
    hist["H_hsxpfp_SIMC"].SetLineColor(40)
    hist["H_hsxpfp_SIMC"].SetLineStyle(10-i)
    hist["H_hsxpfp_SIMC"].Draw("same, E1")
    
hxpfp.Print(outputpdf)

hypfp = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsypfp_DATA"].SetLineColor(i+1)
    hist["H_hsypfp_DATA"].Draw("same, E1")
    hist["H_hsypfp_SIMC"].SetLineColor(40)
    hist["H_hsypfp_SIMC"].SetLineStyle(10-i)
    hist["H_hsypfp_SIMC"].Draw("same, E1")
    
hypfp.Print(outputpdf)

xptar = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssxptar_DATA"].SetLineColor(i+1)
    hist["H_ssxptar_DATA"].Draw("same, E1")
    hist["H_ssxptar_SIMC"].SetLineColor(40)
    hist["H_ssxptar_SIMC"].SetLineStyle(10-i)
    hist["H_ssxptar_SIMC"].Draw("same, E1")
    
xptar.Print(outputpdf)

yptar = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssyptar_DATA"].SetLineColor(i+1)
    hist["H_ssyptar_DATA"].Draw("same, E1")
    hist["H_ssyptar_SIMC"].SetLineColor(40)
    hist["H_ssyptar_SIMC"].SetLineStyle(10-i)
    hist["H_ssyptar_SIMC"].Draw("same, E1")
    
yptar.Print(outputpdf)

hxptar = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsxptar_DATA"].SetLineColor(i+1)
    hist["H_hsxptar_DATA"].Draw("same, E1")
    hist["H_hsxptar_SIMC"].SetLineColor(40)
    hist["H_hsxptar_SIMC"].SetLineStyle(10-i)
    hist["H_hsxptar_SIMC"].Draw("same, E1")
    
hxptar.Print(outputpdf)

hyptar = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsyptar_DATA"].SetLineColor(i+1)
    hist["H_hsyptar_DATA"].Draw("same, E1")
    hist["H_hsyptar_SIMC"].SetLineColor(40)
    hist["H_hsyptar_SIMC"].SetLineStyle(10-i)
    hist["H_hsyptar_SIMC"].Draw("same, E1")
    
hyptar.Print(outputpdf)

Delta = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ssdelta_DATA"].SetLineColor(i+1)
    hist["H_ssdelta_DATA"].Draw("same, E1")
    hist["H_ssdelta_SIMC"].SetLineColor(40)
    hist["H_ssdelta_SIMC"].SetLineStyle(10-i)
    hist["H_ssdelta_SIMC"].Draw("same, E1")
    
Delta.Print(outputpdf)

hDelta = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_hsdelta_DATA"].SetLineColor(i+1)
    hist["H_hsdelta_DATA"].Draw("same, E1")
    hist["H_hsdelta_SIMC"].SetLineColor(40)
    hist["H_hsdelta_SIMC"].SetLineStyle(10-i)
    hist["H_hsdelta_SIMC"].Draw("same, E1")
    
hDelta.Print(outputpdf)

Cph_q = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ph_q_DATA"].SetLineColor(i+1)
    hist["H_ph_q_DATA"].Draw("same, E1")
    hist["H_ph_q_SIMC"].SetLineColor(40)
    hist["H_ph_q_SIMC"].SetLineStyle(10-i)
    hist["H_ph_q_SIMC"].Draw("same, E1")
    
Cph_q.Print(outputpdf)

Cth_q = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_th_q_DATA"].SetLineColor(i+1)
    hist["H_th_q_DATA"].Draw("same, E1")
    hist["H_th_q_SIMC"].SetLineColor(40)
    hist["H_th_q_SIMC"].SetLineStyle(10-i)
    hist["H_th_q_SIMC"].Draw("same, E1")
    
Cth_q.Print(outputpdf)

Cph_recoil = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_ph_recoil_DATA"].SetLineColor(i+1)
    hist["H_ph_recoil_DATA"].Draw("same, E1")
    
Cph_recoil.Print(outputpdf)

Cth_recoil = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_th_recoil_DATA"].SetLineColor(i+1)
    hist["H_th_recoil_DATA"].Draw("same, E1")

Cth_recoil.Print(outputpdf)

Cpmiss = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_pmiss_DATA"].SetLineColor(i+1)
    hist["H_pmiss_DATA"].Draw("same, E1")
    hist["H_pmiss_SIMC"].SetLineColor(40)
    hist["H_pmiss_SIMC"].SetLineStyle(10-i)
    hist["H_pmiss_SIMC"].Draw("same, E1")
    
Cpmiss.Print(outputpdf)

Cemiss = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_emiss_DATA"].SetLineColor(i+1)
    hist["H_emiss_DATA"].Draw("same, E1")
    hist["H_emiss_SIMC"].SetLineColor(40)
    hist["H_emiss_SIMC"].SetLineStyle(10-i)
    hist["H_emiss_SIMC"].Draw("same, E1")
    
Cemiss.Print(outputpdf)

Cpmiss_x = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_pmx_DATA"].SetLineColor(i+1)
    hist["H_pmx_DATA"].Draw("same, E1")
    #hist["H_pmx_SIMC"].SetLineColor(40)
    #hist["H_pmx_SIMC"].SetLineStyle(10-i)
    #hist["H_pmx_SIMC"].Draw("same, E1")
    
Cpmiss_x.Print(outputpdf)

Cpmiss_y = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_pmy_DATA"].SetLineColor(i+1)
    hist["H_pmy_DATA"].Draw("same, E1")
    #hist["H_pmy_SIMC"].SetLineColor(40)
    #hist["H_pmy_SIMC"].SetLineStyle(10-i)
    #hist["H_pmy_SIMC"].Draw("same, E1")
    
Cpmiss_y.Print(outputpdf)

Cpmiss_z = TCanvas()

for i,hist in enumerate(histlist):
    hist["H_pmz_DATA"].SetLineColor(i+1)
    hist["H_pmz_DATA"].Draw("same, E1")
    #hist["H_pmz_SIMC"].SetLineColor(40)
    #hist["H_pmz_SIMC"].SetLineStyle(10-i)
    #hist["H_pmz_SIMC"].Draw("same, E1")
    
Cpmiss_z.Print(outputpdf)

Cmmct = TCanvas()

Cmmct.Divide(2,2)

for i,hist in enumerate(histlist):
    Cmmct.cd(i+1)
    hist["MM_vs_CoinTime_DATA"].SetLineColor(i+1)
    hist["MM_vs_CoinTime_DATA"].Draw("same, COLZ")
    hist["MM_vs_CoinTime_DATA"].SetTitle(phisetlist[i])

Cmmct.Print(outputpdf)

Cctbeta = TCanvas()

Cctbeta.Divide(2,2)

for i,hist in enumerate(histlist):
    Cctbeta.cd(i+1)
    hist["CoinTime_vs_beta_DATA"].SetLineColor(i+1)
    hist["CoinTime_vs_beta_DATA"].Draw("same, COLZ")
    hist["CoinTime_vs_beta_DATA"].SetTitle(phisetlist[i])

Cctbeta.Print(outputpdf)

Cmmbeta = TCanvas()

Cmmbeta.Divide(2,2)

for i,hist in enumerate(histlist):
    Cmmbeta.cd(i+1)
    hist["MM_vs_beta_DATA"].SetLineColor(i+1)
    hist["MM_vs_beta_DATA"].Draw("same, COLZ")
    hist["MM_vs_beta_DATA"].SetTitle(phisetlist[i])

Cmmbeta.Print(outputpdf)

Cqw = TCanvas()

Cqw.Divide(2,2)

for i,hist in enumerate(histlist):
    Cqw.cd(i+1)
    hist["Q2_vs_W_DATA"].SetLineColor(i+1)
    hist["Q2_vs_W_DATA"].Draw("same, COLZ")
    hist["Q2_vs_W_DATA"].SetTitle(phisetlist[i])

Cqw.Print(outputpdf)

Cpht = TCanvas()

for i,hist in enumerate(histlist):
    hist["phiq_vs_t_DATA"].GetYaxis().SetRangeUser(tmin,tmax)
    hist["phiq_vs_t_DATA"].Draw("same, SURF2 POL")
    
# Section for polar plotting
gStyle.SetPalette(55)
gPad.SetTheta(90)
gPad.SetPhi(180)
tvsphi_title = TPaveText(0.0277092,0.89779,0.096428,0.991854,"NDC")
tvsphi_title.AddText("-t vs #phi")
tvsphi_title.Draw()
ptphizero = TPaveText(0.923951,0.513932,0.993778,0.574551,"NDC")
ptphizero.AddText("#phi = 0")
ptphizero.Draw()
phihalfk = TLine(0,0,0,0.6)
phihalfk.SetLineColor(kBlack)
phihalfk.SetLineWidth(2)
phihalfk.Draw()
ptphihalfk = TPaveText(0.417855,0.901876,0.486574,0.996358,"NDC")
ptphihalfk.AddText("#phi = #frac{K}{2}")
ptphihalfk.Draw()
phik = TLine(0,0,-0.6,0)
phik.SetLineColor(kBlack)
phik.SetLineWidth(2)
phik.Draw()
ptphik = TPaveText(0.0277092,0.514217,0.096428,0.572746,"NDC")
ptphik.AddText("#phi = K")
ptphik.Draw()
phithreek = TLine(0,0,0,-0.6)
phithreek.SetLineColor(kBlack)
phithreek.SetLineWidth(2)
phithreek.Draw()
ptphithreek = TPaveText(0.419517,0.00514928,0.487128,0.0996315,"NDC")
ptphithreek.AddText("#phi = #frac{3K}{2}")
ptphithreek.Draw()
Arc = TArc()
for k in range(0, 10):
     Arc.SetFillStyle(0)
     Arc.SetLineWidth(2)
     # To change the arc radius we have to change number 0.6 in the lower line.
     Arc.DrawArc(0,0,0.6*(k+1)/(10),0.,360.,"same")
for i,(n,b) in enumerate(zip(binned_t0,binned_t[1])):
     Arc.SetLineColor(3)
     Arc.SetLineWidth(2)
     # To change the arc radius we have to change number 0.6 in the lower line.
     Arc.DrawArc(0,0,0.6*b,0.,360.,"same")
tradius = TGaxis(0,0,0.6,0,tmin,tmax,10,"-+")
tradius.SetLineColor(2)
tradius.SetLabelColor(2)
tradius.Draw()

Cpht.Print(outputpdf+')')


#############################################################################################################################################
# Create new root file with trees representing cut simc and data used above. Good for those who see python as...problematic

outHistFile = ROOT.TFile.Open(foutname, "RECREATE")
d_Right_Data = outHistFile.mkdir("Right")
d_Left_Data = outHistFile.mkdir("Left")
d_Center_Data = outHistFile.mkdir("Center")

d_Right_Data.cd()
for i,hist in enumerate(histlist):
    if bool(hist):
        continue
    if hist["phi_setting"] == "Left1":
        d_Right_Data.cd()
    if hist["phi_setting"] == "Left2":
        d_Left_Data.cd()
    if hist["phi_setting"] == "Center":
        d_Center_Data.cd()
    hist["H_hsdelta_DATA"].Write()
    hist["H_hsxptar_DATA"].Write()
    hist["H_hsyptar_DATA"].Write()
    hist["H_ssxfp_DATA"].Write()
    hist["H_ssyfp_DATA"].Write()
    hist["H_ssxpfp_DATA"].Write()
    hist["H_ssypfp_DATA"].Write()
    hist["H_hsxfp_DATA"].Write()
    hist["H_hsyfp_DATA"].Write()
    hist["H_hsxpfp_DATA"].Write()
    hist["H_hsypfp_DATA"].Write()
    hist["H_ssdelta_DATA"].Write()
    hist["H_ssxptar_DATA"].Write()
    hist["H_ssyptar_DATA"].Write()
    hist["H_q_DATA"].Write()
    hist["H_Q2_DATA"].Write()
    hist["H_W_DATA"].Write()
    hist["H_t_DATA"].Write()
    hist["H_epsilon_DATA"].Write()
    hist["H_MM_DATA"].Write()
    hist["H_th_DATA"].Write()
    hist["H_ph_DATA"].Write()
    hist["H_ph_q_DATA"].Write()
    hist["H_th_q_DATA"].Write()
    hist["H_ph_recoil_DATA"].Write()
    hist["H_th_recoil_DATA"].Write()
    hist["H_pmiss_DATA"].Write()
    hist["H_emiss_DATA"].Write()
    hist["H_pmx_DATA"].Write()
    hist["H_pmy_DATA"].Write()
    hist["H_pmz_DATA"].Write()
    hist["H_ct_ep_DATA"].Write()

outHistFile.Close()

for i,hist in enumerate(histlist):
    hist["InFile_DATA"].Close()
    
print ("Processing Complete")

