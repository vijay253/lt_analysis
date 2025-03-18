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

void All_Diamonds()
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

    ROOTfilePath = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2";
    OutPath =  "/u/group/c-kaonlt/USERS/vijay/lt_analysis/Q20p42_Analysis/output";
      
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
  TString foutname   = OutPath+"/" + "Analysed_Qp425W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Analysed_Qp425W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Analysed_Qp425W2p2" + ".pdf";

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

  Double_t WC;TBRANCHC->SetBranchAddress("W", &WC);
  Double_t Q2C;TBRANCHC->SetBranchAddress("Q2", &Q2C);
  //dummy
  Double_t WdC;TBRANCHDC->SetBranchAddress("W", &WdC);
  Double_t Q2dC;TBRANCHDC->SetBranchAddress("Q2", &Q2dC);
  Double_t  H_gtr_dpd;TBRANCHDC->SetBranchAddress("hsdelta", &H_gtr_dpd);


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
  Double_t bins[] = {-0.01, 0.00705, 0.0132833, 0.0173167, 0.0220833, 0.0275833, 0.0341833, 0.0440833, 0.06315, 0.0721167, 0.1};
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

  /*
  TH2D *hYSC   = new TH2D("hYSC"," Yield SIMC C;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYSL1   = new TH2D("hYSL1"," Yield SIMC L1;t-Bin; Phi-Bin ", 10 , bins, 18, PhiBinLow, PhiBinHigh);      
  TH2D *hYSL2   = new TH2D("hYSL2"," Yield SIMC L2;t-Bin; Phi-Bin ", 10, bins, 18, PhiBinLow, PhiBinHigh);      
  */
  //SIMC and Data HISTOGRAMS
  //CENTER
  TH1D *hphC  = new TH1D("hphC","ph_q; ph_q;",50, -10, 370);
  TH1D *hphCR  = new TH1D("hphCR","ph_q; ph_q;",  50, -10, 370);
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",50, -10, 370);
  /*
  TH1D *hphC  = new TH1D("hphC","ph_q; ph_q;",150,  PhiBinLow, PhiBinHigh); //0.91625, 0.98375     
  TH1D *hphCR  = new TH1D("hphCR","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",150, PhiBinLow, PhiBinHigh);      
  */
  /*
  TH1D *hphC  = new TH1D("hphC","ph_q; ph_q;",150,  0.918,0.982); //0.91625, 0.98375     
  TH1D *hphCR  = new TH1D("hphCR","ph_q; ph_q;",150, 0.918,0.982);      
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",150, 0.918,0.982);      
  */
  TH1D *hthC  = new TH1D("hthC","th_q; th_q;",   50, -0.01, 5.0);
  TH1D *hthCR  = new TH1D("hthCR","th_q; th_q;",  50, -0.01, 5.0);
  TH1D *hthSC  = new TH1D("hth_qC","th_q; th_q;",   50, -0.01, 5.0);

  TH1D *H_ssdelta_SC  = new TH1D("H_ssdelta_SC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SC  = new TH1D("H_ssxptar_SC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SC  = new TH1D("H_ssyptar_SC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DC  = new TH1D("H_ssdelta_DC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DC  = new TH1D("H_ssxptar_DC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DC  = new TH1D("H_ssyptar_DC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_ssdelta_DCR  = new TH1D("H_ssdelta_DCR","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_DCR  = new TH1D("H_ssxptar_DCR","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_DCR  = new TH1D("H_ssyptar_DCR","SHMS yptar; ssyptar;", 50, -0.04, 0.04);

  TH1D *H_hsdelta_SC  = new TH1D("H_hsdelta_SC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SC  = new TH1D("H_hsxptar_SC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SC  = new TH1D("H_hsyptar_SC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DC  = new TH1D("H_hsdelta_DC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DC  = new TH1D("H_hsxptar_DC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DC  = new TH1D("H_hsyptar_DC","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DCR  = new TH1D("H_hsdelta_DCR","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_DCR  = new TH1D("H_hsxptar_DCR","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_DCR  = new TH1D("H_hsyptar_DCR","HMS yptar; hsyptar;", 50, -0.05, 0.05);

  TH1D *H_hsdelta_DummyC  = new TH1D("H_hsdelta_DummyC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsdelta_DummyCR  = new TH1D("H_hsdelta_DummyCR","HMS Delta; hsdelta;", 50, -20.0, 20.0);

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

  Dcut->SetPoint(0,0.407515,2.18602);Dcut->SetPoint(1,0.348397,2.22735);
  Dcut->SetPoint(2,0.436072,2.20831); Dcut->SetPoint(3,0.501202,2.16576);
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
  // Double_t tOffsetC  = 0.00;
  Double_t tOffsetL2  = 0.00;

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
	  H_ssdelta_DC->Fill(P_gtr_dp);
	  H_ssxptar_DC->Fill(P_gtr_xptar);
	  H_ssyptar_DC->Fill(P_gtr_yptar);

	  H_hsdelta_DC->Fill(H_gtr_dp);
	  H_hsxptar_DC->Fill(H_gtr_xptar);
	  H_hsyptar_DC->Fill(H_gtr_yptar);
	  hQ2WC1->Fill(Q2C, WC);

	  hthtC->Fill(-tC+tOffsetC);
	  htC->Fill(-tC);
	  htInC->Fill(-tC);
	  hph_qC->Fill(ph_qC*57.2958 + 180);
	  hphC->Fill(ph_qC*57.2958 + 180);
	  //  hphC->Fill(mmC+MMpiOffsetC);
	  hYC->Fill(-tC+tOffsetC, ph_qC*57.2958 + 180);
	  //hYC->Fill(-tC, mmC+MMpiOffsetC);   //0.003492
	  hthC->Fill(th_qC*57.2958);
	  
	}
      //Random
      if(CoinPionCR && DiamondC)
	{
	  MPiCR->Fill(mmC+MMpiOffsetC);
	  
	}
      if(CoinPionCR && MMpiC && DiamondC)
	{
	  H_ssdelta_DCR->Fill(P_gtr_dp);
	  H_ssxptar_DCR->Fill(P_gtr_xptar);
	  H_ssyptar_DCR->Fill(P_gtr_yptar);

	  H_hsdelta_DCR->Fill(H_gtr_dp);
	  H_hsxptar_DCR->Fill(H_gtr_xptar);
	  H_hsyptar_DCR->Fill(H_gtr_yptar);

	  hthtCR->Fill(-tC+tOffsetC);
	  htCR->Fill(-tC);
	  htInCR->Fill(-tC);
	  hph_qCR->Fill(ph_qC*57.2958 + 180);
	  hphCR->Fill(ph_qC*57.2958 + 180);
	  //  hphCR->Fill(mmC+MMpiOffsetC);
	  hQ2WCR->Fill(Q2C, WC);
	  hYCR->Fill(-tC+tOffsetC, ph_qC*57.2958 + 180);
	  //hYCR->Fill(-tC, mmC+MMpiOffsetC);
	  hthCR->Fill(th_qC*57.2958);
	  
	}

      //For kin_aver, Q2, W & p_theta
      Double_t w1t = -tC+tOffsetC >= bins[1] && -tC+tOffsetC <= bins[2];
      Double_t w2t = -tC+tOffsetC >= bins[2] && -tC+tOffsetC <= bins[3];
      Double_t w3t = -tC+tOffsetC >= bins[3] && -tC+tOffsetC <= bins[4];
      Double_t w4t = -tC+tOffsetC >= bins[4] && -tC+tOffsetC <= bins[5];
      Double_t w5t = -tC+tOffsetC >= bins[5] && -tC+tOffsetC <= bins[6];
      Double_t w6t = -tC+tOffsetC >= bins[6] && -tC+tOffsetC <= bins[7];
      Double_t w7t = -tC+tOffsetC >= bins[7] && -tC+tOffsetC <= bins[8];
      Double_t w8t = -tC+tOffsetC >= bins[8] && -tC+tOffsetC <= bins[9];

      if(CoinPionC && MMpiC && DiamondC && w1t)
	{
	  hW1C->Fill(WC);
	  hQ21C->Fill(Q2C);
	  ht1C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w2t)
	{
	  hW2C->Fill(WC);
	  hQ22C->Fill(Q2C);
	  ht2C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w3t)
	{
	  hW3C->Fill(WC);
	  hQ23C->Fill(Q2C);
	  ht3C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w4t)
	{
	  hW4C->Fill(WC);
	  hQ24C->Fill(Q2C);
	  ht4C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w5t)
	{
	  hW5C->Fill(WC);
	  hQ25C->Fill(Q2C);
	  ht5C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w6t)
	{
	  hW6C->Fill(WC);
	  hQ26C->Fill(Q2C);
	  ht6C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w7t)
	{
	  hW7C->Fill(WC);
	  hQ27C->Fill(Q2C);
	  ht7C->Fill(-tC);
	  
	}
      if(CoinPionC && MMpiC && DiamondC && w8t)
	{
	  hW8C->Fill(WC);
	  hQ28C->Fill(Q2C);
	  ht8C->Fill(-tC);
	  
	}

      //Random
      if(CoinPionCR && MMpiC && DiamondC && w1t)
	{
	  hW1CR->Fill(WC);
	  hQ21CR->Fill(Q2C);
	  ht1CR->Fill(-tC);
	}

      if(CoinPionCR && MMpiC && DiamondC && w2t)
	{
	  hW2CR->Fill(WC);
	  hQ22CR->Fill(Q2C);
	  ht2CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w3t)
	{
	  hW3CR->Fill(WC);
	  hQ23CR->Fill(Q2C);
	  ht3CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w4t)
	{
	  hW4CR->Fill(WC);
	  hQ24CR->Fill(Q2C);
	  ht4CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w5t)
	{
	  hW5CR->Fill(WC);
	  hQ25CR->Fill(Q2C);
	  ht5CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w6t)
	{
	  hW6CR->Fill(WC);
	  hQ26CR->Fill(Q2C);
	  ht6CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w7t)
	{
	  hW7CR->Fill(WC);
	  hQ27CR->Fill(Q2C);
	  ht7CR->Fill(-tC);
	}
      if(CoinPionCR && MMpiC && DiamondC && w8t)
	{
	  hW8CR->Fill(WC);
	  hQ28CR->Fill(Q2C);
	  ht8CR->Fill(-tC);
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
	  hYdC->Fill(-tdC+tOffsetC, ph_qdC*57.2958 + 180);
	  //hYdC->Fill(-tdC, mmdC+MMpiOffsetC);
	  hthtdC->Fill(-tdC+tOffsetC);
	  H_hsdelta_DummyC->Fill(H_gtr_dpd);

	  
	}

      //Random
      Double_t CUTCCR = ((tcoindC-TimmingOffset>=-15.0 && tcoindC-TimmingOffset <= -9.0) || (tcoindC-TimmingOffset>=7.0 && tcoindC-TimmingOffset <=13.0));

      if(CoinPiondCR && DiamonddC)
	{
	  MPidCR->Fill(mmdC+MMpiOffsetC);
	  
	}
      if(CoinPiondCR && MMpidC && DiamonddC)
	{
	  hYdCR->Fill(-tdC+tOffsetC, ph_qdC*57.2958 + 180);
	  //hYdCR->Fill(-tdC, mmdC+MMpiOffsetC);
	  hthtdCR->Fill(-tdC+tOffsetC);
	  H_hsdelta_DummyCR->Fill(H_gtr_dpd);

	  
	}
          
    }

  //SIMC CENTER
  for(Long64_t i = 0; i < nEntries_TSIMCC; i++)

    {
      TSIMCC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simc, W_simc));

      Double_t fact = 9942030.0/400000.0;
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

      //            Double_t CUTSIMCC = hsxpfp >=-0.08 && hsxpfp <=0.08 && hsypfp >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && ssdelta <=20.0 && ssxpfp >=-0.06 && ssxpfp <=0.06 && ssypfp >=-0.04
      && ssypfp <=0.04 && missmass >= 0.91 && missmass <= 0.98 && Diamond_cut;

      Double_t CUTSIMCC = (hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H) && (hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H) && (hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_
																			 Yptar_High_H) && (ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P) && (ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P) && (ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_Hig
																																					 h_P) && (missmass >=MMPICUTL && missmass <=MMPICUTH) && Diamond_cut;

            Double_t CUTSIMCC2 = hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H && hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H && hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Ypta
	      r_High_H && ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P && ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P && ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P && Dia
	      mond_cut;

	          Double_t CUTSIMCC3 = hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H && hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H && hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Ypta
		    r_High_H && ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P && ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P && ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P;

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
		      //hphSC->Fill(missmass, fact*Weight);
		      hYSC->Fill(t_simc, phipq*57.2958, fact*Weight);
		      //hYSC->Fill(t_simc, missmass, fact*Weight);
		      H_ssdelta_SC->Fill(ssdelta, fact*Weight);
		      H_ssxptar_SC->Fill(ssxptar, fact*Weight);
		      H_ssyptar_SC->Fill(ssyptar, fact*Weight);

		      H_hsdelta_SC->Fill(hsdelta, fact*Weight);
		      H_hsxptar_SC->Fill(hsxptar, fact*Weight);
		      H_hsyptar_SC->Fill(hsyptar, fact*Weight);

		      H_t_RE->Fill((t_simc-ti_simc), fact*Weight);
		      H_t_RE_t->Fill((t_simc-ti_simc), t_simc, fact*Weight);
		      hthtSC->Fill(t_simc, fact*Weight);

		      hQ2WSC1->Fill(Q2_simc, W_simc, fact*Weight);


		      
		    }  
    }
}
