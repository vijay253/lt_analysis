// .... Created Date: Feb 01, 2023 ....
// .... Author: VK ....
// .... This script was prepared to analyse the summer 2019 data....
// .... I really like to have everything in a single script rather than spreading things all over place....
// .... This script updated on Dec 29, 2023 for the final analysis....
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
#define EFFICIENCY_cxx
#include <TStyle.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TMath.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <TSystem.h>
#include <TTree.h>
#include <TArc.h>
#include <TCutG.h>
#include <TExec.h>
#include <TColor.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TPad.h>
#include "Q2AnalysisHistDef.h"
#include "CircleDrawer.h"
#include "NumBinArray.h"
#include <cmath>

void Q2Analysis_V2()
{
  TString Hostname = gSystem->HostName();
  TString User = (gSystem->GetUserInfo())->fUser;
  TString Replaypath;
  TString ROOTfilePath;
  TString OutPath;
  TString rootFile_DATA;

  gStyle->SetPalette(55);
  gStyle->SetOptStat(222);

  // Set paths depending on system you're running on
  if(Hostname.Contains("farm")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    /*
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/KaonLT/";
    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/KaonLT/";
    */
    //Changed to cache on Feb 29, 2024
    /*
    ROOTfilePath = "/lustre19/expphy/cache/hallc/kaonlt/vijay/Analysis/Q1";
    OutPath =  "/lustre19/expphy/cache/hallc/kaonlt/vijay/Analysis/Q1";
    */  
    //Changed to cache on Sep 12, 2024
    ROOTfilePath = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2";
    OutPath =  "/u/home/vijay/LTsepscripts/Analysis/Q2/yield";
  }
  else if(Hostname.Contains("qcd")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    OutPath = Replaypath+"/UTIL_KAONLT/scripts/CoinTimePeak/OUTPUT";
  }
  else if (Hostname.Contains("phys.uregina.ca")){
    ROOTfilePath = "/home/vijay/work/HeepCoinStudy/";
    OutPath = "/home/vijay/work/HeepCoinStudy/";
  }

  //Dummy extra thickness correction factor.
  Double_t THCF = 1.0/4.8579;
  Double_t TimmingOffset = 44.1;
  //SIMC Acceptance Cuts
  //SHMS
  Double_t SIMC_Delta_Low_P   = -10;
  Double_t SIMC_Delta_High_P  =  20;
  Double_t SIMC_Xptar_Low_P   = -0.06;
  Double_t SIMC_Xptar_High_P  =  0.06;
  Double_t SIMC_Yptar_Low_P   = -0.04;
  Double_t SIMC_Yptar_High_P  =  0.04;
  //HMS
  Double_t SIMC_Delta_Low_H   = -8;
  Double_t SIMC_Delta_High_H  =  8;
  Double_t SIMC_Xptar_Low_H   = -0.08;
  Double_t SIMC_Xptar_High_H  =  0.08;
  Double_t SIMC_Yptar_Low_H   = -0.045;
  Double_t SIMC_Yptar_High_H  =  0.045;

  
  // Low epsilon analysis
  cout<<" "<<endl;
  cout<<" Analysing low epsilon data..." <<endl;
  cout<<" "<<endl;
  //Data  
  TString TInDATAFilenameC = "Analysed_Data_Q0p425W2p2_lowe_Center.root";
  TString TInDATAFilenameL1 = "Analysed_Data_Q0p425W2p2_lowe_Left1.root";
  TString TInDATAFilenameL2 = "Analysed_Data_Q0p425W2p2_lowe_Left2.root";
 
  TString rootFile_DATAC  = ROOTfilePath+"/"+TInDATAFilenameC;
  TString rootFile_DATAL1 = ROOTfilePath+"/"+TInDATAFilenameL1;
  TString rootFile_DATAL2 = ROOTfilePath+"/"+TInDATAFilenameL2;
  //Dummy
  TString TInDATAFilenamedC = "Analysed_Data_Q0p425W2p2_lowe_dummy_Center.root";
  TString TInDATAFilenamedL1 = "Analysed_Data_Q0p425W2p2_lowe_dummy_Left1.root";
  TString TInDATAFilenamedL2 = "Analysed_Data_Q0p425W2p2_lowe_dummy_Left2.root";
 
  TString rootFile_DATADC  = ROOTfilePath+"/"+TInDATAFilenamedC;
  TString rootFile_DATADL1 = ROOTfilePath+"/"+TInDATAFilenamedL1;
  TString rootFile_DATADL2 = ROOTfilePath+"/"+TInDATAFilenamedL2;

  //SIMC
  TString TInSIMCFilenameC  = "Pion_2p7_Q2_center.root";
  TString TInSIMCFilenameL1 = "Pion_2p7_Q2_left1.root";
  TString TInSIMCFilenameL2 = "Pion_2p7_Q2_left2.root";

  TString rootFile_SIMCC   = ROOTfilePath+"/"+TInSIMCFilenameC;
  TString rootFile_SIMCL1  = ROOTfilePath+"/"+TInSIMCFilenameL1;
  TString rootFile_SIMCL2  = ROOTfilePath+"/"+TInSIMCFilenameL2;


  if (gSystem->AccessPathName(rootFile_DATAC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAL1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAL1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAL2) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAL2 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  if (gSystem->AccessPathName(rootFile_SIMCC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_SIMCC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  //Data
  TFile *InFile_DATAC = new TFile(rootFile_DATAC, "READ");
  TFile *InFile_DATAL1 = new TFile(rootFile_DATAL1, "READ");
  TFile *InFile_DATAL2 = new TFile(rootFile_DATAL2, "READ");
  //Dummy
  TFile *InFile_DATADC = new TFile(rootFile_DATADC, "READ");
  TFile *InFile_DATADL1 = new TFile(rootFile_DATADL1, "READ");
  TFile *InFile_DATADL2 = new TFile(rootFile_DATADL2, "READ");

  //SIMC
  TFile *InFile_SIMCC = new TFile(rootFile_SIMCC, "READ");
  TFile *InFile_SIMCL1 = new TFile(rootFile_SIMCL1, "READ");
  TFile *InFile_SIMCL2 = new TFile(rootFile_SIMCL2, "READ");

  //Output file names 
  TString foutname   = OutPath+"/" + "Plots_Qp425W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Plots_Qp425W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Plots_Qp425W2p2" + ".pdf";

  //Data
  TTree* TBRANCHC  = (TTree*)InFile_DATAC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHC  = (Long64_t)TBRANCHC->GetEntries();  
  TTree* TBRANCHL1 = (TTree*)InFile_DATAL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL1  = (Long64_t)TBRANCHL1->GetEntries();  
  TTree* TBRANCHL2 = (TTree*)InFile_DATAL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL2  = (Long64_t)TBRANCHL2->GetEntries();  
  //Dummy
  TTree* TBRANCHDC  = (TTree*)InFile_DATADC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHDC  = (Long64_t)TBRANCHDC->GetEntries();  
  TTree* TBRANCHDL1 = (TTree*)InFile_DATADL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHDL1  = (Long64_t)TBRANCHDL1->GetEntries();  
  TTree* TBRANCHDL2 = (TTree*)InFile_DATADL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHDL2  = (Long64_t)TBRANCHDL2->GetEntries();  

  //SIMC  
  TTree* TSIMCC  = (TTree*)InFile_SIMCC->Get("h10");Long64_t nEntries_TSIMCC  = (Long64_t)TSIMCC->GetEntries();  
  TTree* TSIMCL1  = (TTree*)InFile_SIMCL1->Get("h10");Long64_t nEntries_TSIMCL1  = (Long64_t)TSIMCL1->GetEntries();  
  TTree* TSIMCL2  = (TTree*)InFile_SIMCL2->Get("h10");Long64_t nEntries_TSIMCL2  = (Long64_t)TSIMCL2->GetEntries();  

  //DATA VARIABLES
  Double_t  P_gtr_dp;TBRANCHC->SetBranchAddress("ssdelta", &P_gtr_dp);
  Double_t  P_gtr_xptar;TBRANCHC->SetBranchAddress("ssxptar", &P_gtr_xptar);
  Double_t  P_gtr_yptar;TBRANCHC->SetBranchAddress("ssyptar", &P_gtr_yptar);

  Double_t  H_gtr_dp;TBRANCHC->SetBranchAddress("hsdelta", &H_gtr_dp);
  Double_t  H_gtr_xptar;TBRANCHC->SetBranchAddress("hsxptar", &H_gtr_xptar);
  Double_t  H_gtr_yptar;TBRANCHC->SetBranchAddress("hsyptar", &H_gtr_yptar);

  //Dummy
  Double_t  P_gtr_dpd;TBRANCHDC->SetBranchAddress("ssdelta", &P_gtr_dpd);
  Double_t  P_gtr_xptard;TBRANCHDC->SetBranchAddress("ssxptar", &P_gtr_xptard);
  Double_t  P_gtr_yptard;TBRANCHDC->SetBranchAddress("ssyptar", &P_gtr_yptard);

  Double_t  H_gtr_dpd;TBRANCHDC->SetBranchAddress("hsdelta", &H_gtr_dpd);
  Double_t  H_gtr_xptard;TBRANCHDC->SetBranchAddress("hsxptar", &H_gtr_xptard);
  Double_t  H_gtr_yptard;TBRANCHDC->SetBranchAddress("hsyptar", &H_gtr_yptard);

  Double_t WC;TBRANCHC->SetBranchAddress("W", &WC);
  Double_t Q2C;TBRANCHC->SetBranchAddress("Q2", &Q2C);
  Double_t epsilonC;TBRANCHC->SetBranchAddress("epsilon", &epsilonC);
  //dummy
  Double_t WdC;TBRANCHDC->SetBranchAddress("W", &WdC);
  Double_t Q2dC;TBRANCHDC->SetBranchAddress("Q2", &Q2dC);
  Double_t epsilondC;TBRANCHDC->SetBranchAddress("epsilon", &epsilondC);
  
  Double_t  P_gtr_dpL1;TBRANCHL1->SetBranchAddress("ssdelta", &P_gtr_dpL1);
  Double_t  P_gtr_xptarL1;TBRANCHL1->SetBranchAddress("ssxptar", &P_gtr_xptarL1);
  Double_t  P_gtr_yptarL1;TBRANCHL1->SetBranchAddress("ssyptar", &P_gtr_yptarL1);

  Double_t  H_gtr_dpL1;TBRANCHL1->SetBranchAddress("hsdelta", &H_gtr_dpL1);
  Double_t  H_gtr_xptarL1;TBRANCHL1->SetBranchAddress("hsxptar", &H_gtr_xptarL1);
  Double_t  H_gtr_yptarL1;TBRANCHL1->SetBranchAddress("hsyptar", &H_gtr_yptarL1);

  Double_t WL1;TBRANCHL1->SetBranchAddress("W", &WL1);
  Double_t Q2L1;TBRANCHL1->SetBranchAddress("Q2", &Q2L1);
  //dummy
  Double_t WdL1;TBRANCHDL1->SetBranchAddress("W", &WdL1);
  Double_t Q2dL1;TBRANCHDL1->SetBranchAddress("Q2", &Q2dL1);
 
  Double_t  P_gtr_dpL2;TBRANCHL2->SetBranchAddress("ssdelta", &P_gtr_dpL2);
  Double_t  P_gtr_xptarL2;TBRANCHL2->SetBranchAddress("ssxptar", &P_gtr_xptarL2);
  Double_t  P_gtr_yptarL2;TBRANCHL2->SetBranchAddress("ssyptar", &P_gtr_yptarL2);

  Double_t  H_gtr_dpL2;TBRANCHL2->SetBranchAddress("hsdelta", &H_gtr_dpL2);
  Double_t  H_gtr_xptarL2;TBRANCHL2->SetBranchAddress("hsxptar", &H_gtr_xptarL2);
  Double_t  H_gtr_yptarL2;TBRANCHL2->SetBranchAddress("hsyptar", &H_gtr_yptarL2);

  Double_t WL2;TBRANCHL2->SetBranchAddress("W", &WL2);
  Double_t Q2L2;TBRANCHL2->SetBranchAddress("Q2", &Q2L2);
  //dummy
  Double_t WdL2;TBRANCHDL2->SetBranchAddress("W", &WdL2);
  Double_t Q2dL2;TBRANCHDL2->SetBranchAddress("Q2", &Q2dL2);
  
  Double_t tC;TBRANCHC->SetBranchAddress("MandelT", &tC);
  //dummy
  Double_t tdC;TBRANCHDC->SetBranchAddress("MandelT", &tdC);
  Double_t tL1;TBRANCHL1->SetBranchAddress("MandelT", &tL1);
  //dummy
  Double_t tdL1;TBRANCHDL1->SetBranchAddress("MandelT", &tdL1);
  Double_t tL2;TBRANCHL2->SetBranchAddress("MandelT", &tL2);
  //dummy
  Double_t tdL2;TBRANCHDL2->SetBranchAddress("MandelT", &tdL2);

  Double_t ph_qC;TBRANCHC->SetBranchAddress("ph_q", &ph_qC);
  //dummy
  Double_t ph_qdC;TBRANCHDC->SetBranchAddress("ph_q", &ph_qdC);
  Double_t ph_qL1;TBRANCHL1->SetBranchAddress("ph_q", &ph_qL1);
  //dummy
  Double_t ph_qdL1;TBRANCHDL1->SetBranchAddress("ph_q", &ph_qdL1);
  Double_t ph_qL2;TBRANCHL2->SetBranchAddress("ph_q", &ph_qL2);
  //dummy
  Double_t ph_qdL2;TBRANCHDL2->SetBranchAddress("ph_q", &ph_qdL2);

  Double_t th_qC;TBRANCHC->SetBranchAddress("th_q", &th_qC);
  Double_t th_qL1;TBRANCHL1->SetBranchAddress("th_q", &th_qL1);
  Double_t th_qL2;TBRANCHL2->SetBranchAddress("th_q", &th_qL2);

  Double_t mmC;TBRANCHC->SetBranchAddress("MM", &mmC);
  //dummy
  Double_t mmdC;TBRANCHDC->SetBranchAddress("MM", &mmdC);
  Double_t mmL1;TBRANCHL1->SetBranchAddress("MM", &mmL1);
  //dummy
  Double_t mmdL1;TBRANCHDL1->SetBranchAddress("MM", &mmdL1);
  Double_t mmL2;TBRANCHL2->SetBranchAddress("MM", &mmL2);
  //dummy
  Double_t mmdL2;TBRANCHDL2->SetBranchAddress("MM", &mmdL2);

  Double_t tcoinC;TBRANCHC->SetBranchAddress("CTime_ROC1", &tcoinC);
  //dummy
  Double_t tcoindC;TBRANCHDC->SetBranchAddress("CTime_ROC1", &tcoindC);
  Double_t tcoinL1;TBRANCHL1->SetBranchAddress("CTime_ROC1", &tcoinL1);
  //dummy
  Double_t tcoindL1;TBRANCHDL1->SetBranchAddress("CTime_ROC1", &tcoindL1);
  Double_t tcoinL2;TBRANCHL2->SetBranchAddress("CTime_ROC1", &tcoinL2);
  //dummy
  Double_t tcoindL2;TBRANCHDL2->SetBranchAddress("CTime_ROC1", &tcoindL2);

  //SIMC VARIABLES

  //CENTER
  //SHMS SIMC variables                                                                                                                                                                                
  Float_t ssdelta;TSIMCC->SetBranchAddress("ssdelta", &ssdelta);
  Float_t ssxptar;TSIMCC->SetBranchAddress("ssxptar", &ssxptar);
  Float_t ssyptar;TSIMCC->SetBranchAddress("ssyptar", &ssyptar);
  Float_t ssxfp;TSIMCC->SetBranchAddress("ssxfp", &ssxfp);
  Float_t ssyfp;TSIMCC->SetBranchAddress("ssyfp", &ssyfp);
  Float_t ssxpfp;TSIMCC->SetBranchAddress("ssxpfp", &ssxpfp);
  Float_t ssypfp;TSIMCC->SetBranchAddress("ssypfp", &ssypfp);

  //HMS SIMC variables                                                                                                                                                                                
  Float_t hsdelta;TSIMCC->SetBranchAddress("hsdelta", &hsdelta);
  Float_t hsxptar;TSIMCC->SetBranchAddress("hsxptar", &hsxptar);
  Float_t hsyptar;TSIMCC->SetBranchAddress("hsyptar", &hsyptar);
  Float_t hsxfp;TSIMCC->SetBranchAddress("hsxfp", &hsxfp);
  Float_t hsyfp;TSIMCC->SetBranchAddress("hsyfp", &hsyfp);
  Float_t hsxpfp;TSIMCC->SetBranchAddress("hsxpfp", &hsxpfp);
  Float_t hsypfp;TSIMCC->SetBranchAddress("hsypfp", &hsypfp);

  Float_t q;TSIMCC->SetBranchAddress("q", &q);
  Float_t Q2_simc;TSIMCC->SetBranchAddress("Q2", &Q2_simc);
  Float_t W_simc;TSIMCC->SetBranchAddress("W", &W_simc);
  Float_t t_simc;TSIMCC->SetBranchAddress("t", &t_simc);
  Float_t ti_simc;TSIMCC->SetBranchAddress("ti", &ti_simc);
  Float_t epsilon_simc;TSIMCC->SetBranchAddress("epsilon", &epsilon_simc);
  Float_t missmass;TSIMCC->SetBranchAddress("missmass", &missmass);
  Float_t Em;TSIMCC->SetBranchAddress("Em", &Em);
  Float_t Pm;TSIMCC->SetBranchAddress("Pm", &Pm);
  Float_t Weight;TSIMCC->SetBranchAddress("Weight", &Weight);
  Float_t phipq;TSIMCC->SetBranchAddress("phipq", &phipq);
  Float_t thetapq;TSIMCC->SetBranchAddress("thetapq", &thetapq);

  //LEFT1
  //SHMS SIMC variables                                                                                                                                                                                
  Float_t ssdeltaL1;TSIMCL1->SetBranchAddress("ssdelta", &ssdeltaL1);
  Float_t ssxptarL1;TSIMCL1->SetBranchAddress("ssxptar", &ssxptarL1);
  Float_t ssyptarL1;TSIMCL1->SetBranchAddress("ssyptar", &ssyptarL1);
  Float_t ssxfpL1;TSIMCL1->SetBranchAddress("ssxfp", &ssxfpL1);
  Float_t ssyfpL1;TSIMCL1->SetBranchAddress("ssyfp", &ssyfpL1);
  Float_t ssxpfpL1;TSIMCL1->SetBranchAddress("ssxpfp", &ssxpfpL1);
  Float_t ssypfpL1;TSIMCL1->SetBranchAddress("ssypfp", &ssypfpL1);

  //HMS SIMC variables                                                                                                                                                                                
  Float_t hsdeltaL1;TSIMCL1->SetBranchAddress("hsdelta", &hsdeltaL1);
  Float_t hsxptarL1;TSIMCL1->SetBranchAddress("hsxptar", &hsxptarL1);
  Float_t hsyptarL1;TSIMCL1->SetBranchAddress("hsyptar", &hsyptarL1);
  Float_t hsxfpL1;TSIMCL1->SetBranchAddress("hsxfp", &hsxfpL1);
  Float_t hsyfpL1;TSIMCL1->SetBranchAddress("hsyfp", &hsyfpL1);
  Float_t hsxpfpL1;TSIMCL1->SetBranchAddress("hsxpfp", &hsxpfpL1);
  Float_t hsypfpL1;TSIMCL1->SetBranchAddress("hsypfp", &hsypfpL1);

  Float_t qL1;TSIMCL1->SetBranchAddress("q", &qL1);
  Float_t Q2_simcL1;TSIMCL1->SetBranchAddress("Q2", &Q2_simcL1);
  Float_t W_simcL1;TSIMCL1->SetBranchAddress("W", &W_simcL1);
  Float_t t_simcL1;TSIMCL1->SetBranchAddress("t", &t_simcL1);
  Float_t ti_simcL1;TSIMCL1->SetBranchAddress("ti", &ti_simcL1);
  Float_t epsilon_simcL1;TSIMCL1->SetBranchAddress("epsilon", &epsilon_simcL1);
  Float_t missmassL1;TSIMCL1->SetBranchAddress("missmass", &missmassL1);
  Float_t EmL1;TSIMCL1->SetBranchAddress("Em", &EmL1);
  Float_t PmL1;TSIMCL1->SetBranchAddress("Pm", &PmL1);
  Float_t WeightL1;TSIMCL1->SetBranchAddress("Weight", &WeightL1);
  Float_t phipqL1;TSIMCL1->SetBranchAddress("phipq", &phipqL1);
  Float_t thetapqL1;TSIMCL1->SetBranchAddress("thetapq", &thetapqL1);

  //LEFT2
  //SHMS SIMC variables                                                                                                                                                                                
  Float_t ssdeltaL2;TSIMCL2->SetBranchAddress("ssdelta", &ssdeltaL2);
  Float_t ssxptarL2;TSIMCL2->SetBranchAddress("ssxptar", &ssxptarL2);
  Float_t ssyptarL2;TSIMCL2->SetBranchAddress("ssyptar", &ssyptarL2);
  Float_t ssxfpL2;TSIMCL2->SetBranchAddress("ssxfp", &ssxfpL2);
  Float_t ssyfpL2;TSIMCL2->SetBranchAddress("ssyfp", &ssyfpL2);
  Float_t ssxpfpL2;TSIMCL2->SetBranchAddress("ssxpfp", &ssxpfpL2);
  Float_t ssypfpL2;TSIMCL2->SetBranchAddress("ssypfp", &ssypfpL2);

  //HMS SIMC variables                                                                                                                                                                                
  Float_t hsdeltaL2;TSIMCL2->SetBranchAddress("hsdelta", &hsdeltaL2);
  Float_t hsxptarL2;TSIMCL2->SetBranchAddress("hsxptar", &hsxptarL2);
  Float_t hsyptarL2;TSIMCL2->SetBranchAddress("hsyptar", &hsyptarL2);
  Float_t hsxfpL2;TSIMCL2->SetBranchAddress("hsxfp", &hsxfpL2);
  Float_t hsyfpL2;TSIMCL2->SetBranchAddress("hsyfp", &hsyfpL2);
  Float_t hsxpfpL2;TSIMCL2->SetBranchAddress("hsxpfp", &hsxpfpL2);
  Float_t hsypfpL2;TSIMCL2->SetBranchAddress("hsypfp", &hsypfpL2);

  Float_t qL2;TSIMCL2->SetBranchAddress("q", &qL2);
  Float_t Q2_simcL2;TSIMCL2->SetBranchAddress("Q2", &Q2_simcL2);
  Float_t W_simcL2;TSIMCL2->SetBranchAddress("W", &W_simcL2);
  Float_t t_simcL2;TSIMCL2->SetBranchAddress("t", &t_simcL2);
  Float_t ti_simcL2;TSIMCL2->SetBranchAddress("ti", &ti_simcL2);
  Float_t epsilon_simcL2;TSIMCL2->SetBranchAddress("epsilon", &epsilon_simcL2);
  Float_t missmassL2;TSIMCL2->SetBranchAddress("missmass", &missmassL2);
  Float_t EmL2;TSIMCL2->SetBranchAddress("Em", &EmL2);
  Float_t PmL2;TSIMCL2->SetBranchAddress("Pm", &PmL2);
  Float_t WeightL2;TSIMCL2->SetBranchAddress("Weight", &WeightL2);
  Float_t phipqL2;TSIMCL2->SetBranchAddress("phipq", &phipqL2);
  Float_t thetapqL2;TSIMCL2->SetBranchAddress("thetapq", &thetapqL2);

 //*************************
  //plot added on Apr 18, 2024
  //MMpi 
  TH1D *MPiC   = new TH1D("MPiC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiL1   = new TH1D("MPiL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiL2   = new TH1D("MPiL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPiCR   = new TH1D("MPiCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiL1R   = new TH1D("MPiL1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiL2R   = new TH1D("MPiL2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Dummy
  TH1D *MPidC   = new TH1D("MPidC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPidL1   = new TH1D("MPidL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPidL2   = new TH1D("MPidL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPidCR   = new TH1D("MPidCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPidL1R   = new TH1D("MPidL1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPidL2R   = new TH1D("MPidL2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //SIMC
  TH1D *MPiSC   = new TH1D("MPiSC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSL1   = new TH1D("MPiSL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSL2   = new TH1D("MPiSL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      

  //t
  TH1D *hthtC   = new TH1D("hthtC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtL1   = new TH1D("hthtL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtL2   = new TH1D("hthtL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtCR   = new TH1D("hthtCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtL1R   = new TH1D("hthtL1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtL2R   = new TH1D("hthtL2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Dummy
  TH1D *hthtdC   = new TH1D("hthtdC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtdL1   = new TH1D("hthtdL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtdL2   = new TH1D("hthtdL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtdCR   = new TH1D("hthtdCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtdL1R   = new TH1D("hthtdL1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtdL2R   = new TH1D("hthtdL2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //SIMC
  TH1D *hthtSC   = new TH1D("hthtSC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSL1   = new TH1D("hthtSL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSL2   = new TH1D("hthtSL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      

 //*************************

  //DATA HISTOGRAMS

  TH2D *hQ2WC  = new TH2D("hQ2WC","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH2D *hQ2WCR  = new TH2D("hQ2WCR","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1R  = new TH2D("hQ2WC1R","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  //SIMC
  TH2D *hQ2WSC  = new TH2D("hQ2WSC","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WSC1  = new TH2D("hQ2WSC1","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  
  //W

  TH1D *hW1C   = new TH1D("hW1C","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2C   = new TH1D("hW2C","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3C   = new TH1D("hW3C","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4C   = new TH1D("hW4C","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5C   = new TH1D("hW5C","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6C   = new TH1D("hW6C","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7C   = new TH1D("hW7C","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8C   = new TH1D("hW8C","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1L1   = new TH1D("hW1L1","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2L1   = new TH1D("hW2L1","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3L1   = new TH1D("hW3L1","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4L1   = new TH1D("hW4L1","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5L1   = new TH1D("hW5L1","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6L1   = new TH1D("hW6L1","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7L1   = new TH1D("hW7L1","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8L1   = new TH1D("hW8L1","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1L2   = new TH1D("hW1L2","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2L2   = new TH1D("hW2L2","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3L2   = new TH1D("hW3L2","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4L2   = new TH1D("hW4L2","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5L2   = new TH1D("hW5L2","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6L2   = new TH1D("hW6L2","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7L2   = new TH1D("hW7L2","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8L2   = new TH1D("hW8L2","W 8 t L2; W;", 300, 2.1, 2.3);

  //W Random

  TH1D *hW1CR   = new TH1D("hW1CR","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2CR   = new TH1D("hW2CR","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3CR   = new TH1D("hW3CR","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4CR   = new TH1D("hW4CR","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5CR   = new TH1D("hW5CR","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6CR   = new TH1D("hW6CR","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7CR   = new TH1D("hW7CR","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8CR   = new TH1D("hW8CR","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1L1R   = new TH1D("hW1L1R","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2L1R   = new TH1D("hW2L1R","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3L1R   = new TH1D("hW3L1R","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4L1R   = new TH1D("hW4L1R","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5L1R   = new TH1D("hW5L1R","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6L1R   = new TH1D("hW6L1R","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7L1R   = new TH1D("hW7L1R","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8L1R   = new TH1D("hW8L1R","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1L2R   = new TH1D("hW1L2R","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2L2R   = new TH1D("hW2L2R","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3L2R   = new TH1D("hW3L2R","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4L2R   = new TH1D("hW4L2R","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5L2R   = new TH1D("hW5L2R","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6L2R   = new TH1D("hW6L2R","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7L2R   = new TH1D("hW7L2R","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8L2R   = new TH1D("hW8L2R","W 8 t L2; W;", 300, 2.1, 2.3);

  //Q2

  TH1D *hQ21C   = new TH1D("hQ21C","Q2 1 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22C   = new TH1D("hQ22C","Q2 2 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23C   = new TH1D("hQ23C","Q2 3 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24C   = new TH1D("hQ24C","Q2 4 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25C   = new TH1D("hQ25C","Q2 5 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26C   = new TH1D("hQ26C","Q2 6 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27C   = new TH1D("hQ27C","Q2 7 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28C   = new TH1D("hQ28C","Q2 8 t C; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21L1  = new TH1D("hQ21L1","Q2 1 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22L1  = new TH1D("hQ22L1","Q2 2 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23L1  = new TH1D("hQ23L1","Q2 3 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24L1  = new TH1D("hQ24L1","Q2 4 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25L1  = new TH1D("hQ25L1","Q2 5 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26L1  = new TH1D("hQ26L1","Q2 6 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27L1  = new TH1D("hQ27L1","Q2 7 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28L1  = new TH1D("hQ28L1","Q2 8 t L1 Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21L2  = new TH1D("hQ21L2","Q2 1 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22L2  = new TH1D("hQ22L2","Q2 2 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23L2  = new TH1D("hQ23L2","Q2 3 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24L2  = new TH1D("hQ24L2","Q2 4 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25L2  = new TH1D("hQ25L2","Q2 5 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26L2  = new TH1D("hQ26L2","Q2 6 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27L2  = new TH1D("hQ27L2","Q2 7 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28L2  = new TH1D("hQ28L2","Q2 8 t L2 Q2;", 300, 0.2, 0.6);      

  //Q2 Random

  TH1D *hQ21CR   = new TH1D("hQ21CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22CR   = new TH1D("hQ22CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23CR   = new TH1D("hQ23CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24CR   = new TH1D("hQ24CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25CR   = new TH1D("hQ25CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26CR   = new TH1D("hQ26CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27CR   = new TH1D("hQ27CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28CR   = new TH1D("hQ28CR","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21L1R  = new TH1D("hQ21L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22L1R  = new TH1D("hQ22L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23L1R  = new TH1D("hQ23L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24L1R  = new TH1D("hQ24L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25L1R  = new TH1D("hQ25L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26L1R  = new TH1D("hQ26L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27L1R  = new TH1D("hQ27L1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28L1R  = new TH1D("hQ28L1R","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21L2R  = new TH1D("hQ21L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22L2R  = new TH1D("hQ22L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23L2R  = new TH1D("hQ23L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24L2R  = new TH1D("hQ24L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25L2R  = new TH1D("hQ25L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26L2R  = new TH1D("hQ26L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27L2R  = new TH1D("hQ27L2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28L2R  = new TH1D("hQ28L2R","Q2; Q2;", 300, 0.2, 0.6);      

  //t
  TH1D *ht1C  = new TH1D("ht1C","t 1 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2C  = new TH1D("ht2C","t 2 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3C  = new TH1D("ht3C","t 3 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4C  = new TH1D("ht4C","t 4 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5C  = new TH1D("ht5C","t 5 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6C  = new TH1D("ht6C","t 6 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7C  = new TH1D("ht7C","t 7 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8C  = new TH1D("ht8C","t 8 t C; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1L1  = new TH1D("ht1L1","t 1 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2L1  = new TH1D("ht2L1","t 2 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3L1  = new TH1D("ht3L1","t 3 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4L1  = new TH1D("ht4L1","t 4 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5L1  = new TH1D("ht5L1","t 5 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6L1  = new TH1D("ht6L1","t 6 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7L1  = new TH1D("ht7L1","t 7 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8L1  = new TH1D("ht8L1","t 8 t L1; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1L2  = new TH1D("ht1L2","t 1 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2L2  = new TH1D("ht2L2","t 2 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3L2  = new TH1D("ht3L2","t 3 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4L2  = new TH1D("ht4L2","t 4 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5L2  = new TH1D("ht5L2","t 5 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6L2  = new TH1D("ht6L2","t 6 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7L2  = new TH1D("ht7L2","t 7 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8L2  = new TH1D("ht8L2","t 8 t L2; MandelT;",   300, -0.01, 0.1);      

  //t Random
  TH1D *ht1CR  = new TH1D("ht1CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2CR  = new TH1D("ht2CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3CR  = new TH1D("ht3CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4CR  = new TH1D("ht4CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5CR  = new TH1D("ht5CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6CR  = new TH1D("ht6CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7CR  = new TH1D("ht7CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8CR  = new TH1D("ht8CR","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1L1R  = new TH1D("ht1L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2L1R  = new TH1D("ht2L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3L1R  = new TH1D("ht3L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4L1R  = new TH1D("ht4L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5L1R  = new TH1D("ht5L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6L1R  = new TH1D("ht6L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7L1R  = new TH1D("ht7L1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8L1R  = new TH1D("ht8L1R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1L2R  = new TH1D("ht1L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2L2R  = new TH1D("ht2L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3L2R  = new TH1D("ht3L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4L2R  = new TH1D("ht4L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5L2R  = new TH1D("ht5L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6L2R  = new TH1D("ht6L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7L2R  = new TH1D("ht7L2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8L2R  = new TH1D("ht8L2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  //  Double_t bins[] = {-0.01, 0.00595, 0.0118167, 0.0151167, 0.01915, 0.02355, 0.0286833, 0.03565, 0.04555, 0.0701167, 0.1};
  //  Double_t bins[] = {-0.01, 0.00795, 0.0118167, 0.0171167, 0.02115, 0.02555, 0.0306833, 0.03765, 0.04755, 0.0701167, 0.1};
  //  Double_t bins[] = {-0.01, 0.00705, 0.0132833, 0.0173167, 0.0220833, 0.0275833, 0.0341833, 0.0440833, 0.06315, 0.0721167, 0.1};
  Double_t PhiBinLow = 0.932-0.003;
  //  Double_t PhiBinLow = 0.920-0.003;
  Double_t PhiBinHigh = 0.980+0.003;
  //PhiBinW = (0.980-0.932)/NoPhiBIn=0.003

  TH1D *htC  = new TH1D("htC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htL1  = new TH1D("htL1","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htL2  = new TH1D("htL2","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *htInC  = new TH1D("htInC","MandelT; MandelT;",   10, bins);      
  TH1D *htInL1  = new TH1D("htInL1","MandelT; MandelT;", 10, bins);      
  TH1D *htInL2  = new TH1D("htInL2","MandelT; MandelT;", 10, bins);      

  TH1D *htCR  = new TH1D("htCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htL1R  = new TH1D("htL1R","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htL2R  = new TH1D("htL2R","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *htInCR  = new TH1D("htInCR","MandelT; MandelT;",   10, bins);      
  TH1D *htInL1R  = new TH1D("htInL1R","MandelT; MandelT;", 10, bins);      
  TH1D *htInL2R  = new TH1D("htInL2R","MandelT; MandelT;", 10, bins);      

  //DATA
  TH1D *hph_qC  = new TH1D("hph_qC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qL1  = new TH1D("hph_qL1","ph_q; ph_q;", 18, -22.5, 382.5);      
  TH1D *hph_qL2  = new TH1D("hph_qL2","ph_q; ph_q;", 18, -22.5, 382.5);      

  TH1D *hph_qCR  = new TH1D("hph_qCR","ph_q; ph_q;",  18, -22.5, 382.5);      
  TH1D *hph_qL1R  = new TH1D("hph_qL1R","ph_q; ph_q;",18, -22.5, 382.5);      
  TH1D *hph_qL2R  = new TH1D("hph_qL2R","ph_q; ph_q;",18, -22.5, 382.5);      

  //SIMC
  TH1D *hph_qSC  = new TH1D("hph_qSC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qSL1  = new TH1D("hph_qSL1","ph_q; ph_q;", 18, -22.5, 382.5);      
  TH1D *hph_qSL2  = new TH1D("hph_qSL2","ph_q; ph_q;", 18, -22.5, 382.5);      

  TH1D *hmmC  = new TH1D("hmmC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1  = new TH1D("hmmL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2  = new TH1D("hmmL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmCR  = new TH1D("hmmCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1R  = new TH1D("hmmL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2R  = new TH1D("hmmL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinC   = new TH1D("htcoinC","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinL1  = new TH1D("htcoinL1"," ; ;", 300, -20.0, 20.0);      
  TH1D *htcoinL2  = new TH1D("htcoinL2","; ;", 300, -20.0, 20.0);      

  TH2D *hYC   = new TH2D("hYC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYCR  = new TH2D("hYCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYL1   = new TH2D("hYL1"," Yield L1;t-Bin; Phi-Bin ", 10 , bins, 18, -22.5, 382.5);      
  TH2D *hYL1R  = new TH2D("hYL1R"," Yield L1; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYL2   = new TH2D("hYL2"," Yield L2;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYL2R  = new TH2D("hYL2R"," Yield L2; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
 
  /*
  TH2D *hYC   = new TH2D("hYC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYCR  = new TH2D("hYCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYL1   = new TH2D("hYL1"," Yield L1;t-Bin; Phi-Bin ", 10 , bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYL1R  = new TH2D("hYL1R"," Yield L1; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYL2   = new TH2D("hYL2"," Yield L2;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYL2R  = new TH2D("hYL2R"," Yield L2; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  */
  //dummy
  TH2D *hYdC   = new TH2D("hYdC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYdCR  = new TH2D("hYdCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYdL1   = new TH2D("hYdL1"," Yield L1;t-Bin; Phi-Bin ", 10 , bins, 18, -22.5, 382.5);      
  TH2D *hYdL1R  = new TH2D("hYdL1R"," Yield L1; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYdL2   = new TH2D("hYdL2"," Yield L2;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYdL2R  = new TH2D("hYdL2R"," Yield L2; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  /*
  TH2D *hYdC   = new TH2D("hYdC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYdCR  = new TH2D("hYdCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYdL1   = new TH2D("hYdL1"," Yield L1;t-Bin; Phi-Bin ", 10 , bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYdL1R  = new TH2D("hYdL1R"," Yield L1; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYdL2   = new TH2D("hYdL2"," Yield L2;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYdL2R  = new TH2D("hYdL2R"," Yield L2; t-Bin; Phi-Bin", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  */
  //SIMC
  TH2D *hYSC   = new TH2D("hYSC"," Yield SIMC C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYSL1   = new TH2D("hYSL1"," Yield SIMC L1;t-Bin; Phi-Bin ", 10 , bins, 18, -22.5, 382.5);      
  TH2D *hYSL2   = new TH2D("hYSL2"," Yield SIMC L2;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  hYSC->Sumw2();
  hYSL1->Sumw2();
  hYSL2->Sumw2();
  /*
  TH2D *hYSC   = new TH2D("hYSC"," Yield SIMC C;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYSL1   = new TH2D("hYSL1"," Yield SIMC L1;t-Bin; Phi-Bin ", 10 , bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYSL2   = new TH2D("hYSL2"," Yield SIMC L2;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  */
  //SIMC and Data HISTOGRAMS
  //CENTER
  TH1D *hphC  = new TH1D("hphC","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphCR  = new TH1D("hphCR","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphDummyC  = new TH1D("hphDummyC","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphDummyCR  = new TH1D("hphDummyCR","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",50, -10, 370);      

  TH1D *hthC  = new TH1D("hthC","th_q; th_q;",   50, -0.01, 5.0);      
  TH1D *hthCR  = new TH1D("hthCR","th_q; th_q;",  50, -0.01, 5.0);      
  TH1D *hthDummyC  = new TH1D("hthDummyC","th_q; th_q;",   50, -0.01, 5.0);      
  TH1D *hthDummyCR  = new TH1D("hthDummyCR","th_q; th_q;",  50, -0.01, 5.0);      
  TH1D *hthSC  = new TH1D("hth_qC","th_q; th_q;",   50, -0.01, 5.0);      

  TH1D *H_ssdelta_SC  = new TH1D("H_ssdelta_SC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SC  = new TH1D("H_ssxptar_SC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SC  = new TH1D("H_ssyptar_SC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DC  = new TH1D("H_ssdelta_DC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DC  = new TH1D("H_ssxptar_DC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DC  = new TH1D("H_ssyptar_DC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DummyC  = new TH1D("H_ssdelta_DummyC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DummyC  = new TH1D("H_ssxptar_DummyC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DummyC  = new TH1D("H_ssyptar_DummyC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DCR  = new TH1D("H_ssdelta_DCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DCR  = new TH1D("H_ssxptar_DCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DCR  = new TH1D("H_ssyptar_DCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DummyCR  = new TH1D("H_ssdelta_DummyCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DummyCR  = new TH1D("H_ssxptar_DummyCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DummyCR  = new TH1D("H_ssyptar_DummyCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  
  TH1D *H_hsdelta_SC  = new TH1D("H_hsdelta_SC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SC  = new TH1D("H_hsxptar_SC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SC  = new TH1D("H_hsyptar_SC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DC  = new TH1D("H_hsdelta_DC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DC  = new TH1D("H_hsxptar_DC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DC  = new TH1D("H_hsyptar_DC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DummyC  = new TH1D("H_hsdelta_DummyC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DummyC  = new TH1D("H_hsxptar_DummyC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DummyC  = new TH1D("H_hsyptar_DummyC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DCR  = new TH1D("H_hsdelta_DCR","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DCR  = new TH1D("H_hsxptar_DCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DCR  = new TH1D("H_hsyptar_DCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DummyCR  = new TH1D("H_hsdelta_DummyCR","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DummyCR  = new TH1D("H_hsxptar_DummyCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DummyCR  = new TH1D("H_hsyptar_DummyCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *hthQ2C  = new TH1D("hthQ2C"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hthWC   = new TH1D("hthWC"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepC   = new TH1D("hepC"," ;epsilon; Yield (events/mC)", 150, 0.15, 0.35);
  
  TH1D *hthQ2SC  = new TH1D("hthQ2SC"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hthWSC   = new TH1D("hthWSC"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepSC   = new TH1D("hepSC"," ;epsilon; Yield (events/mC)", 150, 0.15, 0.35);

  TH1D *hthQ2CR  = new TH1D("hthQ2CR"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hthWCR   = new TH1D("hthWCR"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepCR   = new TH1D("hepCR"," ;epsilon; Yield (events/mC)", 150, 0.15, 0.35);

  TH1D *hthQ2DummyC  = new TH1D("hthQ2DummyC"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hthWDummyC   = new TH1D("hthWDummyC"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepDummyC   = new TH1D("hepDummyC"," ;epsilon; Yield (events/mC)", 150, 0.15, 0.35);

  TH1D *hthQ2DummyCR  = new TH1D("hthQ2DummyCR"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hthWDummyCR   = new TH1D("hthWDummyCR"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepDummyCR   = new TH1D("hepDummyCR"," ;epsilon; Yield (events/mC)", 150, 0.15, 0.35);

  //LEFT1
  TH1D *hphL1  = new TH1D("hphL1","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphL1R  = new TH1D("hphL1R","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphSL1  = new TH1D("hphSL1","ph_q; ph_q;",50, -10,370);      
  
  /*
  TH1D *hphL1  = new TH1D("hphL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphL1R  = new TH1D("hphL1R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphSL1  = new TH1D("hphSL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  TH1D *hthL1  = new TH1D("hthL1","th_q; th_q;",   50, -0.1, 0.1);      
  TH1D *hthL1R  = new TH1D("hthL1R","th_q; th_q;",  50, -0.1, 0.1);      
  TH1D *hthSL1  = new TH1D("hth_qL1","th_q; th_q;",   50, -0.1, 0.1);      

  TH1D *H_ssdelta_SL1  = new TH1D("H_ssdelta_SL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SL1  = new TH1D("H_ssxptar_SL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SL1  = new TH1D("H_ssyptar_SL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DL1  = new TH1D("H_ssdelta_DL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DL1  = new TH1D("H_ssxptar_DL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DL1  = new TH1D("H_ssyptar_DL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DL1R  = new TH1D("H_ssdelta_DL1R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DL1R  = new TH1D("H_ssxptar_DL1R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DL1R  = new TH1D("H_ssyptar_DL1R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SL1  = new TH1D("H_hsdelta_SL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SL1  = new TH1D("H_hsxptar_SL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SL1  = new TH1D("H_hsyptar_SL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DL1  = new TH1D("H_hsdelta_DL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DL1  = new TH1D("H_hsxptar_DL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DL1  = new TH1D("H_hsyptar_DL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DL1R  = new TH1D("H_hsdelta_DL1R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DL1R  = new TH1D("H_hsxptar_DL1R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DL1R  = new TH1D("H_hsyptar_DL1R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //LEFT2
  TH1D *hphL2  = new TH1D("hphL2","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphL2R  = new TH1D("hphL2R","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphSL2  = new TH1D("hphSL2","ph_q; ph_q;",50, -10, 370);      
  /*
  TH1D *hphL2  = new TH1D("hphL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphL2R  = new TH1D("hphL2R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphSL2  = new TH1D("hphSL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  TH1D *hthL2  = new TH1D("hthL2","th_q; th_q;",   50, -0.1, 0.1);      
  TH1D *hthL2R  = new TH1D("hthL2R","th_q; th_q;",  50, -0.1, 0.1);      
  TH1D *hthSL2  = new TH1D("hth_qL2","th_q; th_q;",   50, -0.1, 0.1);      

  TH1D *H_ssdelta_SL2  = new TH1D("H_ssdelta_SL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SL2  = new TH1D("H_ssxptar_SL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SL2  = new TH1D("H_ssyptar_SL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DL2  = new TH1D("H_ssdelta_DL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DL2  = new TH1D("H_ssxptar_DL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DL2  = new TH1D("H_ssyptar_DL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DL2R  = new TH1D("H_ssdelta_DL2R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DL2R  = new TH1D("H_ssxptar_DL2R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DL2R  = new TH1D("H_ssyptar_DL2R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SL2  = new TH1D("H_hsdelta_SL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SL2  = new TH1D("H_hsxptar_SL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SL2  = new TH1D("H_hsyptar_SL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DL2  = new TH1D("H_hsdelta_DL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DL2  = new TH1D("H_hsxptar_DL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DL2  = new TH1D("H_hsyptar_DL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DL2R  = new TH1D("H_hsdelta_DL2R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DL2R  = new TH1D("H_hsxptar_DL2R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DL2R  = new TH1D("H_hsyptar_DL2R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_t_S  = new TH1D("H_t_S","t; t;", 300, -0.01, 0.1); 
  TH1D *H_ti_S  = new TH1D("H_ti_S","ti; ti;", 300, -0.01, 0.1); 
  TH1D *H_t_RE  = new TH1D("H_t_RE"," ; t Resolution (t-ti);", 300, -0.02, 0.02); 
  TH1D *H_t_RE1  = new TH1D("H_t_RE1"," t Resolution (t-ti) 1 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE2  = new TH1D("H_t_RE2"," t Resolution (t-ti) 2 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE3  = new TH1D("H_t_RE3"," t Resolution (t-ti) 3 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE4  = new TH1D("H_t_RE4"," t Resolution (t-ti) 4 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE5  = new TH1D("H_t_RE5"," t Resolution (t-ti) 5 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE6  = new TH1D("H_t_RE6"," t Resolution (t-ti) 6 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE7  = new TH1D("H_t_RE7"," t Resolution (t-ti) 7 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE8  = new TH1D("H_t_RE8"," t Resolution (t-ti) 8 bin; t Resolution;", 300, -0.03, 0.03); 
  TH2D *H_t_RE_t = new TH2D("H_t_RE_t","t Resolution vs t; t Resolution; t ", 300, -0.02, 0.03, 300, 0.0, 0.04);      

  //Added on Apr 04, 2025
  
  TH2D *shleC    = CreateHistogram("shleC",   " ");
  TH2D *shleL1   = CreateHistogram("shleL1",  " ");
  TH2D *shleL2   = CreateHistogram("shleL2",  " ");
  
  /*
  TCutG *Dcut = new TCutG("Dcut",5);
  Dcut->SetVarX("Q2");
  Dcut->SetVarY("W");
  Dcut->SetPoint(0,0.363783,2.18318);Dcut->SetPoint(1,0.311069,2.22644);  
  Dcut->SetPoint(2,0.37737,2.21136); Dcut->SetPoint(3,0.452314,2.16293);
  Dcut->SetPoint(4,0.363783,2.18318); 
  */
  //Q2 analysis
  TCutG *Dcut = new TCutG("Dcut",5);
  Dcut->SetVarX("Q2");
  Dcut->SetVarY("W");
  
  Dcut->SetPoint(0,0.407515,2.18602);Dcut->SetPoint(1,0.348397,2.22725);
  Dcut->SetPoint(2,0.436072,2.20741); Dcut->SetPoint(3,0.501202,2.16458);
  Dcut->SetPoint(4,0.407515,2.18602);

  /*
  Dcut->SetPoint(0,0.41759,2.18413);Dcut->SetPoint(1,0.361513,2.22456);  
  Dcut->SetPoint(2,0.424513,2.21039); Dcut->SetPoint(3,0.487513,2.17053);
  Dcut->SetPoint(4,0.41759,2.18413); 
  */
  
  /*
  Dcut->SetPoint(0,0.39779305,2.18847622);Dcut->SetPoint(1,0.33438134,2.23125977);  
  Dcut->SetPoint(2,0.44894876,2.20543771); Dcut->SetPoint(3,0.51742541,2.16145995);
  Dcut->SetPoint(4,0.39779305,2.1884762); 
  */  
  //////////////////////////////
  Double_t MMPICUTL = 0.91; 
  Double_t MMPICUTH = 0.98; 
  /////////////////////////////
  //Missing mass cut dependent study
  // Double_t MMPICUTH = 0.978; 
  //  Double_t MMPICUTH = 0.976; 
  // Double_t MMPICUTH = 0.974; 
  // Double_t MMPICUTH = 0.972; 
  
  //  Double_t MMPICUTH = 0.982; 
  // Double_t MMPICUTH = 0.984; 
  // Double_t MMPICUTH = 0.986; 
  //  Double_t MMPICUTH = 0.988; 

  // ULong64_t Q2C1[60000000];
  //ULong64_t WC1[600000000];
  // Q2C1 = new ULong64_t[nEntries_TBRANCHC];
  // WC1 = new ULong64_t[nEntries_TBRANCHC];
  //TVectorD Q2C1(nEntries_TBRANCHC);
  //TVectorD WC1(nEntries_TBRANCHC);
  //  std::vector<Double_t> Q2C1(nEntries_TBRANCHC);
  // std::vector<Double_t> WC1(nEntries_TBRANCHC);

  //  Double_t Q2C1[nEntries_TBRANCHC];
  //  Double_t WC1[nEntries_TBRANCHC]; 
  
  //  Double_t qL1[nEntries_TBRANCHL1];
  // Double_t wL1[nEntries_TBRANCHL1]; 
  /*
  Double_t MMpiOffsetL2 = 0.003097;
  Double_t MMpiOffsetL1 = 0.003584;
  Double_t MMpiOffsetC  = 0.003396;
  */
  Double_t MMpiOffsetL2 = 0.007723;
  Double_t MMpiOffsetL1 = 0.007723;
  Double_t MMpiOffsetC  = 0.006723;
  
  Double_t tOffsetC  = 0.0015;
  Double_t tOffsetL1  = 0.002;

  // Double_t tOffsetC  = 0.00;
  Double_t tOffsetL2  = 0.002;
    
  /*
    Double_t tOffsetC  = 0.00;
    Double_t tOffsetL2  = 0.00;
  */
  //DATA CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);
      
      if(tcoinC -TimmingOffset >=-1.0 && tcoinC -TimmingOffset <= 1.0 && mmC>= MMPICUTL-MMpiOffsetC && mmC <=MMPICUTH -MMpiOffsetC)
	{
	  hQ2WC->Fill(Q2C, WC);
	}
      
      if(tcoinC-TimmingOffset>=-1.0 && tcoinC-TimmingOffset <= 1.0)    
	{
	  hmmC->Fill(mmC);	  
	}	      
      
      {
	htcoinC->Fill(tcoinC-TimmingOffset);
      }
      
      Double_t DiamondC = (Dcut->IsInside(Q2C, WC));  
      Double_t MMpiC = mmC>= MMPICUTL-MMpiOffsetC && mmC <=MMPICUTH-MMpiOffsetC;
      Double_t CoinPionC = tcoinC-TimmingOffset>=-1.0 && tcoinC-TimmingOffset <= 1.0;      
      Double_t CoinPionCR = ((tcoinC-TimmingOffset>=-15.0 && tcoinC-TimmingOffset<= -9.0) || (tcoinC-TimmingOffset>=7.0 && tcoinC-TimmingOffset<=13.0));      

      //      if(CoinPionC && DiamondC)
     if(CoinPionC)
	{      
	  MPiC->Fill(mmC+MMpiOffsetC);
	}     
     if(CoinPionC && MMpiC && DiamondC)
	{
	  H_ssdelta_DC->Fill(P_gtr_dp-0.25);
	  H_ssxptar_DC->Fill(P_gtr_xptar);
	  H_ssyptar_DC->Fill(P_gtr_yptar);

	  H_hsdelta_DC->Fill(H_gtr_dp);
	  H_hsxptar_DC->Fill(H_gtr_xptar);
	  H_hsyptar_DC->Fill(H_gtr_yptar);
	  hthQ2C->Fill(Q2C);
	  hthWC->Fill(WC);
	  hepC->Fill(epsilonC);
	  hQ2WC1->Fill(Q2C, WC);	      	  

	  hthtC->Fill(TMath::Abs(tC)+tOffsetC); 
 	  htC->Fill(TMath::Abs(tC));
	  htInC->Fill(TMath::Abs(tC));	
	  hph_qC->Fill(ph_qC*57.2958 + 180);
	  //  hphC->Fill(mmC+MMpiOffsetC);
	  hYC->Fill(TMath::Abs(tC)+tOffsetC, std::fmod(ph_qC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYC->Fill(-tC, mmC+MMpiOffsetC);   //0.003492

	  hphC->Fill(std::fmod(ph_qC * 180.0 / TMath::Pi() + 360.0,360.0));
	  hthC->Fill(th_qC*57.2958);

	  shleC->Fill(-tC*cos(ph_qC), -tC*sin(ph_qC));	  
	}	

     //Random    
     if(CoinPionCR && DiamondC)
 	{
	  MPiCR->Fill(mmC+MMpiOffsetC);
	}
      if(CoinPionCR && MMpiC && DiamondC)
	{
	  H_ssdelta_DCR->Fill(P_gtr_dp-0.25);
	  H_ssxptar_DCR->Fill(P_gtr_xptar);
	  H_ssyptar_DCR->Fill(P_gtr_yptar);

	  H_hsdelta_DCR->Fill(H_gtr_dp);
	  H_hsxptar_DCR->Fill(H_gtr_xptar);
	  H_hsyptar_DCR->Fill(H_gtr_yptar);
	  hthQ2CR->Fill(Q2C);
	  hthWCR->Fill(WC);	  	  
	  hepCR->Fill(epsilonC);
	  hthtCR->Fill(TMath::Abs(tC)+tOffsetC); 
	  htCR->Fill(TMath::Abs(tC));
	  htInCR->Fill(TMath::Abs(tC));	
	  hph_qCR->Fill(ph_qC*57.2958 + 180);
	  hphCR->Fill(std::fmod(ph_qC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //  hphCR->Fill(mmC+MMpiOffsetC);
	  hQ2WCR->Fill(Q2C, WC);
	  hYCR->Fill(TMath::Abs(tC)+tOffsetC, std::fmod(ph_qC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYCR->Fill(-tC, mmC+MMpiOffsetC);
	  hthCR->Fill(th_qC*57.2958);
	}
      
      //For kin_aver, Q2, W & p_theta 
      /*
      Double_t w1t = -tC+tOffsetC >= bins[1] && -tC+tOffsetC <= bins[2];
      Double_t w2t = -tC+tOffsetC >= bins[2] && -tC+tOffsetC <= bins[3];
      Double_t w3t = -tC+tOffsetC >= bins[3] && -tC+tOffsetC <= bins[4];
      Double_t w4t = -tC+tOffsetC >= bins[4] && -tC+tOffsetC <= bins[5];
      Double_t w5t = -tC+tOffsetC >= bins[5] && -tC+tOffsetC <= bins[6];
      Double_t w6t = -tC+tOffsetC >= bins[6] && -tC+tOffsetC <= bins[7];
      Double_t w7t = -tC+tOffsetC >= bins[7] && -tC+tOffsetC <= bins[8];
      Double_t w8t = -tC+tOffsetC >= bins[8] && -tC+tOffsetC <= bins[9];
      */
      Double_t w1t = TMath::Abs(tC) >= bins[1] && TMath::Abs(tC) <= bins[2];
      Double_t w2t = TMath::Abs(tC) >= bins[2] && TMath::Abs(tC) <= bins[3];
      Double_t w3t = TMath::Abs(tC) >= bins[3] && TMath::Abs(tC) <= bins[4];
      Double_t w4t = TMath::Abs(tC) >= bins[4] && TMath::Abs(tC) <= bins[5];
      Double_t w5t = TMath::Abs(tC) >= bins[5] && TMath::Abs(tC) <= bins[6];
      Double_t w6t = TMath::Abs(tC) >= bins[6] && TMath::Abs(tC) <= bins[7];
      Double_t w7t = TMath::Abs(tC) >= bins[7] && TMath::Abs(tC) <= bins[8];
      Double_t w8t = TMath::Abs(tC) >= bins[8] && TMath::Abs(tC) <= bins[9];

      if(CoinPionC && MMpiC && DiamondC && w1t)
	{
	  hW1C->Fill(WC);	
	  hQ21C->Fill(Q2C);	      	  
	  ht1C->Fill(TMath::Abs(tC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w2t)
	{
	  hW2C->Fill(WC);	
	  hQ22C->Fill(Q2C);	      	  
	  ht2C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w3t)
	{
	  hW3C->Fill(WC);	
	  hQ23C->Fill(Q2C);	      	  
	  ht3C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w4t)
	{
	  hW4C->Fill(WC);	
	  hQ24C->Fill(Q2C);	      	  
	  ht4C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w5t)
	{
	  hW5C->Fill(WC);	
	  hQ25C->Fill(Q2C);	      	  
	  ht5C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w6t)
	{
	  hW6C->Fill(WC);	
	  hQ26C->Fill(Q2C);	      	  
	  ht6C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w7t)
	{
	  hW7C->Fill(WC);	
	  hQ27C->Fill(Q2C);	      	  
	  ht7C->Fill(TMath::Abs(tC)); 
  	}	
      if(CoinPionC && MMpiC && DiamondC && w8t)
	{
	  hW8C->Fill(WC);	
	  hQ28C->Fill(Q2C);	      	  
 	  ht8C->Fill(TMath::Abs(tC)); 
 	}
	
      //Random
      if(CoinPionCR && MMpiC && DiamondC && w1t)
      	{	  
	  hW1CR->Fill(WC);	
	  hQ21CR->Fill(Q2C);	      	  
	  ht1CR->Fill(TMath::Abs(tC)); 
	}      

      if(CoinPionCR && MMpiC && DiamondC && w2t)
      	{	  
	  hW2CR->Fill(WC);	
	  hQ22CR->Fill(Q2C);	      	  
	  ht2CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w3t)
      	{	  
	  hW3CR->Fill(WC);	
	  hQ23CR->Fill(Q2C);	      	  
	  ht3CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w4t)
      	{	  
	  hW4CR->Fill(WC);	
	  hQ24CR->Fill(Q2C);	      	  
	  ht4CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w5t)
      	{	  
	  hW5CR->Fill(WC);	
	  hQ25CR->Fill(Q2C);	      	  
	  ht5CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w6t)
      	{	  
	  hW6CR->Fill(WC);	
	  hQ26CR->Fill(Q2C);	      	  
	  ht6CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w7t)
      	{	  
	  hW7CR->Fill(WC);	
	  hQ27CR->Fill(Q2C);	      	  
	  ht7CR->Fill(TMath::Abs(tC)); 
	}      
      if(CoinPionCR && MMpiC && DiamondC && w8t)
      	{	  
	  hW8CR->Fill(WC);	
	  hQ28CR->Fill(Q2C);	      	  
	  ht8CR->Fill(TMath::Abs(tC)); 
	}      
      
      if (((tcoinC-TimmingOffset>=-15.0 && tcoinC-TimmingOffset <= -9.0) || (tcoinC-TimmingOffset>=7.0 && tcoinC-TimmingOffset<=13.0)))
	
	{
	  hmmCR->Fill(mmC);
	}	
    }  
  
  //DATA dummy CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHDC; i++)
    {
      TBRANCHDC->GetEntry(i);
            
      Double_t DiamonddC = (Dcut->IsInside(Q2dC, WdC));  
      Double_t MMpidC = mmdC>= MMPICUTL-MMpiOffsetC && mmdC <=MMPICUTH-MMpiOffsetC;
      Double_t CoinPiondC = tcoindC-TimmingOffset>=-1.0 && tcoindC-TimmingOffset <= 1.0;      
      Double_t CoinPiondCR = ((tcoindC-TimmingOffset>=-15.0 && tcoindC-TimmingOffset<= -9.0) || (tcoindC-TimmingOffset>=7.0 && tcoindC-TimmingOffset<=13.0));      
      
      if(CoinPiondC && DiamonddC)
	{
	  MPidC->Fill(mmdC+MMpiOffsetC);
	}
      if(CoinPiondC && MMpidC && DiamonddC)
	{
	  hYdC->Fill(TMath::Abs(tdC)+tOffsetC, std::fmod(ph_qdC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYdC->Fill(-tdC, mmdC+MMpiOffsetC);
	  hthtdC->Fill(TMath::Abs(tdC)+tOffsetC); 

	  H_ssdelta_DummyC->Fill(P_gtr_dpd-0.25);
	  H_ssxptar_DummyC->Fill(P_gtr_xptard);
	  H_ssyptar_DummyC->Fill(P_gtr_yptard);
	  
	  H_hsdelta_DummyC->Fill(H_gtr_dpd);
	  H_hsxptar_DummyC->Fill(H_gtr_xptard);
	  H_hsyptar_DummyC->Fill(H_gtr_yptard);

	  hthQ2DummyC->Fill(Q2dC);
	  hthWDummyC->Fill(WdC);
	  hepDummyC->Fill(epsilondC);
	  hphDummyC->Fill(std::fmod(ph_qdC * 180.0 / TMath::Pi() + 360.0,360.0));
	  
	}	

      //Random
      Double_t CUTCCR = ((tcoindC-TimmingOffset>=-15.0 && tcoindC-TimmingOffset <= -9.0) || (tcoindC-TimmingOffset>=7.0 && tcoindC-TimmingOffset <=13.0));      

      if(CoinPiondCR && DiamonddC)
	{
	  MPidCR->Fill(mmdC+MMpiOffsetC);
	}
      if(CoinPiondCR && MMpidC && DiamonddC)
	{
	  hYdCR->Fill(TMath::Abs(tdC)+tOffsetC, std::fmod(ph_qdC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYdCR->Fill(-tdC, mmdC+MMpiOffsetC);
	  hthtdCR->Fill(TMath::Abs(tdC)+tOffsetC); 

	  H_ssdelta_DummyCR->Fill(P_gtr_dpd-0.25);
	  H_ssxptar_DummyCR->Fill(P_gtr_xptard);
	  H_ssyptar_DummyCR->Fill(P_gtr_yptard);
	  
	  H_hsdelta_DummyCR->Fill(H_gtr_dpd);
	  H_hsxptar_DummyCR->Fill(H_gtr_xptard);
	  H_hsyptar_DummyCR->Fill(H_gtr_yptard);
	  hthQ2DummyCR->Fill(Q2dC);
	  hthWDummyCR->Fill(WdC);	  
	  hepDummyCR->Fill(epsilondC);
	  hphDummyCR->Fill(std::fmod(ph_qdC * 180.0 / TMath::Pi() + 360.0,360.0));	  
	}
    }  

  //SIMC CENTER
  for(Long64_t i = 0; i < nEntries_TSIMCC; i++)

    {
      TSIMCC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simc, W_simc));  
      
      //Double_t fact = 9942030.0/400000.0;
      //after simc offsets
      Double_t fact = 9974370.0/400000.0;

      //RadiativeStudy
      // Double_t fact = 7932660.0/400000.0;


      Double_t Sw1t = t_simc >= bins[1] && t_simc <= bins[2];
      Double_t Sw2t = t_simc >= bins[2] && t_simc <= bins[3];
      Double_t Sw3t = t_simc >= bins[3] && t_simc <= bins[4];
      Double_t Sw4t = t_simc >= bins[4] && t_simc <= bins[5];
      Double_t Sw5t = t_simc >= bins[5] && t_simc <= bins[6];
      Double_t Sw6t = t_simc >= bins[6] && t_simc <= bins[7];
      Double_t Sw7t = t_simc >= bins[7] && t_simc <= bins[8];
      Double_t Sw8t = t_simc >= bins[8] && t_simc <= bins[9];

      //            Double_t CUTSIMCC = hsxpfp >=-0.08 && hsxpfp <=0.08 && hsypfp >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && ssdelta <=20.0 && ssxpfp >=-0.06 && ssxpfp <=0.06 && ssypfp >=-0.04 && ssypfp <=0.04 && missmass >= 0.91 && missmass <= 0.98 && Diamond_cut;
      
      Double_t CUTSIMCC = (hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H) && (hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H) && (hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Yptar_High_H) && (ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P) && (ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P) && (ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P) && (missmass >=MMPICUTL && missmass <=MMPICUTH) && Diamond_cut;

      Double_t CUTSIMCC2 = hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H && hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H && hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Yptar_High_H && ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P && ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P && ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P && Diamond_cut;

      Double_t CUTSIMCC3 = hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H && hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H && hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Yptar_High_H && ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P && ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P && ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P;
      
      if(CUTSIMCC3)
	{
	  hQ2WSC->Fill(Q2_simc, W_simc, fact*Weight);
	}
      if(CUTSIMCC2)
	{
	  MPiSC->Fill(missmass, fact*Weight);
	  hQ2WSC->Fill(Q2_simc, W_simc, fact*Weight);
	}

      if(CUTSIMCC)
	{
	  hthSC->Fill(thetapq*57.2958, fact*Weight);
	  hph_qSC->Fill(phipq*57.2958, fact*Weight);
	  hphSC->Fill(phipq*57.2958, fact*Weight);
	  hthQ2SC->Fill(Q2_simc, fact*Weight);
	  hthWSC->Fill(2.225-W_simc+2.165+0.003, fact*Weight);  // I made for plot only
	  hepSC->Fill(0.287-epsilon_simc+0.242, fact*Weight);   // I made for plot only
	  //hphSC->Fill(missmass, fact*Weight);
	  hYSC->Fill(t_simc, phipq*57.2958, fact*Weight);
	  //hYSC->Fill(t_simc, missmass, fact*Weight);
	  H_ssdelta_SC->Fill(ssdelta, fact*Weight);
	  H_ssxptar_SC->Fill(ssxptar, fact*Weight);
	  H_ssyptar_SC->Fill(ssyptar, fact*Weight);
	 
	  H_hsdelta_SC->Fill(-1*hsdelta, fact*Weight);
	  H_hsxptar_SC->Fill(hsxptar, fact*Weight);
	  H_hsyptar_SC->Fill(hsyptar, fact*Weight);
	 
	  H_t_RE->Fill((t_simc-ti_simc), fact*Weight);
	  H_t_RE_t->Fill((t_simc-ti_simc), t_simc, fact*Weight);
	  hthtSC->Fill(t_simc, fact*Weight); 

	  hQ2WSC1->Fill(Q2_simc, W_simc, fact*Weight);


	}    

      if(CUTSIMCC && Sw1t)
	{
	  H_t_RE1->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw2t)
	{
	  H_t_RE2->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw3t)
	{
	  H_t_RE3->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw4t)
	{
	  H_t_RE4->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw5t)
	{
	  H_t_RE5->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw6t)
	{
	  H_t_RE6->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw7t)
	{
	  H_t_RE7->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(CUTSIMCC && Sw8t)
	{
	  H_t_RE8->Fill((t_simc-ti_simc), fact*Weight);
	}
    }  
  
  //DATA LEFT1
  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
  
    {
      TBRANCHL1->GetEntry(i);
            
      if(tcoinL1-TimmingOffset>=-1.0 && tcoinL1-TimmingOffset <= 1.0)	
	{
	  hmmL1->Fill(mmL1);
	}           
      {
	htcoinL1->Fill(tcoinL1-TimmingOffset);
      }
     
      Double_t DiamondL1 = (Dcut->IsInside(Q2L1, WL1));  
      Double_t MMpiL1 = mmL1>= MMPICUTL-MMpiOffsetL1 && mmL1 <=MMPICUTH-MMpiOffsetL1;
      Double_t CoinPionL1 = tcoinL1-TimmingOffset>=-1.0 && tcoinL1-TimmingOffset <= 1.0;      
      Double_t CoinPionL1R = ((tcoinL1-TimmingOffset>=-15.0 && tcoinL1-TimmingOffset<= -9.0) || (tcoinL1-TimmingOffset>=7.0 && tcoinL1-TimmingOffset<=13.0));      

      if(CoinPionL1 && DiamondL1)
	{      
	  MPiL1->Fill(mmL1+MMpiOffsetL1);
	}     

      if(CoinPionL1 && MMpiL1 && DiamondL1)
	{
	  H_ssdelta_DL1->Fill(P_gtr_dpL1);
	  H_ssxptar_DL1->Fill(P_gtr_xptarL1);
	  H_ssyptar_DL1->Fill(P_gtr_yptarL1);

	  H_hsdelta_DL1->Fill(H_gtr_dpL1);
	  H_hsxptar_DL1->Fill(H_gtr_xptarL1);
	  H_hsyptar_DL1->Fill(H_gtr_yptarL1);

	  hthtL1->Fill(TMath::Abs(tL1)+tOffsetL1); 
	  htL1->Fill(TMath::Abs(tL1));
 	  htInL1->Fill(TMath::Abs(tL1));
	  hph_qL1->Fill(ph_qL1*57.2958 + 180);	  
	  hphL1->Fill(std::fmod(ph_qL1 * 180.0 / TMath::Pi() + 360.0,360.0));	  
	  // hphL1->Fill(mmL1+MMpiOffsetL1);	  
	  hYL1->Fill(TMath::Abs(tL1)+tOffsetL1, std::fmod(ph_qL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYL1->Fill(-tL1, mmL1+MMpiOffsetL1);

	  shleL1->Fill(TMath::Abs(tL1)*cos(ph_qL1), TMath::Abs(tL1)*sin(ph_qL1));	  
	}     
 
      //Random            
      if(CoinPionL1R && DiamondL1)		
	{
	  MPiL1R->Fill(mmL1+MMpiOffsetL1);
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1)		
	{
	  H_ssdelta_DL1R->Fill(P_gtr_dpL1);
	  H_ssxptar_DL1R->Fill(P_gtr_xptarL1);
	  H_ssyptar_DL1R->Fill(P_gtr_yptarL1);

	  H_hsdelta_DL1R->Fill(H_gtr_dpL1);
	  H_hsxptar_DL1R->Fill(H_gtr_xptarL1);
	  H_hsyptar_DL1R->Fill(H_gtr_yptarL1);

	  hthtL1R->Fill(TMath::Abs(tL1)+tOffsetL1); 
	  htL1R->Fill(TMath::Abs(tL1));
 	  htInL1R->Fill(TMath::Abs(tL1));
	  hph_qL1R->Fill(ph_qL1*57.2958 + 180);	  
	  hphL1R->Fill(std::fmod(ph_qL1 * 180.0 / TMath::Pi() + 360.0,360.0));	  
	  //hphL1R->Fill(mmL1+MMpiOffsetL1);	  
	  hYL1R->Fill(TMath::Abs(tL1)+tOffsetL1, std::fmod(ph_qL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYL1R->Fill(-tL1, mmL1+MMpiOffsetL1);
	}
      
      //For kin_aver, Q2, W & p_theta 
      /*
      Double_t w1t = -tL1 >= bins[1] && -tL1 <= bins[2];
      Double_t w2t = -tL1 >= bins[2] && -tL1 <= bins[3];
      Double_t w3t = -tL1 >= bins[3] && -tL1 <= bins[4];
      Double_t w4t = -tL1 >= bins[4] && -tL1 <= bins[5];
      Double_t w5t = -tL1 >= bins[5] && -tL1 <= bins[6];
      Double_t w6t = -tL1 >= bins[6] && -tL1 <= bins[7];
      Double_t w7t = -tL1 >= bins[7] && -tL1 <= bins[8];
      Double_t w8t = -tL1 >= bins[8] && -tL1 <= bins[9];
      */
      Double_t w1t = TMath::Abs(tL1) >= bins[1] && TMath::Abs(tL1) <= bins[2];
      Double_t w2t = TMath::Abs(tL1) >= bins[2] && TMath::Abs(tL1) <= bins[3];
      Double_t w3t = TMath::Abs(tL1) >= bins[3] && TMath::Abs(tL1) <= bins[4];
      Double_t w4t = TMath::Abs(tL1) >= bins[4] && TMath::Abs(tL1) <= bins[5];
      Double_t w5t = TMath::Abs(tL1) >= bins[5] && TMath::Abs(tL1) <= bins[6];
      Double_t w6t = TMath::Abs(tL1) >= bins[6] && TMath::Abs(tL1) <= bins[7];
      Double_t w7t = TMath::Abs(tL1) >= bins[7] && TMath::Abs(tL1) <= bins[8];
      Double_t w8t = TMath::Abs(tL1) >= bins[8] && TMath::Abs(tL1) <= bins[9];

      if(CoinPionL1 && MMpiL1 && DiamondL1 && w1t)
	{
	  hW1L1->Fill(WL1);	
	  hQ21L1->Fill(Q2L1);	      	  
	  ht1L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w2t)
	{
	  hW2L1->Fill(WL1);	
	  hQ22L1->Fill(Q2L1);	      	  
	  ht2L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w3t)
	{
	  hW3L1->Fill(WL1);	
	  hQ23L1->Fill(Q2L1);	      	  
	  ht3L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w4t)
	{
	  hW4L1->Fill(WL1);	
	  hQ24L1->Fill(Q2L1);	      	  
	  ht4L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w5t)
	{
	  hW5L1->Fill(WL1);	
	  hQ25L1->Fill(Q2L1);	      	  
	  ht5L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w6t)
	{
	  hW6L1->Fill(WL1);	
	  hQ26L1->Fill(Q2L1);	      	  
	  ht6L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w7t)
	{
	  hW7L1->Fill(WL1);	
	  hQ27L1->Fill(Q2L1);	      	  
	  ht7L1->Fill(TMath::Abs(tL1)); 
  	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w8t)
	{
	  hW8L1->Fill(WL1);	
	  hQ28L1->Fill(Q2L1);	      	  
	  ht8L1->Fill(TMath::Abs(tL1)); 
  	}	

      //Random
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w1t)
      	{	  
	  hW1L1R->Fill(WL1);	
	  hQ21L1R->Fill(Q2L1);	      	  
	  ht1L1R->Fill(TMath::Abs(tL1)); 
	}      

      if(CoinPionL1R && MMpiL1 && DiamondL1 && w2t)
      	{	  
	  hW2L1R->Fill(WL1);	
	  hQ22L1R->Fill(Q2L1);	      	  
	  ht2L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w3t)
      	{	  
	  hW3L1R->Fill(WL1);	
	  hQ23L1R->Fill(Q2L1);	      	  
	  ht3L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w4t)
      	{	  
	  hW4L1R->Fill(WL1);	
	  hQ24L1R->Fill(Q2L1);	      	  
	  ht4L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w5t)
      	{	  
	  hW5L1R->Fill(WL1);	
	  hQ25L1R->Fill(Q2L1);	      	  
	  ht5L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w6t)
      	{	  
	  hW6L1R->Fill(WL1);	
	  hQ26L1R->Fill(Q2L1);	      	  
	  ht6L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w7t)
      	{	  
	  hW7L1R->Fill(WL1);	
	  hQ27L1R->Fill(Q2L1);	      	  
	  ht7L1R->Fill(TMath::Abs(tL1)); 
	}      
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w8t)
      	{	  
	  hW8L1R->Fill(WL1);	
	  hQ28L1R->Fill(Q2L1);	      	  
	  ht8L1R->Fill(TMath::Abs(tL1)); 
	}      

      if (((tcoinL1-TimmingOffset>=-15.0 && tcoinL1-TimmingOffset <= -9.0) || (tcoinL1-TimmingOffset>=7.0 && tcoinL1-TimmingOffset <=13.0)))
	
	{	 
	  hmmL1R->Fill(mmL1);
	}     
    }
  //DATA dummy LEFL1
  for(Long64_t i = 0; i < nEntries_TBRANCHDL1; i++)
    {
      TBRANCHDL1->GetEntry(i);

      Double_t DiamonddL1 = (Dcut->IsInside(Q2dL1, WdL1));  
      Double_t MMpidL1 = mmdL1>= MMPICUTL-MMpiOffsetL1 && mmdL1 <=MMPICUTH-MMpiOffsetL1;
      Double_t CoinPiondL1 = tcoindL1-TimmingOffset>=-1.0 && tcoindL1-TimmingOffset <= 1.0;      
      Double_t CoinPiondL1R = ((tcoindL1-TimmingOffset>=-15.0 && tcoindL1-TimmingOffset<= -9.0) || (tcoindL1-TimmingOffset>=7.0 && tcoindL1-TimmingOffset<=13.0));      
      
      if(CoinPiondL1 && DiamonddL1)
	{
	  MPidL1->Fill(mmdL1+MMpiOffsetL1);
	}
      if(CoinPiondL1 && MMpidL1 && DiamonddL1)
	{
	  hYdL1->Fill(TMath::Abs(tdL1)+tOffsetL1, std::fmod(ph_qdL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYdL1->Fill(-tdL1,mmdL1+MMpiOffsetL1);
	  hthtdL1->Fill(TMath::Abs(tdL1)); 
	}	
      
      //Random
      if(CoinPiondL1R && DiamonddL1)
	{
	  MPidL1R->Fill(mmdL1+MMpiOffsetL1);
	}
      if(CoinPiondL1R && MMpidL1 && DiamonddL1)
	{
	  hYdL1R->Fill(TMath::Abs(tdL1)+tOffsetL1, std::fmod(ph_qdL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYdL1R->Fill(-tdL1,mmdL1+MMpiOffsetL1);
	  hthtdL1R->Fill(TMath::Abs(tdL1)); 
	}
    }    
  
  //SIMC LEFT1
  for(Long64_t i = 0; i < nEntries_TSIMCL1; i++)
    
    {
      TSIMCL1->GetEntry(i);
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcL1, W_simcL1));  

      //Double_t factL1 = 9661780.0/400000.0;
      //After applying simc offsets
      Double_t factL1 = 10175600.0/400000.0;
      //RadiativeStudy
      // Double_t factL1 = 7528680.0/400000.0;

      //      Double_t CUTSIMCL1 = hsdeltaL1 >=-8.0 && hsdeltaL1 <=8.0 && hsxpfpL1 >=-0.08 && hsxpfpL1 <=0.08 && hsypfpL1 >=-0.045 && hsypfpL1 <=0.045 && ssdeltaL1 >=-10.0 && ssdeltaL1 <=20.0 && ssxpfpL1 >=-0.06 && ssxpfpL1 <=0.06 && ssypfpL1 >=-0.04 && ssypfpL1 <=0.04 && missmassL1 >=0.932 && missmassL1 <= 0.98 && Diamond_cut;

      Double_t CUTSIMCL1 = hsdeltaL1 > SIMC_Delta_Low_H && hsdeltaL1 < SIMC_Delta_High_H && hsxptarL1 > SIMC_Xptar_Low_H && hsxptarL1 < SIMC_Xptar_High_H && hsyptarL1 > SIMC_Yptar_Low_H && hsyptarL1 < SIMC_Yptar_High_H && ssdeltaL1 > SIMC_Delta_Low_P && ssdeltaL1 < SIMC_Delta_High_P && ssxptarL1 > SIMC_Xptar_Low_P && ssxptarL1 < SIMC_Xptar_High_P && ssyptarL1 > SIMC_Yptar_Low_P && ssyptarL1 < SIMC_Yptar_High_P && missmassL1 >=MMPICUTL && missmassL1 <=MMPICUTH && Diamond_cut;

      Double_t CUTSIMCL12 = hsdeltaL1 > SIMC_Delta_Low_H && hsdeltaL1 < SIMC_Delta_High_H && hsxptarL1 > SIMC_Xptar_Low_H && hsxptarL1 < SIMC_Xptar_High_H && hsyptarL1 > SIMC_Yptar_Low_H && hsyptarL1 < SIMC_Yptar_High_H && ssdeltaL1 > SIMC_Delta_Low_P && ssdeltaL1 < SIMC_Delta_High_P && ssxptarL1 > SIMC_Xptar_Low_P && ssxptarL1 < SIMC_Xptar_High_P && ssyptarL1 > SIMC_Yptar_Low_P && ssyptarL1 < SIMC_Yptar_High_P && Diamond_cut;

	//hsdeltaL1 > -8.0 && hsdeltaL1 < 8.0 && hsxptarL1 > -0.08 && hsxptarL1 < 0.08 && hsyptarL1 > -0.045 && hsyptarL1 < 0.045 && ssdeltaL1 > -10.0 && ssdeltaL1 < 20.0 && ssxptarL1 > -0.06 && ssxptarL1 < 0.06 && ssyptarL1 > -0.04 && ssyptarL1 < 0.04 && Diamond_cut;

      if(CUTSIMCL12)
	{
	  MPiSL1->Fill(missmassL1, factL1*WeightL1);
	}

      if(CUTSIMCL1)
	{
	  hph_qSL1->Fill(phipqL1*57.2958, factL1*WeightL1);
	  hphSL1->Fill(phipqL1*57.2958, factL1*WeightL1);
	  //hphSL1->Fill(missmassL1, factL1*WeightL1);
	  hYSL1->Fill(t_simcL1, phipqL1*57.2958, factL1*WeightL1);
	  //  hYSL1->Fill(t_simcL1,  missmassL1, factL1*WeightL1);
	  H_ssdelta_SL1->Fill(ssdeltaL1, factL1*WeightL1);
	  H_ssxptar_SL1->Fill(ssxptarL1, factL1*WeightL1);
	  H_ssyptar_SL1->Fill(ssyptarL1, factL1*WeightL1);
	  
	  H_hsdelta_SL1->Fill(hsdeltaL1, factL1*WeightL1);
	  H_hsxptar_SL1->Fill(hsxptarL1, factL1*WeightL1);
	  H_hsyptar_SL1->Fill(hsyptarL1, factL1*WeightL1);
	  hthtSL1->Fill(t_simcL1, factL1*WeightL1); 
	}
    }
  
  //DATA LEFT2
  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
           
      if(tcoinL2-TimmingOffset>=-1.0 && tcoinL2-TimmingOffset <= 1.0)
	
	{
	  hmmL2->Fill(mmL2);
	}   
      
      {
	htcoinL2->Fill(tcoinL2-TimmingOffset);
      }
      Double_t DiamondL2 = (Dcut->IsInside(Q2L2, WL2));  
      Double_t MMpiL2 = mmL2>= MMPICUTL-MMpiOffsetL2 && mmL2 <=MMPICUTH-MMpiOffsetL2;
      Double_t CoinPionL2 = tcoinL2-TimmingOffset>=-1.0 && tcoinL2-TimmingOffset <= 1.0;      
      Double_t CoinPionL2R = ((tcoinL2-TimmingOffset>=-15.0 && tcoinL2-TimmingOffset<= -9.0) || (tcoinL2-TimmingOffset>=7.0 && tcoinL2-TimmingOffset<=13.0));      
      
      if(CoinPionL2 && DiamondL2)
	{      
	  MPiL2->Fill(mmL2+MMpiOffsetL2);
	}     
      if(CoinPionL2 && MMpiL2 && DiamondL2)
	{
	  H_ssdelta_DL2->Fill(P_gtr_dpL2);
	  H_ssxptar_DL2->Fill(P_gtr_xptarL2);
	  H_ssyptar_DL2->Fill(P_gtr_yptarL2);

	  H_hsdelta_DL2->Fill(H_gtr_dpL2);
	  H_hsxptar_DL2->Fill(H_gtr_xptarL2);
	  H_hsyptar_DL2->Fill(H_gtr_yptarL2);

	  hthtL2->Fill(TMath::Abs(tL2)+tOffsetL2); 
	  htL2->Fill(TMath::Abs(tL2));
 	  htInL2->Fill(TMath::Abs(tL2));
	  hph_qL2->Fill(ph_qL2*57.2958 + 180);
	  hphL2->Fill(std::fmod(ph_qL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  // hphL2->Fill(mmL2+MMpiOffsetL2);
	  hYL2->Fill(TMath::Abs(tL2)+tOffsetL2, std::fmod(ph_qL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYL2->Fill(-tL2, mmL2+MMpiOffsetL2);

	  shleL2->Fill(TMath::Abs(tL2)*cos(ph_qL2), TMath::Abs(tL2)*sin(ph_qL2));	  
	}

      //Random
      if(CoinPionL2R && DiamondL2)
	{
	  MPiL2R->Fill(mmL2+MMpiOffsetL2);
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2)
	{
	  H_ssdelta_DL2R->Fill(P_gtr_dpL2);
	  H_ssxptar_DL2R->Fill(P_gtr_xptarL2);
	  H_ssyptar_DL2R->Fill(P_gtr_yptarL2);

	  H_hsdelta_DL2R->Fill(H_gtr_dpL2);
	  H_hsxptar_DL2R->Fill(H_gtr_xptarL2);
	  H_hsyptar_DL2R->Fill(H_gtr_yptarL2);

	  hthtL2R->Fill(TMath::Abs(tL2)+tOffsetL2); 
	  htL2R->Fill(TMath::Abs(tL2));
 	  htInL2R->Fill(TMath::Abs(tL2));
	  hph_qL2R->Fill(ph_qL2*57.2958 + 180);
	  hphL2R->Fill(std::fmod(ph_qL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  // hphL2R->Fill(mmL2+MMpiOffsetL2);
	  hYL2R->Fill(TMath::Abs(tL2)+tOffsetL2, std::fmod(ph_qL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //  hYL2R->Fill(-tL2, mmL2+MMpiOffsetL2);
	}

      //For kin_aver, Q2, W & p_theta 
      /*      Double_t w1t = -tL2+tOffsetL2 >= bins[1] && -tL2+tOffsetL2 <= bins[2];
      Double_t w2t = -tL2+tOffsetL2 >= bins[2] && -tL2+tOffsetL2 <= bins[3];
      Double_t w3t = -tL2+tOffsetL2 >= bins[3] && -tL2+tOffsetL2 <= bins[4];
      Double_t w4t = -tL2+tOffsetL2 >= bins[4] && -tL2+tOffsetL2 <= bins[5];
      Double_t w5t = -tL2+tOffsetL2 >= bins[5] && -tL2+tOffsetL2 <= bins[6];
      Double_t w6t = -tL2+tOffsetL2 >= bins[6] && -tL2+tOffsetL2 <= bins[7];
      Double_t w7t = -tL2+tOffsetL2 >= bins[7] && -tL2+tOffsetL2 <= bins[8];
      Double_t w8t = -tL2+tOffsetL2 >= bins[8] && -tL2+tOffsetL2 <= bins[9];
      */
      Double_t w1t = TMath::Abs(tL2) >= bins[1] && TMath::Abs(tL2) <= bins[2];
      Double_t w2t = TMath::Abs(tL2) >= bins[2] && TMath::Abs(tL2) <= bins[3];
      Double_t w3t = TMath::Abs(tL2) >= bins[3] && TMath::Abs(tL2) <= bins[4];
      Double_t w4t = TMath::Abs(tL2) >= bins[4] && TMath::Abs(tL2) <= bins[5];
      Double_t w5t = TMath::Abs(tL2) >= bins[5] && TMath::Abs(tL2) <= bins[6];
      Double_t w6t = TMath::Abs(tL2) >= bins[6] && TMath::Abs(tL2) <= bins[7];
      Double_t w7t = TMath::Abs(tL2) >= bins[7] && TMath::Abs(tL2) <= bins[8];
      Double_t w8t = TMath::Abs(tL2) >= bins[8] && TMath::Abs(tL2) <= bins[9];

      
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w1t)
	{
	  hW1L2->Fill(WL2);	
	  hQ21L2->Fill(Q2L2);	      	  
	  ht1L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w2t)
	{
	  hW2L2->Fill(WL2);	
	  hQ22L2->Fill(Q2L2);	      	  
	  ht2L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w3t)
	{
	  hW3L2->Fill(WL2);	
	  hQ23L2->Fill(Q2L2);	      	  
	  ht3L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w4t)
	{
	  hW4L2->Fill(WL2);	
	  hQ24L2->Fill(Q2L2);	      	  
	  ht4L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w5t)
	{
	  hW5L2->Fill(WL2);	
	  hQ25L2->Fill(Q2L2);	      	  
	  ht5L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w6t)
	{
	  hW6L2->Fill(WL2);	
	  hQ26L2->Fill(Q2L2);	      	  
	  ht6L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w7t)
	{
	  hW7L2->Fill(WL2);	
	  hQ27L2->Fill(Q2L2);	      	  
	  ht7L2->Fill(TMath::Abs(tL2)); 
  	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w8t)
	{
	  hW8L2->Fill(WL2);	
	  hQ28L2->Fill(Q2L2);	      	  
	  ht8L2->Fill(TMath::Abs(tL2)); 
  	}	

      //Random
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w1t)
      	{	  
	  hW1L2R->Fill(WL2);	
	  hQ21L2R->Fill(Q2L2);	      	  
	  ht1L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w2t)
      	{	  
	  hW2L2R->Fill(WL2);	
	  hQ22L2R->Fill(Q2L2);	      	  
	  ht2L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w3t)
      	{	  
	  hW3L2R->Fill(WL2);	
	  hQ23L2R->Fill(Q2L2);	      	  
	  ht3L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w4t)
      	{	  
	  hW4L2R->Fill(WL2);	
	  hQ24L2R->Fill(Q2L2);	      	  
	  ht4L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w5t)
      	{	  
	  hW5L2R->Fill(WL2);	
	  hQ25L2R->Fill(Q2L2);	      	  
	  ht5L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w6t)
      	{	  
	  hW6L2R->Fill(WL2);	
	  hQ26L2R->Fill(Q2L2);	      	  
	  ht6L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w7t)
      	{	  
	  hW7L2R->Fill(WL2);	
	  hQ27L2R->Fill(Q2L2);	      	  
	  ht7L2R->Fill(TMath::Abs(tL2)); 
	}      
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w8t)
      	{	  
	  hW8L2R->Fill(WL2);	
	  hQ28L2R->Fill(Q2L2);	      	  
	  ht8L2R->Fill(TMath::Abs(tL2)); 
	}      

      if (((tcoinL2-TimmingOffset>=-15.0 && tcoinL2-TimmingOffset <= -9.0) || (tcoinL2-TimmingOffset>=7.0 && tcoinL2-TimmingOffset <=13.0)))
	{
	  hmmL2R->Fill(mmL2);
	}
    }

  //DATA dummy LEFL2
  for(Long64_t i = 0; i < nEntries_TBRANCHDL2; i++)
    {
      TBRANCHDL2->GetEntry(i);

      Double_t DiamonddL2 = (Dcut->IsInside(Q2dL2, WdL2));  
      Double_t MMpidL2 = mmdL2>= MMPICUTL-MMpiOffsetL2 && mmdL2 <=MMPICUTH-MMpiOffsetL2;
      Double_t CoinPiondL2 = tcoindL2-TimmingOffset>=-1.0 && tcoindL2-TimmingOffset <= 1.0;      
      Double_t CoinPiondL2R = ((tcoindL2-TimmingOffset>=-15.0 && tcoindL2-TimmingOffset<= -9.0) || (tcoindL2-TimmingOffset>=7.0 && tcoindL2-TimmingOffset<=13.0));      

      if(CoinPiondL2 && DiamonddL2)
	{
	  MPidL2->Fill(mmdL2+MMpiOffsetL2);
	}
      if(CoinPiondL2 && MMpidL2 && DiamonddL2)
	{
	  hYdL2->Fill(TMath::Abs(tdL2)+tOffsetL2, std::fmod(ph_qdL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  // hYdL2->Fill(-tdL2, mmdL2+MMpiOffsetL2);
	  hthtdL2->Fill(TMath::Abs(tdL2)+tOffsetL2); 
	}	

      //Random
      Double_t CUTCL2R = ((tcoindL2>=-15.0 && tcoindL2 <= -9.0) || (tcoindL2>=7.0 && tcoindL2<=13.0));      
     
      if(CoinPiondL2R && DiamonddL2)
	{
	  MPidL2R->Fill(mmdL2+MMpiOffsetL2);
	}
      if(CoinPiondL2R && MMpidL2 && DiamonddL2)
	{
	  hYdL2R->Fill(TMath::Abs(tdL2)+tOffsetL2, std::fmod(ph_qdL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYdL2R->Fill(-tdL2, mmdL2+MMpiOffsetL2);
	  hthtdL2R->Fill(TMath::Abs(tdL2)+tOffsetL2); 
	}
    }    
  //SIMC LEFT2
  for(Long64_t i = 0; i < nEntries_TSIMCL2; i++)
    
    {
      TSIMCL2->GetEntry(i);
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcL2, W_simcL2));  
      
      //      Double_t factL2 = 9361430.0/400000.0;
      //After applying simc offsets
      Double_t factL2 = 10307500.0/400000.0;
      //RadiativeStudy
      // Double_t factL2 = 7552280.0/400000.0;

      //      Double_t CUTSIMCL2 = hsdeltaL2 >=-8.0 && hsdeltaL2 <=8.0 && hsxpfpL2 >=-0.08 && hsxpfpL2 <=0.08 && hsypfpL2 >=-0.045 && hsypfpL2 <=0.045 && ssdeltaL2 >=-10.0 && ssdeltaL2 <=20.0 && ssxpfpL2 >=-0.06 && ssxpfpL2 <=0.06 && ssypfpL2 >=-0.04 && ssypfpL2 <=0.04 && missmassL2 >= 0.932 && missmassL2 <= 0.98 && Diamond_cut;

      Double_t CUTSIMCL2 = hsdeltaL2 > SIMC_Delta_Low_H && hsdeltaL2 < SIMC_Delta_High_H && hsxptarL2 > SIMC_Xptar_Low_H && hsxptarL2 < SIMC_Xptar_High_H && hsyptarL2 > SIMC_Yptar_Low_H && hsyptarL2 < SIMC_Yptar_High_H && ssdeltaL2 > SIMC_Delta_Low_P && ssdeltaL2 < SIMC_Delta_High_P && ssxptarL2 > SIMC_Xptar_Low_P && ssxptarL2 < SIMC_Xptar_High_P && ssyptarL2 > SIMC_Yptar_Low_P && ssyptarL2 < SIMC_Yptar_High_P && missmassL2 >=MMPICUTL && missmassL2 <=MMPICUTH && Diamond_cut;

      Double_t CUTSIMCL22 = hsdeltaL2 > SIMC_Delta_Low_H && hsdeltaL2 < SIMC_Delta_High_H && hsxptarL2 > SIMC_Xptar_Low_H && hsxptarL2 < SIMC_Xptar_High_H && hsyptarL2 > SIMC_Yptar_Low_H && hsyptarL2 < SIMC_Yptar_High_H && ssdeltaL2 > SIMC_Delta_Low_P && ssdeltaL2 < SIMC_Delta_High_P && ssxptarL2 > SIMC_Xptar_Low_P && ssxptarL2 < SIMC_Xptar_High_P && ssyptarL2 > SIMC_Yptar_Low_P && ssyptarL2 < SIMC_Yptar_High_P && Diamond_cut;
      //hsdeltaL2 > -8.0 && hsdeltaL2 < 8.0 && hsxptarL2 > -0.08 && hsxptarL2 < 0.08 && hsyptarL2 > -0.045 && hsyptarL2 < 0.045 && ssdeltaL2 > -10.0 && ssdeltaL2 < 20.0 && ssxptarL2 > -0.06 && ssxptarL2 < 0.06 && ssyptarL2 > -0.04 && ssyptarL2 < 0.04 && Diamond_cut;

      if(CUTSIMCL22)
	{
	  MPiSL2->Fill(missmassL2, factL2*WeightL2);
	}
      if(CUTSIMCL2)
	
	{
	  hphSL2->Fill(phipqL2*57.2958, factL2*WeightL2);
	  // hphSL2->Fill(missmassL2, factL2*WeightL2);
	  hph_qSL2->Fill(phipqL2*57.2958, factL2*WeightL2);
	  hYSL2->Fill(t_simcL2, phipqL2*57.2958, factL2*WeightL2);
	  //hYSL2->Fill(t_simcL2, missmassL2, factL2*WeightL2);
	  H_ssdelta_SL2->Fill(ssdeltaL2, factL2*WeightL2);
	  H_ssxptar_SL2->Fill(ssxptarL2, factL2*WeightL2);
	  H_ssyptar_SL2->Fill(ssyptarL2, factL2*WeightL2);
	  
	  H_hsdelta_SL2->Fill(hsdeltaL2, factL2*WeightL2);
	  H_hsxptar_SL2->Fill(hsxptarL2, factL2*WeightL2);
	  H_hsyptar_SL2->Fill(hsyptarL2, factL2*WeightL2);
	  hthtSL2->Fill(t_simcL2, factL2*WeightL2); 
	}
    }
  //////////////////////////
  Double_t RTSCALE = 1.0/6.0;   
  //////////////////////////

  MPiCR->Scale(RTSCALE);
  MPiL1R->Scale(RTSCALE);
  MPiL2R->Scale(RTSCALE);
  //dummy
  MPidCR->Scale(RTSCALE);
  MPidL1R->Scale(RTSCALE);
  MPidL2R->Scale(RTSCALE);

  hthtCR->Scale(RTSCALE);
  hthtL1R->Scale(RTSCALE);
  hthtL2R->Scale(RTSCALE);
  //dummy
  hthtdCR->Scale(RTSCALE);
  hthtdL1R->Scale(RTSCALE);
  hthtdL2R->Scale(RTSCALE);

  //Yield
  hYCR->Scale(RTSCALE);
  hYL1R->Scale(RTSCALE);
  hYL2R->Scale(RTSCALE);
  //dummy
  hYdCR->Scale(RTSCALE);
  hYdL1R->Scale(RTSCALE);
  hYdL2R->Scale(RTSCALE);

  H_ssdelta_DCR->Scale(RTSCALE);
  H_ssxptar_DCR->Scale(RTSCALE);
  H_ssyptar_DCR->Scale(RTSCALE);
  hthQ2CR->Scale(RTSCALE);
  hthWCR->Scale(RTSCALE);	  
  hepCR->Scale(RTSCALE);
  
  hphCR->Scale(RTSCALE);
  hphDummyCR->Scale(RTSCALE);
  
  H_ssdelta_DummyCR->Scale(RTSCALE);
  H_ssxptar_DummyCR->Scale(RTSCALE);
  H_ssyptar_DummyCR->Scale(RTSCALE);
  
  H_ssdelta_DL1R->Scale(RTSCALE);
  H_ssxptar_DL1R->Scale(RTSCALE);
  H_ssyptar_DL1R->Scale(RTSCALE);

  H_ssdelta_DL2R->Scale(RTSCALE);
  H_ssxptar_DL2R->Scale(RTSCALE);
  H_ssyptar_DL2R->Scale(RTSCALE);
  
  H_hsdelta_DCR->Scale(RTSCALE);
  H_hsxptar_DCR->Scale(RTSCALE);
  H_hsyptar_DCR->Scale(RTSCALE);

  H_hsdelta_DummyCR->Scale(RTSCALE);
  H_hsxptar_DummyCR->Scale(RTSCALE);
  H_hsyptar_DummyCR->Scale(RTSCALE);

  hthQ2DummyCR->Scale(RTSCALE);
  hthWDummyCR->Scale(RTSCALE);	  
  hepDummyCR->Scale(RTSCALE);
    
  H_hsdelta_DL1R->Scale(RTSCALE);
  H_hsxptar_DL1R->Scale(RTSCALE);
  H_hsyptar_DL1R->Scale(RTSCALE);

  H_hsdelta_DL2R->Scale(RTSCALE);
  H_hsxptar_DL2R->Scale(RTSCALE);
  H_hsyptar_DL2R->Scale(RTSCALE);
  
  hphL1R->Scale(RTSCALE);
  hphL2R->Scale(RTSCALE);
  hthCR->Scale(RTSCALE);

  hW1CR->Scale(1.0/6.0);  
  hW2CR->Scale(1.0/6.0);  
  hW3CR->Scale(1.0/6.0);  
  hW4CR->Scale(1.0/6.0);  
  hW5CR->Scale(1.0/6.0);  
  hW6CR->Scale(1.0/6.0);  
  hW7CR->Scale(1.0/6.0);  
  hW8CR->Scale(1.0/6.0);  

  hW1L1R->Scale(1.0/6.0);  
  hW2L1R->Scale(1.0/6.0);  
  hW3L1R->Scale(1.0/6.0);  
  hW4L1R->Scale(1.0/6.0);  
  hW5L1R->Scale(1.0/6.0);  
  hW6L1R->Scale(1.0/6.0);  
  hW7L1R->Scale(1.0/6.0);  
  hW8L1R->Scale(1.0/6.0);  

  hW1L2R->Scale(1.0/6.0);  
  hW2L2R->Scale(1.0/6.0);  
  hW3L2R->Scale(1.0/6.0);  
  hW4L2R->Scale(1.0/6.0);  
  hW5L2R->Scale(1.0/6.0);  
  hW6L2R->Scale(1.0/6.0);  
  hW7L2R->Scale(1.0/6.0);  
  hW8L2R->Scale(1.0/6.0);  

  hQ21CR->Scale(1.0/6.0);
  hQ22CR->Scale(1.0/6.0);
  hQ23CR->Scale(1.0/6.0);
  hQ24CR->Scale(1.0/6.0);
  hQ25CR->Scale(1.0/6.0);
  hQ26CR->Scale(1.0/6.0);
  hQ27CR->Scale(1.0/6.0);
  hQ28CR->Scale(1.0/6.0);

  hQ21L1R->Scale(1.0/6.0);
  hQ22L1R->Scale(1.0/6.0);
  hQ23L1R->Scale(1.0/6.0);
  hQ24L1R->Scale(1.0/6.0);
  hQ25L1R->Scale(1.0/6.0);
  hQ26L1R->Scale(1.0/6.0);
  hQ27L1R->Scale(1.0/6.0);
  hQ28L1R->Scale(1.0/6.0);

  hQ21L2R->Scale(1.0/6.0);
  hQ22L2R->Scale(1.0/6.0);
  hQ23L2R->Scale(1.0/6.0);
  hQ24L2R->Scale(1.0/6.0);
  hQ25L2R->Scale(1.0/6.0);
  hQ26L2R->Scale(1.0/6.0);
  hQ27L2R->Scale(1.0/6.0);
  hQ28L2R->Scale(1.0/6.0);

  ht1CR->Scale(1.0/6.0); 
  ht2CR->Scale(1.0/6.0); 
  ht3CR->Scale(1.0/6.0); 
  ht4CR->Scale(1.0/6.0); 
  ht5CR->Scale(1.0/6.0); 
  ht6CR->Scale(1.0/6.0); 
  ht7CR->Scale(1.0/6.0); 
  ht8CR->Scale(1.0/6.0); 

  ht1L1R->Scale(1.0/6.0); 
  ht2L1R->Scale(1.0/6.0); 
  ht3L1R->Scale(1.0/6.0); 
  ht4L1R->Scale(1.0/6.0); 
  ht5L1R->Scale(1.0/6.0); 
  ht6L1R->Scale(1.0/6.0); 
  ht7L1R->Scale(1.0/6.0); 
  ht8L1R->Scale(1.0/6.0); 

  ht1L2R->Scale(1.0/6.0); 
  ht2L2R->Scale(1.0/6.0); 
  ht3L2R->Scale(1.0/6.0); 
  ht4L2R->Scale(1.0/6.0); 
  ht5L2R->Scale(1.0/6.0); 
  ht6L2R->Scale(1.0/6.0); 
  ht7L2R->Scale(1.0/6.0); 
  ht8L2R->Scale(1.0/6.0); 

  hQ2WCR->Scale(1.0/6.0);
  hYCR->Scale(1.0/6.0);
  hYL1R->Scale(1.0/6.0);
  hYL2R->Scale(1.0/6.0);

  hmmCR->Scale(1.0/6.0);
  hmmL1R->Scale(1.0/6.0);
  hmmL2R->Scale(1.0/6.0);

  htCR->Scale(1.0/6.0);
  htL1R->Scale(1.0/6.0);
  htL2R->Scale(1.0/6.0);

  htInCR->Scale(1.0/6.0);
  htInL1R->Scale(1.0/6.0);
  htInL2R->Scale(1.0/6.0);

  hph_qCR->Scale(1.0/6.0);
  hph_qL1R->Scale(1.0/6.0);
  hph_qL2R->Scale(1.0/6.0);

  //Effective charge 
  
  std::ifstream inputFile1("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_lowe.csv");
  double effcharleC;
  inputFile1 >> effcharleC;
  inputFile1.close();
  std::ifstream inputFile2("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_lowe_dummy.csv");
  double effcharleCD;
  inputFile2 >> effcharleCD;
  inputFile2.close();
  
  Double_t ECLEC   = (1.0+0.0228046)/effcharleC;
  Double_t ECLEDC  = (1.0+0.0228046)/effcharleCD;
   
  /*
  Double_t ECLEC   = (1.0)/effcharleC;
  Double_t ECLEDC  = (1.0)/effcharleCD;
  */
  std::ifstream inputFile3("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_lowe.csv");
  double effcharleL1;
  inputFile3 >> effcharleL1;
  inputFile3.close();
  std::ifstream inputFile4("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_lowe_dummy.csv");
  double effcharleL1D;
  inputFile4 >> effcharleL1D;
  inputFile4.close();
  
  Double_t ECLEL1  = (1.0+0.115731)/effcharleL1;
  Double_t ECLEDL1 = (1.0+0.115731)/effcharleL1D;
    
  /*
  Double_t ECLEL1  = (1.0)/effcharleL1;
  Double_t ECLEDL1 = (1.0)/effcharleL1D;
  */
  std::ifstream inputFile5("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_lowe.csv");
  double effcharleL2;
  inputFile5 >> effcharleL2;
  inputFile5.close();
  std::ifstream inputFile6("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_lowe_dummy.csv");
  double effcharleL2D;
  inputFile6 >> effcharleL2D;
  inputFile6.close();
  
  Double_t ECLEL2  = (1.0+0.0859983)/effcharleL2;
  Double_t ECLEDL2 = (1.0+0.0859983)/effcharleL2D;
    
  /*
  Double_t ECLEL2  = (1.0)/effcharleL2;
  Double_t ECLEDL2 = (1.0)/effcharleL2D;
  */
  //SIMC SCAL
  Double_t SCFLEC  = 1.0;  //0.95
  Double_t SCFLEL1 = 1.0;  //0.95
  Double_t SCFLEL2 = 1.0;  //0.95

  //////////////////////////////////////////
  TCanvas *cthesiMPis = new TCanvas("cthesiMPis", "cthesiMPis");
  cthesiMPis->Divide(2,2);
  cthesiMPis->cd(1);
  MPiC->Add(MPiCR, -1);
  MPiC->Scale(ECLEC);
  MPidC->Add(MPidCR, -1);
  MPidC->Scale(ECLEDC);
  MPiC->Add(MPidC, -1);
  //  MPiSC->SetStats(0);
  MPiSC->SetLineColor(kRed);
  MPiSC->Draw("hist");
  //  MPiC->SetStats(0);
  MPiC->Draw("same");

  cthesiMPis->cd(2);
  MPiL1->Add(MPiL1R, -1);
  MPiL1->Scale(ECLEL1);
  MPidL1->Add(MPidL1R, -1);
  MPidL1->Scale(ECLEDL1);
  MPiL1->Add(MPidL1, -1);
  //  MPiSL1->SetStats(0);
  MPiSL1->SetLineColor(kRed);
  MPiSL1->Draw("hist");
  //  MPiL1->SetStats(0);
  MPiL1->Draw("same");

  cthesiMPis->cd(3);
  MPiL2->Add(MPiL2R, -1);
  MPiL2->Scale(ECLEL2);
  MPidL2->Add(MPidL2R, -1);
  MPidL2->Scale(ECLEDL2);
  MPiL2->Add(MPidL2, -1);
  //  MPiSL2->SetStats(0);
  MPiSL2->SetLineColor(kRed);
  MPiSL2->Draw("hist");
  //  MPiL2->SetStats(0);
  MPiL2->Draw("same");
  cthesiMPis->Print(outputpdf + '(');

  TAxis *XPiC = MPiC->GetXaxis();
  Double_t InPiC = MPiC->Integral(XPiC->FindBin(0.91), XPiC->FindBin(0.98), "");
  TAxis *XPiSC = MPiSC->GetXaxis();
  Double_t InPiSC = MPiSC->Integral(XPiSC->FindBin(0.91), XPiSC->FindBin(0.98), "");
  Double_t PiC = (1-InPiC/InPiSC); 
  cout<<" Low epsilon PiC = " << PiC << endl;
  
  TAxis *XPiL1 = MPiL1->GetXaxis();
  Double_t InPiL1 = MPiL1->Integral(XPiL1->FindBin(0.91), XPiL1->FindBin(0.98), "");
  TAxis *XPiSL1 = MPiSL1->GetXaxis();
  Double_t InPiSL1 = MPiSL1->Integral(XPiSL1->FindBin(0.91), XPiSL1->FindBin(0.98), "");
  Double_t PiL1 = (1-InPiL1/InPiSL1); 
  cout<<" Low epsilon PiL1 = " << PiL1 << endl;
  
  TAxis *XPiL2 = MPiL2->GetXaxis();
  Double_t InPiL2 = MPiL2->Integral(XPiL2->FindBin(0.91), XPiL2->FindBin(0.98), "");
  TAxis *XPiSL2 = MPiSL2->GetXaxis();
  Double_t InPiSL2 = MPiSL2->Integral(XPiSL2->FindBin(0.91), XPiSL2->FindBin(0.98), "");
  Double_t PiL2 = (1-InPiL2/InPiSL2); 
  cout<<" Low epsilon PiL2 = " << PiL2 << endl;
   
  TCanvas *cthesis = new TCanvas("cthesis", "cthesis");
  cthesis->Divide(2,2);

  cthesis->cd(1);

  cthesis->cd(2);
  hthtL1->Add(hthtL1R, -1);
  hthtL1->Scale(ECLEL1);
  hthtdL1->Add(hthtdL1R, -1);
  hthtdL1->Scale(ECLEDL1);
  hthtL1->Add(hthtdL1, -1);
  // hthtSL1->SetStats(0);
  hthtSL1->SetLineColor(kRed);
  hthtSL1->Draw("hist");
  // hthtL1->SetStats(0);
  hthtL1->Draw("same");

  cthesis->cd(3);
  hthtL2->Add(hthtL2R, -1);
  hthtL2->Scale(ECLEL2);
  hthtdL2->Add(hthtdL2R, -1);
  hthtdL2->Scale(ECLEDL2);
  hthtL2->Add(hthtdL2, -1);
  // hthtSL2->SetStats(0);
  hthtSL2->SetLineColor(kRed);
  hthtSL2->Draw("hist");
  // hthtL2->SetStats(0);
  hthtL2->Draw("same");
  cthesis->Print(outputpdf);

  TCanvas *c1 = new TCanvas("c1", " c1"); 

  htcoinC->SetStats(0);
  htcoinC->SetLineColor(kGreen);
  //  htcoinC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  //  htcoinC->GetXaxis()->SetTitle("Coincidence time (ns)"); 
  htcoinC->Scale(ECLEC);
  htcoinC->Draw("Weight");

  htcoinL1->SetStats(0);
  htcoinL1->SetLineColor(kRed);
  htcoinL1->Scale(ECLEL1);
  htcoinL1->Draw("same Weight ");

  htcoinL2->SetStats(0);
  htcoinL2->SetLineColor(kBlack);
  htcoinL2->Scale(ECLEL2);
  htcoinL2->Draw("same Weight");

  TLine *l1 = new TLine(-1.0, 0.0, -1.0, 14.0);
  TLine *l2 = new TLine(1.0, 0.0, 1.0, 14.0);
  TLine *l3 = new TLine(-15.0, 0.0, -15.0, 14.0);
  TLine *l4 = new TLine(-9.0, 0.0, -9.0, 14.0);
  TLine *l5 = new TLine(7.0, 0.0, 7.0, 14.0);
  TLine *l6 = new TLine(13.0, 0.0, 13.0, 14.0);

  l1->SetLineColor(kBlue);
  l2->SetLineColor(kBlue);
  l3->SetLineColor(kBlue);
  l4->SetLineColor(kBlue);
  l5->SetLineColor(kBlue);
  l6->SetLineColor(kBlue);

  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");
  l4->Draw("same");
  l5->Draw("same");
  l6->Draw("same");

  auto leg = new TLegend(0.1,0.5,0.30,0.9);
  leg->SetHeader("#epsilon = 0.264","C");
  leg->SetTextSize(0.1);
  leg->AddEntry(htcoinC,"Center","l");
  leg->AddEntry(htcoinL1,"Left1","l");
  leg->AddEntry(htcoinL2,"Left2","l");
  leg->Draw("same");

  c1->Print(outputpdf);

  TCanvas *c3 = new TCanvas("c3", " c3");
 
  htC->SetStats(0);
  htC->SetLineColor(kGreen);
  htC->Add(htCR,-1); 
  htC->Draw("Weight");

  htL1->SetStats(0);
  htL1->SetLineColor(kRed);
  htL1->Add(htL1R, -1); 
  htL1->Draw("same Weight");
 
  htL2->SetStats(0);
  htL2->SetLineColor(kBlack);
  htL2->Add(htL2R, -1); 
  htL2->Draw(" same Weight");

  c3->Print(outputpdf);

  TCanvas *c3In = new TCanvas("c3In", " c3In");
 
  htInL2->SetStats(0);
  htInL2->SetLineColor(kBlack);
  htInL2->Add(htInL2R, -1); 
  htInL2->Draw(" Weight");

  htInC->SetStats(0);
  htInC->SetLineColor(kGreen);
  htInC->Add(htInCR,-1); 
  htInC->Draw("same Weight");

  htInL1->SetStats(0);
  htInL1->SetLineColor(kRed);
  htInL1->Add(htInL1R, -1); 
  htInL1->Draw("same Weight");
 
  c3In->Print(outputpdf);

  TCanvas *cWC1 = new TCanvas("cWC1", "cWC1");
  cWC1->Divide(2, 2);
  cWC1->cd(1);
  hW1C->Add(hW1CR, -1);
  hW1C->Draw("Weight");
  cWC1->cd(2);
  hW2C->Add(hW2CR, -1);
  hW2C->Draw(" same Weight");
  cWC1->cd(3);
  hW3C->Add(hW3CR, -1);
  hW3C->Draw(" same Weight");
  cWC1->cd(4);
  hW4C->Add(hW4CR, -1);
  hW4C->Draw(" same Weight");
  cWC1->Print(outputpdf);
  
  TCanvas *cWC2 = new TCanvas("cWC2", "cWC2");
  cWC2->Divide(2, 2);
  cWC2->cd(1);
  hW5C->Add(hW5CR, -1);
  hW5C->Draw("Weight");
  cWC2->cd(2);
  hW6C->Add(hW6CR, -1);
  hW6C->Draw(" same Weight");
  cWC2->cd(3);
  hW7C->Add(hW7CR, -1);
  hW7C->Draw(" same Weight");
  cWC2->cd(4);
  hW8C->Add(hW8CR, -1);
  hW8C->Draw(" same Weight");
  cWC2->Print(outputpdf);

  TCanvas *cWL11 = new TCanvas("cWL11", "cWL11");
  cWL11->Divide(2, 2);
  cWL11->cd(1);
  hW1L1->Add(hW1L1R, -1);
  hW1L1->Draw("Weight");
  cWL11->cd(2);
  hW2L1->Add(hW2L1R, -1);
  hW2L1->Draw(" same Weight");
  cWL11->cd(3);
  hW3L1->Add(hW3L1R, -1);
  hW3L1->Draw(" same Weight");
  cWL11->cd(4);
  hW4L1->Add(hW4L1R, -1);
  hW4L1->Draw(" same Weight");
  cWL11->Print(outputpdf);
  
  TCanvas *cWL12 = new TCanvas("cWL12", "cWL12");
  cWL12->Divide(2, 2);
  cWL12->cd(1);
  hW5L1->Add(hW5L1R, -1);
  hW5L1->Draw("Weight");
  cWL12->cd(2);
  hW6L1->Add(hW6L1R, -1);
  hW6L1->Draw(" same Weight");
  cWL12->cd(3);
  hW7L1->Add(hW7L1R, -1);
  hW7L1->Draw(" same Weight");
  cWL12->cd(4);
  hW8L1->Add(hW8L1R, -1);
  hW8L1->Draw(" same Weight");
  cWL12->Print(outputpdf);

  TCanvas *cWL21 = new TCanvas("cWL21", "cWL21");
  cWL21->Divide(2, 2);
  cWL21->cd(1);
  hW1L2->Add(hW1L2R, -1);
  hW1L2->Draw("Weight");
  cWL21->cd(2);
  hW2L2->Add(hW2L2R, -1);
  hW2L2->Draw(" same Weight");
  cWL21->cd(3);
  hW3L2->Add(hW3L2R, -1);
  hW3L2->Draw(" same Weight");
  cWL21->cd(4);
  hW4L2->Add(hW4L2R, -1);
  hW4L2->Draw(" same Weight");
  cWL21->Print(outputpdf);
  
  TCanvas *cWL22 = new TCanvas("cWL22", "cWL22");
  cWL22->Divide(2, 2);
  cWL22->cd(1);
  hW5L2->Add(hW5L2R, -1);
  hW5L2->Draw("Weight");
  cWL22->cd(2);
  hW6L2->Add(hW6L2R, -1);
  hW6L2->Draw(" same Weight");
  cWL22->cd(3);
  hW7L2->Add(hW7L2R, -1);
  hW7L2->Draw(" same Weight");
  cWL22->cd(4);
  hW8L2->Add(hW8L2R, -1);
  hW8L2->Draw(" same Weight");
  cWL22->Print(outputpdf);

  TCanvas *cQ2C1 = new TCanvas("cQ2C1", "cQ2C1");

  cQ2C1->Divide(2, 2);
  cQ2C1->cd(1);
  hQ21C->Add(hQ21CR, -1); 
  hQ21C->Draw(" Weight");
  cQ2C1->cd(2);
  hQ22C->Add(hQ22CR, -1); 
  hQ22C->Draw("same Weight");
  cQ2C1->cd(3);
  hQ23C->Add(hQ23CR, -1); 
  hQ23C->Draw("same Weight");
  cQ2C1->cd(4);
  hQ24C->Add(hQ24CR, -1); 
  hQ24C->Draw("same Weight");
  cQ2C1->Print(outputpdf);

  TCanvas *cQ2C2 = new TCanvas("cQ2C2", "cQ2C2");

  cQ2C2->Divide(2, 2);
  cQ2C2->cd(1);
  hQ25C->Add(hQ25CR, -1); 
  hQ25C->Draw(" Weight");
  cQ2C2->cd(2);
  hQ26C->Add(hQ26CR, -1); 
  hQ26C->Draw("same Weight");
  cQ2C2->cd(3);
  hQ27C->Add(hQ27CR, -1); 
  hQ27C->Draw("same Weight");
  cQ2C2->cd(4);
  hQ28C->Add(hQ28CR, -1); 
  hQ28C->Draw("same Weight");
  cQ2C2->Print(outputpdf);

  TCanvas *cQ2L11 = new TCanvas("cQ2L11", "cQ2L11");

  cQ2L11->Divide(2, 2);
  cQ2L11->cd(1);
  hQ21L1->Add(hQ21L1R, -1); 
  hQ21L1->Draw(" Weight");
  cQ2L11->cd(2);
  hQ22L1->Add(hQ22L1R, -1); 
  hQ22L1->Draw("same Weight");
  cQ2L11->cd(3);
  hQ23L1->Add(hQ23L1R, -1); 
  hQ23L1->Draw("same Weight");
  cQ2L11->cd(4);
  hQ24L1->Add(hQ24L1R, -1); 
  hQ24L1->Draw("same Weight");
  cQ2L11->Print(outputpdf);

  TCanvas *cQ2L12 = new TCanvas("cQ2L12", "cQ2L12");

  cQ2L12->Divide(2, 2);
  cQ2L12->cd(1);
  hQ25L1->Add(hQ25L1R, -1); 
  hQ25L1->Draw(" Weight");
  cQ2L12->cd(2);
  hQ26L1->Add(hQ26L1R, -1); 
  hQ26L1->Draw("same Weight");
  cQ2L12->cd(3);
  hQ27L1->Add(hQ27L1R, -1); 
  hQ27L1->Draw("same Weight");
  cQ2L12->cd(4);
  hQ28L1->Add(hQ28L1R, -1); 
  hQ28L1->Draw("same Weight");
  cQ2L12->Print(outputpdf);

  TCanvas *cQ2L21 = new TCanvas("cQ2L21", "cQ2L21");

  cQ2L21->Divide(2, 2);
  cQ2L21->cd(1);
  hQ21L2->Add(hQ21L2R, -1); 
  hQ21L2->Draw(" Weight");
  cQ2L21->cd(2);
  hQ22L2->Add(hQ22L2R, -1); 
  hQ22L2->Draw("same Weight");
  cQ2L21->cd(3);
  hQ23L2->Add(hQ23L2R, -1); 
  hQ23L2->Draw("same Weight");
  cQ2L21->cd(4);
  hQ24L2->Add(hQ24L2R, -1); 
  hQ24L2->Draw("same Weight");
  cQ2L21->Print(outputpdf);

  TCanvas *cQ2L22 = new TCanvas("cQ2L22", "cQ2L22");

  cQ2L22->Divide(2, 2);
  cQ2L22->cd(1);
  hQ25L2->Add(hQ25L2R, -1); 
  hQ25L2->Draw(" Weight");
  cQ2L22->cd(2);
  hQ26L2->Add(hQ26L2R, -1); 
  hQ26L2->Draw("same Weight");
  cQ2L22->cd(3);
  hQ27L2->Add(hQ27L2R, -1); 
  hQ27L2->Draw("same Weight");
  cQ2L22->cd(4);
  hQ28L2->Add(hQ28L2R, -1); 
  hQ28L2->Draw("same Weight");
  cQ2L22->Print(outputpdf);

  TCanvas *ctC1 = new TCanvas("ctC1", "ctC1");
  ctC1->Divide(2, 2);
  ctC1->cd(1);
  ht1C->Add(ht1CR, -1);
  ht1C->Draw("Weight");
  ctC1->cd(2);
  ht2C->Add(ht2CR, -1);
  ht2C->Draw("same Weight");
  ctC1->cd(3);
  ht3C->Add(ht3CR, -1);
  ht3C->Draw("same Weight");
  ctC1->cd(4);
  ht4C->Add(ht4CR, -1);
  ht4C->Draw("same Weight");
  ctC1->Print(outputpdf);

  TCanvas *ctC2 = new TCanvas("ctC2", "ctC2");
  ctC2->Divide(2, 2);
  ctC2->cd(1);
  ht5C->Add(ht5CR, -1);
  ht5C->Draw("Weight");
  ctC2->cd(2);
  ht6C->Add(ht6CR, -1);
  ht6C->Draw("same Weight");
  ctC2->cd(3);
  ht7C->Add(ht7CR, -1);
  ht7C->Draw("same Weight");
  ctC2->cd(4);
  ht8C->Add(ht8CR, -1);
  ht8C->Draw("same Weight");
  ctC2->Print(outputpdf);

  TCanvas *ctL11 = new TCanvas("ctL11", "ctL11");
  ctL11->Divide(2, 2);
  ctL11->cd(1);
  ht1L1->Add(ht1L1R, -1);
  ht1L1->Draw("Weight");
  ctL11->cd(2);
  ht2L1->Add(ht2L1R, -1);
  ht2L1->Draw("same Weight");
  ctL11->cd(3);
  ht3L1->Add(ht3L1R, -1);
  ht3L1->Draw("same Weight");
  ctL11->cd(4);
  ht4L1->Add(ht4L1R, -1);
  ht4L1->Draw("same Weight");
  ctL11->Print(outputpdf);

  TCanvas *ctL12 = new TCanvas("ctL12", "ctL12");
  ctL12->Divide(2, 2);
  ctL12->cd(1);
  ht5L1->Add(ht5L1R, -1);
  ht5L1->Draw("Weight");
  ctL12->cd(2);
  ht6L1->Add(ht6L1R, -1);
  ht6L1->Draw("same Weight");
  ctL12->cd(3);
  ht7L1->Add(ht7L1R, -1);
  ht7L1->Draw("same Weight");
  ctL12->cd(4);
  ht8L1->Add(ht8L1R, -1);
  ht8L1->Draw("same Weight");
  ctL12->Print(outputpdf);

  TCanvas *ctL21 = new TCanvas("ctL21", "ctL21");
  ctL21->Divide(2, 2);
  ctL21->cd(1);
  ht1L2->Add(ht1L2R, -1);
  ht1L2->Draw("Weight");
  ctL21->cd(2);
  ht2L2->Add(ht2L2R, -1);
  ht2L2->Draw("same Weight");
  ctL21->cd(3);
  ht3L2->Add(ht3L2R, -1);
  ht3L2->Draw("same Weight");
  ctL21->cd(4);
  ht4L2->Add(ht4L2R, -1);
  ht4L2->Draw("same Weight");
  ctL21->Print(outputpdf);

  TCanvas *ctL22 = new TCanvas("ctL22", "ctL22");
  ctL22->Divide(2, 2);
  ctL22->cd(1);
  ht5L2->Add(ht5L2R, -1);
  ht5L2->Draw("Weight");
  ctL22->cd(2);
  ht6L2->Add(ht6L2R, -1);
  ht6L2->Draw("same Weight");
  ctL22->cd(3);
  ht7L2->Add(ht7L2R, -1);
  ht7L2->Draw("same Weight");
  ctL22->cd(4);
  ht8L2->Add(ht8L2R, -1);
  ht8L2->Draw("same Weight");
  ctL22->Print(outputpdf);

  /**
     ofstream outmeC("kindata/kindata.pl_375_629_+0000.dat");
     ofstream outmeL1("kindata/kindata.pl_375_629_-2000.dat");
     ofstream outmeL2("kindata/kindata.pl_375_629_-4000.dat");
     ofstream outmeR1("kindata/kindata.pl_375_629_+2000.dat");
     ofstream outmeR2("kindata/kindata.pl_375_629_+3120.dat");

     ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
     ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
     ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
     ofstream outheR1("kindata/kindata.pl_375_781_+2680.dat");
  **/

  ofstream outleC("kindata/kindata.pl_425_264_+0000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outleC<<1.000<<endl;
      if(i==1)
	outleC<<hW1C->GetMean()<<"\t"<<hW1C->GetMeanError()<<"\t"<<hQ21C->GetMean()<<"\t"<<hQ21C->GetMeanError()<<"\t"<<ht1C->GetMean()<<"\t"<<ht1C->GetMeanError()<<"\t"<< i <<endl;
      if(i==2)
	outleC<<hW2C->GetMean()<<"\t"<<hW2C->GetMeanError()<<"\t"<<hQ22C->GetMean()<<"\t"<< hQ22C->GetMeanError()<<"\t"<<ht2C->GetMean()<<"\t"<<ht2C->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outleC<<hW3C->GetMean()<<"\t"<<hW3C->GetMeanError()<<"\t"<<hQ23C->GetMean()<<"\t"<< hQ23C->GetMeanError()<<"\t"<<ht3C->GetMean()<<"\t"<<ht3C->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outleC<<hW4C->GetMean()<<"\t"<<hW4C->GetMeanError()<<"\t"<<hQ24C->GetMean()<<"\t"<< hQ24C->GetMeanError()<<"\t"<<ht4C->GetMean()<<"\t"<<ht4C->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outleC<<hW5C->GetMean()<<"\t"<<hW5C->GetMeanError()<<"\t"<<hQ25C->GetMean()<<"\t"<< hQ25C->GetMeanError()<<"\t"<<ht5C->GetMean()<<"\t"<<ht5C->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outleC<<hW6C->GetMean()<<"\t"<<hW6C->GetMeanError()<<"\t"<<hQ26C->GetMean()<<"\t"<< hQ26C->GetMeanError()<<"\t"<<ht6C->GetMean()<<"\t"<<ht6C->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outleC<<hW7C->GetMean()<<"\t"<<hW7C->GetMeanError()<<"\t"<<hQ27C->GetMean()<<"\t"<< hQ27C->GetMeanError()<<"\t"<<ht7C->GetMean()<<"\t"<<ht7C->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outleC<<hW8C->GetMean()<<"\t"<<hW8C->GetMeanError()<<"\t"<<hQ28C->GetMean()<<"\t"<< hQ28C->GetMeanError()<<"\t"<<ht8C->GetMean()<<"\t"<<ht8C->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outleC<<1.000000<<endl;
    } 
  outleC.close();

  ofstream outleL1("kindata/kindata.pl_425_264_-2050.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outleL1<<1.000000<<endl;
      if(i==1)
	outleL1<<hW1L1->GetMean()<<"\t"<<hW1L1->GetMeanError()<<"\t"<<hQ21L1->GetMean()<<"\t"<<hQ21L1->GetMeanError()<<"\t"<<ht1L1->GetMean()<<"\t"<<ht1L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==2)
	outleL1<<hW2L1->GetMean()<<"\t"<<hW2L1->GetMeanError()<<"\t"<<hQ22L1->GetMean()<<"\t"<< hQ22L1->GetMeanError()<<"\t"<<ht2L1->GetMean()<<"\t"<<ht2L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outleL1<<hW3L1->GetMean()<<"\t"<<hW3L1->GetMeanError()<<"\t"<<hQ23L1->GetMean()<<"\t"<< hQ23L1->GetMeanError()<<"\t"<<ht3L1->GetMean()<<"\t"<<ht3L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outleL1<<hW4L1->GetMean()<<"\t"<<hW4L1->GetMeanError()<<"\t"<<hQ24L1->GetMean()<<"\t"<< hQ24L1->GetMeanError()<<"\t"<<ht4L1->GetMean()<<"\t"<<ht4L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outleL1<<hW5L1->GetMean()<<"\t"<<hW5L1->GetMeanError()<<"\t"<<hQ25L1->GetMean()<<"\t"<< hQ25L1->GetMeanError()<<"\t"<<ht5L1->GetMean()<<"\t"<<ht5L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outleL1<<hW6L1->GetMean()<<"\t"<<hW6L1->GetMeanError()<<"\t"<<hQ26L1->GetMean()<<"\t"<< hQ26L1->GetMeanError()<<"\t"<<ht6L1->GetMean()<<"\t"<<ht6L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outleL1<<hW7L1->GetMean()<<"\t"<<hW7L1->GetMeanError()<<"\t"<<hQ27L1->GetMean()<<"\t"<< hQ27L1->GetMeanError()<<"\t"<<ht7L1->GetMean()<<"\t"<<ht7L1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outleL1<<hW8L1->GetMean()<<"\t"<<hW8L1->GetMeanError()<<"\t"<<hQ28L1->GetMean()<<"\t"<< hQ28L1->GetMeanError()<<"\t"<<ht8L1->GetMean()<<"\t"<<ht8L1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outleL1<<1.000000<<endl;
    } 
  outleL1.close();

  ofstream outleL2("kindata/kindata.pl_425_264_-4040.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outleL2<<1.000000<<endl;
      if(i==1)
	outleL2<<hW1L2->GetMean()<<"\t"<<hW1L2->GetMeanError()<<"\t"<<hQ21L2->GetMean()<<"\t"<<hQ21L2->GetMeanError()<<"\t"<<ht1L2->GetMean()<<"\t"<<ht1L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==2)
	outleL2<<hW2L2->GetMean()<<"\t"<<hW2L2->GetMeanError()<<"\t"<<hQ22L2->GetMean()<<"\t"<< hQ22L2->GetMeanError()<<"\t"<<ht2L2->GetMean()<<"\t"<<ht2L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outleL2<<hW3L2->GetMean()<<"\t"<<hW3L2->GetMeanError()<<"\t"<<hQ23L2->GetMean()<<"\t"<< hQ23L2->GetMeanError()<<"\t"<<ht3L2->GetMean()<<"\t"<<ht3L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outleL2<<hW4L2->GetMean()<<"\t"<<hW4L2->GetMeanError()<<"\t"<<hQ24L2->GetMean()<<"\t"<< hQ24L2->GetMeanError()<<"\t"<<ht4L2->GetMean()<<"\t"<<ht4L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outleL2<<hW5L2->GetMean()<<"\t"<<hW5L2->GetMeanError()<<"\t"<<hQ25L2->GetMean()<<"\t"<< hQ25L2->GetMeanError()<<"\t"<<ht5L2->GetMean()<<"\t"<<ht5L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outleL2<<hW6L2->GetMean()<<"\t"<<hW6L2->GetMeanError()<<"\t"<<hQ26L2->GetMean()<<"\t"<< hQ26L2->GetMeanError()<<"\t"<<ht6L2->GetMean()<<"\t"<<ht6L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outleL2<<hW7L2->GetMean()<<"\t"<<hW7L2->GetMeanError()<<"\t"<<hQ27L2->GetMean()<<"\t"<< hQ27L2->GetMeanError()<<"\t"<<ht7L2->GetMean()<<"\t"<<ht7L2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outleL2<<hW8L2->GetMean()<<"\t"<<hW8L2->GetMeanError()<<"\t"<<hQ28L2->GetMean()<<"\t"<< hQ28L2->GetMeanError()<<"\t"<<ht8L2->GetMean()<<"\t"<<ht8L2->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outleL2<<1.000000<<endl;
    } 
  outleL2.close();

  TCanvas *c4 = new TCanvas("c4", " c4"); 
  hph_qL2->SetStats(0);
  hph_qL2->SetLineColor(kBlack);
  hph_qL2->Add(hph_qL2R, -1);
  hph_qL2->Scale(ECLEL2);
  hph_qL2->Draw(" Weight");
  hph_qSL2->SetStats(0);
  //  hph_qSL2->Draw("same Weight");

  hph_qL1->SetStats(0);
  hph_qL1->SetLineColor(kRed);
  hph_qL1->Add(hph_qL1R, -1);
  hph_qL1->Scale(ECLEL1);
  hph_qL1->Draw("same Weight");
  hph_qSL1->SetStats(0);
  //  hph_qSL1->Draw("same Weight");

  hph_qC->SetStats(0);
  hph_qC->SetLineColor(kGreen);
  hph_qC->Add(hph_qCR, -1);
  hph_qC->Scale(ECLEC);
  hph_qC->Draw("same Weight");
  hph_qSC->SetStats(0);
  //  hph_qSC->Draw("same Weight");
  c4->Print(outputpdf);

  TCanvas *phL2 = new TCanvas("phL2", "phL2"); 
  // hphSL2->SetStats(0);
  hphSL2->Scale(SCFLEL2);
  hphSL2->Draw("");
  //  hphL2->SetStats(0);
  hphL2->SetLineColor(kGreen);
  hphL2->Add(hphL2R, -1);
  hphL2->Scale(ECLEL2);
  hphL2->Draw("same");
  phL2->Print(outputpdf);

  TCanvas *phRL2 = new TCanvas("phRL2", " phRL2"); 
  hphL2->Divide(hphSL2);
  hphL2->Draw();
  phRL2->Print(outputpdf);

  TCanvas *phL1 = new TCanvas("phL1", "phL1"); 
  //  hphSL1->SetStats(0);
  hphSL1->Scale(SCFLEL1);
  hphSL1->Draw("");
  //  hphL1->SetStats(0);
  hphL1->SetLineColor(kGreen);
  hphL1->Add(hphL1R, -1);
  hphL1->Scale(ECLEL1);
  hphL1->Draw("same");
  phL1->Print(outputpdf);

  TCanvas *phRL1 = new TCanvas("phRL1", " phRL1"); 
  hphL1->Divide(hphSL1);
  hphL1->Draw();
  phRL1->Print(outputpdf);

  TCanvas *thC = new TCanvas("thC", "thC"); 
  hthSC->SetStats(0);
  hthSC->Draw("");
  hthC->SetStats(0);
  hthC->SetLineColor(kGreen);
  hthC->Add(hthCR, -1);
  hthC->Scale(ECLEC);
  hthC->Draw("same");
  thC->Print(outputpdf);
 
  TCanvas *c5 = new TCanvas("c5", " c5"); 
  hmmL2->SetStats(0);
  hmmL2->SetLineColor(kBlack);
  hmmL2->Add(hmmL2R, -1);
  hmmL2->Draw(" Weight");

  hmmL1->SetStats(0);
  hmmL1->SetLineColor(kRed);
  hmmL1->Add(hmmL1R, -1);
  hmmL1->Draw("same Weight");

  hmmC->SetStats(0);
  hmmC->SetLineColor(kGreen);
  hmmC->Add(hmmCR, -1);
  hmmC->Draw("same Weight");

  TLine *l7 = new TLine(0.92, -100.0, 0.92, 2200.0);
  TLine *l8 = new TLine(0.98, -100.0, 0.98, 2200.0);
  l7->SetLineColor(kBlue);
  l8->SetLineColor(kBlue);
  l7->Draw("same"); 
  l8->Draw("same");

  c5->Print(outputpdf);

  TCanvas *c6 = new TCanvas("c6", " c6");

  c6->Divide(2,0,0.05);
  c6->cd(1);
  Dcut->SetLineColor(kRed);
  // hQ2WC->Add(hQ2WCR, -1);
  // hQ2WC->SetStats(0);
  hQ2WC->Draw("COL");
  Dcut->Draw("same");  
  c6->cd(2);
  // hQ2WC1->SetStats(0);
  Dcut->SetLineColor(kRed);
  hQ2WC1->Draw("COL");
  Dcut->Draw("same");
  //c6->SetLeftMargin(0.15);   
  c6->Print(outputpdf);
  TCanvas *cS6 = new TCanvas("cS6", " cS6");
  // cS6->Divide(2,0,0.05);
  // cS6->cd(1);
  Dcut->SetLineColor(kRed);
  hQ2WSC->Draw("COL");
  Dcut->Draw("same");  
  // cS6->cd(2);
  // Dcut->SetLineColor(kRed);
  // hQ2WSC1->Draw("COL");
  //Dcut->Draw("same");  
  cS6->Print(outputpdf);

  //Plot Added Nov 18, 2025
  TCanvas *clowepC = new TCanvas("clowepC", " clowepC");
  clowepC->Divide(3, 2, 0.01, 0.01);
  TLatex *tlepC = new TLatex(0.33,0.5,"Low Epsilon Centre Setting");
  tlepC->SetNDC();
  tlepC->SetTextSize(0.04);
  tlepC->SetTextColor(kBlue);
  tlepC->Draw();
  clowepC->cd(1);
  H_ssdelta_SC->GetYaxis()->SetTitle("Yield (Events/mC)");
  //  H_ssdelta_SC->Scale(SCFLEC);
  H_ssdelta_SC->SetStats(0);
  H_ssdelta_SC->Draw("same");
  H_ssdelta_DC->Add(H_ssdelta_DCR, -1);
  H_ssdelta_DC->Scale(ECLEC);
  H_ssdelta_DummyC->Add(H_ssdelta_DummyCR, -1);
  H_ssdelta_DummyC->Scale(ECLEDC);
  H_ssdelta_DC->Add(H_ssdelta_DummyC, -1);
  H_ssdelta_DC->SetLineColor(kGreen);
  H_ssdelta_DC->Draw("same"); 
  clowepC->cd(2);
  H_ssxptar_SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SC->SetStats(0);
  H_ssxptar_SC->Scale(SCFLEC);
  H_ssxptar_SC->Draw("same");
  H_ssxptar_DC->Add(H_ssxptar_DCR, -1);
  H_ssxptar_DC->Scale(ECLEC);
  H_ssxptar_DummyC->Add(H_ssxptar_DummyCR, -1);
  H_ssxptar_DummyC->Scale(ECLEDC);
  H_ssxptar_DC->Add(H_ssxptar_DummyC, -1);
  H_ssxptar_DC->SetLineColor(kGreen);
  H_ssxptar_DC->Draw("same");
  clowepC->cd(3);
  H_ssyptar_SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SC->SetStats(0);
  H_ssyptar_SC->Scale(SCFLEC);
  H_ssyptar_SC->Draw("same");
  H_ssyptar_DC->Add( H_ssyptar_DCR, -1);
  H_ssyptar_DC->Scale(ECLEC);
  H_ssyptar_DummyC->Add( H_ssyptar_DummyCR, -1);
  H_ssyptar_DummyC->Scale(ECLEDC);
  H_ssyptar_DC->Add(H_ssyptar_DummyC, -1);
  H_ssyptar_DC->SetLineColor(kGreen);
  H_ssyptar_DC->Draw("same");
  clowepC->cd(4);
  H_hsdelta_SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SC->SetStats(0);
  H_hsdelta_SC->Scale(SCFLEC);
  //  H_hsdelta_SC->Draw("hist");
  H_hsdelta_SC->Draw("same");
  H_hsdelta_DC->Add(H_hsdelta_DCR, -1);
  H_hsdelta_DC->Scale(ECLEC);
  H_hsdelta_DummyC->Add(H_hsdelta_DummyCR, -1);
  H_hsdelta_DummyC->Scale(ECLEDC);
  H_hsdelta_DC->Add(H_hsdelta_DummyC, -1); 
  H_hsdelta_DC->SetLineColor(kGreen);
  H_hsdelta_DC->Draw("same "); 
  clowepC->cd(5);
  H_hsxptar_SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SC->SetStats(0);
  H_hsxptar_SC->Scale(SCFLEC);
  H_hsxptar_SC->Draw("same");
  H_hsxptar_DC->Add(H_hsxptar_DCR, -1);
  H_hsxptar_DC->Scale(ECLEC);
  H_hsxptar_DummyC->Add(H_hsxptar_DummyCR, -1);
  H_hsxptar_DummyC->Scale(ECLEDC);
  H_hsxptar_DC->Add(H_hsxptar_DummyC, -1);  
  H_hsxptar_DC->SetLineColor(kGreen);
  H_hsxptar_DC->Draw("same ");
  clowepC->cd(6);
  H_hsyptar_SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SC->SetStats(0);
  H_hsyptar_SC->Scale(SCFLEC);
  H_hsyptar_SC->Draw("same");
  H_hsyptar_DC->Add(H_hsyptar_DCR, -1);
  H_hsyptar_DC->Scale(ECLEC);
  H_hsyptar_DummyC->Add(H_hsyptar_DummyCR, -1);
  H_hsyptar_DummyC->Scale(ECLEDC);
  H_hsyptar_DC->Add(H_hsyptar_DummyC, -1);
  H_hsyptar_DC->SetLineColor(kGreen);
  H_hsyptar_DC->Draw("same");  
  clowepC->Print(outputpdf);

  //Add kinematic plots
  
  TCanvas *clowep2C = new TCanvas("clowep2C", " clowep2C");
  clowep2C->Divide(3, 2, 0.01, 0.01);
  TLatex *tlep2C = new TLatex(0.33,0.5,"Low Epsilon Centre Setting");
  tlep2C->SetNDC();
  tlep2C->SetTextSize(0.04);
  tlep2C->SetTextColor(kBlue);
  tlep2C->Draw();
  clowep2C->cd(1);
  hthtSC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hthtSC->SetStats(0);
  hthtSC->Draw("same");
  hthtC->Add(hthtCR, -1);
  hthtC->Scale(ECLEC);
  hthtdC->Add(hthtdCR, -1);
  hthtdC->Scale(ECLEDC);
  hthtC->Add(hthtdC, -1);
  hthtC->SetLineColor(kGreen);
  hthtC->Draw("same");

  clowep2C->cd(2);
  hphSC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hphSC->SetStats(0);
  hphSC->Draw("same");
  hphC->Add(hphCR, -1);
  hphC->Scale(ECLEC);
  hphDummyC->Add(hphDummyCR, -1);
  hphDummyC->Scale(ECLEDC);
  hphC->Add(hphDummyC, -1);
  hphC->SetLineColor(kGreen);
  hphC->Draw("same");

  clowep2C->cd(3);
  hthQ2SC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hthQ2SC->SetStats(0);
  hthQ2SC->Draw("same");
  hthQ2C->Add(hthQ2CR, -1);
  hthQ2C->Scale(ECLEC);
  hthQ2DummyC->Add(hthQ2DummyCR, -1);
  hthQ2DummyC->Scale(ECLEDC);
  hthQ2C->Add(hthQ2DummyC, -1);
  hthQ2C->SetLineColor(kGreen);
  hthQ2C->Draw("same");

  clowep2C->cd(4);
  hthWSC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hthWSC->SetStats(0);
  hthWSC->Draw("same");
  hthWC->Add(hthWCR, -1);
  hthWC->Scale(ECLEC);
  hthWDummyC->Add(hthWDummyCR, -1);
  hthWDummyC->Scale(ECLEDC);
  hthWC->Add(hthWDummyC, -1);
  hthWC->SetLineColor(kGreen);
  hthWC->Draw("same");

  clowep2C->cd(5);  
  hepSC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hepSC->SetStats(0);
  hepSC->Draw("same");
  hepC->Add(hepCR, -1);
  hepC->Scale(ECLEC);
  hepDummyC->Add(hepDummyCR, -1);
  hepDummyC->Scale(ECLEDC);
  hepC->Add(hepDummyC, -1);
  hepC->SetLineColor(kGreen);
  hepC->Draw("same");  
  clowep2C->Print(outputpdf);
  
  TCanvas *cdeltaSL1 = new TCanvas("cdeltaSL1", " cdeltaSL1");
  H_ssdelta_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SL1->Scale(SCFLEL1);
  H_ssdelta_SL1->Draw("");
  H_ssdelta_DL1->Add(H_ssdelta_DL1R, -1);
  H_ssdelta_DL1->Scale(ECLEL1);
  H_ssdelta_DL1->SetLineColor(kGreen);
  H_ssdelta_DL1->Draw("same"); 
  cdeltaSL1->Print(outputpdf);

  TCanvas *cdeltaSL2 = new TCanvas("cdeltaSL2", " cdeltaSL2");
  H_ssdelta_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SL2->Scale(SCFLEL2);
  H_ssdelta_SL2->Draw("");
  H_ssdelta_DL2->Add(H_ssdelta_DL2R, -1);
  H_ssdelta_DL2->Scale(ECLEL2);
  H_ssdelta_DL2->SetLineColor(kGreen);
  H_ssdelta_DL2->Draw("same"); 
  cdeltaSL2->Print(outputpdf);

  TCanvas *cdeltaHL1 = new TCanvas("cdeltaHL1", " cdeltaHL1");
  H_hsdelta_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SL1->Scale(SCFLEL1);
  //  H_hsdelta_SL1->Draw("hist");
  H_hsdelta_SL1->Draw("");
  H_hsdelta_DL1->Add(H_hsdelta_DL1R, -1);
  H_hsdelta_DL1->Scale(ECLEL1);
  H_hsdelta_DL1->SetLineColor(kGreen);
  H_hsdelta_DL1->Draw("same"); 
  cdeltaHL1->Print(outputpdf);

  TCanvas *cdeltaHL2 = new TCanvas("cdeltaHL2", " cdeltaHL2");
  H_hsdelta_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SL2->Scale(SCFLEL2);
  //  H_hsdelta_SL2->Draw("hist");
  H_hsdelta_SL2->Draw("");
  H_hsdelta_DL2->Add(H_hsdelta_DL2R, -1);
  H_hsdelta_DL2->Scale(ECLEL2);
  H_hsdelta_DL2->SetLineColor(kGreen);
  H_hsdelta_DL2->Draw("same"); 
  cdeltaHL2->Print(outputpdf);

  TCanvas *cxptarSL1 = new TCanvas("cxptarSL1", " cxptarSL1");
  H_ssxptar_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SL1->Scale(SCFLEL1);
  H_ssxptar_SL1->Draw("");
  H_ssxptar_DL1->Add(H_ssxptar_DL1R, -1);
  H_ssxptar_DL1->Scale(ECLEL1);
  H_ssxptar_DL1->SetLineColor(kGreen);
  H_ssxptar_DL1->Draw("same ");
  cxptarSL1->Print(outputpdf);

  TCanvas *cxptarSL2 = new TCanvas("cxptarSL2", " cxptarSL2");
  H_ssxptar_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SL2->Scale(SCFLEL2);
  H_ssxptar_SL2->Draw("");
  H_ssxptar_DL2->Add(H_ssxptar_DL2R, -1);
  H_ssxptar_DL2->Scale(ECLEL2);
  H_ssxptar_DL2->SetLineColor(kGreen);
  H_ssxptar_DL2->Draw("same");
  cxptarSL2->Print(outputpdf);

  TCanvas *cyptarSL1 = new TCanvas("cyptarSL1", " cyptarSL1");
  H_ssyptar_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SL1->Scale(SCFLEL1);
  H_ssyptar_SL1->Draw("");
  H_ssyptar_DL1->Add( H_ssyptar_DL1R, -1);
  H_ssyptar_DL1->Scale(ECLEL1);
  H_ssyptar_DL1->SetLineColor(kGreen);
  H_ssyptar_DL1->Draw("same ");
  cyptarSL1->Print(outputpdf);

  TCanvas *cyptarSL2 = new TCanvas("cyptarSL2", " cyptarSL2");
  H_ssyptar_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SL2->Scale(SCFLEL2);
  H_ssyptar_SL2->Draw("");
  H_ssyptar_DL2->Add(H_ssyptar_DL2R, -1);
  H_ssyptar_DL2->Scale(ECLEL2);
  H_ssyptar_DL2->SetLineColor(kGreen);
  H_ssyptar_DL2->Draw("same");
  cyptarSL2->Print(outputpdf);

  TCanvas *cxptarHL1 = new TCanvas("cxptarHL1", " cxptarHL1");
  H_hsxptar_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SL1->Scale(SCFLEL1);
  H_hsxptar_SL1->Draw("");
  H_hsxptar_DL1->Add( H_hsxptar_DL1R, -1);
  H_hsxptar_DL1->Scale(ECLEL1);
  H_hsxptar_DL1->SetLineColor(kGreen);
  H_hsxptar_DL1->Draw("same ");
  cxptarHL1->Print(outputpdf);

  TCanvas *cxptarHL2 = new TCanvas("cxptarHL2", " cxptarHL2");
  H_hsxptar_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SL2->Scale(SCFLEL2);
  H_hsxptar_SL2->Draw("");
  H_hsxptar_DL2->Add( H_hsxptar_DL2R, -1);
  H_hsxptar_DL2->Scale(ECLEL2);
  H_hsxptar_DL2->SetLineColor(kGreen);
  H_hsxptar_DL2->Draw("same");
  cxptarHL2->Print(outputpdf);

  TCanvas *cyptarHL1 = new TCanvas("cyptarHL1", " cyptarHL1");
  H_hsyptar_SL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SL1->Scale(SCFLEL1);
  H_hsyptar_SL1->Draw("");
  H_hsyptar_DL1->Add( H_hsyptar_DL1R, -1);
  H_hsyptar_DL1->Scale(ECLEL1);
  H_hsyptar_DL1->SetLineColor(kGreen);
  H_hsyptar_DL1->Draw("same ");
  cyptarHL1->Print(outputpdf);

  TCanvas *cyptarHL2 = new TCanvas("cyptarHL2", " cyptarHL22");
  H_hsyptar_SL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SL2->Scale(SCFLEL2);
  H_hsyptar_SL2->Draw("");
  H_hsyptar_DL2->Add(H_hsyptar_DL2R, -1);
  H_hsyptar_DL2->Scale(ECLEL2);
  H_hsyptar_DL2->SetLineColor(kGreen);
  H_hsyptar_DL2->Draw("same");
  cyptarHL2->Print(outputpdf);

  TCanvas *cY = new TCanvas("cY", " cY");
  hYC->Add(hYCR, -1);  
  hYC->Scale(ECLEC);  
  hYC->SetStats(0);
  hYC->Draw("COLZ");
  cY->Print(outputpdf);

  TCanvas *cYSC = new TCanvas("cYSC", " cYSC");
  hYSC->SetStats(0);
  hYSC->Scale(SCFLEC);
  hYSC->Draw("COLZ");
  cYSC->Print(outputpdf);
  ofstream YleC("yield/yields.pl_425_264_+0000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YleC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YleC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t ECLECError = 0.0;        //fractional error in effective charge
	  Double_t err;
	  Double_t TotNoEvent = hYC->IntegralAndError(j, j, i, i, err, "");
	  Double_t Yield = TotNoEvent*ECLEC;
	  /*	 
		 Double_t YieldError = Yield*sqrt(pow((err/TotNoEvent),2)+ pow((ECLECError),2));    //Error progration in Yield, error in Effective charge is zero for now
		 YleC<<Yield<<"\t"<<YieldError<<"\t"<<j-1<<"\t"<<i-1 <<endl;
	  */
	  //YleC<<hYC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  

	  YleC<<hYC->GetBinContent(j, i)<<"\t"<< hYC->GetBinError(j, i)<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YleC.close();
  hYdC->Add(hYdCR, -1);  
  hYdC->Scale(ECLEDC);   // Yield
  hYdC->Scale(THCF);    // Thickness corrected Yield
  ofstream YdleC("yield/yields.pl_425_264_+0000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdleC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdleC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  //YdleC<<hYdC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	  // YdleC<<0.0<<"\t"<<0.01<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	  YdleC<<hYdC->GetBinContent(j, i)<<"\t"<< hYdC->GetBinError(j, i)<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  

	}
    } 
  YdleC.close();
  ofstream YSleC("yield/yields.pl_425_264_+0000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSleC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSleC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  //	  YSleC<<hYSC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	  YSleC<<hYSC->GetBinContent(j, i)<<"\t"<< hYSC->GetBinError(j, i)<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  

	}
    } 
  YSleC.close();
  
  for(Double_t j = 1; j <= 10; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYC->IntegralAndError(j, j, i, i, err, "");;
	  //	  cout<<Yield <<" +- "<<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  TCanvas *cYL1 = new TCanvas("cYL1", " cYL1");
  hYL1->Add(hYL1R, -1);  
  hYL1->Scale(ECLEL1);  
  hYL1->SetStats(0);
  hYL1->Draw("COLZ");
  cYL1->Print(outputpdf);

  TCanvas *cYSL1 = new TCanvas("cYSL1", " cYSL1");
  hYSL1->SetStats(0);
  hYSL1->Scale(SCFLEL1);
  hYSL1->Draw("COLZ");
  cYSL1->Print(outputpdf);
  ofstream YleL1("yield/yields.pl_425_264_-2050.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YleL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YleL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YleL1<<hYL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YleL1.close();
  hYdL1->Add(hYdL1R, -1);  
  hYdL1->Scale(ECLEDL1);   // Yield  
  hYdL1->Scale(THCF);     // Thickness corrected Yield
  ofstream YdleL1("yield/yields.pl_425_264_-2050.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdleL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdleL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdleL1<<hYdL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	  //YdleL1<<0.0<<"\t"<<0.01<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdleL1.close();
  ofstream YSleL1("yield/yields.pl_425_264_-2050.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSleL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSleL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSleL1<<hYSL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSleL1.close();
 
  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYL1->IntegralAndError(j, j, i, i, err, "");;
	  //	  cout<<Yield <<" +- "<<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  TCanvas *cYL2 = new TCanvas("cYL2", " cYL2");
  hYL2->Add(hYL2R, -1);  
  hYL2->Scale(ECLEL2);  
  hYL2->SetStats(0);
  hYL2->Draw("COLZ");
  cYL2->Print(outputpdf);

  TCanvas *cYSL2 = new TCanvas("cYSL2", " cYSL2");
  hYSL2->SetStats(0);
  hYSL2->Scale(SCFLEL2);
  hYSL2->Draw("COLZ");
  cYSL2->Print(outputpdf);
  ofstream YleL2("yield/yields.pl_425_264_-4040.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YleL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YleL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YleL2<<hYL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YleL2.close();
  hYdL2->Add(hYdL2R, -1);  
  hYdL2->Scale(ECLEDL2);     //Yield  
  hYdL2->Scale(THCF);       // Thickness corrected Yield    
  ofstream YdleL2("yield/yields.pl_425_264_-4040.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdleL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdleL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdleL2<<hYdL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
		  //YdleL2<<0.0<<"\t"<<0.01<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdleL2.close();
  ofstream YSleL2("yield/yields.pl_425_264_-4040.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSleL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSleL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSleL2<<hYSL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSleL2.close();

  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYL2->IntegralAndError(j, j, i, i, err, "");;
	  //	  cout<<Yield <<" +-" <<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  TH2D *Clone = (TH2D*)hYC->Clone("Clone");
  Clone->Add(hYL1,+1);
  Clone->Add(hYL2,+1);
  Clone->SetTitle("Total Yield for low epsilon");
  TCanvas *cYtle = new TCanvas("cYtle", " cYtle");
  Clone->Draw("COLZ");
  cYtle->Print(outputpdf);
  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = Clone->IntegralAndError(j, j, i, i, err, "");;
	  //	  cout<<Yield <<" +-" <<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  
 TCanvas *cRE = new TCanvas("cRE", " cRE");

  TF1 *Gauss = new TF1("Gauss","gaus(0)",-0.004,0.006);
  Gauss->SetLineColor(kRed);
  //  H_t_RE->Fit("Gauss", "RQ"); 
  H_t_RE->SetStats(0);
  H_t_RE->GetYaxis()->SetTitle("Weighted Yield");  
  H_t_RE->Draw("Weight");

  //  auto legRE = new TLegend(0.8,0.7,0.30,0.9); 
  auto legRE = new TLegend(0.1,0.7,0.40,0.9); 
  legRE->SetHeader("SHMS Center Setting (#epsilon = 0.286)","C");
  legRE->SetTextSize(0.03);
  //  legRE->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss->GetParameter(2)), "lep"); 
  legRE->AddEntry(H_t_RE, TString::Format("#sigma = %0.6f",H_t_RE->GetStdDev()), "lep"); 
  legRE->Draw("same");
  cRE->Print(outputpdf);
  
  TCanvas *cREt = new TCanvas("cREt", " cREt");

  H_t_RE_t->SetStats(0);
  H_t_RE_t->Draw("COL");  
  cREt->Print(outputpdf);

  //Here t resolution for each t bin. I've commented out

  /**
     TCanvas *cRE1 = new TCanvas("cRE1", " cRE1");
     TF1 *Gauss1 = new TF1("Gauss1","gaus(0)",-0.004,0.006);
     Gauss1->SetLineColor(kRed);
     H_t_RE1->Fit("Gauss1", "RQ"); 
     H_t_RE1->SetStats(0);
     H_t_RE1->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE1->Draw("Weight");
     auto legRE1 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE1->SetHeader("Center setting at low #epsilon ","C");
     legRE1->SetTextSize(0.03);
     legRE1->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss1->GetParameter(2)), "lep"); 
     legRE1->Draw("same");

     cRE1->Print(outputpdf);

     TCanvas *cRE2 = new TCanvas("cRE2", " cRE2");
     TF1 *Gauss2 = new TF1("Gauss2","gaus(0)",-0.004,0.006);
     Gauss2->SetLineColor(kRed);
     H_t_RE2->Fit("Gauss2", "RQ"); 
     H_t_RE2->SetStats(0);
     H_t_RE2->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE2->Draw("Weight");
     auto legRE2 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE2->SetHeader("Center setting at low #epsilon ","C");
     legRE2->SetTextSize(0.03);
     legRE2->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss2->GetParameter(2)), "lep"); 
     legRE2->Draw("same");

     cRE2->Print(outputpdf);

     TCanvas *cRE3 = new TCanvas("cRE3", " cRE3");
     TF1 *Gauss3 = new TF1("Gauss3","gaus(0)",-0.004,0.006);
     Gauss3->SetLineColor(kRed);
     H_t_RE3->Fit("Gauss3", "RQ"); 
     H_t_RE3->SetStats(0);
     H_t_RE3->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE3->Draw("Weight");
     auto legRE3 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE3->SetHeader("Center setting at low #epsilon ","C");
     legRE3->SetTextSize(0.03);
     legRE3->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss3->GetParameter(2)), "lep"); 
     legRE3->Draw("same");

     cRE3->Print(outputpdf);

     TCanvas *cRE4 = new TCanvas("cRE4", " cRE4");
     TF1 *Gauss4 = new TF1("Gauss4","gaus(0)",-0.004,0.006);
     Gauss4->SetLineColor(kRed);
     H_t_RE4->Fit("Gauss4", "RQ"); 
     H_t_RE4->SetStats(0);
     H_t_RE4->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE4->Draw("Weight");
     auto legRE4 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE4->SetHeader("Center setting at low #epsilon ","C");
     legRE4->SetTextSize(0.03);
     legRE4->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss4->GetParameter(2)), "lep"); 
     legRE4->Draw("same");

     cRE4->Print(outputpdf);

     TCanvas *cRE5 = new TCanvas("cRE5", " cRE5");
     TF1 *Gauss5 = new TF1("Gauss5","gaus(0)",-0.004,0.006);
     Gauss5->SetLineColor(kRed);
     H_t_RE5->Fit("Gauss5", "RQ"); 
     H_t_RE5->SetStats(0);
     H_t_RE5->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE5->Draw("Weight");
     auto legRE5 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE5->SetHeader("Center setting at low #epsilon ","C");
     legRE5->SetTextSize(0.03);
     legRE5->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss5->GetParameter(2)), "lep"); 
     legRE5->Draw("same");

     cRE5->Print(outputpdf);

     TCanvas *cRE6 = new TCanvas("cRE6", " cRE6");
     TF1 *Gauss6 = new TF1("Gauss6","gaus(0)",-0.004,0.006);
     Gauss6->SetLineColor(kRed);
     H_t_RE6->Fit("Gauss6", "RQ"); 
     H_t_RE6->SetStats(0);
     H_t_RE6->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE6->Draw("Weight");
     auto legRE6 = new TLegend(0.1,0.7,0.40,0.9); 
     legRE6->SetHeader("Center setting at low #epsilon ","C");
     legRE6->SetTextSize(0.03);
     legRE6->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss6->GetParameter(2)), "lep"); 
     legRE6->Draw("same");

     cRE6->Print(outputpdf);

     TCanvas *cRE7 = new TCanvas("cRE7", " cRE7");

     H_t_RE7->SetStats(0);
     H_t_RE7->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE7->Draw("Weight");
     cRE7->Print(outputpdf);

     TCanvas *cRE8 = new TCanvas("cRE8", " cRE8");

     H_t_RE8->SetStats(0);
     H_t_RE8->GetYaxis()->SetTitle("Weighted Yield");  
     H_t_RE8->Draw("Weight");
     cRE8->Print(outputpdf);
  **/


  /**
     TCanvas *c7 = new TCanvas("c7", " c7");
   
     auto mg  = new TMultiGraph("mg","Low epsilon");
     auto grC  = new TGraph( nEntries_TBRANCHC, Q2C1, WC1);
     // auto grL1 = new TGraph( nEntries_TBRANCHL1, Q2L11, WL11);
     // auto grL2 = new TGraph (Q2L21, WL21);
     grC->SetName("grC");
     grC->SetTitle("low epsilon");
     grC->SetMarkerStyle(6);
     grC->SetDrawOption("AP");
     grC->SetMarkerColor(3);
  
     grL1->SetName("grL1");
     grL1->SetTitle("low epsilon");
     grL1->SetMarkerStyle(6);
     grL1->SetDrawOption("AP");
     grL1->SetMarkerColor(4);
 
     grL2->SetName("grL2");
     grL2->SetTitle("low epsilon");
     grL2->SetMarkerStyle(6);
     grL2->SetDrawOption("AP");
     grL2->SetMarkerColor(5);
     
     mg->Add(grC);
     // mg->Add(grL1);
     // mg->Add(grL2);
  
     mg->GetXaxis()->SetLimits(0.2,0.6);
     mg->GetYaxis()->SetRangeUser(2.1, 2.3);
     mg->Draw("AP");
     Diamond->Draw("same");  
     c7->Print(outputpdf);
  **/

  //Mid epsilon analysis
  cout<<" "<<endl;
  cout<<" Analysing mid epsilon data..." <<endl;
  cout<<" "<<endl;
  //Data 
  TString TInDATAFilenameMideC  = "Analysed_Data_Q0p425W2p2_mide_Center.root";
  TString TInDATAFilenameMideL1 = "Analysed_Data_Q0p425W2p2_mide_Left1.root";
  TString TInDATAFilenameMideL2 = "Analysed_Data_Q0p425W2p2_mide_Left2.root";
 
  TString rootFile_DATAMIDEC  = ROOTfilePath+"/"+TInDATAFilenameMideC;
  TString rootFile_DATAMIDEL1 = ROOTfilePath+"/"+TInDATAFilenameMideL1;
  TString rootFile_DATAMIDEL2 = ROOTfilePath+"/"+TInDATAFilenameMideL2;
  //Dummy
  TString TInDATAFilenameMidedC = "Analysed_Data_Q0p425W2p2_mide_dummy_Center.root";
  TString TInDATAFilenameMidedL1 = "Analysed_Data_Q0p425W2p2_mide_dummy_Left1.root";
  TString TInDATAFilenameMidedL2 = "Analysed_Data_Q0p425W2p2_mide_dummy_Left2.root";

  TString rootFile_DATAMIDEDC   = ROOTfilePath+"/"+TInDATAFilenameMidedC;
  TString rootFile_DATAMIDEDL1  = ROOTfilePath+"/"+TInDATAFilenameMidedL1;
  TString rootFile_DATAMIDEDL2  = ROOTfilePath+"/"+TInDATAFilenameMidedL2;

  //SIMC                                                  
  TString TInSIMCFilenameMideC  = "Pion_3p6_Q2_center.root";
  TString TInSIMCFilenameMideL1 = "Pion_3p6_Q2_left1.root";
  TString TInSIMCFilenameMideL2 = "Pion_3p6_Q2_left2.root";

  TString rootFile_SIMCMIDEC   = ROOTfilePath+"/"+TInSIMCFilenameMideC;
  TString rootFile_SIMCMIDEL1  = ROOTfilePath+"/"+TInSIMCFilenameMideL1;
  TString rootFile_SIMCMIDEL2  = ROOTfilePath+"/"+TInSIMCFilenameMideL2;


  if (gSystem->AccessPathName(rootFile_DATAMIDEC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAMIDEC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAMIDEL1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAMIDEL1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAMIDEL2) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAMIDEL2 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  //Data  
  TFile *InFile_DATAMIDEC = new TFile(rootFile_DATAMIDEC, "READ");
  TFile *InFile_DATAMIDEL1 = new TFile(rootFile_DATAMIDEL1, "READ");
  TFile *InFile_DATAMIDEL2 = new TFile(rootFile_DATAMIDEL2, "READ");

  //Dummy  
  TFile *InFile_DATAMIDEDC = new TFile(rootFile_DATAMIDEDC, "READ");
  TFile *InFile_DATAMIDEDL1 = new TFile(rootFile_DATAMIDEDL1, "READ");
  TFile *InFile_DATAMIDEDL2 = new TFile(rootFile_DATAMIDEDL2, "READ");

  //SIMC 
  TFile *InFile_SIMCMIDEC  = new TFile(rootFile_SIMCMIDEC,  "READ");
  TFile *InFile_SIMCMIDEL1 = new TFile(rootFile_SIMCMIDEL1, "READ");
  TFile *InFile_SIMCMIDEL2 = new TFile(rootFile_SIMCMIDEL2, "READ");

  // TREES
  //Data
  TTree* TBRANCHMIDEC   = (TTree*)InFile_DATAMIDEC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHMIDEC   = (Long64_t)TBRANCHMIDEC->GetEntries();  
  TTree* TBRANCHMIDEL1  = (TTree*)InFile_DATAMIDEL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEL1  = (Long64_t)TBRANCHMIDEL1->GetEntries();  
  TTree* TBRANCHMIDEL2  = (TTree*)InFile_DATAMIDEL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEL2  = (Long64_t)TBRANCHMIDEL2->GetEntries();  
  //Dummy
  TTree* TBRANCHMIDEDC   = (TTree*)InFile_DATAMIDEDC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHMIDEDC   = (Long64_t)TBRANCHMIDEDC->GetEntries();  
  TTree* TBRANCHMIDEDL1  = (TTree*)InFile_DATAMIDEDL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEDL1  = (Long64_t)TBRANCHMIDEDL1->GetEntries();  
  TTree* TBRANCHMIDEDL2  = (TTree*)InFile_DATAMIDEDL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEDL2  = (Long64_t)TBRANCHMIDEDL2->GetEntries();  
  
  //SIMC 
  TTree* TSIMCMIDEC   = (TTree*)InFile_SIMCMIDEC->Get("h10");Long64_t nEntries_TSIMCMIDEC    = (Long64_t)TSIMCMIDEC->GetEntries();
  TTree* TSIMCMIDEL1  = (TTree*)InFile_SIMCMIDEL1->Get("h10");Long64_t nEntries_TSIMCMIDEL1  = (Long64_t)TSIMCMIDEL1->GetEntries();
  TTree* TSIMCMIDEL2  = (TTree*)InFile_SIMCMIDEL2->Get("h10");Long64_t nEntries_TSIMCMIDEL2  = (Long64_t)TSIMCMIDEL2->GetEntries();
  
  //VARIABLES

  Double_t  P_gtr_dpmeC;TBRANCHMIDEC->SetBranchAddress("ssdelta", &P_gtr_dpmeC);
  Double_t  P_gtr_xptarmeC;TBRANCHMIDEC->SetBranchAddress("ssxptar", &P_gtr_xptarmeC);
  Double_t  P_gtr_yptarmeC;TBRANCHMIDEC->SetBranchAddress("ssyptar", &P_gtr_yptarmeC);

  Double_t  H_gtr_dpmeC;TBRANCHMIDEC->SetBranchAddress("hsdelta", &H_gtr_dpmeC);
  Double_t  H_gtr_xptarmeC;TBRANCHMIDEC->SetBranchAddress("hsxptar", &H_gtr_xptarmeC);
  Double_t  H_gtr_yptarmeC;TBRANCHMIDEC->SetBranchAddress("hsyptar", &H_gtr_yptarmeC);
  //Dummy
  Double_t  P_gtr_dpmedC;TBRANCHMIDEDC->SetBranchAddress("ssdelta", &P_gtr_dpmedC);
  Double_t  P_gtr_xptarmedC;TBRANCHMIDEDC->SetBranchAddress("ssxptar", &P_gtr_xptarmedC);
  Double_t  P_gtr_yptarmedC;TBRANCHMIDEDC->SetBranchAddress("ssyptar", &P_gtr_yptarmedC);

  Double_t  H_gtr_dpmedC;TBRANCHMIDEDC->SetBranchAddress("hsdelta", &H_gtr_dpmedC);
  Double_t  H_gtr_xptarmedC;TBRANCHMIDEDC->SetBranchAddress("hsxptar", &H_gtr_xptarmedC);
  Double_t  H_gtr_yptarmedC;TBRANCHMIDEDC->SetBranchAddress("hsyptar", &H_gtr_yptarmedC);


  Double_t WMEC;TBRANCHMIDEC->SetBranchAddress("W", &WMEC);
  Double_t Q2MEC;TBRANCHMIDEC->SetBranchAddress("Q2", &Q2MEC);
  Double_t epsilonMEC;TBRANCHMIDEC->SetBranchAddress("epsilon", &epsilonMEC);
  //Dummy
  Double_t WMEDC;TBRANCHMIDEDC->SetBranchAddress("W", &WMEDC);
  Double_t Q2MEDC;TBRANCHMIDEDC->SetBranchAddress("Q2", &Q2MEDC);
  Double_t epsilonMEDC;TBRANCHMIDEDC->SetBranchAddress("epsilon", &epsilonMEDC);

  Double_t  P_gtr_dpmeL1;TBRANCHMIDEL1->SetBranchAddress("ssdelta", &P_gtr_dpmeL1);
  Double_t  P_gtr_xptarmeL1;TBRANCHMIDEL1->SetBranchAddress("ssxptar", &P_gtr_xptarmeL1);
  Double_t  P_gtr_yptarmeL1;TBRANCHMIDEL1->SetBranchAddress("ssyptar", &P_gtr_yptarmeL1);

  Double_t  H_gtr_dpmeL1;TBRANCHMIDEL1->SetBranchAddress("hsdelta", &H_gtr_dpmeL1);
  Double_t  H_gtr_xptarmeL1;TBRANCHMIDEL1->SetBranchAddress("hsxptar", &H_gtr_xptarmeL1);
  Double_t  H_gtr_yptarmeL1;TBRANCHMIDEL1->SetBranchAddress("hsyptar", &H_gtr_yptarmeL1);

  Double_t WMEL1;TBRANCHMIDEL1->SetBranchAddress("W", &WMEL1);
  Double_t Q2MEL1;TBRANCHMIDEL1->SetBranchAddress("Q2", &Q2MEL1);
  //Dummy
  Double_t WMEDL1;TBRANCHMIDEDL1->SetBranchAddress("W", &WMEDL1);
  Double_t Q2MEDL1;TBRANCHMIDEDL1->SetBranchAddress("Q2", &Q2MEDL1);

  Double_t  P_gtr_dpmeL2;TBRANCHMIDEL2->SetBranchAddress("ssdelta", &P_gtr_dpmeL2);
  Double_t  P_gtr_xptarmeL2;TBRANCHMIDEL2->SetBranchAddress("ssxptar", &P_gtr_xptarmeL2);
  Double_t  P_gtr_yptarmeL2;TBRANCHMIDEL2->SetBranchAddress("ssyptar", &P_gtr_yptarmeL2);

  Double_t  H_gtr_dpmeL2;TBRANCHMIDEL2->SetBranchAddress("hsdelta", &H_gtr_dpmeL2);
  Double_t  H_gtr_xptarmeL2;TBRANCHMIDEL2->SetBranchAddress("hsxptar", &H_gtr_xptarmeL2);
  Double_t  H_gtr_yptarmeL2;TBRANCHMIDEL2->SetBranchAddress("hsyptar", &H_gtr_yptarmeL2);

  Double_t WMEL2;TBRANCHMIDEL2->SetBranchAddress("W", &WMEL2);
  Double_t Q2MEL2;TBRANCHMIDEL2->SetBranchAddress("Q2", &Q2MEL2);
  //Dummy
  Double_t WMEDL2;TBRANCHMIDEDL2->SetBranchAddress("W", &WMEDL2);
  Double_t Q2MEDL2;TBRANCHMIDEDL2->SetBranchAddress("Q2", &Q2MEDL2);

  Double_t tmeC;TBRANCHMIDEC->SetBranchAddress("MandelT", &tmeC);
  Double_t tmeL1;TBRANCHMIDEL1->SetBranchAddress("MandelT", &tmeL1);
  Double_t tmeL2;TBRANCHMIDEL2->SetBranchAddress("MandelT", &tmeL2);
  //Dummy
  Double_t tmedC;TBRANCHMIDEDC->SetBranchAddress("MandelT", &tmedC);
  Double_t tmedL1;TBRANCHMIDEDL1->SetBranchAddress("MandelT", &tmedL1);
  Double_t tmedL2;TBRANCHMIDEDL2->SetBranchAddress("MandelT", &tmedL2);

  Double_t ph_qmeC;TBRANCHMIDEC->SetBranchAddress("ph_q", &ph_qmeC);
  Double_t ph_qmeL1;TBRANCHMIDEL1->SetBranchAddress("ph_q", &ph_qmeL1);
  Double_t ph_qmeL2;TBRANCHMIDEL2->SetBranchAddress("ph_q", &ph_qmeL2);
  //Dummy
  Double_t ph_qmedC;TBRANCHMIDEDC->SetBranchAddress("ph_q", &ph_qmedC);
  Double_t ph_qmedL1;TBRANCHMIDEDL1->SetBranchAddress("ph_q", &ph_qmedL1);
  Double_t ph_qmedL2;TBRANCHMIDEDL2->SetBranchAddress("ph_q", &ph_qmedL2);

  Double_t mmmeC;TBRANCHMIDEC->SetBranchAddress("MM", &mmmeC);
  Double_t mmmeL1;TBRANCHMIDEL1->SetBranchAddress("MM", &mmmeL1);
  Double_t mmmeL2;TBRANCHMIDEL2->SetBranchAddress("MM", &mmmeL2);
  //Dummy
  Double_t mmmedC;TBRANCHMIDEDC->SetBranchAddress("MM", &mmmedC);
  Double_t mmmedL1;TBRANCHMIDEDL1->SetBranchAddress("MM", &mmmedL1);
  Double_t mmmedL2;TBRANCHMIDEDL2->SetBranchAddress("MM", &mmmedL2);

  Double_t tcoinmeC;TBRANCHMIDEC->SetBranchAddress("CTime_ROC1", &tcoinmeC);
  Double_t tcoinmeL1;TBRANCHMIDEL1->SetBranchAddress("CTime_ROC1", &tcoinmeL1);
  Double_t tcoinmeL2;TBRANCHMIDEL2->SetBranchAddress("CTime_ROC1", &tcoinmeL2);
  //Dummy
  Double_t tcoinmedC;TBRANCHMIDEDC->SetBranchAddress("CTime_ROC1", &tcoinmedC);
  Double_t tcoinmedL1;TBRANCHMIDEDL1->SetBranchAddress("CTime_ROC1", &tcoinmedL1);
  Double_t tcoinmedL2;TBRANCHMIDEDL2->SetBranchAddress("CTime_ROC1", &tcoinmedL2);

  //SIMC VARIABLES
  //CENTER
  //SHMS SIMC variables                                               
  Float_t ssdeltameC;TSIMCMIDEC->SetBranchAddress("ssdelta", &ssdeltameC);
  Float_t ssxptarmeC;TSIMCMIDEC->SetBranchAddress("ssxptar", &ssxptarmeC);
  Float_t ssyptarmeC;TSIMCMIDEC->SetBranchAddress("ssyptar", &ssyptarmeC);
  Float_t ssxfpmeC;TSIMCMIDEC->SetBranchAddress("ssxfp", &ssxfpmeC);
  Float_t ssyfpmeC;TSIMCMIDEC->SetBranchAddress("ssyfp", &ssyfpmeC);
  Float_t ssxpfpmeC;TSIMCMIDEC->SetBranchAddress("ssxpfp", &ssxpfpmeC);
  Float_t ssypfpmeC;TSIMCMIDEC->SetBranchAddress("ssypfp", &ssypfpmeC);

  //HMS SIMC variables 
  Float_t hsdeltameC;TSIMCMIDEC->SetBranchAddress("hsdelta", &hsdeltameC);
  Float_t hsxptarmeC;TSIMCMIDEC->SetBranchAddress("hsxptar", &hsxptarmeC);
  Float_t hsyptarmeC;TSIMCMIDEC->SetBranchAddress("hsyptar", &hsyptarmeC);
  Float_t hsxfpmeC;TSIMCMIDEC->SetBranchAddress("hsxfp", &hsxfpmeC);
  Float_t hsyfpmeC;TSIMCMIDEC->SetBranchAddress("hsyfp", &hsyfpmeC);
  Float_t hsxpfpmeC;TSIMCMIDEC->SetBranchAddress("hsxpfp", &hsxpfpmeC);
  Float_t hsypfpmeC;TSIMCMIDEC->SetBranchAddress("hsypfp", &hsypfpmeC); 

  Float_t qmeC;TSIMCMIDEC->SetBranchAddress("q", &qmeC);
  Float_t Q2_simcmeC;TSIMCMIDEC->SetBranchAddress("Q2", &Q2_simcmeC);
  Float_t W_simcmeC;TSIMCMIDEC->SetBranchAddress("W", &W_simcmeC);
  Float_t t_simcmeC;TSIMCMIDEC->SetBranchAddress("t", &t_simcmeC);
  Float_t ti_simcmeC;TSIMCMIDEC->SetBranchAddress("ti", &ti_simcmeC);
  Float_t epsilon_simcmeC;TSIMCMIDEC->SetBranchAddress("epsilon", &epsilon_simcmeC);
  Float_t missmassmeC;TSIMCMIDEC->SetBranchAddress("missmass", &missmassmeC);
  Float_t EmmeC;TSIMCMIDEC->SetBranchAddress("Em", &EmmeC);
  Float_t PmmeC;TSIMCMIDEC->SetBranchAddress("Pm", &PmmeC);
  Float_t WeightmeC;TSIMCMIDEC->SetBranchAddress("Weight", &WeightmeC);
  Float_t phipqmeC;TSIMCMIDEC->SetBranchAddress("phipq", &phipqmeC);

  //LEFT1
  //SHMS SIMC variables                                               
  Float_t ssdeltameL1;TSIMCMIDEL1->SetBranchAddress("ssdelta", &ssdeltameL1);
  Float_t ssxptarmeL1;TSIMCMIDEL1->SetBranchAddress("ssxptar", &ssxptarmeL1);
  Float_t ssyptarmeL1;TSIMCMIDEL1->SetBranchAddress("ssyptar", &ssyptarmeL1);
  Float_t ssxfpmeL1;TSIMCMIDEL1->SetBranchAddress("ssxfp", &ssxfpmeL1);
  Float_t ssyfpmeL1;TSIMCMIDEL1->SetBranchAddress("ssyfp", &ssyfpmeL1);
  Float_t ssxpfpmeL1;TSIMCMIDEL1->SetBranchAddress("ssxpfp", &ssxpfpmeL1);
  Float_t ssypfpmeL1;TSIMCMIDEL1->SetBranchAddress("ssypfp", &ssypfpmeL1);

  //HMS SIMC variables 
  Float_t hsdeltameL1;TSIMCMIDEL1->SetBranchAddress("hsdelta", &hsdeltameL1);
  Float_t hsxptarmeL1;TSIMCMIDEL1->SetBranchAddress("hsxptar", &hsxptarmeL1);
  Float_t hsyptarmeL1;TSIMCMIDEL1->SetBranchAddress("hsyptar", &hsyptarmeL1);
  Float_t hsxfpmeL1;TSIMCMIDEL1->SetBranchAddress("hsxfp", &hsxfpmeL1);
  Float_t hsyfpmeL1;TSIMCMIDEL1->SetBranchAddress("hsyfp", &hsyfpmeL1);
  Float_t hsxpfpmeL1;TSIMCMIDEL1->SetBranchAddress("hsxpfp", &hsxpfpmeL1);
  Float_t hsypfpmeL1;TSIMCMIDEL1->SetBranchAddress("hsypfp", &hsypfpmeL1); 

  Float_t qmeL1;TSIMCMIDEL1->SetBranchAddress("q", &qmeL1);
  Float_t Q2_simcmeL1;TSIMCMIDEL1->SetBranchAddress("Q2", &Q2_simcmeL1);
  Float_t W_simcmeL1;TSIMCMIDEL1->SetBranchAddress("W", &W_simcmeL1);
  Float_t t_simcmeL1;TSIMCMIDEL1->SetBranchAddress("t", &t_simcmeL1);
  Float_t ti_simcmeL1;TSIMCMIDEL1->SetBranchAddress("ti", &ti_simcmeL1);
  Float_t epsilon_simcmeL1;TSIMCMIDEL1->SetBranchAddress("epsilon", &epsilon_simcmeL1);
  Float_t missmassmeL1;TSIMCMIDEL1->SetBranchAddress("missmass", &missmassmeL1);
  Float_t EmmeL1;TSIMCMIDEL1->SetBranchAddress("Em", &EmmeL1);
  Float_t PmmeL1;TSIMCMIDEL1->SetBranchAddress("Pm", &PmmeL1);
  Float_t WeightmeL1;TSIMCMIDEL1->SetBranchAddress("Weight", &WeightmeL1);
  Float_t phipqmeL1;TSIMCMIDEL1->SetBranchAddress("phipq", &phipqmeL1);

  //LEFT2
  //SHMS SIMC variables                                               
  Float_t ssdeltameL2;TSIMCMIDEL2->SetBranchAddress("ssdelta", &ssdeltameL2);
  Float_t ssxptarmeL2;TSIMCMIDEL2->SetBranchAddress("ssxptar", &ssxptarmeL2);
  Float_t ssyptarmeL2;TSIMCMIDEL2->SetBranchAddress("ssyptar", &ssyptarmeL2);
  Float_t ssxfpmeL2;TSIMCMIDEL2->SetBranchAddress("ssxfp", &ssxfpmeL2);
  Float_t ssyfpmeL2;TSIMCMIDEL2->SetBranchAddress("ssyfp", &ssyfpmeL2);
  Float_t ssxpfpmeL2;TSIMCMIDEL2->SetBranchAddress("ssxpfp", &ssxpfpmeL2);
  Float_t ssypfpmeL2;TSIMCMIDEL2->SetBranchAddress("ssypfp", &ssypfpmeL2);

  //HMS SIMC variables 
  Float_t hsdeltameL2;TSIMCMIDEL2->SetBranchAddress("hsdelta", &hsdeltameL2);
  Float_t hsxptarmeL2;TSIMCMIDEL2->SetBranchAddress("hsxptar", &hsxptarmeL2);
  Float_t hsyptarmeL2;TSIMCMIDEL2->SetBranchAddress("hsyptar", &hsyptarmeL2);
  Float_t hsxfpmeL2;TSIMCMIDEL2->SetBranchAddress("hsxfp", &hsxfpmeL2);
  Float_t hsyfpmeL2;TSIMCMIDEL2->SetBranchAddress("hsyfp", &hsyfpmeL2);
  Float_t hsxpfpmeL2;TSIMCMIDEL2->SetBranchAddress("hsxpfp", &hsxpfpmeL2);
  Float_t hsypfpmeL2;TSIMCMIDEL2->SetBranchAddress("hsypfp", &hsypfpmeL2); 

  Float_t qmeL2;TSIMCMIDEL2->SetBranchAddress("q", &qmeL2);
  Float_t Q2_simcmeL2;TSIMCMIDEL2->SetBranchAddress("Q2", &Q2_simcmeL2);
  Float_t W_simcmeL2;TSIMCMIDEL2->SetBranchAddress("W", &W_simcmeL2);
  Float_t t_simcmeL2;TSIMCMIDEL2->SetBranchAddress("t", &t_simcmeL2);
  Float_t ti_simcmeL2;TSIMCMIDEL2->SetBranchAddress("ti", &ti_simcmeL2);
  Float_t epsilon_simcmeL2;TSIMCMIDEL2->SetBranchAddress("epsilon", &epsilon_simcmeL2);
  Float_t missmassmeL2;TSIMCMIDEL2->SetBranchAddress("missmass", &missmassmeL2);
  Float_t EmmeL2;TSIMCMIDEL2->SetBranchAddress("Em", &EmmeL2);
  Float_t PmmeL2;TSIMCMIDEL2->SetBranchAddress("Pm", &PmmeL2);
  Float_t WeightmeL2;TSIMCMIDEL2->SetBranchAddress("Weight", &WeightmeL2);
  Float_t phipqmeL2;TSIMCMIDEL2->SetBranchAddress("phipq", &phipqmeL2);

  // HISTOGRAMS

  TH2D *hQ2WMEC  = new TH2D("hQ2WMERC","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WMEC1  = new TH2D("hQ2WMERC1","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      

  TH2D *hQ2WMECR  = new TH2D("hQ2WMECR","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WMEC1R  = new TH2D("hQ2WMEC1R","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      

  //W

  TH1D *hW1meC   = new TH1D("hW1meC","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2meC   = new TH1D("hW2meC","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3meC   = new TH1D("hW3meC","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4meC   = new TH1D("hW4meC","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5meC   = new TH1D("hW5meC","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6meC   = new TH1D("hW6meC","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7meC   = new TH1D("hW7meC","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8meC   = new TH1D("hW8meC","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1meL1   = new TH1D("hW1meL1","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2meL1   = new TH1D("hW2meL1","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3meL1   = new TH1D("hW3meL1","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4meL1   = new TH1D("hW4meL1","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5meL1   = new TH1D("hW5meL1","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6meL1   = new TH1D("hW6meL1","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7meL1   = new TH1D("hW7meL1","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8meL1   = new TH1D("hW8meL1","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1meL2   = new TH1D("hW1meL2","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2meL2   = new TH1D("hW2meL2","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3meL2   = new TH1D("hW3meL2","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4meL2   = new TH1D("hW4meL2","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5meL2   = new TH1D("hW5meL2","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6meL2   = new TH1D("hW6meL2","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7meL2   = new TH1D("hW7meL2","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8meL2   = new TH1D("hW8meL2","W 8 t L2; W;", 300, 2.1, 2.3);

  //W Random

  TH1D *hW1meCR   = new TH1D("hW1meCR","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2meCR   = new TH1D("hW2meCR","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3meCR   = new TH1D("hW3meCR","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4meCR   = new TH1D("hW4meCR","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5meCR   = new TH1D("hW5meCR","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6meCR   = new TH1D("hW6meCR","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7meCR   = new TH1D("hW7meCR","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8meCR   = new TH1D("hW8meCR","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1meL1R   = new TH1D("hW1meL1R","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2meL1R   = new TH1D("hW2meL1R","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3meL1R   = new TH1D("hW3meL1R","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4meL1R   = new TH1D("hW4meL1R","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5meL1R   = new TH1D("hW5meL1R","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6meL1R   = new TH1D("hW6meL1R","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7meL1R   = new TH1D("hW7meL1R","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8meL1R   = new TH1D("hW8meL1R","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1meL2R   = new TH1D("hW1meL2R","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2meL2R   = new TH1D("hW2meL2R","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3meL2R   = new TH1D("hW3meL2R","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4meL2R   = new TH1D("hW4meL2R","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5meL2R   = new TH1D("hW5meL2R","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6meL2R   = new TH1D("hW6meL2R","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7meL2R   = new TH1D("hW7meL2R","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8meL2R   = new TH1D("hW8meL2R","W 8 t L2; W;", 300, 2.1, 2.3);

  //Q2

  TH1D *hQ21meC   = new TH1D("hQ21meC","Q2 1 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meC   = new TH1D("hQ22meC","Q2 2 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meC   = new TH1D("hQ23meC","Q2 3 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meC   = new TH1D("hQ24meC","Q2 4 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meC   = new TH1D("hQ25meC","Q2 5 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meC   = new TH1D("hQ26meC","Q2 6 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meC   = new TH1D("hQ27meC","Q2 7 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meC   = new TH1D("hQ28meC","Q2 8 t C; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meL1  = new TH1D("hQ21meL1","Q2 1 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meL1  = new TH1D("hQ22meL1","Q2 2 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meL1  = new TH1D("hQ23meL1","Q2 3 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meL1  = new TH1D("hQ24meL1","Q2 4 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meL1  = new TH1D("hQ25meL1","Q2 5 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meL1  = new TH1D("hQ26meL1","Q2 6 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meL1  = new TH1D("hQ27meL1","Q2 7 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meL1  = new TH1D("hQ28meL1","Q2 8 t L1 Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meL2  = new TH1D("hQ21meL2","Q2 1 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22meL2  = new TH1D("hQ22meL2","Q2 2 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23meL2  = new TH1D("hQ23meL2","Q2 3 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24meL2  = new TH1D("hQ24meL2","Q2 4 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25meL2  = new TH1D("hQ25meL2","Q2 5 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26meL2  = new TH1D("hQ26meL2","Q2 6 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27meL2  = new TH1D("hQ27meL2","Q2 7 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28meL2  = new TH1D("hQ28meL2","Q2 8 t L2 Q2;", 300, 0.2, 0.6);      

  //Q2 Random

  TH1D *hQ21meCR   = new TH1D("hQ21meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meCR   = new TH1D("hQ22meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meCR   = new TH1D("hQ23meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meCR   = new TH1D("hQ24meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meCR   = new TH1D("hQ25meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meCR   = new TH1D("hQ26meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meCR   = new TH1D("hQ27meCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meCR   = new TH1D("hQ28meCR","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meL1R  = new TH1D("hQ21meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meL1R  = new TH1D("hQ22meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meL1R  = new TH1D("hQ23meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meL1R  = new TH1D("hQ24meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meL1R  = new TH1D("hQ25meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meL1R  = new TH1D("hQ26meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meL1R  = new TH1D("hQ27meL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meL1R  = new TH1D("hQ28meL1R","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meL2R  = new TH1D("hQ21meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22meL2R  = new TH1D("hQ22meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23meL2R  = new TH1D("hQ23meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24meL2R  = new TH1D("hQ24meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25meL2R  = new TH1D("hQ25meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26meL2R  = new TH1D("hQ26meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27meL2R  = new TH1D("hQ27meL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28meL2R  = new TH1D("hQ28meL2R","Q2; Q2;", 300, 0.2, 0.6);      

  //t
  TH1D *ht1meC  = new TH1D("ht1meC","t 1 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meC  = new TH1D("ht2meC","t 2 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meC  = new TH1D("ht3meC","t 3 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meC  = new TH1D("ht4meC","t 4 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meC  = new TH1D("ht5meC","t 5 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meC  = new TH1D("ht6meC","t 6 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meC  = new TH1D("ht7meC","t 7 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meC  = new TH1D("ht8meC","t 8 t C; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meL1  = new TH1D("ht1meL1","t 1 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meL1  = new TH1D("ht2meL1","t 2 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meL1  = new TH1D("ht3meL1","t 3 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meL1  = new TH1D("ht4meL1","t 4 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meL1  = new TH1D("ht5meL1","t 5 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meL1  = new TH1D("ht6meL1","t 6 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meL1  = new TH1D("ht7meL1","t 7 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meL1  = new TH1D("ht8meL1","t 8 t L1; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meL2  = new TH1D("ht1meL2","t 1 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meL2  = new TH1D("ht2meL2","t 2 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meL2  = new TH1D("ht3meL2","t 3 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meL2  = new TH1D("ht4meL2","t 4 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meL2  = new TH1D("ht5meL2","t 5 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meL2  = new TH1D("ht6meL2","t 6 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meL2  = new TH1D("ht7meL2","t 7 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meL2  = new TH1D("ht8meL2","t 8 t L2; MandelT;",   300, -0.01, 0.1);      

  //t Random
  TH1D *ht1meCR  = new TH1D("ht1meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meCR  = new TH1D("ht2meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meCR  = new TH1D("ht3meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meCR  = new TH1D("ht4meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meCR  = new TH1D("ht5meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meCR  = new TH1D("ht6meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meCR  = new TH1D("ht7meCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meCR  = new TH1D("ht8meCR","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meL1R  = new TH1D("ht1meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meL1R  = new TH1D("ht2meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meL1R  = new TH1D("ht3meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meL1R  = new TH1D("ht4meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meL1R  = new TH1D("ht5meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meL1R  = new TH1D("ht6meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meL1R  = new TH1D("ht7meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meL1R  = new TH1D("ht8meL1R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meL2R  = new TH1D("ht1meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meL2R  = new TH1D("ht2meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meL2R  = new TH1D("ht3meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meL2R  = new TH1D("ht4meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meL2R  = new TH1D("ht5meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meL2R  = new TH1D("ht6meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meL2R  = new TH1D("ht7meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meL2R  = new TH1D("ht8meL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  //*************************
  //Thesis Plots
  TH1D *MPimeC   = new TH1D("MPimeC"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *hthQ2meC  = new TH1D("hthQ2meC"," ; Q^{2} (GeV^{2}); Yield (events/mC)", 150, 0.2, 0.6);      
  TH1D *hthWmeC   = new TH1D("hthWmeC"," ;W (GeV); Yield (events/mC)", 150, 2.14, 2.26);
  TH1D *hthtmeC   = new TH1D("hthtmeC"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthepmeC   = new TH1D("hthepmeC"," ;epsilon; Yield (events/mC)", 150, 0.60, 0.66);      
  //Random
  TH1D *MPimeCR   = new TH1D("MPimeCR"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *hthQ2meCR  = new TH1D("hthQ2meCR"," ; Q^{2} (GeV^{2}); Yield (events/mC)", 150, 0.2, 0.6);      
  TH1D *hthWmeCR   = new TH1D("hthWmeCR"," ;W (GeV); Yield (events/mC)", 150, 2.14, 2.26);
  TH1D *hthtmeCR   = new TH1D("hthtmeCR"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthepmeCR   = new TH1D("hthepmeCR"," ;epsilon; Yield (events/mC)", 150, 0.60, 0.66);      

  //Dummy
  TH1D *MPimedC   = new TH1D("MPimedC"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *hthQ2medC  = new TH1D("hthQ2medC"," ; Q^{2}(GeV^{2}); Yield (events/mC)", 150, 0.2, 0.6);      
  TH1D *hthWmedC   = new TH1D("hthWmedC"," ;W (GeV); Yield (events/mC)", 150, 2.14, 2.26);
  TH1D *hthtmedC   = new TH1D("hthtmedC"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthepmedC   = new TH1D("hthepmedC"," ;epsilon; Yield (events/mC)", 150, 0.60, 0.66);      
  //Random
  TH1D *MPimedCR   = new TH1D("MPimedCR"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *hthQ2medCR  = new TH1D("hthQ2medCR"," ; Q^{2}(GeV^{2}); Yield (events/mC)", 150, 0.2, 0.6);      
  TH1D *hthWmedCR   = new TH1D("hthWmedCR"," ;W (GeV); Yield (events/mC)", 150, 2.14, 2.26);
  TH1D *hthtmedCR   = new TH1D("hthtmedCR"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthepmedCR   = new TH1D("hthepmedCR"," ;epsilon; Yield (events/mC)", 150, 0.60, 0.66);      
 
  //SIMC
  TH1D *MPiSmeC   = new TH1D("MPiSmeC"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *hthQ2SmeC  = new TH1D("hthQ2SmeC"," ; Q^{2}(GeV^{2}); Yield (events/mC)", 150, 0.2, 0.6);      
  TH1D *hthWSmeC   = new TH1D("hthWSmeC"," ;W (GeV); Yield (events/mC)", 150, 2.14, 2.26);
  TH1D *hthtSmeC   = new TH1D("hthtSmeC"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthepSmeC   = new TH1D("hthepSmeC"," ;epsilon; Yield (events/mC)", 150, 0.60, 0.66);      

 //*************************
  //plot added on Apr 17, 2024
  //MMpi 
  TH1D *MPimeL1   = new TH1D("MPimeL1"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPimeL2   = new TH1D("MPimeL2"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPimeL1R   = new TH1D("MPimeL1R"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPimeL2R   = new TH1D("MPimeL2R"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  //Dummy
  TH1D *MPimedL1   = new TH1D("MPimedL1"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPimedL2   = new TH1D("MPimedL2"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPimedL1R   = new TH1D("MPimedL1R"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPimedL2R   = new TH1D("MPimedL2R"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  //SIMC
  TH1D *MPiSmeL1   = new TH1D("MPiSmeL1"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSmeL2   = new TH1D("MPiSmeL2"," ; Missing Mass (GeV); Yield (events/mC)", 150, 0.9, 1.0);      

  //t
  TH1D *hthtmeL1   = new TH1D("hthtmeL1"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtmeL2   = new TH1D("hthtmeL2"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtmeL1R   = new TH1D("hthtmeL1R"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtmeL2R   = new TH1D("hthtmeL2R"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  //Dummy
  TH1D *hthtmedL1   = new TH1D("hthtmedL1"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtmedL2   = new TH1D("hthtmedL2"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtmedL1R   = new TH1D("hthtmedL1R"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtmedL2R   = new TH1D("hthtmedL2R"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  //SIMC
  TH1D *hthtSmeL1   = new TH1D("hthtSmeL1"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSmeL2   = new TH1D("hthtSmeL2"," ;t (GeV^{2}); Yield (events/mC)",   150, 0.00, 0.071);      


 //*************************
  
  TH1D *htmeC   = new TH1D("htmeC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1  = new TH1D("htmeL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2  = new TH1D("htmeL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htmeInC   = new TH1D("htmeInC","MandelT; MandelT;",    10, bins);      
  TH1D *htmeInL1  = new TH1D("htmeInL1","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInL2  = new TH1D("htmeInL2","MandelT; MandelT;",   10, bins);      

  TH1D *htmeCR   = new TH1D("htmeCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1R  = new TH1D("htmeL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2R  = new TH1D("htmeL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htmeInCR   = new TH1D("htmeInCR","MandelT; MandelT;",    10, bins);      
  TH1D *htmeInL1R  = new TH1D("htmeInL1R","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInL2R  = new TH1D("htmeInL2R","MandelT; MandelT;",   10, bins);      

  TH1D *hph_qmeC   = new TH1D("hph_qmeC","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qmeL1  = new TH1D("hph_qmeL1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeL2  = new TH1D("hph_qmeL2","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH1D *hph_qmeCR   = new TH1D("hph_qmeCR","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qmeL1R  = new TH1D("hph_qmeL1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeL2R  = new TH1D("hph_qmeL2R","ph_q; ph_q;",   18, -22.5, 382.5);      
  
  TH2D *hYmeC    = new TH2D("hYmeC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL1   = new TH2D("hYmeL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL2   = new TH2D("hYmeL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYmeCR    = new TH2D("hYmeCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL1R   = new TH2D("hYmeL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL2R   = new TH2D("hYmeL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      
  
  /*
  TH2D *hYmeC    = new TH2D("hYmeC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmeL1   = new TH2D("hYmeL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmeL2   = new TH2D("hYmeL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);

  TH2D *hYmeCR    = new TH2D("hYmeCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmeL1R   = new TH2D("hYmeL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmeL2R   = new TH2D("hYmeL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);      
  */

  //Dummy
  TH2D *hYmedC    = new TH2D("hYmedC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYmedL1   = new TH2D("hYmedL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmedL2   = new TH2D("hYmedL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYmedCR    = new TH2D("hYmedCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYmedL1R   = new TH2D("hYmedL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmedL2R   = new TH2D("hYmedL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      
  
  /*
  TH2D *hYmedC    = new TH2D("hYmedC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmedL1   = new TH2D("hYmedL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmedL2   = new TH2D("hYmedL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);

  TH2D *hYmedCR    = new TH2D("hYmedCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmedL1R   = new TH2D("hYmedL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYmedL2R   = new TH2D("hYmedL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);      
  */ 
  
  //SIMC
  TH2D *hYSmeC   = new TH2D("hYSmeC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSmeL1   = new TH2D("hYSmeL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSmeL2   = new TH2D("hYSmeL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  hYSmeC->Sumw2();
  hYSmeL1->Sumw2();
  hYSmeL2->Sumw2();

  
  /*
  TH2D *hYSmeC   = new TH2D("hYSmeC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSmeL1   = new TH2D("hYSmeL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSmeL2   = new TH2D("hYSmeL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  */
  TH1D *hmmmeC   = new TH1D("hmmmeC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL1  = new TH1D("hmmmeL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL2  = new TH1D("hmmmeL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmmeCR   = new TH1D("hmmmeCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL1R  = new TH1D("hmmmeL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL2R  = new TH1D("hmmmeL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinmeC    = new TH1D("htcoinmeC","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinmeL1   = new TH1D("htcoinmeL1","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinmeL2   = new TH1D("htcoinmeL2","; ;",  300, -20.0, 20.0);      
  
  //SIMC and Data HISTOGRAMS  

  //CENTER 
    
  TH1D *hphmeC  = new TH1D("hphmeC","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphmeCR  = new TH1D("hphmeCR","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphmeSC  = new TH1D("hphmeSC","ph_q; ph_q;",50, -10, 370);      
  
  /*
  TH1D *hphmeC  = new TH1D("hphmeC","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeCR  = new TH1D("hphmeCR","ph_q; ph_q;",  150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeSC  = new TH1D("hphmeSC","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  //  TH2D *hYSmeC   = new TH2D("hYSmeC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SmeC  = new TH1D("H_ssdelta_SmeC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SmeC  = new TH1D("H_ssxptar_SmeC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SmeC  = new TH1D("H_ssyptar_SmeC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DmeC  = new TH1D("H_ssdelta_DmeC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeC  = new TH1D("H_ssxptar_DmeC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeC  = new TH1D("H_ssyptar_DmeC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  //Random
  TH1D *H_ssdelta_DmeCR  = new TH1D("H_ssdelta_DmeCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeCR  = new TH1D("H_ssxptar_DmeCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeCR  = new TH1D("H_ssyptar_DmeCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  //Dummy
  TH1D *H_ssdelta_DmedC  = new TH1D("H_ssdelta_DmedC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmedC  = new TH1D("H_ssxptar_DmedC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmedC  = new TH1D("H_ssyptar_DmedC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  //Random
  TH1D *H_ssdelta_DmedCR  = new TH1D("H_ssdelta_DmedCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmedCR  = new TH1D("H_ssxptar_DmedCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmedCR  = new TH1D("H_ssyptar_DmedCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SmeC  = new TH1D("H_hsdelta_SmeC","HMS Delta; hsdelta;", 50, -7.0, 7.0);
  TH1D *H_hsxptar_SmeC  = new TH1D("H_hsxptar_SmeC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SmeC  = new TH1D("H_hsyptar_SmeC","HMS yptar; hsyptar;", 50, -0.05, 0.05);
  
  TH1D *H_hsdelta_DmeC  = new TH1D("H_hsdelta_DmeC","HMS Delta; hsdelta;", 50, -7.0, 7.0);
  TH1D *H_hsxptar_DmeC  = new TH1D("H_hsxptar_DmeC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeC  = new TH1D("H_hsyptar_DmeC","HMS yptar; hsyptar;", 50, -0.05, 0.05);
  //Random
  TH1D *H_hsdelta_DmeCR  = new TH1D("H_hsdelta_DmeCR","HMS Delta; hsdelta;", 50, -7.0, 7.0);
  TH1D *H_hsxptar_DmeCR  = new TH1D("H_hsxptar_DmeCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeCR  = new TH1D("H_hsyptar_DmeCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);
  //Dummy
  TH1D *H_hsdelta_DmedC  = new TH1D("H_hsdelta_DmedC","HMS Delta; hsdelta;", 50, -7.0, 7.0);
  TH1D *H_hsxptar_DmedC  = new TH1D("H_hsxptar_DmedC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmedC  = new TH1D("H_hsyptar_DmedC","HMS yptar; hsyptar;", 50, -0.05, 0.05);
  //Random
  TH1D *H_hsdelta_DmedCR  = new TH1D("H_hsdelta_DmedCR","HMS Delta; hsdelta;", 50, -7.0, 7.0);
  TH1D *H_hsxptar_DmedCR  = new TH1D("H_hsxptar_DmedCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmedCR  = new TH1D("H_hsyptar_DmedCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);


  //LEFT1
    
  TH1D *hphmeL1  = new TH1D("hphmeL1","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphmeL1R  = new TH1D("hphmeL1R","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphmeSL1  = new TH1D("hphmeSL1","ph_q; ph_q;",50, -10, 370);      
  
  /*
  TH1D *hphmeL1  = new TH1D("hphmeL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeL1R  = new TH1D("hphmeL1R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeSL1  = new TH1D("hphmeSL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  // TH2D *hYSmeL1   = new TH2D("hYSmeL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SmeL1  = new TH1D("H_ssdelta_SmeL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SmeL1  = new TH1D("H_ssxptar_SmeL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SmeL1  = new TH1D("H_ssyptar_SmeL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DmeL1  = new TH1D("H_ssdelta_DmeL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeL1  = new TH1D("H_ssxptar_DmeL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeL1  = new TH1D("H_ssyptar_DmeL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DmeL1R  = new TH1D("H_ssdelta_DmeL1R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeL1R  = new TH1D("H_ssxptar_DmeL1R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeL1R  = new TH1D("H_ssyptar_DmeL1R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SmeL1  = new TH1D("H_hsdelta_SmeL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SmeL1  = new TH1D("H_hsxptar_SmeL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SmeL1  = new TH1D("H_hsyptar_SmeL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DmeL1  = new TH1D("H_hsdelta_DmeL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DmeL1  = new TH1D("H_hsxptar_DmeL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeL1  = new TH1D("H_hsyptar_DmeL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DmeL1R  = new TH1D("H_hsdelta_DmeL1R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DmeL1R  = new TH1D("H_hsxptar_DmeL1R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeL1R  = new TH1D("H_hsyptar_DmeL1R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //LEFT2 
    
  TH1D *hphmeL2  = new TH1D("hphmeL2","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphmeL2R  = new TH1D("hphmeL2R","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphmeSL2  = new TH1D("hphmeSL2","ph_q; ph_q;",50, -10, 370);      
  
  /*
  TH1D *hphmeL2  = new TH1D("hphmeL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeL2R  = new TH1D("hphmeL2R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphmeSL2  = new TH1D("hphmeSL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  // TH2D *hYSmeL2   = new TH2D("hYSmeL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SmeL2  = new TH1D("H_ssdelta_SmeL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SmeL2  = new TH1D("H_ssxptar_SmeL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SmeL2  = new TH1D("H_ssyptar_SmeL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DmeL2  = new TH1D("H_ssdelta_DmeL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeL2  = new TH1D("H_ssxptar_DmeL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeL2  = new TH1D("H_ssyptar_DmeL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DmeL2R  = new TH1D("H_ssdelta_DmeL2R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DmeL2R  = new TH1D("H_ssxptar_DmeL2R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DmeL2R  = new TH1D("H_ssyptar_DmeL2R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SmeL2  = new TH1D("H_hsdelta_SmeL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SmeL2  = new TH1D("H_hsxptar_SmeL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SmeL2  = new TH1D("H_hsyptar_SmeL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DmeL2  = new TH1D("H_hsdelta_DmeL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DmeL2  = new TH1D("H_hsxptar_DmeL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeL2  = new TH1D("H_hsyptar_DmeL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DmeL2R  = new TH1D("H_hsdelta_DmeL2R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DmeL2R  = new TH1D("H_hsxptar_DmeL2R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DmeL2R  = new TH1D("H_hsyptar_DmeL2R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //Added on Apr 04, 2025
  
  TH2D *shmeC    = CreateHistogram("shmeC",   " ");
  TH2D *shmeL1   = CreateHistogram("shmeL1",  " ");
  TH2D *shmeL2   = CreateHistogram("shmeL2",  " ");
   
  Double_t MMpiOffsetmeC  = 0.0015;
  Double_t MMpiOffsetmeL1 = 0.002;
  Double_t MMpiOffsetmeL2 = 0.003;
  
  
  Double_t tOffsetmeC  = 0.00;
  Double_t tOffsetmeL1  = 0.0003;
  Double_t tOffsetmeL2  = 0.0003;  
  
  /*
  Double_t tOffsetmeC  = 0.00;
  Double_t tOffsetmeL1  = 0.00;
  Double_t tOffsetmeL2  = 0.00;
  */

  //Data Center
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEC; i++)

    {
      TBRANCHMIDEC->GetEntry(i);
      
      if(tcoinmeC-TimmingOffset>=-1.0 && tcoinmeC-TimmingOffset <= 1.0 && mmmeC>= 0.932-MMpiOffsetmeC && mmmeC <=0.98-MMpiOffsetmeC)
	{
	  hQ2WMEC->Fill(Q2MEC, WMEC);
	}
 
      if(tcoinmeC-TimmingOffset>=-1.0 && tcoinmeC-TimmingOffset <= 1.0)    
 	{
	  hmmmeC->Fill(mmmeC);	  
	}	      
      
      {
	htcoinmeC->Fill(tcoinmeC-TimmingOffset);
      }
      Double_t DiamondC = (Dcut->IsInside(Q2MEC, WMEC));  
      //      Double_t MMpiC = mmmeC>= 0.932 +MMpiOffsetmeC && mmmeC <=0.98+MMpiOffsetmeC;
      Double_t MMpiC = mmmeC+MMpiOffsetmeC>=MMPICUTL && mmmeC+MMpiOffsetmeC <=MMPICUTH;
      Double_t CoinPionC = tcoinmeC-TimmingOffset>=-1.0 && tcoinmeC-TimmingOffset <=1.0;      
      Double_t CoinPionCR = ((tcoinmeC-TimmingOffset>=-15.0 && tcoinmeC-TimmingOffset <= -9.0) || (tcoinmeC-TimmingOffset>=7.0 && tcoinmeC-TimmingOffset<=13.0));      

      if(CoinPionC && DiamondC)
	{
	  MPimeC->Fill(mmmeC+MMpiOffsetmeC);
	}  
      
      if(CoinPionC && MMpiC && DiamondC)
	{
          hthQ2meC->Fill(Q2MEC);
          hthWmeC->Fill(WMEC);
          hthtmeC->Fill(TMath::Abs(tmeC)+tOffsetmeC);
          hthepmeC->Fill(epsilonMEC);

          H_ssdelta_DmeC->Fill(P_gtr_dpmeC);
          H_ssxptar_DmeC->Fill(P_gtr_xptarmeC);
          H_ssyptar_DmeC->Fill(P_gtr_yptarmeC);

          H_hsdelta_DmeC->Fill(H_gtr_dpmeC);
          H_hsxptar_DmeC->Fill(H_gtr_xptarmeC);
          H_hsyptar_DmeC->Fill(H_gtr_yptarmeC+0.0025);

	  hQ2WMEC1->Fill(Q2MEC, WMEC);	      	  
	  htmeC->Fill(TMath::Abs(tmeC));
	  htmeInC->Fill(TMath::Abs(tmeC));
	  hph_qmeC->Fill(ph_qmeC*57.2958 + 180);
	  hphmeC->Fill(std::fmod(ph_qmeC * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeC->Fill(mmmeC+MMpiOffsetmeC);
	  hYmeC->Fill(TMath::Abs(tmeC)+tOffsetmeC, std::fmod(ph_qmeC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmeC->Fill(-tmeC, mmmeC+MMpiOffsetmeC);

	  shmeC->Fill(TMath::Abs(tmeC)*cos(ph_qmeC), TMath::Abs(tmeC)*sin(ph_qmeC));	  
	}	      

      //Random
     if(CoinPionCR && DiamondC) 
       {
	 MPimeCR->Fill(mmmeC+MMpiOffsetmeC);
       } 
          
     if(CoinPionCR && MMpiC && DiamondC)
       {
          hthQ2meCR->Fill(Q2MEC);
          hthWmeCR->Fill(WMEC);
          hthtmeCR->Fill(-tmeC+tOffsetmeC);
          hthepmeCR->Fill(epsilonMEC);

          H_ssdelta_DmeCR->Fill(P_gtr_dpmeC);
          H_ssxptar_DmeCR->Fill(P_gtr_xptarmeC);
          H_ssyptar_DmeCR->Fill(P_gtr_yptarmeC);

          H_hsdelta_DmeCR->Fill(H_gtr_dpmeC);
          H_hsxptar_DmeCR->Fill(H_gtr_xptarmeC);
          H_hsyptar_DmeCR->Fill(H_gtr_yptarmeC+0.0025);

	  htmeCR->Fill(TMath::Abs(tmeC));
	  htmeInCR->Fill(TMath::Abs(tmeC));
	  hph_qmeCR->Fill(ph_qmeC*57.2958 + 180);
	  //hYmeCR->Fill(-tmeC, mmmeC+MMpiOffsetmeC);
	  hYmeCR->Fill(TMath::Abs(tmeC)+tOffsetmeC, std::fmod(ph_qmeC * 180.0 / TMath::Pi() + 360.0,360.0));
	  hphmeCR->Fill(std::fmod(ph_qmeC * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeCR->Fill(mmmeC+MMpiOffsetmeC);
	  hQ2WMECR->Fill(Q2MEC, WMEC);
	}
     Double_t w1t = TMath::Abs(tmeC) >= bins[1] && TMath::Abs(tmeC) <= bins[2];
     Double_t w2t = TMath::Abs(tmeC) >= bins[2] && TMath::Abs(tmeC) <= bins[3];
     Double_t w3t = TMath::Abs(tmeC) >= bins[3] && TMath::Abs(tmeC) <= bins[4];
     Double_t w4t = TMath::Abs(tmeC) >= bins[4] && TMath::Abs(tmeC) <= bins[5];
     Double_t w5t = TMath::Abs(tmeC) >= bins[5] && TMath::Abs(tmeC) <= bins[6];
     Double_t w6t = TMath::Abs(tmeC) >= bins[6] && TMath::Abs(tmeC) <= bins[7];
     Double_t w7t = TMath::Abs(tmeC) >= bins[7] && TMath::Abs(tmeC) <= bins[8];
     Double_t w8t = TMath::Abs(tmeC) >= bins[8] && TMath::Abs(tmeC) <= bins[9];
      /*
      Double_t w1t = -tmeC+tOffsetmeC >= bins[1] && -tmeC+tOffsetmeC <= bins[2];
      Double_t w2t = -tmeC+tOffsetmeC >= bins[2] && -tmeC+tOffsetmeC <= bins[3];
      Double_t w3t = -tmeC+tOffsetmeC >= bins[3] && -tmeC+tOffsetmeC <= bins[4];
      Double_t w4t = -tmeC+tOffsetmeC >= bins[4] && -tmeC+tOffsetmeC <= bins[5];
      Double_t w5t = -tmeC+tOffsetmeC >= bins[5] && -tmeC+tOffsetmeC <= bins[6];
      Double_t w6t = -tmeC+tOffsetmeC >= bins[6] && -tmeC+tOffsetmeC <= bins[7];
      Double_t w7t = -tmeC+tOffsetmeC >= bins[7] && -tmeC+tOffsetmeC <= bins[8];
      Double_t w8t = -tmeC+tOffsetmeC >= bins[8] && -tmeC+tOffsetmeC <= bins[9];
      */
      
      if(CoinPionC && MMpiC && DiamondC && w1t)
	{
	  hW1meC->Fill(WMEC);	
	  hQ21meC->Fill(Q2MEC);	      	  
	  ht1meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w2t)
	{
	  hW2meC->Fill(WMEC);	
	  hQ22meC->Fill(Q2MEC);	      	  
	  ht2meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w3t)
	{
	  hW3meC->Fill(WMEC);	
	  hQ23meC->Fill(Q2MEC);	      	  
	  ht3meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w4t)
	{
	  hW4meC->Fill(WMEC);	
	  hQ24meC->Fill(Q2MEC);	      	  
	  ht4meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w5t)
	{
	  hW5meC->Fill(WMEC);	
	  hQ25meC->Fill(Q2MEC);	      	  
	  ht5meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w6t)
	{
	  hW6meC->Fill(WMEC);	
	  hQ26meC->Fill(Q2MEC);	      	  
	  ht6meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w7t)
	{
	  hW7meC->Fill(WMEC);	
	  hQ27meC->Fill(Q2MEC);	      	  
	  ht7meC->Fill(TMath::Abs(tmeC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w8t)
	{
	  hW8meC->Fill(WMEC);	
	  hQ28meC->Fill(Q2MEC);	      	  
	  ht8meC->Fill(TMath::Abs(tmeC)); 
 	}	
      //Random
      if(CoinPionCR && MMpiC && DiamondC && w1t)
	
	{
	  hW1meCR->Fill(WMEC);	
	  hQ21meCR->Fill(Q2MEC);	      	  
	  ht1meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w2t)
	
	{
	  hW2meCR->Fill(WMEC);	
	  hQ22meCR->Fill(Q2MEC);	      	  
	  ht2meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w3t)
	
	{
	  hW3meCR->Fill(WMEC);	
	  hQ23meCR->Fill(Q2MEC);	      	  
	  ht3meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w4t)
	
	{
	  hW4meCR->Fill(WMEC);	
	  hQ24meCR->Fill(Q2MEC);	      	  
	  ht4meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w5t)
	
	{
	  hW5meCR->Fill(WMEC);	
	  hQ25meCR->Fill(Q2MEC);	      	  
	  ht5meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w6t)
	
	{
	  hW6meCR->Fill(WMEC);	
	  hQ26meCR->Fill(Q2MEC);	      	  
	  ht6meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w7t)
	
	{
	  hW7meCR->Fill(WMEC);	
	  hQ27meCR->Fill(Q2MEC);	      	  
	  ht7meCR->Fill(TMath::Abs(tmeC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w8t)
	
	{
	  hW8meCR->Fill(WMEC);	
	  hQ28meCR->Fill(Q2MEC);	      	  
	  ht8meCR->Fill(TMath::Abs(tmeC)); 
	}
      
      if(((tcoinmeC-TimmingOffset>=-15.0 && tcoinmeC-TimmingOffset <= -9.0) || (tcoinmeC-TimmingOffset>=7.0 && tcoinmeC-TimmingOffset <=13.0)))
	
	{
	  hmmmeCR->Fill(mmmeC);
	}	
    }
  
  //Dummy Center
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEDC; i++)
    {
      TBRANCHMIDEDC->GetEntry(i);
      Double_t DimonddC = (Dcut->IsInside(Q2MEDC, WMEDC));  
      Double_t MMpidC = mmmedC+MMpiOffsetmeC>=MMPICUTL && mmmedC+MMpiOffsetmeC <=MMPICUTH;
      Double_t CoinPiondC = tcoinmedC-TimmingOffset>=-1.0 && tcoinmedC-TimmingOffset <= 1.0;      
      Double_t CoinPiondCR = ((tcoinmedC-TimmingOffset>=-15.0 && tcoinmedC-TimmingOffset<= -9.0) || (tcoinmedC-TimmingOffset>=7.0 && tcoinmedC-TimmingOffset<=13.0));      
      
      if(CoinPiondC && DimonddC)
	{
	  MPimedC->Fill(mmmedC+MMpiOffsetmeC);
	}
      if(CoinPiondC && MMpidC && DimonddC)
	{
          hthQ2medC->Fill(Q2MEDC);
          hthWmedC->Fill(WMEDC);
          hthtmedC->Fill(TMath::Abs(tmedC)+tOffsetmeC);
          hthepmedC->Fill(epsilonMEDC);
	  hYmedC->Fill(TMath::Abs(tmedC)+tOffsetmeC, std::fmod(ph_qmedC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedC->Fill(-tmedC, mmmedC+MMpiOffsetmeC);

          H_ssdelta_DmedC->Fill(P_gtr_dpmedC);
          H_ssxptar_DmedC->Fill(P_gtr_xptarmedC);
          H_ssyptar_DmedC->Fill(P_gtr_yptarmedC);

          H_hsdelta_DmedC->Fill(H_gtr_dpmedC);
          H_hsxptar_DmedC->Fill(H_gtr_xptarmedC);
          H_hsyptar_DmedC->Fill(H_gtr_yptarmedC+0.0025);
	}

      //Random      
     if(CoinPiondCR && DimonddC)
	{
	  MPimedCR->Fill(mmmedC+MMpiOffsetmeC);
	}

      if(CoinPiondCR && MMpidC && DimonddC)
	{
          hthQ2medCR->Fill(Q2MEDC);
          hthWmedCR->Fill(WMEDC);
          hthtmedCR->Fill(TMath::Abs(tmedC)+tOffsetmeC);
          hthepmedCR->Fill(epsilonMEDC);
	  hYmedCR->Fill(TMath::Abs(tmedC)+tOffsetmeC, std::fmod(ph_qmedC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedCR->Fill(-tmedC, mmmedC+MMpiOffsetmeC);

          H_ssdelta_DmedCR->Fill(P_gtr_dpmedC);
          H_ssxptar_DmedCR->Fill(P_gtr_xptarmedC);
          H_ssyptar_DmedCR->Fill(P_gtr_yptarmedC);

          H_hsdelta_DmedCR->Fill(H_gtr_dpmedC);
          H_hsxptar_DmedCR->Fill(H_gtr_xptarmedC);
          H_hsyptar_DmedCR->Fill(H_gtr_yptarmedC+0.0025);

	}      
    }    
  //SIMC CENTER                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCMIDEC; i++)

    {
      TSIMCMIDEC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcmeC, W_simcmeC));

      //      Double_t NFMC = 22512600.0/400000.0;
      //After applying SIMC offsets
      // Double_t NFMC = 22593800.0/400000.0;
      Double_t NFMC = 22615700.0/400000.0;
      //RadiativeStudy
      //Double_t NFMC = 23290000.0/400000.0;

      //      if(hsdeltameC >=-8.0 && hsdeltameC <=8.0 && hsxpfpmeC >=-0.08 && hsxpfpmeC <=0.08 && hsypfpmeC >=-0.045 && hsypfpmeC <=0.045 && ssdeltameC >=-10.0 && ssdeltameC <=20.0 && ssxpfpmeC >=-0.06 && ssxpfpmeC <=0.06 && ssypfpmeC >=-0.04 && ssypfpmeC <=0.04 && missmassmeC >=0.932  && missmassmeC <= 0.98 && Diamond_cut)

      Double_t CUTSIMCMEC = hsdeltameC > SIMC_Delta_Low_H && hsdeltameC < SIMC_Delta_High_H && hsxptarmeC > SIMC_Xptar_Low_H && hsxptarmeC < SIMC_Xptar_High_H && hsyptarmeC > SIMC_Yptar_Low_H && hsyptarmeC < SIMC_Yptar_High_H && ssdeltameC > SIMC_Delta_Low_P && ssdeltameC < SIMC_Delta_High_P && ssxptarmeC > SIMC_Xptar_Low_P && ssxptarmeC < SIMC_Xptar_High_P && ssyptarmeC > SIMC_Yptar_Low_P && ssyptarmeC < SIMC_Yptar_High_P && missmassmeC >=MMPICUTL  && missmassmeC <=MMPICUTH && Diamond_cut;

      Double_t CUTSIMCMEC2 = hsdeltameC > SIMC_Delta_Low_H && hsdeltameC < SIMC_Delta_High_H && hsxptarmeC > SIMC_Xptar_Low_H && hsxptarmeC < SIMC_Xptar_High_H && hsyptarmeC > SIMC_Yptar_Low_H && hsyptarmeC < SIMC_Yptar_High_H && ssdeltameC > SIMC_Delta_Low_P && ssdeltameC < SIMC_Delta_High_P && ssxptarmeC > SIMC_Xptar_Low_P && ssxptarmeC < SIMC_Xptar_High_P && ssyptarmeC > SIMC_Yptar_Low_P && ssyptarmeC < SIMC_Yptar_High_P && Diamond_cut;
	//hsdeltameC > -8.0 && hsdeltameC < 8.0 && hsxptarmeC > -0.08 && hsxptarmeC < 0.08 && hsyptarmeC > -0.045 && hsyptarmeC < 0.045 && ssdeltameC > -10.0 && ssdeltameC < 20.0 && ssxptarmeC > -0.06 && ssxptarmeC < 0.06 && ssyptarmeC > -0.04 && ssyptarmeC < 0.04 && Diamond_cut;
      
      if(CUTSIMCMEC2)
	{
	  MPiSmeC->Fill(missmassmeC, NFMC*WeightmeC);
	}
      
       if(CUTSIMCMEC)
	{
	  hthQ2SmeC->Fill(Q2_simcmeC, NFMC*WeightmeC);
          hthWSmeC->Fill(W_simcmeC, NFMC*WeightmeC);
          hthtSmeC->Fill(t_simcmeC, NFMC*WeightmeC);
          hthepSmeC->Fill(epsilon_simcmeC, NFMC*WeightmeC);

	  hphmeSC->Fill(phipqmeC*57.2958, NFMC*WeightmeC);
	  //hphmeSC->Fill(missmassmeC, NFMC*WeightmeC);
	  hYSmeC->Fill(t_simcmeC,phipqmeC*57.2958, NFMC*WeightmeC);
	  //hYSmeC->Fill(t_simcmeC,missmassmeC, NFMC*WeightmeC);
          H_ssdelta_SmeC->Fill(ssdeltameC, NFMC*WeightmeC);
          H_ssxptar_SmeC->Fill(ssxptarmeC, NFMC*WeightmeC);
          H_ssyptar_SmeC->Fill(ssyptarmeC, NFMC*WeightmeC);
	  
          H_hsdelta_SmeC->Fill(hsdeltameC, NFMC*WeightmeC);
          H_hsxptar_SmeC->Fill(hsxptarmeC, NFMC*WeightmeC);
          H_hsyptar_SmeC->Fill(hsyptarmeC, NFMC*WeightmeC);
	  
        }
    }
  //Data Left1
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL1; i++)
  
    {
      TBRANCHMIDEL1->GetEntry(i);
            
      if(tcoinmeL1-TimmingOffset>=-1.0 && tcoinmeL1-TimmingOffset <= 1.0)    
	{
	  hmmmeL1->Fill(mmmeL1);
	}           
      
      {
	htcoinmeL1->Fill(tcoinmeL1-TimmingOffset);
      }
      Double_t DiamondL1 = (Dcut->IsInside(Q2MEL1, WMEL1));  
      Double_t MMpiL1 = mmmeL1+MMpiOffsetmeL1>=MMPICUTL && mmmeL1+MMpiOffsetmeL1 <=MMPICUTH;
      Double_t CoinPionL1 = tcoinmeL1-TimmingOffset>=-1.0 && tcoinmeL1-TimmingOffset<=1.0;      
      Double_t CoinPionL1R = ((tcoinmeL1-TimmingOffset>=-15.0 && tcoinmeL1-TimmingOffset<= -9.0) || (tcoinmeL1-TimmingOffset>=7.0 && tcoinmeL1-TimmingOffset<=13.0));      

      if(CoinPionL1 && DiamondL1)
	{
	  MPimeL1->Fill(mmmeL1+MMpiOffsetmeL1);
	}
      if(CoinPionL1 && MMpiL1 && DiamondL1)
	{
          H_ssdelta_DmeL1->Fill(P_gtr_dpmeL1);
          H_ssxptar_DmeL1->Fill(P_gtr_xptarmeL1);
          H_ssyptar_DmeL1->Fill(P_gtr_yptarmeL1);

          H_hsdelta_DmeL1->Fill(H_gtr_dpmeL1);
          H_hsxptar_DmeL1->Fill(H_gtr_xptarmeL1);
          H_hsyptar_DmeL1->Fill(H_gtr_yptarmeL1);

          hthtmeL1->Fill(TMath::Abs(tmeL1)+tOffsetmeL1);
	  htmeL1->Fill(TMath::Abs(tmeL1));
	  htmeInL1->Fill(TMath::Abs(tmeL1));
	  hph_qmeL1->Fill(ph_qmeL1*57.2958 + 180);	  
	  hphmeL1->Fill(std::fmod(ph_qmeL1 * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeL1->Fill(mmmeL1+MMpiOffsetmeL1);
	  hYmeL1->Fill(TMath::Abs(tmeL1)+tOffsetmeL1, std::fmod(ph_qmeL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmeL1->Fill(-tmeL1, mmmeL1+MMpiOffsetmeL1);

	  shmeL1->Fill(TMath::Abs(tmeL1)*cos(ph_qmeL1), TMath::Abs(tmeL1)*sin(ph_qmeL1));
	}
      
      //Random
      if(CoinPionL1R && MMpiL1 && DiamondL1) 	
	{
	  MPimeL1R->Fill(mmmeL1+MMpiOffsetmeL1);
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1) 	
	{
          H_ssdelta_DmeL1R->Fill(P_gtr_dpmeL1);
          H_ssxptar_DmeL1R->Fill(P_gtr_xptarmeL1);
          H_ssyptar_DmeL1R->Fill(P_gtr_yptarmeL1);

          H_hsdelta_DmeL1R->Fill(H_gtr_dpmeL1);
          H_hsxptar_DmeL1R->Fill(H_gtr_xptarmeL1);
          H_hsyptar_DmeL1R->Fill(H_gtr_yptarmeL1);

          hthtmeL1R->Fill(TMath::Abs(tmeL1)+tOffsetmeL1);
	  htmeL1R->Fill(TMath::Abs(tmeL1));
	  htmeInL1R->Fill(TMath::Abs(tmeL1));
	  hph_qmeL1R->Fill(ph_qmeL1*57.2958 + 180);	  
	  hphmeL1R->Fill(std::fmod(ph_qmeL1 * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeL1R->Fill(mmmeL1+MMpiOffsetmeL1);
	  hYmeL1R->Fill(TMath::Abs(tmeL1)+tOffsetmeL1, std::fmod(ph_qmeL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmeL1R->Fill(-tmeL1, mmmeL1+MMpiOffsetmeL1);
	}
      /* Double_t w1t = -tmeL1+tOffsetmeL1 >= bins[1] && -tmeL1+tOffsetmeL1 <= bins[2];
      Double_t w2t = -tmeL1+tOffsetmeL1 >= bins[2] && -tmeL1+tOffsetmeL1 <= bins[3];
      Double_t w3t = -tmeL1+tOffsetmeL1 >= bins[3] && -tmeL1+tOffsetmeL1 <= bins[4];
      Double_t w4t = -tmeL1+tOffsetmeL1 >= bins[4] && -tmeL1+tOffsetmeL1 <= bins[5];
      Double_t w5t = -tmeL1+tOffsetmeL1 >= bins[5] && -tmeL1+tOffsetmeL1 <= bins[6];
      Double_t w6t = -tmeL1+tOffsetmeL1 >= bins[6] && -tmeL1+tOffsetmeL1 <= bins[7];
      Double_t w7t = -tmeL1+tOffsetmeL1 >= bins[7] && -tmeL1+tOffsetmeL1 <= bins[8];
      Double_t w8t = -tmeL1+tOffsetmeL1 >= bins[8] && -tmeL1+tOffsetmeL1 <= bins[9];
      */
      
      Double_t w1t = TMath::Abs(tmeL1) >= bins[1] && TMath::Abs(tmeL1) <= bins[2];
      Double_t w2t = TMath::Abs(tmeL1) >= bins[2] && TMath::Abs(tmeL1) <= bins[3];
      Double_t w3t = TMath::Abs(tmeL1) >= bins[3] && TMath::Abs(tmeL1) <= bins[4];
      Double_t w4t = TMath::Abs(tmeL1) >= bins[4] && TMath::Abs(tmeL1) <= bins[5];
      Double_t w5t = TMath::Abs(tmeL1) >= bins[5] && TMath::Abs(tmeL1) <= bins[6];
      Double_t w6t = TMath::Abs(tmeL1) >= bins[6] && TMath::Abs(tmeL1) <= bins[7];
      Double_t w7t = TMath::Abs(tmeL1) >= bins[7] && TMath::Abs(tmeL1) <= bins[8];
      Double_t w8t = TMath::Abs(tmeL1) >= bins[8] && TMath::Abs(tmeL1) <= bins[9];
      
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w1t)
	{
	  hW1meL1->Fill(WMEL1);	
	  hQ21meL1->Fill(Q2MEL1);	      	  
	  ht1meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w2t)
	{
	  hW2meL1->Fill(WMEL1);	
	  hQ22meL1->Fill(Q2MEL1);	      	  
	  ht2meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w3t)
	{
	  hW3meL1->Fill(WMEL1);	
	  hQ23meL1->Fill(Q2MEL1);	      	  
	  ht3meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w4t)
	{
	  hW4meL1->Fill(WMEL1);	
	  hQ24meL1->Fill(Q2MEL1);	      	  
	  ht4meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w5t)
	{
	  hW5meL1->Fill(WMEL1);	
	  hQ25meL1->Fill(Q2MEL1);	      	  
	  ht5meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w6t)
	{
	  hW6meL1->Fill(WMEL1);	
	  hQ26meL1->Fill(Q2MEL1);	      	  
	  ht6meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w7t)
	{
	  hW7meL1->Fill(WMEL1);	
	  hQ27meL1->Fill(Q2MEL1);	      	  
	  ht7meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w8t)
	{
	  hW8meL1->Fill(WMEL1);	
	  hQ28meL1->Fill(Q2MEL1);	      	  
	  ht8meL1->Fill(TMath::Abs(tmeL1)); 
 	}	
      //Random
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w1t)
	
	{
	  hW1meL1R->Fill(WMEL1);	
	  hQ21meL1R->Fill(Q2MEL1);	      	  
	  ht1meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w2t)
	
	{
	  hW2meL1R->Fill(WMEL1);	
	  hQ22meL1R->Fill(Q2MEL1);	      	  
	  ht2meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w3t)
	
	{
	  hW3meL1R->Fill(WMEL1);	
	  hQ23meL1R->Fill(Q2MEL1);	      	  
	  ht3meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w4t)
	
	{
	  hW4meL1R->Fill(WMEL1);	
	  hQ24meL1R->Fill(Q2MEL1);	      	  
	  ht4meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w5t)
	
	{
	  hW5meL1R->Fill(WMEL1);	
	  hQ25meL1R->Fill(Q2MEL1);	      	  
	  ht5meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w6t)
	
	{
	  hW6meL1R->Fill(WMEL1);	
	  hQ26meL1R->Fill(Q2MEL1);	      	  
	  ht6meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w7t)
	
	{
	  hW7meL1R->Fill(WMEL1);	
	  hQ27meL1R->Fill(Q2MEL1);	      	  
	  ht7meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w8t)
	
	{
	  hW8meL1R->Fill(WMEL1);	
	  hQ28meL1R->Fill(Q2MEL1);	      	  
	  ht8meL1R->Fill(TMath::Abs(tmeL1)); 
	}
      
      if(((tcoinmeL1-TimmingOffset>=-15.0 && tcoinmeL1-TimmingOffset <= -9.0) || (tcoinmeL1-TimmingOffset>=7.0 && tcoinmeL1-TimmingOffset <=13.0)))
	
	{	 
	  hmmmeL1R->Fill(mmmeL1);
	}     
    }
  //Dummy Left1
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEDL1; i++)
    {
      TBRANCHMIDEDL1->GetEntry(i);
      Double_t DimonddL1 = (Dcut->IsInside(Q2MEDL1, WMEDL1));  
      Double_t MMpidL1 = mmmedL1+MMpiOffsetmeL1>=MMPICUTL && mmmedL1+MMpiOffsetmeL1<=MMPICUTH;
      Double_t CoinPiondL1 = tcoinmedL1-TimmingOffset>=-1.0 && tcoinmedL1-TimmingOffset <= 1.0;      
      Double_t CoinPiondL1R = ((tcoinmedL1-TimmingOffset>=-15.0 && tcoinmedL1-TimmingOffset<= -9.0) || (tcoinmedL1-TimmingOffset>=7.0 && tcoinmedL1-TimmingOffset<=13.0));      
      
      if(CoinPiondL1 && DimonddL1)
	{
	  MPimedL1->Fill(mmmedL1+MMpiOffsetmeL1);
	}
      if(CoinPiondL1 && MMpidL1 && DimonddL1)
	{
	  hYmedL1->Fill(TMath::Abs(tmedL1)+tOffsetmeL1, std::fmod(ph_qmedL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedL1->Fill(-tmedL1, mmmedL1+MMpiOffsetmeL1);
          hthtmedL1->Fill(TMath::Abs(tmedL1)+tOffsetmeL1);

	}
      //Random
      if(CoinPiondL1R && DimonddL1)
	{
	  MPimedL1R->Fill(mmmedL1+MMpiOffsetmeL1);
	}
      if(CoinPiondL1R && MMpidL1 && DimonddL1)
	{
	  hYmedL1R->Fill(TMath::Abs(tmedL1)+tOffsetmeL1, std::fmod(ph_qmedL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedL1R->Fill(-tmedL1, mmmedL1+MMpiOffsetmeL1);
         hthtmedL1R->Fill(-tmedL1+tOffsetmeL1);

	}      
    }    
  //SIMC LEFT1                                                 
  for(Long64_t i = 0; i < nEntries_TSIMCMIDEL1; i++)

    {
      TSIMCMIDEL1->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcmeL1, W_simcmeL1));

      //      Double_t NFML1 = 22549900.0/400000.0;
      //After applying SIMC offsets
      Double_t NFML1 = 22778900.0/400000.0;
      //RadiativeStudy
      //Double_t NFML1 = 23349600.0/400000.0;

      //      if(hsdeltameL1 >=-8.0 && hsdeltameL1 <=8.0 && hsxpfpmeL1 >=-0.08 && hsxpfpmeL1 <=0.08 && hsypfpmeL1 >=-0.045 && hsypfpmeL1 <=0.045 && ssdeltameL1 >=-10.0 && ssdeltameL1 <=20.0 && ssxpfpmeL1 >=-0.06 && ssxpfpmeL1 <=0.06 && ssypfpmeL1 >=-0.04 && ssypfpmeL1 <=0.04 && missmassmeL1 >= 0.932 && missmassmeL1 <= 0.98 && Diamond_cut)

      Double_t CUTSIMCMEL1 = hsdeltameL1 > SIMC_Delta_Low_H && hsdeltameL1 < SIMC_Delta_High_H && hsxptarmeL1 > SIMC_Xptar_Low_H && hsxptarmeL1 < SIMC_Xptar_High_H && hsyptarmeL1 > SIMC_Yptar_Low_H && hsyptarmeL1 < SIMC_Yptar_High_H && ssdeltameL1 > SIMC_Delta_Low_P && ssdeltameL1 < SIMC_Delta_High_P && ssxptarmeL1 > SIMC_Xptar_Low_P && ssxptarmeL1 < SIMC_Xptar_High_P && ssyptarmeL1 > SIMC_Yptar_Low_P && ssyptarmeL1 < SIMC_Yptar_High_P && missmassmeL1 >=MMPICUTL && missmassmeL1 <=MMPICUTH && Diamond_cut; 

      Double_t CUTSIMCMEL12 = hsdeltameL1 > SIMC_Delta_Low_H && hsdeltameL1 < SIMC_Delta_High_H && hsxptarmeL1 > SIMC_Xptar_Low_H && hsxptarmeL1 < SIMC_Xptar_High_H && hsyptarmeL1 > SIMC_Yptar_Low_H && hsyptarmeL1 < SIMC_Yptar_High_H && ssdeltameL1 > SIMC_Delta_Low_P && ssdeltameL1 < SIMC_Delta_High_P && ssxptarmeL1 > SIMC_Xptar_Low_P && ssxptarmeL1 < SIMC_Xptar_High_P && ssyptarmeL1 > SIMC_Yptar_Low_P && ssyptarmeL1 < SIMC_Yptar_High_P && Diamond_cut; 
	//hsdeltameL1 > -8.0 && hsdeltameL1 < 8.0 && hsxptarmeL1 > -0.08 && hsxptarmeL1 < 0.08 && hsyptarmeL1 > -0.045 && hsyptarmeL1 < 0.045 && ssdeltameL1 > -10.0 && ssdeltameL1 < 20.0 && ssxptarmeL1 > -0.06 && ssxptarmeL1 < 0.06 && ssyptarmeL1 > -0.04 && ssyptarmeL1 < 0.04 && Diamond_cut; 
      
      if(CUTSIMCMEL12)
	{
	  MPiSmeL1->Fill(missmassmeL1, NFML1*WeightmeL1);
	}
      if(CUTSIMCMEL1)
	{
	  hphmeSL1->Fill(phipqmeL1*57.2958, NFML1*WeightmeL1);	  
	  //hphmeSL1->Fill(missmassmeL1, NFML1*WeightmeL1);	  
	  hYSmeL1->Fill(t_simcmeL1,phipqmeL1*57.2958, NFML1*WeightmeL1);
	  //hYSmeL1->Fill(t_simcmeL1,missmassmeL1, NFML1*WeightmeL1);
	  H_ssdelta_SmeL1->Fill(ssdeltameL1, NFML1*WeightmeL1);
	  H_ssxptar_SmeL1->Fill(ssxptarmeL1, NFML1*WeightmeL1);
	  H_ssyptar_SmeL1->Fill(ssyptarmeL1, NFML1*WeightmeL1);
      
	  H_hsdelta_SmeL1->Fill(hsdeltameL1, NFML1*WeightmeL1);
	  H_hsxptar_SmeL1->Fill(hsxptarmeL1, NFML1*WeightmeL1);
	  H_hsyptar_SmeL1->Fill(hsyptarmeL1, NFML1*WeightmeL1);
          hthtSmeL1->Fill(t_simcmeL1, NFML1*WeightmeL1);
	  
	}
    }
  //Data Left2
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL2; i++)
    {
      TBRANCHMIDEL2->GetEntry(i);
            
      if(tcoinmeL2-TimmingOffset>=-1.0 && tcoinmeL2-TimmingOffset <= 1.0)
	
	{
	  hmmmeL2->Fill(mmmeL2);
	}   
      
      {
	htcoinmeL2->Fill(tcoinmeL2-TimmingOffset);
      }
      
      Double_t DiamondL2 = (Dcut->IsInside(Q2MEL2, WMEL2));  
      Double_t MMpiL2 = mmmeL2+MMpiOffsetmeL2>=MMPICUTL && mmmeL2+MMpiOffsetmeL2 <=MMPICUTH;
      Double_t CoinPionL2 = tcoinmeL2-TimmingOffset>=-1.0 && tcoinmeL2-TimmingOffset<=1.0;      
      Double_t CoinPionL2R = ((tcoinmeL2-TimmingOffset>=-15.0 && tcoinmeL2-TimmingOffset<= -9.0) || (tcoinmeL2-TimmingOffset>=7.0 && tcoinmeL2-TimmingOffset<=13.0));      
      
      if(CoinPionL2 && DiamondL2)
	{
	  MPimeL2->Fill(mmmeL2+MMpiOffsetmeL2);
	}

      if(CoinPionL2 && MMpiL2 && DiamondL2)
	
	{
          H_ssdelta_DmeL2->Fill(P_gtr_dpmeL2);
          H_ssxptar_DmeL2->Fill(P_gtr_xptarmeL2);
          H_ssyptar_DmeL2->Fill(P_gtr_yptarmeL2);

          H_hsdelta_DmeL2->Fill(H_gtr_dpmeL2);
          H_hsxptar_DmeL2->Fill(H_gtr_xptarmeL2);
          H_hsyptar_DmeL2->Fill(H_gtr_yptarmeL2);

          hthtmeL2->Fill(TMath::Abs(tmeL2)+tOffsetmeL2);
	  htmeL2->Fill(TMath::Abs(tmeL2));
	  htmeInL2->Fill(TMath::Abs(tmeL2));
	  hph_qmeL2->Fill(ph_qmeL2*57.2958 + 180);
	  hphmeL2->Fill(std::fmod(ph_qmeL2 * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeL2->Fill(mmmeL2+MMpiOffsetmeL2);  //+MMpiOffsetme
	  hYmeL2->Fill(TMath::Abs(tmeL2)+tOffsetmeL2, std::fmod(ph_qmeL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmeL2->Fill(-tmeL2, mmmeL2+MMpiOffsetmeL2);

	  shmeL2->Fill(TMath::Abs(tmeL2)*cos(ph_qmeL2), TMath::Abs(tmeL2)*sin(ph_qmeL2));
	}
      
      //Random
      if(CoinPionL2R && DiamondL2)
	{
	  MPimeL2R->Fill(mmmeL2+MMpiOffsetmeL2);
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2)
	{
          H_ssdelta_DmeL2R->Fill(P_gtr_dpmeL2);
          H_ssxptar_DmeL2R->Fill(P_gtr_xptarmeL2);
          H_ssyptar_DmeL2R->Fill(P_gtr_yptarmeL2);

          H_hsdelta_DmeL2R->Fill(H_gtr_dpmeL2);
          H_hsxptar_DmeL2R->Fill(H_gtr_xptarmeL2);
          H_hsyptar_DmeL2R->Fill(H_gtr_yptarmeL2);

          hthtmeL2R->Fill(TMath::Abs(tmeL2+tOffsetmeL2));
	  htmeL2R->Fill(TMath::Abs(tmeL2));
	  htmeInL2R->Fill(TMath::Abs(tmeL2));
	  hph_qmeL2R->Fill(ph_qmeL2*57.2958 + 180);
	  hphmeL2R->Fill(std::fmod(ph_qmeL2 * 180.0 / TMath::Pi() + 360.0,360.0));
 	  //hphmeL2R->Fill(mmmeL2+MMpiOffsetmeL2);
	  hYmeL2R->Fill(TMath::Abs(tmeL2)+tOffsetmeL2, std::fmod(ph_qmeL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmeL2R->Fill(-tmeL2, mmmeL2+MMpiOffsetmeL2);
	}
      /*      Double_t w1t = -tmeL2+tOffsetmeL2 >= bins[1] && -tmeL2+tOffsetmeL2 <= bins[2];
      Double_t w2t = -tmeL2+tOffsetmeL2 >= bins[2] && -tmeL2+tOffsetmeL2 <= bins[3];
      Double_t w3t = -tmeL2+tOffsetmeL2 >= bins[3] && -tmeL2+tOffsetmeL2 <= bins[4];
      Double_t w4t = -tmeL2+tOffsetmeL2 >= bins[4] && -tmeL2+tOffsetmeL2 <= bins[5];
      Double_t w5t = -tmeL2+tOffsetmeL2 >= bins[5] && -tmeL2+tOffsetmeL2 <= bins[6];
      Double_t w6t = -tmeL2+tOffsetmeL2 >= bins[6] && -tmeL2+tOffsetmeL2 <= bins[7];
      Double_t w7t = -tmeL2+tOffsetmeL2 >= bins[7] && -tmeL2+tOffsetmeL2 <= bins[8];
      Double_t w8t = -tmeL2+tOffsetmeL2 >= bins[8] && -tmeL2+tOffsetmeL2 <= bins[9];
      */
      Double_t w1t = TMath::Abs(tmeL2) >= bins[1] && TMath::Abs(tmeL2) <= bins[2];
      Double_t w2t = TMath::Abs(tmeL2) >= bins[2] && TMath::Abs(tmeL2) <= bins[3];
      Double_t w3t = TMath::Abs(tmeL2) >= bins[3] && TMath::Abs(tmeL2) <= bins[4];
      Double_t w4t = TMath::Abs(tmeL2) >= bins[4] && TMath::Abs(tmeL2) <= bins[5];
      Double_t w5t = TMath::Abs(tmeL2) >= bins[5] && TMath::Abs(tmeL2) <= bins[6];
      Double_t w6t = TMath::Abs(tmeL2) >= bins[6] && TMath::Abs(tmeL2) <= bins[7];
      Double_t w7t = TMath::Abs(tmeL2) >= bins[7] && TMath::Abs(tmeL2) <= bins[8];
      Double_t w8t = TMath::Abs(tmeL2) >= bins[8] && TMath::Abs(tmeL2) <= bins[9];

      
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w1t)
	{
	  hW1meL2->Fill(WMEL2);	
	  hQ21meL2->Fill(Q2MEL2);	      	  
	  ht1meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w2t)
	{
	  hW2meL2->Fill(WMEL2);	
	  hQ22meL2->Fill(Q2MEL2);	      	  
	  ht2meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w3t)
	{
	  hW3meL2->Fill(WMEL2);	
	  hQ23meL2->Fill(Q2MEL2);	      	  
	  ht3meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w4t)
	{
	  hW4meL2->Fill(WMEL2);	
	  hQ24meL2->Fill(Q2MEL2);	      	  
	  ht4meL2->Fill(TMath::Abs(tmeL2)); 
 	}
      
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w5t)
	{
	  hW5meL2->Fill(WMEL2);	
	  hQ25meL2->Fill(Q2MEL2);	      	  
	  ht5meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w6t)
	{
	  hW6meL2->Fill(WMEL2);	
	  hQ26meL2->Fill(Q2MEL2);	      	  
	  ht6meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w7t)
	{
	  hW7meL2->Fill(WMEL2);	
	  hQ27meL2->Fill(Q2MEL2);	      	  
	  ht7meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w8t)
	{
	  hW8meL2->Fill(WMEL2);	
	  hQ28meL2->Fill(Q2MEL2);	      	  
	  ht8meL2->Fill(TMath::Abs(tmeL2)); 
 	}	
      //Random
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w1t)
	
	{
	  hW1meL2R->Fill(WMEL2);	
	  hQ21meL2R->Fill(Q2MEL2);	      	  
	  ht1meL2R->Fill(TMath::Abs(tmeL2)); 
	}

      if(CoinPionL2R && MMpiL2 && DiamondL2 && w2t)
	
	{
	  hW2meL2R->Fill(WMEL2);	
	  hQ22meL2R->Fill(Q2MEL2);	      	  
	  ht2meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w3t)
	
	{
	  hW3meL2R->Fill(WMEL2);	
	  hQ23meL2R->Fill(Q2MEL2);	      	  
	  ht3meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w4t)
	
	{
	  hW4meL2R->Fill(WMEL2);	
	  hQ24meL2R->Fill(Q2MEL2);	      	  
	  ht4meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w5t)
	
	{
	  hW5meL2R->Fill(WMEL2);	
	  hQ25meL2R->Fill(Q2MEL2);	      	  
	  ht5meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w6t)
	
	{
	  hW6meL2R->Fill(WMEL2);	
	  hQ26meL2R->Fill(Q2MEL2);	      	  
	  ht6meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w7t)
	
	{
	  hW7meL2R->Fill(WMEL2);	
	  hQ27meL2R->Fill(Q2MEL2);	      	  
	  ht7meL2R->Fill(TMath::Abs(tmeL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w8t)
	
	{
	  hW8meL2R->Fill(WMEL2);	
	  hQ28meL2R->Fill(Q2MEL2);	      	  
	  ht8meL2R->Fill(TMath::Abs(tmeL2)); 
	}

      if(((tcoinmeL2-TimmingOffset>=-15.0 && tcoinmeL2-TimmingOffset <= -9.0) || (tcoinmeL2-TimmingOffset>=7.0 && tcoinmeL2-TimmingOffset <=13.0)))
	{
	  hmmmeL2R->Fill(mmmeL2);
	}
    }
  //Dummy Left2
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEDL2; i++)
    {
      TBRANCHMIDEDL2->GetEntry(i);
      Double_t DimonddL2 = (Dcut->IsInside(Q2MEDL2, WMEDL2));  
      Double_t MMpidL2 = mmmedL2+MMpiOffsetmeL2>=MMPICUTL && mmmedL2+MMpiOffsetmeL2 <=MMPICUTH;
      Double_t CoinPiondL2 = tcoinmedL2-TimmingOffset>=-1.0 && tcoinmedL2-TimmingOffset<= 1.0;      
      Double_t CoinPiondL2R = ((tcoinmedL2-TimmingOffset>=-15.0 && tcoinmedL2-TimmingOffset<= -9.0) || (tcoinmedL2-TimmingOffset>=7.0 && tcoinmedL2-TimmingOffset<=13.0));      
      
      if(CoinPiondL2 && DimonddL2)
	{
	  MPimedL2->Fill(mmmedL2+MMpiOffsetmeL2);
	}
      if(CoinPiondL2 && MMpidL2 && DimonddL2)
	{
	  hYmedL2->Fill(TMath::Abs(tmedL2)+tOffsetmeL2, std::fmod(ph_qmedL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedL2->Fill(-tmedL2, mmmedL2+MMpiOffsetmeL2);
          hthtmedL2->Fill(TMath::Abs(tmedL2)+tOffsetmeL2);

	}
      //Random
      if(CoinPiondL2R && DimonddL2)
	{
	  MPimedL2R->Fill(mmmedL2+MMpiOffsetmeL2);
	}
      if(CoinPiondL2R && MMpidL2 && DimonddL2)
	{
	  hYmedL2R->Fill(TMath::Abs(tmedL2)+tOffsetmeL2, std::fmod(ph_qmedL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYmedL2R->Fill(-tmedL2, mmmedL2+MMpiOffsetmeL2);
          hthtmedL2R->Fill(TMath::Abs(tmedL2)+tOffsetmeL2);

	}      
    }    
  //SIMC LEFT2                                                 
  for(Long64_t i = 0; i < nEntries_TSIMCMIDEL2; i++)

    {
      TSIMCMIDEL2->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcmeL2, W_simcmeL2));

      //      Double_t NFML2 = 22358000.0/400000.0;
      //After applying SIMC offsets
      Double_t NFML2 = 22809400.0/400000.0;
      //RadiativeStudy    
      //Double_t NFML2 = 23437000.0/400000.0;

      //      if(hsdeltameL2 >=-8.0 && hsdeltameL2 <=8.0 && hsxpfpmeL2 >=-0.08 && hsxpfpmeL2 <=0.08 && hsypfpmeL2 >=-0.045 && hsypfpmeL2 <=0.045 && ssdeltameL2 >=-10.0 && ssdeltameL2 <=20.0 && ssxpfpmeL2 >=-0.06 && ssxpfpmeL2 <=0.06 && ssypfpmeL2 >=-0.04 && ssypfpmeL2 <=0.04 && missmassmeL2 >= 0.932 && missmassmeL2 <= 0.98 && Diamond_cut)
      
      Double_t CUTSIMCMEL2 = hsdeltameL2 > SIMC_Delta_Low_H && hsdeltameL2 < SIMC_Delta_High_H && hsxptarmeL2 > SIMC_Xptar_Low_H && hsxptarmeL2 < SIMC_Xptar_High_H && hsyptarmeL2 > SIMC_Yptar_Low_H && hsyptarmeL2 < SIMC_Yptar_High_H && ssdeltameL2 > SIMC_Delta_Low_P && ssdeltameL2 < SIMC_Delta_High_P && ssxptarmeL2 > SIMC_Xptar_Low_P && ssxptarmeL2 < SIMC_Xptar_High_P && ssyptarmeL2 > SIMC_Yptar_Low_P && ssyptarmeL2 < SIMC_Yptar_High_P && missmassmeL2 >=MMPICUTL && missmassmeL2 <=MMPICUTH && Diamond_cut;
      
      Double_t CUTSIMCMEL22 =  hsdeltameL2 > SIMC_Delta_Low_H && hsdeltameL2 < SIMC_Delta_High_H && hsxptarmeL2 > SIMC_Xptar_Low_H && hsxptarmeL2 < SIMC_Xptar_High_H && hsyptarmeL2 > SIMC_Yptar_Low_H && hsyptarmeL2 < SIMC_Yptar_High_H && ssdeltameL2 > SIMC_Delta_Low_P && ssdeltameL2 < SIMC_Delta_High_P && ssxptarmeL2 > SIMC_Xptar_Low_P && ssxptarmeL2 < SIMC_Xptar_High_P && ssyptarmeL2 > SIMC_Yptar_Low_P && ssyptarmeL2 < SIMC_Yptar_High_P && Diamond_cut;
	// hsdeltameL2 > -8.0 && hsdeltameL2 < 8.0 && hsxptarmeL2 > -0.08 && hsxptarmeL2 < 0.08 && hsyptarmeL2 > -0.045 && hsyptarmeL2 < 0.045 && ssdeltameL2 > -10.0 && ssdeltameL2 < 20.0 && ssxptarmeL2 > -0.06 && ssxptarmeL2 < 0.06 && ssyptarmeL2 > -0.04 && ssyptarmeL2 < 0.04 && Diamond_cut;

      if(CUTSIMCMEL22)	
	{
	  MPiSmeL2->Fill(missmassmeL2, NFML2*WeightmeL2);
	}
      
      if(CUTSIMCMEL2)	
        {
	  hphmeSL2->Fill(phipqmeL2*57.2958, NFML2*WeightmeL2);	  
	  //hphmeSL2->Fill(missmassmeL2, NFML2*WeightmeL2);	  
	  hYSmeL2->Fill(t_simcmeL2,phipqmeL2*57.2958, NFML2*WeightmeL2);
	  //hYSmeL2->Fill(t_simcmeL2,missmassmeL2, NFML2*WeightmeL2);
          H_ssdelta_SmeL2->Fill(ssdeltameL2, NFML2*WeightmeL2);
          H_ssxptar_SmeL2->Fill(ssxptarmeL2, NFML2*WeightmeL2);
          H_ssyptar_SmeL2->Fill(ssyptarmeL2, NFML2*WeightmeL2);
	  
          H_hsdelta_SmeL2->Fill(hsdeltameL2, NFML2*WeightmeL2);
          H_hsxptar_SmeL2->Fill(hsxptarmeL2, NFML2*WeightmeL2);
          H_hsyptar_SmeL2->Fill(hsyptarmeL2, NFML2*WeightmeL2);
          hthtSmeL2->Fill(t_simcmeL2, NFML2*WeightmeL2);
	  
        }
    }
  H_ssdelta_DmedCR->Scale(RTSCALE);
  H_ssxptar_DmedCR->Scale(RTSCALE);
  H_ssyptar_DmedCR->Scale(RTSCALE);

  H_hsdelta_DmedCR->Scale(RTSCALE);
  H_hsxptar_DmedCR->Scale(RTSCALE);
  H_hsyptar_DmedCR->Scale(RTSCALE);

  MPimeCR->Scale(RTSCALE);
  MPimeL1R->Scale(RTSCALE);
  MPimeL2R->Scale(RTSCALE);
  //Dummy
  MPimedCR->Scale(RTSCALE);
  MPimedL1R->Scale(RTSCALE);
  MPimedL2R->Scale(RTSCALE);

  hthtmeCR->Scale(RTSCALE);
  hthtmeL1R->Scale(RTSCALE);
  hthtmeL2R->Scale(RTSCALE);
  //Dummy
  hthtmedCR->Scale(RTSCALE);
  hthtmedL1R->Scale(RTSCALE);
  hthtmedL2R->Scale(RTSCALE);

  hthQ2meCR->Scale(RTSCALE);
  hthWmeCR->Scale(RTSCALE);
  hthepmeCR->Scale(RTSCALE);
  //Dummy 
  hthQ2medCR->Scale(RTSCALE);
  hthWmedCR->Scale(RTSCALE);
  hthepmedCR->Scale(RTSCALE);
  //Yield
  hYmeCR->Scale(RTSCALE);
  hYmeL1R->Scale(RTSCALE);
  hYmeL2R->Scale(RTSCALE);
  //Yield Dummy
  hYmedCR->Scale(RTSCALE);
  hYmedL1R->Scale(RTSCALE);
  hYmedL2R->Scale(RTSCALE);

  hphmeCR->Scale(RTSCALE);
  hphmeL1R->Scale(RTSCALE);
  hphmeL2R->Scale(RTSCALE);

  H_ssdelta_DmeCR->Scale(RTSCALE);
  H_ssxptar_DmeCR->Scale(RTSCALE);
  H_ssyptar_DmeCR->Scale(RTSCALE);

  H_ssdelta_DmeL1R->Scale(RTSCALE);
  H_ssxptar_DmeL1R->Scale(RTSCALE);
  H_ssyptar_DmeL1R->Scale(RTSCALE);

  H_ssdelta_DmeL2R->Scale(RTSCALE);
  H_ssxptar_DmeL2R->Scale(RTSCALE);
  H_ssyptar_DmeL2R->Scale(RTSCALE);

  //HMS

  H_hsdelta_DmeCR->Scale(RTSCALE);
  H_hsxptar_DmeCR->Scale(RTSCALE);
  H_hsyptar_DmeCR->Scale(RTSCALE);

  H_hsdelta_DmeL1R->Scale(RTSCALE);
  H_hsxptar_DmeL1R->Scale(RTSCALE);
  H_hsyptar_DmeL1R->Scale(RTSCALE);

  H_hsdelta_DmeL2R->Scale(RTSCALE);
  H_hsxptar_DmeL2R->Scale(RTSCALE);
  H_hsyptar_DmeL2R->Scale(RTSCALE);

  hQ2WMECR->Scale(1.0/6.0);

  hW1meCR->Scale(1.0/6.0);
  hW2meCR->Scale(1.0/6.0);
  hW3meCR->Scale(1.0/6.0);
  hW4meCR->Scale(1.0/6.0);
  hW5meCR->Scale(1.0/6.0);
  hW6meCR->Scale(1.0/6.0);
  hW7meCR->Scale(1.0/6.0);
  hW8meCR->Scale(1.0/6.0);

  hW1meL1R->Scale(1.0/6.0);
  hW2meL1R->Scale(1.0/6.0);
  hW3meL1R->Scale(1.0/6.0);
  hW4meL1R->Scale(1.0/6.0);
  hW5meL1R->Scale(1.0/6.0);
  hW6meL1R->Scale(1.0/6.0);
  hW7meL1R->Scale(1.0/6.0);
  hW8meL1R->Scale(1.0/6.0);

  hW1meL2R->Scale(1.0/6.0);
  hW2meL2R->Scale(1.0/6.0);
  hW3meL2R->Scale(1.0/6.0);
  hW4meL2R->Scale(1.0/6.0);
  hW5meL2R->Scale(1.0/6.0);
  hW6meL2R->Scale(1.0/6.0);
  hW7meL2R->Scale(1.0/6.0);
  hW8meL2R->Scale(1.0/6.0);

  hQ21meCR->Scale(1.0/6.0);
  hQ22meCR->Scale(1.0/6.0);
  hQ23meCR->Scale(1.0/6.0);
  hQ24meCR->Scale(1.0/6.0);
  hQ25meCR->Scale(1.0/6.0);
  hQ26meCR->Scale(1.0/6.0);
  hQ27meCR->Scale(1.0/6.0);
  hQ28meCR->Scale(1.0/6.0);

  hQ21meL1R->Scale(1.0/6.0);
  hQ22meL1R->Scale(1.0/6.0);
  hQ23meL1R->Scale(1.0/6.0);
  hQ24meL1R->Scale(1.0/6.0);
  hQ25meL1R->Scale(1.0/6.0);
  hQ26meL1R->Scale(1.0/6.0);
  hQ27meL1R->Scale(1.0/6.0);
  hQ28meL1R->Scale(1.0/6.0);

  hQ21meL2R->Scale(1.0/6.0);
  hQ22meL2R->Scale(1.0/6.0);
  hQ23meL2R->Scale(1.0/6.0);
  hQ24meL2R->Scale(1.0/6.0);
  hQ25meL2R->Scale(1.0/6.0);
  hQ26meL2R->Scale(1.0/6.0);
  hQ27meL2R->Scale(1.0/6.0);
  hQ28meL2R->Scale(1.0/6.0);

  ht1meCR->Scale(1.0/6.0); 
  ht2meCR->Scale(1.0/6.0); 
  ht3meCR->Scale(1.0/6.0); 
  ht4meCR->Scale(1.0/6.0); 
  ht5meCR->Scale(1.0/6.0); 
  ht6meCR->Scale(1.0/6.0); 
  ht7meCR->Scale(1.0/6.0); 
  ht8meCR->Scale(1.0/6.0); 

  ht1meL1R->Scale(1.0/6.0); 
  ht2meL1R->Scale(1.0/6.0); 
  ht3meL1R->Scale(1.0/6.0); 
  ht4meL1R->Scale(1.0/6.0); 
  ht5meL1R->Scale(1.0/6.0); 
  ht6meL1R->Scale(1.0/6.0); 
  ht7meL1R->Scale(1.0/6.0); 
  ht8meL1R->Scale(1.0/6.0); 

  ht1meL2R->Scale(1.0/6.0); 
  ht2meL2R->Scale(1.0/6.0); 
  ht3meL2R->Scale(1.0/6.0); 
  ht4meL2R->Scale(1.0/6.0); 
  ht5meL2R->Scale(1.0/6.0); 
  ht6meL2R->Scale(1.0/6.0); 
  ht7meL2R->Scale(1.0/6.0); 
  ht8meL2R->Scale(1.0/6.0); 
  
  hmmmeCR->Scale(1.0/6.0);
  hmmmeL1R->Scale(1.0/6.0);
  hmmmeL2R->Scale(1.0/6.0);

  htmeCR->Scale(1.0/6.0);
  htmeL1R->Scale(1.0/6.0);
  htmeL2R->Scale(1.0/6.0);

  htmeInCR->Scale(1.0/6.0);
  htmeInL1R->Scale(1.0/6.0);
  htmeInL2R->Scale(1.0/6.0);

  hph_qmeCR->Scale(1.0/6.0);
  hph_qmeL1R->Scale(1.0/6.0);
  hph_qmeL2R->Scale(1.0/6.0);

  hYmeCR->Scale(1.0/6.0);
  hYmeL1R->Scale(1.0/6.0);
  hYmeL2R->Scale(1.0/6.0);

  //Effective Charge  
    std::ifstream inputFile7("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_mide.csv");
  double effcharmeC;
  inputFile7 >> effcharmeC;
  inputFile7.close();
  std::ifstream inputFile8("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_mide_dummy.csv");
  double effcharmeCD;
  inputFile8 >> effcharmeCD;
  inputFile8.close();
  
  Double_t ECMEC    = (1.0+0.0872982)/effcharmeC;
  Double_t ECMEDC   = (1.0+0.0872982)/effcharmeCD;
   
  /*
  Double_t ECMEC    = (1.0)/effcharmeC;
  Double_t ECMEDC   = (1.0)/effcharmeCD;
  */
  std::ifstream inputFile9("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_mide.csv");
  double effcharmeL1;
  inputFile9 >> effcharmeL1;
  inputFile9.close();
  std::ifstream inputFile10("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_mide_dummy.csv");
  double effcharmeL1D;
  inputFile10 >> effcharmeL1D;
  inputFile10.close();

  
  Double_t ECMEL1   = (1.0+0.0687776)/effcharmeL1;
  Double_t ECMEDL1  = (1.0+0.0687776)/effcharmeL1D;
    
  /*
  Double_t ECMEL1   = (1.0)/effcharmeL1;
  Double_t ECMEDL1  = (1.0)/effcharmeL1D;
  */
 
  std::ifstream inputFile11("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_mide.csv");
  double effcharmeL2;
  inputFile11 >> effcharmeL2;
  inputFile11.close();
  std::ifstream inputFile12("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_mide_dummy.csv");
  double effcharmeL2D;
  inputFile12 >> effcharmeL2D;
  inputFile12.close();
  
  Double_t ECMEL2   = (1.0-0.022579)/effcharmeL2;
  Double_t ECMEDL2  = (1.0-0.022579)/effcharmeL2D;
   
  /*
  Double_t ECMEL2   = (1.0)/effcharmeL2;
  Double_t ECMEDL2  = (1.0)/effcharmeL2D;
  */
  //SIMC SCAL
  Double_t SCFMEC  = 1.0;
  Double_t SCFMEL1 = 1.0;
  Double_t SCFMEL2 = 1.0;

  TCanvas *c8me = new TCanvas("c8me", " tcoin"); 

  htcoinmeC->SetStats(0);
  htcoinmeC->SetLineColor(kGreen);
  //  htcoinmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  //  htcoinmeC->GetXaxis()->SetTitle("Coincidence time (ns)"); 
  htcoinmeC->Scale(ECMEC);
  htcoinmeC->Draw("Weight");

  htcoinmeL1->SetStats(0);
  htcoinmeL1->SetLineColor(kRed);
  htcoinmeL1->Scale(ECMEL1);
  htcoinmeL1->Draw("same Weight ");
 
  htcoinmeL2->SetStats(0);
  htcoinmeL2->SetLineColor(kBlack);
  htcoinmeL2->Scale(ECMEL2);
  htcoinmeL2->Draw("same Weight");
   
  TLine *l1me = new TLine(-1.0, 0.0, -1.0, 250.0);
  TLine *l2me = new TLine(1.0, 0.0, 1.0, 250.0);
  TLine *l3me = new TLine(-15.0, 0.0, -15.0, 250.0);
  TLine *l4me = new TLine(-9.0, 0.0, -9.0, 250.0);
  TLine *l5me = new TLine(7.0, 0.0, 7.0, 250.0);
  TLine *l6me = new TLine(13.0, 0.0, 13.0, 250.0);
  l1me->SetLineColor(kBlue);
  l2me->SetLineColor(kBlue);
  l3me->SetLineColor(kBlue);
  l4me->SetLineColor(kBlue);
  l5me->SetLineColor(kBlue);
  l6me->SetLineColor(kBlue);
  l1me->Draw("same");
  l2me->Draw("same");
  l3me->Draw("same");
  l4me->Draw("same");
  l5me->Draw("same");
  l6me->Draw("same");
     
  auto legme = new TLegend(0.1,0.3,0.30,0.9);
  legme->SetHeader("#epsilon = 0.617","C");
  legme->SetTextSize(0.1);
  legme->AddEntry(htcoinmeC,"Center","l");
  legme->AddEntry(htcoinmeL1,"Left1","l");
  legme->AddEntry(htcoinmeL2,"Left2","l");
  legme->Draw("same");

  c8me->Print(outputpdf);

  TCanvas *c9me = new TCanvas("c9me", " c9me"); 
  htmeC->SetStats(0);
  htmeC->SetLineColor(kGreen);
  htmeC->Add(htmeCR,-1); 
  htmeC->Draw("Weight");

  htmeL1->SetStats(0);
  htmeL1->SetLineColor(kRed);
  htmeL1->Add(htmeL1R, -1); 
  htmeL1->Draw("same Weight");

  htmeL2->SetStats(0);
  htmeL2->SetLineColor(kBlack);
  htmeL2->Add(htmeL2R, -1); 
  htmeL2->Draw("same Weight");
  c9me->Print(outputpdf);

  TCanvas *c9meIn = new TCanvas("c9meIn", " c9meIn"); 

  htmeInL2->SetStats(0);
  htmeInL2->SetLineColor(kBlack);
  htmeInL2->Add(htmeInL2R, -1); 
  htmeInL2->Draw("Weight");

  htmeInC->SetStats(0);
  htmeInC->SetLineColor(kGreen);
  htmeInC->Add(htmeInCR,-1); 
  htmeInC->Draw("same Weight");

  htmeInL1->SetStats(0);
  htmeInL1->SetLineColor(kRed);
  htmeInL1->Add(htmeInL1R, -1); 
  htmeInL1->Draw("same Weight");
  c9meIn->Print(outputpdf);

  TCanvas *cWmeC1 = new TCanvas("cWmeC1", "cWmeC1");
  cWmeC1->Divide(2, 2);
  cWmeC1->cd(1);
  hW1meC->Add(hW1meCR, -1);
  hW1meC->Draw("Weight");
  cWmeC1->cd(2);
  hW2meC->Add(hW2meCR, -1);
  hW2meC->Draw(" same Weight");
  cWmeC1->cd(3);
  hW3meC->Add(hW3meCR, -1);
  hW3meC->Draw(" same Weight");
  cWmeC1->cd(4);
  hW4meC->Add(hW4meCR, -1);
  hW4meC->Draw(" same Weight");
  cWmeC1->Print(outputpdf);
  
  TCanvas *cWmeC2 = new TCanvas("cWmeC2", "cWmeC2");
  cWmeC2->Divide(2, 2);
  cWmeC2->cd(1);
  hW5meC->Add(hW5meCR, -1);
  hW5meC->Draw("Weight");
  cWmeC2->cd(2);
  hW6meC->Add(hW6meCR, -1);
  hW6meC->Draw(" same Weight");
  cWmeC2->cd(3);
  hW7meC->Add(hW7meCR, -1);
  hW7meC->Draw(" same Weight");
  cWmeC2->cd(4);
  hW8meC->Add(hW8meCR, -1);
  hW8meC->Draw(" same Weight");
  cWmeC2->Print(outputpdf);

  TCanvas *cWmeL11 = new TCanvas("cWmeL11", "cWmeL11");
  cWmeL11->Divide(2, 2);
  cWmeL11->cd(1);
  hW1meL1->Add(hW1meL1R, -1);
  hW1meL1->Draw("Weight");
  cWmeL11->cd(2);
  hW2meL1->Add(hW2meL1R, -1);
  hW2meL1->Draw(" same Weight");
  cWmeL11->cd(3);
  hW3meL1->Add(hW3meL1R, -1);
  hW3meL1->Draw(" same Weight");
  cWmeL11->cd(4);
  hW4meL1->Add(hW4meL1R, -1);
  hW4meL1->Draw(" same Weight");
  cWmeL11->Print(outputpdf);
  
  TCanvas *cWmeL12 = new TCanvas("cWmeL12", "cWmeL12");
  cWmeL12->Divide(2, 2);
  cWmeL12->cd(1);
  hW5meL1->Add(hW5meL1R, -1);
  hW5meL1->Draw("Weight");
  cWmeL12->cd(2);
  hW6meL1->Add(hW6meL1R, -1);
  hW6meL1->Draw(" same Weight");
  cWmeL12->cd(3);
  hW7meL1->Add(hW7meL1R, -1);
  hW7meL1->Draw(" same Weight");
  cWmeL12->cd(4);
  hW8meL1->Add(hW8meL1R, -1);
  hW8meL1->Draw(" same Weight");
  cWmeL12->Print(outputpdf);

  TCanvas *cWmeL21 = new TCanvas("cWmeL21", "cWmeL21");
  cWmeL21->Divide(2, 2);
  cWmeL21->cd(1);
  hW1meL2->Add(hW1meL2R, -1);
  hW1meL2->Draw("Weight");
  cWmeL21->cd(2);
  hW2meL2->Add(hW2meL2R, -1);
  hW2meL2->Draw(" same Weight");
  cWmeL21->cd(3);
  hW3meL2->Add(hW3meL2R, -1);
  hW3meL2->Draw(" same Weight");
  cWmeL21->cd(4);
  hW4meL2->Add(hW4meL2R, -1);
  hW4meL2->Draw(" same Weight");
  cWmeL21->Print(outputpdf);
  
  TCanvas *cWmeL22 = new TCanvas("cWmeL22", "cWmeL22");
  cWmeL22->Divide(2, 2);
  cWmeL22->cd(1);
  hW5meL2->Add(hW5meL2R, -1);
  hW5meL2->Draw("Weight");
  cWmeL22->cd(2);
  hW6meL2->Add(hW6meL2R, -1);
  hW6meL2->Draw(" same Weight");
  cWmeL22->cd(3);
  hW7meL2->Add(hW7meL2R, -1);
  hW7meL2->Draw(" same Weight");
  cWmeL22->cd(4);
  hW8meL2->Add(hW8meL2R, -1);
  hW8meL2->Draw(" same Weight");
  cWmeL22->Print(outputpdf);

  TCanvas *cQ2meC1 = new TCanvas("cQ2meC1", "cQ2meC1");

  cQ2meC1->Divide(2, 2);
  cQ2meC1->cd(1);
  hQ21meC->Add(hQ21meCR, -1); 
  hQ21meC->Draw(" Weight");
  cQ2meC1->cd(2);
  hQ22meC->Add(hQ22meCR, -1); 
  hQ22meC->Draw("same Weight");
  cQ2meC1->cd(3);
  hQ23meC->Add(hQ23meCR, -1); 
  hQ23meC->Draw("same Weight");
  cQ2meC1->cd(4);
  hQ24meC->Add(hQ24meCR, -1); 
  hQ24meC->Draw("same Weight");
  cQ2meC1->Print(outputpdf);

  TCanvas *cQ2meC2 = new TCanvas("cQ2meC2", "cQ2meC2");

  cQ2meC2->Divide(2, 2);
  cQ2meC2->cd(1);
  hQ25meC->Add(hQ25meCR, -1); 
  hQ25meC->Draw(" Weight");
  cQ2meC2->cd(2);
  hQ26meC->Add(hQ26meCR, -1); 
  hQ26meC->Draw("same Weight");
  cQ2meC2->cd(3);
  hQ27meC->Add(hQ27meCR, -1); 
  hQ27meC->Draw("same Weight");
  cQ2meC2->cd(4);
  hQ28meC->Add(hQ28meCR, -1); 
  hQ28meC->Draw("same Weight");
  cQ2meC2->Print(outputpdf);

  TCanvas *cQ2meL11 = new TCanvas("cQ2meL11", "cQ2meL11");

  cQ2meL11->Divide(2, 2);
  cQ2meL11->cd(1);
  hQ21meL1->Add(hQ21meL1R, -1); 
  hQ21meL1->Draw(" Weight");
  cQ2meL11->cd(2);
  hQ22meL1->Add(hQ22meL1R, -1); 
  hQ22meL1->Draw("same Weight");
  cQ2meL11->cd(3);
  hQ23meL1->Add(hQ23meL1R, -1); 
  hQ23meL1->Draw("same Weight");
  cQ2meL11->cd(4);
  hQ24meL1->Add(hQ24meL1R, -1); 
  hQ24meL1->Draw("same Weight");
  cQ2meL11->Print(outputpdf);

  TCanvas *cQ2meL12 = new TCanvas("cQ2meL12", "cQ2meL12");

  cQ2meL12->Divide(2, 2);
  cQ2meL12->cd(1);
  hQ25meL1->Add(hQ25meL1R, -1); 
  hQ25meL1->Draw(" Weight");
  cQ2meL12->cd(2);
  hQ26meL1->Add(hQ26meL1R, -1); 
  hQ26meL1->Draw("same Weight");
  cQ2meL12->cd(3);
  hQ27meL1->Add(hQ27meL1R, -1); 
  hQ27meL1->Draw("same Weight");
  cQ2meL12->cd(4);
  hQ28meL1->Add(hQ28meL1R, -1); 
  hQ28meL1->Draw("same Weight");
  cQ2meL12->Print(outputpdf);

  TCanvas *cQ2meL21 = new TCanvas("cQ2meL21", "cQ2meL21");

  cQ2meL21->Divide(2, 2);
  cQ2meL21->cd(1);
  hQ21meL2->Add(hQ21meL2R, -1); 
  hQ21meL2->Draw(" Weight");
  cQ2meL21->cd(2);
  hQ22meL2->Add(hQ22meL2R, -1); 
  hQ22meL2->Draw("same Weight");
  cQ2meL21->cd(3);
  hQ23meL2->Add(hQ23meL2R, -1); 
  hQ23meL2->Draw("same Weight");
  cQ2meL21->cd(4);
  hQ24meL2->Add(hQ24meL2R, -1); 
  hQ24meL2->Draw("same Weight");
  cQ2meL21->Print(outputpdf);

  TCanvas *cQ2meL22 = new TCanvas("cQ2meL22", "cQ2meL22");

  cQ2meL22->Divide(2, 2);
  cQ2meL22->cd(1);
  hQ25meL2->Add(hQ25meL2R, -1); 
  hQ25meL2->Draw(" Weight");
  cQ2meL22->cd(2);
  hQ26meL2->Add(hQ26meL2R, -1); 
  hQ26meL2->Draw("same Weight");
  cQ2meL22->cd(3);
  hQ27meL2->Add(hQ27meL2R, -1); 
  hQ27meL2->Draw("same Weight");
  cQ2meL22->cd(4);
  hQ28meL2->Add(hQ28meL2R, -1); 
  hQ28meL2->Draw("same Weight");
  cQ2meL22->Print(outputpdf);

  TCanvas *ctmeC1 = new TCanvas("ctmeC1", "ctmeC1");
  ctmeC1->Divide(2, 2);
  ctmeC1->cd(1);
  ht1meC->Add(ht1meCR, -1);
  ht1meC->Draw("Weight");
  ctmeC1->cd(2);
  ht2meC->Add(ht2meCR, -1);
  ht2meC->Draw("same Weight");
  ctmeC1->cd(3);
  ht3meC->Add(ht3meCR, -1);
  ht3meC->Draw("same Weight");
  ctmeC1->cd(4);
  ht4meC->Add(ht4meCR, -1);
  ht4meC->Draw("same Weight");
  ctmeC1->Print(outputpdf);

  TCanvas *ctmeC2 = new TCanvas("ctmeC2", "ctmeC2");
  ctmeC2->Divide(2, 2);
  ctmeC2->cd(1);
  ht5meC->Add(ht5meCR, -1);
  ht5meC->Draw("Weight");
  ctmeC2->cd(2);
  ht6meC->Add(ht6meCR, -1);
  ht6meC->Draw("same Weight");
  ctmeC2->cd(3);
  ht7meC->Add(ht7meCR, -1);
  ht7meC->Draw("same Weight");
  ctmeC2->cd(4);
  ht8meC->Add(ht8meCR, -1);
  ht8meC->Draw("same Weight");
  ctmeC2->Print(outputpdf);

  TCanvas *ctmeL11 = new TCanvas("ctmeL11", "ctmeL11");
  ctmeL11->Divide(2, 2);
  ctmeL11->cd(1);
  ht1meL1->Add(ht1meL1R, -1);
  ht1meL1->Draw("Weight");
  ctmeL11->cd(2);
  ht2meL1->Add(ht2meL1R, -1);
  ht2meL1->Draw("same Weight");
  ctmeL11->cd(3);
  ht3meL1->Add(ht3meL1R, -1);
  ht3meL1->Draw("same Weight");
  ctmeL11->cd(4);
  ht4meL1->Add(ht4meL1R, -1);
  ht4meL1->Draw("same Weight");
  ctmeL11->Print(outputpdf);

  TCanvas *ctmeL12 = new TCanvas("ctmeL12", "ctmeL12");
  ctmeL12->Divide(2, 2);
  ctmeL12->cd(1);
  ht5meL1->Add(ht5meL1R, -1);
  ht5meL1->Draw("Weight");
  ctmeL12->cd(2);
  ht6meL1->Add(ht6meL1R, -1);
  ht6meL1->Draw("same Weight");
  ctmeL12->cd(3);
  ht7meL1->Add(ht7meL1R, -1);
  ht7meL1->Draw("same Weight");
  ctmeL12->cd(4);
  ht8meL1->Add(ht8meL1R, -1);
  ht8meL1->Draw("same Weight");
  ctmeL12->Print(outputpdf);

  TCanvas *ctmeL21 = new TCanvas("ctmeL21", "ctmeL21");
  ctmeL21->Divide(2, 2);
  ctmeL21->cd(1);
  ht1meL2->Add(ht1meL2R, -1);
  ht1meL2->Draw("Weight");
  ctmeL21->cd(2);
  ht2meL2->Add(ht2meL2R, -1);
  ht2meL2->Draw("same Weight");
  ctmeL21->cd(3);
  ht3meL2->Add(ht3meL2R, -1);
  ht3meL2->Draw("same Weight");
  ctmeL21->cd(4);
  ht4meL2->Add(ht4meL2R, -1);
  ht4meL2->Draw("same Weight");
  ctmeL21->Print(outputpdf);

  TCanvas *ctmeL22 = new TCanvas("ctmeL22", "ctmeL22");
  ctmeL22->Divide(2, 2);
  ctmeL22->cd(1);
  ht5meL2->Add(ht5meL2R, -1);
  ht5meL2->Draw("Weight");
  ctmeL22->cd(2);
  ht6meL2->Add(ht6meL2R, -1);
  ht6meL2->Draw("same Weight");
  ctmeL22->cd(3);
  ht7meL2->Add(ht7meL2R, -1);
  ht7meL2->Draw("same Weight");
  ctmeL22->cd(4);
  ht8meL2->Add(ht8meL2R, -1);
  ht8meL2->Draw("same Weight");
  ctmeL22->Print(outputpdf);

  /**
     ofstream outmeC("kindata/kindata.pl_375_629_+0000.dat");
     ofstream outmeL1("kindata/kindata.pl_375_629_-2000.dat");
     ofstream outmeL2("kindata/kindata.pl_375_629_-4000.dat");

     ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
     ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
     ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
  **/

  ofstream outmeC("kindata/kindata.pl_425_617_+0000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outmeC<<1.000<<endl;
      if(i==1)
	outmeC<<hW1meC->GetMean()<<"\t"<<hW1meC->GetMeanError()<<"\t"<<hQ21meC->GetMean()<<"\t"<<hQ21meC->GetMeanError()<<"\t"<<ht1meC->GetMean()<<"\t"<<ht1meC->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outmeC<<hW2meC->GetMean()<<"\t"<<hW2meC->GetMeanError()<<"\t"<<hQ22meC->GetMean()<<"\t"<< hQ22meC->GetMeanError()<<"\t"<<ht2meC->GetMean()<<"\t"<<ht2meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outmeC<<hW3meC->GetMean()<<"\t"<<hW3meC->GetMeanError()<<"\t"<<hQ23meC->GetMean()<<"\t"<< hQ23meC->GetMeanError()<<"\t"<<ht3meC->GetMean()<<"\t"<<ht3meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outmeC<<hW4meC->GetMean()<<"\t"<<hW4meC->GetMeanError()<<"\t"<<hQ24meC->GetMean()<<"\t"<< hQ24meC->GetMeanError()<<"\t"<<ht4meC->GetMean()<<"\t"<<ht4meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outmeC<<hW5meC->GetMean()<<"\t"<<hW5meC->GetMeanError()<<"\t"<<hQ25meC->GetMean()<<"\t"<< hQ25meC->GetMeanError()<<"\t"<<ht5meC->GetMean()<<"\t"<<ht5meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outmeC<<hW6meC->GetMean()<<"\t"<<hW6meC->GetMeanError()<<"\t"<<hQ26meC->GetMean()<<"\t"<< hQ26meC->GetMeanError()<<"\t"<<ht6meC->GetMean()<<"\t"<<ht6meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outmeC<<hW7meC->GetMean()<<"\t"<<hW7meC->GetMeanError()<<"\t"<<hQ27meC->GetMean()<<"\t"<< hQ27meC->GetMeanError()<<"\t"<<ht7meC->GetMean()<<"\t"<<ht7meC->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outmeC<<hW8meC->GetMean()<<"\t"<<hW8meC->GetMeanError()<<"\t"<<hQ28meC->GetMean()<<"\t"<< hQ28meC->GetMeanError()<<"\t"<<ht8meC->GetMean()<<"\t"<<ht8meC->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outmeC<<1.000000<<endl;
    } 
  outmeC.close();

  ofstream outmeL1("kindata/kindata.pl_425_617_-2000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outmeL1<<1.000<<endl;
      if(i==1)
	outmeL1<<hW1meL1->GetMean()<<"\t"<<hW1meL1->GetMeanError()<<"\t"<<hQ21meL1->GetMean()<<"\t"<<hQ21meL1->GetMeanError()<<"\t"<<ht1meL1->GetMean()<<"\t"<<ht1meL1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outmeL1<<hW2meL1->GetMean()<<"\t"<<hW2meL1->GetMeanError()<<"\t"<<hQ22meL1->GetMean()<<"\t"<< hQ22meL1->GetMeanError()<<"\t"<<ht2meL1->GetMean()<<"\t"<<ht2meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outmeL1<<hW3meL1->GetMean()<<"\t"<<hW3meL1->GetMeanError()<<"\t"<<hQ23meL1->GetMean()<<"\t"<< hQ23meL1->GetMeanError()<<"\t"<<ht3meL1->GetMean()<<"\t"<<ht3meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outmeL1<<hW4meL1->GetMean()<<"\t"<<hW4meL1->GetMeanError()<<"\t"<<hQ24meL1->GetMean()<<"\t"<< hQ24meL1->GetMeanError()<<"\t"<<ht4meL1->GetMean()<<"\t"<<ht4meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outmeL1<<hW5meL1->GetMean()<<"\t"<<hW5meL1->GetMeanError()<<"\t"<<hQ25meL1->GetMean()<<"\t"<< hQ25meL1->GetMeanError()<<"\t"<<ht5meL1->GetMean()<<"\t"<<ht5meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outmeL1<<hW6meL1->GetMean()<<"\t"<<hW6meL1->GetMeanError()<<"\t"<<hQ26meL1->GetMean()<<"\t"<< hQ26meL1->GetMeanError()<<"\t"<<ht6meL1->GetMean()<<"\t"<<ht6meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outmeL1<<hW7meL1->GetMean()<<"\t"<<hW7meL1->GetMeanError()<<"\t"<<hQ27meL1->GetMean()<<"\t"<< hQ27meL1->GetMeanError()<<"\t"<<ht7meL1->GetMean()<<"\t"<<ht7meL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outmeL1<<hW8meL1->GetMean()<<"\t"<<hW8meL1->GetMeanError()<<"\t"<<hQ28meL1->GetMean()<<"\t"<< hQ28meL1->GetMeanError()<<"\t"<<ht8meL1->GetMean()<<"\t"<<ht8meL1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outmeL1<<1.000000<<endl;
    } 
  outmeL1.close();

  ofstream outmeL2("kindata/kindata.pl_425_617_-4000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outmeL2<<1.000<<endl;
      if(i==1)
	outmeL2<<hW1meL2->GetMean()<<"\t"<<hW1meL2->GetMeanError()<<"\t"<<hQ21meL2->GetMean()<<"\t"<<hQ21meL2->GetMeanError()<<"\t"<<ht1meL2->GetMean()<<"\t"<<ht1meL2->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outmeL2<<hW2meL2->GetMean()<<"\t"<<hW2meL2->GetMeanError()<<"\t"<<hQ22meL2->GetMean()<<"\t"<< hQ22meL2->GetMeanError()<<"\t"<<ht2meL2->GetMean()<<"\t"<<ht2meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outmeL2<<hW3meL2->GetMean()<<"\t"<<hW3meL2->GetMeanError()<<"\t"<<hQ23meL2->GetMean()<<"\t"<< hQ23meL2->GetMeanError()<<"\t"<<ht3meL2->GetMean()<<"\t"<<ht3meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outmeL2<<hW4meL2->GetMean()<<"\t"<<hW4meL2->GetMeanError()<<"\t"<<hQ24meL2->GetMean()<<"\t"<< hQ24meL2->GetMeanError()<<"\t"<<ht4meL2->GetMean()<<"\t"<<ht4meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outmeL2<<hW5meL2->GetMean()<<"\t"<<hW5meL2->GetMeanError()<<"\t"<<hQ25meL2->GetMean()<<"\t"<< hQ25meL2->GetMeanError()<<"\t"<<ht5meL2->GetMean()<<"\t"<<ht5meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outmeL2<<hW6meL2->GetMean()<<"\t"<<hW6meL2->GetMeanError()<<"\t"<<hQ26meL2->GetMean()<<"\t"<< hQ26meL2->GetMeanError()<<"\t"<<ht6meL2->GetMean()<<"\t"<<ht6meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outmeL2<<hW7meL2->GetMean()<<"\t"<<hW7meL2->GetMeanError()<<"\t"<<hQ27meL2->GetMean()<<"\t"<< hQ27meL2->GetMeanError()<<"\t"<<ht7meL2->GetMean()<<"\t"<<ht7meL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outmeL2<<hW8meL2->GetMean()<<"\t"<<hW8meL2->GetMeanError()<<"\t"<<hQ28meL2->GetMean()<<"\t"<< hQ28meL2->GetMeanError()<<"\t"<<ht8meL2->GetMean()<<"\t"<<ht8meL2->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outmeL2<<1.000000<<endl;
    } 
  outmeL2.close();

  TCanvas *c10me = new TCanvas("c10me", " c10me"); 
  
  hph_qmeC->SetStats(0);
  hph_qmeC->SetLineColor(kGreen);
  hph_qmeC->Add(hph_qmeCR, -1);
  hph_qmeC->Draw("same Weight");

  hph_qmeL2->SetStats(0);
  hph_qmeL2->SetLineColor(kBlack);
  hph_qmeL2->Add(hph_qmeL2R, -1);
  hph_qmeL2->Draw("same Weight");
  
  hph_qmeL1->SetStats(0);
  hph_qmeL1->SetLineColor(kRed);
  hph_qmeL1->Draw("same Weight");
 
  c10me->Print(outputpdf);

  TCanvas *phmeC = new TCanvas("phmeC", "phmeC"); 
  //  hphmeSC->SetStats(0);
  hphmeSC->Scale(SCFMEC);
  TF1 *gSmeC = new TF1("gSmeC","gaus(0)",0.936,0.942);    
  gSmeC->SetLineColor(kRed);
  //  hphmeSC->Fit("gSmeC", "R");
  hphmeSC->Draw("");
  // hphmeC->SetStats(0);
  hphmeC->SetLineColor(kGreen);
  hphmeC->Add(hphmeCR, -1);
  hphmeC->Scale(ECMEC);
  TF1 *gmeC = new TF1("gmeC","gaus(0)",0.940,0.944);    
  gmeC->SetLineColor(kRed);
  //  hphmeC->Fit("gmeC", "R+");
  hphmeC->Draw("same");
  phmeC->Print(outputpdf);

  TCanvas *phmeRC = new TCanvas("phmeRC", "phmeRC"); 
  hphmeC->Divide(hphmeSC);
  hphmeC->Draw();
  phmeRC->Print(outputpdf);

  TCanvas *phmeL1 = new TCanvas("phmeL1", "phmeL1"); 
  //  hphmeSL1->SetStats(0);
  hphmeSL1->Scale(SCFMEL1);
  hphmeSL1->Draw("");
  // hphmeL1->SetStats(0);
  hphmeL1->SetLineColor(kGreen);
  hphmeL1->Add(hphmeL1R, -1);
  hphmeL1->Scale(ECMEL1);
  hphmeL1->Draw("same");
  phmeL1->Print(outputpdf);

  TCanvas *phmeRL1 = new TCanvas("phmeRL1", "phmeRL1"); 
  hphmeL1->Divide(hphmeSL1);
  hphmeL1->Draw();
  phmeRL1->Print(outputpdf);

  TCanvas *phmeL2 = new TCanvas("phmeL2", "phmeL2"); 
  //  hphmeSL2->SetStats(0);
  hphmeSL2->Scale(SCFMEL2);
  hphmeSL2->Draw("");
  // hphmeL2->SetStats(0);
  hphmeL2->SetLineColor(kGreen);
  hphmeL2->Add(hphmeL2R, -1);
  hphmeL2->Scale(ECMEL2);
  hphmeL2->Draw("same");
  phmeL2->Print(outputpdf);

  TCanvas *phmeRL2 = new TCanvas("phmeRL2", "phmeRL2"); 
  hphmeL2->Divide(hphmeSL2);
  hphmeL2->Draw();
  phmeRL2->Print(outputpdf);

  TCanvas *c11me = new TCanvas("c11me", " c11me"); 

  hmmmeL1->SetStats(0);
  hmmmeL1->SetLineColor(kRed);
  hmmmeL1->Add(hmmmeL1R, -1);
  hmmmeL1->Draw(" Weight");

  hmmmeC->SetStats(0);
  hmmmeC->SetLineColor(kGreen);
  hmmmeC->Add(hmmmeCR, -1);
  hmmmeC->Draw("same Weight");

  hmmmeL2->SetStats(0);
  hmmmeL2->SetLineColor(kBlack);
  hmmmeL2->Add(hmmmeL2R, -1);
  hmmmeL2->Draw("same Weight");
  	    
  TLine *l7me = new TLine(0.92, -1300.0, 0.92, 29000.0);
  TLine *l8me = new TLine(0.98, -1300.0, 0.98, 29000.0);
  l7me->SetLineColor(kBlue);
  l8me->SetLineColor(kBlue);
  l7me->Draw("same"); 
  l8me->Draw("same");
  c11me->Print(outputpdf);
	    	    
  TCanvas *c12me = new TCanvas("c12me", " c12me");
  c12me->Divide(2, 0, 0.05);
  c12me->cd(1); 
  hQ2WMEC->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  c12me->cd(2); 
  hQ2WMEC1->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");
  c12me->Print(outputpdf);

  /*
  TCanvas *cdeltaSmeC = new TCanvas("cdeltaSmeC", " cdeltaSmeC");
  // H_ssdelta_SmeC->SetStats(0);
  H_ssdelta_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SmeC->Scale(SCFMEC);
  // H_ssdelta_SmeC->Draw("");
  H_ssdelta_DmeC->Add(H_ssdelta_DmeCR, -1);
  H_ssdelta_DmeC->Scale(ECMEC);
  H_ssdelta_DmeC->SetLineColor(kGreen);
  H_ssdelta_DmeC->Draw("same"); 
  cdeltaSmeC->Print(outputpdf);
  */
  TCanvas *cdeltaSmeL1 = new TCanvas("cdeltaSmeL1", " cdeltaSmeL1");
  // H_ssdelta_SmeL1->SetStats(0);
  H_ssdelta_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SmeL1->Scale(SCFMEL1);
  H_ssdelta_SmeL1->Draw("");
  H_ssdelta_DmeL1->Add(H_ssdelta_DmeL1R, -1);
  H_ssdelta_DmeL1->Scale(ECMEL1);
  H_ssdelta_DmeL1->SetLineColor(kGreen);
  H_ssdelta_DmeL1->Draw("same"); 
  cdeltaSmeL1->Print(outputpdf);

  TCanvas *cdeltaSmeL2 = new TCanvas("cdeltaSmeL2", " cdeltaSmeL2");
  //  H_ssdelta_SmeL2->SetStats(0);
  H_ssdelta_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SmeL2->Scale(SCFMEL2);
  H_ssdelta_SmeL2->Draw("");
  H_ssdelta_DmeL2->Add(H_ssdelta_DmeL2R, -1);
  H_ssdelta_DmeL2->Scale(ECMEL2);
  H_ssdelta_DmeL2->SetLineColor(kGreen);
  H_ssdelta_DmeL2->Draw("same"); 
  cdeltaSmeL2->Print(outputpdf);

  /*
  TCanvas *cdeltaHmeC = new TCanvas("cdeltaHmeC", " cdeltaHmeC");
  // H_hsdelta_SmeC->SetStats(0);
  H_hsdelta_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SmeC->Scale(SCFMEC);
  H_hsdelta_SmeC->Draw("");
  H_hsdelta_DmeC->Add(H_hsdelta_DmeCR, -1);
  H_hsdelta_DmeC->Scale(ECMEC);
  H_hsdelta_DmeC->SetLineColor(kGreen);
  H_hsdelta_DmeC->Draw("same"); 
  cdeltaHmeC->Print(outputpdf);
  */
  TCanvas *cdeltaHmeL1 = new TCanvas("cdeltaHmeL1", " cdeltaHmeL1");
  // H_hsdelta_SmeL1->SetStats(0);
  H_hsdelta_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SmeL1->Scale(SCFMEL1);
  H_hsdelta_SmeL1->Draw("");
  H_hsdelta_DmeL1->Add(H_hsdelta_DmeL1R, -1);
  H_hsdelta_DmeL1->Scale(ECMEL1);
  H_hsdelta_DmeL1->SetLineColor(kGreen);
  H_hsdelta_DmeL1->Draw("same"); 
  cdeltaHmeL1->Print(outputpdf);

  TCanvas *cdeltaHmeL2 = new TCanvas("cdeltaHmeL2", " cdeltaHmeL2");
  // H_hsdelta_SmeL2->SetStats(0);
  H_hsdelta_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SmeL2->Scale(SCFMEL2);
  H_hsdelta_SmeL2->Draw("");
  H_hsdelta_DmeL2->Add(H_hsdelta_DmeL2R, -1);
  H_hsdelta_DmeL2->Scale(ECMEL2);
  H_hsdelta_DmeL2->SetLineColor(kGreen);
  H_hsdelta_DmeL2->Draw("same"); 
  cdeltaHmeL2->Print(outputpdf);
  /*
  TCanvas *cxptarSmeC = new TCanvas("cxptarSmeC", " cxptarSmeC");
  //  H_ssxptar_SmeC->SetStats(0);
  H_ssxptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SmeC->Scale(SCFMEC);
  H_ssxptar_SmeC->Draw("");
  H_ssxptar_DmeC->Add(H_ssxptar_DmeCR, -1);
  H_ssxptar_DmeC->Scale(ECMEC);
  H_ssxptar_DmeC->SetLineColor(kGreen);
  H_ssxptar_DmeC->Draw("same");
  cxptarSmeC->Print(outputpdf);
  */
  TCanvas *cxptarSmeL1 = new TCanvas("cxptarSmeL1", " cxptarSmeL1");
  // H_ssxptar_SmeL1->SetStats(0);
  H_ssxptar_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SmeL1->Scale(SCFMEL1);
  H_ssxptar_SmeL1->Draw("");
  H_ssxptar_DmeL1->Add(H_ssxptar_DmeL1R, -1);
  H_ssxptar_DmeL1->Scale(ECMEL1);
  H_ssxptar_DmeL1->SetLineColor(kGreen);
  H_ssxptar_DmeL1->Draw("same");
  cxptarSmeL1->Print(outputpdf);

  TCanvas *cxptarSmeL2 = new TCanvas("cxptarSmeL2", " cxptarSmeL2");
  // H_ssxptar_SmeL2->SetStats(0);
  H_ssxptar_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SmeL2->Scale(SCFMEL2);
  H_ssxptar_SmeL2->Draw("");
  H_ssxptar_DmeL2->Add(H_ssxptar_DmeL2R, -1);
  H_ssxptar_DmeL2->Scale(ECMEL2);
  H_ssxptar_DmeL2->SetLineColor(kGreen);
  H_ssxptar_DmeL2->Draw("same");
  cxptarSmeL2->Print(outputpdf);
  /*
  TCanvas *cyptarSmeC = new TCanvas("cyptarSmeC", " cyptarSmeC");
  // H_ssyptar_SmeC->SetStats(0);
  H_ssyptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SmeC->Scale(SCFMEC);
  H_ssyptar_SmeC->Draw("");
  H_ssyptar_DmeC->Add(H_ssyptar_DmeCR, -1);
  H_ssyptar_DmeC->Scale(ECMEC);
  H_ssyptar_DmeC->SetLineColor(kGreen);
  H_ssyptar_DmeC->Draw("same");
  cyptarSmeC->Print(outputpdf);
  */
  TCanvas *cyptarSmeL1 = new TCanvas("cyptarSmeL1", " cyptarSmeL1");
  // H_ssyptar_SmeL1->SetStats(0);
  H_ssyptar_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SmeL1->Scale(SCFMEL1);
  H_ssyptar_SmeL1->Draw("");
  H_ssyptar_DmeL1->Add(H_ssyptar_DmeL1R, -1);
  H_ssyptar_DmeL1->Scale(ECMEL1);
  H_ssyptar_DmeL1->SetLineColor(kGreen);
  H_ssyptar_DmeL1->Draw("same");
  cyptarSmeL1->Print(outputpdf);

  TCanvas *cyptarSmeL2 = new TCanvas("cyptarSmeL2", " cyptarSmeL2");
  //  H_ssyptar_SmeL2->SetStats(0);
  H_ssyptar_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SmeL2->Scale(SCFMEL2);
  H_ssyptar_SmeL2->Draw("");
  H_ssyptar_DmeL2->Add(H_ssyptar_DmeL2R, -1);
  H_ssyptar_DmeL2->Scale(ECMEL2);
  H_ssyptar_DmeL2->SetLineColor(kGreen);
  H_ssyptar_DmeL2->Draw("same");
  cyptarSmeL2->Print(outputpdf);
  /*
  TCanvas *cxptarHmeC = new TCanvas("cxptarHmeC", " cxptarHmeC");
  // H_hsxptar_SmeC->SetStats(0);
  H_hsxptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SmeC->Scale(SCFMEC);
  H_hsxptar_SmeC->Draw("");
  H_hsxptar_DmeC->Add(H_hsxptar_DmeCR, -1);
  H_hsxptar_DmeC->Scale(ECMEC);
  H_hsxptar_DmeC->SetLineColor(kGreen);
  H_hsxptar_DmeC->Draw("same");
  cxptarHmeC->Print(outputpdf);
  */
  TCanvas *cxptarHmeL1 = new TCanvas("cxptarHmeL1", " cxptarHmeL1");
  // H_hsxptar_SmeL1->SetStats(0);
  H_hsxptar_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SmeL1->Scale(SCFMEL1);
  H_hsxptar_SmeL1->Draw("");
  H_hsxptar_DmeL1->Add(H_hsxptar_DmeL1R, -1);
  H_hsxptar_DmeL1->Scale(ECMEL1);
  H_hsxptar_DmeL1->SetLineColor(kGreen);
  H_hsxptar_DmeL1->Draw("same");
  cxptarHmeL1->Print(outputpdf);

  TCanvas *cxptarHmeL2 = new TCanvas("cxptarHmeL2", " cxptarHmeL2");
  // H_hsxptar_SmeL2->SetStats(0);
  H_hsxptar_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SmeL2->Scale(SCFMEL2);
  H_hsxptar_SmeL2->Draw("");
  H_hsxptar_DmeL2->Add(H_hsxptar_DmeL2R, -1);
  H_hsxptar_DmeL2->Scale(ECMEL2);
  H_hsxptar_DmeL2->SetLineColor(kGreen);
  H_hsxptar_DmeL2->Draw("same");
  cxptarHmeL2->Print(outputpdf);
  /*
  TCanvas *cyptarHmeC = new TCanvas("cyptarHmeC", " cyptarHmeC");
  //  H_hsyptar_SmeC->SetStats(0);
  H_hsyptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SmeC->Scale(SCFMEC);
  H_hsyptar_SmeC->Draw("");
  H_hsyptar_DmeC->Add(H_hsyptar_DmeCR, -1);
  H_hsyptar_DmeC->Scale(ECMEC);
  H_hsyptar_DmeC->SetLineColor(kGreen);
  H_hsyptar_DmeC->Draw("same");
  cyptarHmeC->Print(outputpdf);
  */
  TCanvas *cyptarHmeL1 = new TCanvas("cyptarHmeL1", " cyptarHmeL1");
  // H_hsyptar_SmeL1->SetStats(0);
  H_hsyptar_SmeL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SmeL1->Scale(SCFMEL1);
  H_hsyptar_SmeL1->Draw("");
  H_hsyptar_DmeL1->Add(H_hsyptar_DmeL1R, -1);
  H_hsyptar_DmeL1->Scale(ECMEL1);
  H_hsyptar_DmeL1->SetLineColor(kGreen);
  H_hsyptar_DmeL1->Draw("same");
  cyptarHmeL1->Print(outputpdf);

  TCanvas *cyptarHmeL2 = new TCanvas("cyptarHmeL2", " cyptarHmeL2");
  // H_hsyptar_SmeL2->SetStats(0);
  H_hsyptar_SmeL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SmeL2->Scale(SCFMEL2);
  H_hsyptar_SmeL2->Draw("");
  H_hsyptar_DmeL2->Add(H_hsyptar_DmeL2R, -1);
  H_hsyptar_DmeL2->Scale(ECMEL2);
  H_hsyptar_DmeL2->SetLineColor(kGreen);
  H_hsyptar_DmeL2->Draw("same");
  cyptarHmeL2->Print(outputpdf);

  TCanvas *cYmeC = new TCanvas("cYmeC", " cYmeC");
  hYmeC->Add(hYmeCR, -1);  
  hYmeC->Scale(ECMEC);  
  hYmeC->SetStats(0);
  hYmeC->Draw("COLZ");
  cYmeC->Print(outputpdf);

  TCanvas *cYSmeC = new TCanvas("cYSmeC", " cYSmeC");
  hYSmeC->SetStats(0);
  hYSmeC->Scale(SCFMEC);
  hYSmeC->Draw("COLZ");
  cYSmeC->Print(outputpdf);
  ofstream YmeC("yield/yields.pl_425_617_+0000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YmeC << 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YmeC << 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YmeC<<hYmeC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YmeC.close();
  //Dummy
  hYmedC->Add(hYmedCR, -1);  
  hYmedC->Scale(ECMEDC);    //Yield  
  hYmedC->Scale(THCF);     // Thickness corrected Yield

  ofstream YdmeC("yield/yields.pl_425_617_+0000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdmeC << 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdmeC << 1.0000<<endl;	
	  break;
	}  
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdmeC<<hYmedC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdmeC.close();
  ofstream YSmeC("yield/yields.pl_425_617_+0000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSmeC << 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSmeC << 1.0000<<endl;	
	  break;
	}  
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSmeC<<hYSmeC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSmeC.close();

  TCanvas *cYmeL1 = new TCanvas("cYmeL1", " cYmeL1");
  hYmeL1->Add(hYmeL1R, -1);  
  hYmeL1->Scale(ECMEL1);  
  hYmeL1->SetStats(0);
  hYmeL1->Draw("COLZ");
  cYmeL1->Print(outputpdf);

  TCanvas *cYSmeL1 = new TCanvas("cYSmeL1", " cYSmeL1");
  hYSmeL1->SetStats(0);
  hYSmeL1->Scale(SCFMEL1);
  hYSmeL1->Draw("COLZ");
  cYSmeL1->Print(outputpdf);
  ofstream YmeL1("yield/yields.pl_425_617_-2000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YmeL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YmeL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YmeL1<<hYmeL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YmeL1.close();
  //Dummy
  hYmedL1->Add(hYmedL1R, -1);  
  hYmedL1->Scale(ECMEDL1);     // Yield  
  hYmedL1->Scale(THCF);       // Thickness corrected Yield

  ofstream YdmeL1("yield/yields.pl_425_617_-2000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdmeL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdmeL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdmeL1<<hYmedL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdmeL1.close();
  ofstream YSmeL1("yield/yields.pl_425_617_-2000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSmeL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSmeL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSmeL1<<hYSmeL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSmeL1.close();

  TCanvas *cYmeL2 = new TCanvas("cYmeL2", " cYmeL2");
  hYmeL2->Add(hYmeL2R, -1);  
  hYmeL2->Scale(ECMEL2);  
  hYmeL2->SetStats(0);
  hYmeL2->Draw("COLZ");
  cYmeL2->Print(outputpdf);

  TCanvas *cYSmeL2 = new TCanvas("cYSmeL2", " cYSmeL2");
  hYSmeL2->SetStats(0);
  hYSmeL2->Scale(SCFMEL2);
  hYSmeL2->Draw("COLZ");
  cYSmeL2->Print(outputpdf);
  ofstream YmeL2("yield/yields.pl_425_617_-4000.target");
  for(int j=1; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YmeL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YmeL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YmeL2<<hYmeL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YmeL2.close();
  //Dummy
  hYmedL2->Add(hYmedL2R, -1);  
  hYmedL2->Scale(ECMEDL2);    // Yield  
  hYmedL2->Scale(THCF);      // Thickness corrected Yield

  ofstream YdmeL2("yield/yields.pl_425_617_-4000.dummy");
  for(int j=1 ; j <=10; ++j) 
    {
      if (j==1)
	{
	  YdmeL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdmeL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdmeL2<<hYmedL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdmeL2.close();
  ofstream YSmeL2("yield/yields.pl_425_617_-4000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSmeL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSmeL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSmeL2<<hYSmeL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSmeL2.close();

  TH2D *Cloneme = (TH2D*)hYmeC->Clone("Cloneme");
  // Cloneme->Add(hYmeC,+1);
  Cloneme->Add(hYmeL1,+1);
  Cloneme->Add(hYmeL2,+1);
  Cloneme->SetTitle("Total Yield for mid epsilon");
  TCanvas *cYtme = new TCanvas("cYtme", " cYtme");
  Cloneme->Draw("COLZ");
  cYtme->Print(outputpdf);
 
 //thesis plots
  TCanvas *cthesisme = new TCanvas("cthesisme", " cthesisme");
  cthesisme->Divide(2,2);
  cthesisme->cd(1);
  hthQ2meC->Add(hthQ2meCR, -1);
  hthQ2meC->Scale(ECMEC);
  hthQ2medC->Add(hthQ2medCR, -1);
  hthQ2medC->Scale(ECMEDC);
  hthQ2meC->Add(hthQ2medC, -1);
  TAxis *XQ2meC = hthQ2meC->GetXaxis();
  Double_t InQ2meC = hthQ2meC->Integral(XQ2meC->FindBin(0.3), XQ2meC->FindBin(0.45), "");
  TAxis *XQ2SmeC = hthQ2SmeC->GetXaxis();
  Double_t InQ2SmeC = hthQ2SmeC->Integral(XQ2SmeC->FindBin(0.3), XQ2SmeC->FindBin(0.45), "");
  // hthQ2meC->Scale(InQ2SmeC/InQ2meC);
  hthQ2SmeC->SetStats(0);
  hthQ2SmeC->SetLineColor(kRed);
  hthQ2SmeC->Draw("hist"); 
  //hthQ2SmeC->Draw(""); 
  hthQ2meC->SetStats(0);
  hthQ2meC->Draw("same"); 

  cthesisme->cd(2);
  hthWmeC->Add(hthWmeCR, -1);
  hthWmeC->Scale(ECMEC);
  hthWmedC->Add(hthWmedCR, -1);
  hthWmedC->Scale(ECMEDC);
  hthWmeC->Add(hthWmedC, -1);
  TAxis *XWmeC = hthWmeC->GetXaxis();
  Double_t InWmeC = hthWmeC->Integral(XWmeC->FindBin(2.16), XWmeC->FindBin(2.23), "");
  TAxis *XWSmeC = hthWSmeC->GetXaxis();
  Double_t InWSmeC = hthWSmeC->Integral(XWSmeC->FindBin(2.16), XWSmeC->FindBin(2.23), "");
  // hthWmeC->Scale(InWSmeC/InWmeC);
  hthWSmeC->SetStats(0);
  hthWSmeC->SetLineColor(kRed);
  hthWSmeC->Draw("hist");
  //hthWSmeC->Draw("");
  hthWmeC->SetStats(0);
  hthWmeC->Draw("same");

  cthesisme->cd(3);
  hthtmeC->Add(hthtmeCR, -1);
  hthtmeC->Scale(ECMEC);
  hthtmedC->Add(hthtmedCR, -1);
  hthtmedC->Scale(ECMEDC);
  hthtmeC->Add(hthtmedC, -1);
  TAxis *XtmeC =  hthtmeC->GetXaxis();
  Double_t IntmeC = hthtmeC->Integral(XtmeC->FindBin(0.0), XtmeC->FindBin(0.06), "");
  TAxis *XtSmeC = hthtSmeC->GetXaxis();
  Double_t IntSmeC = hthtSmeC->Integral(XtSmeC->FindBin(0.0), XtSmeC->FindBin(0.06), "");
  // hthtmeC->Scale(IntSmeC/IntmeC);
  hthtSmeC->SetStats(0);
  hthtSmeC->SetLineColor(kRed);
  hthtSmeC->Draw("hist");
  //hthtSmeC->Draw("");
  hthtmeC->SetStats(0);
  hthtmeC->Draw("same");

  cthesisme->cd(4);
  hthepmeC->Add(hthepmeCR, -1);
  hthepmeC->Scale(ECMEC);
  hthepmedC->Add(hthepmedCR, -1);
  hthepmedC->Scale(ECMEDC);
  hthepmeC->Add(hthepmedC, -1);
  TAxis *XepmeC =  hthepmeC->GetXaxis();
  Double_t InepmeC = hthepmeC->Integral(XepmeC->FindBin(0.616), XepmeC->FindBin(0.65), "");
  TAxis *XepSmeC = hthepSmeC->GetXaxis();
  Double_t InepSmeC = hthepSmeC->Integral(XepSmeC->FindBin(0.616), XepSmeC->FindBin(0.65), "");
  // hthepmeC->Scale(InepSmeC/InepmeC);
  hthepSmeC->SetStats(0);
  hthepSmeC->SetLineColor(kRed);
  hthepSmeC->Draw("hist");
  //hthepSmeC->Draw("");
  hthepmeC->SetStats(0);
  hthepmeC->Draw("same");
  cthesisme->Print(outputpdf);

  TCanvas *cthesisAcc = new TCanvas("cthesisAcc", "cthesisAcc");
  cthesisAcc->Divide(2,3);
  cthesisAcc->cd(1);
  H_ssdelta_SmeC->SetStats(0);
  H_ssdelta_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SmeC->SetLineColor(kRed);
  H_ssdelta_SmeC->Draw("hist");
  //H_ssdelta_SmeC->Draw("");
  H_ssdelta_DmeC->Add(H_ssdelta_DmeCR, -1);
  H_ssdelta_DmeC->Scale(ECMEC);
  H_ssdelta_DmedC->Add(H_ssdelta_DmedCR, -1);
  H_ssdelta_DmedC->Scale(ECMEDC);
  H_ssdelta_DmeC->Add(H_ssdelta_DmedC, -1);
  //  H_ssdelta_DmeC->SetStats(0);
  H_ssdelta_DmeC->Draw("same"); 

  cthesisAcc->cd(2);
  H_ssxptar_SmeC->SetStats(0);
  H_ssxptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SmeC->SetLineColor(kRed);
  H_ssxptar_SmeC->Draw("hist");
  //H_ssxptar_SmeC->Draw("");
  H_ssxptar_DmeC->Add(H_ssxptar_DmeCR, -1);
  H_ssxptar_DmeC->Scale(ECMEC);
  H_ssxptar_DmedC->Add(H_ssxptar_DmedCR, -1);
  H_ssxptar_DmedC->Scale(ECMEDC);
  H_ssxptar_DmeC->Add(H_ssxptar_DmedC, -1);
  //  H_ssxptar_DmeC->SetStats(0);
  H_ssxptar_DmeC->Draw("same");

  cthesisAcc->cd(3);
  H_ssyptar_SmeC->SetStats(0);
  H_ssyptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SmeC->SetLineColor(kRed);
  H_ssyptar_SmeC->Draw("hist");
  //H_ssyptar_SmeC->Draw("");
  H_ssyptar_DmeC->Add(H_ssyptar_DmeCR, -1);
  H_ssyptar_DmeC->Scale(ECMEC);
  H_ssyptar_DmedC->Add(H_ssyptar_DmedCR, -1);
  H_ssyptar_DmedC->Scale(ECMEDC);
  H_ssyptar_DmeC->Add(H_ssyptar_DmedC, -1);
 //  H_ssyptar_DmeC->SetStats(0);
  H_ssyptar_DmeC->Draw("same");
 
  cthesisAcc->cd(4);
  H_hsdelta_SmeC->SetStats(0);
  H_hsdelta_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SmeC->SetLineColor(kRed);
  H_hsdelta_SmeC->Draw("hist");
  //H_hsdelta_SmeC->Draw("");
  H_hsdelta_DmeC->Add(H_hsdelta_DmeCR, -1);
  H_hsdelta_DmeC->Scale(ECMEC);
  H_hsdelta_DmedC->Add(H_hsdelta_DmedCR, -1);
  H_hsdelta_DmedC->Scale(ECMEDC);
  H_hsdelta_DmeC->Add(H_hsdelta_DmedC, -1);
  //  H_hsdelta_DmeC->SetStats(0);
  H_hsdelta_DmeC->Draw("same"); 

  cthesisAcc->cd(5);
  H_hsxptar_SmeC->SetStats(0);
  H_hsxptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SmeC->SetLineColor(kRed);
  H_hsxptar_SmeC->Draw("hist");
  //H_hsxptar_SmeC->Draw("");
  H_hsxptar_DmeC->Add(H_hsxptar_DmeCR, -1);
  H_hsxptar_DmeC->Scale(ECMEC);
  H_hsxptar_DmedC->Add(H_hsxptar_DmedCR, -1);
  H_hsxptar_DmedC->Scale(ECMEDC);
  H_hsxptar_DmeC->Add(H_hsxptar_DmedC, -1);
  //  H_hsxptar_DmeC->SetStats(0);
  H_hsxptar_DmeC->Draw("same");

  cthesisAcc->cd(6);
  H_hsyptar_SmeC->SetStats(0);
  H_hsyptar_SmeC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SmeC->SetLineColor(kRed);
  H_hsyptar_SmeC->Draw("hist");
  //H_hsyptar_SmeC->Draw("");
  H_hsyptar_DmeC->Add(H_hsyptar_DmeCR, -1);
  H_hsyptar_DmeC->Scale(ECMEC);
  H_hsyptar_DmedC->Add(H_hsyptar_DmedCR, -1);
  H_hsyptar_DmedC->Scale(ECMEDC);
  H_hsyptar_DmeC->Add(H_hsyptar_DmedC, -1);
  // H_hsyptar_DmeC->SetStats(0);
  H_hsyptar_DmeC->Draw("same");
  cthesisAcc->Print(outputpdf);

  TCanvas *cthesiMPisme1 = new TCanvas("cthesiMPisme1", "cthesiMPisme1");
  cthesiMPisme1->Divide(2,2);
  
  cthesiMPisme1->cd(3);
  MPimeL1->Add(MPimeL1R, -1);
  MPimeL1->Scale(ECMEL1);
  MPimedL1->Add(MPimedL1R, -1);
  MPimedL1->Scale(ECMEDL1);
  MPimeL1->Add(MPimedL1, -1);
  //  MPiSmeL1->SetStats(0);
  MPiSmeL1->SetLineColor(kRed);
  MPiSmeL1->Draw("hist");
  //  MPimeL1->SetStats(0);
  MPimeL1->Draw("same");

  cthesiMPisme1->cd(4);
  MPimeL2->Add(MPimeL2R, -1);
  MPimeL2->Scale(ECMEL2);
  MPimedL2->Add(MPimedL2R, -1);
  MPimedL2->Scale(ECMEDL2);
  MPimeL2->Add(MPimedL2, -1);
  //  MPiSmeL2->SetStats(0);
  MPiSmeL2->SetLineColor(kRed);
  MPiSmeL2->Draw("hist");
  //  MPimeL2->SetStats(0);
  MPimeL2->Draw("same");
  cthesiMPisme1->Print(outputpdf);

  /*
  TCanvas *cthesiMPisme2 = new TCanvas("cthesiMPisme2", "cthesiMPisme2");
  MPimeR1->Add(MPimeR1R, -1);
  MPimeR1->Scale(ECMER1);
  MPimedR1->Add(MPimedR1R, -1);
  MPimedR1->Scale(ECMEDR1);
  MPimeR1->Add(MPimedR1, -1);
  // MPiSmeR1->SetStats(0);
  MPiSmeR1->SetLineColor(kRed);
  MPiSmeR1->Draw("hist");
  // MPimeR1->SetStats(0);
  MPimeR1->Draw("same");
  cthesiMPisme2->Print(outputpdf);
  */
  TCanvas *cthesiMPisme3 = new TCanvas("cthesiMPisme3", "cthesiMPisme3");
  MPimeC->Add(MPimeCR, -1);
  MPimeC->Scale(ECMEC);
  MPimedC->Add(MPimedCR, -1);
  MPimedC->Scale(ECMEDC);
  MPimeC->Add(MPimedC, -1);
  /*
  TAxis *XmpimeC = MPimeC->GetXaxis();
  Double_t InmpimeC = MPimeC->Integral(XmpimeC->FindBin(0.91), XmpimeC->FindBin(0.98), "");
  TAxis *XmpiSmeC = MPiSmeC->GetXaxis();
  Double_t InmpiSmeC = MPiSmeC->Integral(XmpiSmeC->FindBin(0.91), XmpiSmeC->FindBin(0.98), "");
  // MPimeC->Scale(InmpiSmeC/InmpimeC);
  cout<< "InmpimeC/InmpiSmeC = "<<InmpimeC/InmpiSmeC<<endl;
  */
  TAxis *XPimeC = MPimeC->GetXaxis();
  Double_t InPimeC = MPimeC->Integral(XPimeC->FindBin(0.91), XPimeC->FindBin(0.98), "");
  TAxis *XPiSmeC = MPiSmeC->GetXaxis();
  Double_t InPiSmeC = MPiSmeC->Integral(XPiSmeC->FindBin(0.91), XPiSmeC->FindBin(0.98), "");
  Double_t PimeC = (1-InPimeC/InPiSmeC); 
  cout<< " PimeC = "<<PimeC<<endl;
  
  TAxis *XPimeL1 = MPimeL1->GetXaxis();
  Double_t InPimeL1 = MPimeL1->Integral(XPimeL1->FindBin(0.91), XPimeL1->FindBin(0.98), "");
  TAxis *XPiSmeL1 = MPiSmeL1->GetXaxis();
  Double_t InPiSmeL1 = MPiSmeL1->Integral(XPiSmeL1->FindBin(0.91), XPiSmeL1->FindBin(0.98), "");
  Double_t PimeL1 = (1-InPimeL1/InPiSmeL1); 
  cout<< " PimeL1 = "<<PimeL1<<endl;
  
  TAxis *XPimeL2 = MPimeL2->GetXaxis();
  Double_t InPimeL2 = MPimeL2->Integral(XPimeL2->FindBin(0.91), XPimeL2->FindBin(0.98), "");
  TAxis *XPiSmeL2 = MPiSmeL2->GetXaxis();
  Double_t InPiSmeL2 = MPiSmeL2->Integral(XPiSmeL2->FindBin(0.91), XPiSmeL2->FindBin(0.98), "");
  Double_t PimeL2 = (1-InPimeL2/InPiSmeL2); 
  cout<< " PimeL2 = "<<PimeL2<<endl;

  //cout<< "InmpimeC/InmpiSmeC = "<<InmpiSmeC<<endl;
  MPiSmeC->SetStats(0);
  MPiSmeC->SetLineColor(kRed);
  MPiSmeC->Draw("hist");
  MPimeC->SetStats(0);
  MPimeC->Draw("same");
  TLine *MMpil1me = new TLine(MMPICUTL, 0.0, MMPICUTL, 6.0);
  TLine *MMpil2me = new TLine(MMPICUTH, 0.0, MMPICUTH, 6.0);
  //MM scan
  TLine *MMpil3me = new TLine(MMPICUTH-0.002, 0.0, MMPICUTH-0.002, 3.0);
  TLine *MMpil4me = new TLine(MMPICUTH-0.004, 0.0, MMPICUTH-0.004, 3.0);
  TLine *MMpil5me = new TLine(MMPICUTH-0.006, 0.0, MMPICUTH-0.006, 3.0);
  TLine *MMpil6me = new TLine(MMPICUTH-0.008, 0.0, MMPICUTH-0.008, 3.0);
  TLine *MMpil7me = new TLine(MMPICUTH+0.002, 0.0, MMPICUTH+0.002, 3.0);
  TLine *MMpil8me = new TLine(MMPICUTH+0.004, 0.0, MMPICUTH+0.004, 3.0);
  TLine *MMpil9me = new TLine(MMPICUTH+0.006, 0.0, MMPICUTH+0.006, 3.0);
  TLine *MMpil10me = new TLine(MMPICUTH+0.008, 0.0, MMPICUTH+0.008, 3.0);
  MMpil1me->SetLineColor(kBlue);
  MMpil2me->SetLineColor(kBlue);
  MMpil1me->Draw("same");
  MMpil2me->Draw("same");
  MMpil3me->SetLineColor(kRed);
  MMpil4me->SetLineColor(kRed);
  MMpil5me->SetLineColor(kRed);
  MMpil6me->SetLineColor(kRed);
  MMpil7me->SetLineColor(kRed);
  MMpil8me->SetLineColor(kRed);
  MMpil9me->SetLineColor(kRed);
  MMpil10me->SetLineColor(kRed);
  MMpil3me->Draw("same");
  MMpil4me->Draw("same");
  MMpil5me->Draw("same");
  MMpil6me->Draw("same");
  MMpil7me->Draw("same");
  MMpil8me->Draw("same");
  MMpil9me->Draw("same");
  MMpil10me->Draw("same");
  cthesiMPisme3->Print(outputpdf);
  /*
  TCanvas *cthesiMPisme4 = new TCanvas("cthesiMPisme4", "cthesiMPisme4");
  MPimeL1->Add(MPimeL1R, -1);
  MPimeL1->Scale(ECMEL1);
  MPimedL1->Add(MPimedL1R, -1);
  MPimedL1->Scale(ECMEDL1);
  MPimeL1->Add(MPimedL1, -1);
  //  MPiSmeL1->SetStats(0);
  MPiSmeL1->SetLineColor(kRed);
  MPiSmeL1->Draw("hist");
  //  MPimeL1->SetStats(0);
  MPimeL1->Draw("same");
  cthesiMPisme4->Print(outputpdf);
    TCanvas *cthesiMPisme5 = new TCanvas("cthesiMPisme5", "cthesiMPisme5");
  MPimeL2->Add(MPimeL2R, -1);
  MPimeL2->Scale(ECMEL2);
  MPimedL2->Add(MPimedL2R, -1);
  MPimedL2->Scale(ECMEDL2);
  MPimeL2->Add(MPimedL2, -1);
  //  MPiSmeL2->SetStats(0);
  MPiSmeL2->SetLineColor(kRed);
  MPiSmeL2->Draw("hist");
  //  MPimeL2->SetStats(0);
  MPimeL2->Draw("same");
  cthesiMPisme5->Print(outputpdf);
  */

  TCanvas *cthesisme1 = new TCanvas("cthesisme1", "cthesisme1");
  cthesisme1->Divide(2,2);
  
  cthesisme1->cd(3);
  hthtmeL1->Add(hthtmeL1R, -1);
  hthtmeL1->Scale(ECMEL1);
  hthtmedL1->Add(hthtmedL1R, -1);
  hthtmedL1->Scale(ECMEDL1);
  hthtmeL1->Add(hthtmedL1, -1);
  // hthtSmeL1->SetStats(0);
  hthtSmeL1->SetLineColor(kRed);
  hthtSmeL1->Draw("hist");
  // hthtmeL1->SetStats(0);
  hthtmeL1->Draw("same");

  cthesisme1->cd(4);
  hthtmeL2->Add(hthtmeL2R, -1);
  hthtmeL2->Scale(ECMEL2);
  hthtmedL2->Add(hthtmedL2R, -1);
  hthtmedL2->Scale(ECMEDL2);
  hthtmeL2->Add(hthtmedL2, -1);
  // hthtSmeL2->SetStats(0);
  hthtSmeL2->SetLineColor(kRed);
  hthtSmeL2->Draw("hist");
  //  hthtmeL2->SetStats(0);
  hthtmeL2->Draw("same");
  cthesisme1->Print(outputpdf);

  //High epsilon analysis Feb 07, 2023

  cout<<" "<<endl;
  cout<<" Analysing high epsilon data..." <<endl;
  cout<<" "<<endl;
 
  //Data
  TString TInDATAFilenameHigheR1 = "Analysed_Data_Q0p425W2p2_highe_Right1.root";
  TString TInDATAFilenameHigheR2 = "Analysed_Data_Q0p425W2p2_highe_Right2.root";
  TString TInDATAFilenameHigheC  = "Analysed_Data_Q0p425W2p2_highe_Center.root";
  TString TInDATAFilenameHigheL1 = "Analysed_Data_Q0p425W2p2_highe_Left1.root";
  TString TInDATAFilenameHigheL2 = "Analysed_Data_Q0p425W2p2_highe_Left2.root";
 
  TString rootFile_DATAHIGHER1  = ROOTfilePath+"/"+TInDATAFilenameHigheR1;
  TString rootFile_DATAHIGHER2  = ROOTfilePath+"/"+TInDATAFilenameHigheR2;
  TString rootFile_DATAHIGHEC  = ROOTfilePath+"/"+TInDATAFilenameHigheC;
  TString rootFile_DATAHIGHEL1 = ROOTfilePath+"/"+TInDATAFilenameHigheL1;
  TString rootFile_DATAHIGHEL2 = ROOTfilePath+"/"+TInDATAFilenameHigheL2;
  //Dummy
  TString TInDATAFilenameHighedR1 = "Analysed_Data_Q0p425W2p2_highe_dummy_Right1.root";
  TString TInDATAFilenameHighedR2 = "Analysed_Data_Q0p425W2p2_highe_dummy_Right2.root";
  TString TInDATAFilenameHighedC = "Analysed_Data_Q0p425W2p2_highe_dummy_Center.root";
  TString TInDATAFilenameHighedL1 = "Analysed_Data_Q0p425W2p2_highe_dummy_Left1.root";
  TString TInDATAFilenameHighedL2 = "Analysed_Data_Q0p425W2p2_highe_dummy_Left2.root";

  TString rootFile_DATAHIGHEDR1  = ROOTfilePath+"/"+TInDATAFilenameHighedR1;
  TString rootFile_DATAHIGHEDR2  = ROOTfilePath+"/"+TInDATAFilenameHighedR2;
  TString rootFile_DATAHIGHEDC   = ROOTfilePath+"/"+TInDATAFilenameHighedC;
  TString rootFile_DATAHIGHEDL1  = ROOTfilePath+"/"+TInDATAFilenameHighedL1;
  TString rootFile_DATAHIGHEDL2  = ROOTfilePath+"/"+TInDATAFilenameHighedL2;

  //SIMC                                                  
  TString TInSIMCFilenameHigheR1 = "Pion_4p5_Q2_right1.root";
  TString TInSIMCFilenameHigheR2 = "Pion_4p5_Q2_right2.root";
  TString TInSIMCFilenameHigheC  = "Pion_4p5_Q2_center.root";
  TString TInSIMCFilenameHigheL1 = "Pion_4p5_Q2_left1.root";
  TString TInSIMCFilenameHigheL2 = "Pion_4p5_Q2_left2.root";

  TString rootFile_SIMCHIGHER1  = ROOTfilePath+"/"+TInSIMCFilenameHigheR1;
  TString rootFile_SIMCHIGHER2  = ROOTfilePath+"/"+TInSIMCFilenameHigheR2;
  TString rootFile_SIMCHIGHEC   = ROOTfilePath+"/"+TInSIMCFilenameHigheC;
  TString rootFile_SIMCHIGHEL1  = ROOTfilePath+"/"+TInSIMCFilenameHigheL1;
  TString rootFile_SIMCHIGHEL2  = ROOTfilePath+"/"+TInSIMCFilenameHigheL2;

  if (gSystem->AccessPathName(rootFile_DATAHIGHER1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHER1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAHIGHER2) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHER2 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAHIGHEC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHEC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAHIGHEL1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHEL1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_DATAHIGHEL2) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHEL2 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  //Data  
  TFile *InFile_DATAHIGHER1 = new TFile(rootFile_DATAHIGHER1, "READ");
  TFile *InFile_DATAHIGHER2 = new TFile(rootFile_DATAHIGHER2, "READ");
  TFile *InFile_DATAHIGHEC = new TFile(rootFile_DATAHIGHEC, "READ");
  TFile *InFile_DATAHIGHEL1 = new TFile(rootFile_DATAHIGHEL1, "READ");
  TFile *InFile_DATAHIGHEL2 = new TFile(rootFile_DATAHIGHEL2, "READ");
  //Dummy  
  TFile *InFile_DATAHIGHEDR1 = new TFile(rootFile_DATAHIGHEDR1, "READ");
  TFile *InFile_DATAHIGHEDR2 = new TFile(rootFile_DATAHIGHEDR2, "READ");
  TFile *InFile_DATAHIGHEDC  = new TFile(rootFile_DATAHIGHEDC, "READ");
  TFile *InFile_DATAHIGHEDL1 = new TFile(rootFile_DATAHIGHEDL1, "READ");
  TFile *InFile_DATAHIGHEDL2 = new TFile(rootFile_DATAHIGHEDL2, "READ");
  //SIMC 
  TFile *InFile_SIMCHIGHER1 = new TFile(rootFile_SIMCHIGHER1, "READ");
  TFile *InFile_SIMCHIGHER2 = new TFile(rootFile_SIMCHIGHER2, "READ");
  TFile *InFile_SIMCHIGHEC  = new TFile(rootFile_SIMCHIGHEC,  "READ");
  TFile *InFile_SIMCHIGHEL1 = new TFile(rootFile_SIMCHIGHEL1, "READ");
  TFile *InFile_SIMCHIGHEL2 = new TFile(rootFile_SIMCHIGHEL2, "READ");

  // TREES
  //DATA
  TTree* TBRANCHHIGHER1  = (TTree*)InFile_DATAHIGHER1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHER1  = (Long64_t)TBRANCHHIGHER1->GetEntries();  
  TTree* TBRANCHHIGHER2  = (TTree*)InFile_DATAHIGHER2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHER2  = (Long64_t)TBRANCHHIGHER2->GetEntries();  
  TTree* TBRANCHHIGHEC  = (TTree*)InFile_DATAHIGHEC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHHIGHEC  = (Long64_t)TBRANCHHIGHEC->GetEntries();  
  TTree* TBRANCHHIGHEL1  = (TTree*)InFile_DATAHIGHEL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEL1  = (Long64_t)TBRANCHHIGHEL1->GetEntries();  
  TTree* TBRANCHHIGHEL2  = (TTree*)InFile_DATAHIGHEL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEL2  = (Long64_t)TBRANCHHIGHEL2->GetEntries();  
  //Dummy
  TTree* TBRANCHHIGHEDR1  = (TTree*)InFile_DATAHIGHEDR1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEDR1  = (Long64_t)TBRANCHHIGHEDR1->GetEntries();  
  TTree* TBRANCHHIGHEDR2  = (TTree*)InFile_DATAHIGHEDR2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEDR2  = (Long64_t)TBRANCHHIGHEDR2->GetEntries();  
  TTree* TBRANCHHIGHEDC   = (TTree*)InFile_DATAHIGHEDC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHHIGHEDC   = (Long64_t)TBRANCHHIGHEDC->GetEntries();  
  TTree* TBRANCHHIGHEDL1  = (TTree*)InFile_DATAHIGHEDL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEDL1  = (Long64_t)TBRANCHHIGHEDL1->GetEntries();  
  TTree* TBRANCHHIGHEDL2  = (TTree*)InFile_DATAHIGHEDL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEDL2  = (Long64_t)TBRANCHHIGHEDL2->GetEntries();  

  //SIMC 
  TTree* TSIMCHIGHER1  = (TTree*)InFile_SIMCHIGHER1->Get("h10");Long64_t nEntries_TSIMCHIGHER1  = (Long64_t)TSIMCHIGHER1->GetEntries();
  TTree* TSIMCHIGHER2  = (TTree*)InFile_SIMCHIGHER2->Get("h10");Long64_t nEntries_TSIMCHIGHER2  = (Long64_t)TSIMCHIGHER2->GetEntries();
  TTree* TSIMCHIGHEC   = (TTree*)InFile_SIMCHIGHEC->Get("h10");Long64_t nEntries_TSIMCHIGHEC    = (Long64_t)TSIMCHIGHEC->GetEntries();
  TTree* TSIMCHIGHEL1  = (TTree*)InFile_SIMCHIGHEL1->Get("h10");Long64_t nEntries_TSIMCHIGHEL1  = (Long64_t)TSIMCHIGHEL1->GetEntries();
  TTree* TSIMCHIGHEL2  = (TTree*)InFile_SIMCHIGHEL2->Get("h10");Long64_t nEntries_TSIMCHIGHEL2  = (Long64_t)TSIMCHIGHEL2->GetEntries();

  //VARIABLES
 //R1
  Double_t  P_gtr_dpheR1;TBRANCHHIGHER1->SetBranchAddress("ssdelta", &P_gtr_dpheR1);
  Double_t  P_gtr_xptarheR1;TBRANCHHIGHER1->SetBranchAddress("ssxptar", &P_gtr_xptarheR1);
  Double_t  P_gtr_yptarheR1;TBRANCHHIGHER1->SetBranchAddress("ssyptar", &P_gtr_yptarheR1);

  Double_t  H_gtr_dpheR1;TBRANCHHIGHER1->SetBranchAddress("hsdelta", &H_gtr_dpheR1);
  Double_t  H_gtr_xptarheR1;TBRANCHHIGHER1->SetBranchAddress("hsxptar", &H_gtr_xptarheR1);
  Double_t  H_gtr_yptarheR1;TBRANCHHIGHER1->SetBranchAddress("hsyptar", &H_gtr_yptarheR1);

  
  Double_t WHER1;TBRANCHHIGHER1->SetBranchAddress("W", &WHER1);
  Double_t Q2HER1;TBRANCHHIGHER1->SetBranchAddress("Q2", &Q2HER1);

  //R2
  Double_t  P_gtr_dpheR2;TBRANCHHIGHER2->SetBranchAddress("ssdelta", &P_gtr_dpheR2);
  Double_t  P_gtr_xptarheR2;TBRANCHHIGHER2->SetBranchAddress("ssxptar", &P_gtr_xptarheR2);
  Double_t  P_gtr_yptarheR2;TBRANCHHIGHER2->SetBranchAddress("ssyptar", &P_gtr_yptarheR2);

  Double_t  H_gtr_dpheR2;TBRANCHHIGHER2->SetBranchAddress("hsdelta", &H_gtr_dpheR2);
  Double_t  H_gtr_xptarheR2;TBRANCHHIGHER2->SetBranchAddress("hsxptar", &H_gtr_xptarheR2);
  Double_t  H_gtr_yptarheR2;TBRANCHHIGHER2->SetBranchAddress("hsyptar", &H_gtr_yptarheR2);

  Double_t WHER2;TBRANCHHIGHER2->SetBranchAddress("W", &WHER2);
  Double_t Q2HER2;TBRANCHHIGHER2->SetBranchAddress("Q2", &Q2HER2);
  Double_t epsilonHER2;TBRANCHHIGHER2->SetBranchAddress("epsilon", &epsilonHER2);

  //Dummy
  //R1
  Double_t WHEDR1;TBRANCHHIGHEDR1->SetBranchAddress("W", &WHEDR1);
  Double_t Q2HEDR1;TBRANCHHIGHEDR1->SetBranchAddress("Q2", &Q2HEDR1);
  //R2
  Double_t WHEDR2;TBRANCHHIGHEDR2->SetBranchAddress("W", &WHEDR2);
  Double_t Q2HEDR2;TBRANCHHIGHEDR2->SetBranchAddress("Q2", &Q2HEDR2);
  Double_t epsilonHEDR2;TBRANCHHIGHEDR2->SetBranchAddress("epsilon", &epsilonHEDR2);

  Double_t  P_gtr_dphedR2;TBRANCHHIGHEDR2->SetBranchAddress("ssdelta", &P_gtr_dphedR2);
  Double_t  P_gtr_xptarhedR2;TBRANCHHIGHEDR2->SetBranchAddress("ssxptar", &P_gtr_xptarhedR2);
  Double_t  P_gtr_yptarhedR2;TBRANCHHIGHEDR2->SetBranchAddress("ssyptar", &P_gtr_yptarhedR2);

  Double_t  H_gtr_dphedR2;TBRANCHHIGHEDR2->SetBranchAddress("hsdelta", &H_gtr_dphedR2);
  Double_t  H_gtr_xptarhedR2;TBRANCHHIGHEDR2->SetBranchAddress("hsxptar", &H_gtr_xptarhedR2);
  Double_t  H_gtr_yptarhedR2;TBRANCHHIGHEDR2->SetBranchAddress("hsyptar", &H_gtr_yptarhedR2);
  
  //Center  
  Double_t  P_gtr_dpheC;TBRANCHHIGHEC->SetBranchAddress("ssdelta", &P_gtr_dpheC);
  Double_t  P_gtr_xptarheC;TBRANCHHIGHEC->SetBranchAddress("ssxptar", &P_gtr_xptarheC);
  Double_t  P_gtr_yptarheC;TBRANCHHIGHEC->SetBranchAddress("ssyptar", &P_gtr_yptarheC);

  Double_t  H_gtr_dpheC;TBRANCHHIGHEC->SetBranchAddress("hsdelta", &H_gtr_dpheC);
  Double_t  H_gtr_xptarheC;TBRANCHHIGHEC->SetBranchAddress("hsxptar", &H_gtr_xptarheC);
  Double_t  H_gtr_yptarheC;TBRANCHHIGHEC->SetBranchAddress("hsyptar", &H_gtr_yptarheC);

  Double_t WHEC;TBRANCHHIGHEC->SetBranchAddress("W", &WHEC);
  Double_t Q2HEC;TBRANCHHIGHEC->SetBranchAddress("Q2", &Q2HEC);
  //Dummy
  Double_t WHEDC;TBRANCHHIGHEDC->SetBranchAddress("W", &WHEDC);
  Double_t Q2HEDC;TBRANCHHIGHEDC->SetBranchAddress("Q2", &Q2HEDC);
 
  Double_t  P_gtr_dpheL1;TBRANCHHIGHEL1->SetBranchAddress("ssdelta", &P_gtr_dpheL1);
  Double_t  P_gtr_xptarheL1;TBRANCHHIGHEL1->SetBranchAddress("ssxptar", &P_gtr_xptarheL1);
  Double_t  P_gtr_yptarheL1;TBRANCHHIGHEL1->SetBranchAddress("ssyptar", &P_gtr_yptarheL1);

  Double_t  H_gtr_dpheL1;TBRANCHHIGHEL1->SetBranchAddress("hsdelta", &H_gtr_dpheL1);
  Double_t  H_gtr_xptarheL1;TBRANCHHIGHEL1->SetBranchAddress("hsxptar", &H_gtr_xptarheL1);
  Double_t  H_gtr_yptarheL1;TBRANCHHIGHEL1->SetBranchAddress("hsyptar", &H_gtr_yptarheL1);

  Double_t WHEL1;TBRANCHHIGHEL1->SetBranchAddress("W", &WHEL1);
  Double_t Q2HEL1;TBRANCHHIGHEL1->SetBranchAddress("Q2", &Q2HEL1);
  //Dummy
  Double_t WHEDL1;TBRANCHHIGHEDL1->SetBranchAddress("W", &WHEDL1);
  Double_t Q2HEDL1;TBRANCHHIGHEDL1->SetBranchAddress("Q2", &Q2HEDL1);
 
  Double_t  P_gtr_dpheL2;TBRANCHHIGHEL2->SetBranchAddress("ssdelta", &P_gtr_dpheL2);
  Double_t  P_gtr_xptarheL2;TBRANCHHIGHEL2->SetBranchAddress("ssxptar", &P_gtr_xptarheL2);
  Double_t  P_gtr_yptarheL2;TBRANCHHIGHEL2->SetBranchAddress("ssyptar", &P_gtr_yptarheL2);

  Double_t  H_gtr_dpheL2;TBRANCHHIGHEL2->SetBranchAddress("hsdelta", &H_gtr_dpheL2);
  Double_t  H_gtr_xptarheL2;TBRANCHHIGHEL2->SetBranchAddress("hsxptar", &H_gtr_xptarheL2);
  Double_t  H_gtr_yptarheL2;TBRANCHHIGHEL2->SetBranchAddress("hsyptar", &H_gtr_yptarheL2);

  Double_t WHEL2;TBRANCHHIGHEL2->SetBranchAddress("W", &WHEL2);
  Double_t Q2HEL2;TBRANCHHIGHEL2->SetBranchAddress("Q2", &Q2HEL2);
  //Dummy
  Double_t WHEDL2;TBRANCHHIGHEDL2->SetBranchAddress("W", &WHEDL2);
  Double_t Q2HEDL2;TBRANCHHIGHEDL2->SetBranchAddress("Q2", &Q2HEDL2);
  
  Double_t theR1;TBRANCHHIGHER1->SetBranchAddress("MandelT", &theR1);
  Double_t theR2;TBRANCHHIGHER2->SetBranchAddress("MandelT", &theR2);
  Double_t theC;TBRANCHHIGHEC->SetBranchAddress("MandelT", &theC);
  Double_t theL1;TBRANCHHIGHEL1->SetBranchAddress("MandelT", &theL1);
  Double_t theL2;TBRANCHHIGHEL2->SetBranchAddress("MandelT", &theL2);
  //Dummy
  Double_t thedR1;TBRANCHHIGHEDR1->SetBranchAddress("MandelT", &thedR1);
  Double_t thedR2;TBRANCHHIGHEDR2->SetBranchAddress("MandelT", &thedR2);
  Double_t thedC;TBRANCHHIGHEDC->SetBranchAddress("MandelT", &thedC);
  Double_t thedL1;TBRANCHHIGHEDL1->SetBranchAddress("MandelT", &thedL1);
  Double_t thedL2;TBRANCHHIGHEDL2->SetBranchAddress("MandelT", &thedL2);

  Double_t ph_qheR1;TBRANCHHIGHER1->SetBranchAddress("ph_q", &ph_qheR1);
  Double_t ph_qheR2;TBRANCHHIGHER2->SetBranchAddress("ph_q", &ph_qheR2);
  Double_t ph_qheC;TBRANCHHIGHEC->SetBranchAddress("ph_q", &ph_qheC);
  Double_t ph_qheL1;TBRANCHHIGHEL1->SetBranchAddress("ph_q", &ph_qheL1);
  Double_t ph_qheL2;TBRANCHHIGHEL2->SetBranchAddress("ph_q", &ph_qheL2);
  //Dummy
  Double_t ph_qhedR1;TBRANCHHIGHEDR1->SetBranchAddress("ph_q", &ph_qhedR1);
  Double_t ph_qhedR2;TBRANCHHIGHEDR2->SetBranchAddress("ph_q", &ph_qhedR2);
  Double_t ph_qhedC;TBRANCHHIGHEDC->SetBranchAddress("ph_q", &ph_qhedC);
  Double_t ph_qhedL1;TBRANCHHIGHEDL1->SetBranchAddress("ph_q", &ph_qhedL1);
  Double_t ph_qhedL2;TBRANCHHIGHEDL2->SetBranchAddress("ph_q", &ph_qhedL2);

  Double_t mmheR1;TBRANCHHIGHER1->SetBranchAddress("MM", &mmheR1);
  Double_t mmheR2;TBRANCHHIGHER2->SetBranchAddress("MM", &mmheR2);
  Double_t mmheC;TBRANCHHIGHEC->SetBranchAddress("MM", &mmheC);
  Double_t mmheL1;TBRANCHHIGHEL1->SetBranchAddress("MM", &mmheL1);
  Double_t mmheL2;TBRANCHHIGHEL2->SetBranchAddress("MM", &mmheL2);
  //Dummy
  Double_t mmhedR1;TBRANCHHIGHEDR1->SetBranchAddress("MM", &mmhedR1);
  Double_t mmhedR2;TBRANCHHIGHEDR2->SetBranchAddress("MM", &mmhedR2);
  Double_t mmhedC;TBRANCHHIGHEDC->SetBranchAddress("MM", &mmhedC);
  Double_t mmhedL1;TBRANCHHIGHEDL1->SetBranchAddress("MM", &mmhedL1);
  Double_t mmhedL2;TBRANCHHIGHEDL2->SetBranchAddress("MM", &mmhedL2);

  Double_t tcoinheR1;TBRANCHHIGHER1->SetBranchAddress("CTime_ROC1", &tcoinheR1);
  Double_t tcoinheR2;TBRANCHHIGHER2->SetBranchAddress("CTime_ROC1", &tcoinheR2);
  Double_t tcoinheC;TBRANCHHIGHEC->SetBranchAddress("CTime_ROC1", &tcoinheC);
  Double_t tcoinheL1;TBRANCHHIGHEL1->SetBranchAddress("CTime_ROC1", &tcoinheL1);
  Double_t tcoinheL2;TBRANCHHIGHEL2->SetBranchAddress("CTime_ROC1", &tcoinheL2);
  //Dummy
  Double_t tcoinhedR1;TBRANCHHIGHEDR1->SetBranchAddress("CTime_ROC1", &tcoinhedR1);
  Double_t tcoinhedR2;TBRANCHHIGHEDR2->SetBranchAddress("CTime_ROC1", &tcoinhedR2);
  Double_t tcoinhedC;TBRANCHHIGHEDC->SetBranchAddress("CTime_ROC1", &tcoinhedC);
  Double_t tcoinhedL1;TBRANCHHIGHEDL1->SetBranchAddress("CTime_ROC1", &tcoinhedL1);
  Double_t tcoinhedL2;TBRANCHHIGHEDL2->SetBranchAddress("CTime_ROC1", &tcoinhedL2);

  //SIMC VARIABLES
  //Right1
  //SHMS SIMC variables                                               
  Float_t ssdeltaheR1;TSIMCHIGHER1->SetBranchAddress("ssdelta", &ssdeltaheR1);
  Float_t ssxptarheR1;TSIMCHIGHER1->SetBranchAddress("ssxptar", &ssxptarheR1);
  Float_t ssyptarheR1;TSIMCHIGHER1->SetBranchAddress("ssyptar", &ssyptarheR1);
  Float_t ssxfpheR1;TSIMCHIGHER1->SetBranchAddress("ssxfp", &ssxfpheR1);
  Float_t ssyfpheR1;TSIMCHIGHER1->SetBranchAddress("ssyfp", &ssyfpheR1);
  Float_t ssxpfpheR1;TSIMCHIGHER1->SetBranchAddress("ssxpfp", &ssxpfpheR1);
  Float_t ssypfpheR1;TSIMCHIGHER1->SetBranchAddress("ssypfp", &ssypfpheR1);

  //HMS SIMC variables 
  Float_t hsdeltaheR1;TSIMCHIGHER1->SetBranchAddress("hsdelta", &hsdeltaheR1);
  Float_t hsxptarheR1;TSIMCHIGHER1->SetBranchAddress("hsxptar", &hsxptarheR1);
  Float_t hsyptarheR1;TSIMCHIGHER1->SetBranchAddress("hsyptar", &hsyptarheR1);
  Float_t hsxfpheR1;TSIMCHIGHER1->SetBranchAddress("hsxfp", &hsxfpheR1);
  Float_t hsyfpheR1;TSIMCHIGHER1->SetBranchAddress("hsyfp", &hsyfpheR1);
  Float_t hsxpfpheR1;TSIMCHIGHER1->SetBranchAddress("hsxpfp", &hsxpfpheR1);
  Float_t hsypfpheR1;TSIMCHIGHER1->SetBranchAddress("hsypfp", &hsypfpheR1); 

  Float_t qheR1;TSIMCHIGHER1->SetBranchAddress("q", &qheR1);
  Float_t Q2_simcheR1;TSIMCHIGHER1->SetBranchAddress("Q2", &Q2_simcheR1);
  Float_t W_simcheR1;TSIMCHIGHER1->SetBranchAddress("W", &W_simcheR1);
  Float_t t_simcheR1;TSIMCHIGHER1->SetBranchAddress("t", &t_simcheR1);
  Float_t ti_simcheR1;TSIMCHIGHER1->SetBranchAddress("ti", &ti_simcheR1);
  Float_t epsilon_simcheR1;TSIMCHIGHER1->SetBranchAddress("epsilon", &epsilon_simcheR1);
  Float_t missmassheR1;TSIMCHIGHER1->SetBranchAddress("missmass", &missmassheR1);
  Float_t EmheR1;TSIMCHIGHER1->SetBranchAddress("Em", &EmheR1);
  Float_t PmheR1;TSIMCHIGHER1->SetBranchAddress("Pm", &PmheR1);
  Float_t WeightheR1;TSIMCHIGHER1->SetBranchAddress("Weight", &WeightheR1);
  Float_t phipqheR1;TSIMCHIGHER1->SetBranchAddress("phipq", &phipqheR1);

  //Right2
  //SHMS SIMC variables                                               
  Float_t ssdeltaheR2;TSIMCHIGHER2->SetBranchAddress("ssdelta", &ssdeltaheR2);
  Float_t ssxptarheR2;TSIMCHIGHER2->SetBranchAddress("ssxptar", &ssxptarheR2);
  Float_t ssyptarheR2;TSIMCHIGHER2->SetBranchAddress("ssyptar", &ssyptarheR2);
  Float_t ssxfpheR2;TSIMCHIGHER2->SetBranchAddress("ssxfp", &ssxfpheR2);
  Float_t ssyfpheR2;TSIMCHIGHER2->SetBranchAddress("ssyfp", &ssyfpheR2);
  Float_t ssxpfpheR2;TSIMCHIGHER2->SetBranchAddress("ssxpfp", &ssxpfpheR2);
  Float_t ssypfpheR2;TSIMCHIGHER2->SetBranchAddress("ssypfp", &ssypfpheR2);

  //HMS SIMC variables 
  Float_t hsdeltaheR2;TSIMCHIGHER2->SetBranchAddress("hsdelta", &hsdeltaheR2);
  Float_t hsxptarheR2;TSIMCHIGHER2->SetBranchAddress("hsxptar", &hsxptarheR2);
  Float_t hsyptarheR2;TSIMCHIGHER2->SetBranchAddress("hsyptar", &hsyptarheR2);
  Float_t hsxfpheR2;TSIMCHIGHER2->SetBranchAddress("hsxfp", &hsxfpheR2);
  Float_t hsyfpheR2;TSIMCHIGHER2->SetBranchAddress("hsyfp", &hsyfpheR2);
  Float_t hsxpfpheR2;TSIMCHIGHER2->SetBranchAddress("hsxpfp", &hsxpfpheR2);
  Float_t hsypfpheR2;TSIMCHIGHER2->SetBranchAddress("hsypfp", &hsypfpheR2); 

  Float_t qheR2;TSIMCHIGHER2->SetBranchAddress("q", &qheR2);
  Float_t Q2_simcheR2;TSIMCHIGHER2->SetBranchAddress("Q2", &Q2_simcheR2);
  Float_t W_simcheR2;TSIMCHIGHER2->SetBranchAddress("W", &W_simcheR2);
  Float_t t_simcheR2;TSIMCHIGHER2->SetBranchAddress("t", &t_simcheR2);
  Float_t ti_simcheR2;TSIMCHIGHER2->SetBranchAddress("ti", &ti_simcheR2);
  Float_t epsilon_simcheR2;TSIMCHIGHER2->SetBranchAddress("epsilon", &epsilon_simcheR2);
  Float_t missmassheR2;TSIMCHIGHER2->SetBranchAddress("missmass", &missmassheR2);
  Float_t EmheR2;TSIMCHIGHER2->SetBranchAddress("Em", &EmheR2);
  Float_t PmheR2;TSIMCHIGHER2->SetBranchAddress("Pm", &PmheR2);
  Float_t WeightheR2;TSIMCHIGHER2->SetBranchAddress("Weight", &WeightheR2);
  Float_t phipqheR2;TSIMCHIGHER2->SetBranchAddress("phipq", &phipqheR2);

  //CENTER
  //SHMS SIMC variables
  Float_t ssdeltaheC;TSIMCHIGHEC->SetBranchAddress("ssdelta", &ssdeltaheC);
  Float_t ssxptarheC;TSIMCHIGHEC->SetBranchAddress("ssxptar", &ssxptarheC);
  Float_t ssyptarheC;TSIMCHIGHEC->SetBranchAddress("ssyptar", &ssyptarheC);
  Float_t ssxfpheC;TSIMCHIGHEC->SetBranchAddress("ssxfp", &ssxfpheC);
  Float_t ssyfpheC;TSIMCHIGHEC->SetBranchAddress("ssyfp", &ssyfpheC);
  Float_t ssxpfpheC;TSIMCHIGHEC->SetBranchAddress("ssxpfp", &ssxpfpheC);
  Float_t ssypfpheC;TSIMCHIGHEC->SetBranchAddress("ssypfp", &ssypfpheC);

  //HMS SIMC variables 
  Float_t hsdeltaheC;TSIMCHIGHEC->SetBranchAddress("hsdelta", &hsdeltaheC);
  Float_t hsxptarheC;TSIMCHIGHEC->SetBranchAddress("hsxptar", &hsxptarheC);
  Float_t hsyptarheC;TSIMCHIGHEC->SetBranchAddress("hsyptar", &hsyptarheC);
  Float_t hsxfpheC;TSIMCHIGHEC->SetBranchAddress("hsxfp", &hsxfpheC);
  Float_t hsyfpheC;TSIMCHIGHEC->SetBranchAddress("hsyfp", &hsyfpheC);
  Float_t hsxpfpheC;TSIMCHIGHEC->SetBranchAddress("hsxpfp", &hsxpfpheC);
  Float_t hsypfpheC;TSIMCHIGHEC->SetBranchAddress("hsypfp", &hsypfpheC); 

  Float_t qheC;TSIMCHIGHEC->SetBranchAddress("q", &qheC);
  Float_t Q2_simcheC;TSIMCHIGHEC->SetBranchAddress("Q2", &Q2_simcheC);
  Float_t W_simcheC;TSIMCHIGHEC->SetBranchAddress("W", &W_simcheC);
  Float_t t_simcheC;TSIMCHIGHEC->SetBranchAddress("t", &t_simcheC);
  Float_t ti_simcheC;TSIMCHIGHEC->SetBranchAddress("ti", &ti_simcheC);
  Float_t epsilon_simcheC;TSIMCHIGHEC->SetBranchAddress("epsilon", &epsilon_simcheC);
  Float_t missmassheC;TSIMCHIGHEC->SetBranchAddress("missmass", &missmassheC);
  Float_t EmheC;TSIMCHIGHEC->SetBranchAddress("Em", &EmheC);
  Float_t PmheC;TSIMCHIGHEC->SetBranchAddress("Pm", &PmheC);
  Float_t WeightheC;TSIMCHIGHEC->SetBranchAddress("Weight", &WeightheC);
  Float_t phipqheC;TSIMCHIGHEC->SetBranchAddress("phipq", &phipqheC);

  //LEFT1
  //SHMS SIMC variables
  
  Float_t ssdeltaheL1;TSIMCHIGHEL1->SetBranchAddress("ssdelta", &ssdeltaheL1);
  Float_t ssxptarheL1;TSIMCHIGHEL1->SetBranchAddress("ssxptar", &ssxptarheL1);
  Float_t ssyptarheL1;TSIMCHIGHEL1->SetBranchAddress("ssyptar", &ssyptarheL1);
  Float_t ssxfpheL1;TSIMCHIGHEL1->SetBranchAddress("ssxfp", &ssxfpheL1);
  Float_t ssyfpheL1;TSIMCHIGHEL1->SetBranchAddress("ssyfp", &ssyfpheL1);
  Float_t ssxpfpheL1;TSIMCHIGHEL1->SetBranchAddress("ssxpfp", &ssxpfpheL1);
  Float_t ssypfpheL1;TSIMCHIGHEL1->SetBranchAddress("ssypfp", &ssypfpheL1);

  //HMS SIMC variables 
  Float_t hsdeltaheL1;TSIMCHIGHEL1->SetBranchAddress("hsdelta", &hsdeltaheL1);
  Float_t hsxptarheL1;TSIMCHIGHEL1->SetBranchAddress("hsxptar", &hsxptarheL1);
  Float_t hsyptarheL1;TSIMCHIGHEL1->SetBranchAddress("hsyptar", &hsyptarheL1);
  Float_t hsxfpheL1;TSIMCHIGHEL1->SetBranchAddress("hsxfp", &hsxfpheL1);
  Float_t hsyfpheL1;TSIMCHIGHEL1->SetBranchAddress("hsyfp", &hsyfpheL1);
  Float_t hsxpfpheL1;TSIMCHIGHEL1->SetBranchAddress("hsxpfp", &hsxpfpheL1);
  Float_t hsypfpheL1;TSIMCHIGHEL1->SetBranchAddress("hsypfp", &hsypfpheL1); 

  Float_t qheL1;TSIMCHIGHEL1->SetBranchAddress("q", &qheL1);
  Float_t Q2_simcheL1;TSIMCHIGHEL1->SetBranchAddress("Q2", &Q2_simcheL1);
  Float_t W_simcheL1;TSIMCHIGHEL1->SetBranchAddress("W", &W_simcheL1);
  Float_t t_simcheL1;TSIMCHIGHEL1->SetBranchAddress("t", &t_simcheL1);
  Float_t ti_simcheL1;TSIMCHIGHEL1->SetBranchAddress("ti", &ti_simcheL1);
  Float_t epsilon_simcheL1;TSIMCHIGHEL1->SetBranchAddress("epsilon", &epsilon_simcheL1);
  Float_t missmassheL1;TSIMCHIGHEL1->SetBranchAddress("missmass", &missmassheL1);
  Float_t EmheL1;TSIMCHIGHEL1->SetBranchAddress("Em", &EmheL1);
  Float_t PmheL1;TSIMCHIGHEL1->SetBranchAddress("Pm", &PmheL1);
  Float_t WeightheL1;TSIMCHIGHEL1->SetBranchAddress("Weight", &WeightheL1);
  Float_t phipqheL1;TSIMCHIGHEL1->SetBranchAddress("phipq", &phipqheL1);

  //LEFT2
  //SHMS SIMC variables 
  
  Float_t ssdeltaheL2;TSIMCHIGHEL2->SetBranchAddress("ssdelta", &ssdeltaheL2);
  Float_t ssxptarheL2;TSIMCHIGHEL2->SetBranchAddress("ssxptar", &ssxptarheL2);
  Float_t ssyptarheL2;TSIMCHIGHEL2->SetBranchAddress("ssyptar", &ssyptarheL2);
  Float_t ssxfpheL2;TSIMCHIGHEL2->SetBranchAddress("ssxfp", &ssxfpheL2);
  Float_t ssyfpheL2;TSIMCHIGHEL2->SetBranchAddress("ssyfp", &ssyfpheL2);
  Float_t ssxpfpheL2;TSIMCHIGHEL2->SetBranchAddress("ssxpfp", &ssxpfpheL2);
  Float_t ssypfpheL2;TSIMCHIGHEL2->SetBranchAddress("ssypfp", &ssypfpheL2);

  //HMS SIMC variables 
  Float_t hsdeltaheL2;TSIMCHIGHEL2->SetBranchAddress("hsdelta", &hsdeltaheL2);
  Float_t hsxptarheL2;TSIMCHIGHEL2->SetBranchAddress("hsxptar", &hsxptarheL2);
  Float_t hsyptarheL2;TSIMCHIGHEL2->SetBranchAddress("hsyptar", &hsyptarheL2);
  Float_t hsxfpheL2;TSIMCHIGHEL2->SetBranchAddress("hsxfp", &hsxfpheL2);
  Float_t hsyfpheL2;TSIMCHIGHEL2->SetBranchAddress("hsyfp", &hsyfpheL2);
  Float_t hsxpfpheL2;TSIMCHIGHEL2->SetBranchAddress("hsxpfp", &hsxpfpheL2);
  Float_t hsypfpheL2;TSIMCHIGHEL2->SetBranchAddress("hsypfp", &hsypfpheL2); 

  Float_t qheL2;TSIMCHIGHEL2->SetBranchAddress("q", &qheL2);
  Float_t Q2_simcheL2;TSIMCHIGHEL2->SetBranchAddress("Q2", &Q2_simcheL2);
  Float_t W_simcheL2;TSIMCHIGHEL2->SetBranchAddress("W", &W_simcheL2);
  Float_t t_simcheL2;TSIMCHIGHEL2->SetBranchAddress("t", &t_simcheL2);
  Float_t ti_simcheL2;TSIMCHIGHEL2->SetBranchAddress("ti", &ti_simcheL2);
  Float_t epsilon_simcheL2;TSIMCHIGHEL2->SetBranchAddress("epsilon", &epsilon_simcheL2);
  Float_t missmassheL2;TSIMCHIGHEL2->SetBranchAddress("missmass", &missmassheL2);
  Float_t EmheL2;TSIMCHIGHEL2->SetBranchAddress("Em", &EmheL2);
  Float_t PmheL2;TSIMCHIGHEL2->SetBranchAddress("Pm", &PmheL2);
  Float_t WeightheL2;TSIMCHIGHEL2->SetBranchAddress("Weight", &WeightheL2);
  Float_t phipqheL2;TSIMCHIGHEL2->SetBranchAddress("phipq", &phipqheL2);

  // HISTOGRAMS

  TH2D *hQ2WHEC  = new TH2D("hQ2WHERC","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WHEC1  = new TH2D("hQ2WHERC1","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      

  TH2D *hQ2WHECR  = new TH2D("hQ2WHECR","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WHEC1R  = new TH2D("hQ2WHEC1R","; ; ", 300, 0.2, 0.6, 300, 2.05, 2.4);      

  //W

  TH1D *hW1heC   = new TH1D("hW1heC","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2heC   = new TH1D("hW2heC","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3heC   = new TH1D("hW3heC","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4heC   = new TH1D("hW4heC","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5heC   = new TH1D("hW5heC","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6heC   = new TH1D("hW6heC","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7heC   = new TH1D("hW7heC","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8heC   = new TH1D("hW8heC","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1heR1   = new TH1D("hW1heR1","W 1 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW2heR1   = new TH1D("hW2heR1","W 2 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW3heR1   = new TH1D("hW3heR1","W 3 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW4heR1   = new TH1D("hW4heR1","W 4 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW5heR1   = new TH1D("hW5heR1","W 5 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW6heR1   = new TH1D("hW6heR1","W 6 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW7heR1   = new TH1D("hW7heR1","W 7 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW8heR1   = new TH1D("hW8heR1","W 8 t R1; W;", 300, 2.1, 2.3);

  TH1D *hW1heR2   = new TH1D("hW1heR2","W 1 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW2heR2   = new TH1D("hW2heR2","W 2 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW3heR2   = new TH1D("hW3heR2","W 3 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW4heR2   = new TH1D("hW4heR2","W 4 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW5heR2   = new TH1D("hW5heR2","W 5 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW6heR2   = new TH1D("hW6heR2","W 6 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW7heR2   = new TH1D("hW7heR2","W 7 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW8heR2   = new TH1D("hW8heR2","W 8 t R2; W;", 300, 2.1, 2.3);

  TH1D *hW1heL1   = new TH1D("hW1heL1","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2heL1   = new TH1D("hW2heL1","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3heL1   = new TH1D("hW3heL1","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4heL1   = new TH1D("hW4heL1","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5heL1   = new TH1D("hW5heL1","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6heL1   = new TH1D("hW6heL1","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7heL1   = new TH1D("hW7heL1","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8heL1   = new TH1D("hW8heL1","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1heL2   = new TH1D("hW1heL2","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2heL2   = new TH1D("hW2heL2","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3heL2   = new TH1D("hW3heL2","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4heL2   = new TH1D("hW4heL2","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5heL2   = new TH1D("hW5heL2","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6heL2   = new TH1D("hW6heL2","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7heL2   = new TH1D("hW7heL2","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8heL2   = new TH1D("hW8heL2","W 8 t L2; W;", 300, 2.1, 2.3);

  //W Random

  TH1D *hW1heCR   = new TH1D("hW1heCR","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2heCR   = new TH1D("hW2heCR","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3heCR   = new TH1D("hW3heCR","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4heCR   = new TH1D("hW4heCR","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5heCR   = new TH1D("hW5heCR","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6heCR   = new TH1D("hW6heCR","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7heCR   = new TH1D("hW7heCR","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8heCR   = new TH1D("hW8heCR","W 8 t C; W;", 300, 2.1, 2.3);

  TH1D *hW1heR1R   = new TH1D("hW1heR1R","W 1 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW2heR1R   = new TH1D("hW2heR1R","W 2 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW3heR1R   = new TH1D("hW3heR1R","W 3 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW4heR1R   = new TH1D("hW4heR1R","W 4 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW5heR1R   = new TH1D("hW5heR1R","W 5 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW6heR1R   = new TH1D("hW6heR1R","W 6 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW7heR1R   = new TH1D("hW7heR1R","W 7 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW8heR1R   = new TH1D("hW8heR1R","W 8 t R1; W;", 300, 2.1, 2.3);

  TH1D *hW1heR2R   = new TH1D("hW1heR2R","W 1 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW2heR2R   = new TH1D("hW2heR2R","W 2 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW3heR2R   = new TH1D("hW3heR2R","W 3 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW4heR2R   = new TH1D("hW4heR2R","W 4 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW5heR2R   = new TH1D("hW5heR2R","W 5 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW6heR2R   = new TH1D("hW6heR2R","W 6 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW7heR2R   = new TH1D("hW7heR2R","W 7 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW8heR2R   = new TH1D("hW8heR2R","W 8 t R2; W;", 300, 2.1, 2.3);

  TH1D *hW1heL1R   = new TH1D("hW1heL1R","W 1 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW2heL1R   = new TH1D("hW2heL1R","W 2 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW3heL1R   = new TH1D("hW3heL1R","W 3 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW4heL1R   = new TH1D("hW4heL1R","W 4 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW5heL1R   = new TH1D("hW5heL1R","W 5 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW6heL1R   = new TH1D("hW6heL1R","W 6 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW7heL1R   = new TH1D("hW7heL1R","W 7 t L1; W;", 300, 2.1, 2.3);
  TH1D *hW8heL1R   = new TH1D("hW8heL1R","W 8 t L1; W;", 300, 2.1, 2.3);

  TH1D *hW1heL2R   = new TH1D("hW1heL2R","W 1 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW2heL2R   = new TH1D("hW2heL2R","W 2 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW3heL2R   = new TH1D("hW3heL2R","W 3 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW4heL2R   = new TH1D("hW4heL2R","W 4 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW5heL2R   = new TH1D("hW5heL2R","W 5 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW6heL2R   = new TH1D("hW6heL2R","W 6 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW7heL2R   = new TH1D("hW7heL2R","W 7 t L2; W;", 300, 2.1, 2.3);
  TH1D *hW8heL2R   = new TH1D("hW8heL2R","W 8 t L2; W;", 300, 2.1, 2.3);

  //Q2

  TH1D *hQ21heC   = new TH1D("hQ21heC","Q2 1 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heC   = new TH1D("hQ22heC","Q2 2 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heC   = new TH1D("hQ23heC","Q2 3 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heC   = new TH1D("hQ24heC","Q2 4 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heC   = new TH1D("hQ25heC","Q2 5 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heC   = new TH1D("hQ26heC","Q2 6 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heC   = new TH1D("hQ27heC","Q2 7 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heC   = new TH1D("hQ28heC","Q2 8 t C; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21heR1  = new TH1D("hQ21heR1","Q2 1 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heR1  = new TH1D("hQ22heR1","Q2 2 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heR1  = new TH1D("hQ23heR1","Q2 3 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heR1  = new TH1D("hQ24heR1","Q2 4 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heR1  = new TH1D("hQ25heR1","Q2 5 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heR1  = new TH1D("hQ26heR1","Q2 6 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heR1  = new TH1D("hQ27heR1","Q2 7 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heR1  = new TH1D("hQ28heR1","Q2 8 t R1 Q2;", 300, 0.2, 0.6);

  TH1D *hQ21heR2  = new TH1D("hQ21heR2","Q2 1 t R2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heR2  = new TH1D("hQ22heR2","Q2 2 t R2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heR2  = new TH1D("hQ23heR2","Q2 3 t R2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heR2  = new TH1D("hQ24heR2","Q2 4 t R2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heR2  = new TH1D("hQ25heR2","Q2 5 t R2 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heR2  = new TH1D("hQ26heR2","Q2 6 t R2 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heR2  = new TH1D("hQ27heR2","Q2 7 t R2 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heR2  = new TH1D("hQ28heR2","Q2 8 t R2 Q2;", 300, 0.2, 0.6);
 
  TH1D *hQ21heL1  = new TH1D("hQ21heL1","Q2 1 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heL1  = new TH1D("hQ22heL1","Q2 2 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heL1  = new TH1D("hQ23heL1","Q2 3 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heL1  = new TH1D("hQ24heL1","Q2 4 t L1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heL1  = new TH1D("hQ25heL1","Q2 5 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heL1  = new TH1D("hQ26heL1","Q2 6 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heL1  = new TH1D("hQ27heL1","Q2 7 t L1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heL1  = new TH1D("hQ28heL1","Q2 8 t L1 Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21heL2  = new TH1D("hQ21heL2","Q2 1 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22heL2  = new TH1D("hQ22heL2","Q2 2 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23heL2  = new TH1D("hQ23heL2","Q2 3 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24heL2  = new TH1D("hQ24heL2","Q2 4 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25heL2  = new TH1D("hQ25heL2","Q2 5 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26heL2  = new TH1D("hQ26heL2","Q2 6 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27heL2  = new TH1D("hQ27heL2","Q2 7 t L2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28heL2  = new TH1D("hQ28heL2","Q2 8 t L2 Q2;", 300, 0.2, 0.6);      

  //Q2 Random

  TH1D *hQ21heCR   = new TH1D("hQ21heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heCR   = new TH1D("hQ22heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heCR   = new TH1D("hQ23heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heCR   = new TH1D("hQ24heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heCR   = new TH1D("hQ25heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heCR   = new TH1D("hQ26heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heCR   = new TH1D("hQ27heCR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heCR   = new TH1D("hQ28heCR","Q2; Q2;", 300, 0.2, 0.6);

  TH1D *hQ21heR1R  = new TH1D("hQ21heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heR1R  = new TH1D("hQ22heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heR1R  = new TH1D("hQ23heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heR1R  = new TH1D("hQ24heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heR1R  = new TH1D("hQ25heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heR1R  = new TH1D("hQ26heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heR1R  = new TH1D("hQ27heR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heR1R  = new TH1D("hQ28heR1R","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21heR2R  = new TH1D("hQ21heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heR2R  = new TH1D("hQ22heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heR2R  = new TH1D("hQ23heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heR2R  = new TH1D("hQ24heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heR2R  = new TH1D("hQ25heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heR2R  = new TH1D("hQ26heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heR2R  = new TH1D("hQ27heR2R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heR2R  = new TH1D("hQ28heR2R","Q2; Q2;", 300, 0.2, 0.6);

  TH1D *hQ21heL1R  = new TH1D("hQ21heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22heL1R  = new TH1D("hQ22heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23heL1R  = new TH1D("hQ23heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24heL1R  = new TH1D("hQ24heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25heL1R  = new TH1D("hQ25heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26heL1R  = new TH1D("hQ26heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27heL1R  = new TH1D("hQ27heL1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28heL1R  = new TH1D("hQ28heL1R","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21heL2R  = new TH1D("hQ21heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22heL2R  = new TH1D("hQ22heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23heL2R  = new TH1D("hQ23heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24heL2R  = new TH1D("hQ24heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25heL2R  = new TH1D("hQ25heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26heL2R  = new TH1D("hQ26heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27heL2R  = new TH1D("hQ27heL2R","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28heL2R  = new TH1D("hQ28heL2R","Q2; Q2;", 300, 0.2, 0.6);      

  //t
  TH1D *ht1heC  = new TH1D("ht1heC","t 1 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heC  = new TH1D("ht2heC","t 2 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heC  = new TH1D("ht3heC","t 3 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heC  = new TH1D("ht4heC","t 4 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heC  = new TH1D("ht5heC","t 5 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heC  = new TH1D("ht6heC","t 6 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heC  = new TH1D("ht7heC","t 7 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heC  = new TH1D("ht8heC","t 8 t C; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heR1  = new TH1D("ht1heR1","t 1 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heR1  = new TH1D("ht2heR1","t 2 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heR1  = new TH1D("ht3heR1","t 3 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heR1  = new TH1D("ht4heR1","t 4 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heR1  = new TH1D("ht5eR1","t 5 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heR1  = new TH1D("ht6heR1","t 6 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heR1  = new TH1D("ht7heR1","t 7 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heR1  = new TH1D("ht8heR1","t 8 t R1; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heR2  = new TH1D("ht1heR2","t 1 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heR2  = new TH1D("ht2heR2","t 2 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heR2  = new TH1D("ht3heR2","t 3 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heR2  = new TH1D("ht4heR2","t 4 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heR2  = new TH1D("ht5eR2","t 5 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heR2  = new TH1D("ht6heR2","t 6 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heR2  = new TH1D("ht7heR2","t 7 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heR2  = new TH1D("ht8heR2","t 8 t R2; MandelT;",   300, -0.01, 0.1);      
  
  TH1D *ht1heL1  = new TH1D("ht1heL1","t 1 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heL1  = new TH1D("ht2heL1","t 2 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heL1  = new TH1D("ht3heL1","t 3 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heL1  = new TH1D("ht4heL1","t 4 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heL1  = new TH1D("ht5heL1","t 5 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heL1  = new TH1D("ht6heL1","t 6 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heL1  = new TH1D("ht7heL1","t 7 t L1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heL1  = new TH1D("ht8heL1","t 8 t L1; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heL2  = new TH1D("ht1heL2","t 1 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heL2  = new TH1D("ht2heL2","t 2 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heL2  = new TH1D("ht3heL2","t 3 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heL2  = new TH1D("ht4heL2","t 4 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heL2  = new TH1D("ht5heL2","t 5 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heL2  = new TH1D("ht6heL2","t 6 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heL2  = new TH1D("ht7heL2","t 7 t L2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heL2  = new TH1D("ht8heL2","t 8 t L2; MandelT;",   300, -0.01, 0.1);      

  //t Random
  TH1D *ht1heCR  = new TH1D("ht1heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heCR  = new TH1D("ht2heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heCR  = new TH1D("ht3heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heCR  = new TH1D("ht4heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heCR  = new TH1D("ht5heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heCR  = new TH1D("ht6heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heCR  = new TH1D("ht7heCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heCR  = new TH1D("ht8heCR","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heR1R  = new TH1D("ht1heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heR1R  = new TH1D("ht2heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heR1R  = new TH1D("ht3heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heR1R  = new TH1D("ht4heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heR1R  = new TH1D("ht5heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heR1R  = new TH1D("ht6heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heR1R  = new TH1D("ht7heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heR1R  = new TH1D("ht8heR1R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heR2R  = new TH1D("ht1heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heR2R  = new TH1D("ht2heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heR2R  = new TH1D("ht3heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heR2R  = new TH1D("ht4heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heR2R  = new TH1D("ht5heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heR2R  = new TH1D("ht6heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heR2R  = new TH1D("ht7heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heR2R  = new TH1D("ht8heR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  
  TH1D *ht1heL1R  = new TH1D("ht1heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heL1R  = new TH1D("ht2heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heL1R  = new TH1D("ht3heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heL1R  = new TH1D("ht4heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heL1R  = new TH1D("ht5heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heL1R  = new TH1D("ht6heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heL1R  = new TH1D("ht7heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heL1R  = new TH1D("ht8heL1R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1heL2R  = new TH1D("ht1heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2heL2R  = new TH1D("ht2heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3heL2R  = new TH1D("ht3heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4heL2R  = new TH1D("ht4heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5heL2R  = new TH1D("ht5heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6heL2R  = new TH1D("ht6heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7heL2R  = new TH1D("ht7heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8heL2R  = new TH1D("ht8heL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheR1  = new TH1D("htheR1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheR2  = new TH1D("htheR2","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheC   = new TH1D("htheC","MandelT; MandelT;",    300, -0.01, 0.1);      
  TH1D *htheL1  = new TH1D("htheL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2  = new TH1D("htheL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheInR1  = new TH1D("htheInR1","MandelT; MandelT;",   10, bins);      
  TH1D *htheInR2  = new TH1D("htheInR2","MandelT; MandelT;",   10, bins);      
  TH1D *htheInC   = new TH1D("htheInC","MandelT; MandelT;",    10, bins);      
  TH1D *htheInL1  = new TH1D("htheInL1","MandelT; MandelT;",   10, bins);      
  TH1D *htheInL2  = new TH1D("htheInL2","MandelT; MandelT;",   10, bins);      

  TH1D *htheR1R  = new TH1D("htheR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheR2R  = new TH1D("htheR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheCR   = new TH1D("htheCR","MandelT; MandelT;",     300, -0.01, 0.1);      
  TH1D *htheL1R  = new TH1D("htheL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2R  = new TH1D("htheL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheInR1R  = new TH1D("htheInR1R","MandelT; MandelT;",   10, bins);      
  TH1D *htheInR2R  = new TH1D("htheInR2R","MandelT; MandelT;",   10, bins);      
  TH1D *htheInCR   = new TH1D("htheInCR","MandelT; MandelT;",    10, bins);      
  TH1D *htheInL1R  = new TH1D("htheInL1R","MandelT; MandelT;",   10, bins);      
  TH1D *htheInL2R  = new TH1D("htheInL2R","MandelT; MandelT;",   10, bins);      

  TH1D *hph_qheR1  = new TH1D("hph_qheR1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheR2  = new TH1D("hph_qheR2","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheC   = new TH1D("hph_qheC","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qheL1  = new TH1D("hph_qheL1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheL2  = new TH1D("hph_qheL2","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH1D *hph_qheR1R  = new TH1D("hph_qheR1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheR2R  = new TH1D("hph_qheR2R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheCR   = new TH1D("hph_qheCR","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qheL1R  = new TH1D("hph_qheL1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheL2R  = new TH1D("hph_qheL2R","ph_q; ph_q;",   18, -22.5, 382.5);      
  
  TH2D *hYheR1   = new TH2D("hYheR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheR2   = new TH2D("hYheR2"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheC    = new TH2D("hYheC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYheL1   = new TH2D("hYheL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheL2   = new TH2D("hYheL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYheR1R   = new TH2D("hYheR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheR2R   = new TH2D("hYheR2R"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheCR    = new TH2D("hYheCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYheL1R   = new TH2D("hYheL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheL2R   = new TH2D("hYheL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      
  
  /*
  TH2D *hYheR1   = new TH2D("hYheR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheR2   = new TH2D("hYheR2"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheC    = new TH2D("hYheC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheL1   = new TH2D("hYheL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheL2   = new TH2D("hYheL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);

  TH2D *hYheR1R   = new TH2D("hYheR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheR2R   = new TH2D("hYheR2R"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheCR    = new TH2D("hYheCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheL1R   = new TH2D("hYheL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYheL2R   = new TH2D("hYheL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);      
  */ 
 //Dummy

  TH2D *hYhedR1   = new TH2D("hYhedR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedR2   = new TH2D("hYhedR2"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedC    = new TH2D("hYhedC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYhedL1   = new TH2D("hYhedL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedL2   = new TH2D("hYhedL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYhedR1R   = new TH2D("hYhedR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedR2R   = new TH2D("hYhedR2R"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedCR    = new TH2D("hYhedCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYhedL1R   = new TH2D("hYhedL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYhedL2R   = new TH2D("hYhedL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      
  /*
  TH2D *hYhedR1   = new TH2D("hYhedR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedR2   = new TH2D("hYhedR2"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedC    = new TH2D("hYhedC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedL1   = new TH2D("hYhedL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedL2   = new TH2D("hYhedL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);

  TH2D *hYhedR1R   = new TH2D("hYhedR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedR2R   = new TH2D("hYhedR2R"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedCR    = new TH2D("hYhedCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedL1R   = new TH2D("hYhedL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYhedL2R   = new TH2D("hYhedL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);      
  */
  //SIMC

  TH2D *hYSheR1   = new TH2D("hYSheR1"," Yield SIMC R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSheR2   = new TH2D("hYSheR2"," Yield SIMC R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSheC    = new TH2D("hYSheC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSheL1   = new TH2D("hYSheL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYSheL2   = new TH2D("hYSheL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  hYSheC->Sumw2();
  hYSheL1->Sumw2();
  hYSheL2->Sumw2();
  hYSheR1->Sumw2();
  hYSheR2->Sumw2();
  /*
  TH2D *hYSheR1   = new TH2D("hYSheR1"," Yield SIMC R1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSheR2   = new TH2D("hYSheR2"," Yield SIMC R2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSheC   = new TH2D("hYSheC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSheL1   = new TH2D("hYSheL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  TH2D *hYSheL2   = new TH2D("hYSheL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, PhiBinLow, PhiBinHigh);
  */
  TH1D *hmmheR1  = new TH1D("hmmheR1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheR2  = new TH1D("hmmheR2","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheC   = new TH1D("hmmheC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL1  = new TH1D("hmmheL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL2  = new TH1D("hmmheL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmheR1R  = new TH1D("hmmheR1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheR2R  = new TH1D("hmmheR2R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheCR   = new TH1D("hmmheCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL1R  = new TH1D("hmmheL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL2R  = new TH1D("hmmheL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinheR1   = new TH1D("htcoinheR1","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinheR2   = new TH1D("htcoinheR2","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinheC    = new TH1D("htcoinheC","; ;",   300, -20.0, 20.0);      
  TH1D *htcoinheL1   = new TH1D("htcoinheL1","; ;",  300, -20.0, 20.0);      
  TH1D *htcoinheL2   = new TH1D("htcoinheL2","; ;",  300, -20.0, 20.0);      

  //SIMC and Data HISTOGRAMS  
  //RIGHT1
  TH1D *hphheR1  = new TH1D("hphheR1","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphheR1R  = new TH1D("hphheR1R","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheSR1  = new TH1D("hphheSR1","ph_q; ph_q;", 50, -10, 370);      

  //RIGHT2
  TH1D *hphheR2  = new TH1D("hphheR2","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphheR2R  = new TH1D("hphheR2R","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphhedR2  = new TH1D("hphhedR2","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphhedR2R  = new TH1D("hphhedR2R","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheSR2  = new TH1D("hphheSR2","ph_q; ph_q;", 50, -10, 370);      

  /*
  TH1D *hphheR1  = new TH1D("hphheR1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheR1R  = new TH1D("hphheR1R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheSR1  = new TH1D("hphheSR1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      

  TH1D *hphheR2  = new TH1D("hphheR2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheR2R  = new TH1D("hphheR2R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheSR2  = new TH1D("hphheSR2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  //  TH2D *hYSheR1   = new TH2D("hYSheR1"," Yield SIMC R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  //  TH2D *hYSheR2   = new TH2D("hYSheR2"," Yield SIMC R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SheR1  = new TH1D("H_ssdelta_SheR1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SheR1  = new TH1D("H_ssxptar_SheR1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SheR1  = new TH1D("H_ssyptar_SheR1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheR1  = new TH1D("H_ssdelta_DheR1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheR1  = new TH1D("H_ssxptar_DheR1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheR1  = new TH1D("H_ssyptar_DheR1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheR1R  = new TH1D("H_ssdelta_DheR1R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheR1R  = new TH1D("H_ssxptar_DheR1R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheR1R  = new TH1D("H_ssyptar_DheR1R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SheR1  = new TH1D("H_hsdelta_SheR1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SheR1  = new TH1D("H_hsxptar_SheR1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SheR1  = new TH1D("H_hsyptar_SheR1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheR1  = new TH1D("H_hsdelta_DheR1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheR1  = new TH1D("H_hsxptar_DheR1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheR1  = new TH1D("H_hsyptar_DheR1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheR1R  = new TH1D("H_hsdelta_DheR1R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheR1R  = new TH1D("H_hsxptar_DheR1R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheR1R  = new TH1D("H_hsyptar_DheR1R","HMS yptar; hsyptar;", 50, -0.05, 0.05);


  TH1D *H_ssdelta_SheR2  = new TH1D("H_ssdelta_SheR2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SheR2  = new TH1D("H_ssxptar_SheR2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SheR2  = new TH1D("H_ssyptar_SheR2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  TH1D *hQ2heSR2  = new TH1D("hQ2DheSR2"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hWheSR2   = new TH1D("hWheSR2"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hepheSR2   = new TH1D("hepheSR2"," ;epsilon; Yield (events/mC)", 150, 0.70, 0.85);
  
  TH1D *H_ssdelta_DheR2  = new TH1D("H_ssdelta_DheR2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *hQ2DheR2  = new TH1D("hQ2DheR2"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hWhDeR2   = new TH1D("hWhDeR2"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hephDeR2   = new TH1D("hephDeR2"," ;epsilon; Yield (events/mC)", 150, 0.70, 0.85);
  TH1D *H_ssxptar_DheR2  = new TH1D("H_ssxptar_DheR2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheR2  = new TH1D("H_ssyptar_DheR2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DummyheR2  = new TH1D("H_ssdelta_DummyheR2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *hQ2DummyheR2  = new TH1D("hQ2DummyheR2"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6);
  TH1D *hWhDummyeR2   = new TH1D("hWhDummyeR2"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hephDummyeR2   = new TH1D("hephDummyeR2"," ;epsilon; Yield (events/mC)", 150, 0.70, 0.85);
  TH1D *H_ssxptar_DummyheR2  = new TH1D("H_ssxptar_DummyheR2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DummyheR2  = new TH1D("H_ssyptar_DummyheR2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheR2R  = new TH1D("H_ssdelta_DheR2R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *hQ2DheR2R  = new TH1D("hQ2DheR2R"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6); 
  TH1D *hWhDeR2R   = new TH1D("hWhDeR2R"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hephDeR2R  = new TH1D("hephDeR2R"," ;epsilon; Yield (events/mC)", 150, 0.70, 0.85);
  TH1D *H_ssxptar_DheR2R  = new TH1D("H_ssxptar_DheR2R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheR2R  = new TH1D("H_ssyptar_DheR2R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DummyheR2R  = new TH1D("H_ssdelta_DummyheR2R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *hQ2DummyheR2R  = new TH1D("hQ2DummyheR2R"," ; Q^{2} (GeV^{2}); Yield (Events/mC)", 150, 0.2, 0.6); 
  TH1D *hWhDummyeR2R   = new TH1D("hWhDummyeR2R"," ;W (GeV); Yield (Events/mC)", 150, 2.14, 2.26);
  TH1D *hephDummyeR2R  = new TH1D("hephDummyeR2R"," ;epsilon; Yield (events/mC)", 150, 0.70, 0.85);
  TH1D *H_ssxptar_DummyheR2R  = new TH1D("H_ssxptar_DummyheR2R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DummyheR2R  = new TH1D("H_ssyptar_DummyheR2R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);  
  
  TH1D *H_hsdelta_SheR2  = new TH1D("H_hsdelta_SheR2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SheR2  = new TH1D("H_hsxptar_SheR2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SheR2  = new TH1D("H_hsyptar_SheR2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheR2  = new TH1D("H_hsdelta_DheR2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheR2  = new TH1D("H_hsxptar_DheR2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheR2  = new TH1D("H_hsyptar_DheR2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DummyheR2  = new TH1D("H_hsdelta_DummyheR2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DummyheR2  = new TH1D("H_hsxptar_DummyheR2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DummyheR2  = new TH1D("H_hsyptar_DummyheR2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheR2R  = new TH1D("H_hsdelta_DheR2R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheR2R  = new TH1D("H_hsxptar_DheR2R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheR2R  = new TH1D("H_hsyptar_DheR2R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DummyheR2R  = new TH1D("H_hsdelta_DummyheR2R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DummyheR2R  = new TH1D("H_hsxptar_DummyheR2R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DummyheR2R  = new TH1D("H_hsyptar_DummyheR2R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //CENTER  
  TH1D *hphheC  = new TH1D("hphheC","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheCR  = new TH1D("hphheCR","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheSC  = new TH1D("hphheSC","ph_q; ph_q;", 50, -10, 370);      
  /*
  TH1D *hphheC  = new TH1D("hphheC","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheCR  = new TH1D("hphheCR","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheSC  = new TH1D("hphheSC","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  //  TH2D *hYSheC   = new TH2D("hYSheC"," Yield SIMC C;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SheC  = new TH1D("H_ssdelta_SheC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SheC  = new TH1D("H_ssxptar_SheC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SheC  = new TH1D("H_ssyptar_SheC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheC  = new TH1D("H_ssdelta_DheC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheC  = new TH1D("H_ssxptar_DheC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheC  = new TH1D("H_ssyptar_DheC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheCR  = new TH1D("H_ssdelta_DheCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheCR  = new TH1D("H_ssxptar_DheCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheCR  = new TH1D("H_ssyptar_DheCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SheC  = new TH1D("H_hsdelta_SheC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SheC  = new TH1D("H_hsxptar_SheC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SheC  = new TH1D("H_hsyptar_SheC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheC  = new TH1D("H_hsdelta_DheC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheC  = new TH1D("H_hsxptar_DheC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheC  = new TH1D("H_hsyptar_DheC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheCR  = new TH1D("H_hsdelta_DheCR","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheCR  = new TH1D("H_hsxptar_DheCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheCR  = new TH1D("H_hsyptar_DheCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //LEFT1  
  TH1D *hphheL1  = new TH1D("hphheL1","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheL1R  = new TH1D("hphheL1R","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheSL1  = new TH1D("hphheSL1","ph_q; ph_q;", 50, -10, 370);      
  /*
  TH1D *hphheL1  = new TH1D("hphheL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheL1R  = new TH1D("hphheL1R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheSL1  = new TH1D("hphheSL1","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  //  TH2D *hYSheL1   = new TH2D("hYSheL1"," Yield SIMC L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SheL1  = new TH1D("H_ssdelta_SheL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SheL1  = new TH1D("H_ssxptar_SheL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SheL1  = new TH1D("H_ssyptar_SheL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheL1  = new TH1D("H_ssdelta_DheL1","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheL1  = new TH1D("H_ssxptar_DheL1","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheL1  = new TH1D("H_ssyptar_DheL1","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheL1R  = new TH1D("H_ssdelta_DheL1R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheL1R  = new TH1D("H_ssxptar_DheL1R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheL1R  = new TH1D("H_ssyptar_DheL1R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SheL1  = new TH1D("H_hsdelta_SheL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SheL1  = new TH1D("H_hsxptar_SheL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SheL1  = new TH1D("H_hsyptar_SheL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheL1  = new TH1D("H_hsdelta_DheL1","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheL1  = new TH1D("H_hsxptar_DheL1","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheL1  = new TH1D("H_hsyptar_DheL1","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheL1R  = new TH1D("H_hsdelta_DheL1R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheL1R  = new TH1D("H_hsxptar_DheL1R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheL1R  = new TH1D("H_hsyptar_DheL1R","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  //LEFT2  
  TH1D *hphheL2  = new TH1D("hphheL2","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheL2R  = new TH1D("hphheL2R","ph_q; ph_q;", 50, -10, 370);      
  TH1D *hphheSL2  = new TH1D("hphheSL2","ph_q; ph_q;", 50, -10, 370);      
  /*
  TH1D *hphheL2  = new TH1D("hphheL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheL2R  = new TH1D("hphheL2R","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphheSL2  = new TH1D("hphheSL2","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  //  TH2D *hYSheL2   = new TH2D("hYSheL2"," Yield SIMC L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH1D *H_ssdelta_SheL2  = new TH1D("H_ssdelta_SheL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SheL2  = new TH1D("H_ssxptar_SheL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SheL2  = new TH1D("H_ssyptar_SheL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheL2  = new TH1D("H_ssdelta_DheL2","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheL2  = new TH1D("H_ssxptar_DheL2","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheL2  = new TH1D("H_ssyptar_DheL2","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DheL2R  = new TH1D("H_ssdelta_DheL2R","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DheL2R  = new TH1D("H_ssxptar_DheL2R","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DheL2R  = new TH1D("H_ssyptar_DheL2R","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SheL2  = new TH1D("H_hsdelta_SheL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SheL2  = new TH1D("H_hsxptar_SheL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SheL2  = new TH1D("H_hsyptar_SheL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheL2  = new TH1D("H_hsdelta_DheL2","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheL2  = new TH1D("H_hsxptar_DheL2","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheL2  = new TH1D("H_hsyptar_DheL2","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DheL2R  = new TH1D("H_hsdelta_DheL2R","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DheL2R  = new TH1D("H_hsxptar_DheL2R","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DheL2R  = new TH1D("H_hsyptar_DheL2R","HMS yptar; hsyptar;", 50, -0.05, 0.05);
  
 //*************************
  //plot added on Apr 17, 2024
  //MMpi 
  TH1D *MPiheR1   = new TH1D("MPiheR1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheR2   = new TH1D("MPiheR2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheC   = new TH1D("MPiheC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheL1   = new TH1D("MPiheL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheL2   = new TH1D("MPiheL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPiheR1R   = new TH1D("MPiheR1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheR2R   = new TH1D("MPiheR2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheCR   = new TH1D("MPiheCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheL1R   = new TH1D("MPiheL1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiheL2R   = new TH1D("MPiheL2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Dummy
  TH1D *MPihedR1   = new TH1D("MPihedR1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedR2   = new TH1D("MPihedR2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedC   = new TH1D("MPihedC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedL1   = new TH1D("MPihedL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedL2   = new TH1D("MPihedL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPihedR1R   = new TH1D("MPihedR1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedR2R   = new TH1D("MPihedR2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedCR   = new TH1D("MPihedCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedL1R   = new TH1D("MPihedL1R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPihedL2R   = new TH1D("MPihedL2R"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //SIMC
  TH1D *MPiSheR1   = new TH1D("MPiSheR1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSheR2   = new TH1D("MPiSheR2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSheC   = new TH1D("MPiSheC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSheL1   = new TH1D("MPiSheL1"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  TH1D *MPiSheL2   = new TH1D("MPiSheL2"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      

  //t
  TH1D *hththeR1   = new TH1D("hththeR1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeR2   = new TH1D("hththeR2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeC   = new TH1D("hththeC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeL1   = new TH1D("hththeL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeL2   = new TH1D("hththeL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hththeR1R   = new TH1D("hththeR1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeR2R   = new TH1D("hththeR2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeCR   = new TH1D("hththeCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeL1R   = new TH1D("hththeL1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththeL2R   = new TH1D("hththeL2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Dummy
  TH1D *hththedR1   = new TH1D("hththedR1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedR2   = new TH1D("hththedR2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedC   = new TH1D("hththedC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedL1   = new TH1D("hththedL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedL2   = new TH1D("hththedL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hththedR1R   = new TH1D("hththedR1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedR2R   = new TH1D("hththedR2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedCR   = new TH1D("hththedCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedL1R   = new TH1D("hththedL1R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hththedL2R   = new TH1D("hththedL2R"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //SIMC
  TH1D *hthtSheR1   = new TH1D("hthtSheR1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSheR2   = new TH1D("hthtSheR2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSheC   = new TH1D("hthtSheC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSheL1   = new TH1D("hthtSheL1"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  TH1D *hthtSheL2   = new TH1D("hthtSheL2"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      

  //Added on March 26, 2025
  
  TH2D *shheR2   = CreateHistogram("shheR2",  " ");
  TH2D *shheR1   = CreateHistogram("shheR1",  " ");
  TH2D *shheC    = CreateHistogram("shheC",   " ");
  TH2D *shheL1   = CreateHistogram("shheL1",  " ");
  TH2D *shheL2   = CreateHistogram("shheL2",  " ");

  //*************************
  /*
  Double_t MMpiOffsetheR1 = -0.000966;
  Double_t MMpiOffsetheR2 = -0.000966;
  Double_t MMpiOffsetheC  = -0.000321;
  Double_t MMpiOffsetheL1 = -0.000304;
  Double_t MMpiOffsetheL2 = -0.000371;
  */
  Double_t MMpiOffsetheR1 = 0.0015;
  Double_t MMpiOffsetheR2 = 0.0015;
  Double_t MMpiOffsetheC  = 0.0025;
  Double_t MMpiOffsetheL1 = 0.0031;
  Double_t MMpiOffsetheL2 = 0.004;
  
  Double_t tOffsetheR1 = 0.000;
  Double_t tOffsetheR2 = 0.000;
  Double_t tOffsetheC = 0.000;
  Double_t tOffsetheL1 = 0.0005;
  Double_t tOffsetheL2 = 0.0005;
   
  /*
  Double_t tOffsetheR1 = 0.00;
  Double_t tOffsetheR2 = 0.00;
  Double_t tOffsetheL1 = -0.00;
  Double_t tOffsetheL2 = -0.00;
  */
  /*
  Double_t tOffsetheR1 = 0.00;
  Double_t tOffsetheL1 = 0.00;
  Double_t tOffsetheL2 = 0.00;
  */

  //Data R1

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHER1; i++)

    {
      TBRANCHHIGHER1->GetEntry(i);
      
      if(tcoinheR1-TimmingOffset>=-1.0 && tcoinheR1-TimmingOffset <= 1.0)    
	{
	  hmmheR1->Fill(mmheR1);	  
	}	      
      
      {
	htcoinheR1->Fill(tcoinheR1-TimmingOffset);
      }

      Double_t DiamondR1 = (Dcut->IsInside(Q2HER1, WHER1));  
      Double_t MMpiR1 = mmheR1>=MMPICUTL-MMpiOffsetheR1 && mmheR1 <=MMPICUTH-MMpiOffsetheR1;
      Double_t CoinPionR1 = tcoinheR1-TimmingOffset>=-1.0 && tcoinheR1-TimmingOffset <= 1.0;      
      Double_t CoinPionR1R = ((tcoinheR1-TimmingOffset>=-15.0 && tcoinheR1-TimmingOffset <= -9.0) || (tcoinheR1-TimmingOffset>=7.0 && tcoinheR1-TimmingOffset <=13.0));      
      
      if(CoinPionR1 && DiamondR1)
	{
	  MPiheR1->Fill(mmheR1+MMpiOffsetheR1);
	}
      if(CoinPionR1 && MMpiR1 && DiamondR1)
	{
          H_ssdelta_DheR1->Fill(P_gtr_dpheR1);
          H_ssxptar_DheR1->Fill(P_gtr_xptarheR1);
          H_ssyptar_DheR1->Fill(P_gtr_yptarheR1);

          H_hsdelta_DheR1->Fill(H_gtr_dpheR1);
          H_hsxptar_DheR1->Fill(H_gtr_xptarheR1);
          H_hsyptar_DheR1->Fill(H_gtr_yptarheR1);

	  hththeR1->Fill(TMath::Abs(theR1)+tOffsetheR1);
	  htheR1->Fill(TMath::Abs(theR1));
	  htheInR1->Fill(TMath::Abs(theR1));
	  hph_qheR1->Fill(ph_qheR1*57.2958 + 180);
	  hphheR1->Fill(std::fmod(ph_qheR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheR1->Fill(mmheR1+MMpiOffsetheR1);
	  hYheR1->Fill(TMath::Abs(theR1)+tOffsetheR1, std::fmod(ph_qheR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheR1->Fill(-theR1, mmheR1+MMpiOffsetheR1);

	  //	  shheR1->Fill(-theR1*sin(ph_qheR1*57.2958), -theR1*cos(ph_qheR1*57.2958));
	  shheR1->Fill(TMath::Abs(theR1)*cos(ph_qheR1), TMath::Abs(theR1)*sin(ph_qheR1));
	}
      //Random
      if(CoinPionR1R && DiamondR1)
	{
	  MPiheR1R->Fill(mmheR1+MMpiOffsetheR1);
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1)
	{
          H_ssdelta_DheR1R->Fill(P_gtr_dpheR1);
          H_ssxptar_DheR1R->Fill(P_gtr_xptarheR1);
          H_ssyptar_DheR1R->Fill(P_gtr_yptarheR1);

          H_hsdelta_DheR1R->Fill(H_gtr_dpheR1);
          H_hsxptar_DheR1R->Fill(H_gtr_xptarheR1);
          H_hsyptar_DheR1R->Fill(H_gtr_yptarheR1);

	  hththeR1R->Fill(TMath::Abs(theR1)+tOffsetheR1);
	  htheR1R->Fill(TMath::Abs(theR1));
	  htheInR1R->Fill(TMath::Abs(theR1));
	  hph_qheR1R->Fill(ph_qheR1*57.2958 + 180);
	  hphheR1R->Fill(std::fmod(ph_qheR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheR1R->Fill(mmheR1+MMpiOffsetheR1);
	  hYheR1R->Fill(TMath::Abs(theR1)+tOffsetheR1, std::fmod(ph_qheR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheR1R->Fill(-theR1, mmheR1+MMpiOffsetheR1);
	}
      /* Double_t w1t = -theR1+tOffsetheR1 >= bins[1] && -theR1+tOffsetheR1 <= bins[2];
      Double_t w2t = -theR1+tOffsetheR1 >= bins[2] && -theR1+tOffsetheR1 <= bins[3];
      Double_t w3t = -theR1+tOffsetheR1 >= bins[3] && -theR1+tOffsetheR1 <= bins[4];
      Double_t w4t = -theR1+tOffsetheR1 >= bins[4] && -theR1+tOffsetheR1 <= bins[5];
      Double_t w5t = -theR1+tOffsetheR1 >= bins[5] && -theR1+tOffsetheR1 <= bins[6];
      Double_t w6t = -theR1+tOffsetheR1 >= bins[6] && -theR1+tOffsetheR1 <= bins[7];
      Double_t w7t = -theR1+tOffsetheR1 >= bins[7] && -theR1+tOffsetheR1 <= bins[8];
      Double_t w8t = -theR1+tOffsetheR1 >= bins[8] && -theR1+tOffsetheR1 <= bins[9];
      */
      Double_t w1t = TMath::Abs(theR1) >= bins[1] && TMath::Abs(theR1) <= bins[2];
      Double_t w2t = TMath::Abs(theR1) >= bins[2] && TMath::Abs(theR1) <= bins[3];
      Double_t w3t = TMath::Abs(theR1) >= bins[3] && TMath::Abs(theR1) <= bins[4];
      Double_t w4t = TMath::Abs(theR1) >= bins[4] && TMath::Abs(theR1) <= bins[5];
      Double_t w5t = TMath::Abs(theR1) >= bins[5] && TMath::Abs(theR1) <= bins[6];
      Double_t w6t = TMath::Abs(theR1) >= bins[6] && TMath::Abs(theR1) <= bins[7];
      Double_t w7t = TMath::Abs(theR1) >= bins[7] && TMath::Abs(theR1) <= bins[8];
      Double_t w8t = TMath::Abs(theR1) >= bins[8] && TMath::Abs(theR1) <= bins[9];

      
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w1t)
	{
	  hW1heR1->Fill(WHER1);	
	  hQ21heR1->Fill(Q2HER1);	      	  
	  ht1heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w2t)
	{
	  hW2heR1->Fill(WHER1);	
	  hQ22heR1->Fill(Q2HER1);	      	  
	  ht2heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w3t)
	{
	  hW3heR1->Fill(WHER1);	
	  hQ23heR1->Fill(Q2HER1);	      	  
	  ht3heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w4t)
	{
	  hW4heR1->Fill(WHER1);	
	  hQ24heR1->Fill(Q2HER1);	      	  
	  ht4heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w5t)
	{
	  hW5heR1->Fill(WHER1);	
	  hQ25heR1->Fill(Q2HER1);	      	  
	  ht5heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w6t)
	{
	  hW6heR1->Fill(WHER1);	
	  hQ26heR1->Fill(Q2HER1);	      	  
	  ht6heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w7t)
	{
	  hW7heR1->Fill(WHER1);	
	  hQ27heR1->Fill(Q2HER1);	      	  
	  ht7heR1->Fill(TMath::Abs(theR1)); 
 	}	
      if(CoinPionR1 && MMpiR1 && DiamondR1 && w8t)
	{
	  hW8heR1->Fill(WHER1);	
	  hQ28heR1->Fill(Q2HER1);	      	  
	  ht8heR1->Fill(TMath::Abs(theR1)); 
 	}	
      //Random
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w1t)
	
	{
	  hW1heR1R->Fill(WHER1);	
	  hQ21heR1R->Fill(Q2HER1);	      	  
	  ht1heR1R->Fill(TMath::Abs(theR1)); 
	}

      if(CoinPionR1R && MMpiR1 && DiamondR1 && w2t)
	
	{
	  hW2heR1R->Fill(WHER1);	
	  hQ22heR1R->Fill(Q2HER1);	      	  
	  ht2heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w3t)
	
	{
	  hW3heR1R->Fill(WHER1);	
	  hQ23heR1R->Fill(Q2HER1);	      	  
	  ht3heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w4t)
	
	{
	  hW4heR1R->Fill(WHER1);	
	  hQ24heR1R->Fill(Q2HER1);	      	  
	  ht4heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w5t)
	
	{
	  hW5heR1R->Fill(WHER1);	
	  hQ25heR1R->Fill(Q2HER1);	      	  
	  ht5heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w6t)
	
	{
	  hW6heR1R->Fill(WHER1);	
	  hQ26heR1R->Fill(Q2HER1);	      	  
	  ht6heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w7t)
	
	{
	  hW7heR1R->Fill(WHER1);	
	  hQ27heR1R->Fill(Q2HER1);	      	  
	  ht7heR1R->Fill(TMath::Abs(theR1)); 
	}
      if(CoinPionR1R && MMpiR1 && DiamondR1 && w8t)
	
	{
	  hW8heR1R->Fill(WHER1);	
	  hQ28heR1R->Fill(Q2HER1);	      	  
	  ht8heR1R->Fill(TMath::Abs(theR1)); 
	}

      if (((tcoinheR1-TimmingOffset>=-15.0 && tcoinheR1-TimmingOffset <= -9.0) || (tcoinheR1-TimmingOffset>=7.0 && tcoinheR1-TimmingOffset <=13.0)))
	
	{
	  hmmheR1R->Fill(mmheR1);
	}	
    }  
  //Dummy R1
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEDR1; i++)
    {
      TBRANCHHIGHEDR1->GetEntry(i);

      Double_t DiamonddR1 = (Dcut->IsInside(Q2HEDR1, WHEDR1));  
      Double_t MMpidR1 = mmhedR1>=MMPICUTL-MMpiOffsetheR1 && mmhedR1 <=MMPICUTH-MMpiOffsetheR1;
      Double_t CoinPiondR1 = tcoinhedR1-TimmingOffset>=-1.0 && tcoinhedR1-TimmingOffset <= 1.0;      
      Double_t CoinPiondR1R = ((tcoinhedR1-TimmingOffset>=-15.0 && tcoinhedR1-TimmingOffset <= -9.0) || (tcoinhedR1-TimmingOffset>=7.0 && tcoinhedR1-TimmingOffset <=13.0));      

      if(CoinPiondR1 && DiamonddR1)
	{
	  MPihedR1->Fill(mmhedR1+MMpiOffsetheR1);
	}
      if(CoinPiondR1 && MMpidR1 && DiamonddR1)
	{
	  hYhedR1->Fill(TMath::Abs(thedR1)+tOffsetheR1, std::fmod(ph_qhedR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedR1->Fill(-thedR1, mmhedR1+MMpiOffsetheR1);
	  hththedR1->Fill(TMath::Abs(thedR1)+tOffsetheR1);
	}
      //Random
      if(CoinPiondR1R && DiamonddR1)
	{
      	  MPihedR1R->Fill(mmhedR1+MMpiOffsetheR1);
	}
      if(CoinPiondR1R && MMpidR1 && DiamonddR1)
	{
	  hYhedR1R->Fill(TMath::Abs(thedR1)+tOffsetheR1, std::fmod(ph_qhedR1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedR1R->Fill(-thedR1, mmhedR1+MMpiOffsetheR1);
	  hththedR1R->Fill(-thedR1+tOffsetheR1);
	}      
    }    

  //SIMC RIGHT1                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCHIGHER1; i++)

    {
      TSIMCHIGHER1->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcheR1, W_simcheR1));

      //      Double_t NFR1 = 35893100.0/400000.0;
      //     Double_t NFR1 = 36034400.0/400000.0;
      //After applying SIMC offsets
       Double_t NFR1 = 35550400.0/400000.0;
      //RadiativeStudy
      //Double_t NFR1 = 38411900.0/400000.0;

      //      if(hsdeltaheR1 >=-8.0 && hsdeltaheR1 <=8.0 && hsxpfpheR1 >=-0.08 && hsxpfpheR1 <=0.08 && hsypfpheR1 >=-0.045 && hsypfpheR1 <=0.045 && ssdeltaheR1 >=-10.0 && ssdeltaheR1 <=20.0 && ssxpfpheR1 >=-0.06 && ssxpfpheR1 <=0.06 && ssypfpheR1 >=-0.04 && ssypfpheR1 <=0.04 && missmassheR1 >= 0.932 && missmassheR1 <= 0.98 && Diamond_cut)

      Double_t CUTSIMCHER1 = hsdeltaheR1 > SIMC_Delta_Low_H && hsdeltaheR1 < SIMC_Delta_High_H && hsxptarheR1 > SIMC_Xptar_Low_H && hsxptarheR1 < SIMC_Xptar_High_H && hsyptarheR1 > SIMC_Yptar_Low_H && hsyptarheR1 < SIMC_Yptar_High_H && ssdeltaheR1 > SIMC_Delta_Low_P && ssdeltaheR1 < SIMC_Delta_High_P && ssxptarheR1 > SIMC_Xptar_Low_P && ssxptarheR1 < SIMC_Xptar_High_P && ssyptarheR1 > SIMC_Yptar_Low_P && ssyptarheR1 < SIMC_Yptar_High_P && missmassheR1 >=MMPICUTL && missmassheR1 <=MMPICUTH && Diamond_cut; 

      Double_t CUTSIMCHER12 = hsdeltaheR1 > SIMC_Delta_Low_H && hsdeltaheR1 < SIMC_Delta_High_H && hsxptarheR1 > SIMC_Xptar_Low_H && hsxptarheR1 < SIMC_Xptar_High_H && hsyptarheR1 > SIMC_Yptar_Low_H && hsyptarheR1 < SIMC_Yptar_High_H && ssdeltaheR1 > SIMC_Delta_Low_P && ssdeltaheR1 < SIMC_Delta_High_P && ssxptarheR1 > SIMC_Xptar_Low_P && ssxptarheR1 < SIMC_Xptar_High_P && ssyptarheR1 > SIMC_Yptar_Low_P && ssyptarheR1 < SIMC_Yptar_High_P && Diamond_cut;
	// hsdeltaheR1 > -8.0 && hsdeltaheR1 < 8.0 && hsxptarheR1 > -0.08 && hsxptarheR1 < 0.08 && hsyptarheR1 > -0.045 && hsyptarheR1 < 0.045 && ssdeltaheR1 > -10.0 && ssdeltaheR1 < 20.0 && ssxptarheR1 > -0.06 && ssxptarheR1 < 0.06 && ssyptarheR1 > -0.04 && ssyptarheR1 < 0.04 && Diamond_cut; 
      
      if(CUTSIMCHER12)	
	{
     	  MPiSheR1->Fill(missmassheR1, NFR1*WeightheR1);
	}
      if(CUTSIMCHER1)	
        {
	  hphheSR1->Fill(phipqheR1*57.2958, NFR1*WeightheR1);	
	  //hphheSR1->Fill(missmassheR1, NFR1*WeightheR1);	
	  hYSheR1->Fill(t_simcheR1,phipqheR1*57.2958, NFR1*WeightheR1);
	  //hYSheR1->Fill(t_simcheR1,missmassheR1, NFR1*WeightheR1);
          H_ssdelta_SheR1->Fill(ssdeltaheR1, NFR1*WeightheR1);
          H_ssxptar_SheR1->Fill(ssxptarheR1, NFR1*WeightheR1);
          H_ssyptar_SheR1->Fill(ssyptarheR1, NFR1*WeightheR1);
	  
          H_hsdelta_SheR1->Fill(hsdeltaheR1, NFR1*WeightheR1);
          H_hsxptar_SheR1->Fill(hsxptarheR1, NFR1*WeightheR1);
          H_hsyptar_SheR1->Fill(hsyptarheR1, NFR1*WeightheR1);
	  hthtSheR1->Fill(t_simcheR1,NFR1*WeightheR1);
  	  
        }
    }

  //Data R2

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHER2; i++)

    {
      TBRANCHHIGHER2->GetEntry(i);
      
      if(tcoinheR2-TimmingOffset>=-1.0 && tcoinheR2-TimmingOffset <= 1.0)    
	{
	  hmmheR2->Fill(mmheR2);	  
	}	      
      
      {
	htcoinheR2->Fill(tcoinheR2-TimmingOffset);
      }

      Double_t DiamondR2 = (Dcut->IsInside(Q2HER2, WHER2));  
      Double_t MMpiR2 = mmheR2>=MMPICUTL-MMpiOffsetheR2 && mmheR2 <=MMPICUTH-MMpiOffsetheR2;
      Double_t CoinPionR2 = tcoinheR2-TimmingOffset>=-1.0 && tcoinheR2-TimmingOffset <= 1.0;      
      Double_t CoinPionR2R = ((tcoinheR2-TimmingOffset>=-15.0 && tcoinheR2-TimmingOffset <= -9.0) || (tcoinheR2-TimmingOffset>=7.0 && tcoinheR2-TimmingOffset <=13.0));      
      
      if(CoinPionR2 && DiamondR2)
	{
	  MPiheR2->Fill(mmheR2+MMpiOffsetheR2);
	}
      if(CoinPionR2 && MMpiR2 && DiamondR2)
	{
          H_ssdelta_DheR2->Fill(P_gtr_dpheR2);
          H_ssxptar_DheR2->Fill(P_gtr_xptarheR2);
          H_ssyptar_DheR2->Fill(P_gtr_yptarheR2);

          H_hsdelta_DheR2->Fill(H_gtr_dpheR2);
          H_hsxptar_DheR2->Fill(H_gtr_xptarheR2);
          H_hsyptar_DheR2->Fill(H_gtr_yptarheR2+0.003);
	  hQ2DheR2->Fill(Q2HER2-0.005); 
	  hWhDeR2->Fill(WHER2+0.003);
	  hephDeR2->Fill(epsilonHER2);
	  
	  hththeR2->Fill(TMath::Abs(theR2)+tOffsetheR2);
	  htheR2->Fill(TMath::Abs(theR2));
	  htheInR2->Fill(TMath::Abs(theR2));
	  hph_qheR2->Fill(ph_qheR2*57.2958 + 180);
	  hphheR2->Fill(std::fmod(ph_qheR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheR2->Fill(mmheR2+MMpiOffsetheR2);
	  hYheR2->Fill(TMath::Abs(theR2)+tOffsetheR2, std::fmod(ph_qheR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheR2->Fill(-theR2, mmheR2+MMpiOffsetheR2);

	  //	  shheR2->Fill(-theR2*sin(ph_qheR2*57.2958), -theR2*cos(ph_qheR2*57.2958));
	  shheR2->Fill(TMath::Abs(theR2)*cos(ph_qheR2), TMath::Abs(theR2)*sin(ph_qheR2));
	}
      //Random
      if(CoinPionR2R && DiamondR2)
	{
	  MPiheR2R->Fill(mmheR2+MMpiOffsetheR2);
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2)
	{
          H_ssdelta_DheR2R->Fill(P_gtr_dpheR2);
          H_ssxptar_DheR2R->Fill(P_gtr_xptarheR2);
          H_ssyptar_DheR2R->Fill(P_gtr_yptarheR2);

          H_hsdelta_DheR2R->Fill(H_gtr_dpheR2);
          H_hsxptar_DheR2R->Fill(H_gtr_xptarheR2);
          H_hsyptar_DheR2R->Fill(H_gtr_yptarheR2+0.003);

	  hQ2DheR2R->Fill(Q2HER2-0.005); 
	  hWhDeR2R->Fill(WHER2+0.003);
	  hephDeR2R->Fill(epsilonHER2);

	  hththeR2R->Fill(TMath::Abs(theR2)+tOffsetheR2);
	  htheR2R->Fill(TMath::Abs(theR2));
	  htheInR2R->Fill(TMath::Abs(theR2));
	  hph_qheR2R->Fill(ph_qheR2*57.2958 + 180);
	  hphheR2R->Fill(std::fmod(ph_qheR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheR2R->Fill(mmheR2+MMpiOffsetheR2);
	  hYheR2R->Fill(TMath::Abs(theR2+tOffsetheR2), std::fmod(ph_qheR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheR2R->Fill(-theR2, mmheR2+MMpiOffsetheR2);
	}
      /*      Double_t w1t = -theR2+tOffsetheR2 >= bins[1] && -theR2+tOffsetheR2 <= bins[2];
      Double_t w2t = -theR2+tOffsetheR2 >= bins[2] && -theR2+tOffsetheR2 <= bins[3];
      Double_t w3t = -theR2+tOffsetheR2 >= bins[3] && -theR2+tOffsetheR2 <= bins[4];
      Double_t w4t = -theR2+tOffsetheR2 >= bins[4] && -theR2+tOffsetheR2 <= bins[5];
      Double_t w5t = -theR2+tOffsetheR2 >= bins[5] && -theR2+tOffsetheR2 <= bins[6];
      Double_t w6t = -theR2+tOffsetheR2 >= bins[6] && -theR2+tOffsetheR2 <= bins[7];
      Double_t w7t = -theR2+tOffsetheR2 >= bins[7] && -theR2+tOffsetheR2 <= bins[8];
      Double_t w8t = -theR2+tOffsetheR2 >= bins[8] && -theR2+tOffsetheR2 <= bins[9];
      */
      Double_t w1t = TMath::Abs(theR2) >= bins[1] && TMath::Abs(theR2) <= bins[2];
      Double_t w2t = TMath::Abs(theR2) >= bins[2] && TMath::Abs(theR2) <= bins[3];
      Double_t w3t = TMath::Abs(theR2) >= bins[3] && TMath::Abs(theR2) <= bins[4];
      Double_t w4t = TMath::Abs(theR2) >= bins[4] && TMath::Abs(theR2) <= bins[5];
      Double_t w5t = TMath::Abs(theR2) >= bins[5] && TMath::Abs(theR2) <= bins[6];
      Double_t w6t = TMath::Abs(theR2) >= bins[6] && TMath::Abs(theR2) <= bins[7];
      Double_t w7t = TMath::Abs(theR2) >= bins[7] && TMath::Abs(theR2) <= bins[8];
      Double_t w8t = TMath::Abs(theR2) >= bins[8] && TMath::Abs(theR2) <= bins[9];

      if(CoinPionR2 && MMpiR2 && DiamondR2 && w1t)
	{
	  hW1heR2->Fill(WHER2);	
	  hQ21heR2->Fill(Q2HER2);	      	  
	  ht1heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w2t)
	{
	  hW2heR2->Fill(WHER2);	
	  hQ22heR2->Fill(Q2HER2);	      	  
	  ht2heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w3t)
	{
	  hW3heR2->Fill(WHER2);	
	  hQ23heR2->Fill(Q2HER2);	      	  
	  ht3heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w4t)
	{
	  hW4heR2->Fill(WHER2);	
	  hQ24heR2->Fill(Q2HER2);	      	  
	  ht4heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w5t)
	{
	  hW5heR2->Fill(WHER2);	
	  hQ25heR2->Fill(Q2HER2);	      	  
	  ht5heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w6t)
	{
	  hW6heR2->Fill(WHER2);	
	  hQ26heR2->Fill(Q2HER2);	      	  
	  ht6heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w7t)
	{
	  hW7heR2->Fill(WHER2);	
	  hQ27heR2->Fill(Q2HER2);	      	  
	  ht7heR2->Fill(TMath::Abs(theR2)); 
 	}	
      if(CoinPionR2 && MMpiR2 && DiamondR2 && w8t)
	{
	  hW8heR2->Fill(WHER2);	
	  hQ28heR2->Fill(Q2HER2);	      	  
	  ht8heR2->Fill(TMath::Abs(theR2)); 
 	}	
      //Random
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w1t)
	
	{
	  hW1heR2R->Fill(WHER2);	
	  hQ21heR2R->Fill(Q2HER2);	      	  
	  ht1heR2R->Fill(TMath::Abs(theR2)); 
	}

      if(CoinPionR2R && MMpiR2 && DiamondR2 && w2t)
	
	{
	  hW2heR2R->Fill(WHER2);	
	  hQ22heR2R->Fill(Q2HER2);	      	  
	  ht2heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w3t)
	
	{
	  hW3heR2R->Fill(WHER2);	
	  hQ23heR2R->Fill(Q2HER2);	      	  
	  ht3heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w4t)
	
	{
	  hW4heR2R->Fill(WHER2);	
	  hQ24heR2R->Fill(Q2HER2);	      	  
	  ht4heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w5t)
	
	{
	  hW5heR2R->Fill(WHER2);	
	  hQ25heR2R->Fill(Q2HER2);	      	  
	  ht5heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w6t)
	
	{
	  hW6heR2R->Fill(WHER2);	
	  hQ26heR2R->Fill(Q2HER2);	      	  
	  ht6heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w7t)
	
	{
	  hW7heR2R->Fill(WHER2);	
	  hQ27heR2R->Fill(Q2HER2);	      	  
	  ht7heR2R->Fill(TMath::Abs(theR2)); 
	}
      if(CoinPionR2R && MMpiR2 && DiamondR2 && w8t)
	
	{
	  hW8heR2R->Fill(WHER2);	
	  hQ28heR2R->Fill(Q2HER2);	      	  
	  ht8heR2R->Fill(TMath::Abs(theR2)); 
	}

      if (((tcoinheR2-TimmingOffset>=-15.0 && tcoinheR2-TimmingOffset <= -9.0) || (tcoinheR2-TimmingOffset>=7.0 && tcoinheR2-TimmingOffset <=13.0)))
	
	{
	  hmmheR2R->Fill(mmheR2);
	}	
    }  
  //Dummy R2
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEDR2; i++)
    {
      TBRANCHHIGHEDR2->GetEntry(i);

      Double_t DiamonddR2 = (Dcut->IsInside(Q2HEDR2, WHEDR2));  
      Double_t MMpidR2 = mmhedR2>=MMPICUTL-MMpiOffsetheR2 && mmhedR2 <=MMPICUTH-MMpiOffsetheR2;
      Double_t CoinPiondR2 = tcoinhedR2-TimmingOffset>=-1.0 && tcoinhedR2-TimmingOffset <= 1.0;      
      Double_t CoinPiondR2R = ((tcoinhedR2-TimmingOffset>=-15.0 && tcoinhedR2-TimmingOffset <= -9.0) || (tcoinhedR2-TimmingOffset>=7.0 && tcoinhedR2-TimmingOffset <=13.0));      

      if(CoinPiondR2 && DiamonddR2)
	{
	  MPihedR2->Fill(mmhedR2+MMpiOffsetheR2);
	}
      if(CoinPiondR2 && MMpidR2 && DiamonddR2)
	{
	  H_ssdelta_DummyheR2->Fill(P_gtr_dphedR2);  
	  H_ssxptar_DummyheR2->Fill(P_gtr_xptarhedR2);  
	  H_ssyptar_DummyheR2->Fill(P_gtr_yptarhedR2);  

	  H_hsdelta_DummyheR2->Fill(H_gtr_dphedR2);
          H_hsxptar_DummyheR2->Fill(H_gtr_xptarhedR2);
          H_hsyptar_DummyheR2->Fill(H_gtr_yptarhedR2+0.003);

	  hQ2DummyheR2->Fill(Q2HEDR2-0.005); 
	  hWhDummyeR2->Fill(WHEDR2+0.003);
	  hephDummyeR2->Fill(epsilonHEDR2);

	  hththedR2->Fill(TMath::Abs(thedR2)+tOffsetheR2);
	  hphhedR2->Fill(std::fmod(ph_qhedR2 * 180.0 / TMath::Pi() + 360.0,360.0));	  

	  hYhedR2->Fill(TMath::Abs(thedR2)+tOffsetheR2, std::fmod(ph_qhedR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedR2->Fill(-thedR2, mmhedR2+MMpiOffsetheR2);
	}
      //Random
      if(CoinPiondR2R && DiamonddR2)
	{
      	  MPihedR2R->Fill(mmhedR2+MMpiOffsetheR2);
	}
      if(CoinPiondR2R && MMpidR2 && DiamonddR2)
	{
	  H_ssdelta_DummyheR2R->Fill(P_gtr_dphedR2);  
	  H_ssxptar_DummyheR2R->Fill(P_gtr_xptarhedR2);  
	  H_ssyptar_DummyheR2R->Fill(P_gtr_yptarhedR2);  

	  H_hsdelta_DummyheR2R->Fill(H_gtr_dphedR2);
          H_hsxptar_DummyheR2R->Fill(H_gtr_xptarhedR2);
          H_hsyptar_DummyheR2R->Fill(H_gtr_yptarhedR2+0.003);

	  hQ2DummyheR2R->Fill(Q2HEDR2-0.005); 
	  hWhDummyeR2R->Fill(WHEDR2+0.003);
	  hephDummyeR2R->Fill(epsilonHEDR2);
	  
	  hththedR2R->Fill(TMath::Abs(thedR2)+tOffsetheR2);
	  hphhedR2R->Fill(std::fmod(ph_qhedR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  hYhedR2R->Fill(TMath::Abs(thedR2)+tOffsetheR2, std::fmod(ph_qhedR2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedR2R->Fill(-thedR2, mmhedR2+MMpiOffsetheR2);
	}      
    }    

  //SIMC RIGHT2                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCHIGHER2; i++)

    {
      TSIMCHIGHER2->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcheR2, W_simcheR2));

      //      Double_t NFR2 = 36049300.0/400000.0;
      //After applying SIMC offsets
       Double_t NFR2 = 35246800.0/400000.0;
      //RadiativeStudy
      //Double_t NFR2 = 38411900.0/400000.0;

      //      if(hsdeltaheR1 >=-8.0 && hsdeltaheR1 <=8.0 && hsxpfpheR1 >=-0.08 && hsxpfpheR1 <=0.08 && hsypfpheR1 >=-0.045 && hsypfpheR1 <=0.045 && ssdeltaheR1 >=-10.0 && ssdeltaheR1 <=20.0 && ssxpfpheR1 >=-0.06 && ssxpfpheR1 <=0.06 && ssypfpheR1 >=-0.04 && ssypfpheR1 <=0.04 && missmassheR1 >= 0.932 && missmassheR1 <= 0.98 && Diamond_cut)

      Double_t CUTSIMCHER2 = hsdeltaheR2 > SIMC_Delta_Low_H && hsdeltaheR2 < SIMC_Delta_High_H && hsxptarheR2 > SIMC_Xptar_Low_H && hsxptarheR2 < SIMC_Xptar_High_H && hsyptarheR2 > SIMC_Yptar_Low_H && hsyptarheR2 < SIMC_Yptar_High_H && ssdeltaheR2 > SIMC_Delta_Low_P && ssdeltaheR2 < SIMC_Delta_High_P && ssxptarheR2 > SIMC_Xptar_Low_P && ssxptarheR2 < SIMC_Xptar_High_P && ssyptarheR2 > SIMC_Yptar_Low_P && ssyptarheR2 < SIMC_Yptar_High_P && missmassheR2 >=MMPICUTL && missmassheR2 <=MMPICUTH && Diamond_cut; 

      Double_t CUTSIMCHER22 = hsdeltaheR2 > SIMC_Delta_Low_H && hsdeltaheR2 < SIMC_Delta_High_H && hsxptarheR2 > SIMC_Xptar_Low_H && hsxptarheR2 < SIMC_Xptar_High_H && hsyptarheR2 > SIMC_Yptar_Low_H && hsyptarheR2 < SIMC_Yptar_High_H && ssdeltaheR2 > SIMC_Delta_Low_P && ssdeltaheR2 < SIMC_Delta_High_P && ssxptarheR2 > SIMC_Xptar_Low_P && ssxptarheR2 < SIMC_Xptar_High_P && ssyptarheR2 > SIMC_Yptar_Low_P && ssyptarheR2 < SIMC_Yptar_High_P && Diamond_cut;
	// hsdeltaheR1 > -8.0 && hsdeltaheR1 < 8.0 && hsxptarheR1 > -0.08 && hsxptarheR1 < 0.08 && hsyptarheR1 > -0.045 && hsyptarheR1 < 0.045 && ssdeltaheR1 > -10.0 && ssdeltaheR1 < 20.0 && ssxptarheR1 > -0.06 && ssxptarheR1 < 0.06 && ssyptarheR1 > -0.04 && ssyptarheR1 < 0.04 && Diamond_cut; 
      
      if(CUTSIMCHER22)	
	{
     	  MPiSheR2->Fill(missmassheR2, NFR2*WeightheR2);
	}
      if(CUTSIMCHER2)	
        {
	  hphheSR2->Fill(phipqheR2*57.2958, NFR2*WeightheR2);	
	  //hphheSR2->Fill(missmassheR2, NFR2*WeightheR2);	
	  hYSheR2->Fill(t_simcheR2,phipqheR2*57.2958, NFR2*WeightheR2);
	  //hYSheR2->Fill(t_simcheR2,missmassheR2, NFR2*WeightheR2);
          H_ssdelta_SheR2->Fill(ssdeltaheR2, NFR2*WeightheR2);
          H_ssxptar_SheR2->Fill(ssxptarheR2, NFR2*WeightheR2);
          H_ssyptar_SheR2->Fill(ssyptarheR2, NFR2*WeightheR2);
	  
          H_hsdelta_SheR2->Fill(hsdeltaheR2, NFR2*WeightheR2);
          H_hsxptar_SheR2->Fill(hsxptarheR2, NFR2*WeightheR2);
          H_hsyptar_SheR2->Fill(hsyptarheR2, NFR2*WeightheR2);
	  hthtSheR2->Fill(t_simcheR2,NFR2*WeightheR2);
  	  
	  hQ2heSR2->Fill(Q2_simcheR2, NFR2*WeightheR2); 
	  hWheSR2->Fill(W_simcheR2, NFR2*WeightheR2);
	  hepheSR2->Fill(epsilon_simcheR2, NFR2*WeightheR2);
        }
    }
  
  //Data C  

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEC; i++)
    
    {
      TBRANCHHIGHEC->GetEntry(i);
      
      if(tcoinheC-TimmingOffset>=-1.0 && tcoinheC-TimmingOffset <= 1.0 && 0.932-MMpiOffsetheC && mmheC <=0.98-MMpiOffsetheC)
	{
	  hQ2WHEC->Fill(Q2HEC, WHEC);
	}
 
      if(tcoinheC-TimmingOffset>=-1.0 && tcoinheC-TimmingOffset <= 1.0)    
 	{
	  hmmheC->Fill(mmheC);	  
	}	      
      
      {
	htcoinheC->Fill(tcoinheC-TimmingOffset);
      }
      
      Double_t DiamondC = (Dcut->IsInside(Q2HEC, WHEC));  
      Double_t MMpiC = mmheC>=MMPICUTL-MMpiOffsetheC && mmheC <=MMPICUTH-MMpiOffsetheC;
      Double_t CoinPionC = tcoinheC-TimmingOffset>=-1.0 && tcoinheC-TimmingOffset <= 1.0;      
      Double_t CoinPionCR = ((tcoinheC-TimmingOffset>=-15.0 && tcoinheC-TimmingOffset <= -9.0) || (tcoinheC-TimmingOffset>=7.0 && tcoinheC-TimmingOffset <=13.0));      

      if(CoinPionC && DiamondC)
	{
	  MPiheC->Fill(mmheC+MMpiOffsetheC);
	}

      if(CoinPionC && MMpiC && DiamondC)
	{
	  H_ssdelta_DheC->Fill(P_gtr_dpheC);
          H_ssxptar_DheC->Fill(P_gtr_xptarheC);
          H_ssyptar_DheC->Fill(P_gtr_yptarheC);

          H_hsdelta_DheC->Fill(H_gtr_dpheC);
          H_hsxptar_DheC->Fill(H_gtr_xptarheC);
          H_hsyptar_DheC->Fill(H_gtr_yptarheC);
	  hQ2WHEC1->Fill(Q2HEC, WHEC);	      	  

	  hththeC->Fill(TMath::Abs(theC)+tOffsetheC);
	  htheC->Fill(TMath::Abs(theC));
	  htheInC->Fill(TMath::Abs(theC));
	  hph_qheC->Fill(ph_qheC*57.2958 + 180);
	  hphheC->Fill(std::fmod(ph_qheC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheC->Fill(mmheC+MMpiOffsetheC);
	  hYheC->Fill(TMath::Abs(theC)+tOffsetheC, std::fmod(ph_qheC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheC->Fill(-theC, mmheC+MMpiOffsetheC);

	  //	  shheC->Fill(-theC*sin(ph_qheC*57.2958), -theC*cos(ph_qheC*57.2958));
	  shheC->Fill(TMath::Abs(theC)*cos(ph_qheC), TMath::Abs(theC)*sin(ph_qheC));

	}	      

      //Random
      if(CoinPionCR && DiamondC)
	{
	  MPiheCR->Fill(mmheC+MMpiOffsetheC);
	}
           
      if(CoinPionCR && MMpiC && DiamondC)
  	{
	  H_ssdelta_DheCR->Fill(P_gtr_dpheC);
          H_ssxptar_DheCR->Fill(P_gtr_xptarheC);
          H_ssyptar_DheCR->Fill(P_gtr_yptarheC);

          H_hsdelta_DheCR->Fill(H_gtr_dpheC);
          H_hsxptar_DheCR->Fill(H_gtr_xptarheC);
          H_hsyptar_DheCR->Fill(H_gtr_yptarheC);

	  hththeCR->Fill(TMath::Abs(theC)+tOffsetheC);
	  htheCR->Fill(TMath::Abs(theC));
	  htheInCR->Fill(TMath::Abs(theC));
	  hph_qheCR->Fill(ph_qheC*57.2958 + 180);
	  hQ2WHECR->Fill(Q2HEC, WHEC);
	  hphheCR->Fill(std::fmod(ph_qheC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheCR->Fill(mmheC+MMpiOffsetheC);
	  hYheCR->Fill(TMath::Abs(theC)+tOffsetheC, std::fmod(ph_qheC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheCR->Fill(-theC, mmheC+MMpiOffsetheC);
	}
      
      /*      Double_t w1t = -theC+tOffsetheC >= bins[1] && -theC+tOffsetheC <= bins[2];
      Double_t w2t = -theC+tOffsetheC >= bins[2] && -theC+tOffsetheC <= bins[3];
      Double_t w3t = -theC+tOffsetheC >= bins[3] && -theC+tOffsetheC <= bins[4];
      Double_t w4t = -theC+tOffsetheC >= bins[4] && -theC+tOffsetheC <= bins[5];
      Double_t w5t = -theC+tOffsetheC >= bins[5] && -theC+tOffsetheC <= bins[6];
      Double_t w6t = -theC+tOffsetheC >= bins[6] && -theC+tOffsetheC <= bins[7];
      Double_t w7t = -theC+tOffsetheC >= bins[7] && -theC+tOffsetheC <= bins[8];
      Double_t w8t = -theC+tOffsetheC >= bins[8] && -theC+tOffsetheC <= bins[9];
      */
      Double_t w1t = TMath::Abs(theC) >= bins[1] && TMath::Abs(theC) <= bins[2];
      Double_t w2t = TMath::Abs(theC) >= bins[2] && TMath::Abs(theC) <= bins[3];
      Double_t w3t = TMath::Abs(theC) >= bins[3] && TMath::Abs(theC) <= bins[4];
      Double_t w4t = TMath::Abs(theC) >= bins[4] && TMath::Abs(theC) <= bins[5];
      Double_t w5t = TMath::Abs(theC) >= bins[5] && TMath::Abs(theC) <= bins[6];
      Double_t w6t = TMath::Abs(theC) >= bins[6] && TMath::Abs(theC) <= bins[7];
      Double_t w7t = TMath::Abs(theC) >= bins[7] && TMath::Abs(theC) <= bins[8];
      Double_t w8t = TMath::Abs(theC) >= bins[8] && TMath::Abs(theC) <= bins[9];

      if(CoinPionC && MMpiC && DiamondC && w1t)
	{
	  hW1heC->Fill(WHEC);	
	  hQ21heC->Fill(Q2HEC);	      	  
	  ht1heC->Fill(TMath::Abs(theC)); 
 	}	

      if(CoinPionC && MMpiC && DiamondC && w2t)
	{
	  hW2heC->Fill(WHEC);	
	  hQ22heC->Fill(Q2HEC);	      	  
	  ht2heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w3t)
	{
	  hW3heC->Fill(WHEC);	
	  hQ23heC->Fill(Q2HEC);	      	  
	  ht3heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w4t)
	{
	  hW4heC->Fill(WHEC);	
	  hQ24heC->Fill(Q2HEC);	      	  
	  ht4heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w5t)
	{
	  hW5heC->Fill(WHEC);	
	  hQ25heC->Fill(Q2HEC);	      	  
	  ht5heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w6t)
	{
	  hW6heC->Fill(WHEC);	
	  hQ26heC->Fill(Q2HEC);	      	  
	  ht6heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w7t)
	{
	  hW7heC->Fill(WHEC);	
	  hQ27heC->Fill(Q2HEC);	      	  
	  ht7heC->Fill(TMath::Abs(theC)); 
 	}	
      if(CoinPionC && MMpiC && DiamondC && w8t)
	{
	  hW8heC->Fill(WHEC);	
	  hQ28heC->Fill(Q2HEC);	      	  
	  ht8heC->Fill(TMath::Abs(theC)); 
 	}	
      //Random
      if(CoinPionCR && MMpiC && DiamondC && w1t)
	
	{
	  hW1heCR->Fill(WHEC);	
	  hQ21heCR->Fill(Q2HEC);	      	  
	  ht1heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w2t)
	
	{
	  hW2heCR->Fill(WHEC);	
	  hQ22heCR->Fill(Q2HEC);	      	  
	  ht2heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w3t)
	
	{
	  hW3heCR->Fill(WHEC);	
	  hQ23heCR->Fill(Q2HEC);	      	  
	  ht3heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w4t)
	
	{
	  hW4heCR->Fill(WHEC);	
	  hQ24heCR->Fill(Q2HEC);	      	  
	  ht4heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w5t)
	
	{
	  hW5heCR->Fill(WHEC);	
	  hQ25heCR->Fill(Q2HEC);	      	  
	  ht5heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w6t)
	
	{
	  hW6heCR->Fill(WHEC);	
	  hQ26heCR->Fill(Q2HEC);	      	  
	  ht6heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w7t)
	
	{
	  hW7heCR->Fill(WHEC);	
	  hQ27heCR->Fill(Q2HEC);	      	  
	  ht7heCR->Fill(TMath::Abs(theC)); 
	}
      if(CoinPionCR && MMpiC && DiamondC && w8t)
	
	{
	  hW8heCR->Fill(WHEC);	
	  hQ28heCR->Fill(Q2HEC);	      	  
	  ht8heCR->Fill(TMath::Abs(theC)); 
	}

      if(((tcoinheC-TimmingOffset>=-15.0 && tcoinheC-TimmingOffset <= -9.0) || (tcoinheC-TimmingOffset>=7.0 && tcoinheC-TimmingOffset <=13.0)))
	
	{
	  hmmheCR->Fill(mmheC);
	}	
    }  
  //Dummy CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEDC; i++)
    {
      TBRANCHHIGHEDC->GetEntry(i);

      Double_t DiamonddC = (Dcut->IsInside(Q2HEDC, WHEDC));  
      Double_t MMpidC = mmhedC>=MMPICUTL-MMpiOffsetheC && mmhedC <=MMPICUTH-MMpiOffsetheC;
      Double_t CoinPiondC = tcoinhedC-TimmingOffset>=-1.0 && tcoinhedC-TimmingOffset <= 1.0;      
      Double_t CoinPiondCR = ((tcoinhedC-TimmingOffset>=-15.0 && tcoinhedC-TimmingOffset <= -9.0) || (tcoinhedC-TimmingOffset>=7.0 && tcoinhedC-TimmingOffset<=13.0));      

      if(CoinPiondC && DiamonddC)
	{
	  MPihedC->Fill(mmhedC+MMpiOffsetheC);
	}
      if(CoinPiondC && MMpidC && DiamonddC)
	{
	  hYhedC->Fill(TMath::Abs(thedC)+tOffsetheC, std::fmod(ph_qhedC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedC->Fill(-thedC, mmhedC+MMpiOffsetheC);
	  hththedC->Fill(TMath::Abs(thedC));

	}

      //Random
      if(CoinPiondCR && DiamonddC)
	{
	  MPihedCR->Fill(mmhedC+MMpiOffsetheC);
	}      
      if(CoinPiondCR && MMpidC && DiamonddC)
	{
	  hYhedCR->Fill(TMath::Abs(thedC)+tOffsetheC, std::fmod(ph_qhedC * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedCR->Fill(-thedC, mmhedC+MMpiOffsetheC);
	  hththedCR->Fill(TMath::Abs(thedC));

	}      
    }    

  //SIMC CENTER                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCHIGHEC; i++)

    {
      TSIMCHIGHEC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcheC, W_simcheC));

      //      Double_t NFC = 35824900.0/400000.0;
      //After applying SIMC offsets
       Double_t NFC = 35781500.0/400000.0;
      //RadiativeStudy
      // Double_t NFC =  38432000.0/400000.0;

      //      if(hsdeltaheC >=-8.0 && hsdeltaheC <=8.0 && hsxpfpheC >=-0.08 && hsxpfpheC <=0.08 && hsypfpheC >=-0.045 && hsypfpheC <=0.045 && ssdeltaheC >=-10.0 && ssdeltaheC <=20.0 && ssxpfpheC >=-0.06 && ssxpfpheC <=0.06 && ssypfpheC >=-0.04 && ssypfpheC <=0.04 && missmassheC >= 0.932 && missmassheC <= 0.98 && Diamond_cut
      
      Double_t CUTSIMCHEC = hsdeltaheC > SIMC_Delta_Low_H && hsdeltaheC < SIMC_Delta_High_H && hsxptarheC > SIMC_Xptar_Low_H && hsxptarheC < SIMC_Xptar_High_H && hsyptarheC > SIMC_Yptar_Low_H && hsyptarheC < SIMC_Yptar_High_H && ssdeltaheC > SIMC_Delta_Low_P && ssdeltaheC < SIMC_Delta_High_P && ssxptarheC > SIMC_Xptar_Low_P && ssxptarheC < SIMC_Xptar_High_P && ssyptarheC > SIMC_Yptar_Low_P && ssyptarheC < SIMC_Yptar_High_P && missmassheC >=MMPICUTL && missmassheC <=MMPICUTH && Diamond_cut;

      Double_t CUTSIMCHEC2 = hsdeltaheC > SIMC_Delta_Low_H && hsdeltaheC < SIMC_Delta_High_H && hsxptarheC > SIMC_Xptar_Low_H && hsxptarheC < SIMC_Xptar_High_H && hsyptarheC > SIMC_Yptar_Low_H && hsyptarheC < SIMC_Yptar_High_H && ssdeltaheC > SIMC_Delta_Low_P && ssdeltaheC < SIMC_Delta_High_P && ssxptarheC > SIMC_Xptar_Low_P && ssxptarheC < SIMC_Xptar_High_P && ssyptarheC > SIMC_Yptar_Low_P && ssyptarheC < SIMC_Yptar_High_P && Diamond_cut;
	//hsdeltaheC > -8.0 && hsdeltaheC < 8.0 && hsxptarheC > -0.08 && hsxptarheC < 0.08 && hsyptarheC > -0.045 && hsyptarheC < 0.045 && ssdeltaheC > -10.0 && ssdeltaheC < 20.0 && ssxptarheC > -0.06 && ssxptarheC < 0.06 && ssyptarheC > -0.04 && ssyptarheC < 0.04 && Diamond_cut;
      
      if (CUTSIMCHEC2)
	{
	  MPiSheC->Fill(missmassheC, NFC*WeightheC);
	}
      if (CUTSIMCHEC)
	{
	  hphheSC->Fill(phipqheC*57.2958, NFC*WeightheC);	
	  //hphheSC->Fill(missmassheC, NFC*WeightheC);	
	  hYSheC->Fill(t_simcheC,phipqheC*57.2958, NFC*WeightheC);
	  //hYSheC->Fill(t_simcheC,missmassheC, NFC*WeightheC);
          H_ssdelta_SheC->Fill(ssdeltaheC, NFC*WeightheC);
          H_ssxptar_SheC->Fill(ssxptarheC, NFC*WeightheC);
          H_ssyptar_SheC->Fill(ssyptarheC, NFC*WeightheC);
	  
          H_hsdelta_SheC->Fill(hsdeltaheC, NFC*WeightheC);
          H_hsxptar_SheC->Fill(hsxptarheC, NFC*WeightheC);
          H_hsyptar_SheC->Fill(hsyptarheC, NFC*WeightheC);
	  hthtSheC->Fill(t_simcheC, NFC*WeightheC);
	  
        }
    }

  //Data L1  

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL1; i++)
  
    {
      TBRANCHHIGHEL1->GetEntry(i);
      
      
      if(tcoinheL1-TimmingOffset>=-1.0 && tcoinheL1-TimmingOffset <= 1.0)    
	{
	  hmmheL1->Fill(mmheL1);
	}           
      
      {
	htcoinheL1->Fill(tcoinheL1-TimmingOffset);
      }

      Double_t DiamondL1 = (Dcut->IsInside(Q2HEL1, WHEL1));  
      Double_t MMpiL1 = mmheL1>=MMPICUTL-MMpiOffsetheL1 && mmheL1 <=MMPICUTH-MMpiOffsetheL1;
      Double_t CoinPionL1 = tcoinheL1-TimmingOffset>=-1.0 && tcoinheL1-TimmingOffset <= 1.0;      
      Double_t CoinPionL1R = ((tcoinheL1-TimmingOffset>=-15.0 && tcoinheL1-TimmingOffset <= -9.0) || (tcoinheL1-TimmingOffset>=7.0 && tcoinheL1-TimmingOffset <=13.0));      

      if(CoinPionL1 && DiamondL1)
	{
	  MPiheL1->Fill(mmheL1+MMpiOffsetheL1);
	}

      if(CoinPionL1 && MMpiL1 && DiamondL1)
	{
	  H_ssdelta_DheL1->Fill(P_gtr_dpheL1);
          H_ssxptar_DheL1->Fill(P_gtr_xptarheL1);
          H_ssyptar_DheL1->Fill(P_gtr_yptarheL1);

          H_hsdelta_DheL1->Fill(H_gtr_dpheL1);
          H_hsxptar_DheL1->Fill(H_gtr_xptarheL1);
          H_hsyptar_DheL1->Fill(H_gtr_yptarheL1);

	  hththeL1->Fill(TMath::Abs(theL1)+tOffsetheL1);
	  htheL1->Fill(TMath::Abs(theL1));
	  htheInL1->Fill(TMath::Abs(theL1));
	  hph_qheL1->Fill(ph_qheL1*57.2958 + 180);	  
	  hphheL1->Fill(std::fmod(ph_qheL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheL1->Fill(mmheL1+MMpiOffsetheL1);
	  hYheL1->Fill(TMath::Abs(theL1)+tOffsetheL1, std::fmod(ph_qheL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheL1->Fill(-theL1, mmheL1+MMpiOffsetheL1);

	  //	  shheL1->Fill(-theL1*sin(ph_qheL1*57.2958), -theL1*cos(ph_qheL1*57.2958));
	  shheL1->Fill(TMath::Abs(theL1)*cos(ph_qheL1), TMath::Abs(theL1)*sin(ph_qheL1));
	}      
      //Random
      if(CoinPionL1R && DiamondL1)
	{
	  MPiheL1R->Fill(mmheL1+MMpiOffsetheL1);
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1)
 	
	{
	  H_ssdelta_DheL1R->Fill(P_gtr_dpheL1);
          H_ssxptar_DheL1R->Fill(P_gtr_xptarheL1);
          H_ssyptar_DheL1R->Fill(P_gtr_yptarheL1);

          H_hsdelta_DheL1R->Fill(H_gtr_dpheL1);
          H_hsxptar_DheL1R->Fill(H_gtr_xptarheL1);
          H_hsyptar_DheL1R->Fill(H_gtr_yptarheL1);

	  hththeL1R->Fill(TMath::Abs(theL1)+tOffsetheL1);
	  htheL1R->Fill(TMath::Abs(theL1));
	  htheInL1R->Fill(TMath::Abs(theL1));
	  hph_qheL1R->Fill(ph_qheL1*57.2958 + 180);	  
	  hphheL1R->Fill(std::fmod(ph_qheL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheL1R->Fill(mmheL1+MMpiOffsetheL1);
	  hYheL1R->Fill(TMath::Abs(theL1)+tOffsetheL1, std::fmod(ph_qheL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheL1R->Fill(-theL1, mmheL1+MMpiOffsetheL1);
	}
      /*      Double_t w1t = -theL1+tOffsetheL1 >= bins[1] && -theL1+tOffsetheL1 <= bins[2];
      Double_t w2t = -theL1+tOffsetheL1 >= bins[2] && -theL1+tOffsetheL1 <= bins[3];
      Double_t w3t = -theL1+tOffsetheL1 >= bins[3] && -theL1+tOffsetheL1 <= bins[4];
      Double_t w4t = -theL1+tOffsetheL1 >= bins[4] && -theL1+tOffsetheL1 <= bins[5];
      Double_t w5t = -theL1+tOffsetheL1 >= bins[5] && -theL1+tOffsetheL1 <= bins[6];
      Double_t w6t = -theL1+tOffsetheL1 >= bins[6] && -theL1+tOffsetheL1 <= bins[7];
      Double_t w7t = -theL1+tOffsetheL1 >= bins[7] && -theL1+tOffsetheL1 <= bins[8];
      Double_t w8t = -theL1+tOffsetheL1 >= bins[8] && -theL1+tOffsetheL1 <= bins[9];
      */
      Double_t w1t = TMath::Abs(theL1) >= bins[1] && TMath::Abs(theL1) <= bins[2];
      Double_t w2t = TMath::Abs(theL1) >= bins[2] && TMath::Abs(theL1) <= bins[3];
      Double_t w3t = TMath::Abs(theL1) >= bins[3] && TMath::Abs(theL1) <= bins[4];
      Double_t w4t = TMath::Abs(theL1) >= bins[4] && TMath::Abs(theL1) <= bins[5];
      Double_t w5t = TMath::Abs(theL1) >= bins[5] && TMath::Abs(theL1) <= bins[6];
      Double_t w6t = TMath::Abs(theL1) >= bins[6] && TMath::Abs(theL1) <= bins[7];
      Double_t w7t = TMath::Abs(theL1) >= bins[7] && TMath::Abs(theL1) <= bins[8];
      Double_t w8t = TMath::Abs(theL1) >= bins[8] && TMath::Abs(theL1) <= bins[9];

      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w1t)
	{
	  hW1heL1->Fill(WHEL1);	
	  hQ21heL1->Fill(Q2HEL1);	      	  
	  ht1heL1->Fill(TMath::Abs(theL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w2t)
	{
	  hW2heL1->Fill(WHEL1);	
	  hQ22heL1->Fill(Q2HEL1);	      	  
	  ht2heL1->Fill(TMath::Abs(theL1)); 
 	}	
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w3t)
	{
	  hW3heL1->Fill(WHEL1);	
	  hQ23heL1->Fill(Q2HEL1);	      	  
	  ht3heL1->Fill(TMath::Abs(theL1)); 
 	}	
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w4t)
	{
	  hW4heL1->Fill(WHEL1);	
	  hQ24heL1->Fill(Q2HEL1);	      	  
	  ht4heL1->Fill(TMath::Abs(theL1)); 
 	}	
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w5t)
	{
	  hW5heL1->Fill(WHEL1);	
	  hQ25heL1->Fill(Q2HEL1);	      	  
	  ht5heL1->Fill(TMath::Abs(theL1)); 
 	}	
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w6t)
	{
	  hW6heL1->Fill(WHEL1);	
	  hQ26heL1->Fill(Q2HEL1);	      	  
	  ht6heL1->Fill(TMath::Abs(theL1)); 
 	}	
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w7t)
	{
	  hW7heL1->Fill(WHEL1);	
	  hQ27heL1->Fill(Q2HEL1);	      	  
	  ht7heL1->Fill(TMath::Abs(theL1)); 
 	}	
      
      if(CoinPionL1 && MMpiL1 && DiamondL1 && w8t)
	{
	  hW8heL1->Fill(WHEL1);	
	  hQ28heL1->Fill(Q2HEL1);	      	  
	  ht8heL1->Fill(TMath::Abs(theL1)); 
 	}
      //Random	
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w1t)
	
	{
	  hW1heL1R->Fill(WHEL1);	
	  hQ21heL1R->Fill(Q2HEL1);	      	  
	  ht1heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w2t)
	
	{
	  hW2heL1R->Fill(WHEL1);	
	  hQ22heL1R->Fill(Q2HEL1);	      	  
	  ht2heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w3t)
	
	{
	  hW3heL1R->Fill(WHEL1);	
	  hQ23heL1R->Fill(Q2HEL1);	      	  
	  ht3heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w4t)
	
	{
	  hW4heL1R->Fill(WHEL1);	
	  hQ24heL1R->Fill(Q2HEL1);	      	  
	  ht4heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w5t)
	
	{
	  hW5heL1R->Fill(WHEL1);	
	  hQ25heL1R->Fill(Q2HEL1);	      	  
	  ht5heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w6t)
	
	{
	  hW6heL1R->Fill(WHEL1);	
	  hQ26heL1R->Fill(Q2HEL1);	      	  
	  ht6heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w7t)
	
	{
	  hW7heL1R->Fill(WHEL1);	
	  hQ27heL1R->Fill(Q2HEL1);	      	  
	  ht7heL1R->Fill(TMath::Abs(theL1)); 
	}
      if(CoinPionL1R && MMpiL1 && DiamondL1 && w8t)
	
	{
	  hW8heL1R->Fill(WHEL1);	
	  hQ28heL1R->Fill(Q2HEL1);	      	  
	  ht8heL1R->Fill(TMath::Abs(theL1)); 
	}
  
      if(((tcoinheL1-TimmingOffset>=-15.0 && tcoinheL1-TimmingOffset <= -9.0) || (tcoinheL1-TimmingOffset>=7.0 && tcoinheL1-TimmingOffset <=13.0)))
	
	{	 
	  hmmheL1R->Fill(mmheL1);
	}     
    }
  //Dummy LEFT1
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEDL1; i++)
    {
      TBRANCHHIGHEDL1->GetEntry(i);

      Double_t DiamonddL1 = (Dcut->IsInside(Q2HEDL1, WHEDL1));  
      Double_t MMpidL1 = mmhedL1>=MMPICUTL-MMpiOffsetheL1 && mmhedL1 <=MMPICUTH-MMpiOffsetheL1;
      Double_t CoinPiondL1 = tcoinhedL1-TimmingOffset>=-1.0 && tcoinhedL1-TimmingOffset <= 1.0;      
      Double_t CoinPiondL1R = ((tcoinhedL1-TimmingOffset>=-15.0 && tcoinhedL1-TimmingOffset <= -9.0) || (tcoinhedL1-TimmingOffset>=7.0 && tcoinhedL1-TimmingOffset<=13.0));      

      if(CoinPiondL1 && DiamonddL1)
	{
	  MPihedL1->Fill(mmhedL1+MMpiOffsetheL1);
	}
      if(CoinPiondL1 && MMpidL1 && DiamonddL1)
	{
	  hYhedL1->Fill(TMath::Abs(thedL1)+tOffsetheL1, std::fmod(ph_qhedL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedL1->Fill(-thedL1, mmhedL1+MMpiOffsetheL1);
	  hththedL1->Fill(TMath::Abs(thedL1)+tOffsetheL1);

	}

      //Random
      if(CoinPiondL1R && DiamonddL1)
	{
	  MPihedL1R->Fill(mmhedL1+MMpiOffsetheL1);
	}
      if(CoinPiondL1R && MMpidL1 && DiamonddL1)
	{
	  hYhedL1R->Fill(TMath::Abs(thedL1)+tOffsetheL1, std::fmod(ph_qhedL1 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedL1R->Fill(-thedL1, mmhedL1+MMpiOffsetheL1);
	  hththedL1R->Fill(TMath::Abs(thedL1)+tOffsetheL1);

	}      
    }    

  //SIMC LEFT1                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCHIGHEL1; i++)

    {
      TSIMCHIGHEL1->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcheL1, W_simcheL1));

      //      Double_t NFL1 = 35489900.0/400000.0;
      //After applying SIMC offsets
       Double_t NFL1 = 35928800.0/400000.0;
      //RadiativeStudy
      //Double_t NFL1 = 38310200.0/400000.0;

      //      if(hsdeltaheL1 >=-8.0 && hsdeltaheL1 <=8.0 && hsxpfpheL1 >=-0.08 && hsxpfpheL1 <=0.08 && hsypfpheL1 >=-0.045 && hsypfpheL1 <=0.045 && ssdeltaheL1 >=-10.0 && ssdeltaheL1 <=20.0 && ssxpfpheL1 >=-0.06 && ssxpfpheL1 <=0.06 && ssypfpheL1 >=-0.04 && ssypfpheL1 <=0.04 && missmassheL1 >= 0.932 && missmassheL1 <= 0.98 && Diamond_cut)
      
      Double_t CUTSIMCHEL1 = hsdeltaheL1 > SIMC_Delta_Low_H && hsdeltaheL1 < SIMC_Delta_High_H && hsxptarheL1 > SIMC_Xptar_Low_H && hsxptarheL1 < SIMC_Xptar_High_H && hsyptarheL1 > SIMC_Yptar_Low_H && hsyptarheL1 < SIMC_Yptar_High_H && ssdeltaheL1 > SIMC_Delta_Low_P && ssdeltaheL1 < SIMC_Delta_High_P && ssxptarheL1 > SIMC_Xptar_Low_P && ssxptarheL1 < SIMC_Xptar_High_P && ssyptarheL1 > SIMC_Yptar_Low_P && ssyptarheL1 < SIMC_Yptar_High_P && missmassheL1 >=MMPICUTL && missmassheL1 <=MMPICUTH && Diamond_cut;	

      Double_t CUTSIMCHEL12 = hsdeltaheL1 > SIMC_Delta_Low_H && hsdeltaheL1 < SIMC_Delta_High_H && hsxptarheL1 > SIMC_Xptar_Low_H && hsxptarheL1 < SIMC_Xptar_High_H && hsyptarheL1 > SIMC_Yptar_Low_H && hsyptarheL1 < SIMC_Yptar_High_H && ssdeltaheL1 > SIMC_Delta_Low_P && ssdeltaheL1 < SIMC_Delta_High_P && ssxptarheL1 > SIMC_Xptar_Low_P && ssxptarheL1 < SIMC_Xptar_High_P && ssyptarheL1 > SIMC_Yptar_Low_P && ssyptarheL1 < SIMC_Yptar_High_P && Diamond_cut;
	//hsdeltaheL1 > -8.0 && hsdeltaheL1 < 8.0 && hsxptarheL1 > -0.08 && hsxptarheL1 < 0.08 && hsyptarheL1 > -0.045 && hsyptarheL1 < 0.045 && ssdeltaheL1 > -10.0 && ssdeltaheL1 < 20.0 && ssxptarheL1 > -0.06 && ssxptarheL1 < 0.06 && ssyptarheL1 > -0.04 && ssyptarheL1 < 0.04 && Diamond_cut;	

      if(CUTSIMCHEL12)
	{
	  MPiSheL1->Fill(missmassheL1, NFL1*WeightheL1);
	}
      if(CUTSIMCHEL1)
        {
	  hphheSL1->Fill(phipqheL1*57.2958, NFL1*WeightheL1);		
	  //hphheSL1->Fill(missmassheL1, NFL1*WeightheL1);		
	  hYSheL1->Fill(t_simcheL1,phipqheL1*57.2958, NFL1*WeightheL1);
	  //hYSheL1->Fill(t_simcheL1,missmassheL1, NFL1*WeightheL1);
          H_ssdelta_SheL1->Fill(ssdeltaheL1, NFL1*WeightheL1);
          H_ssxptar_SheL1->Fill(ssxptarheL1, NFL1*WeightheL1);
          H_ssyptar_SheL1->Fill(ssyptarheL1, NFL1*WeightheL1);
	  
          H_hsdelta_SheL1->Fill(hsdeltaheL1, NFL1*WeightheL1);
          H_hsxptar_SheL1->Fill(hsxptarheL1, NFL1*WeightheL1);
          H_hsyptar_SheL1->Fill(hsyptarheL1, NFL1*WeightheL1);
	  hthtSheL1->Fill(t_simcheL1, NFL1*WeightheL1);
	  
        }
    }
  //Data L2
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL2; i++)
    {
      TBRANCHHIGHEL2->GetEntry(i);
            
      if(tcoinheL2-TimmingOffset>=-1.0 && tcoinheL2-TimmingOffset <= 1.0)
	
	{
	  hmmheL2->Fill(mmheL2);
	}   
      
      {
	htcoinheL2->Fill(tcoinheL2-TimmingOffset);
      }

      Double_t DiamondL2 = (Dcut->IsInside(Q2HEL2, WHEL2));  
      Double_t MMpiL2 = mmheL2>=MMPICUTL-MMpiOffsetheL2 && mmheL2 <=MMPICUTH-MMpiOffsetheL2;
      Double_t CoinPionL2 = tcoinheL2-TimmingOffset>=-1.0 && tcoinheL2-TimmingOffset <= 1.0;      
      Double_t CoinPionL2R = ((tcoinheL2-TimmingOffset>=-15.0 && tcoinheL2-TimmingOffset <= -9.0) || (tcoinheL2-TimmingOffset>=7.0 && tcoinheL2-TimmingOffset <=13.0));      

      if(CoinPionL2 && DiamondL2)
	{
	  MPiheL2->Fill(mmheL2+MMpiOffsetheL2);
	}

      if(CoinPionL2 && MMpiL2 && DiamondL2)
  
	{
	  H_ssdelta_DheL2->Fill(P_gtr_dpheL2);
          H_ssxptar_DheL2->Fill(P_gtr_xptarheL2);
          H_ssyptar_DheL2->Fill(P_gtr_yptarheL2);

          H_hsdelta_DheL2->Fill(H_gtr_dpheL2);
          H_hsxptar_DheL2->Fill(H_gtr_xptarheL2);
          H_hsyptar_DheL2->Fill(H_gtr_yptarheL2);

	  hththeL2->Fill(TMath::Abs(theL2)+tOffsetheL2);
	  htheL2->Fill(TMath::Abs(theL2));
	  htheInL2->Fill(TMath::Abs(theL2));
	  hph_qheL2->Fill(ph_qheL2*57.2958 + 180);
	  hphheL2->Fill(std::fmod(ph_qheL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheL2->Fill(mmheL2+MMpiOffsetheL2);
	  hYheL2->Fill(TMath::Abs(theL2)+tOffsetheL2, std::fmod(ph_qheL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheL2->Fill(-theL2,mmheL2+MMpiOffsetheL2);

	  //	  shheL2->Fill(-theL2*sin(ph_qheL2*57.2958), -theL2*cos(ph_qheL2*57.2958));
	  shheL2->Fill(TMath::Abs(theL2)*cos(ph_qheL2), TMath::Abs(theL2)*sin(ph_qheL2));
	}

      //Random
      if(CoinPionL2R && DiamondL2)
	{
	  MPiheL2R->Fill(mmheL2+MMpiOffsetheL2);
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2)
	{
	  H_ssdelta_DheL2R->Fill(P_gtr_dpheL2);
          H_ssxptar_DheL2R->Fill(P_gtr_xptarheL2);
          H_ssyptar_DheL2R->Fill(P_gtr_yptarheL2);

          H_hsdelta_DheL2R->Fill(H_gtr_dpheL2);
          H_hsxptar_DheL2R->Fill(H_gtr_xptarheL2);
          H_hsyptar_DheL2R->Fill(H_gtr_yptarheL2);

	  hththeL2R->Fill(TMath::Abs(theL2+tOffsetheL2));
	  htheL2R->Fill(TMath::Abs(theL2));
	  htheInL2R->Fill(TMath::Abs(theL2));
	  hph_qheL2R->Fill(ph_qheL2*57.2958 + 180);
	  hphheL2R->Fill(std::fmod(ph_qheL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hphheL2R->Fill(mmheL2+MMpiOffsetheL2);
	  hYheL2R->Fill(TMath::Abs(theL2+tOffsetheL2), std::fmod(ph_qheL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYheL2R->Fill(-theL2,mmheL2+MMpiOffsetheL2);
	}
      /*  Double_t w1t = -theL2+tOffsetheL2 >= bins[1] && -theL2+tOffsetheL2 <= bins[2];
      Double_t w2t = -theL2+tOffsetheL2 >= bins[2] && -theL2+tOffsetheL2 <= bins[3];
      Double_t w3t = -theL2+tOffsetheL2 >= bins[3] && -theL2+tOffsetheL2 <= bins[4];
      Double_t w4t = -theL2+tOffsetheL2 >= bins[4] && -theL2+tOffsetheL2 <= bins[5];
      Double_t w5t = -theL2+tOffsetheL2 >= bins[5] && -theL2+tOffsetheL2 <= bins[6];
      Double_t w6t = -theL2+tOffsetheL2 >= bins[6] && -theL2+tOffsetheL2 <= bins[7];
      Double_t w7t = -theL2+tOffsetheL2 >= bins[7] && -theL2+tOffsetheL2 <= bins[8];
      Double_t w8t = -theL2+tOffsetheL2 >= bins[8] && -theL2+tOffsetheL2 <= bins[9];
      */
      Double_t w1t = TMath::Abs(theL2) >= bins[1] && TMath::Abs(theL2) <= bins[2];
      Double_t w2t = TMath::Abs(theL2) >= bins[2] && TMath::Abs(theL2) <= bins[3];
      Double_t w3t = TMath::Abs(theL2) >= bins[3] && TMath::Abs(theL2) <= bins[4];
      Double_t w4t = TMath::Abs(theL2) >= bins[4] && TMath::Abs(theL2) <= bins[5];
      Double_t w5t = TMath::Abs(theL2) >= bins[5] && TMath::Abs(theL2) <= bins[6];
      Double_t w6t = TMath::Abs(theL2) >= bins[6] && TMath::Abs(theL2) <= bins[7];
      Double_t w7t = TMath::Abs(theL2) >= bins[7] && TMath::Abs(theL2) <= bins[8];
      Double_t w8t = TMath::Abs(theL2) >= bins[8] && TMath::Abs(theL2) <= bins[9];

      
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w1t)
	{
	  hW1heL2->Fill(WHEL2);	
	  hQ21heL2->Fill(Q2HEL2);	      	  
	  ht1heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w2t)
	{
	  hW2heL2->Fill(WHEL2);	
	  hQ22heL2->Fill(Q2HEL2);	      	  
	  ht2heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w3t)
	{
	  hW3heL2->Fill(WHEL2);	
	  hQ23heL2->Fill(Q2HEL2);	      	  
	  ht3heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w4t)
	{
	  hW4heL2->Fill(WHEL2);	
	  hQ24heL2->Fill(Q2HEL2);	      	  
	  ht4heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w5t)
	{
	  hW5heL2->Fill(WHEL2);	
	  hQ25heL2->Fill(Q2HEL2);	      	  
	  ht5heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w6t)
	{
	  hW6heL2->Fill(WHEL2);	
	  hQ26heL2->Fill(Q2HEL2);	      	  
	  ht6heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w7t)
	{
	  hW7heL2->Fill(WHEL2);	
	  hQ27heL2->Fill(Q2HEL2);	      	  
	  ht7heL2->Fill(TMath::Abs(theL2)); 
 	}	
      if(CoinPionL2 && MMpiL2 && DiamondL2 && w8t)
	{
	  hW8heL2->Fill(WHEL2);	
	  hQ28heL2->Fill(Q2HEL2);	      	  
	  ht8heL2->Fill(TMath::Abs(theL2)); 
 	}	
      //Random
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w1t)
	
	{
	  hW1heL2R->Fill(WHEL2);	
	  hQ21heL2R->Fill(Q2HEL2);	      	  
	  ht1heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w2t)
	
	{
	  hW2heL2R->Fill(WHEL2);	
	  hQ22heL2R->Fill(Q2HEL2);	      	  
	  ht2heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w3t)
	
	{
	  hW3heL2R->Fill(WHEL2);	
	  hQ23heL2R->Fill(Q2HEL2);	      	  
	  ht3heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w4t)
	
	{
	  hW4heL2R->Fill(WHEL2);	
	  hQ24heL2R->Fill(Q2HEL2);	      	  
	  ht4heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w5t)
	
	{
	  hW5heL2R->Fill(WHEL2);	
	  hQ25heL2R->Fill(Q2HEL2);	      	  
	  ht5heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w6t)
	
	{
	  hW6heL2R->Fill(WHEL2);	
	  hQ26heL2R->Fill(Q2HEL2);	      	  
	  ht6heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w7t)
	
	{
	  hW7heL2R->Fill(WHEL2);	
	  hQ27heL2R->Fill(Q2HEL2);	      	  
	  ht7heL2R->Fill(TMath::Abs(theL2)); 
	}
      if(CoinPionL2R && MMpiL2 && DiamondL2 && w8t)
	
	{
	  hW8heL2R->Fill(WHEL2);	
	  hQ28heL2R->Fill(Q2HEL2);	      	  
	  ht8heL2R->Fill(TMath::Abs(theL2)); 
	}

      if(((tcoinheL2-TimmingOffset>=-15.0 && tcoinheL2-TimmingOffset <= -9.0) || (tcoinheL2-TimmingOffset>=7.0 && tcoinheL2-TimmingOffset <=13.0)))
	{
	  hmmheL2R->Fill(mmheL2);
	}
    }
  //Dummy LEFT2
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEDL2; i++)
    {
      TBRANCHHIGHEDL2->GetEntry(i);

      Double_t DiamonddL2 = (Dcut->IsInside(Q2HEDL2, WHEDL2));  
      Double_t MMpidL2 = mmhedL2>=MMPICUTL-MMpiOffsetheL2 && mmhedL2 <=MMPICUTH-MMpiOffsetheL2;
      Double_t CoinPiondL2 = tcoinhedL2-TimmingOffset>=-1.0 && tcoinhedL2-TimmingOffset <= 1.0;      
      Double_t CoinPiondL2R = ((tcoinhedL2-TimmingOffset>=-15.0 && tcoinhedL2-TimmingOffset <= -9.0) || (tcoinhedL2-TimmingOffset>=7.0 && tcoinhedL2-TimmingOffset<=13.0));      

      if(CoinPiondL2 && DiamonddL2)
	{
	  MPihedL2->Fill(mmhedL2+MMpiOffsetheL2);
	}
      if(CoinPiondL2 && MMpidL2 && DiamonddL2)
	{
	  hYhedL2->Fill(TMath::Abs(thedL2)+tOffsetheL2, std::fmod(ph_qhedL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedL2->Fill(-thedL2, mmhedL2+MMpiOffsetheL2);
	  hththedL2->Fill(TMath::Abs(thedL2)+tOffsetheL2);

	}
      
      //Random
      if(CoinPiondL2R && MMpidL2 && DiamonddL2)
	{
	  MPihedL2R->Fill(mmhedL2+MMpiOffsetheL2);
	}
      if(CoinPiondL2R && MMpidL2 && DiamonddL2)
	{
	  hYhedL2R->Fill(TMath::Abs(thedL2)+tOffsetheL2, std::fmod(ph_qhedL2 * 180.0 / TMath::Pi() + 360.0,360.0));
	  //hYhedL2R->Fill(-thedL2, mmhedL2+MMpiOffsetheL2);
	  hththedL2R->Fill(TMath::Abs(thedL2)+tOffsetheL2);
	}      
    }    

  //SIMC LEFT2                                                                                                                                                                                      
  for(Long64_t i = 0; i < nEntries_TSIMCHIGHEL2; i++)

    {
      TSIMCHIGHEL2->GetEntry(i);
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2_simcheL2, W_simcheL2));
      
      // Double_t NFL2 = 35811900.0/400000.0;
      //After applying SIMC offsets
       Double_t NFL2 = 36073200.0/400000.0;
      //RadiativeStudy
      //Double_t NFL2 = 38073700.0/400000.0;
      
      //      if(hsdeltaheL2 >=-8.0 && hsdeltaheL2 <=8.0 && hsxpfpheL2 >=-0.08 && hsxpfpheL2 <=0.08 && hsypfpheL2 >=-0.045 && hsypfpheL2 <=0.045 && ssdeltaheL2 >=-10.0 && ssdeltaheL2 <=20.0 && ssxpfpheL2 >=-0.06 && ssxpfpheL2 <=0.06 && ssypfpheL2 >=-0.04 && ssypfpheL2 <=0.04 && missmassheL2 >= 0.932 && missmassheL2 <= 0.98 && Diamond_cut)
      
      Double_t CUTSIMCHEL2 = hsdeltaheL2 > SIMC_Delta_Low_H && hsdeltaheL2 < SIMC_Delta_High_H && hsxptarheL2 > SIMC_Xptar_Low_H && hsxptarheL2 < SIMC_Xptar_High_H && hsyptarheL2 > SIMC_Yptar_Low_H && hsyptarheL2 < SIMC_Yptar_High_H && ssdeltaheL2 > SIMC_Delta_Low_P && ssdeltaheL2 < SIMC_Delta_High_P && ssxptarheL2 > SIMC_Xptar_Low_P && ssxptarheL2 < SIMC_Xptar_High_P && ssyptarheL2 > SIMC_Yptar_Low_P && ssyptarheL2 < SIMC_Yptar_High_P && missmassheL2 >=MMPICUTL && missmassheL2 <=MMPICUTH && Diamond_cut;	

      Double_t CUTSIMCHEL22 = hsdeltaheL2 > SIMC_Delta_Low_H && hsdeltaheL2 < SIMC_Delta_High_H && hsxptarheL2 > SIMC_Xptar_Low_H && hsxptarheL2 < SIMC_Xptar_High_H && hsyptarheL2 > SIMC_Yptar_Low_H && hsyptarheL2 < SIMC_Yptar_High_H && ssdeltaheL2 > SIMC_Delta_Low_P && ssdeltaheL2 < SIMC_Delta_High_P && ssxptarheL2 > SIMC_Xptar_Low_P && ssxptarheL2 < SIMC_Xptar_High_P && ssyptarheL2 > SIMC_Yptar_Low_P && ssyptarheL2 < SIMC_Yptar_High_P && Diamond_cut;
	// hsdeltaheL2 > -8.0 && hsdeltaheL2 < 8.0 && hsxptarheL2 > -0.08 && hsxptarheL2 < 0.08 && hsyptarheL2 > -0.045 && hsyptarheL2 < 0.045 && ssdeltaheL2 > -10.0 && ssdeltaheL2 < 20.0 && ssxptarheL2 > -0.06 && ssxptarheL2 < 0.06 && ssyptarheL2 >=-0.04 && ssyptarheL2 <=0.04 && Diamond_cut;	
      
      if(CUTSIMCHEL22)       
	{
	  MPiSheL2->Fill(missmassheL2, NFL2*WeightheL2);
	}
      if(CUTSIMCHEL2)       
	{
	  hphheSL2->Fill(phipqheL2*57.2958, NFL2*WeightheL2);			
	  //hphheSL2->Fill(missmassheL2, NFL2*WeightheL2);			
	  hYSheL2->Fill(t_simcheL2,phipqheL2*57.2958, NFL2*WeightheL2);
	  //hYSheL2->Fill(t_simcheL2,missmassheL2, NFL2*WeightheL2);
	  H_ssdelta_SheL2->Fill(ssdeltaheL2, NFL2*WeightheL2);
	  H_ssxptar_SheL2->Fill(ssxptarheL2, NFL2*WeightheL2);
          H_ssyptar_SheL2->Fill(ssyptarheL2, NFL2*WeightheL2);
	  
          H_hsdelta_SheL2->Fill(hsdeltaheL2, NFL2*WeightheL2);
          H_hsxptar_SheL2->Fill(hsxptarheL2, NFL2*WeightheL2);
          H_hsyptar_SheL2->Fill(hsyptarheL2, NFL2*WeightheL2);
	  hthtSheL2->Fill(t_simcheL2, NFL2*WeightheL2);
	  
        }
    }
  MPiheR1R->Scale(RTSCALE);
  MPiheR2R->Scale(RTSCALE);
  MPiheCR->Scale(RTSCALE);
  MPiheL1R->Scale(RTSCALE);
  MPiheL2R->Scale(RTSCALE);
  //dummy
  MPihedR1R->Scale(RTSCALE);
  MPihedR2R->Scale(RTSCALE);
  MPihedCR->Scale(RTSCALE);
  MPihedL1R->Scale(RTSCALE);
  MPihedL2R->Scale(RTSCALE);

  hththeR1R->Scale(RTSCALE);
  hththeR2R->Scale(RTSCALE);
  hththeCR->Scale(RTSCALE);
  hththeL1R->Scale(RTSCALE);
  hththeL2R->Scale(RTSCALE);
  //dummy
  hththedR1R->Scale(RTSCALE);
  hththedR2R->Scale(RTSCALE);
  hphhedR2R->Scale(RTSCALE);
  hththedCR->Scale(RTSCALE);
  hththedL1R->Scale(RTSCALE);
  hththedL2R->Scale(RTSCALE);

  //Yield
  hYheR1R->Scale(RTSCALE);
  hYheR2R->Scale(RTSCALE);
  hYheCR->Scale(RTSCALE);
  hYheL1R->Scale(RTSCALE);
  hYheL2R->Scale(RTSCALE);
  //Yield Dummy
  hYhedR1R->Scale(RTSCALE);
  hYhedR2R->Scale(RTSCALE);
  hYhedCR->Scale(RTSCALE);
  hYhedL1R->Scale(RTSCALE);
  hYhedL2R->Scale(RTSCALE);

  hphheR1R->Scale(RTSCALE);
  hphheR2R->Scale(RTSCALE);
  hphheCR->Scale(RTSCALE);
  hphheL1R->Scale(RTSCALE);
  hphheL2R->Scale(RTSCALE);
  //SHMS
  H_ssdelta_DheR1R->Scale(RTSCALE);
  H_ssxptar_DheR1R->Scale(RTSCALE);
  H_ssyptar_DheR1R->Scale(RTSCALE);

  H_ssdelta_DheR2R->Scale(RTSCALE);
  H_ssxptar_DheR2R->Scale(RTSCALE);
  H_ssyptar_DheR2R->Scale(RTSCALE);

  hQ2DheR2R->Scale(RTSCALE); 
  hQ2DummyheR2R->Scale(RTSCALE); 
  hWhDeR2R->Scale(RTSCALE);
  hWhDummyeR2R->Scale(RTSCALE);
  hephDeR2R->Scale(RTSCALE);
  hephDummyeR2R->Scale(RTSCALE);
    
  H_ssdelta_DummyheR2R->Scale(RTSCALE);
  H_ssxptar_DummyheR2R->Scale(RTSCALE);
  H_ssyptar_DummyheR2R->Scale(RTSCALE);
  
  H_ssdelta_DheCR->Scale(RTSCALE);
  H_ssxptar_DheCR->Scale(RTSCALE);
  H_ssyptar_DheCR->Scale(RTSCALE);

  H_ssdelta_DheL1R->Scale(RTSCALE);
  H_ssxptar_DheL1R->Scale(RTSCALE);
  H_ssyptar_DheL1R->Scale(RTSCALE);

  H_ssdelta_DheL2R->Scale(RTSCALE);
  H_ssxptar_DheL2R->Scale(RTSCALE);
  H_ssyptar_DheL2R->Scale(RTSCALE);

  //HMS

  H_hsdelta_DheR1R->Scale(RTSCALE);
  H_hsxptar_DheR1R->Scale(RTSCALE);
  H_hsyptar_DheR1R->Scale(RTSCALE);

  H_hsdelta_DheR2R->Scale(RTSCALE);
  H_hsxptar_DheR2R->Scale(RTSCALE);
  H_hsyptar_DheR2R->Scale(RTSCALE);
  
  H_hsdelta_DummyheR2R->Scale(RTSCALE);
  H_hsxptar_DummyheR2R->Scale(RTSCALE);
  H_hsyptar_DummyheR2R->Scale(RTSCALE);

  H_hsdelta_DheCR->Scale(RTSCALE);
  H_hsxptar_DheCR->Scale(RTSCALE);
  H_hsyptar_DheCR->Scale(RTSCALE);

  H_hsdelta_DheL1R->Scale(RTSCALE);
  H_hsxptar_DheL1R->Scale(RTSCALE);
  H_hsyptar_DheL1R->Scale(RTSCALE);

  H_hsdelta_DheL2R->Scale(RTSCALE);
  H_hsxptar_DheL2R->Scale(RTSCALE);
  H_hsyptar_DheL2R->Scale(RTSCALE);
  
  hQ2WHECR->Scale(1.0/6.0);

  hW1heR1R->Scale(1.0/6.0);
  hW2heR1R->Scale(1.0/6.0);
  hW3heR1R->Scale(1.0/6.0);
  hW4heR1R->Scale(1.0/6.0);
  hW5heR1R->Scale(1.0/6.0);
  hW6heR1R->Scale(1.0/6.0);
  hW7heR1R->Scale(1.0/6.0);
  hW8heR1R->Scale(1.0/6.0);

  hW1heR2R->Scale(1.0/6.0);
  hW2heR2R->Scale(1.0/6.0);
  hW3heR2R->Scale(1.0/6.0);
  hW4heR2R->Scale(1.0/6.0);
  hW5heR2R->Scale(1.0/6.0);
  hW6heR2R->Scale(1.0/6.0);
  hW7heR2R->Scale(1.0/6.0);
  hW8heR2R->Scale(1.0/6.0);
  
  hW1heCR->Scale(1.0/6.0);
  hW2heCR->Scale(1.0/6.0);
  hW3heCR->Scale(1.0/6.0);
  hW4heCR->Scale(1.0/6.0);
  hW5heCR->Scale(1.0/6.0);
  hW6heCR->Scale(1.0/6.0);
  hW7heCR->Scale(1.0/6.0);
  hW8heCR->Scale(1.0/6.0);

  hW1heL1R->Scale(1.0/6.0);
  hW2heL1R->Scale(1.0/6.0);
  hW3heL1R->Scale(1.0/6.0);
  hW4heL1R->Scale(1.0/6.0);
  hW5heL1R->Scale(1.0/6.0);
  hW6heL1R->Scale(1.0/6.0);
  hW7heL1R->Scale(1.0/6.0);
  hW8heL1R->Scale(1.0/6.0);

  hW1heL2R->Scale(1.0/6.0);
  hW2heL2R->Scale(1.0/6.0);
  hW3heL2R->Scale(1.0/6.0);
  hW4heL2R->Scale(1.0/6.0);
  hW5heL2R->Scale(1.0/6.0);
  hW6heL2R->Scale(1.0/6.0);
  hW7heL2R->Scale(1.0/6.0);
  hW8heL2R->Scale(1.0/6.0);
	
  hQ21heR1R->Scale(1.0/6.0);
  hQ22heR1R->Scale(1.0/6.0);
  hQ23heR1R->Scale(1.0/6.0);
  hQ24heR1R->Scale(1.0/6.0);
  hQ25heR1R->Scale(1.0/6.0);
  hQ26heR1R->Scale(1.0/6.0);
  hQ27heR1R->Scale(1.0/6.0);
  hQ28heR1R->Scale(1.0/6.0);

  hQ21heR2R->Scale(1.0/6.0);
  hQ22heR2R->Scale(1.0/6.0);
  hQ23heR2R->Scale(1.0/6.0);
  hQ24heR2R->Scale(1.0/6.0);
  hQ25heR2R->Scale(1.0/6.0);
  hQ26heR2R->Scale(1.0/6.0);
  hQ27heR2R->Scale(1.0/6.0);
  hQ28heR2R->Scale(1.0/6.0);
  
  hQ21heCR->Scale(1.0/6.0);
  hQ22heCR->Scale(1.0/6.0);
  hQ23heCR->Scale(1.0/6.0);
  hQ24heCR->Scale(1.0/6.0);
  hQ25heCR->Scale(1.0/6.0);
  hQ26heCR->Scale(1.0/6.0);
  hQ27heCR->Scale(1.0/6.0);
  hQ28heCR->Scale(1.0/6.0);

  hQ21heL1R->Scale(1.0/6.0);
  hQ22heL1R->Scale(1.0/6.0);
  hQ23heL1R->Scale(1.0/6.0);
  hQ24heL1R->Scale(1.0/6.0);
  hQ25heL1R->Scale(1.0/6.0);
  hQ26heL1R->Scale(1.0/6.0);
  hQ27heL1R->Scale(1.0/6.0);
  hQ28heL1R->Scale(1.0/6.0);

  hQ21heL2R->Scale(1.0/6.0);
  hQ22heL2R->Scale(1.0/6.0);
  hQ23heL2R->Scale(1.0/6.0);
  hQ24heL2R->Scale(1.0/6.0);
  hQ25heL2R->Scale(1.0/6.0);
  hQ26heL2R->Scale(1.0/6.0);
  hQ27heL2R->Scale(1.0/6.0);
  hQ28heL2R->Scale(1.0/6.0);
	      	  
  ht1heR1R->Scale(1.0/6.0);
  ht2heR1R->Scale(1.0/6.0);
  ht3heR1R->Scale(1.0/6.0);
  ht4heR1R->Scale(1.0/6.0);
  ht5heR1R->Scale(1.0/6.0);
  ht6heR1R->Scale(1.0/6.0);
  ht7heR1R->Scale(1.0/6.0);
  ht8heR1R->Scale(1.0/6.0);

  ht1heR2R->Scale(1.0/6.0);
  ht2heR2R->Scale(1.0/6.0);
  ht3heR2R->Scale(1.0/6.0);
  ht4heR2R->Scale(1.0/6.0);
  ht5heR2R->Scale(1.0/6.0);
  ht6heR2R->Scale(1.0/6.0);
  ht7heR2R->Scale(1.0/6.0);
  ht8heR2R->Scale(1.0/6.0);

  ht1heCR->Scale(1.0/6.0);
  ht2heCR->Scale(1.0/6.0);
  ht3heCR->Scale(1.0/6.0);
  ht4heCR->Scale(1.0/6.0);
  ht5heCR->Scale(1.0/6.0);
  ht6heCR->Scale(1.0/6.0);
  ht7heCR->Scale(1.0/6.0);
  ht8heCR->Scale(1.0/6.0);

  ht1heL1R->Scale(1.0/6.0);
  ht2heL1R->Scale(1.0/6.0);
  ht3heL1R->Scale(1.0/6.0);
  ht4heL1R->Scale(1.0/6.0);
  ht5heL1R->Scale(1.0/6.0);
  ht6heL1R->Scale(1.0/6.0);
  ht7heL1R->Scale(1.0/6.0);
  ht8heL1R->Scale(1.0/6.0);

  ht1heL2R->Scale(1.0/6.0);
  ht2heL2R->Scale(1.0/6.0);
  ht3heL2R->Scale(1.0/6.0);
  ht4heL2R->Scale(1.0/6.0);
  ht5heL2R->Scale(1.0/6.0);
  ht6heL2R->Scale(1.0/6.0);
  ht7heL2R->Scale(1.0/6.0);
  ht8heL2R->Scale(1.0/6.0);

  hmmheR1R->Scale(1.0/6.0);
  hmmheR2R->Scale(1.0/6.0);
  hmmheCR->Scale(1.0/6.0);
  hmmheL1R->Scale(1.0/6.0);
  hmmheL2R->Scale(1.0/6.0);

  htheR1R->Scale(1.0/6.0);
  htheR2R->Scale(1.0/6.0);
  htheCR->Scale(1.0/6.0);
  htheL1R->Scale(1.0/6.0);
  htheL2R->Scale(1.0/6.0);

  htheInR1R->Scale(1.0/6.0);
  htheInR2R->Scale(1.0/6.0);
  htheInCR->Scale(1.0/6.0);
  htheInL1R->Scale(1.0/6.0);
  htheInL2R->Scale(1.0/6.0);

  hph_qheR1R->Scale(1.0/6.0);
  hph_qheR2R->Scale(1.0/6.0);
  hph_qheCR->Scale(1.0/6.0);
  hph_qheL1R->Scale(1.0/6.0);
  hph_qheL2R->Scale(1.0/6.0);

  //Effective charge  
  std::ifstream inputFile13("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2right1_highe.csv");
  double effcharheR1;
  inputFile13 >> effcharheR1;
  inputFile13.close();
  std::ifstream inputFile14("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2right1_highe_dummy.csv");
  double effcharheR1D;
  inputFile14 >> effcharheR1D;
  inputFile14.close();
  
  Double_t ECHER1   = (1.0+0.0946832)/effcharheR1;
  Double_t ECHEDR1  = (1.0+0.0946832)/effcharheR1D;
  
  /*
  Double_t ECHER1   = (1.0)/effcharheR1;
  Double_t ECHEDR1  = (1.0)/effcharheR1D;
  */
 
  std::ifstream inputFile15("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2right2_highe.csv");
  double effcharheR2;
  inputFile15 >> effcharheR2;
  inputFile15.close();
  std::ifstream inputFile16("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2right2_highe_dummy.csv");
  double effcharheR2D;
  inputFile16 >> effcharheR2D;
  inputFile16.close();
  
  Double_t ECHER2   = (1.0+0.0501988)/effcharheR2;
  Double_t ECHEDR2  = (1.0+0.0501988)/effcharheR2D;
    
  /*
  Double_t ECHER2   = (1.0)/effcharheR2;
  Double_t ECHEDR2  = (1.0)/effcharheR2D;
  */
 
  std::ifstream inputFile17("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_highe.csv");
  double effcharheC;
  inputFile17 >> effcharheC;
  inputFile17.close();
  std::ifstream inputFile18("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2center_highe_dummy.csv");
  double effcharheCD;
  inputFile18 >> effcharheCD;
  inputFile18.close();
  
  Double_t ECHEC    = (1.0+0.0635043)/effcharheC;
  Double_t ECHEDC   = (1.0+0.0635043)/effcharheCD;
  
  /*
  Double_t ECHEC    = (1.0)/effcharheC;
  Double_t ECHEDC   = (1.0)/effcharheCD;
  */
  std::ifstream inputFile19("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_highe.csv");
  double effcharheL1;
  inputFile19 >> effcharheL1;
  inputFile19.close();
  std::ifstream inputFile20("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left1_highe_dummy.csv");
  double effcharheL1D;
  inputFile20 >> effcharheL1D;
  inputFile20.close();
  
  Double_t ECHEL1   = (1.0+0.0162015)/effcharheL1;
  Double_t ECHEDL1  = (1.0+0.0162015)/effcharheL1D;
  /* 
  Double_t ECHEL1   = (1.0)/effcharheL1;
  Double_t ECHEDL1  = (1.0)/effcharheL1D;
  */
  std::ifstream inputFile21("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_highe.csv");
  double effcharheL2;
  inputFile21 >> effcharheL2;
  inputFile21.close();
  std::ifstream inputFile22("/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/EffCharge_Q0p425W2p2left2_highe_dummy.csv");
  double effcharheL2D;
  inputFile22 >> effcharheL2D;
  inputFile22.close();
  
  Double_t ECHEL2   = (1.0-0.0181955)/effcharheL2;
  Double_t ECHEDL2  = (1.0-0.0181955)/effcharheL2D;
   
  /*
  Double_t ECHEL2   = (1.0)/effcharheL2;
  Double_t ECHEDL2  = (1.0)/effcharheL2D;
  */
  //SIMC SCAL
  Double_t SCFHER1 = 1.0; // 0.85;
  Double_t SCFHER2 = 1.0; // 0.85;
  Double_t SCFHEC =  1.0; //0.55;
  Double_t SCFHEL1 = 1.0; //0.55;
  Double_t SCFHEL2 = 1.0; //0.55;

  //////////////////////////////////////////
  TCanvas *cthesiMPishe = new TCanvas("cthesiMPishe", "cthesiMPishe");
  cthesiMPishe->Divide(3,2);
  cthesiMPishe->cd(1);
  MPiheR2->Add(MPiheR2R, -1);
  MPiheR2->Scale(ECHER2);
  MPihedR2->Add(MPihedR2R, -1);
  MPihedR2->Scale(ECHEDR2);
  MPiheR2->Add(MPihedR2, -1);
  //  MPiSheR2->SetStats(0);
  MPiSheR2->SetLineColor(kRed);
  MPiSheR2->Draw("hist");
  //  MPiheR2->SetStats(0);
  MPiheR2->Draw("same");

  cthesiMPishe->cd(2);
  MPiheR1->Add(MPiheR1R, -1);
  MPiheR1->Scale(ECHER1);
  MPihedR1->Add(MPihedR1R, -1);
  MPihedR1->Scale(ECHEDR1);
  MPiheR1->Add(MPihedR1, -1);
  //  MPiSheR1->SetStats(0);
  MPiSheR1->SetLineColor(kRed);
  MPiSheR1->Draw("hist");
  //  MPiheR1->SetStats(0);
  MPiheR1->Draw("same");

  cthesiMPishe->cd(3);
  MPiheC->Add(MPiheCR, -1);
  MPiheC->Scale(ECHEC);
  MPihedC->Add(MPihedCR, -1);
  MPihedC->Scale(ECHEDC);
  MPiheC->Add(MPihedC, -1);
  //  MPiSheC->SetStats(0);
  MPiSheC->SetLineColor(kRed);
  MPiSheC->Draw("hist");
  //  MPiheC->SetStats(0);
  MPiheC->Draw("same");

  cthesiMPishe->cd(4);
  MPiheL1->Add(MPiheL1R, -1);
  MPiheL1->Scale(ECHEL1);
  MPihedL1->Add(MPihedL1R, -1);
  MPihedL1->Scale(ECHEDL1);
  MPiheL1->Add(MPihedL1, -1);
  //  MPiSheL1->SetStats(0);
  MPiSheL1->SetLineColor(kRed);
  MPiSheL1->Draw("hist");
  //  MPiheL1->SetStats(0);
  MPiheL1->Draw("same");

  cthesiMPishe->cd(5);
  MPiheL2->Add(MPiheL2R, -1);
  MPiheL2->Scale(ECHEL2);
  MPihedL2->Add(MPihedL2R, -1);
  MPihedL2->Scale(ECHEDL2);
  MPiheL2->Add(MPihedL2, -1);
  //  MPiSheL2->SetStats(0);
  MPiSheL2->SetLineColor(kRed);
  MPiSheL2->Draw("hist");
  //  MPiheL2->SetStats(0);
  MPiheL2->Draw("same");
  cthesiMPishe->Print(outputpdf);

  TAxis *XPiheR1 = MPiheR1->GetXaxis();
  Double_t InPiheR1 = MPiheR1->Integral(XPiheR1->FindBin(0.91), XPiheR1->FindBin(0.98), "");
  TAxis *XPiSheR1 = MPiSheR1->GetXaxis();
  Double_t InPiSheR1 = MPiSheR1->Integral(XPiSheR1->FindBin(0.91), XPiSheR1->FindBin(0.98), "");
  Double_t PiheR1 = (1-InPiheR1/InPiSheR1); 
  cout << "PiheR1 =  " << PiheR1 << endl;
  
  TAxis *XPiheR2 = MPiheR2->GetXaxis();
  Double_t InPiheR2 = MPiheR2->Integral(XPiheR2->FindBin(0.91), XPiheR2->FindBin(0.98), "");
  TAxis *XPiSheR2 = MPiSheR2->GetXaxis();
  Double_t InPiSheR2 = MPiSheR2->Integral(XPiSheR2->FindBin(0.91), XPiSheR2->FindBin(0.98), "");
  Double_t PiheR2 = (1-InPiheR2/InPiSheR2); 
  cout << "PiheR2 =  " << PiheR2 << endl;
  
  TAxis *XPiheC = MPiheC->GetXaxis();
  Double_t InPiheC = MPiheC->Integral(XPiheC->FindBin(0.91), XPiheC->FindBin(0.98), "");
  TAxis *XPiSheC = MPiSheC->GetXaxis();
  Double_t InPiSheC = MPiSheC->Integral(XPiSheC->FindBin(0.91), XPiSheC->FindBin(0.98), "");
  Double_t PiheC = (1-InPiheC/InPiSheC); 
  cout << "PiheC =  " << PiheC << endl;
  
  TAxis *XPiheL1 = MPiheL1->GetXaxis();
  Double_t InPiheL1 = MPiheL1->Integral(XPiheL1->FindBin(0.91), XPiheL1->FindBin(0.98), "");
  TAxis *XPiSheL1 = MPiSheL1->GetXaxis();
  Double_t InPiSheL1 = MPiSheL1->Integral(XPiSheL1->FindBin(0.91), XPiSheL1->FindBin(0.98), "");
  Double_t PiheL1 = (1-InPiheL1/InPiSheL1);
  cout << "PiheL1 =  " << PiheL1 << endl;

  TAxis *XPiheL2 = MPiheL2->GetXaxis();
  Double_t InPiheL2 = MPiheL2->Integral(XPiheL2->FindBin(0.91), XPiheL2->FindBin(0.98), "");
  TAxis *XPiSheL2 = MPiSheL2->GetXaxis();
  Double_t InPiSheL2 = MPiSheL2->Integral(XPiSheL2->FindBin(0.91), XPiSheL2->FindBin(0.98), "");
  Double_t PiheL2 = (1-InPiheL2/InPiSheL2); 
  cout << "PiheL2 =  " << PiheL2 << endl;
    
  TCanvas *cthesishe = new TCanvas("cthesishe", "cthesishe");
  cthesishe->Divide(3,2);
  cthesishe->cd(1);
  
  cthesishe->cd(2);
  hththeR1->Add(hththeR1R, -1);
  hththeR1->Scale(ECHER1);
  hththedR1->Add(hththedR1R, -1);
  hththedR1->Scale(ECHEDR1);
  hththeR1->Add(hththedR1, -1);
  // hthtSheR1->SetStats(0);
  hthtSheR1->SetLineColor(kRed);
  hthtSheR1->Scale(SCFHER1);
  hthtSheR1->Draw("hist");
  // hththeR1->SetStats(0);
  hththeR1->Draw("same");

  cthesishe->cd(3);
  hththeC->Add(hththeCR, -1);
  hththeC->Scale(ECHEC);
  hththedC->Add(hththedCR, -1);
  hththedC->Scale(ECHEDC);
  hththeC->Add(hththedC, -1);
  // hthtSheC->SetStats(0);
  hthtSheC->SetLineColor(kRed);
  hthtSheC->Draw("hist");
  // hththeC->SetStats(0);
  hththeC->Draw("same");

  cthesishe->cd(4);
  hththeL1->Add(hththeL1R, -1);
  hththeL1->Scale(ECHEL1);
  hththedL1->Add(hththedL1R, -1);
  hththedL1->Scale(ECHEDL1);
  hththeL1->Add(hththedL1, -1);
  // hthtSheL1->SetStats(0);
  hthtSheL1->SetLineColor(kRed);
  hthtSheL1->Scale(SCFHEL1);
  hthtSheL1->Draw("hist");
  // hththeL1->SetStats(0);
  hththeL1->Draw("same");

  cthesishe->cd(5);
  hththeL2->Add(hththeL2R, -1);
  hththeL2->Scale(ECHEL2);
  hththedL2->Add(hththedL2R, -1);
  hththedL2->Scale(ECHEDL2);
  hththeL2->Add(hththedL2, -1);
  // hthtSheL2->SetStats(0);
  hthtSheL2->SetLineColor(kRed);
  hthtSheL2->Scale(SCFHEL2);
  hthtSheL2->Draw("hist");
  // hththeL2->SetStats(0);
  hththeL2->Draw("same");
  cthesishe->Print(outputpdf);


  TCanvas *c8 = new TCanvas("c8", " tcoin"); 
 
  htcoinheC->SetStats(0);
  htcoinheC->SetLineColor(kGreen);
  // htcoinheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  // htcoinheC->GetXaxis()->SetTitle("Coincidence time (ns)"); 
  htcoinheC->Scale(ECHEDC);
  htcoinheC->Draw("Weight");

  htcoinheR1->SetStats(0);
  htcoinheR1->SetLineColor(kBlue);
  htcoinheR1->Scale(ECHEDR1);
  htcoinheR1->Draw(" same Weight");

  htcoinheR2->SetStats(0);
  htcoinheR2->SetLineColor(kYellow);
  htcoinheR2->Scale(ECHEDR2);
  htcoinheR2->Draw(" same Weight");

  htcoinheL2->SetStats(0);
  htcoinheL2->SetLineColor(kBlack);
  htcoinheL2->Scale(ECHEDL2);
  htcoinheL2->Draw("same Weight");

  htcoinheL1->SetStats(0);
  htcoinheL1->SetLineColor(kRed);
  htcoinheL1->Scale(ECHEDL1);
  htcoinheL1->Draw("same Weight ");
  
  TLine *l1he = new TLine(-1.0, 0.0, -1.0, 4500.0);
  TLine *l2he = new TLine(1.0, 0.0, 1.0, 4500.0);
  TLine *l3he = new TLine(-15.0, 0.0, -15.0, 4500.0);
  TLine *l4he = new TLine(-9.0, 0.0, -9.0, 4500.0);
  TLine *l5he = new TLine(7.0, 0.0, 7.0, 4500.0);
  TLine *l6he = new TLine(13.0, 0.0, 13.0, 4500.0);
  l1he->SetLineColor(kBlue);
  l2he->SetLineColor(kBlue);
  l3he->SetLineColor(kBlue);
  l4he->SetLineColor(kBlue);
  l5he->SetLineColor(kBlue);
  l6he->SetLineColor(kBlue);
  l1he->Draw("same");
  l2he->Draw("same");
  l3he->Draw("same");
  l4he->Draw("same");
  l5he->Draw("same");
  l6he->Draw("same");
  
  auto leghe = new TLegend(0.1,0.3,0.30,0.9);
  leghe->SetHeader("#epsilon = 0.774","C");
  leghe->SetTextSize(0.1);
  leghe->AddEntry(htcoinheR1,"Right1","l");
  leghe->AddEntry(htcoinheR2,"Right2","l");
  leghe->AddEntry(htcoinheC,"Center","l");
  leghe->AddEntry(htcoinheL1,"Left1","l");
  leghe->AddEntry(htcoinheL2,"Left2","l");
  leghe->Draw("same");

  c8->Print(outputpdf);

  TCanvas *c9 = new TCanvas("c9", " c9"); 
  htheC->SetStats(0);
  htheC->SetLineColor(kGreen);
  htheC->Add(htheCR,-1); 
  htheC->Draw("Weight");

  htheR1->SetStats(0);
  htheR1->SetLineColor(kBlue);
  htheR1->Add(htheR1R,-1); 
  htheR1->Draw("same Weight");

  htheR2->SetStats(0);
  htheR2->SetLineColor(kYellow);
  htheR2->Add(htheR2R,-1); 
  htheR2->Draw("same Weight");

  htheL1->SetStats(0);
  htheL1->SetLineColor(kRed);
  htheL1->Add(htheL1R, -1); 
  htheL1->Draw("same Weight");

  htheL2->SetStats(0);
  htheL2->SetLineColor(kBlack);
  htheL2->Add(htheL2R, -1); 
  htheL2->Draw("same Weight");
  c9->Print(outputpdf);

  TCanvas *c9In = new TCanvas("c9In", " c9In"); 
  htheInC->SetStats(0);
  htheInC->SetLineColor(kGreen);
  htheInC->Add(htheInCR,-1); 
  htheInC->Draw("Weight");

  htheInR1->SetStats(0);
  htheInR1->SetLineColor(kBlue);
  htheInR1->Add(htheInR1R,-1); 
  htheInR1->Draw("same Weight");

  htheInR2->SetStats(0);
  htheInR2->SetLineColor(kYellow);
  htheInR2->Add(htheInR2R,-1); 
  htheInR2->Draw("same Weight");

  htheInL1->SetStats(0);
  htheInL1->SetLineColor(kRed);
  htheInL1->Add(htheInL1R, -1); 
  htheInL1->Draw("same Weight");

  htheInL2->SetStats(0);
  htheInL2->SetLineColor(kBlack);
  htheInL2->Add(htheInL2R, -1); 
  htheInL2->Draw("same Weight");
  c9In->Print(outputpdf);
  
  TCanvas *cWheR11 = new TCanvas("cWheR11", "cWheR11");
  cWheR11->Divide(2, 2);
  cWheR11->cd(1);
  hW1heR1->Add(hW1heR1R, -1);
  hW1heR1->Draw("Weight");
  cWheR11->cd(2);
  hW2heR1->Add(hW2heR1R, -1);
  hW2heR1->Draw(" same Weight");
  cWheR11->cd(3);
  hW3heR1->Add(hW3heR1R, -1);
  hW3heR1->Draw(" same Weight");
  cWheR11->cd(4);
  hW4heR1->Add(hW4heR1R, -1);
  hW4heR1->Draw(" same Weight");
  cWheR11->Print(outputpdf);
  
  TCanvas *cWheR12 = new TCanvas("cWheR12", "cWheR12");
  cWheR12->Divide(2, 2);
  cWheR12->cd(1);
  hW5heR1->Add(hW5heR1R, -1);
  hW5heR1->Draw("Weight");
  cWheR12->cd(2);
  hW6heR1->Add(hW6heR1R, -1);
  hW6heR1->Draw(" same Weight");
  cWheR12->cd(3);
  hW7heR1->Add(hW7heR1R, -1);
  hW7heR1->Draw(" same Weight");
  cWheR12->cd(4);
  hW8heR1->Add(hW8heR1R, -1);
  hW8heR1->Draw(" same Weight");
  cWheR12->Print(outputpdf);

  TCanvas *cWheR21 = new TCanvas("cWheR21", "cWheR21");
  cWheR21->Divide(2, 2);
  cWheR21->cd(1);
  hW1heR2->Add(hW1heR2R, -1);
  hW1heR2->Draw("Weight");
  cWheR21->cd(2);
  hW2heR2->Add(hW2heR2R, -1);
  hW2heR2->Draw(" same Weight");
  cWheR21->cd(3);
  hW3heR2->Add(hW3heR2R, -1);
  hW3heR2->Draw(" same Weight");
  cWheR21->cd(4);
  hW4heR2->Add(hW4heR2R, -1);
  hW4heR2->Draw(" same Weight");
  cWheR21->Print(outputpdf);
  
  TCanvas *cWheR22 = new TCanvas("cWheR22", "cWheR22");
  cWheR22->Divide(2, 2);
  cWheR22->cd(1);
  hW5heR2->Add(hW5heR2R, -1);
  hW5heR2->Draw("Weight");
  cWheR22->cd(2);
  hW6heR2->Add(hW6heR2R, -1);
  hW6heR2->Draw(" same Weight");
  cWheR22->cd(3);
  hW7heR2->Add(hW7heR2R, -1);
  hW7heR2->Draw(" same Weight");
  cWheR22->cd(4);
  hW8heR2->Add(hW8heR2R, -1);
  hW8heR2->Draw(" same Weight");
  cWheR22->Print(outputpdf);
  
  TCanvas *cWheC1 = new TCanvas("cWheC1", "cWheC1");
  cWheC1->Divide(2, 2);
  cWheC1->cd(1);
  hW1heC->Add(hW1heCR, -1);
  hW1heC->Draw("Weight");
  cWheC1->cd(2);
  hW2heC->Add(hW2heCR, -1);
  hW2heC->Draw(" same Weight");
  cWheC1->cd(3);
  hW3heC->Add(hW3heCR, -1);
  hW3heC->Draw(" same Weight");
  cWheC1->cd(4);
  hW4heC->Add(hW4heCR, -1);
  hW4heC->Draw(" same Weight");
  cWheC1->Print(outputpdf);
  
  TCanvas *cWheC2 = new TCanvas("cWheC2", "cWheC2");
  cWheC2->Divide(2, 2);
  cWheC2->cd(1);
  hW5heC->Add(hW5heCR, -1);
  hW5heC->Draw("Weight");
  cWheC2->cd(2);
  hW6heC->Add(hW6heCR, -1);
  hW6heC->Draw(" same Weight");
  cWheC2->cd(3);
  hW7heC->Add(hW7heCR, -1);
  hW7heC->Draw(" same Weight");
  cWheC2->cd(4);
  hW8heC->Add(hW8heCR, -1);
  hW8heC->Draw(" same Weight");
  cWheC2->Print(outputpdf);

  TCanvas *cWheL11 = new TCanvas("cWheL11", "cWheL11");
  cWheL11->Divide(2, 2);
  cWheL11->cd(1);
  hW1heL1->Add(hW1heL1R, -1);
  hW1heL1->Draw("Weight");
  cWheL11->cd(2);
  hW2heL1->Add(hW2heL1R, -1);
  hW2heL1->Draw(" same Weight");
  cWheL11->cd(3);
  hW3heL1->Add(hW3heL1R, -1);
  hW3heL1->Draw(" same Weight");
  cWheL11->cd(4);
  hW4heL1->Add(hW4heL1R, -1);
  hW4heL1->Draw(" same Weight");
  cWheL11->Print(outputpdf);
  
  TCanvas *cWheL12 = new TCanvas("cWheL12", "cWheL12");
  cWheL12->Divide(2, 2);
  cWheL12->cd(1);
  hW5heL1->Add(hW5heL1R, -1);
  hW5heL1->Draw("Weight");
  cWheL12->cd(2);
  hW6heL1->Add(hW6heL1R, -1);
  hW6heL1->Draw(" same Weight");
  cWheL12->cd(3);
  hW7heL1->Add(hW7heL1R, -1);
  hW7heL1->Draw(" same Weight");
  cWheL12->cd(4);
  hW8heL1->Add(hW8heL1R, -1);
  hW8heL1->Draw(" same Weight");
  cWheL12->Print(outputpdf);

  TCanvas *cWheL21 = new TCanvas("cWheL21", "cWheL21");
  cWheL21->Divide(2, 2);
  cWheL21->cd(1);
  hW1heL2->Add(hW1heL2R, -1);
  hW1heL2->Draw("Weight");
  cWheL21->cd(2);
  hW2heL2->Add(hW2heL2R, -1);
  hW2heL2->Draw(" same Weight");
  cWheL21->cd(3);
  hW3heL2->Add(hW3heL2R, -1);
  hW3heL2->Draw(" same Weight");
  cWheL21->cd(4);
  hW4heL2->Add(hW4heL2R, -1);
  hW4heL2->Draw(" same Weight");
  cWheL21->Print(outputpdf);
  
  TCanvas *cWheL22 = new TCanvas("cWheL22", "cWheL22");
  cWheL22->Divide(2, 2);
  cWheL22->cd(1);
  hW5heL2->Add(hW5heL2R, -1);
  hW5heL2->Draw("Weight");
  cWheL22->cd(2);
  hW6heL2->Add(hW6heL2R, -1);
  hW6heL2->Draw(" same Weight");
  cWheL22->cd(3);
  hW7heL2->Add(hW7heL2R, -1);
  hW7heL2->Draw(" same Weight");
  cWheL22->cd(4);
  hW8heL2->Add(hW8heL2R, -1);
  hW8heL2->Draw(" same Weight");
  cWheL22->Print(outputpdf);

  TCanvas *cQ2heR11 = new TCanvas("cQ2heR11", "cQ2heR11");

  cQ2heR11->Divide(2, 2);
  cQ2heR11->cd(1);
  hQ21heR1->Add(hQ21heR1R, -1); 
  hQ21heR1->Draw(" Weight");
  cQ2heR11->cd(2);
  hQ22heR1->Add(hQ22heR1R, -1); 
  hQ22heR1->Draw("same Weight");
  cQ2heR11->cd(3);
  hQ23heR1->Add(hQ23heR1R, -1); 
  hQ23heR1->Draw("same Weight");
  cQ2heR11->cd(4);
  hQ24heR1->Add(hQ24heR1R, -1); 
  hQ24heR1->Draw("same Weight");
  cQ2heR11->Print(outputpdf);

  TCanvas *cQ2heR12 = new TCanvas("cQ2heR12", "cQ2heR12");

  cQ2heR12->Divide(2, 2);
  cQ2heR12->cd(1);
  hQ25heR1->Add(hQ25heR1R, -1); 
  hQ25heR1->Draw(" Weight");
  cQ2heR12->cd(2);
  hQ26heR1->Add(hQ26heR1R, -1); 
  hQ26heR1->Draw("same Weight");
  cQ2heR12->cd(3);
  hQ27heR1->Add(hQ27heR1R, -1); 
  hQ27heR1->Draw("same Weight");
  cQ2heR12->cd(4);
  hQ28heR1->Add(hQ28heR1R, -1); 
  hQ28heR1->Draw("same Weight");
  cQ2heR12->Print(outputpdf);

  TCanvas *cQ2heR21 = new TCanvas("cQ2heR21", "cQ2heR21");

  cQ2heR21->Divide(2, 2);
  cQ2heR21->cd(1);
  hQ21heR2->Add(hQ21heR2R, -1); 
  hQ21heR2->Draw(" Weight");
  cQ2heR21->cd(2);
  hQ22heR2->Add(hQ22heR2R, -1); 
  hQ22heR2->Draw("same Weight");
  cQ2heR21->cd(3);
  hQ23heR2->Add(hQ23heR2R, -1); 
  hQ23heR2->Draw("same Weight");
  cQ2heR21->cd(4);
  hQ24heR2->Add(hQ24heR2R, -1); 
  hQ24heR2->Draw("same Weight");
  cQ2heR21->Print(outputpdf);

  TCanvas *cQ2heR22 = new TCanvas("cQ2heR22", "cQ2heR22");

  cQ2heR22->Divide(2, 2);
  cQ2heR22->cd(1);
  hQ25heR2->Add(hQ25heR2R, -1); 
  hQ25heR2->Draw(" Weight");
  cQ2heR22->cd(2);
  hQ26heR2->Add(hQ26heR2R, -1); 
  hQ26heR2->Draw("same Weight");
  cQ2heR22->cd(3);
  hQ27heR2->Add(hQ27heR2R, -1); 
  hQ27heR2->Draw("same Weight");
  cQ2heR22->cd(4);
  hQ28heR2->Add(hQ28heR2R, -1); 
  hQ28heR2->Draw("same Weight");
  cQ2heR22->Print(outputpdf);


  TCanvas *cQ2heC1 = new TCanvas("cQ2heC1", "cQ2heC1");

  cQ2heC1->Divide(2, 2);
  cQ2heC1->cd(1);
  hQ21heC->Add(hQ21heCR, -1); 
  hQ21heC->Draw(" Weight");
  cQ2heC1->cd(2);
  hQ22heC->Add(hQ22heCR, -1); 
  hQ22heC->Draw("same Weight");
  cQ2heC1->cd(3);
  hQ23heC->Add(hQ23heCR, -1); 
  hQ23heC->Draw("same Weight");
  cQ2heC1->cd(4);
  hQ24heC->Add(hQ24heCR, -1); 
  hQ24heC->Draw("same Weight");
  cQ2heC1->Print(outputpdf);

  TCanvas *cQ2heC2 = new TCanvas("cQ2heC2", "cQ2heC2");

  cQ2heC2->Divide(2, 2);
  cQ2heC2->cd(1);
  hQ25heC->Add(hQ25heCR, -1); 
  hQ25heC->Draw(" Weight");
  cQ2heC2->cd(2);
  hQ26heC->Add(hQ26heCR, -1); 
  hQ26heC->Draw("same Weight");
  cQ2heC2->cd(3);
  hQ27heC->Add(hQ27heCR, -1); 
  hQ27heC->Draw("same Weight");
  cQ2heC2->cd(4);
  hQ28heC->Add(hQ28heCR, -1); 
  hQ28heC->Draw("same Weight");
  cQ2heC2->Print(outputpdf);

  TCanvas *cQ2heL11 = new TCanvas("cQ2heL11", "cQ2heL11");

  cQ2heL11->Divide(2, 2);
  cQ2heL11->cd(1);
  hQ21heL1->Add(hQ21heL1R, -1); 
  hQ21heL1->Draw(" Weight");
  cQ2heL11->cd(2);
  hQ22heL1->Add(hQ22heL1R, -1); 
  hQ22heL1->Draw("same Weight");
  cQ2heL11->cd(3);
  hQ23heL1->Add(hQ23heL1R, -1); 
  hQ23heL1->Draw("same Weight");
  cQ2heL11->cd(4);
  hQ24heL1->Add(hQ24heL1R, -1); 
  hQ24heL1->Draw("same Weight");
  cQ2heL11->Print(outputpdf);

  TCanvas *cQ2heL12 = new TCanvas("cQ2heL12", "cQ2heL12");

  cQ2heL12->Divide(2, 2);
  cQ2heL12->cd(1);
  hQ25heL1->Add(hQ25heL1R, -1); 
  hQ25heL1->Draw(" Weight");
  cQ2heL12->cd(2);
  hQ26heL1->Add(hQ26heL1R, -1); 
  hQ26heL1->Draw("same Weight");
  cQ2heL12->cd(3);
  hQ27heL1->Add(hQ27heL1R, -1); 
  hQ27heL1->Draw("same Weight");
  cQ2heL12->cd(4);
  hQ28heL1->Add(hQ28heL1R, -1); 
  hQ28heL1->Draw("same Weight");
  cQ2heL12->Print(outputpdf);

  TCanvas *cQ2heL21 = new TCanvas("cQ2heL21", "cQ2heL21");

  cQ2heL21->Divide(2, 2);
  cQ2heL21->cd(1);
  hQ21heL2->Add(hQ21heL2R, -1); 
  hQ21heL2->Draw(" Weight");
  cQ2heL21->cd(2);
  hQ22heL2->Add(hQ22heL2R, -1); 
  hQ22heL2->Draw("same Weight");
  cQ2heL21->cd(3);
  hQ23heL2->Add(hQ23heL2R, -1); 
  hQ23heL2->Draw("same Weight");
  cQ2heL21->cd(4);
  hQ24heL2->Add(hQ24heL2R, -1); 
  hQ24heL2->Draw("same Weight");
  cQ2heL21->Print(outputpdf);

  TCanvas *cQ2heL22 = new TCanvas("cQ2heL22", "cQ2heL22");

  cQ2heL22->Divide(2, 2);
  cQ2heL22->cd(1);
  hQ25heL2->Add(hQ25heL2R, -1); 
  hQ25heL2->Draw(" Weight");
  cQ2heL22->cd(2);
  hQ26heL2->Add(hQ26heL2R, -1); 
  hQ26heL2->Draw("same Weight");
  cQ2heL22->cd(3);
  hQ27heL2->Add(hQ27heL2R, -1); 
  hQ27heL2->Draw("same Weight");
  cQ2heL22->cd(4);
  hQ28heL2->Add(hQ28heL2R, -1); 
  hQ28heL2->Draw("same Weight");
  cQ2heL22->Print(outputpdf);

  TCanvas *ctheR11 = new TCanvas("ctheR11", "ctheR11");
  ctheR11->Divide(2, 2);
  ctheR11->cd(1);
  ht1heR1->Add(ht1heR1R, -1);
  ht1heR1->Draw("Weight");
  ctheR11->cd(2);
  ht2heR1->Add(ht2heR1R, -1);
  ht2heR1->Draw("same Weight");
  ctheR11->cd(3);
  ht3heR1->Add(ht3heR1R, -1);
  ht3heR1->Draw("same Weight");
  ctheR11->cd(4);
  ht4heR1->Add(ht4heR1R, -1);
  ht4heR1->Draw("same Weight");
  ctheR11->Print(outputpdf);

  TCanvas *ctheR12 = new TCanvas("ctheR12", "ctheR12");
  ctheR12->Divide(2, 2);
  ctheR12->cd(1);
  ht5heR1->Add(ht5heR1R, -1);
  ht5heR1->Draw("Weight");
  ctheR12->cd(2);
  ht6heR1->Add(ht6heR1R, -1);
  ht6heR1->Draw("same Weight");
  ctheR12->cd(3);
  ht7heR1->Add(ht7heR1R, -1);
  ht7heR1->Draw("same Weight");
  ctheR12->cd(4);
  ht8heR1->Add(ht8heR1R, -1);
  ht8heR1->Draw("same Weight");
  ctheR12->Print(outputpdf);

  TCanvas *ctheR21 = new TCanvas("ctheR21", "ctheR21");
  ctheR21->Divide(2, 2);
  ctheR21->cd(1);
  ht1heR2->Add(ht1heR2R, -1);
  ht1heR2->Draw("Weight");
  ctheR21->cd(2);
  ht2heR2->Add(ht2heR2R, -1);
  ht2heR2->Draw("same Weight");
  ctheR21->cd(3);
  ht3heR2->Add(ht3heR2R, -1);
  ht3heR2->Draw("same Weight");
  ctheR21->cd(4);
  ht4heR2->Add(ht4heR2R, -1);
  ht4heR2->Draw("same Weight");
  ctheR21->Print(outputpdf);

  TCanvas *ctheR22 = new TCanvas("ctheR22", "ctheR22");
  ctheR22->Divide(2, 2);
  ctheR22->cd(1);
  ht5heR2->Add(ht5heR2R, -1);
  ht5heR2->Draw("Weight");
  ctheR22->cd(2);
  ht6heR2->Add(ht6heR2R, -1);
  ht6heR2->Draw("same Weight");
  ctheR22->cd(3);
  ht7heR2->Add(ht7heR2R, -1);
  ht7heR2->Draw("same Weight");
  ctheR22->cd(4);
  ht8heR2->Add(ht8heR2R, -1);
  ht8heR2->Draw("same Weight");
  ctheR22->Print(outputpdf);

  TCanvas *ctheC1 = new TCanvas("ctheC1", "ctheC1");
  ctheC1->Divide(2, 2);
  ctheC1->cd(1);
  ht1heC->Add(ht1heCR, -1);
  ht1heC->Draw("Weight");
  ctheC1->cd(2);
  ht2heC->Add(ht2heCR, -1);
  ht2heC->Draw("same Weight");
  ctheC1->cd(3);
  ht3heC->Add(ht3heCR, -1);
  ht3heC->Draw("same Weight");
  ctheC1->cd(4);
  ht4heC->Add(ht4heCR, -1);
  ht4heC->Draw("same Weight");
  ctheC1->Print(outputpdf);

  TCanvas *ctheC2 = new TCanvas("ctheC2", "ctheC2");
  ctheC2->Divide(2, 2);
  ctheC2->cd(1);
  ht5heC->Add(ht5heCR, -1);
  ht5heC->Draw("Weight");
  ctheC2->cd(2);
  ht6heC->Add(ht6heCR, -1);
  ht6heC->Draw("same Weight");
  ctheC2->cd(3);
  ht7heC->Add(ht7heCR, -1);
  ht7heC->Draw("same Weight");
  ctheC2->cd(4);
  ht8heC->Add(ht8heCR, -1);
  ht8heC->Draw("same Weight");
  ctheC2->Print(outputpdf);

  TCanvas *ctheL11 = new TCanvas("ctheL11", "ctheL11");
  ctheL11->Divide(2, 2);
  ctheL11->cd(1);
  ht1heL1->Add(ht1heL1R, -1);
  ht1heL1->Draw("Weight");
  ctheL11->cd(2);
  ht2heL1->Add(ht2heL1R, -1);
  ht2heL1->Draw("same Weight");
  ctheL11->cd(3);
  ht3heL1->Add(ht3heL1R, -1);
  ht3heL1->Draw("same Weight");
  ctheL11->cd(4);
  ht4heL1->Add(ht4heL1R, -1);
  ht4heL1->Draw("same Weight");
  ctheL11->Print(outputpdf);

  TCanvas *ctheL12 = new TCanvas("ctheL12", "ctheL12");
  ctheL12->Divide(2, 2);
  ctheL12->cd(1);
  ht5heL1->Add(ht5heL1R, -1);
  ht5heL1->Draw("Weight");
  ctheL12->cd(2);
  ht6heL1->Add(ht6heL1R, -1);
  ht6heL1->Draw("same Weight");
  ctheL12->cd(3);
  ht7heL1->Add(ht7heL1R, -1);
  ht7heL1->Draw("same Weight");
  ctheL12->cd(4);
  ht8heL1->Add(ht8heL1R, -1);
  ht8heL1->Draw("same Weight");
  ctheL12->Print(outputpdf);

  TCanvas *ctheL21 = new TCanvas("ctheL21", "ctheL21");
  ctheL21->Divide(2, 2);
  ctheL21->cd(1);
  ht1heL2->Add(ht1heL2R, -1);
  ht1heL2->Draw("Weight");
  ctheL21->cd(2);
  ht2heL2->Add(ht2heL2R, -1);
  ht2heL2->Draw("same Weight");
  ctheL21->cd(3);
  ht3heL2->Add(ht3heL2R, -1);
  ht3heL2->Draw("same Weight");
  ctheL21->cd(4);
  ht4heL2->Add(ht4heL2R, -1);
  ht4heL2->Draw("same Weight");
  ctheL21->Print(outputpdf);

  TCanvas *ctheL22 = new TCanvas("ctheL22", "ctheL22");
  ctheL22->Divide(2, 2);
  ctheL22->cd(1);
  ht5heL2->Add(ht5heL2R, -1);
  ht5heL2->Draw("Weight");
  ctheL22->cd(2);
  ht6heL2->Add(ht6heL2R, -1);
  ht6heL2->Draw("same Weight");
  ctheL22->cd(3);
  ht7heL2->Add(ht7heL2R, -1);
  ht7heL2->Draw("same Weight");
  ctheL22->cd(4);
  ht8heL2->Add(ht8heL2R, -1);
  ht8heL2->Draw("same Weight");
  ctheL22->Print(outputpdf);
  //Vijay
  /**
     ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
     ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
     ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
     ofstream outheR1("kindata/kindata.pl_375_781_+2680.dat");
     ofstream outheR2("kindata/kindata.pl_375_781_+2680.dat");
  **/

  ofstream outheR1("kindata/kindata.pl_425_774_+2000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheR1<<1.000<<endl;
      if(i==1)
	outheR1<<hW1heR1->GetMean()<<"\t"<<hW1heR1->GetMeanError()<<"\t"<<hQ21heR1->GetMean()<<"\t"<<hQ21heR1->GetMeanError()<<"\t"<<ht1heR1->GetMean()<<"\t"<<ht1heR1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheR1<<hW2heR1->GetMean()<<"\t"<<hW2heR1->GetMeanError()<<"\t"<<hQ22heR1->GetMean()<<"\t"<< hQ22heR1->GetMeanError()<<"\t"<<ht2heR1->GetMean()<<"\t"<<ht2heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheR1<<hW3heR1->GetMean()<<"\t"<<hW3heR1->GetMeanError()<<"\t"<<hQ23heR1->GetMean()<<"\t"<< hQ23heR1->GetMeanError()<<"\t"<<ht3heR1->GetMean()<<"\t"<<ht3heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheR1<<hW4heR1->GetMean()<<"\t"<<hW4heR1->GetMeanError()<<"\t"<<hQ24heR1->GetMean()<<"\t"<< hQ24heR1->GetMeanError()<<"\t"<<ht4heR1->GetMean()<<"\t"<<ht4heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheR1<<hW5heR1->GetMean()<<"\t"<<hW5heR1->GetMeanError()<<"\t"<<hQ25heR1->GetMean()<<"\t"<< hQ25heR1->GetMeanError()<<"\t"<<ht5heR1->GetMean()<<"\t"<<ht5heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheR1<<hW6heR1->GetMean()<<"\t"<<hW6heR1->GetMeanError()<<"\t"<<hQ26heR1->GetMean()<<"\t"<< hQ26heR1->GetMeanError()<<"\t"<<ht6heR1->GetMean()<<"\t"<<ht6heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheR1<<hW7heR1->GetMean()<<"\t"<<hW7heR1->GetMeanError()<<"\t"<<hQ27heR1->GetMean()<<"\t"<< hQ27heR1->GetMeanError()<<"\t"<<ht7heR1->GetMean()<<"\t"<<ht7heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheR1<<hW8heR1->GetMean()<<"\t"<<hW8heR1->GetMeanError()<<"\t"<<hQ28heR1->GetMean()<<"\t"<< hQ28heR1->GetMeanError()<<"\t"<<ht8heR1->GetMean()<<"\t"<<ht8heR1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheR1<<1.000000<<endl;
    } 
  outheR1.close();

  ofstream outheR2("kindata/kindata.pl_425_774_+3205.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheR2<<1.000<<endl;
      if(i==1)
	outheR2<<hW1heR2->GetMean()<<"\t"<<hW1heR2->GetMeanError()<<"\t"<<hQ21heR2->GetMean()<<"\t"<<hQ21heR2->GetMeanError()<<"\t"<<ht1heR2->GetMean()<<"\t"<<ht1heR2->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheR2<<hW2heR2->GetMean()<<"\t"<<hW2heR2->GetMeanError()<<"\t"<<hQ22heR2->GetMean()<<"\t"<< hQ22heR2->GetMeanError()<<"\t"<<ht2heR2->GetMean()<<"\t"<<ht2heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheR2<<hW3heR2->GetMean()<<"\t"<<hW3heR2->GetMeanError()<<"\t"<<hQ23heR2->GetMean()<<"\t"<< hQ23heR2->GetMeanError()<<"\t"<<ht3heR2->GetMean()<<"\t"<<ht3heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheR2<<hW4heR2->GetMean()<<"\t"<<hW4heR2->GetMeanError()<<"\t"<<hQ24heR2->GetMean()<<"\t"<< hQ24heR2->GetMeanError()<<"\t"<<ht4heR2->GetMean()<<"\t"<<ht4heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheR2<<hW5heR2->GetMean()<<"\t"<<hW5heR2->GetMeanError()<<"\t"<<hQ25heR2->GetMean()<<"\t"<< hQ25heR2->GetMeanError()<<"\t"<<ht5heR2->GetMean()<<"\t"<<ht5heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheR2<<hW6heR2->GetMean()<<"\t"<<hW6heR2->GetMeanError()<<"\t"<<hQ26heR2->GetMean()<<"\t"<< hQ26heR2->GetMeanError()<<"\t"<<ht6heR2->GetMean()<<"\t"<<ht6heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheR2<<hW7heR2->GetMean()<<"\t"<<hW7heR2->GetMeanError()<<"\t"<<hQ27heR2->GetMean()<<"\t"<< hQ27heR2->GetMeanError()<<"\t"<<ht7heR2->GetMean()<<"\t"<<ht7heR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheR2<<hW8heR2->GetMean()<<"\t"<<hW8heR2->GetMeanError()<<"\t"<<hQ28heR2->GetMean()<<"\t"<< hQ28heR2->GetMeanError()<<"\t"<<ht8heR2->GetMean()<<"\t"<<ht8heR2->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheR2<<1.000000<<endl;
    } 
  outheR2.close();
  
  ofstream outheC("kindata/kindata.pl_425_774_+0000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheC<<1.000<<endl;
      if(i==1)
	outheC<<hW1heC->GetMean()<<"\t"<<hW1heC->GetMeanError()<<"\t"<<hQ21heC->GetMean()<<"\t"<<hQ21heC->GetMeanError()<<"\t"<<ht1heC->GetMean()<<"\t"<<ht1heC->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheC<<hW2heC->GetMean()<<"\t"<<hW2heC->GetMeanError()<<"\t"<<hQ22heC->GetMean()<<"\t"<< hQ22heC->GetMeanError()<<"\t"<<ht2heC->GetMean()<<"\t"<<ht2heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheC<<hW3heC->GetMean()<<"\t"<<hW3heC->GetMeanError()<<"\t"<<hQ23heC->GetMean()<<"\t"<< hQ23heC->GetMeanError()<<"\t"<<ht3heC->GetMean()<<"\t"<<ht3heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheC<<hW4heC->GetMean()<<"\t"<<hW4heC->GetMeanError()<<"\t"<<hQ24heC->GetMean()<<"\t"<< hQ24heC->GetMeanError()<<"\t"<<ht4heC->GetMean()<<"\t"<<ht4heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheC<<hW5heC->GetMean()<<"\t"<<hW5heC->GetMeanError()<<"\t"<<hQ25heC->GetMean()<<"\t"<< hQ25heC->GetMeanError()<<"\t"<<ht5heC->GetMean()<<"\t"<<ht5heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheC<<hW6heC->GetMean()<<"\t"<<hW6heC->GetMeanError()<<"\t"<<hQ26heC->GetMean()<<"\t"<< hQ26heC->GetMeanError()<<"\t"<<ht6heC->GetMean()<<"\t"<<ht6heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheC<<hW7heC->GetMean()<<"\t"<<hW7heC->GetMeanError()<<"\t"<<hQ27heC->GetMean()<<"\t"<< hQ27heC->GetMeanError()<<"\t"<<ht7heC->GetMean()<<"\t"<<ht7heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheC<<hW8heC->GetMean()<<"\t"<<hW8heC->GetMeanError()<<"\t"<<hQ28heC->GetMean()<<"\t"<< hQ28heC->GetMeanError()<<"\t"<<ht8heC->GetMean()<<"\t"<<ht8heC->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheC<<1.000000<<endl;
    } 
  outheC.close();

  ofstream outheL1("kindata/kindata.pl_425_774_-2000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheL1<<1.000<<endl;
      if(i==1)
	outheL1<<hW1heL1->GetMean()<<"\t"<<hW1heL1->GetMeanError()<<"\t"<<hQ21heL1->GetMean()<<"\t"<<hQ21heL1->GetMeanError()<<"\t"<<ht1heL1->GetMean()<<"\t"<<ht1heL1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheL1<<hW2heL1->GetMean()<<"\t"<<hW2heL1->GetMeanError()<<"\t"<<hQ22heL1->GetMean()<<"\t"<< hQ22heL1->GetMeanError()<<"\t"<<ht2heL1->GetMean()<<"\t"<<ht2heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheL1<<hW3heL1->GetMean()<<"\t"<<hW3heL1->GetMeanError()<<"\t"<<hQ23heL1->GetMean()<<"\t"<< hQ23heL1->GetMeanError()<<"\t"<<ht3heL1->GetMean()<<"\t"<<ht3heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheL1<<hW4heL1->GetMean()<<"\t"<<hW4heL1->GetMeanError()<<"\t"<<hQ24heL1->GetMean()<<"\t"<< hQ24heL1->GetMeanError()<<"\t"<<ht4heL1->GetMean()<<"\t"<<ht4heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheL1<<hW5heL1->GetMean()<<"\t"<<hW5heL1->GetMeanError()<<"\t"<<hQ25heL1->GetMean()<<"\t"<< hQ25heL1->GetMeanError()<<"\t"<<ht5heL1->GetMean()<<"\t"<<ht5heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheL1<<hW6heL1->GetMean()<<"\t"<<hW6heL1->GetMeanError()<<"\t"<<hQ26heL1->GetMean()<<"\t"<< hQ26heL1->GetMeanError()<<"\t"<<ht6heL1->GetMean()<<"\t"<<ht6heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheL1<<hW7heL1->GetMean()<<"\t"<<hW7heL1->GetMeanError()<<"\t"<<hQ27heL1->GetMean()<<"\t"<< hQ27heL1->GetMeanError()<<"\t"<<ht7heL1->GetMean()<<"\t"<<ht7heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheL1<<hW8heL1->GetMean()<<"\t"<<hW8heL1->GetMeanError()<<"\t"<<hQ28heL1->GetMean()<<"\t"<< hQ28heL1->GetMeanError()<<"\t"<<ht8heL1->GetMean()<<"\t"<<ht8heL1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheL1<<1.000000<<endl;
    } 
  outheL1.close();

  ofstream outheL2("kindata/kindata.pl_425_774_-4005.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheL2<<1.000<<endl;
      if(i==1)
	outheL2<<hW1heL2->GetMean()<<"\t"<<hW1heL2->GetMeanError()<<"\t"<<hQ21heL2->GetMean()<<"\t"<<hQ21heL2->GetMeanError()<<"\t"<<ht1heL2->GetMean()<<"\t"<<ht1heL2->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheL2<<hW2heL2->GetMean()<<"\t"<<hW2heL2->GetMeanError()<<"\t"<<hQ22heL2->GetMean()<<"\t"<< hQ22heL2->GetMeanError()<<"\t"<<ht2heL2->GetMean()<<"\t"<<ht2heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheL2<<hW3heL2->GetMean()<<"\t"<<hW3heL2->GetMeanError()<<"\t"<<hQ23heL2->GetMean()<<"\t"<< hQ23heL2->GetMeanError()<<"\t"<<ht3heL2->GetMean()<<"\t"<<ht3heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheL2<<hW4heL2->GetMean()<<"\t"<<hW4heL2->GetMeanError()<<"\t"<<hQ24heL2->GetMean()<<"\t"<< hQ24heL2->GetMeanError()<<"\t"<<ht4heL2->GetMean()<<"\t"<<ht4heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheL2<<hW5heL2->GetMean()<<"\t"<<hW5heL2->GetMeanError()<<"\t"<<hQ25heL2->GetMean()<<"\t"<< hQ25heL2->GetMeanError()<<"\t"<<ht5heL2->GetMean()<<"\t"<<ht5heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheL2<<hW6heL2->GetMean()<<"\t"<<hW6heL2->GetMeanError()<<"\t"<<hQ26heL2->GetMean()<<"\t"<< hQ26heL2->GetMeanError()<<"\t"<<ht6heL2->GetMean()<<"\t"<<ht6heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheL2<<hW7heL2->GetMean()<<"\t"<<hW7heL2->GetMeanError()<<"\t"<<hQ27heL2->GetMean()<<"\t"<< hQ27heL2->GetMeanError()<<"\t"<<ht7heL2->GetMean()<<"\t"<<ht7heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheL2<<hW8heL2->GetMean()<<"\t"<<hW8heL2->GetMeanError()<<"\t"<<hQ28heL2->GetMean()<<"\t"<< hQ28heL2->GetMeanError()<<"\t"<<ht8heL2->GetMean()<<"\t"<<ht8heL2->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheL2<<1.000000<<endl;
    } 
  outheL2.close();

  TCanvas *c10 = new TCanvas("c10", " c10"); 
  
  hph_qheC->SetStats(0);
  hph_qheC->SetLineColor(kGreen);
  hph_qheC->Add(hph_qheCR, -1);
  hph_qheC->Draw(" Weight");

  hph_qheL2->SetStats(0);
  hph_qheL2->SetLineColor(kBlack);
  hph_qheL2->Add(hph_qheL2R, -1);
  hph_qheL2->Draw("same Weight");
  
  hph_qheL1->SetStats(0);
  hph_qheL1->SetLineColor(kRed);
  hph_qheL1->Add(hph_qheL1R, -1);
  hph_qheL1->Draw("same Weight");
 
  hph_qheR1->SetStats(0);
  hph_qheR1->SetLineColor(kBlue);
  hph_qheR1->Add(hph_qheR1R, -1);
  hph_qheR1->Draw("same Weight");

  hph_qheR2->SetStats(0);
  hph_qheR2->SetLineColor(kYellow);
  hph_qheR2->Add(hph_qheR2R, -1);
  hph_qheR2->Draw("same Weight");
  c10->Print(outputpdf);

  TCanvas *phheR1 = new TCanvas("phheR1", "phheR1"); 
  //  hphheSR1->SetStats(0);
  hphheSR1->Scale(SCFHER1);
  hphheSR1->Draw("");
  // hphheR1->SetStats(0);
  hphheR1->SetLineColor(kGreen);
  hphheR1->Add(hphheR1R, -1);
  hphheR1->Scale(ECHER1);
  hphheR1->Draw("same");
  phheR1->Print(outputpdf);

  TCanvas *phheRR1 = new TCanvas("phheRR1", "phheRR1"); 
  hphheR1->Divide(hphheSR1);
  hphheR1->Draw();
  phheRR1->Print(outputpdf);
 
  TCanvas *phheC = new TCanvas("phheC", "phheC"); 
  //  hphheSC->SetStats(0);
  hphheSC->Scale(SCFHEC);
  hphheSC->Draw("");
  // hphheC->SetStats(0);
  hphheC->SetLineColor(kGreen);
  hphheC->Add(hphheCR, -1);
  hphheC->Scale(ECHEC);
  hphheC->Draw("same");
  phheC->Print(outputpdf);

  TCanvas *phheRC = new TCanvas("phheRC", "phheRC"); 
  hphheC->Divide(hphheSC);
  hphheC->Draw();
  phheRC->Print(outputpdf);

  TCanvas *phheL1 = new TCanvas("phheL1", "phheL1"); 
  //  hphheSL1->SetStats(0);
  hphheSL1->Scale(SCFHEL1);
  hphheSL1->Draw("");
  // hphheL1->SetStats(0);
  hphheL1->SetLineColor(kGreen);
  hphheL1->Add(hphheL1R, -1);
  hphheL1->Scale(ECHEL1);
  hphheL1->Draw("same");
  phheL1->Print(outputpdf);

  TCanvas *phheRL1 = new TCanvas("phheRL1", "phheRL1"); 
  hphheL1->Divide(hphheSL1);
  hphheL1->Draw();
  phheRL1->Print(outputpdf);

  TCanvas *phheL2 = new TCanvas("phheL2", "phheL2"); 
  //  hphheSL2->SetStats(0);
  hphheSL2->Scale(SCFHEL2);
  hphheSL2->Draw("");
  // hphheL2->SetStats(0);
  hphheL2->SetLineColor(kGreen);
  hphheL2->Add(hphheL2R, -1);
  hphheL2->Scale(ECHEL2);
  hphheL2->Draw("same");
  phheL2->Print(outputpdf);

  TCanvas *phheRL2 = new TCanvas("phheRL2", "phheRL2"); 
  hphheL2->Divide(hphheSL2);
  hphheL2->Draw();
  phheRL2->Print(outputpdf);

  TCanvas *c11 = new TCanvas("c11", " c11"); 
  hmmheC->SetStats(0);
  hmmheC->SetLineColor(kGreen);
  hmmheC->Add(hmmheCR, -1);
  hmmheC->Draw("Weight");

  hmmheL2->SetStats(0);
  hmmheL2->SetLineColor(kBlack);
  hmmheL2->Add(hmmheL2R, -1);
  hmmheL2->Draw("same Weight");

  hmmheL1->SetStats(0);
  hmmheL1->SetLineColor(kRed);
  hmmheL1->Add(hmmheL1R, -1);
  hmmheL1->Draw("same Weight");

  hmmheR1->SetStats(0);
  hmmheR1->SetLineColor(kBlue);
  hmmheR1->Add(hmmheR1R, -1);
  hmmheR1->Draw("same Weight");

  hmmheR2->SetStats(0);
  hmmheR2->SetLineColor(kYellow);
  hmmheR2->Add(hmmheR2R, -1);
  hmmheR2->Draw("same Weight");
  
  TLine *l7he = new TLine(0.92, -5000.0, 0.92, 100000.0);
  TLine *l8he = new TLine(0.98, -5000.0, 0.98, 100000.0);
  l7he->SetLineColor(kBlue);
  l8he->SetLineColor(kBlue);
  l7he->Draw("same"); 
  l8he->Draw("same");
  c11->Print(outputpdf);
	    	    
  TCanvas *c12 = new TCanvas("c12", " c12");
  c12->Divide(2, 0, 0.05); 
  c12->cd(1); 
  hQ2WHEC->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  c12->cd(2); 
  hQ2WHEC1->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");
  c12->Print(outputpdf);

  TCanvas *cdeltaSheR1 = new TCanvas("cdeltaSheR1", " cdeltaSheR1");
  //  H_ssdelta_SheR1->SetStats(0);
  H_ssdelta_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SheR1->Scale(SCFHER1);
  H_ssdelta_SheR1->Draw("");
  H_ssdelta_DheR1->Add(H_ssdelta_DheR1R, -1);
  H_ssdelta_DheR1->Scale(ECHER1);
  H_ssdelta_DheR1->SetLineColor(kGreen);
  H_ssdelta_DheR1->Draw("same"); 
  cdeltaSheR1->Print(outputpdf);

  //Plot Added Nov 19, 2025
  TCanvas *chighepR2 = new TCanvas("chighepR2", "chighepR2");
  chighepR2->Divide(3, 2, 0.01, 0.01);
  TLatex *thepR2 = new TLatex(0.33,0.5,"High Epsilon Right2 Setting");
  thepR2->SetNDC();
  thepR2->SetTextSize(0.04);
  thepR2->SetTextColor(kBlue);
  thepR2->Draw();

  chighepR2->cd(1);
  H_ssdelta_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  //  H_ssdelta_SheR2->Scale(SCFHER2);
  H_ssdelta_SheR2->SetStats(0);
  H_ssdelta_SheR2->Draw("same");  
  H_ssdelta_DheR2->Add(H_ssdelta_DheR2R, -1);
  H_ssdelta_DheR2->Scale(ECHER2);
  H_ssdelta_DummyheR2->Add(H_ssdelta_DummyheR2R, -1);
  H_ssdelta_DummyheR2->Scale(ECHEDR2);
  H_ssdelta_DheR2->Add(H_ssdelta_DummyheR2, -1);
  H_ssdelta_DheR2->SetLineColor(kGreen);
  H_ssdelta_DheR2->Draw("same"); 

  chighepR2->cd(2);
  H_ssxptar_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SheR2->SetStats(0);
  H_ssxptar_SheR2->Draw("same");  
  H_ssxptar_DheR2->Add( H_ssxptar_DheR2R, -1);
  H_ssxptar_DheR2->Scale(ECHER2);
  H_ssxptar_DummyheR2->Add( H_ssxptar_DummyheR2R, -1);
  H_ssxptar_DummyheR2->Scale(ECHEDR2);
  H_ssxptar_DheR2->Add(H_ssxptar_DummyheR2, -1);
  H_ssxptar_DheR2->SetLineColor(kGreen);
  H_ssxptar_DheR2->Draw("same"); 

  chighepR2->cd(3);
  H_ssyptar_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SheR2->SetStats(0);
  H_ssyptar_SheR2->Draw("same");  
  H_ssyptar_DheR2->Add(H_ssyptar_DheR2R, -1);
  H_ssyptar_DheR2->Scale(ECHER2);
  H_ssyptar_DummyheR2->Add(H_ssyptar_DummyheR2R, -1);
  H_ssyptar_DummyheR2->Scale(ECHEDR2);
  H_ssyptar_DheR2->Add(H_ssyptar_DummyheR2, -1);
  H_ssyptar_DheR2->SetLineColor(kGreen);
  H_ssyptar_DheR2->Draw("same"); 

  chighepR2->cd(4);
  H_hsdelta_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SheR2->SetStats(0);
  H_hsdelta_SheR2->Draw("same");  
  H_hsdelta_DheR2->Add(H_hsdelta_DheR2R, -1);
  H_hsdelta_DheR2->Scale(ECHER2);
  H_hsdelta_DummyheR2->Add(H_hsdelta_DummyheR2R, -1);
  H_hsdelta_DummyheR2->Scale(ECHEDR2);
  H_hsdelta_DheR2->Add(H_hsdelta_DummyheR2, -1);
  H_hsdelta_DheR2->SetLineColor(kGreen);
  H_hsdelta_DheR2->Draw("same"); 

  chighepR2->cd(5);
  H_hsxptar_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SheR2->SetStats(0);
  H_hsxptar_SheR2->Draw("same");
  H_hsxptar_DheR2->Add(H_hsxptar_DheR2R, -1);
  H_hsxptar_DheR2->Scale(ECHER2);
  H_hsxptar_DummyheR2->Add(H_hsxptar_DummyheR2R, -1);
  H_hsxptar_DummyheR2->Scale(ECHEDR2);
  H_hsxptar_DheR2->Add(H_hsxptar_DummyheR2, -1);
  H_hsxptar_DheR2->SetLineColor(kGreen);
  H_hsxptar_DheR2->Draw("same"); 
  
  chighepR2->cd(6);
  H_hsyptar_SheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SheR2->SetStats(0);
  H_hsyptar_SheR2->Draw("same");  
  H_hsyptar_DheR2->Add(H_hsyptar_DheR2R, -1);
  H_hsyptar_DheR2->Scale(ECHER2);
  H_hsyptar_DummyheR2->Add(H_hsyptar_DummyheR2R, -1);
  H_hsyptar_DummyheR2->Scale(ECHEDR2);
  H_hsyptar_DheR2->Add(H_hsyptar_DummyheR2, -1);
  H_hsyptar_DheR2->SetLineColor(kGreen);
  H_hsyptar_DheR2->Draw("same");   
  chighepR2->Print(outputpdf);

  //Add kinematic plots
  
  TCanvas *chighep2C = new TCanvas("chighep2C", "chighep2C");
  chighep2C->Divide(3, 2, 0.01, 0.01);
  TLatex *thep2C = new TLatex(0.33,0.5,"High Epsilon Right2 Setting");
  thep2C->SetNDC();
  thep2C->SetTextSize(0.04);
  thep2C->SetTextColor(kBlue);
  thep2C->Draw();
  chighep2C->cd(1);
  hthtSheR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hthtSheR2->SetStats(0);
  hthtSheR2->Draw("same");
  hththeR2->Add(hththeR2R, -1);
  hththeR2->Scale(ECHER2);
  hththedR2->Add(hththedR2R, -1);
  hththedR2->Scale(ECHEDR2);
  hththeR2->Add(hththedR2, -1);
  hththeR2->SetLineColor(kGreen);
  hththeR2->Draw("same");

  chighep2C->cd(2);
  hphheSR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hphheSR2->SetStats(0); 
  hphheSR2->Draw("same");
  hphheR2->Add(hphheR2R, -1);
  hphheR2->Scale(ECHER2);
  hphhedR2->Add(hphhedR2R, -1);
  hphhedR2->Scale(ECHEDR2);
  hphheR2->Add(hphhedR2, -1);
  hphheR2->SetLineColor(kGreen);
  hphheR2->Draw("same");

  chighep2C->cd(3);
  hQ2heSR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hQ2heSR2->SetStats(0); 
  hQ2heSR2->Draw("same");
  hQ2DheR2->Add(hQ2DheR2R, -1);
  hQ2DheR2->Scale(ECHER2);
  hQ2DummyheR2->Add(hQ2DummyheR2R, -1);
  hQ2DummyheR2->Scale(ECHEDR2);
  hQ2DheR2->Add(hQ2DummyheR2, -1);
  hQ2DheR2->SetLineColor(kGreen);
  hQ2DheR2->Draw("same");

  chighep2C->cd(4);
  hWheSR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hWheSR2->SetStats(0); 
  hWheSR2->Draw("same");
  hWhDeR2->Add(hWhDeR2R, -1);
  hWhDeR2->Scale(ECHER2);
  hWhDummyeR2->Add(hWhDummyeR2R, -1);
  hWhDummyeR2->Scale(ECHEDR2);
  hWhDeR2->Add(hWhDummyeR2, -1);
  hWhDeR2->SetLineColor(kGreen);
  hWhDeR2->Draw("same");

  chighep2C->cd(5);
  hepheSR2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  hepheSR2->SetStats(0); 
  hepheSR2->Draw("same");
  hephDeR2->Add(hephDeR2R, -1);
  hephDeR2->Scale(ECHER2);
  hephDummyeR2->Add(hephDummyeR2R, -1);
  hephDummyeR2->Scale(ECHEDR2);
  hephDeR2->Add(hephDummyeR2, -1);
  hephDeR2->SetLineColor(kGreen);
  hephDeR2->Draw("same");  
  chighep2C->Print(outputpdf);
  
  
  TCanvas *cdeltaSheC = new TCanvas("cdeltaSheC", " cdeltaSheC");
  // H_ssdelta_SheC->SetStats(0);
  H_ssdelta_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SheC->Scale(SCFHEC);
  H_ssdelta_SheC->Draw("");
  H_ssdelta_DheC->Add(H_ssdelta_DheCR, -1);
  H_ssdelta_DheC->Scale(ECHEC);
  H_ssdelta_DheC->SetLineColor(kGreen);
  H_ssdelta_DheC->Draw("same"); 
  cdeltaSheC->Print(outputpdf);

  TCanvas *cdeltaSheL1 = new TCanvas("cdeltaSheL1", " cdeltaSheL1");
  //  H_ssdelta_SheL1->SetStats(0);
  H_ssdelta_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SheL1->Scale(SCFHEL1);
  H_ssdelta_SheL1->Draw("");
  H_ssdelta_DheL1->Add(H_ssdelta_DheL1R, -1);
  H_ssdelta_DheL1->Scale(ECHEL1);
  H_ssdelta_DheL1->SetLineColor(kGreen);
  H_ssdelta_DheL1->Draw("same"); 
  cdeltaSheL1->Print(outputpdf);

  TCanvas *cdeltaSheL2 = new TCanvas("cdeltaSheL2", " cdeltaSheL2");
  // H_ssdelta_SheL2->SetStats(0);
  H_ssdelta_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssdelta_SheL2->Scale(SCFHEL2);
  H_ssdelta_SheL2->Draw("");
  H_ssdelta_DheL2->Add(H_ssdelta_DheL2R, -1);
  H_ssdelta_DheL2->Scale(ECHEL2);
  H_ssdelta_DheL2->SetLineColor(kGreen);
  H_ssdelta_DheL2->Draw("same"); 
  cdeltaSheL2->Print(outputpdf);

  TCanvas *cdeltaHheR1 = new TCanvas("cdeltaHheR1", " cdeltaHheR1");
  //  H_hsdelta_SheR1->SetStats(0);
  H_hsdelta_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SheR1->Scale(SCFHER1);
  H_hsdelta_SheR1->Draw("");
  H_hsdelta_DheR1->Add(H_hsdelta_DheR1R, -1);
  H_hsdelta_DheR1->Scale(ECHER1);
  H_hsdelta_DheR1->SetLineColor(kGreen);
  H_hsdelta_DheR1->Draw("same"); 
  cdeltaHheR1->Print(outputpdf);
  
  TCanvas *cdeltaHheC = new TCanvas("cdeltaHheC", " cdeltaHheC");
  // H_hsdelta_SheC->SetStats(0);
  H_hsdelta_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SheC->Scale(SCFHEC);
  H_hsdelta_SheC->Draw("");
  H_hsdelta_DheC->Add(H_hsdelta_DheCR, -1);
  H_hsdelta_DheC->Scale(ECHEC);
  H_hsdelta_DheC->SetLineColor(kGreen);
  H_hsdelta_DheC->Draw("same"); 
  cdeltaHheC->Print(outputpdf);

  TCanvas *cdeltaHheL1 = new TCanvas("cdeltaHheL1", " cdeltaHheL1");
  // H_hsdelta_SheL1->SetStats(0);
  H_hsdelta_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SheL1->Scale(SCFHEL1);
  H_hsdelta_SheL1->Draw("");
  H_hsdelta_DheL1->Add(H_hsdelta_DheL1R, -1);
  H_hsdelta_DheL1->Scale(ECHEL1);
  H_hsdelta_DheL1->SetLineColor(kGreen);
  H_hsdelta_DheL1->Draw("same"); 
  cdeltaHheL1->Print(outputpdf);

  TCanvas *cdeltaHheL2 = new TCanvas("cdeltaHheL2", " cdeltaHheL2");
  // H_hsdelta_SheL2->SetStats(0);
  H_hsdelta_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsdelta_SheL2->Scale(SCFHEL2);
  H_hsdelta_SheL2->Draw("");
  H_hsdelta_DheL2->Add(H_hsdelta_DheL2R, -1);
  H_hsdelta_DheL2->Scale(ECHEL2);
  H_hsdelta_DheL2->SetLineColor(kGreen);
  H_hsdelta_DheL2->Draw("same"); 
  cdeltaHheL2->Print(outputpdf);

  TCanvas *cxptarSheR1 = new TCanvas("cxptarSheR1", " cxptarSheR1");
  // H_ssxptar_SheR1->SetStats(0);
  H_ssxptar_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SheR1->Scale(SCFHER1);
  H_ssxptar_SheR1->Draw("");
  H_ssxptar_DheR1->Add(H_ssxptar_DheR1R, -1);
  H_ssxptar_DheR1->Scale(ECHER1);
  H_ssxptar_DheR1->SetLineColor(kGreen);
  H_ssxptar_DheR1->Draw("same");
  cxptarSheR1->Print(outputpdf);
  
  TCanvas *cxptarSheC = new TCanvas("cxptarSheC", " cxptarSheC");
  // H_ssxptar_SheC->SetStats(0);
  H_ssxptar_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SheC->Scale(SCFHEC);
  H_ssxptar_SheC->Draw("");
  H_ssxptar_DheC->Add(H_ssxptar_DheCR, -1);
  H_ssxptar_DheC->Scale(ECHEC);
  H_ssxptar_DheC->SetLineColor(kGreen);
  H_ssxptar_DheC->Draw("same");
  cxptarSheC->Print(outputpdf);

  TCanvas *cxptarSheL1 = new TCanvas("cxptarSheL1", " cxptarSheL1");
  // H_ssxptar_SheL1->SetStats(0);
  H_ssxptar_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SheL1->Scale(SCFHEL1);
  H_ssxptar_SheL1->Draw("");
  H_ssxptar_DheL1->Add(H_ssxptar_DheL1R, -1);
  H_ssxptar_DheL1->Scale(ECHEL1);
  H_ssxptar_DheL1->SetLineColor(kGreen);
  H_ssxptar_DheL1->Draw("same");
  cxptarSheL1->Print(outputpdf);

  TCanvas *cxptarSheL2 = new TCanvas("cxptarSheL2", " cxptarSheL2");
  // H_ssxptar_SheL2->SetStats(0);
  H_ssxptar_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssxptar_SheL2->Scale(SCFHEL2);
  H_ssxptar_SheL2->Draw("");
  H_ssxptar_DheL2->Add(H_ssxptar_DheL2R, -1);
  H_ssxptar_DheL2->Scale(ECHEL2);
  H_ssxptar_DheL2->SetLineColor(kGreen);
  H_ssxptar_DheL2->Draw("same");
  cxptarSheL2->Print(outputpdf);

  TCanvas *cyptarSheR1 = new TCanvas("cyptarSheR1", " cyptarSheR1");
  // H_ssyptar_SheR1->SetStats(0);
  H_ssyptar_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SheR1->Scale(SCFHER1);
  H_ssyptar_SheR1->Draw("");
  H_ssyptar_DheR1->Add(H_ssyptar_DheR1R, -1);
  H_ssyptar_DheR1->Scale(ECHER1);
  H_ssyptar_DheR1->SetLineColor(kGreen);
  H_ssyptar_DheR1->Draw("same");
  cyptarSheR1->Print(outputpdf);
  
  TCanvas *cyptarSheC = new TCanvas("cyptarSheC", " cyptarSheC");
  // H_ssyptar_SheC->SetStats(0);
  H_ssyptar_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SheC->Scale(SCFHEC);
  H_ssyptar_SheC->Draw("");
  H_ssyptar_DheC->Add(H_ssyptar_DheCR, -1);
  H_ssyptar_DheC->Scale(ECHEC);
  H_ssyptar_DheC->SetLineColor(kGreen);
  H_ssyptar_DheC->Draw("same");
  cyptarSheC->Print(outputpdf);

  TCanvas *cyptarSheL1 = new TCanvas("cyptarSheL1", " cyptarSheL1");
  // H_ssyptar_SheL1->SetStats(0);
  H_ssyptar_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SheL1->Scale(SCFHEL1);
  H_ssyptar_SheL1->Draw("");
  H_ssyptar_DheL1->Add(H_ssyptar_DheL1R, -1);
  H_ssyptar_DheL1->Scale(ECHEL1);
  H_ssyptar_DheL1->SetLineColor(kGreen);
  H_ssyptar_DheL1->Draw("same");
  cyptarSheL1->Print(outputpdf);

  TCanvas *cyptarSheL2 = new TCanvas("cyptarSheL2", " cyptarSheL2");
  // H_ssyptar_SheL2->SetStats(0);
  H_ssyptar_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_ssyptar_SheL2->Scale(SCFHEL2);
  H_ssyptar_SheL2->Draw("");
  H_ssyptar_DheL2->Add(H_ssyptar_DheL2R, -1);
  H_ssyptar_DheL2->Scale(ECHEL2);
  H_ssyptar_DheL2->SetLineColor(kGreen);
  H_ssyptar_DheL2->Draw("same");
  cyptarSheL2->Print(outputpdf);

  TCanvas *cxptarHheR1 = new TCanvas("cxptarHheR1", " cxptarHheR1");
  // H_hsxptar_SheR1->SetStats(0);
  H_hsxptar_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SheR1->Scale(SCFHER1);
  H_hsxptar_SheR1->Draw("");
  H_hsxptar_DheR1->Add(H_hsxptar_DheR1R, -1);
  H_hsxptar_DheR1->Scale(ECHER1);
  H_hsxptar_DheR1->SetLineColor(kGreen);
  H_hsxptar_DheR1->Draw("same");
  cxptarHheR1->Print(outputpdf);
  
  TCanvas *cxptarHheC = new TCanvas("cxptarHheC", " cxptarHheC");
  // H_hsxptar_SheC->SetStats(0);
  H_hsxptar_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SheC->Scale(SCFHEC);
  H_hsxptar_SheC->Draw("");
  H_hsxptar_DheC->Add(H_hsxptar_DheCR, -1);
  H_hsxptar_DheC->Scale(ECHEC);
  H_hsxptar_DheC->SetLineColor(kGreen);
  H_hsxptar_DheC->Draw("same");
  cxptarHheC->Print(outputpdf);

  TCanvas *cxptarHheL1 = new TCanvas("cxptarHheL1", " cxptarHheL1");
  // H_hsxptar_SheL1->SetStats(0);
  H_hsxptar_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SheL1->Scale(SCFHEL1);
  H_hsxptar_SheL1->Draw("");
  H_hsxptar_DheL1->Add(H_hsxptar_DheL1R, -1);
  H_hsxptar_DheL1->Scale(ECHEL1);
  H_hsxptar_DheL1->SetLineColor(kGreen);
  H_hsxptar_DheL1->Draw("same");
  cxptarHheL1->Print(outputpdf);

  TCanvas *cxptarHheL2 = new TCanvas("cxptarHheL2", " cxptarHheL2");
  // H_hsxptar_SheL2->SetStats(0);
  H_hsxptar_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsxptar_SheL2->Scale(SCFHEL2);
  H_hsxptar_SheL2->Draw("");
  H_hsxptar_DheL2->Add(H_hsxptar_DheL2R, -1);
  H_hsxptar_DheL2->Scale(ECHEL2);
  H_hsxptar_DheL2->SetLineColor(kGreen);
  H_hsxptar_DheL2->Draw("same");
  cxptarHheL2->Print(outputpdf);

  TCanvas *cyptarHheR1 = new TCanvas("cyptarHheR1", " cyptarHheR1");
  // H_hsyptar_SheR1->SetStats(0);
  H_hsyptar_SheR1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SheR1->Scale(SCFHER1);
  H_hsyptar_SheR1->Draw("");
  H_hsyptar_DheR1->Add(H_hsyptar_DheR1R, -1);
  H_hsyptar_DheR1->Scale(ECHER1);
  H_hsyptar_DheR1->SetLineColor(kGreen);
  H_hsyptar_DheR1->Draw("same");
  cyptarHheR1->Print(outputpdf);
  
  TCanvas *cyptarHheC = new TCanvas("cyptarHheC", " cyptarHheC");
  // H_hsyptar_SheC->SetStats(0);
  H_hsyptar_SheC->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SheC->Scale(SCFHEC);
  H_hsyptar_SheC->Draw("");
  H_hsyptar_DheC->Add(H_hsyptar_DheCR, -1);
  H_hsyptar_DheC->Scale(ECHEC);
  H_hsyptar_DheC->SetLineColor(kGreen);
  H_hsyptar_DheC->Draw("same");
  cyptarHheC->Print(outputpdf);

  TCanvas *cyptarHheL1 = new TCanvas("cyptarHheL1", " cyptarHheL1");
  // H_hsyptar_SheL1->SetStats(0);
  H_hsyptar_SheL1->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SheL1->Scale(SCFHEL1);
  H_hsyptar_SheL1->Draw("");
  H_hsyptar_DheL1->Add(H_hsyptar_DheL1R, -1);
  H_hsyptar_DheL1->Scale(ECHEL1);
  H_hsyptar_DheL1->SetLineColor(kGreen);
  H_hsyptar_DheL1->Draw("same");
  cyptarHheL1->Print(outputpdf);

  TCanvas *cyptarHheL2 = new TCanvas("cyptarHheL2", " cyptarHheL2");
  // H_hsyptar_SheL2->SetStats(0);
  H_hsyptar_SheL2->GetYaxis()->SetTitle("Yield (Events/mC)"); 
  H_hsyptar_SheL2->Scale(SCFHEL2);
  H_hsyptar_SheL2->Draw("");
  H_hsyptar_DheL2->Add(H_hsyptar_DheL2R, -1);
  H_hsyptar_DheL2->Scale(ECHEL2);
  H_hsyptar_DheL2->SetLineColor(kGreen);
  H_hsyptar_DheL2->Draw("same");
  cyptarHheL2->Print(outputpdf);

  TCanvas *cYheR1 = new TCanvas("cYheR1", " cYheR1");
  hYheR1->Add(hYheR1R, -1);  
  hYheR1->Scale(ECHER1);  
  hYheR1->SetStats(0);
  hYheR1->Draw("COLZ");
  cYheR1->Print(outputpdf);

  TCanvas *cYSheR1 = new TCanvas("cYSheR1", " cYSheR1");
  hYSheR1->SetStats(0);
  hYSheR1->Scale(SCFHER1);
  hYSheR1->Draw("COLZ");
  cYSheR1->Print(outputpdf);
  ofstream YheR1("yield/yields.pl_425_774_+2000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YheR1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YheR1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YheR1<<hYheR1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YheR1.close();
  //Dummy
  hYhedR1->Add(hYhedR1R, -1);  
  hYhedR1->Scale(ECHEDR1);     // Yield  
  hYhedR1->Scale(THCF);       // Thickness corrected Yield

  ofstream YdheR1("yield/yields.pl_425_774_+2000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdheR1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdheR1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdheR1<<hYhedR1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdheR1.close();
  ofstream YSheR1("yield/yields.pl_425_774_+2000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSheR1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSheR1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSheR1<<hYSheR1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSheR1.close();

  TCanvas *cYheR2 = new TCanvas("cYheR2", " cYheR2");
  hYheR2->Add(hYheR2R, -1);  
  hYheR2->Scale(ECHER2);  
  hYheR2->SetStats(0);
  hYheR2->Draw("COLZ");
  cYheR2->Print(outputpdf);

  TCanvas *cYSheR2 = new TCanvas("cYSheR2", " cYSheR2");
  hYSheR2->SetStats(0);
  hYSheR2->Scale(SCFHER2);
  hYSheR2->Draw("COLZ");
  cYSheR2->Print(outputpdf);
  
  ofstream YheR2("yield/yields.pl_425_774_+3205.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YheR2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YheR2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YheR2<<hYheR2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YheR2.close();
  //Dummy
  hYhedR2->Add(hYhedR2R, -1);  
  hYhedR2->Scale(ECHEDR2);     // Yield  
  hYhedR2->Scale(THCF);       // Thickness corrected Yield

  ofstream YdheR2("yield/yields.pl_425_774_+3205.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdheR2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdheR2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdheR2<<hYhedR2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdheR2.close();
  ofstream YSheR2("yield/yields.pl_425_774_+3205.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSheR2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSheR2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSheR2<<hYSheR2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSheR2.close();

  TCanvas *cYheC = new TCanvas("cYheC", " cYheC");
  hYheC->Add(hYheCR, -1);  
  hYheC->Scale(ECHEC);  
  hYheC->SetStats(0);
  hYheC->Draw("COLZ");
  cYheC->Print(outputpdf);

  TCanvas *cYSheC = new TCanvas("cYSheC", " cYSheC");
  hYSheC->SetStats(0);
  hYSheC->Scale(SCFHEC);
  hYSheC->Draw("COLZ");
  cYSheC->Print(outputpdf);
  ofstream YheC("yield/yields.pl_425_774_+0000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YheC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YheC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YheC<<hYheC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YheC.close();
  //Dummy
  hYhedC->Add(hYhedCR, -1);  
  hYhedC->Scale(ECHEDC);     // Yield  
  hYhedC->Scale(THCF);      // Thickness corrected Yield

  ofstream YdheC("yield/yields.pl_425_774_+0000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdheC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdheC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdheC<<hYhedC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdheC.close();
  ofstream YSheC("yield/yields.pl_425_774_+0000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSheC<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSheC<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSheC<<hYSheC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSheC.close();

  TCanvas *cYheL1 = new TCanvas("cYheL1", " cYheL1");
  hYheL1->Add(hYheL1R, -1);  
  hYheL1->Scale(ECHEL1);  
  hYheL1->SetStats(0);
  hYheL1->Draw("COLZ");
  cYheL1->Print(outputpdf);

  TCanvas *cYSheL1 = new TCanvas("cYSheL1", " cYSheL1");
  hYSheL1->SetStats(0);
  hYSheL1->Scale(SCFHEL1);
  hYSheL1->Draw("COLZ");
  cYSheL1->Print(outputpdf);
  ofstream YheL1("yield/yields.pl_425_774_-2000.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YheL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YheL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YheL1<<hYheL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YheL1.close();
  //Dummy
  hYhedL1->Add(hYhedL1R, -1);  
  hYhedL1->Scale(ECHEDL1);    //Yield  
  hYhedL1->Scale(THCF);      // Thickness corrected Yield

  ofstream YdheL1("yield/yields.pl_425_774_-2000.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdheL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdheL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdheL1<<hYhedL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdheL1.close();
  ofstream YSheL1("yield/yields.pl_425_774_-2000.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSheL1<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSheL1<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSheL1<<hYSheL1->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSheL1.close();

  TCanvas *cYheL2 = new TCanvas("cYheL2", " cYheL2");
  hYheL2->Add(hYheL2R, -1);  
  hYheL2->Scale(ECHEL2);  
  hYheL2->SetStats(0);
  hYheL2->Draw("COLZ");
  cYheL2->Print(outputpdf);

  TCanvas *cYSheL2 = new TCanvas("cYSheL2", " cYSheL2");
  hYSheL2->SetStats(0);
  hYSheL2->Scale(SCFHEL2);
  hYSheL2->Draw("COLZ");
  cYSheL2->Print(outputpdf);
  ofstream YheL2("yield/yields.pl_425_774_-4005.target");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YheL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YheL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YheL2<<hYheL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YheL2.close();
  //Dummy
  hYhedL2->Add(hYhedL2R, -1);  
  hYhedL2->Scale(ECHEDL2);    // Yield  
  hYhedL2->Scale(THCF);      // Thickness corrected Yield

  ofstream YdheL2("yield/yields.pl_425_774_-4005.dummy");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YdheL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YdheL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YdheL2<<hYhedL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YdheL2.close();
  ofstream YSheL2("yield/yields.pl_425_774_-4005.simc");
  for(int j=1 ; j <=10 ; ++j) 
    {
      if (j==1)
	{
	  YSheL2<< 1.0000<<endl;	
	  continue;
	}
      if (j==10)
	{
	  YSheL2<< 1.0000<<endl;	
	  break;
	}
      for(Long64_t i = 2; i <= 17; i++) 
	{
	  Double_t err;
	  YSheL2<<hYSheL2->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YSheL2.close();

  TH2D *Clonehe = (TH2D*)hYheR1->Clone("Clonehe");
  Clonehe->Add(hYheR2,+1);
  Clonehe->Add(hYheC,+1);
  Clonehe->Add(hYheL1,+1);
  Clonehe->Add(hYheL2,+1);
  Clonehe->SetTitle("Total Yield for High epsilon");
  TCanvas *cYthe = new TCanvas("cYthe", " cYthe");
  Clonehe->Draw("COLZ");
  cYthe->Print(outputpdf);

  // Write some info in root file

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *Hist = OutHisto_file->mkdir("Hist");
  Hist->cd();
  hphSL2->Write();
  hphL2->Write();
  hphSL1->Write();
  hphL1->Write();
  hphSC->Write();
  hphC->Write();

  hphmeSL2->Write();
  hphmeL2->Write();
  hphmeSL1->Write();
  hphmeL1->Write();
  hphmeSC->Write();
  hphmeC->Write();

  hphheSL2->Write();
  hphheL2->Write();
  hphheSL1->Write();
  hphheL1->Write();
  hphheSC->Write();
  hphheC->Write();
  hphheSR1->Write();
  hphheR1->Write();
  hphheSR2->Write();
  hphheR2->Write();
 
  htcoinC->Write();
  htcoinL1->Write();
  htcoinL2->Write();
  htC->Write();
  htL1->Write();
  htL2->Write();
  hQ2WC->GetListOfFunctions()->Add(Dcut,"L");
  hQ2WC->Write();
  hQ2WC->GetListOfFunctions()->Clear("L");
  OutHisto_file->Close();


  // t limit for the analysis
  /*
  Double_t tmn = 0.00595+0.002;
  Double_t tmx = 0.07+0.002;
  */
  Double_t tmn = 0.00795;
  Double_t tmx = 0.072;

  //-t for all three e
  TCanvas *c13 = new TCanvas("c13", " c13");
  // Mid epsilon
  
  htmeC->SetStats(0);
  htmeC->SetLineColor(kBlue);
  htmeC->Add(htmeCR,-1); 
  htheC->GetXaxis()->SetTitle("t (GeV^{2})");
  htheC->GetYaxis()->SetTitle("Events");
  TAxis *In5 = htmeC->GetXaxis();
  Double_t meC =  htmeC->Integral(In5->FindBin(tmn), In5->FindBin(tmx), "");
  cout<< " Mid epsilon C events: "<<meC<<endl; 
  htmeC->Draw("Weight");

  htmeL1->SetStats(0);
  htmeL1->SetLineColor(kBlue);
  htmeL1->Add(htmeL1R, -1); 
  TAxis *In8 = htmeL1->GetXaxis();
  Double_t meL1 =  htmeL1->Integral(In8->FindBin(tmn), In8->FindBin(tmx), "");
  cout<< " Mid epsilon L1 events: "<<meL1<<endl; 
  htmeL1->Draw("same Weight");

  htmeL2->SetStats(0);
  htmeL2->SetLineColor(kBlue);
  htmeL2->Add(htmeL2R, -1); 
  TAxis *In9 = htmeL2->GetXaxis();
  Double_t meL2 =  htmeL2->Integral(In9->FindBin(tmn), In9->FindBin(tmx), "");
  cout<< " Mid epsilon L2 events: "<<meL2<<endl; 
  htmeL2->Draw("same Weight");  

  //High epsilon  
  htheC->SetStats(0);
  htheC->SetLineColor(kGreen);
  htheC->Add(htheCR,-1);
  // htheC->GetXaxis()->SetTitle("t (GeV^{2})");
  // htheC->GetYaxis()->SetTitle("Events");
  TAxis *In1 = htheC->GetXaxis();
  Double_t heC =  htheC->Integral(In1->FindBin(tmn), In1->FindBin(tmx), "");
  cout<< " High epsilon C events: "<<heC<<endl;
  htheC->Draw("Same Weight");

  htheR1->SetStats(0);
  htheR1->SetLineColor(kGreen);
  htheR1->Add(htheR1R,-1); 
  TAxis *In2 = htheR1->GetXaxis();
  Double_t heR1 =  htheR1->Integral(In2->FindBin(tmn), In2->FindBin(tmx), "");
  cout<< " High epsilon R1 events: "<<heR1<<endl;
  htheR1->Draw("same Weight");
  
  htheR2->SetStats(0);
  htheR2->SetLineColor(kGreen);
  htheR2->Add(htheR2R,-1); 
  TAxis *Inh2 = htheR2->GetXaxis();
  Double_t heR2 =  htheR2->Integral(Inh2->FindBin(tmn), Inh2->FindBin(tmx), "");
  cout<< " High epsilon R2 events: "<<heR2<<endl;
  htheR2->Draw("same Weight");
  
  htheL1->SetStats(0);
  htheL1->SetLineColor(kGreen);
  htheL1->Add(htheL1R, -1); 
  TAxis *In3 = htheL1->GetXaxis();
  Double_t heL1 =  htheL1->Integral(In3->FindBin(tmn), In3->FindBin(tmx), "");
  cout<< " High epsilon L1 events: "<<heL1<<endl;
  htheL1->Draw("same Weight");

  htheL2->SetStats(0);
  htheL2->SetLineColor(kGreen);
  htheL2->Add(htheL2R, -1); 
  TAxis *In4 = htheL2->GetXaxis();
  Double_t heL2 =  htheL2->Integral(In4->FindBin(tmn), In4->FindBin(tmx), "");
  cout<< " High epsilon L2 events: "<<heL2<<endl; 
  htheL2->Draw("same Weight");
 
  //Low epsilon

  htC->SetStats(0);
  htC->SetLineColor(kRed);
  htC->Add(htCR,-1); 
  TAxis *In10 = htC->GetXaxis();
  Double_t leC =  htC->Integral(In10->FindBin(tmn), In10->FindBin(tmx), "");
  cout<< " Low epsilon C events: "<<leC<<endl; 
  htC->Draw("same Weight");

  htL1->SetStats(0);
  htL1->SetLineColor(kRed);
  htL1->Add(htL1R, -1); 
  TAxis *In11 = htL1->GetXaxis();
  Double_t leL1 =  htL1->Integral(In11->FindBin(tmn), In11->FindBin(tmx), "");
  cout<< " Low epsilon L1 events: "<<leL1<<endl; 
  htL1->Draw("same Weight");
 
  htL2->SetStats(0);
  htL2->SetLineColor(kRed);
  htL2->Add(htL2R, -1); 
  TAxis *In12 = htL2->GetXaxis();
  Double_t leL2 =  htL2->Integral(In12->FindBin(tmn), In12->FindBin(tmx), "");
  cout<< " Low epsilon L2 events: "<<leL2<<endl; 
  htL2->Draw(" same Weight");

  //choose -t

  cout<<In1->FindBin(tmn)<<endl;
  cout<<In1->FindBin(tmx)<<endl;

  Double_t Events1 = 0.0;
  Double_t Events2 = 0.0;
  Double_t Events3 = 0.0;
  Double_t Events4 = 0.0;
  Double_t Events5 = 0.0;
  Double_t Events55 = 0.0;
  Double_t Events6 = 0.0;
  Double_t Events7 = 0.0;
  Double_t Events8 = 0.0;
  Double_t Events9 = 0.0;
  Double_t Events10 = 0.0;
  Double_t Events11 = 0.0;
  Double_t Events12 = 0.0;

  
  // calculations of events in each t

  /*
     for(Long64_t i = 2; i <=9; i++) 
     {
     Events1 = htheInC->GetBinContent(i);
     cout<<i<<"  = "<<Events1<<endl;
     Events2 = htheInR1->GetBinContent(i);
     cout<<i<<"  = "<<Events2<<endl;        
     Events3 = htheInR2->GetBinContent(i);
     cout<<i<<"  = "<<Events3<<endl;        
     Events4 = htheInL1->GetBinContent(i);
     cout<<i<<"  = "<<Events4<<endl;
     Events55 = htheInL2->GetBinContent(i);
     cout<<i<<"  = "<<Events55<<endl;

     cout<<"Total Events he"<< " i "<<" = "<<Events1+Events2+Events3+Events4+Events55<<endl;
     }
     
     for(Long64_t i = 2; i <=9; i++) 
     {
      
     Events5 = htmeInC->GetBinContent(i);
     cout<<i<<"  = "<<Events5<<endl;
     Events8 = htmeInL1->GetBinContent(i);
     cout<<i<<"  = "<<Events8<<endl;
     Events9 = htmeInL2->GetBinContent(i);
     cout<<i<<"  = "<<Events9<<endl;

     cout<<"Total Events me"<< " i "<<" = "<<Events5+Events8+Events9<<endl;
     }

     for(Long64_t i = 2; i <=9; i++) 
    
     {
      
     Events10 = htInC->GetBinContent(i);
     cout<<i<<"  = "<<Events10<<endl;
     Events11 = htInL1->GetBinContent(i);
     cout<<i<<"  = "<<Events11<<endl;
     Events12 = htInL2->GetBinContent(i);
     cout<<i<<"  = "<<Events12<<endl;

     cout<<"Total Events Le"<< " i "<<" = "<<Events10+Events11+Events12<<endl;

     }
  */
  
   // if(In1->FindBin(0.005) >=41 && In1->FindBin(0.005) <= 246)  
   {
     // for(Long64_t i = 47; i < 65; i++)       // 1st t  updated one
     // for(Long64_t i = 65; i < 76; i++)           // 2nd t
     // for(Long64_t i = 76; i < 89; i++)     // 3rd t
     // for(Long64_t i = 89; i < 104; i++)     // 4th t
     // for(Long64_t i = 104; i < 122; i++)           // 5th t
     // for(Long64_t i = 122; i < 149; i++)           // 6th t
     for(Long64_t i = 149; i <= 200; i++)           // 7th t
     // for(Long64_t i = 47; i <= 200; i++)           //get total evernts
	
   {
   	Events1 += htheC->GetBinContent(i);
   	Events2 += htheR1->GetBinContent(i);
   	Events3 += htheR2->GetBinContent(i);
   	Events4 += htheL1->GetBinContent(i);
   	Events5 += htheL2->GetBinContent(i);
   	Events6 += htmeC->GetBinContent(i);
   	Events7 += htmeL1->GetBinContent(i);
   	Events8 += htmeL2->GetBinContent(i);
	Events9 += htC->GetBinContent(i);
	Events10 += htL1->GetBinContent(i);
	Events11 += htL2->GetBinContent(i);
   }   
      
   //    cout<< "  "<<Events1<<endl;    
   //    cout<< "  "<<Events2<<endl;    
   //    cout<< "  "<<Events3<<endl;    
   //    cout<< "  "<<Events4<<endl;    
   //   cout<< "  "<<Events5<<endl;    
   //   cout<< "  "<<Events6<<endl;    
   //   cout<< "  "<<Events7<<endl;    
   //   cout<< "  "<<Events8<<endl;    
   //    cout<< "  "<<Events9<<endl;    
   // cout<< "  "<<Events10<<endl;    
   // cout<< "  "<<Events11<<endl;    
   //cout<< "  "<<Events12<<endl;    
   }
    
   cout<<" High Epsilon Events: " <<Events1 + Events2 +Events3 +Events4 + Events5 <<endl;
   cout<<" Mid Epsilon Events: " <<Events6 + Events7 + Events8<<endl;
   cout<<" Low Epsilon Events: " <<Events9 + Events10 + Events11<<endl;
   cout<<" Total Events: " <<Events1 + Events2 +Events3 +Events4 + Events5 + Events6 + Events7 + Events8 + Events9 + Events10 + Events11<<endl;
  
  
  cout<<" Integral show is commented out "<<endl;

  //  TLine *ltlow  = new TLine(0.005, -80.0, 0.005, 1800);
  //TLine *lthigh = new TLine(0.08, -80.0, 0.08, 1800.0);
  // ltlow->SetLineColor(kBlack);
  //lthigh->SetLineColor(kBlack);
  //  ltlow->Draw("same"); 
  //lthigh->Draw("same");
    
  Double_t Ymin = 0.0;
  Double_t Ymax = 1750.0;

  Double_t fstedge = ((TAxis*)htheC->GetXaxis())->GetBinCenter(47);
  cout<<" fstedge "<<fstedge<<endl;
  TLine *fstedgel = new TLine(fstedge, Ymin, fstedge, Ymax);
  fstedgel->SetLineColor(kBlack);
  fstedgel->Draw("same");

  Double_t lastedge = ((TAxis*)htheC->GetXaxis())->GetBinCenter(200);
  cout<<" lastedge "<<lastedge<<endl;
  TLine *lastedgel = new TLine(lastedge, Ymin, lastedge, Ymax);
  lastedgel->SetLineColor(kBlack);
  lastedgel->Draw("same");

  Double_t bin_center1 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(64);
  cout<<" 1 "<<bin_center1<<endl;
  TLine *lt1  = new TLine(bin_center1, Ymin, bin_center1, Ymax);
  lt1->SetLineColor(kBlack);
  lt1->Draw("same");

  Double_t bin_center2 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(75);
  cout<<"2 "<<bin_center2<<endl;
  TLine *lt2  = new TLine(bin_center2, Ymin, bin_center2, Ymax);
  lt2->SetLineColor(kBlack);
  lt2->Draw("same");

  Double_t bin_center3 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(88);
  cout<<"3 "<<bin_center3<<endl;
  TLine *lt3  = new TLine(bin_center3, Ymin, bin_center3, Ymax);
  lt3->SetLineColor(kBlack);
  lt3->Draw("same");

  Double_t bin_center4 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(103);
  cout<<"4 "<<bin_center4<<endl;
  TLine *lt4  = new TLine(bin_center4, Ymin, bin_center4, Ymax);
  lt4->SetLineColor(kBlack);
  lt4->Draw("same");

  Double_t bin_center5 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(121);
  cout<<"5 "<<bin_center5<<endl;
  TLine *lt5  = new TLine(bin_center5, Ymin, bin_center5, Ymax);
  lt5->SetLineColor(kBlack);
  lt5->Draw("same");

  Double_t bin_center6 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(148);
  cout<<"6 "<<bin_center6<<endl;
  TLine *lt6  = new TLine(bin_center6, Ymin, bin_center6, Ymax);
  lt6->SetLineColor(kBlack);
  lt6->Draw("same");
  /*
  Double_t bin_center7 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(152);
  cout<<"7 "<<bin_center7<<endl;
  TLine *lt7  = new TLine(bin_center7, Ymin, bin_center7, Ymax);
  lt7->SetLineColor(kBlack);
  lt7->Draw("same");
  */
  auto legt = new TLegend(0.7,0.7,0.30,0.9); // 0.1 0.7
  legt->SetHeader("All #epsilon at Q^{2} = 0.425 GeV^{2}","C");
  legt->AddEntry(htheC,"High epsilon (5)","l");
  legt->AddEntry(htmeC,"Mid epsilon (3)","l");
  legt->AddEntry(htC,"Low epsilon (3)","l");
  legt->Draw("same");

  c13->Print(outputpdf);

  //Phi for all three e
  TCanvas *c14 = new TCanvas("c14", " c14");
  //High epsilon  

  hph_qheC->SetStats(0);
  hph_qheC->SetLineColor(kGreen);
  hph_qheC->Draw("Weight");

  hph_qheL2->SetStats(0);
  hph_qheL2->SetLineColor(kGreen);
  hph_qheL2->Draw("same Weight");
  
  hph_qheL1->SetStats(0);
  hph_qheL1->SetLineColor(kGreen);
  hph_qheL1->Draw("same Weight");
 
  hph_qheR1->SetStats(0);
  hph_qheR1->SetLineColor(kGreen);
  hph_qheR1->Draw("same Weight");

  hph_qheR2->SetStats(0);
  hph_qheR2->SetLineColor(kYellow);
  hph_qheR2->Draw("same Weight");

  //Mid epsilon
  
  hph_qmeC->SetStats(0);
  hph_qmeC->SetLineColor(kBlue);
  hph_qmeC->Draw("same Weight");

  hph_qmeL2->SetStats(0);
  hph_qmeL2->SetLineColor(kBlue);
  hph_qmeL2->Draw("same Weight");
  
  hph_qmeL1->SetStats(0);
  hph_qmeL1->SetLineColor(kBlue);
  hph_qmeL1->Draw("same Weight");
 
  //Low epsilon

  hph_qL2->SetStats(0);
  hph_qL2->SetLineColor(kRed);
  hph_qL2->Draw("same Weight");

  hph_qL1->SetStats(0);
  hph_qL1->SetLineColor(kRed);
  hph_qL1->Draw("same Weight");

  hph_qC->SetStats(0);
  hph_qC->SetLineColor(kRed);
  hph_qC->Draw("same Weight");

  //Check phi edges
  /**
     Double_t Phiymin = 0.0;
     Double_t Phiymax = 12750.0;
  
     Double_t fstphi1 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(4);
     TLine *fstphi1l = new TLine(fstphi1, Phiymin, fstphi1, Phiymax);
     fstphi1l->SetLineColor(kBlack);
     fstphi1l->Draw("same");

     Double_t fstphi2 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(5);
     TLine *fstphi2l = new TLine(fstphi2, Phiymin, fstphi2, Phiymax);
     fstphi2l->SetLineColor(kBlack);
     fstphi2l->Draw("same");

     Double_t fstphi3 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(6);
     TLine *fstphi3l = new TLine(fstphi3, Phiymin, fstphi3, Phiymax);
     fstphi3l->SetLineColor(kBlack);
     fstphi3l->Draw("same");

     Double_t fstphi4 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(7);
     TLine *fstphi4l = new TLine(fstphi4, Phiymin, fstphi4, Phiymax);
     fstphi4l->SetLineColor(kBlack);
     fstphi4l->Draw("same");

     Double_t fstphi5 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(8);
     TLine *fstphi5l = new TLine(fstphi5, Phiymin, fstphi5, Phiymax);
     fstphi5l->SetLineColor(kBlack);
     fstphi5l->Draw("same");

     Double_t fstphi6 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(9);
     TLine *fstphi6l = new TLine(fstphi6, Phiymin, fstphi6, Phiymax);
     fstphi6l->SetLineColor(kBlack);
     fstphi6l->Draw("same");

     Double_t fstphi7 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(10);
     TLine *fstphi7l = new TLine(fstphi7, Phiymin, fstphi7, Phiymax);
     fstphi7l->SetLineColor(kBlack);
     fstphi7l->Draw("same");

     Double_t fstphi8 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(11);
     TLine *fstphi8l = new TLine(fstphi8, Phiymin, fstphi8, Phiymax);
     fstphi8l->SetLineColor(kBlack);
     fstphi8l->Draw("same");

     Double_t fstphi9 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(12);
     TLine *fstphi9l = new TLine(fstphi9, Phiymin, fstphi9, Phiymax);
     fstphi9l->SetLineColor(kBlack);
     fstphi9l->Draw("same");

     Double_t fstphi10 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(13);
     TLine *fstphi10l = new TLine(fstphi10, Phiymin, fstphi10, Phiymax);
     fstphi10l->SetLineColor(kBlack);
     fstphi10l->Draw("same");

     Double_t fstphi11 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(14);
     TLine *fstphi11l = new TLine(fstphi11, Phiymin, fstphi11, Phiymax);
     fstphi11l->SetLineColor(kBlack);
     fstphi11l->Draw("same");

     Double_t fstphi12 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(15);
     TLine *fstphi12l = new TLine(fstphi12, Phiymin, fstphi12, Phiymax);
     fstphi12l->SetLineColor(kBlack);
     fstphi12l->Draw("same");

     Double_t fstphi13 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(16);
     TLine *fstphi13l = new TLine(fstphi13, Phiymin, fstphi13, Phiymax);
     fstphi13l->SetLineColor(kBlack);
     fstphi13l->Draw("same");

     Double_t fstphi14 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(17);
     TLine *fstphi14l = new TLine(fstphi14, Phiymin, fstphi14, Phiymax);
     fstphi14l->SetLineColor(kBlack);
     fstphi14l->Draw("same");

     Double_t fstphi15 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(18);
     TLine *fstphi15l = new TLine(fstphi15, Phiymin, fstphi15, Phiymax);
     fstphi15l->SetLineColor(kBlack);
     fstphi15l->Draw("same");

     Double_t fstphi16 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(19);
     TLine *fstphi16l = new TLine(fstphi16, Phiymin, fstphi16, Phiymax);
     fstphi16l->SetLineColor(kBlack);
     fstphi16l->Draw("same");

     Double_t fstphi17 = ((TAxis*)hph_qheC->GetXaxis())->GetBinLowEdge(20);
     TLine *fstphi17l = new TLine(fstphi17, Phiymin, fstphi17, Phiymax);
     fstphi17l->SetLineColor(kBlack);
     fstphi17l->Draw("same");
  **/
  c14->Print(outputpdf);

  TCanvas *canvas = new TCanvas("canvas", " ", 700, 700);
  canvas->Divide(2, 2);  
  canvas->cd(1);
  TPad* pad1 = (TPad*)canvas->GetPad(1);
  pad1->SetLeftMargin(0.13);
  pad1->SetBottomMargin(0.07);
  pad1->SetTopMargin(0.02);
  pad1->SetRightMargin(0.0);
  hQ2WC->SetStats(0);
  hQ2WC->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  canvas->cd(2);
  TPad* pad2 = (TPad*)canvas->GetPad(2);
  pad2->SetLeftMargin(0.07);
  pad2->SetBottomMargin(0.07);
  pad2->SetTopMargin(0.02);
  pad2->SetRightMargin(0.0);
  hQ2WMEC->SetStats(0);
  hQ2WMEC->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  canvas->cd(3);
  TPad* pad3 = (TPad*)canvas->GetPad(3);
  pad3->SetLeftMargin(0.13);
  pad3->SetBottomMargin(0.15);
  pad3->SetTopMargin(0.02);
  pad3->SetRightMargin(0.0);
  hQ2WHEC->SetStats(0);
  hQ2WHEC->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  canvas->cd(4);
  TPad* pad4 = (TPad*)canvas->GetPad(4);
  pad4->SetLeftMargin(0.07);
  pad4->SetBottomMargin(0.15);
  pad4->SetTopMargin(0.02);
  pad4->SetRightMargin(0.0);
  hQ2WHEC1->SetStats(0);
  hQ2WHEC1->Draw("COL");
  Dcut->SetLineColor(kRed);
  Dcut->Draw("same");

  // Set common axis properties
  canvas->cd();
  canvas->Update();

  // Add common axis labels
  /* TPad *axisPad = new TPad("axisPad", "axisPad", 0, 0, 1, 1);
  axisPad->Draw();
  axisPad->cd();
  axisPad->SetRightMargin(0.18);
  axisPad->SetLeftMargin(0.1);
  axisPad->SetTopMargin(0.05);
  axisPad->SetBottomMargin(0.13);
  axisPad->SetFillStyle(0);
  axisPad->SetBorderSize(0);
  axisPad->SetTickx(0);
  axisPad->SetTicky(0);
  */ 
 // Add common axis title and labels
  TLatex label;
  label.SetTextFont(42);
  label.SetTextSize(0.04);
  label.DrawLatex(0.5, 0.02, "Q^{2} (GeV^{2})");
  label.SetTextAngle(90);
  label.DrawLatex(0.04, 0.5, "W (GeV)");
  canvas->Print(outputpdf);

  TCanvas *Coincanvas = new TCanvas("Coincanvas", " ", 700, 700);
  Coincanvas->Divide(1, 3);  
  Coincanvas->cd(1);
  TPad* coinpad1 = (TPad*)Coincanvas->GetPad(1);
  coinpad1->SetLeftMargin(0.07);
  coinpad1->SetBottomMargin(0.04);
  coinpad1->SetTopMargin(0.02);
  coinpad1->SetRightMargin(0.0);
  htcoinC->SetStats(0);
  htcoinC->Draw("Weight");
  htcoinL1->Draw("same Weight");
  htcoinL2->Draw("same Weight");
  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");
  l4->Draw("same");
  l5->Draw("same");
  l6->Draw("same");
  leg->Draw("same");
  Coincanvas->cd(2);

  TPad* coinpad2 = (TPad*)Coincanvas->GetPad(2);
  coinpad2->SetLeftMargin(0.07);
  coinpad2->SetBottomMargin(0.04);
  coinpad2->SetTopMargin(0.02);
  coinpad2->SetRightMargin(0.0);
  htcoinmeC->SetStats(0);
  htcoinmeC->Draw("Weight");
  htcoinmeL1->Draw("same Weight");
  htcoinmeL2->Draw("same Weight");
  l1me->Draw("same");
  l2me->Draw("same");
  l3me->Draw("same");
  l4me->Draw("same");
  l5me->Draw("same");
  l6me->Draw("same");
  legme->Draw("same");

  Coincanvas->cd(3);
  TPad* coinpad3 = (TPad*)Coincanvas->GetPad(3);
  coinpad3->SetLeftMargin(0.07);
  coinpad3->SetBottomMargin(0.07);
  coinpad3->SetTopMargin(0.02);
  coinpad3->SetRightMargin(0.0);
  htcoinheC->SetStats(0);
  htcoinheL1->Draw("Weight");
  htcoinheR2->Draw("same Weight");
  htcoinheC->Draw("same Weight");
  htcoinheL2->Draw("same Weight");
  htcoinheR1->Draw("same Weight");
  l1he->Draw("same");
  l2he->Draw("same");
  l3he->Draw("same");
  l4he->Draw("same");
  l5he->Draw("same");
  l6he->Draw("same");
  leghe->Draw("same");
  Coincanvas->cd();

  Coincanvas->Update();
 // Add common axis title and labels
  TLatex label1;
  label1.SetTextFont(42);
  label1.SetTextSize(0.03);
  label1.DrawLatex(0.5, 0.008, "Coincidence time (ns)");
  label1.SetTextAngle(90);
  label1.DrawLatex(0.04, 0.5, "Yield (Events/mC) ");
  Coincanvas->Print(outputpdf);

  TCanvas *tcshheR1 = new TCanvas("tcshheR1", " ", 700, 700);
  tcshheR1->SetLeftMargin(0.13);
  tcshheR1->SetBottomMargin(0.10);
  tcshheR1->SetTopMargin(0.02);
  tcshheR1->SetRightMargin(0.10);
  shheR1->SetStats(0);
  shheR1->Draw("colz");
  //  tcshheR1->Print(outputpdf);

  TCanvas *tcshheR2 = new TCanvas("tcshheR2", " ", 700, 700);
  tcshheR2->SetLeftMargin(0.13);
  tcshheR2->SetBottomMargin(0.10);
  tcshheR2->SetTopMargin(0.02);
  tcshheR2->SetRightMargin(0.10);
  shheR2->SetStats(0);
  shheR2->Draw("colz");
  //  tcshheR2->Print(outputpdf);

  TCanvas *tcshheC = new TCanvas("tcshheC", " ", 700, 700);
  tcshheC->SetLeftMargin(0.13);
  tcshheC->SetBottomMargin(0.10);
  tcshheC->SetTopMargin(0.02);
  tcshheC->SetRightMargin(0.10);
  shheC->SetStats(0);
  shheC->Draw("colz");
  // tcshheC->Print(outputpdf);

  TCanvas *tcshheL1 = new TCanvas("tcshheL1", " ", 700, 700);
  tcshheL1->SetLeftMargin(0.13);
  tcshheL1->SetBottomMargin(0.10);
  tcshheL1->SetTopMargin(0.02);
  tcshheL1->SetRightMargin(0.10);
  shheL1->SetStats(0);
  shheL1->Draw("colz");
  // tcshheL1->Print(outputpdf);

  TCanvas *tcshheL2 = new TCanvas("tcshheL2", " ", 700, 700);
  tcshheL2->SetLeftMargin(0.13);
  tcshheL2->SetBottomMargin(0.10);
  tcshheL2->SetTopMargin(0.02);
  tcshheL2->SetRightMargin(0.10);
  shheL2->SetStats(0);
  shheL2->Draw("colz");
  //tcshheL2->Print(outputpdf);

  TCanvas *tcshhe = new TCanvas("tcshhe", " ", 700, 700);
  tcshhe->SetLeftMargin(0.13);
  tcshhe->SetBottomMargin(0.10);
  tcshhe->SetTopMargin(0.02);
  tcshhe->SetRightMargin(0.13);
  TH2D *h_sum = new TH2D(*shheC);
  h_sum->Add(shheL2);
  h_sum->Add(shheL1);
  h_sum->Add(shheR2);
  h_sum->Add(shheR1);
  h_sum->Draw("COLZ");
  double customRadii[] = {bins[1],bins[2],bins[3],bins[4],bins[5],bins[6],bins[7],bins[8]};
  int customColors[] = {kRed,kRed,kRed,kRed,kRed,kRed,kRed,kRed,};

  int nCircles = 8;
  double maxRadius = bins[8];
  double angle = 45.0; 
  CircleDrawer drawer2(h_sum, nCircles, maxRadius, angle);
  drawer2.SetRadii(customRadii, 8);
  drawer2.SetColors(customColors, 8);
  // drawer2.SetAngle(30.0); // 30 degree angle
  //  drawer2.SetAngle(0.0); // 30 degree angle
  drawer2.DrawCirclesWithRadii();
    
  tcshhe->Print(outputpdf);

  TCanvas *tcshme = new TCanvas("tcshme", " ", 700, 700);
  tcshme->SetLeftMargin(0.13);
  tcshme->SetBottomMargin(0.10);
  tcshme->SetTopMargin(0.02);
  tcshme->SetRightMargin(0.13);
  TH2D *h_sum_me = new TH2D(*shmeC);
  h_sum_me->SetStats(0);
  h_sum_me->Add(shmeL2);
  h_sum_me->Add(shmeL1);
  h_sum_me->Draw("COLZ");
  CircleDrawer drawer21(h_sum_me, nCircles, maxRadius, angle);
  drawer21.SetRadii(customRadii, 8);
  drawer21.SetColors(customColors, 8);
  drawer21.DrawCirclesWithRadii();
  
  tcshme->Print(outputpdf);

  TCanvas *tcshle = new TCanvas("tcshle", " ", 700, 700);
  tcshle->SetLeftMargin(0.13);
  tcshle->SetBottomMargin(0.10);
  tcshle->SetTopMargin(0.02);
  tcshle->SetRightMargin(0.13);
  TH2D *h_sum_le = new TH2D(*shleC);
  h_sum_le->SetStats(0);
  h_sum_le->Add(shleL2);
  h_sum_le->Add(shleL1);
  h_sum_le->Draw("COLZ");
  CircleDrawer drawer22(h_sum_le, nCircles, maxRadius, angle);
  drawer22.SetRadii(customRadii, 8);
  drawer22.SetColors(customColors, 8);
  drawer22.DrawCirclesWithRadii();
  
  tcshle->Print(outputpdf + ')');
  
  ///The end

}
