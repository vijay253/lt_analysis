// .... Created Date: March 18, 2025 ....
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
  TString foutname   = OutPath+"/" + "Diamond_Qp425W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Diamond_Qp425W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Diamond_Qp425W2p2" + ".pdf";

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
  //DATA HISTOGRAMS

  Double_t hisbin = 300;

  Double_t hisxaxisl = 0.3;
  Double_t hisxaxish = 0.55;

  Double_t hisyaxisl = 2.14;
  Double_t hisyaxish = 2.26;
  
  TH2D *hQ2WC  = new TH2D("hQ2WC","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);

  TH2D *hQ2WCR  = new TH2D("hQ2WCR","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);
  TH2D *hQ2WC1R  = new TH2D("hQ2WC1R","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);

  //SIMC
  TH2D *hQ2WSC  = new TH2D("hQ2WSC","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);
  TH2D *hQ2WSC1  = new TH2D("hQ2WSC1","; ; ", hisbin, hisxaxisl, hisxaxish, hisbin, hisyaxisl, hisyaxish);


 
  TCutG *Dcut = new TCutG("Dcut",5);
  Dcut->SetVarX("Q2");
  Dcut->SetVarY("W");

  Dcut->SetPoint(0,0.407515,2.18602);Dcut->SetPoint(1,0.348397,2.22735);
  Dcut->SetPoint(2,0.436072,2.20831); Dcut->SetPoint(3,0.501202,2.16576);
  Dcut->SetPoint(4,0.407515,2.18602);

  //////////////////////////////
  Double_t MMPICUTL = 0.91;
  Double_t MMPICUTH = 0.98;
  /////////////////////////////
  Double_t TimmingOffset = 44.1;
  
  //DATA CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);

      if(tcoinC -TimmingOffset >=-1.0 && tcoinC -TimmingOffset <= 1.0 && mmC>= MMPICUTL && mmC <=MMPICUTH)
	{
	  hQ2WC->Fill(Q2C, WC);
	  
	}

      Double_t DiamondC = (Dcut->IsInside(Q2C, WC));
      Double_t MMpiC = mmC>= MMPICUTL && mmC <=MMPICUTH;
      Double_t CoinPionC = tcoinC-TimmingOffset>=-1.0 && tcoinC-TimmingOffset <= 1.0;
      Double_t CoinPionCR = ((tcoinC-TimmingOffset>=-15.0 && tcoinC-TimmingOffset<= -9.0) || (tcoinC-TimmingOffset>=7.0 && tcoinC-TimmingOffset<=13.0));

      if(CoinPionC && MMpiC && DiamondC)
	{
	  hQ2WC1->Fill(Q2C, WC);
	  
	}
    }
  
  //SIMC CENTER
  for(Long64_t i = 0; i < nEntries_TSIMCC; i++)
    
    {
      TSIMCC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simc, W_simc));

      Double_t CUTSIMCC = hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H && hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H && hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Yptar_High_H && ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P && ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P && ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P;
      
      Double_t fact = 9942030.0/400000.0;

      if (CUTSIMCC)
	{
	  hQ2WSC->Fill(Q2_simc, W_simc, fact*Weight);	  
	}	  
    }
  
TCanvas *c = new TCanvas("c", " c");

Dcut->SetLineColor(kRed);
hQ2WC->Draw("COL");
Dcut->Draw("same");
// hQ2WC1->SetStats(0);
//Dcut->SetLineColor(kRed);
//hQ2WC1->Draw("COL");
//Dcut->Draw("same");
c->Print(outputpdf + '(');

TCanvas *cs = new TCanvas("cs", " cs");

Dcut->SetLineColor(kRed);
hQ2WSC->Draw("COL");
Dcut->Draw("same");
cs->Print(outputpdf + ')');

}
