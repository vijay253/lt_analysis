// .... Created Date: Feb 01, 2023 ....
// .... Author: VK ....
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


void t_plots()
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
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/KaonLT/";
    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/KaonLT/";
  }
  else if(Hostname.Contains("qcd")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    OutPath = Replaypath+"/UTIL_KAONLT/scripts/CoinTimePeak/OUTPUT";
  }
  else if (Hostname.Contains("phys.uregina.ca")){
    ROOTfilePath = "/home/vijay/work/HeepCoinStudy/";
    OutPath = "/home/vijay/work/HeepCoinStudy/";
  }

  // Low epsilon analysis
  cout<<" Analysing low epsilon data..." <<endl;
  //Data  
  TString TInDATAFilenameC = "Analysed_Data_Q0p375W2p2_lowe_Center.root";
  TString TInDATAFilenameL1 = "Analysed_Data_Q0p375W2p2_lowe_Left1.root";
  TString TInDATAFilenameL2 = "Analysed_Data_Q0p375W2p2_lowe_Left2.root";
 
  TString rootFile_DATAC  = ROOTfilePath+"/"+TInDATAFilenameC;
  TString rootFile_DATAL1 = ROOTfilePath+"/"+TInDATAFilenameL1;
  TString rootFile_DATAL2 = ROOTfilePath+"/"+TInDATAFilenameL2;
  //SIMC

  TString TInSIMCFilenameC = "Pion_2p7_Q1_center.root";
  TString TInSIMCFilenameL1 = "Pion_2p7_Q1_left1.root";
  TString TInSIMCFilenameL2 = "Pion_2p7_Q1_left2.root";

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
  //SIMC

  TFile *InFile_SIMCC = new TFile(rootFile_SIMCC, "READ");
  TFile *InFile_SIMCL1 = new TFile(rootFile_SIMCL1, "READ");
  TFile *InFile_SIMCL2 = new TFile(rootFile_SIMCL2, "READ");

  //Output file names 
  TString foutname   = OutPath+"/" + "Analysed_Qp375W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Analysed_Qp375W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Analysed_Qp375W2p2" + ".pdf";

  //Data
  TTree* TBRANCHC  = (TTree*)InFile_DATAC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHC  = (Long64_t)TBRANCHC->GetEntries();  
  TTree* TBRANCHL1 = (TTree*)InFile_DATAL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL1  = (Long64_t)TBRANCHL1->GetEntries();  
  TTree* TBRANCHL2 = (TTree*)InFile_DATAL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL2  = (Long64_t)TBRANCHL2->GetEntries();  
  //SIMC  
  TTree* TSIMCC  = (TTree*)InFile_SIMCC->Get("h10");Long64_t nEntries_TSIMCC  = (Long64_t)TSIMCC->GetEntries();  
  TTree* TSIMCL1  = (TTree*)InFile_SIMCL1->Get("h10");Long64_t nEntries_TSIMCL1  = (Long64_t)TSIMCL1->GetEntries();  
  TTree* TSIMCL2  = (TTree*)InFile_SIMCL2->Get("h10");Long64_t nEntries_TSIMCL2  = (Long64_t)TSIMCL2->GetEntries();  

  //DATA VARIABLES

  Double_t WC;TBRANCHC->SetBranchAddress("W", &WC);
  Double_t Q2C;TBRANCHC->SetBranchAddress("Q2", &Q2C);
 
  Double_t WL1;TBRANCHL1->SetBranchAddress("W", &WL1);
  Double_t Q2L1;TBRANCHL1->SetBranchAddress("Q2", &Q2L1);
 
  Double_t WL2;TBRANCHL2->SetBranchAddress("W", &WL2);
  Double_t Q2L2;TBRANCHL2->SetBranchAddress("Q2", &Q2L2);
  
  Double_t tC;TBRANCHC->SetBranchAddress("MandelT", &tC);
  Double_t tL1;TBRANCHL1->SetBranchAddress("MandelT", &tL1);
  Double_t tL2;TBRANCHL2->SetBranchAddress("MandelT", &tL2);

  Double_t ph_qC;TBRANCHC->SetBranchAddress("ph_q", &ph_qC);
  Double_t ph_qL1;TBRANCHL1->SetBranchAddress("ph_q", &ph_qL1);
  Double_t ph_qL2;TBRANCHL2->SetBranchAddress("ph_q", &ph_qL2);

  Double_t mmC;TBRANCHC->SetBranchAddress("MM", &mmC);
  Double_t mmL1;TBRANCHL1->SetBranchAddress("MM", &mmL1);
  Double_t mmL2;TBRANCHL2->SetBranchAddress("MM", &mmL2);

  Double_t tcoinC;TBRANCHC->SetBranchAddress("CTime_ROC1", &tcoinC);
  Double_t tcoinL1;TBRANCHL1->SetBranchAddress("CTime_ROC1", &tcoinL1);
  Double_t tcoinL2;TBRANCHL2->SetBranchAddress("CTime_ROC1", &tcoinL2);

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

  //DATA HISTOGRAMS

  TH2D *hQ2WC  = new TH2D("hQ2WC","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH2D *hQ2WCR  = new TH2D("hQ2WCR","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1R  = new TH2D("hQ2WC1R","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

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

  Double_t bins[] = {-0.01, 0.00595, 0.0118167, 0.0151167, 0.01915, 0.02355, 0.0286833, 0.03565, 0.04555, 0.0701167, 0.1};

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

  TH1D *hph_qC  = new TH1D("hph_qC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qL1  = new TH1D("hph_qL1","ph_q; ph_q;", 18, -22.5, 382.5);      
  TH1D *hph_qL2  = new TH1D("hph_qL2","ph_q; ph_q;", 18, -22.5, 382.5);      

  TH1D *hph_qCR  = new TH1D("hph_qCR","ph_q; ph_q;",  18, -22.5, 382.5);      
  TH1D *hph_qL1R  = new TH1D("hph_qL1R","ph_q; ph_q;",18, -22.5, 382.5);      
  TH1D *hph_qL2R  = new TH1D("hph_qL2R","ph_q; ph_q;",18, -22.5, 382.5);      

  TH1D *hmmC  = new TH1D("hmmC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1  = new TH1D("hmmL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2  = new TH1D("hmmL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmCR  = new TH1D("hmmCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1R  = new TH1D("hmmL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2R  = new TH1D("hmmL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinC   = new TH1D("htcoinC","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinL1  = new TH1D("htcoinL1","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      
  TH1D *htcoinL2  = new TH1D("htcoinL2","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      

  TH2D *hYC   = new TH2D("hYC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYCR  = new TH2D("hYCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      

  TH2D *hYL1   = new TH2D("hYL1"," Yield L1;t-Bin; Phi-Bin ", 10 , bins, 18, -22.5, 382.5);      
  TH2D *hYL1R  = new TH2D("hYL1R"," Yield L1; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      

  TH2D *hYL2   = new TH2D("hYL2"," Yield L2;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYL2R  = new TH2D("hYL2R"," Yield L2; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      


  //SIMC and Data HISTOGRAMS
  //CENTER
  TH1D *H_ssdelta_SC  = new TH1D("H_ssdelta_SC","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_SC  = new TH1D("H_ssxptar_SC","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_SC  = new TH1D("H_ssyptar_SC","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_ssdelta_DC  = new TH1D("H_ssdelta_DC","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_DC  = new TH1D("H_ssxptar_DC","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_DC  = new TH1D("H_ssyptar_DC","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_hsdelta_SC  = new TH1D("H_hsdelta_SC","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_SC  = new TH1D("H_hsxptar_SC","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_SC  = new TH1D("H_hsyptar_SC","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  TH1D *H_hsdelta_DC  = new TH1D("H_hsdelta_DC","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_DC  = new TH1D("H_hsxptar_DC","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_DC  = new TH1D("H_hsyptar_DC","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  //LEFT1
  TH1D *H_ssdelta_SL1  = new TH1D("H_ssdelta_SL1","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_SL1  = new TH1D("H_ssxptar_SL1","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_SL1  = new TH1D("H_ssyptar_SL1","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_ssdelta_DL1  = new TH1D("H_ssdelta_DL1","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_DL1  = new TH1D("H_ssxptar_DL1","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_DL1  = new TH1D("H_ssyptar_DL1","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_hsdelta_SL1  = new TH1D("H_hsdelta_SL1","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_SL1  = new TH1D("H_hsxptar_SL1","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_SL1  = new TH1D("H_hsyptar_SL1","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  TH1D *H_hsdelta_DL1  = new TH1D("H_hsdelta_DL1","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_DL1  = new TH1D("H_hsxptar_DL1","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_DL1  = new TH1D("H_hsyptar_DL1","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  //LEFT2
  TH1D *H_ssdelta_SL2  = new TH1D("H_ssdelta_SL2","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_SL2  = new TH1D("H_ssxptar_SL2","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_SL2  = new TH1D("H_ssyptar_SL2","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_ssdelta_DL2  = new TH1D("H_ssdelta_DL2","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar_DL2  = new TH1D("H_ssxptar_DL2","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar_DL2  = new TH1D("H_ssyptar_DL2","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_hsdelta_SL2  = new TH1D("H_hsdelta_SL2","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_SL2  = new TH1D("H_hsxptar_SL2","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_SL2  = new TH1D("H_hsyptar_SL2","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  TH1D *H_hsdelta_DL2  = new TH1D("H_hsdelta_DL2","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar_DL2  = new TH1D("H_hsxptar_DL2","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_DL2  = new TH1D("H_hsyptar_DL2","HMS yptar; hsyptar;", 300, -0.05, 0.05);

  TH1D *H_t_S  = new TH1D("H_t_S","t; t;", 300, -0.01, 0.1); 
  TH1D *H_ti_S  = new TH1D("H_ti_S","ti; ti;", 300, -0.01, 0.1); 
  TH1D *H_t_RE  = new TH1D("H_t_RE"," t Resolution (t-ti); t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE1  = new TH1D("H_t_RE1"," t Resolution (t-ti) 1 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE2  = new TH1D("H_t_RE2"," t Resolution (t-ti) 2 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE3  = new TH1D("H_t_RE3"," t Resolution (t-ti) 3 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE4  = new TH1D("H_t_RE4"," t Resolution (t-ti) 4 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE5  = new TH1D("H_t_RE5"," t Resolution (t-ti) 5 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE6  = new TH1D("H_t_RE6"," t Resolution (t-ti) 6 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE7  = new TH1D("H_t_RE7"," t Resolution (t-ti) 7 bin; t Resolution;", 300, -0.03, 0.03); 
  TH1D *H_t_RE8  = new TH1D("H_t_RE8"," t Resolution (t-ti) 8 bin; t Resolution;", 300, -0.03, 0.03); 
  TH2D *H_t_RE_t = new TH2D("H_t_RE_t","t Resolution vs t; t Resolution; t ", 300, -0.02, 0.03, 300, 0.0, 0.04);      

  TCutG *Dcut = new TCutG("Dcut",5);
  Dcut->SetVarX("Q2");
  Dcut->SetVarY("W");
  Dcut->SetPoint(0,0.363783,2.18318);Dcut->SetPoint(1,0.311069,2.22644);  
  Dcut->SetPoint(2,0.37737,2.21136); Dcut->SetPoint(3,0.452314,2.16293);
  Dcut->SetPoint(4,0.363783,2.18318); 

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
  

  //DATA CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)

    {
      TBRANCHC->GetEntry(i);
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98)
	{
	  hQ2WC->Fill(Q2C, WC);
	}
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0)    
	{
	  hmmC->Fill(mmC);	  
	}	      
      
      {
	htcoinC->Fill(tcoinC);
      }
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2C, WC));  
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut)
	{
	  hQ2WC1->Fill(Q2C, WC);	      	  
  	  htC->Fill(-tC);
	  htInC->Fill(-tC);	
	  hph_qC->Fill(ph_qC*57.2958 + 180);
	  hYC->Fill(-tC, ph_qC*57.2958 + 180);
	}	
       
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut)
	{
	  htCR->Fill(-tC);
	  htInCR->Fill(-tC);	
	  hph_qCR->Fill(ph_qC*57.2958 + 180);
	  hQ2WCR->Fill(Q2C, WC);
	  hYCR->Fill(-tC, ph_qC*57.2958 + 180);
	}
      
     //For kin_aver, Q2, W & p_theta 
 
      //      Double_t w1t = -tC >= 0.00595 && -tC <= 0.0118167;
      Double_t w1t = -tC >= bins[1] && -tC <= bins[2];
      Double_t w2t = -tC >= bins[2] && -tC <= bins[3];
      Double_t w3t = -tC >= bins[3] && -tC <= bins[4];
      Double_t w4t = -tC >= bins[4] && -tC <= bins[5];
      Double_t w5t = -tC >= bins[5] && -tC <= bins[6];
      Double_t w6t = -tC >= bins[6] && -tC <= bins[7];
      Double_t w7t = -tC >= bins[7] && -tC <= bins[8];
      Double_t w8t = -tC >= bins[8] && -tC <= bins[9];

      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w1t)
	{
	  hW1C->Fill(WC);	
	  hQ21C->Fill(Q2C);	      	  
	  ht1C->Fill(-tC); 
 	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w2t)
	{
	  hW2C->Fill(WC);	
	  hQ22C->Fill(Q2C);	      	  
	  ht2C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w3t)
	{
	  hW3C->Fill(WC);	
	  hQ23C->Fill(Q2C);	      	  
	  ht3C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w4t)
	{
	  hW4C->Fill(WC);	
	  hQ24C->Fill(Q2C);	      	  
	  ht4C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w5t)
	{
	  hW5C->Fill(WC);	
	  hQ25C->Fill(Q2C);	      	  
	  ht5C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w6t)
	{
	  hW6C->Fill(WC);	
	  hQ26C->Fill(Q2C);	      	  
	  ht6C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w7t)
	{
	  hW7C->Fill(WC);	
	  hQ27C->Fill(Q2C);	      	  
	  ht7C->Fill(-tC); 
  	}	
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w8t)
	{
	  hW8C->Fill(WC);	
	  hQ28C->Fill(Q2C);	      	  
 	  ht8C->Fill(-tC); 
 	}	

      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w1t)
      	{	  
	  hW1CR->Fill(WC);	
	  hQ21CR->Fill(Q2C);	      	  
	  ht1CR->Fill(-tC); 
	}      

      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w2t)
      	{	  
	  hW2CR->Fill(WC);	
	  hQ22CR->Fill(Q2C);	      	  
	  ht2CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w3t)
      	{	  
	  hW3CR->Fill(WC);	
	  hQ23CR->Fill(Q2C);	      	  
	  ht3CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w4t)
      	{	  
	  hW4CR->Fill(WC);	
	  hQ24CR->Fill(Q2C);	      	  
	  ht4CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w5t)
      	{	  
	  hW5CR->Fill(WC);	
	  hQ25CR->Fill(Q2C);	      	  
	  ht5CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w6t)
      	{	  
	  hW6CR->Fill(WC);	
	  hQ26CR->Fill(Q2C);	      	  
	  ht6CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w7t)
      	{	  
	  hW7CR->Fill(WC);	
	  hQ27CR->Fill(Q2C);	      	  
	  ht7CR->Fill(-tC); 
	}      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut && w8t)
      	{	  
	  hW8CR->Fill(WC);	
	  hQ28CR->Fill(Q2C);	      	  
	  ht8CR->Fill(-tC); 
	}      
      
      if (((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)))
	
	{
	  hmmCR->Fill(mmC);
	}	
    }  

  //SIMC CENTER

  for(Long64_t i = 0; i < nEntries_TSIMCC; i++)

    {
      TSIMCC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simc, W_simc));  

      Double_t fact = 10938400.0/400000.0;


      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxptar <=0.08 && hsyptar >=-0.045 && hsyptar <=0.045 && ssdelta >=-10.0 && ssdelta <=20.0 && ssxptar >=-0.06 && ssxptar <=0.06 && ssyptar >=-0.04 && hsyptar <=0.04 && missmass >= 0.92 && missmass <= 0.98 && Diamond_cut)
	
	{
	  H_ssdelta_SC->Fill(ssdelta, fact*Weight);
	  H_ssxptar_SC->Fill(ssxptar, fact*Weight);
	  H_ssyptar_SC->Fill(hsyptar, fact*Weight);

	  H_hsdelta_SC->Fill(hsdelta, fact*Weight);
	  H_hsxptar_SC->Fill(hsxptar, fact*Weight);
	  H_hsyptar_SC->Fill(hsyptar, fact*Weight);

	  H_t_RE->Fill((t_simc-ti_simc), fact*Weight);
	  H_t_RE_t->Fill((t_simc-ti_simc), t_simc, fact*Weight);
	}    

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.00595 && t_simc <= 0.0118167)
	{
	  H_t_RE1->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.0118167 && t_simc <= 0.0151167)
	{
	  H_t_RE2->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.0151167 && t_simc <= 0.01915)
	{
	  H_t_RE3->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.01915 && t_simc <= 0.02355)
	{
	  H_t_RE4->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.02355 && t_simc <= 0.0286833)
	{
	  H_t_RE5->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.0286833 && t_simc <= 0.03565)
	{
	  H_t_RE6->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.03565 && t_simc <= 0.04555)
	{
	  H_t_RE7->Fill((t_simc-ti_simc), fact*Weight);
	}

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98 && t_simc >= 0.04555 && t_simc <= 0.0800167)
	{
	  H_t_RE8->Fill((t_simc-ti_simc), fact*Weight);
	}
    }  
  
  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
  
    {
      TBRANCHL1->GetEntry(i);
            
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0)	
	{
	  hmmL1->Fill(mmL1);
	}           
      {
	htcoinL1->Fill(tcoinL1);
      }

      Double_t Diamond_cut = (Dcut->IsInside(Q2L1, WL1));  
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut)
	{
	  htL1->Fill(-tL1);
 	  htInL1->Fill(-tL1);
	  hph_qL1->Fill(ph_qL1*57.2958 + 180);	  
	  hYL1->Fill(-tL1, ph_qL1*57.2958 + 180);
	}      
      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut)	
	
	{
	  htL1R->Fill(-tL1);
 	  htInL1R->Fill(-tL1);
	  hph_qL1R->Fill(ph_qL1*57.2958 + 180);	  
	  hYL1R->Fill(-tL1, ph_qL1*57.2958 + 180);
	}
      
     //For kin_aver, Q2, W & p_theta 
 
      //      Double_t w1t = -tC >= 0.00595 && -tC <= 0.0118167;
      Double_t w1t = -tL1 >= bins[1] && -tL1 <= bins[2];
      Double_t w2t = -tL1 >= bins[2] && -tL1 <= bins[3];
      Double_t w3t = -tL1 >= bins[3] && -tL1 <= bins[4];
      Double_t w4t = -tL1 >= bins[4] && -tL1 <= bins[5];
      Double_t w5t = -tL1 >= bins[5] && -tL1 <= bins[6];
      Double_t w6t = -tL1 >= bins[6] && -tL1 <= bins[7];
      Double_t w7t = -tL1 >= bins[7] && -tL1 <= bins[8];
      Double_t w8t = -tL1 >= bins[8] && -tL1 <= bins[9];

      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w1t)
	{
	  hW1L1->Fill(WL1);	
	  hQ21L1->Fill(Q2L1);	      	  
	  ht1L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w2t)
	{
	  hW2L1->Fill(WL1);	
	  hQ22L1->Fill(Q2L1);	      	  
	  ht2L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w3t)
	{
	  hW3L1->Fill(WL1);	
	  hQ23L1->Fill(Q2L1);	      	  
	  ht3L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w4t)
	{
	  hW4L1->Fill(WL1);	
	  hQ24L1->Fill(Q2L1);	      	  
	  ht4L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w5t)
	{
	  hW5L1->Fill(WL1);	
	  hQ25L1->Fill(Q2L1);	      	  
	  ht5L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w6t)
	{
	  hW6L1->Fill(WL1);	
	  hQ26L1->Fill(Q2L1);	      	  
	  ht6L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w7t)
	{
	  hW7L1->Fill(WL1);	
	  hQ27L1->Fill(Q2L1);	      	  
	  ht7L1->Fill(-tL1); 
  	}	
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w8t)
	{
	  hW8L1->Fill(WL1);	
	  hQ28L1->Fill(Q2L1);	      	  
	  ht8L1->Fill(-tL1); 
  	}	

      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w1t)
      	{	  
	  hW1L1R->Fill(WL1);	
	  hQ21L1R->Fill(Q2L1);	      	  
	  ht1L1R->Fill(-tL1); 
	}      

      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w2t)
      	{	  
	  hW2L1R->Fill(WL1);	
	  hQ22L1R->Fill(Q2L1);	      	  
	  ht2L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w3t)
      	{	  
	  hW3L1R->Fill(WL1);	
	  hQ23L1R->Fill(Q2L1);	      	  
	  ht3L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w4t)
      	{	  
	  hW4L1R->Fill(WL1);	
	  hQ24L1R->Fill(Q2L1);	      	  
	  ht4L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w5t)
      	{	  
	  hW5L1R->Fill(WL1);	
	  hQ25L1R->Fill(Q2L1);	      	  
	  ht5L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w6t)
      	{	  
	  hW6L1R->Fill(WL1);	
	  hQ26L1R->Fill(Q2L1);	      	  
	  ht6L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w7t)
      	{	  
	  hW7L1R->Fill(WL1);	
	  hQ27L1R->Fill(Q2L1);	      	  
	  ht7L1R->Fill(-tL1); 
	}      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut && w8t)
      	{	  
	  hW8L1R->Fill(WL1);	
	  hQ28L1R->Fill(Q2L1);	      	  
	  ht8L1R->Fill(-tL1); 
	}      

      if (((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)))
	
	{	 
	  hmmL1R->Fill(mmL1);
	}     
    }
  
  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0)
	
	{
	  hmmL2->Fill(mmL2);
	}   
      
      {
	htcoinL2->Fill(tcoinL2);
      }

      Double_t Diamond_cut = (Dcut->IsInside(Q2L2, WL2));  
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut)
	{
	  htL2->Fill(-tL2);
 	  htInL2->Fill(-tL2);
	  hph_qL2->Fill(ph_qL2*57.2958 + 180);
	  hYL2->Fill(-tL2, ph_qL2*57.2958 + 180);
	}

      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut)
	{
	  htL2R->Fill(-tL2);
 	  htInL2R->Fill(-tL2);
	  hph_qL2R->Fill(ph_qL2*57.2958 + 180);
	  hYL2R->Fill(-tL2, ph_qL2*57.2958 + 180);
	}

     //For kin_aver, Q2, W & p_theta 
 
      //      Double_t w1t = -tC >= 0.00595 && -tC <= 0.0118167;
      Double_t w1t = -tL2 >= bins[1] && -tL2 <= bins[2];
      Double_t w2t = -tL2 >= bins[2] && -tL2 <= bins[3];
      Double_t w3t = -tL2 >= bins[3] && -tL2 <= bins[4];
      Double_t w4t = -tL2 >= bins[4] && -tL2 <= bins[5];
      Double_t w5t = -tL2 >= bins[5] && -tL2 <= bins[6];
      Double_t w6t = -tL2 >= bins[6] && -tL2 <= bins[7];
      Double_t w7t = -tL2 >= bins[7] && -tL2 <= bins[8];
      Double_t w8t = -tL2 >= bins[8] && -tL2 <= bins[9];

      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w1t)
	{
	  hW1L2->Fill(WL2);	
	  hQ21L2->Fill(Q2L2);	      	  
	  ht1L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w2t)
	{
	  hW2L2->Fill(WL2);	
	  hQ22L2->Fill(Q2L2);	      	  
	  ht2L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w3t)
	{
	  hW3L2->Fill(WL2);	
	  hQ23L2->Fill(Q2L2);	      	  
	  ht3L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w4t)
	{
	  hW4L2->Fill(WL2);	
	  hQ24L2->Fill(Q2L2);	      	  
	  ht4L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w5t)
	{
	  hW5L2->Fill(WL2);	
	  hQ25L2->Fill(Q2L2);	      	  
	  ht5L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w6t)
	{
	  hW6L2->Fill(WL2);	
	  hQ26L2->Fill(Q2L2);	      	  
	  ht6L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w7t)
	{
	  hW7L2->Fill(WL2);	
	  hQ27L2->Fill(Q2L2);	      	  
	  ht7L2->Fill(-tL2); 
  	}	
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w8t)
	{
	  hW8L2->Fill(WL2);	
	  hQ28L2->Fill(Q2L2);	      	  
	  ht8L2->Fill(-tL2); 
  	}	

      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w1t)
      	{	  
	  hW1L2R->Fill(WL2);	
	  hQ21L2R->Fill(Q2L2);	      	  
	  ht1L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w2t)
      	{	  
	  hW2L2R->Fill(WL2);	
	  hQ22L2R->Fill(Q2L2);	      	  
	  ht2L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w3t)
      	{	  
	  hW3L2R->Fill(WL2);	
	  hQ23L2R->Fill(Q2L2);	      	  
	  ht3L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w4t)
      	{	  
	  hW4L2R->Fill(WL2);	
	  hQ24L2R->Fill(Q2L2);	      	  
	  ht4L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w5t)
      	{	  
	  hW5L2R->Fill(WL2);	
	  hQ25L2R->Fill(Q2L2);	      	  
	  ht5L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w6t)
      	{	  
	  hW6L2R->Fill(WL2);	
	  hQ26L2R->Fill(Q2L2);	      	  
	  ht6L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w7t)
      	{	  
	  hW7L2R->Fill(WL2);	
	  hQ27L2R->Fill(Q2L2);	      	  
	  ht7L2R->Fill(-tL2); 
	}      
      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut && w8t)
      	{	  
	  hW8L2R->Fill(WL2);	
	  hQ28L2R->Fill(Q2L2);	      	  
	  ht8L2R->Fill(-tL2); 
	}      

      if (((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)))
	{
	  hmmL2R->Fill(mmL2);
	}
    }

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

  Double_t scaleFC  = 1.0/513.911;
  Double_t scaleFL1 = 1.0/695.738;
  Double_t scaleFL2 = 1.0/825.881;
  
  TCanvas *c1 = new TCanvas("c1", " c1"); 

  htcoinL2->SetStats(0);
  htcoinL2->SetLineColor(kBlack);
  htcoinL2->Draw("Weight");

  htcoinL1->SetStats(0);
  htcoinL1->SetLineColor(kRed);
  htcoinL1->Draw("same Weight ");

  htcoinC->SetStats(0);
  htcoinC->SetLineColor(kGreen);
  htcoinC->Draw("same Weight");

  TLine *l1 = new TLine(-1.0, 0.0, -1.0, 7000.0);
  TLine *l2 = new TLine(1.0, 0.0, 1.0, 7000.0);
  TLine *l3 = new TLine(-15.0, 0.0, -15.0, 7000.0);
  TLine *l4 = new TLine(-9.0, 0.0, -9.0, 7000.0);
  TLine *l5 = new TLine(7.0, 0.0, 7.0, 7000.0);
  TLine *l6 = new TLine(13.0, 0.0, 13.0, 7000.0);

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

  auto leg = new TLegend(0.1,0.7,0.30,0.9);
  leg->SetHeader("Low epsilon","C");
  leg->AddEntry(htcoinC,"Center","l");
  leg->AddEntry(htcoinL1,"Left1","l");
  leg->AddEntry(htcoinL2,"Left2","l");
  leg->Draw("same");

  c1->Print(outputpdf + '(');

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

  ofstream outleC("kindata/kindata.pl_375_286_+0000.dat");
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

  ofstream outleL1("kindata/kindata.pl_375_286_-1995.dat");
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

  ofstream outleL2("kindata/kindata.pl_375_286_-4005.dat");
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
  hph_qL2->Draw(" Weight");

  hph_qL1->SetStats(0);
  hph_qL1->SetLineColor(kRed);
  hph_qL1->Add(hph_qL1R, -1);
  hph_qL1->Draw("same Weight");

  hph_qC->SetStats(0);
  hph_qC->SetLineColor(kGreen);
  hph_qC->Add(hph_qCR, -1);
  hph_qC->Draw("same Weight");

  c4->Print(outputpdf);

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

  TCanvas *cdeltaS = new TCanvas("cdeltaS", " cdeltaS");
  H_ssdelta_SC->Draw("Weight");
  cdeltaS->Print(outputpdf);

  TCanvas *cdeltaH = new TCanvas("cdeltaH", " cdeltaH");
  H_hsdelta_SC->Draw("Weight");
  cdeltaH->Print(outputpdf);

  TCanvas *cxptarS = new TCanvas("cxptarS", " cxptarS");
  H_ssxptar_SC->Draw("Weight");
  cxptarS->Print(outputpdf);

  TCanvas *cxptarH = new TCanvas("cxptarH", " cxptarH");
  H_hsxptar_SC->Draw("Weight");
  cxptarH->Print(outputpdf);

  TCanvas *cyptarS = new TCanvas("cyptarS", " cyptarS");
  H_ssyptar_SC->Draw("Weight");
  cyptarS->Print(outputpdf);

  TCanvas *cyptarH = new TCanvas("cyptarH", " cyptarH");
  H_hsyptar_SC->Draw("Weight");
  cyptarH->Print(outputpdf);

  TCanvas *cY = new TCanvas("cY", " cY");
  hYC->Add(hYCR, -1);  
  hYC->Scale(scaleFC);  
  hYC->SetStats(0);
  hYC->Draw("COLZ");
  cY->Print(outputpdf);

  cout<< "Center Setting Yield"<<endl;
  cout<< "  "<<endl;

  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYC->IntegralAndError(j, j, i, i, err, "");;
	  cout<<Yield <<" +- "<<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  TCanvas *cYL1 = new TCanvas("cYL1", " cYL1");
  hYL1->Add(hYL1R, -1);  
  hYL1->Scale(scaleFL1);  
  hYL1->SetStats(0);
  hYL1->Draw("COLZ");
  cYL1->Print(outputpdf);

  cout<< "Left 1 Setting Yield"<<endl;

  cout<< "  "<<endl;

  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYL1->IntegralAndError(j, j, i, i, err, "");;
	  cout<<Yield <<" +- "<<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }


  TCanvas *cYL2 = new TCanvas("cYL2", " cYL2");
  hYL2->Add(hYL2R, -1);  
  hYL2->Scale(scaleFL2);  
  hYL2->SetStats(0);
  hYL2->Draw("COLZ");
  cYL2->Print(outputpdf);

  cout<< "Left 2 Setting Yield"<<endl;

  cout<< "  "<<endl;

  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = hYL2->IntegralAndError(j, j, i, i, err, "");;
	  cout<<Yield <<" +-" <<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  TH2D *Clone = (TH2D*)hYC->Clone("Clone");
  Clone->Add(hYL1,+1);
  Clone->Add(hYL2,+1);
  Clone->SetTitle("Total Yield for low epsilon");
  TCanvas *cYtle = new TCanvas("cYtle", " cYtle");
  Clone->Draw("COLZ");
  cYtle->Print(outputpdf);

  cout<< "Total Yield for low epsilon"<<endl;

  for(Double_t j = 2; j <= 9; j++)
    {
      for(Long64_t i = 2; i <= 17; i++)       
	{
	  Double_t err;
	  Double_t Yield = Clone->IntegralAndError(j, j, i, i, err, "");;
	  cout<<Yield <<" +-" <<err << "  "<< j-1 << "  "<< i-1 << endl;   
	}
    }

  cout<< "End of the low e Yield calculations " <<endl;

  TCanvas *cRE = new TCanvas("cRE", " cRE");

  TF1 *Gauss = new TF1("Gauss","gaus(0)",-0.004,0.006);
  Gauss->SetLineColor(kRed);
  H_t_RE->Fit("Gauss", "RQ"); 
  // H_t_RE->SetStats(0);
  H_t_RE->GetYaxis()->SetTitle("Weighted Yield");  
  H_t_RE->Draw("Weight");

  //  auto legRE = new TLegend(0.8,0.7,0.30,0.9); 
  auto legRE = new TLegend(0.1,0.7,0.40,0.9); 
  legRE->SetHeader("Center setting at low #epsilon ","C");
  legRE->SetTextSize(0.03);
  legRE->AddEntry(Gauss, TString::Format("Gauss(2); #sigma = %0.6f",Gauss->GetParameter(2)), "lep"); 
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

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *Lowe = OutHisto_file->mkdir("Lowe");
  Lowe->cd();  
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
 
  //Mid epsilon analysis

  cout<<" Analysing mid epsilon data..." <<endl;

  TString TInDATAFilenameMideR1 = "Analysed_Data_Q0p375W2p2_mide_Right1.root";
  TString TInDATAFilenameMideR2 = "Analysed_Data_Q0p375W2p2_mide_Right2.root";
  TString TInDATAFilenameMideC  = "Analysed_Data_Q0p375W2p2_mide_Center.root";
  TString TInDATAFilenameMideL1 = "Analysed_Data_Q0p375W2p2_mide_Left1.root";
  TString TInDATAFilenameMideL2 = "Analysed_Data_Q0p375W2p2_mide_Left2.root";
 
  TString rootFile_DATAMIDER1  = ROOTfilePath+"/"+TInDATAFilenameMideR1;
  TString rootFile_DATAMIDER2 = ROOTfilePath+"/"+TInDATAFilenameMideR2;
  TString rootFile_DATAMIDEC  = ROOTfilePath+"/"+TInDATAFilenameMideC;
  TString rootFile_DATAMIDEL1 = ROOTfilePath+"/"+TInDATAFilenameMideL1;
  TString rootFile_DATAMIDEL2 = ROOTfilePath+"/"+TInDATAFilenameMideL2;

  if (gSystem->AccessPathName(rootFile_DATAMIDER1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAMIDER1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  if (gSystem->AccessPathName(rootFile_DATAMIDER2) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAMIDER2 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
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
  
  TFile *InFile_DATAMIDER1 = new TFile(rootFile_DATAMIDER1, "READ");
  TFile *InFile_DATAMIDER2 = new TFile(rootFile_DATAMIDER2, "READ");
  TFile *InFile_DATAMIDEC = new TFile(rootFile_DATAMIDEC, "READ");
  TFile *InFile_DATAMIDEL1 = new TFile(rootFile_DATAMIDEL1, "READ");
  TFile *InFile_DATAMIDEL2 = new TFile(rootFile_DATAMIDEL2, "READ");

  // TREES
 
  TTree* TBRANCHMIDER1  = (TTree*)InFile_DATAMIDER1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDER1  = (Long64_t)TBRANCHMIDER1->GetEntries();  
  TTree* TBRANCHMIDER2  = (TTree*)InFile_DATAMIDER2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDER2  = (Long64_t)TBRANCHMIDER2->GetEntries();  
  TTree* TBRANCHMIDEC   = (TTree*)InFile_DATAMIDEC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHMIDEC   = (Long64_t)TBRANCHMIDEC->GetEntries();  
  TTree* TBRANCHMIDEL1  = (TTree*)InFile_DATAMIDEL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEL1  = (Long64_t)TBRANCHMIDEL1->GetEntries();  
  TTree* TBRANCHMIDEL2  = (TTree*)InFile_DATAMIDEL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHMIDEL2  = (Long64_t)TBRANCHMIDEL2->GetEntries();  
  
  //VARIABLES

  Double_t WMER1;TBRANCHMIDER1->SetBranchAddress("W", &WMER1);
  Double_t Q2MER1;TBRANCHMIDER1->SetBranchAddress("Q2", &Q2MER1);

  Double_t WMER2;TBRANCHMIDER2->SetBranchAddress("W", &WMER2);
  Double_t Q2MER2;TBRANCHMIDER2->SetBranchAddress("Q2", &Q2MER2);

  Double_t WMEC;TBRANCHMIDEC->SetBranchAddress("W", &WMEC);
  Double_t Q2MEC;TBRANCHMIDEC->SetBranchAddress("Q2", &Q2MEC);

  Double_t WMEL1;TBRANCHMIDEL1->SetBranchAddress("W", &WMEL1);
  Double_t Q2MEL1;TBRANCHMIDEL1->SetBranchAddress("Q2", &Q2MEL1);

  Double_t WMEL2;TBRANCHMIDEL2->SetBranchAddress("W", &WMEL2);
  Double_t Q2MEL2;TBRANCHMIDEL2->SetBranchAddress("Q2", &Q2MEL2);

  Double_t tmeR1;TBRANCHMIDER1->SetBranchAddress("MandelT", &tmeR1);
  Double_t tmeR2;TBRANCHMIDER2->SetBranchAddress("MandelT", &tmeR2);
  Double_t tmeC;TBRANCHMIDEC->SetBranchAddress("MandelT", &tmeC);
  Double_t tmeL1;TBRANCHMIDEL1->SetBranchAddress("MandelT", &tmeL1);
  Double_t tmeL2;TBRANCHMIDEL2->SetBranchAddress("MandelT", &tmeL2);
 
  Double_t ph_qmeR1;TBRANCHMIDER1->SetBranchAddress("ph_q", &ph_qmeR1);
  Double_t ph_qmeR2;TBRANCHMIDER2->SetBranchAddress("ph_q", &ph_qmeR2);
  Double_t ph_qmeC;TBRANCHMIDEC->SetBranchAddress("ph_q", &ph_qmeC);
  Double_t ph_qmeL1;TBRANCHMIDEL1->SetBranchAddress("ph_q", &ph_qmeL1);
  Double_t ph_qmeL2;TBRANCHMIDEL2->SetBranchAddress("ph_q", &ph_qmeL2);

  Double_t mmmeR1;TBRANCHMIDER1->SetBranchAddress("MM", &mmmeR1);
  Double_t mmmeR2;TBRANCHMIDER2->SetBranchAddress("MM", &mmmeR2);
  Double_t mmmeC;TBRANCHMIDEC->SetBranchAddress("MM", &mmmeC);
  Double_t mmmeL1;TBRANCHMIDEL1->SetBranchAddress("MM", &mmmeL1);
  Double_t mmmeL2;TBRANCHMIDEL2->SetBranchAddress("MM", &mmmeL2);

  Double_t tcoinmeR1;TBRANCHMIDER1->SetBranchAddress("CTime_ROC1", &tcoinmeR1);
  Double_t tcoinmeR2;TBRANCHMIDER2->SetBranchAddress("CTime_ROC1", &tcoinmeR2);
  Double_t tcoinmeC;TBRANCHMIDEC->SetBranchAddress("CTime_ROC1", &tcoinmeC);
  Double_t tcoinmeL1;TBRANCHMIDEL1->SetBranchAddress("CTime_ROC1", &tcoinmeL1);
  Double_t tcoinmeL2;TBRANCHMIDEL2->SetBranchAddress("CTime_ROC1", &tcoinmeL2);

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

  TH1D *hW1meR1   = new TH1D("hW1meR1","W 1 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW2meR1   = new TH1D("hW2meR1","W 2 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW3meR1   = new TH1D("hW3meR1","W 3 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW4meR1   = new TH1D("hW4meR1","W 4 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW5meR1   = new TH1D("hW5meR1","W 5 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW6meR1   = new TH1D("hW6meR1","W 6 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW7meR1   = new TH1D("hW7meR1","W 7 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW8meR1   = new TH1D("hW8meR1","W 8 t R1; W;", 300, 2.1, 2.3);

  TH1D *hW1meR2   = new TH1D("hW1meR2","W 1 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW2meR2   = new TH1D("hW2meR2","W 2 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW3meR2   = new TH1D("hW3meR2","W 3 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW4meR2   = new TH1D("hW4meR2","W 4 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW5meR2   = new TH1D("hW5meR2","W 5 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW6meR2   = new TH1D("hW6meR2","W 6 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW7meR2   = new TH1D("hW7meR2","W 7 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW8meR2   = new TH1D("hW8meR2","W 8 t R2; W;", 300, 2.1, 2.3);

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

  TH1D *hW1meR1R   = new TH1D("hW1meR1R","W 1 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW2meR1R   = new TH1D("hW2meR1R","W 2 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW3meR1R   = new TH1D("hW3meR1R","W 3 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW4meR1R   = new TH1D("hW4meR1R","W 4 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW5meR1R   = new TH1D("hW5meR1R","W 5 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW6meR1R   = new TH1D("hW6meR1R","W 6 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW7meR1R   = new TH1D("hW7meR1R","W 7 t R1; W;", 300, 2.1, 2.3);
  TH1D *hW8meR1R   = new TH1D("hW8meR1R","W 8 t R1; W;", 300, 2.1, 2.3);

  TH1D *hW1meR2R   = new TH1D("hW1meR2R","W 1 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW2meR2R   = new TH1D("hW2meR2R","W 2 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW3meR2R   = new TH1D("hW3meR2R","W 3 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW4meR2R   = new TH1D("hW4meR2R","W 4 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW5meR2R   = new TH1D("hW5meR2R","W 5 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW6meR2R   = new TH1D("hW6meR2R","W 6 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW7meR2R   = new TH1D("hW7meR2R","W 7 t R2; W;", 300, 2.1, 2.3);
  TH1D *hW8meR2R   = new TH1D("hW8meR2R","W 8 t R2; W;", 300, 2.1, 2.3);

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
      
  TH1D *hQ21meR1  = new TH1D("hQ21meR1","Q2 1 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meR1  = new TH1D("hQ22meR1","Q2 2 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meR1  = new TH1D("hQ23meR1","Q2 3 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meR1  = new TH1D("hQ24meR1","Q2 4 t R1; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meR1  = new TH1D("hQ25meR1","Q2 5 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meR1  = new TH1D("hQ26meR1","Q2 6 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meR1  = new TH1D("hQ27meR1","Q2 7 t R1 Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meR1  = new TH1D("hQ28meR1","Q2 8 t R1 Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meR2  = new TH1D("hQ21meR2","Q2 1 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22meR2  = new TH1D("hQ22meR2","Q2 2 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23meR2  = new TH1D("hQ23meR2","Q2 3 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24meR2  = new TH1D("hQ24meR2","Q2 4 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25meR2  = new TH1D("hQ25meR2","Q2 5 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26meR2  = new TH1D("hQ26meR2","Q2 6 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27meR2  = new TH1D("hQ27meR2","Q2 7 t R2 Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28meR2  = new TH1D("hQ28meR2","Q2 8 t R2 Q2;", 300, 0.2, 0.6);      

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

  TH1D *hQ21meR1R  = new TH1D("hQ21meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22meR1R  = new TH1D("hQ22meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23meR1R  = new TH1D("hQ23meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24meR1R  = new TH1D("hQ24meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25meR1R  = new TH1D("hQ25meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26meR1R  = new TH1D("hQ26meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27meR1R  = new TH1D("hQ27meR1R","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28meR1R  = new TH1D("hQ28meR1R","Q2; Q2;", 300, 0.2, 0.6);
      
  TH1D *hQ21meR2R  = new TH1D("hQ21meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ22meR2R  = new TH1D("hQ22meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ23meR2R  = new TH1D("hQ23meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ24meR2R  = new TH1D("hQ24meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ25meR2R  = new TH1D("hQ25meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ26meR2R  = new TH1D("hQ26meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ27meR2R  = new TH1D("hQ27meLRR","Q2; Q2;", 300, 0.2, 0.6);      
  TH1D *hQ28meR2R  = new TH1D("hQ28meLRR","Q2; Q2;", 300, 0.2, 0.6);      
      
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

  TH1D *ht1meR1  = new TH1D("ht1meR1","t 1 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meR1  = new TH1D("ht2meR1","t 2 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meR1  = new TH1D("ht3meR1","t 3 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meR1  = new TH1D("ht4meR1","t 4 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meR1  = new TH1D("ht5meR1","t 5 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meR1  = new TH1D("ht6meR1","t 6 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meR1  = new TH1D("ht7meR1","t 7 t R1; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meR1  = new TH1D("ht8meR1","t 8 t R1; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meR2  = new TH1D("ht1meR2","t 1 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meR2  = new TH1D("ht2meR2","t 2 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meR2  = new TH1D("ht3meR2","t 3 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meR2  = new TH1D("ht4meR2","t 4 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meR2  = new TH1D("ht5meR2","t 5 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meR2  = new TH1D("ht6meR2","t 6 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meR2  = new TH1D("ht7meR2","t 7 t R2; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meR2  = new TH1D("ht8meR2","t 8 t R2; MandelT;",   300, -0.01, 0.1);      

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

  TH1D *ht1meR1R  = new TH1D("ht1meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meR1R  = new TH1D("ht2meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meR1R  = new TH1D("ht3meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meR1R  = new TH1D("ht4meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meR1R  = new TH1D("ht5meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meR1R  = new TH1D("ht6meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meR1R  = new TH1D("ht7meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meR1R  = new TH1D("ht8meR1R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *ht1meR2R  = new TH1D("ht1meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2meR2R  = new TH1D("ht2meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3meR2R  = new TH1D("ht3meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4meR2R  = new TH1D("ht4meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5meR2R  = new TH1D("ht5meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6meR2R  = new TH1D("ht6meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7meR2R  = new TH1D("ht7meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8meR2R  = new TH1D("ht8meR2R","MandelT; MandelT;",   300, -0.01, 0.1);      

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

  TH1D *htmeR1  = new TH1D("htmeR1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeR2  = new TH1D("htmeR2","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeC   = new TH1D("htmeC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1  = new TH1D("htmeL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2  = new TH1D("htmeL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htmeInR1  = new TH1D("htmeInR1","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInR2  = new TH1D("htmeInR2","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInC   = new TH1D("htmeInC","MandelT; MandelT;",    10, bins);      
  TH1D *htmeInL1  = new TH1D("htmeInL1","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInL2  = new TH1D("htmeInL2","MandelT; MandelT;",   10, bins);      

  TH1D *htmeR1R  = new TH1D("htmeR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeR2R  = new TH1D("htmeR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeCR   = new TH1D("htmeCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1R  = new TH1D("htmeL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2R  = new TH1D("htmeL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htmeInR1R  = new TH1D("htmeInR1R","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInR2R  = new TH1D("htmeInR2R","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInCR   = new TH1D("htmeInCR","MandelT; MandelT;",    10, bins);      
  TH1D *htmeInL1R  = new TH1D("htmeInL1R","MandelT; MandelT;",   10, bins);      
  TH1D *htmeInL2R  = new TH1D("htmeInL2R","MandelT; MandelT;",   10, bins);      

  TH1D *hph_qmeR1  = new TH1D("hph_qmeR1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeR2  = new TH1D("hph_qmeR2","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeC   = new TH1D("hph_qmeC","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qmeL1  = new TH1D("hph_qmeL1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeL2  = new TH1D("hph_qmeL2","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH1D *hph_qmeR1R  = new TH1D("hph_qmeR1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeR2R  = new TH1D("hph_qmeR2R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeCR   = new TH1D("hph_qmeCR","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qmeL1R  = new TH1D("hph_qmeL1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qmeL2R  = new TH1D("hph_qmeL2R","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH2D *hYmeR1   = new TH2D("hYmeR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeR2   = new TH2D("hYmeR2"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeC    = new TH2D("hYmeC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL1   = new TH2D("hYmeL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL2   = new TH2D("hYmeL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYmeR1R   = new TH2D("hYmeR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeR2R   = new TH2D("hYmeR2R"," Yield R2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeCR    = new TH2D("hYmeCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL1R   = new TH2D("hYmeL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYmeL2R   = new TH2D("hYmeL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      

  TH1D *hmmmeR1  = new TH1D("hmmmeR1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeR2  = new TH1D("hmmmeR2","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeC   = new TH1D("hmmmeC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL1  = new TH1D("hmmmeL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL2  = new TH1D("hmmmeL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmmeR1R  = new TH1D("hmmmeR1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeR2R  = new TH1D("hmmmeR2R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeCR   = new TH1D("hmmmeCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL1R  = new TH1D("hmmmeL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmmeL2R  = new TH1D("hmmmeL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinmeR1   = new TH1D("htcoinmeR1","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinmeR2   = new TH1D("htcoinmeR2","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinmeC    = new TH1D("htcoinmeC","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinmeL1   = new TH1D("htcoinmeL1","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinmeL2   = new TH1D("htcoinmeL2","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  
  for(Long64_t i = 0; i < nEntries_TBRANCHMIDER1; i++)

    {
      TBRANCHMIDER1->GetEntry(i);
      
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0)    
	{
	  hmmmeR1->Fill(mmmeR1);	  
	}	      
      
      {
	htcoinmeR1->Fill(tcoinmeR1);
      }

      Double_t Diamond_cut = (Dcut->IsInside(Q2MER1, WMER1));  
      
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut)
	{
	  htmeR1->Fill(-tmeR1);
	  htmeInR1->Fill(-tmeR1);
	  hph_qmeR1->Fill(ph_qmeR1*57.2958 + 180);
	  hYmeR1->Fill(-tmeR1, ph_qmeR1*57.2958 + 180);
	}

      if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut)
	{
	  htmeR1R->Fill(-tmeR1);
	  htmeInR1R->Fill(-tmeR1);
	  hph_qmeR1R->Fill(ph_qmeR1*57.2958 + 180);
	  hYmeR1R->Fill(-tmeR1, ph_qmeR1*57.2958 + 180);
	}

      Double_t w1t = -tmeR1 >= bins[1] && -tmeR1 <= bins[2];
      Double_t w2t = -tmeR1 >= bins[2] && -tmeR1 <= bins[3];
      Double_t w3t = -tmeR1 >= bins[3] && -tmeR1 <= bins[4];
      Double_t w4t = -tmeR1 >= bins[4] && -tmeR1 <= bins[5];
      Double_t w5t = -tmeR1 >= bins[5] && -tmeR1 <= bins[6];
      Double_t w6t = -tmeR1 >= bins[6] && -tmeR1 <= bins[7];
      Double_t w7t = -tmeR1 >= bins[7] && -tmeR1 <= bins[8];
      Double_t w8t = -tmeR1 >= bins[8] && -tmeR1 <= bins[9];

      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w1t)
	{
	  hW1meR1->Fill(WMER1);	
	  hQ21meR1->Fill(Q2MER1);	      	  
	  ht1meR1->Fill(-tmeR1); 
 	}	

      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w2t)
	{
	  hW2meR1->Fill(WMER1);	
	  hQ22meR1->Fill(Q2MER1);	      	  
	  ht2meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w3t)
	{
	  hW3meR1->Fill(WMER1);	
	  hQ23meR1->Fill(Q2MER1);	      	  
	  ht3meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w4t)
	{
	  hW4meR1->Fill(WMER1);	
	  hQ24meR1->Fill(Q2MER1);	      	  
	  ht4meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w5t)
	{
	  hW5meR1->Fill(WMER1);	
	  hQ25meR1->Fill(Q2MER1);	      	  
	  ht5meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w6t)
	{
	  hW6meR1->Fill(WMER1);	
	  hQ26meR1->Fill(Q2MER1);	      	  
	  ht6meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w7t)
	{
	  hW7meR1->Fill(WMER1);	
	  hQ27meR1->Fill(Q2MER1);	      	  
	  ht7meR1->Fill(-tmeR1); 
 	}	
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w8t)
	{
	  hW8meR1->Fill(WMER1);	
	  hQ28meR1->Fill(Q2MER1);	      	  
	  ht8meR1->Fill(-tmeR1); 
 	}	
      
      if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1meR1R->Fill(WMER1);	
	  hQ21meR1R->Fill(Q2MER1);	      	  
	  ht1meR1R->Fill(-tmeR1); 
	}

   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2meR1R->Fill(WMER1);	
	  hQ22meR1R->Fill(Q2MER1);	      	  
	  ht2meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3meR1R->Fill(WMER1);	
	  hQ23meR1R->Fill(Q2MER1);	      	  
	  ht3meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4meR1R->Fill(WMER1);	
	  hQ24meR1R->Fill(Q2MER1);	      	  
	  ht4meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5meR1R->Fill(WMER1);	
	  hQ25meR1R->Fill(Q2MER1);	      	  
	  ht5meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6meR1R->Fill(WMER1);	
	  hQ26meR1R->Fill(Q2MER1);	      	  
	  ht6meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7meR1R->Fill(WMER1);	
	  hQ27meR1R->Fill(Q2MER1);	      	  
	  ht7meR1R->Fill(-tmeR1); 
	}
   if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8meR1R->Fill(WMER1);	
	  hQ28meR1R->Fill(Q2MER1);	      	  
	  ht8meR1R->Fill(-tmeR1); 
	}
      
      if (((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)))
	
	{
	  hmmmeR1R->Fill(mmmeR1);
	}	
    }  

  for(Long64_t i = 0; i < nEntries_TBRANCHMIDER2; i++)

    {
      TBRANCHMIDER2->GetEntry(i);
      
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0)    
	{
	  hmmmeR2->Fill(mmmeR2);	  
	}	      
      
      {
	htcoinmeR2->Fill(tcoinmeR2);
      }
           
      Double_t Diamond_cut = (Dcut->IsInside(Q2MER2, WMER2));  
     
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut)
	{
	  htmeR2->Fill(-tmeR2);
	  htmeInR2->Fill(-tmeR2);
	  hph_qmeR2->Fill(ph_qmeR2*57.2958 + 180);
	  hYmeR2->Fill(-tmeR2, ph_qmeR2*57.2958 + 180);
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut)
	{
	  htmeR2R->Fill(-tmeR2);
	  htmeInR2R->Fill(-tmeR2);
	  hph_qmeR2R->Fill(ph_qmeR2*57.2958 + 180);
	  hYmeR2R->Fill(-tmeR2, ph_qmeR2*57.2958 + 180);

	}
      
      Double_t w1t = -tmeR2 >= bins[1] && -tmeR2 <= bins[2];
      Double_t w2t = -tmeR2 >= bins[2] && -tmeR2 <= bins[3];
      Double_t w3t = -tmeR2 >= bins[3] && -tmeR2 <= bins[4];
      Double_t w4t = -tmeR2 >= bins[4] && -tmeR2 <= bins[5];
      Double_t w5t = -tmeR2 >= bins[5] && -tmeR2 <= bins[6];
      Double_t w6t = -tmeR2 >= bins[6] && -tmeR2 <= bins[7];
      Double_t w7t = -tmeR2 >= bins[7] && -tmeR2 <= bins[8];
      Double_t w8t = -tmeR2 >= bins[8] && -tmeR2 <= bins[9];

      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w1t)
	{
	  hW1meR2->Fill(WMER2);	
	  hQ21meR2->Fill(Q2MER2);	      	  
	  ht1meR2->Fill(-tmeR2); 
 	}	

      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w2t)
	{
	  hW2meR2->Fill(WMER2);	
	  hQ22meR2->Fill(Q2MER2);	      	  
	  ht2meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w3t)
	{
	  hW3meR2->Fill(WMER2);	
	  hQ23meR2->Fill(Q2MER2);	      	  
	  ht3meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w4t)
	{
	  hW4meR2->Fill(WMER2);	
	  hQ24meR2->Fill(Q2MER2);	      	  
	  ht4meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w5t)
	{
	  hW5meR2->Fill(WMER2);	
	  hQ25meR2->Fill(Q2MER2);	      	  
	  ht5meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w6t)
	{
	  hW6meR2->Fill(WMER2);	
	  hQ26meR2->Fill(Q2MER2);	      	  
	  ht6meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w7t)
	{
	  hW7meR2->Fill(WMER2);	
	  hQ27meR2->Fill(Q2MER2);	      	  
	  ht7meR2->Fill(-tmeR2); 
 	}	
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w8t)
	{
	  hW8meR2->Fill(WMER2);	
	  hQ28meR2->Fill(Q2MER2);	      	  
	  ht8meR2->Fill(-tmeR2); 
 	}	
      
      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1meR2R->Fill(WMER2);	
	  hQ21meR2R->Fill(Q2MER2);	      	  
	  ht1meR2R->Fill(-tmeR2); 
	}
      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2meR2R->Fill(WMER2);	
	  hQ22meR2R->Fill(Q2MER2);	      	  
	  ht2meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3meR2R->Fill(WMER2);	
	  hQ23meR2R->Fill(Q2MER2);	      	  
	  ht3meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4meR2R->Fill(WMER2);	
	  hQ24meR2R->Fill(Q2MER2);	      	  
	  ht4meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5meR2R->Fill(WMER2);	
	  hQ25meR2R->Fill(Q2MER2);	      	  
	  ht5meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6meR2R->Fill(WMER2);	
	  hQ26meR2R->Fill(Q2MER2);	      	  
	  ht6meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7meR2R->Fill(WMER2);	
	  hQ27meR2R->Fill(Q2MER2);	      	  
	  ht7meR2R->Fill(-tmeR2); 
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8meR2R->Fill(WMER2);	
	  hQ28meR2R->Fill(Q2MER2);	      	  
	  ht8meR2R->Fill(-tmeR2); 
	}


      if (((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)))
	
	{
	  hmmmeR2R->Fill(mmmeR2);
	}	
    }  

  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEC; i++)

    {
      TBRANCHMIDEC->GetEntry(i);
      
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98)
	{
	  hQ2WMEC->Fill(Q2MEC, WMEC);
	}
 
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0)    
 	{
	  hmmmeC->Fill(mmmeC);	  
	}	      
      
      {
	htcoinmeC->Fill(tcoinmeC);
      }
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2MEC, WMEC));  
      
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut)
	{
	  hQ2WMEC1->Fill(Q2MEC, WMEC);	      	  
	  htmeC->Fill(-tmeC);
	  htmeInC->Fill(-tmeC);
	  hph_qmeC->Fill(ph_qmeC*57.2958 + 180);
	  hYmeC->Fill(-tmeC, ph_qmeC*57.2958 + 180);

	}	      

           
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut)
  	{
	  htmeCR->Fill(-tmeC);
	  htmeInCR->Fill(-tmeC);
	  hph_qmeCR->Fill(ph_qmeC*57.2958 + 180);
	  hYmeCR->Fill(-tmeC, ph_qmeC*57.2958 + 180);
	  hQ2WMECR->Fill(Q2MEC, WMEC);
	}
      Double_t w1t = -tmeC >= bins[1] && -tmeC <= bins[2];
      Double_t w2t = -tmeC >= bins[2] && -tmeC <= bins[3];
      Double_t w3t = -tmeC >= bins[3] && -tmeC <= bins[4];
      Double_t w4t = -tmeC >= bins[4] && -tmeC <= bins[5];
      Double_t w5t = -tmeC >= bins[5] && -tmeC <= bins[6];
      Double_t w6t = -tmeC >= bins[6] && -tmeC <= bins[7];
      Double_t w7t = -tmeC >= bins[7] && -tmeC <= bins[8];
      Double_t w8t = -tmeC >= bins[8] && -tmeC <= bins[9];

      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w1t)
	{
	  hW1meC->Fill(WMEC);	
	  hQ21meC->Fill(Q2MEC);	      	  
	  ht1meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w2t)
	{
	  hW2meC->Fill(WMEC);	
	  hQ22meC->Fill(Q2MEC);	      	  
	  ht2meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w3t)
	{
	  hW3meC->Fill(WMEC);	
	  hQ23meC->Fill(Q2MEC);	      	  
	  ht3meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w4t)
	{
	  hW4meC->Fill(WMEC);	
	  hQ24meC->Fill(Q2MEC);	      	  
	  ht4meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w5t)
	{
	  hW5meC->Fill(WMEC);	
	  hQ25meC->Fill(Q2MEC);	      	  
	  ht5meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w6t)
	{
	  hW6meC->Fill(WMEC);	
	  hQ26meC->Fill(Q2MEC);	      	  
	  ht6meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w7t)
	{
	  hW7meC->Fill(WMEC);	
	  hQ27meC->Fill(Q2MEC);	      	  
	  ht7meC->Fill(-tmeC); 
 	}	
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w8t)
	{
	  hW8meC->Fill(WMEC);	
	  hQ28meC->Fill(Q2MEC);	      	  
	  ht8meC->Fill(-tmeC); 
 	}	

      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1meCR->Fill(WMEC);	
	  hQ21meCR->Fill(Q2MEC);	      	  
	  ht1meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2meCR->Fill(WMEC);	
	  hQ22meCR->Fill(Q2MEC);	      	  
	  ht2meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3meCR->Fill(WMEC);	
	  hQ23meCR->Fill(Q2MEC);	      	  
	  ht3meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4meCR->Fill(WMEC);	
	  hQ24meCR->Fill(Q2MEC);	      	  
	  ht4meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5meCR->Fill(WMEC);	
	  hQ25meCR->Fill(Q2MEC);	      	  
	  ht5meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6meCR->Fill(WMEC);	
	  hQ26meCR->Fill(Q2MEC);	      	  
	  ht6meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7meCR->Fill(WMEC);	
	  hQ27meCR->Fill(Q2MEC);	      	  
	  ht7meCR->Fill(-tmeC); 
	}
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8meCR->Fill(WMEC);	
	  hQ28meCR->Fill(Q2MEC);	      	  
	  ht8meCR->Fill(-tmeC); 
	}
      
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)))
	
	{
	  hmmmeCR->Fill(mmmeC);
	}	
    }  


  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL1; i++)
  
    {
      TBRANCHMIDEL1->GetEntry(i);
            
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0)    
	{
	  hmmmeL1->Fill(mmmeL1);
	}           
      
      {
	htcoinmeL1->Fill(tcoinmeL1);
      }
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2MEL1, WMEL1));  

      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut)
	{
	  htmeL1->Fill(-tmeL1);
	  htmeInL1->Fill(-tmeL1);
	  hph_qmeL1->Fill(ph_qmeL1*57.2958 + 180);	  
	  hYmeL1->Fill(-tmeL1, ph_qmeL1*57.2958 + 180);

	}

      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut)
 	
	{
	  htmeL1R->Fill(-tmeL1);
	  htmeInL1R->Fill(-tmeL1);
	  hph_qmeL1R->Fill(ph_qmeL1*57.2958 + 180);	  
	  hYmeL1R->Fill(-tmeL1, ph_qmeL1*57.2958 + 180);
	}
      Double_t w1t = -tmeL1 >= bins[1] && -tmeL1 <= bins[2];
      Double_t w2t = -tmeL1 >= bins[2] && -tmeL1 <= bins[3];
      Double_t w3t = -tmeL1 >= bins[3] && -tmeL1 <= bins[4];
      Double_t w4t = -tmeL1 >= bins[4] && -tmeL1 <= bins[5];
      Double_t w5t = -tmeL1 >= bins[5] && -tmeL1 <= bins[6];
      Double_t w6t = -tmeL1 >= bins[6] && -tmeL1 <= bins[7];
      Double_t w7t = -tmeL1 >= bins[7] && -tmeL1 <= bins[8];
      Double_t w8t = -tmeL1 >= bins[8] && -tmeL1 <= bins[9];

      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w1t)
	{
	  hW1meL1->Fill(WMEL1);	
	  hQ21meL1->Fill(Q2MEL1);	      	  
	  ht1meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w2t)
	{
	  hW2meL1->Fill(WMEL1);	
	  hQ22meL1->Fill(Q2MEL1);	      	  
	  ht2meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w3t)
	{
	  hW3meL1->Fill(WMEL1);	
	  hQ23meL1->Fill(Q2MEL1);	      	  
	  ht3meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w4t)
	{
	  hW4meL1->Fill(WMEL1);	
	  hQ24meL1->Fill(Q2MEL1);	      	  
	  ht4meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w5t)
	{
	  hW5meL1->Fill(WMEL1);	
	  hQ25meL1->Fill(Q2MEL1);	      	  
	  ht5meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w6t)
	{
	  hW6meL1->Fill(WMEL1);	
	  hQ26meL1->Fill(Q2MEL1);	      	  
	  ht6meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w7t)
	{
	  hW7meL1->Fill(WMEL1);	
	  hQ27meL1->Fill(Q2MEL1);	      	  
	  ht7meL1->Fill(-tmeL1); 
 	}	
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w8t)
	{
	  hW8meL1->Fill(WMEL1);	
	  hQ28meL1->Fill(Q2MEL1);	      	  
	  ht8meL1->Fill(-tmeL1); 
 	}	
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1meL1R->Fill(WMEL1);	
	  hQ21meL1R->Fill(Q2MEL1);	      	  
	  ht1meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2meL1R->Fill(WMEL1);	
	  hQ22meL1R->Fill(Q2MEL1);	      	  
	  ht2meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3meL1R->Fill(WMEL1);	
	  hQ23meL1R->Fill(Q2MEL1);	      	  
	  ht3meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4meL1R->Fill(WMEL1);	
	  hQ24meL1R->Fill(Q2MEL1);	      	  
	  ht4meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5meL1R->Fill(WMEL1);	
	  hQ25meL1R->Fill(Q2MEL1);	      	  
	  ht5meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6meL1R->Fill(WMEL1);	
	  hQ26meL1R->Fill(Q2MEL1);	      	  
	  ht6meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7meL1R->Fill(WMEL1);	
	  hQ27meL1R->Fill(Q2MEL1);	      	  
	  ht7meL1R->Fill(-tmeL1); 
	}
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8meL1R->Fill(WMEL1);	
	  hQ28meL1R->Fill(Q2MEL1);	      	  
	  ht8meL1R->Fill(-tmeL1); 
	}
      
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)))
	
	{	 
	  hmmmeL1R->Fill(mmmeL1);
	}     
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL2; i++)
    {
      TBRANCHMIDEL2->GetEntry(i);
            
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0)
	
	{
	  hmmmeL2->Fill(mmmeL2);
	}   
      
      {
	htcoinmeL2->Fill(tcoinmeL2);
      }
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2MEL2, WMEL2));  
      
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut)
	
	{
	  htmeL2->Fill(-tmeL2);
	  htmeInL2->Fill(-tmeL2);
	  hph_qmeL2->Fill(ph_qmeL2*57.2958 + 180);
	  hYmeL2->Fill(-tmeL2, ph_qmeL2*57.2958 + 180);
	}

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut)
	{
	  htmeL2R->Fill(-tmeL2);
	  htmeInL2R->Fill(-tmeL2);
	  hph_qmeL2R->Fill(ph_qmeL2*57.2958 + 180);
	  hYmeL2R->Fill(-tmeL2, ph_qmeL2*57.2958 + 180);

	}
      Double_t w1t = -tmeL2 >= bins[1] && -tmeL2 <= bins[2];
      Double_t w2t = -tmeL2 >= bins[2] && -tmeL2 <= bins[3];
      Double_t w3t = -tmeL2 >= bins[3] && -tmeL2 <= bins[4];
      Double_t w4t = -tmeL2 >= bins[4] && -tmeL2 <= bins[5];
      Double_t w5t = -tmeL2 >= bins[5] && -tmeL2 <= bins[6];
      Double_t w6t = -tmeL2 >= bins[6] && -tmeL2 <= bins[7];
      Double_t w7t = -tmeL2 >= bins[7] && -tmeL2 <= bins[8];
      Double_t w8t = -tmeL2 >= bins[8] && -tmeL2 <= bins[9];

      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w1t)
	{
	  hW1meL2->Fill(WMEL2);	
	  hQ21meL2->Fill(Q2MEL2);	      	  
	  ht1meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w2t)
	{
	  hW2meL2->Fill(WMEL2);	
	  hQ22meL2->Fill(Q2MEL2);	      	  
	  ht2meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w3t)
	{
	  hW3meL2->Fill(WMEL2);	
	  hQ23meL2->Fill(Q2MEL2);	      	  
	  ht3meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w4t)
	{
	  hW4meL2->Fill(WMEL2);	
	  hQ24meL2->Fill(Q2MEL2);	      	  
	  ht4meL2->Fill(-tmeL2); 
 	}
      
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w5t)
	{
	  hW5meL2->Fill(WMEL2);	
	  hQ25meL2->Fill(Q2MEL2);	      	  
	  ht5meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w6t)
	{
	  hW6meL2->Fill(WMEL2);	
	  hQ26meL2->Fill(Q2MEL2);	      	  
	  ht6meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w7t)
	{
	  hW7meL2->Fill(WMEL2);	
	  hQ27meL2->Fill(Q2MEL2);	      	  
	  ht7meL2->Fill(-tmeL2); 
 	}	
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w8t)
	{
	  hW8meL2->Fill(WMEL2);	
	  hQ28meL2->Fill(Q2MEL2);	      	  
	  ht8meL2->Fill(-tmeL2); 
 	}	

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1meL2R->Fill(WMEL2);	
	  hQ21meL2R->Fill(Q2MEL2);	      	  
	  ht1meL2R->Fill(-tmeL2); 
	}

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2meL2R->Fill(WMEL2);	
	  hQ22meL2R->Fill(Q2MEL2);	      	  
	  ht2meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3meL2R->Fill(WMEL2);	
	  hQ23meL2R->Fill(Q2MEL2);	      	  
	  ht3meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4meL2R->Fill(WMEL2);	
	  hQ24meL2R->Fill(Q2MEL2);	      	  
	  ht4meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5meL2R->Fill(WMEL2);	
	  hQ25meL2R->Fill(Q2MEL2);	      	  
	  ht5meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6meL2R->Fill(WMEL2);	
	  hQ26meL2R->Fill(Q2MEL2);	      	  
	  ht6meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7meL2R->Fill(WMEL2);	
	  hQ27meL2R->Fill(Q2MEL2);	      	  
	  ht7meL2R->Fill(-tmeL2); 
	}
      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8meL2R->Fill(WMEL2);	
	  hQ28meL2R->Fill(Q2MEL2);	      	  
	  ht8meL2R->Fill(-tmeL2); 
	}

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)))
	{
	  hmmmeL2R->Fill(mmmeL2);
	}
    }

  hQ2WMECR->Scale(1.0/6.0);

  hW1meR1R->Scale(1.0/6.0);
  hW2meR1R->Scale(1.0/6.0);
  hW3meR1R->Scale(1.0/6.0);
  hW4meR1R->Scale(1.0/6.0);
  hW5meR1R->Scale(1.0/6.0);
  hW6meR1R->Scale(1.0/6.0);
  hW7meR1R->Scale(1.0/6.0);
  hW8meR1R->Scale(1.0/6.0);

  hW1meR2R->Scale(1.0/6.0);
  hW2meR2R->Scale(1.0/6.0);
  hW3meR2R->Scale(1.0/6.0);
  hW4meR2R->Scale(1.0/6.0);
  hW5meR2R->Scale(1.0/6.0);
  hW6meR2R->Scale(1.0/6.0);
  hW7meR2R->Scale(1.0/6.0);
  hW8meR2R->Scale(1.0/6.0);

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

  hQ21meR1R->Scale(1.0/6.0);
  hQ22meR1R->Scale(1.0/6.0);
  hQ23meR1R->Scale(1.0/6.0);
  hQ24meR1R->Scale(1.0/6.0);
  hQ25meR1R->Scale(1.0/6.0);
  hQ26meR1R->Scale(1.0/6.0);
  hQ27meR1R->Scale(1.0/6.0);
  hQ28meR1R->Scale(1.0/6.0);

  hQ21meR2R->Scale(1.0/6.0);
  hQ22meR2R->Scale(1.0/6.0);
  hQ23meR2R->Scale(1.0/6.0);
  hQ24meR2R->Scale(1.0/6.0);
  hQ25meR2R->Scale(1.0/6.0);
  hQ26meR2R->Scale(1.0/6.0);
  hQ27meR2R->Scale(1.0/6.0);
  hQ28meR2R->Scale(1.0/6.0);

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

  ht1meR1R->Scale(1.0/6.0); 
  ht2meR1R->Scale(1.0/6.0); 
  ht3meR1R->Scale(1.0/6.0); 
  ht4meR1R->Scale(1.0/6.0); 
  ht5meR1R->Scale(1.0/6.0); 
  ht6meR1R->Scale(1.0/6.0); 
  ht7meR1R->Scale(1.0/6.0); 
  ht8meR1R->Scale(1.0/6.0); 

  ht1meR2R->Scale(1.0/6.0); 
  ht2meR2R->Scale(1.0/6.0); 
  ht3meR2R->Scale(1.0/6.0); 
  ht4meR2R->Scale(1.0/6.0); 
  ht5meR2R->Scale(1.0/6.0); 
  ht6meR2R->Scale(1.0/6.0); 
  ht7meR2R->Scale(1.0/6.0); 
  ht8meR2R->Scale(1.0/6.0); 

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
  
  hmmmeR1R->Scale(1.0/6.0);
  hmmmeR2R->Scale(1.0/6.0);
  hmmmeCR->Scale(1.0/6.0);
  hmmmeL1R->Scale(1.0/6.0);
  hmmmeL2R->Scale(1.0/6.0);

  htmeR1R->Scale(1.0/6.0);
  htmeR2R->Scale(1.0/6.0);
  htmeCR->Scale(1.0/6.0);
  htmeL1R->Scale(1.0/6.0);
  htmeL2R->Scale(1.0/6.0);

  htmeInR1R->Scale(1.0/6.0);
  htmeInR2R->Scale(1.0/6.0);
  htmeInCR->Scale(1.0/6.0);
  htmeInL1R->Scale(1.0/6.0);
  htmeInL2R->Scale(1.0/6.0);

  hph_qmeR1R->Scale(1.0/6.0);
  hph_qmeR2R->Scale(1.0/6.0);
  hph_qmeCR->Scale(1.0/6.0);
  hph_qmeL1R->Scale(1.0/6.0);
  hph_qmeL2R->Scale(1.0/6.0);

  hYmeR1R->Scale(1.0/6.0);
  hYmeR2R->Scale(1.0/6.0);
  hYmeCR->Scale(1.0/6.0);
  hYmeL1R->Scale(1.0/6.0);
  hYmeL2R->Scale(1.0/6.0);

  Double_t SFMER1  = 1.0/213.121;
  Double_t SFMER2  = 1.0/326.60;
  Double_t SFMEC   = 1.0/260.320;
  Double_t SFMEL1  = 1.0/348.721;
  Double_t SFMEL2  = 1.0/417.783;


  TCanvas *c8me = new TCanvas("c8me", " tcoin"); 

  htcoinmeL1->SetStats(0);
  htcoinmeL1->SetLineColor(kRed);
  htcoinmeL1->Draw("Weight ");
  
  htcoinmeR2->SetStats(0);
  htcoinmeR2->SetLineColor(kYellow);
  htcoinmeR2->Draw("same Weight");
 
  htcoinmeC->SetStats(0);
  htcoinmeC->SetLineColor(kGreen);
  htcoinmeC->Draw("same Weight");

  htcoinmeR1->SetStats(0);
  htcoinmeR1->SetLineColor(kBlue);
  htcoinmeR1->Draw(" same Weight");

  htcoinmeL2->SetStats(0);
  htcoinmeL2->SetLineColor(kBlack);
  htcoinmeL2->Draw("same Weight");
  
  TLine *l1me = new TLine(-1.0, 0.0, -1.0, 74000.0);
  TLine *l2me = new TLine(1.0, 0.0, 1.0, 74000.0);
  TLine *l3me = new TLine(-15.0, 0.0, -15.0, 74000.0);
  TLine *l4me = new TLine(-9.0, 0.0, -9.0, 74000.0);
  TLine *l5me = new TLine(7.0, 0.0, 7.0, 74000.0);
  TLine *l6me = new TLine(13.0, 0.0, 13.0, 74000.0);
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
     
  auto legme = new TLegend(0.1,0.7,0.30,0.9);
  legme->SetHeader("Mid epsilon","C");
  legme->AddEntry(htcoinmeR1,"Right1","l");
  legme->AddEntry(htcoinmeR2,"Right2","l");
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

  htmeR1->SetStats(0);
  htmeR1->SetLineColor(kBlue);
  htmeR1->Add(htmeR1R,-1); 
  htmeR1->Draw("same Weight");

  htmeR2->SetStats(0);
  htmeR2->SetLineColor(kYellow);
  htmeR2->Add(htmeR2R,-1); 
  htmeR2->Draw("same Weight");

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

  htmeInR1->SetStats(0);
  htmeInR1->SetLineColor(kBlue);
  htmeInR1->Add(htmeInR1R,-1); 
  htmeInR1->Draw("same Weight");

  htmeInR2->SetStats(0);
  htmeInR2->SetLineColor(kYellow);
  htmeInR2->Add(htmeInR2R,-1); 
  htmeInR2->Draw("same Weight");

  htmeInL1->SetStats(0);
  htmeInL1->SetLineColor(kRed);
  htmeInL1->Add(htmeInL1R, -1); 
  htmeInL1->Draw("same Weight");
  c9meIn->Print(outputpdf);

  TCanvas *cWmeR11 = new TCanvas("cWmeR11", "cWmeR11");
  cWmeR11->Divide(2, 2);
  cWmeR11->cd(1);
  hW1meR1->Add(hW1meR1R, -1);
  hW1meR1->Draw("Weight");
  cWmeR11->cd(2);
  hW2meR1->Add(hW2meR1R, -1);
  hW2meR1->Draw(" same Weight");
  cWmeR11->cd(3);
  hW3meR1->Add(hW3meR1R, -1);
  hW3meR1->Draw(" same Weight");
  cWmeR11->cd(4);
  hW4meR1->Add(hW4meR1R, -1);
  hW4meR1->Draw(" same Weight");
  cWmeR11->Print(outputpdf);
  
  TCanvas *cWmeR12 = new TCanvas("cWmeR12", "cWmeR12");
  cWmeR12->Divide(2, 2);
  cWmeR12->cd(1);
  hW5meR1->Add(hW5meR1R, -1);
  hW5meR1->Draw("Weight");
  cWmeR12->cd(2);
  hW6meR1->Add(hW6meR1R, -1);
  hW6meR1->Draw(" same Weight");
  cWmeR12->cd(3);
  hW7meR1->Add(hW7meR1R, -1);
  hW7meR1->Draw(" same Weight");
  cWmeR12->cd(4);
  hW8meR1->Add(hW8meR1R, -1);
  hW8meR1->Draw(" same Weight");
  cWmeR12->Print(outputpdf);

  TCanvas *cWmeR21 = new TCanvas("cWmeR21", "cWmeR21");
  cWmeR21->Divide(2, 2);
  cWmeR21->cd(1);
  hW1meR2->Add(hW1meR2R, -1);
  hW1meR2->Draw("Weight");
  cWmeR21->cd(2);
  hW2meR2->Add(hW2meR2R, -1);
  hW2meR2->Draw(" same Weight");
  cWmeR21->cd(3);
  hW3meR2->Add(hW3meR2R, -1);
  hW3meR2->Draw(" same Weight");
  cWmeR21->cd(4);
  hW4meR2->Add(hW4meR2R, -1);
  hW4meR2->Draw(" same Weight");
  cWmeR21->Print(outputpdf);
  
  TCanvas *cWmeR22 = new TCanvas("cWmeR22", "cWmeR22");
  cWmeR22->Divide(2, 2);
  cWmeR22->cd(1);
  hW5meR2->Add(hW5meR2R, -1);
  hW5meR2->Draw("Weight");
  cWmeR22->cd(2);
  hW6meR2->Add(hW6meR2R, -1);
  hW6meR2->Draw(" same Weight");
  cWmeR22->cd(3);
  hW7meR2->Add(hW7meR2R, -1);
  hW7meR2->Draw(" same Weight");
  cWmeR22->cd(4);
  hW8meR2->Add(hW8meR2R, -1);
  hW8meR2->Draw(" same Weight");
  cWmeR22->Print(outputpdf);

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

  TCanvas *cQ2meR11 = new TCanvas("cQ2meR11", "cQ2meR11");

  cQ2meR11->Divide(2, 2);
  cQ2meR11->cd(1);
  hQ21meR1->Add(hQ21meR1R, -1); 
  hQ21meR1->Draw(" Weight");
  cQ2meR11->cd(2);
  hQ22meR1->Add(hQ22meR1R, -1); 
  hQ22meR1->Draw("same Weight");
  cQ2meR11->cd(3);
  hQ23meR1->Add(hQ23meR1R, -1); 
  hQ23meR1->Draw("same Weight");
  cQ2meR11->cd(4);
  hQ24meR1->Add(hQ24meR1R, -1); 
  hQ24meR1->Draw("same Weight");
  cQ2meR11->Print(outputpdf);

  TCanvas *cQ2meR12 = new TCanvas("cQ2meR12", "cQ2meR12");

  cQ2meR12->Divide(2, 2);
  cQ2meR12->cd(1);
  hQ25meR1->Add(hQ25meR1R, -1); 
  hQ25meR1->Draw(" Weight");
  cQ2meR12->cd(2);
  hQ26meR1->Add(hQ26meR1R, -1); 
  hQ26meR1->Draw("same Weight");
  cQ2meR12->cd(3);
  hQ27meR1->Add(hQ27meR1R, -1); 
  hQ27meR1->Draw("same Weight");
  cQ2meR12->cd(4);
  hQ28meR1->Add(hQ28meR1R, -1); 
  hQ28meR1->Draw("same Weight");
  cQ2meR12->Print(outputpdf);

  TCanvas *cQ2meR21 = new TCanvas("cQ2meR21", "cQ2meR21");

  cQ2meR21->Divide(2, 2);
  cQ2meR21->cd(1);
  hQ21meR2->Add(hQ21meR2R, -1); 
  hQ21meR2->Draw(" Weight");
  cQ2meR21->cd(2);
  hQ22meR2->Add(hQ22meR2R, -1); 
  hQ22meR2->Draw("same Weight");
  cQ2meR21->cd(3);
  hQ23meR2->Add(hQ23meR2R, -1); 
  hQ23meR2->Draw("same Weight");
  cQ2meR21->cd(4);
  hQ24meR2->Add(hQ24meR2R, -1); 
  hQ24meR2->Draw("same Weight");
  cQ2meR21->Print(outputpdf);

  TCanvas *cQ2meR22 = new TCanvas("cQ2meR22", "cQ2meR22");

  cQ2meR22->Divide(2, 2);
  cQ2meR22->cd(1);
  hQ25meR2->Add(hQ25meR2R, -1); 
  hQ25meR2->Draw(" Weight");
  cQ2meR22->cd(2);
  hQ26meR2->Add(hQ26meR2R, -1); 
  hQ26meR2->Draw("same Weight");
  cQ2meR22->cd(3);
  hQ27meR2->Add(hQ27meR2R, -1); 
  hQ27meR2->Draw("same Weight");
  cQ2meR22->cd(4);
  hQ28meR2->Add(hQ28meR2R, -1); 
  hQ28meR2->Draw("same Weight");
  cQ2meR22->Print(outputpdf);

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

  TCanvas *ctmeR11 = new TCanvas("ctmeR11", "ctmeR11");
  ctmeR11->Divide(2, 2);
  ctmeR11->cd(1);
  ht1meR1->Add(ht1meR1R, -1);
  ht1meR1->Draw("Weight");
  ctmeR11->cd(2);
  ht2meR1->Add(ht2meR1R, -1);
  ht2meR1->Draw("same Weight");
  ctmeR11->cd(3);
  ht3meR1->Add(ht3meR1R, -1);
  ht3meR1->Draw("same Weight");
  ctmeR11->cd(4);
  ht4meR1->Add(ht4meR1R, -1);
  ht4meR1->Draw("same Weight");
  ctmeR11->Print(outputpdf);

  TCanvas *ctmeR12 = new TCanvas("ctmeR12", "ctmeR12");
  ctmeR12->Divide(2, 2);
  ctmeR12->cd(1);
  ht5meR1->Add(ht5meR1R, -1);
  ht5meR1->Draw("Weight");
  ctmeR12->cd(2);
  ht6meR1->Add(ht6meR1R, -1);
  ht6meR1->Draw("same Weight");
  ctmeR12->cd(3);
  ht7meR1->Add(ht7meR1R, -1);
  ht7meR1->Draw("same Weight");
  ctmeR12->cd(4);
  ht8meR1->Add(ht8meR1R, -1);
  ht8meR1->Draw("same Weight");
  ctmeR12->Print(outputpdf);

  TCanvas *ctmeR21 = new TCanvas("ctmeR21", "ctmeR21");
  ctmeR21->Divide(2, 2);
  ctmeR21->cd(1);
  ht1meR2->Add(ht1meR2R, -1);
  ht1meR2->Draw("Weight");
  ctmeR21->cd(2);
  ht2meR2->Add(ht2meR2R, -1);
  ht2meR2->Draw("same Weight");
  ctmeR21->cd(3);
  ht3meR2->Add(ht3meR2R, -1);
  ht3meR2->Draw("same Weight");
  ctmeR21->cd(4);
  ht4meR2->Add(ht4meR2R, -1);
  ht4meR2->Draw("same Weight");
  ctmeR21->Print(outputpdf);

  TCanvas *ctmeR22 = new TCanvas("ctmeR22", "ctmeR22");
  ctmeR22->Divide(2, 2);
  ctmeR22->cd(1);
  ht5meR2->Add(ht5meR2R, -1);
  ht5meR2->Draw("Weight");
  ctmeR22->cd(2);
  ht6meR2->Add(ht6meR2R, -1);
  ht6meR2->Draw("same Weight");
  ctmeR22->cd(3);
  ht7meR2->Add(ht7meR2R, -1);
  ht7meR2->Draw("same Weight");
  ctmeR22->cd(4);
  ht8meR2->Add(ht8meR2R, -1);
  ht8meR2->Draw("same Weight");
  ctmeR22->Print(outputpdf);

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
  ofstream outmeR1("kindata/kindata.pl_375_629_+2000.dat");
  ofstream outmeR2("kindata/kindata.pl_375_629_+3120.dat");

  ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
  ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
  ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
  ofstream outheR1("kindata/kindata.pl_375_781_+2680.dat");
  **/

  ofstream outmeR1("kindata/kindata.pl_375_629_+2000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outmeR1<<1.000<<endl;
      if(i==1)
	outmeR1<<hW1meR1->GetMean()<<"\t"<<hW1meR1->GetMeanError()<<"\t"<<hQ21meR1->GetMean()<<"\t"<<hQ21meR1->GetMeanError()<<"\t"<<ht1meR1->GetMean()<<"\t"<<ht1meR1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outmeR1<<hW2meR1->GetMean()<<"\t"<<hW2meR1->GetMeanError()<<"\t"<<hQ22meR1->GetMean()<<"\t"<< hQ22meR1->GetMeanError()<<"\t"<<ht2meR1->GetMean()<<"\t"<<ht2meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outmeR1<<hW3meR1->GetMean()<<"\t"<<hW3meR1->GetMeanError()<<"\t"<<hQ23meR1->GetMean()<<"\t"<< hQ23meR1->GetMeanError()<<"\t"<<ht3meR1->GetMean()<<"\t"<<ht3meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outmeR1<<hW4meR1->GetMean()<<"\t"<<hW4meR1->GetMeanError()<<"\t"<<hQ24meR1->GetMean()<<"\t"<< hQ24meR1->GetMeanError()<<"\t"<<ht4meR1->GetMean()<<"\t"<<ht4meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outmeR1<<hW5meR1->GetMean()<<"\t"<<hW5meR1->GetMeanError()<<"\t"<<hQ25meR1->GetMean()<<"\t"<< hQ25meR1->GetMeanError()<<"\t"<<ht5meR1->GetMean()<<"\t"<<ht5meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outmeR1<<hW6meR1->GetMean()<<"\t"<<hW6meR1->GetMeanError()<<"\t"<<hQ26meR1->GetMean()<<"\t"<< hQ26meR1->GetMeanError()<<"\t"<<ht6meR1->GetMean()<<"\t"<<ht6meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outmeR1<<hW7meR1->GetMean()<<"\t"<<hW7meR1->GetMeanError()<<"\t"<<hQ27meR1->GetMean()<<"\t"<< hQ27meR1->GetMeanError()<<"\t"<<ht7meR1->GetMean()<<"\t"<<ht7meR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outmeR1<<hW8meR1->GetMean()<<"\t"<<hW8meR1->GetMeanError()<<"\t"<<hQ28meR1->GetMean()<<"\t"<< hQ28meR1->GetMeanError()<<"\t"<<ht8meR1->GetMean()<<"\t"<<ht8meR1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outmeR1<<1.000000<<endl;
    } 
  outmeR1.close();

  ofstream outmeR2("kindata/kindata.pl_375_629_+3120.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outmeR2<<1.000<<endl;
      if(i==1)
	outmeR2<<hW1meR2->GetMean()<<"\t"<<hW1meR2->GetMeanError()<<"\t"<<hQ21meR2->GetMean()<<"\t"<<hQ21meR2->GetMeanError()<<"\t"<<ht1meR2->GetMean()<<"\t"<<ht1meR2->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outmeR2<<hW2meR2->GetMean()<<"\t"<<hW2meR2->GetMeanError()<<"\t"<<hQ22meR2->GetMean()<<"\t"<< hQ22meR2->GetMeanError()<<"\t"<<ht2meR2->GetMean()<<"\t"<<ht2meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outmeR2<<hW3meR2->GetMean()<<"\t"<<hW3meR2->GetMeanError()<<"\t"<<hQ23meR2->GetMean()<<"\t"<< hQ23meR2->GetMeanError()<<"\t"<<ht3meR2->GetMean()<<"\t"<<ht3meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outmeR2<<hW4meR2->GetMean()<<"\t"<<hW4meR2->GetMeanError()<<"\t"<<hQ24meR2->GetMean()<<"\t"<< hQ24meR2->GetMeanError()<<"\t"<<ht4meR2->GetMean()<<"\t"<<ht4meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outmeR2<<hW5meR2->GetMean()<<"\t"<<hW5meR2->GetMeanError()<<"\t"<<hQ25meR2->GetMean()<<"\t"<< hQ25meR2->GetMeanError()<<"\t"<<ht5meR2->GetMean()<<"\t"<<ht5meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outmeR2<<hW6meR2->GetMean()<<"\t"<<hW6meR2->GetMeanError()<<"\t"<<hQ26meR2->GetMean()<<"\t"<< hQ26meR2->GetMeanError()<<"\t"<<ht6meR2->GetMean()<<"\t"<<ht6meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outmeR2<<hW7meR2->GetMean()<<"\t"<<hW7meR2->GetMeanError()<<"\t"<<hQ27meR2->GetMean()<<"\t"<< hQ27meR2->GetMeanError()<<"\t"<<ht7meR2->GetMean()<<"\t"<<ht7meR2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outmeR2<<hW8meR2->GetMean()<<"\t"<<hW8meR2->GetMeanError()<<"\t"<<hQ28meR2->GetMean()<<"\t"<< hQ28meR2->GetMeanError()<<"\t"<<ht8meR2->GetMean()<<"\t"<<ht8meR2->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outmeR2<<1.000000<<endl;
    } 
  outmeR2.close();

  ofstream outmeC("kindata/kindata.pl_375_629_+0000.dat");
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

  ofstream outmeL1("kindata/kindata.pl_375_629_-2000.dat");
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

  ofstream outmeL2("kindata/kindata.pl_375_629_-4000.dat");
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
  hph_qmeR2->SetStats(0);
  hph_qmeR2->SetLineColor(kYellow);
  hph_qmeR2->Add(hph_qmeR2R, -1);
  hph_qmeR2->Draw(" Weight");
  
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
 
  hph_qmeR1->SetStats(0);
  hph_qmeR1->SetLineColor(kBlue);
  hph_qmeL1->Add(hph_qmeL1R, -1);
  hph_qmeR1->Draw("same Weight");

  c10me->Print(outputpdf);

  TCanvas *c11me = new TCanvas("c11me", " c11me"); 

  hmmmeL1->SetStats(0);
  hmmmeL1->SetLineColor(kRed);
  hmmmeL1->Add(hmmmeL1R, -1);
  hmmmeL1->Draw(" Weight");

  hmmmeC->SetStats(0);
  hmmmeC->SetLineColor(kGreen);
  hmmmeC->Add(hmmmeCR, -1);
  hmmmeC->Draw("same Weight");

  hmmmeR2->SetStats(0);
  hmmmeR2->SetLineColor(kYellow);
  hmmmeR2->Add(hmmmeR2R, -1);
  hmmmeR2->Draw("same Weight");

  hmmmeL2->SetStats(0);
  hmmmeL2->SetLineColor(kBlack);
  hmmmeL2->Add(hmmmeL2R, -1);
  hmmmeL2->Draw("same Weight");

  hmmmeR1->SetStats(0);
  hmmmeR1->SetLineColor(kBlue);
  hmmmeR1->Add(hmmmeR1R, -1);
  hmmmeR1->Draw("same Weight");
  	    
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

  TCanvas *cYmeR1 = new TCanvas("cYmeR1", " cYmeR1");
  hYmeR1->Add(hYmeR1R, -1);  
  hYmeR1->Scale(SFMER1);  
  hYmeR1->SetStats(0);
  hYmeR1->Draw("COLZ");
  cYmeR1->Print(outputpdf);

  cout<< "R1 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYmeR2 = new TCanvas("cYmeR2", " cYmeR2");
  hYmeR2->Add(hYmeR2R, -1);  
  hYmeR2->Scale(SFMER2);  
  hYmeR2->SetStats(0);
  hYmeR2->Draw("COLZ");
  cYmeR2->Print(outputpdf);

  cout<< "R2 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYmeC = new TCanvas("cYmeC", " cYmeC");
  hYmeC->Add(hYmeCR, -1);  
  hYmeC->Scale(SFMEC);  
  hYmeC->SetStats(0);
  hYmeC->Draw("COLZ");
  cYmeC->Print(outputpdf);

  cout<< "C Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYmeL1 = new TCanvas("cYmeL1", " cYmeL1");
  hYmeL1->Add(hYmeL1R, -1);  
  hYmeL1->Scale(SFMEL1);  
  hYmeL1->SetStats(0);
  hYmeL1->Draw("COLZ");
  cYmeL1->Print(outputpdf);

  cout<< "L1 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYmeL2 = new TCanvas("cYmeL2", " cYmeL2");
  hYmeL2->Add(hYmeL2R, -1);  
  hYmeL2->Scale(SFMEL2);  
  hYmeL2->SetStats(0);
  hYmeL2->Draw("COLZ");
  cYmeL2->Print(outputpdf);

  cout<< "L2 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TH2D *Cloneme = (TH2D*)hYmeR1->Clone("Cloneme");
  Cloneme->Add(hYmeR2,+1);
  Cloneme->Add(hYmeC,+1);
  Cloneme->Add(hYmeL1,+1);
  Cloneme->Add(hYmeL2,+1);
  Cloneme->SetTitle("Total Yield for mid epsilon");
  TCanvas *cYtme = new TCanvas("cYtme", " cYtme");
  Cloneme->Draw("COLZ");
  cYtme->Print(outputpdf);

  cout<< "Total Yield for mid epsilon"<<endl;
  cout<<"..."<<endl;

  cout<<"End of the mid e Yield calculations "<<endl;

  //High epsilon analysis Feb 07, 2023

  cout<<" Analysing high epsilon data..." <<endl;

  TString TInDATAFilenameHigheR1 = "Analysed_Data_Q0p375W2p2_highe_Right1.root";
  TString TInDATAFilenameHigheC  = "Analysed_Data_Q0p375W2p2_highe_Center.root";
  TString TInDATAFilenameHigheL1 = "Analysed_Data_Q0p375W2p2_highe_Left1.root";
  TString TInDATAFilenameHigheL2 = "Analysed_Data_Q0p375W2p2_highe_Left2.root";
 
  TString rootFile_DATAHIGHER1  = ROOTfilePath+"/"+TInDATAFilenameHigheR1;
  TString rootFile_DATAHIGHEC  = ROOTfilePath+"/"+TInDATAFilenameHigheC;
  TString rootFile_DATAHIGHEL1 = ROOTfilePath+"/"+TInDATAFilenameHigheL1;
  TString rootFile_DATAHIGHEL2 = ROOTfilePath+"/"+TInDATAFilenameHigheL2;

  if (gSystem->AccessPathName(rootFile_DATAHIGHER1) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAHIGHER1 <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
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
  
  TFile *InFile_DATAHIGHER1 = new TFile(rootFile_DATAHIGHER1, "READ");
  TFile *InFile_DATAHIGHEC = new TFile(rootFile_DATAHIGHEC, "READ");
  TFile *InFile_DATAHIGHEL1 = new TFile(rootFile_DATAHIGHEL1, "READ");
  TFile *InFile_DATAHIGHEL2 = new TFile(rootFile_DATAHIGHEL2, "READ");

  // TREES
 
  TTree* TBRANCHHIGHER1  = (TTree*)InFile_DATAHIGHER1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHER1  = (Long64_t)TBRANCHHIGHER1->GetEntries();  
  TTree* TBRANCHHIGHEC  = (TTree*)InFile_DATAHIGHEC->Get("Cut_Kaon_Events_prompt_noRF"); Long64_t nEntries_TBRANCHHIGHEC  = (Long64_t)TBRANCHHIGHEC->GetEntries();  
  TTree* TBRANCHHIGHEL1  = (TTree*)InFile_DATAHIGHEL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEL1  = (Long64_t)TBRANCHHIGHEL1->GetEntries();  
  TTree* TBRANCHHIGHEL2  = (TTree*)InFile_DATAHIGHEL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHHIGHEL2  = (Long64_t)TBRANCHHIGHEL2->GetEntries();  
  
  //VARIABLES

  Double_t WHER1;TBRANCHHIGHER1->SetBranchAddress("W", &WHER1);
  Double_t Q2HER1;TBRANCHHIGHER1->SetBranchAddress("Q2", &Q2HER1);

  Double_t WHEC;TBRANCHHIGHEC->SetBranchAddress("W", &WHEC);
  Double_t Q2HEC;TBRANCHHIGHEC->SetBranchAddress("Q2", &Q2HEC);
 
  Double_t WHEL1;TBRANCHHIGHEL1->SetBranchAddress("W", &WHEL1);
  Double_t Q2HEL1;TBRANCHHIGHEL1->SetBranchAddress("Q2", &Q2HEL1);
 
  Double_t WHEL2;TBRANCHHIGHEL2->SetBranchAddress("W", &WHEL2);
  Double_t Q2HEL2;TBRANCHHIGHEL2->SetBranchAddress("Q2", &Q2HEL2);
  
  Double_t theR1;TBRANCHHIGHER1->SetBranchAddress("MandelT", &theR1);
  Double_t theC;TBRANCHHIGHEC->SetBranchAddress("MandelT", &theC);
  Double_t theL1;TBRANCHHIGHEL1->SetBranchAddress("MandelT", &theL1);
  Double_t theL2;TBRANCHHIGHEL2->SetBranchAddress("MandelT", &theL2);

  Double_t ph_qheR1;TBRANCHHIGHER1->SetBranchAddress("ph_q", &ph_qheR1);
  Double_t ph_qheC;TBRANCHHIGHEC->SetBranchAddress("ph_q", &ph_qheC);
  Double_t ph_qheL1;TBRANCHHIGHEL1->SetBranchAddress("ph_q", &ph_qheL1);
  Double_t ph_qheL2;TBRANCHHIGHEL2->SetBranchAddress("ph_q", &ph_qheL2);

  Double_t mmheR1;TBRANCHHIGHER1->SetBranchAddress("MM", &mmheR1);
  Double_t mmheC;TBRANCHHIGHEC->SetBranchAddress("MM", &mmheC);
  Double_t mmheL1;TBRANCHHIGHEL1->SetBranchAddress("MM", &mmheL1);
  Double_t mmheL2;TBRANCHHIGHEL2->SetBranchAddress("MM", &mmheL2);

  Double_t tcoinheR1;TBRANCHHIGHER1->SetBranchAddress("CTime_ROC1", &tcoinheR1);
  Double_t tcoinheC;TBRANCHHIGHEC->SetBranchAddress("CTime_ROC1", &tcoinheC);
  Double_t tcoinheL1;TBRANCHHIGHEL1->SetBranchAddress("CTime_ROC1", &tcoinheL1);
  Double_t tcoinheL2;TBRANCHHIGHEL2->SetBranchAddress("CTime_ROC1", &tcoinheL2);

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
  TH1D *htheC   = new TH1D("htheC","MandelT; MandelT;",    300, -0.01, 0.1);      
  TH1D *htheL1  = new TH1D("htheL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2  = new TH1D("htheL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheInR1  = new TH1D("htheInR1","MandelT; MandelT;",   10, bins);      
  TH1D *htheInC   = new TH1D("htheInC","MandelT; MandelT;",    10, bins);      
  TH1D *htheInL1  = new TH1D("htheInL1","MandelT; MandelT;",   10, bins);      
  TH1D *htheInL2  = new TH1D("htheInL2","MandelT; MandelT;",   10, bins);      

  TH1D *htheR1R  = new TH1D("htheR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheCR   = new TH1D("htheCR","MandelT; MandelT;",     300, -0.01, 0.1);      
  TH1D *htheL1R  = new TH1D("htheL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2R  = new TH1D("htheL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheInR1R  = new TH1D("htheInR1R","MandelT; MandelT;",   10, bins);      
  TH1D *htheInCR   = new TH1D("htheInCR","MandelT; MandelT;",    10, bins);      
  TH1D *htheInL1R  = new TH1D("htheInL1R","MandelT; MandelT;",   10, bins);      
  TH1D *htheInL2R  = new TH1D("htheInL2R","MandelT; MandelT;",   10, bins);      

  TH1D *hph_qheR1  = new TH1D("hph_qheR1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheC   = new TH1D("hph_qheC","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qheL1  = new TH1D("hph_qheL1","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheL2  = new TH1D("hph_qheL2","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH1D *hph_qheR1R  = new TH1D("hph_qheR1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheCR   = new TH1D("hph_qheCR","ph_q; ph_q;",    18, -22.5, 382.5);      
  TH1D *hph_qheL1R  = new TH1D("hph_qheL1R","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qheL2R  = new TH1D("hph_qheL2R","ph_q; ph_q;",   18, -22.5, 382.5);      

  TH2D *hYheR1   = new TH2D("hYheR1"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheC    = new TH2D("hYheC"," Yield C;t-Bin; Phi-Bin ",    10, bins, 18, -22.5, 382.5);
  TH2D *hYheL1   = new TH2D("hYheL1"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheL2   = new TH2D("hYheL2"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);

  TH2D *hYheR1R   = new TH2D("hYheR1R"," Yield R1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheCR    = new TH2D("hYheCR","  Yield C;t-Bin; Phi-Bin ",   10, bins, 18, -22.5, 382.5);
  TH2D *hYheL1R   = new TH2D("hYheL1R"," Yield L1;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);
  TH2D *hYheL2R   = new TH2D("hYheL2R"," Yield L2;t-Bin; Phi-Bin ",  10, bins, 18, -22.5, 382.5);      


  TH1D *hmmheR1  = new TH1D("hmmheR1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheC   = new TH1D("hmmheC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL1  = new TH1D("hmmheL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL2  = new TH1D("hmmheL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmheR1R  = new TH1D("hmmheR1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheCR   = new TH1D("hmmheCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL1R  = new TH1D("hmmheL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmheL2R  = new TH1D("hmmheL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinheR1   = new TH1D("htcoinheR1","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinheC    = new TH1D("htcoinheC","CTime_ROC1; CTime_ROC1;",   300, -20.0, 20.0);      
  TH1D *htcoinheL1   = new TH1D("htcoinheL1","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinheL2   = new TH1D("htcoinheL2","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  
  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHER1; i++)

    {
      TBRANCHHIGHER1->GetEntry(i);
      
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0)    
	{
	  hmmheR1->Fill(mmheR1);	  
	}	      
      
      {
	htcoinheR1->Fill(tcoinheR1);
      }
      Double_t Diamond_cut = (Dcut->IsInside(Q2HER1, WHER1));  
      
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut)
	{
	  htheR1->Fill(-theR1);
	  htheInR1->Fill(-theR1);
	  hph_qheR1->Fill(ph_qheR1*57.2958 + 180);
	  hYheR1->Fill(-theR1, ph_qheR1*57.2958 + 180);
	}
      
      if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut)
	{
	  htheR1R->Fill(-theR1);
	  htheInR1R->Fill(-theR1);
	  hph_qheR1R->Fill(ph_qheR1*57.2958 + 180);
	  hYheR1R->Fill(-theR1, ph_qheR1*57.2958 + 180);
	}
      Double_t w1t = -theR1 >= bins[1] && -theR1 <= bins[2];
      Double_t w2t = -theR1 >= bins[2] && -theR1 <= bins[3];
      Double_t w3t = -theR1 >= bins[3] && -theR1 <= bins[4];
      Double_t w4t = -theR1 >= bins[4] && -theR1 <= bins[5];
      Double_t w5t = -theR1 >= bins[5] && -theR1 <= bins[6];
      Double_t w6t = -theR1 >= bins[6] && -theR1 <= bins[7];
      Double_t w7t = -theR1 >= bins[7] && -theR1 <= bins[8];
      Double_t w8t = -theR1 >= bins[8] && -theR1 <= bins[9];

      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w1t)
	{
	  hW1heR1->Fill(WHER1);	
	  hQ21heR1->Fill(Q2HER1);	      	  
	  ht1heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w2t)
	{
	  hW2heR1->Fill(WHER1);	
	  hQ22heR1->Fill(Q2HER1);	      	  
	  ht2heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w3t)
	{
	  hW3heR1->Fill(WHER1);	
	  hQ23heR1->Fill(Q2HER1);	      	  
	  ht3heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w4t)
	{
	  hW4heR1->Fill(WHER1);	
	  hQ24heR1->Fill(Q2HER1);	      	  
	  ht4heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w5t)
	{
	  hW5heR1->Fill(WHER1);	
	  hQ25heR1->Fill(Q2HER1);	      	  
	  ht5heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w6t)
	{
	  hW6heR1->Fill(WHER1);	
	  hQ26heR1->Fill(Q2HER1);	      	  
	  ht6heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w7t)
	{
	  hW7heR1->Fill(WHER1);	
	  hQ27heR1->Fill(Q2HER1);	      	  
	  ht7heR1->Fill(-theR1); 
 	}	
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w8t)
	{
	  hW8heR1->Fill(WHER1);	
	  hQ28heR1->Fill(Q2HER1);	      	  
	  ht8heR1->Fill(-theR1); 
 	}	

      if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1heR1R->Fill(WHER1);	
	  hQ21heR1R->Fill(Q2HER1);	      	  
	  ht1heR1R->Fill(-theR1); 
	}

   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2heR1R->Fill(WHER1);	
	  hQ22heR1R->Fill(Q2HER1);	      	  
	  ht2heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3heR1R->Fill(WHER1);	
	  hQ23heR1R->Fill(Q2HER1);	      	  
	  ht3heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4heR1R->Fill(WHER1);	
	  hQ24heR1R->Fill(Q2HER1);	      	  
	  ht4heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5heR1R->Fill(WHER1);	
	  hQ25heR1R->Fill(Q2HER1);	      	  
	  ht5heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6heR1R->Fill(WHER1);	
	  hQ26heR1R->Fill(Q2HER1);	      	  
	  ht6heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7heR1R->Fill(WHER1);	
	  hQ27heR1R->Fill(Q2HER1);	      	  
	  ht7heR1R->Fill(-theR1); 
	}
   if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8heR1R->Fill(WHER1);	
	  hQ28heR1R->Fill(Q2HER1);	      	  
	  ht8heR1R->Fill(-theR1); 
	}

      if (((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)))
	
	{
	  hmmheR1R->Fill(mmheR1);
	}	
    }  

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEC; i++)

    {
      TBRANCHHIGHEC->GetEntry(i);
      
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98)
	{
	  hQ2WHEC->Fill(Q2HEC, WHEC);
	}
 
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0)    
 	{
	  hmmheC->Fill(mmheC);	  
	}	      
      
      {
	htcoinheC->Fill(tcoinheC);
      }
      
      Double_t Diamond_cut = (Dcut->IsInside(Q2HEC, WHEC));  
      
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut)
	{
	  hQ2WHEC1->Fill(Q2HEC, WHEC);	      	  
	  htheC->Fill(-theC);
	  htheInC->Fill(-theC);
	  hph_qheC->Fill(ph_qheC*57.2958 + 180);
	  hYheC->Fill(-theC, ph_qheC*57.2958 + 180);
	}	      

           
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut)
  	{
	  htheCR->Fill(-theC);
	  htheInCR->Fill(-theC);
	  hph_qheCR->Fill(ph_qheC*57.2958 + 180);
	  hQ2WHECR->Fill(Q2HEC, WHEC);
	  hYheCR->Fill(-theC, ph_qheC*57.2958 + 180);
	}
      
      Double_t w1t = -theC >= bins[1] && -theC <= bins[2];
      Double_t w2t = -theC >= bins[2] && -theC <= bins[3];
      Double_t w3t = -theC >= bins[3] && -theC <= bins[4];
      Double_t w4t = -theC >= bins[4] && -theC <= bins[5];
      Double_t w5t = -theC >= bins[5] && -theC <= bins[6];
      Double_t w6t = -theC >= bins[6] && -theC <= bins[7];
      Double_t w7t = -theC >= bins[7] && -theC <= bins[8];
      Double_t w8t = -theC >= bins[8] && -theC <= bins[9];

      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w1t)
	{
	  hW1heC->Fill(WHEC);	
	  hQ21heC->Fill(Q2HEC);	      	  
	  ht1heC->Fill(-theC); 
 	}	

      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w2t)
	{
	  hW2heC->Fill(WHEC);	
	  hQ22heC->Fill(Q2HEC);	      	  
	  ht2heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w3t)
	{
	  hW3heC->Fill(WHEC);	
	  hQ23heC->Fill(Q2HEC);	      	  
	  ht3heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w4t)
	{
	  hW4heC->Fill(WHEC);	
	  hQ24heC->Fill(Q2HEC);	      	  
	  ht4heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w5t)
	{
	  hW5heC->Fill(WHEC);	
	  hQ25heC->Fill(Q2HEC);	      	  
	  ht5heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w6t)
	{
	  hW6heC->Fill(WHEC);	
	  hQ26heC->Fill(Q2HEC);	      	  
	  ht6heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w7t)
	{
	  hW7heC->Fill(WHEC);	
	  hQ27heC->Fill(Q2HEC);	      	  
	  ht7heC->Fill(-theC); 
 	}	
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w8t)
	{
	  hW8heC->Fill(WHEC);	
	  hQ28heC->Fill(Q2HEC);	      	  
	  ht8heC->Fill(-theC); 
 	}	

      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1heCR->Fill(WHEC);	
	  hQ21heCR->Fill(Q2HEC);	      	  
	  ht1heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2heCR->Fill(WHEC);	
	  hQ22heCR->Fill(Q2HEC);	      	  
	  ht2heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3heCR->Fill(WHEC);	
	  hQ23heCR->Fill(Q2HEC);	      	  
	  ht3heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4heCR->Fill(WHEC);	
	  hQ24heCR->Fill(Q2HEC);	      	  
	  ht4heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5heCR->Fill(WHEC);	
	  hQ25heCR->Fill(Q2HEC);	      	  
	  ht5heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6heCR->Fill(WHEC);	
	  hQ26heCR->Fill(Q2HEC);	      	  
	  ht6heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7heCR->Fill(WHEC);	
	  hQ27heCR->Fill(Q2HEC);	      	  
	  ht7heCR->Fill(-theC); 
	}
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8heCR->Fill(WHEC);	
	  hQ28heCR->Fill(Q2HEC);	      	  
	  ht8heCR->Fill(-theC); 
	}

      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)))
	
	{
	  hmmheCR->Fill(mmheC);
	}	
    }  


  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL1; i++)
  
    {
      TBRANCHHIGHEL1->GetEntry(i);
      
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0)    
	{
	  hmmheL1->Fill(mmheL1);
	}           
      
      {
	htcoinheL1->Fill(tcoinheL1);
      }

      Double_t Diamond_cut = (Dcut->IsInside(Q2HEL1, WHEL1));  

      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut)
	{
	  htheL1->Fill(-theL1);
	  htheInL1->Fill(-theL1);
	  hph_qheL1->Fill(ph_qheL1*57.2958 + 180);	  
	  hYheL1->Fill(-theL1, ph_qheL1*57.2958 + 180);
	}      

      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut)
 	
	{
	  htheL1R->Fill(-theL1);
	  htheInL1R->Fill(-theL1);
	  hph_qheL1R->Fill(ph_qheL1*57.2958 + 180);	  
	  hYheL1R->Fill(-theL1, ph_qheL1*57.2958 + 180);
	}
      Double_t w1t = -theL1 >= bins[1] && -theL1 <= bins[2];
      Double_t w2t = -theL1 >= bins[2] && -theL1 <= bins[3];
      Double_t w3t = -theL1 >= bins[3] && -theL1 <= bins[4];
      Double_t w4t = -theL1 >= bins[4] && -theL1 <= bins[5];
      Double_t w5t = -theL1 >= bins[5] && -theL1 <= bins[6];
      Double_t w6t = -theL1 >= bins[6] && -theL1 <= bins[7];
      Double_t w7t = -theL1 >= bins[7] && -theL1 <= bins[8];
      Double_t w8t = -theL1 >= bins[8] && -theL1 <= bins[9];

      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w1t)
	{
	  hW1heL1->Fill(WHEL1);	
	  hQ21heL1->Fill(Q2HEL1);	      	  
	  ht1heL1->Fill(-theL1); 
 	}	
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w2t)
	{
	  hW2heL1->Fill(WHEL1);	
	  hQ22heL1->Fill(Q2HEL1);	      	  
	  ht2heL1->Fill(-theL1); 
 	}	
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w3t)
	{
	  hW3heL1->Fill(WHEL1);	
	  hQ23heL1->Fill(Q2HEL1);	      	  
	  ht3heL1->Fill(-theL1); 
 	}	
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w4t)
	{
	  hW4heL1->Fill(WHEL1);	
	  hQ24heL1->Fill(Q2HEL1);	      	  
	  ht4heL1->Fill(-theL1); 
 	}	
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w5t)
	{
	  hW5heL1->Fill(WHEL1);	
	  hQ25heL1->Fill(Q2HEL1);	      	  
	  ht5heL1->Fill(-theL1); 
 	}	
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w6t)
	{
	  hW6heL1->Fill(WHEL1);	
	  hQ26heL1->Fill(Q2HEL1);	      	  
	  ht6heL1->Fill(-theL1); 
 	}	
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w7t)
	{
	  hW7heL1->Fill(WHEL1);	
	  hQ27heL1->Fill(Q2HEL1);	      	  
	  ht7heL1->Fill(-theL1); 
 	}	
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w8t)
	{
	  hW8heL1->Fill(WHEL1);	
	  hQ28heL1->Fill(Q2HEL1);	      	  
	  ht8heL1->Fill(-theL1); 
 	}	
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1heL1R->Fill(WHEL1);	
	  hQ21heL1R->Fill(Q2HEL1);	      	  
	  ht1heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2heL1R->Fill(WHEL1);	
	  hQ22heL1R->Fill(Q2HEL1);	      	  
	  ht2heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3heL1R->Fill(WHEL1);	
	  hQ23heL1R->Fill(Q2HEL1);	      	  
	  ht3heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4heL1R->Fill(WHEL1);	
	  hQ24heL1R->Fill(Q2HEL1);	      	  
	  ht4heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5heL1R->Fill(WHEL1);	
	  hQ25heL1R->Fill(Q2HEL1);	      	  
	  ht5heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6heL1R->Fill(WHEL1);	
	  hQ26heL1R->Fill(Q2HEL1);	      	  
	  ht6heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7heL1R->Fill(WHEL1);	
	  hQ27heL1R->Fill(Q2HEL1);	      	  
	  ht7heL1R->Fill(-theL1); 
	}
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8heL1R->Fill(WHEL1);	
	  hQ28heL1R->Fill(Q2HEL1);	      	  
	  ht8heL1R->Fill(-theL1); 
	}
  
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)))
	
	{	 
	  hmmheL1R->Fill(mmheL1);
	}     
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL2; i++)
    {
      TBRANCHHIGHEL2->GetEntry(i);
            
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0)
	
	{
	  hmmheL2->Fill(mmheL2);
	}   
      
      {
	htcoinheL2->Fill(tcoinheL2);
      }

      Double_t Diamond_cut = (Dcut->IsInside(Q2HEL2, WHEL2));  

      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut)
  
	{
	  htheL2->Fill(-theL2);
	  htheInL2->Fill(-theL2);
	  hph_qheL2->Fill(ph_qheL2*57.2958 + 180);
	  hYheL2->Fill(-theL2, ph_qheL2*57.2958 + 180);
	}

      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut)
	{
	  htheL2R->Fill(-theL2);
	  htheInL2R->Fill(-theL2);
	  hph_qheL2R->Fill(ph_qheL2*57.2958 + 180);
	  hYheL2R->Fill(-theL2, ph_qheL2*57.2958 + 180);
	}
      Double_t w1t = -theL2 >= bins[1] && -theL2 <= bins[2];
      Double_t w2t = -theL2 >= bins[2] && -theL2 <= bins[3];
      Double_t w3t = -theL2 >= bins[3] && -theL2 <= bins[4];
      Double_t w4t = -theL2 >= bins[4] && -theL2 <= bins[5];
      Double_t w5t = -theL2 >= bins[5] && -theL2 <= bins[6];
      Double_t w6t = -theL2 >= bins[6] && -theL2 <= bins[7];
      Double_t w7t = -theL2 >= bins[7] && -theL2 <= bins[8];
      Double_t w8t = -theL2 >= bins[8] && -theL2 <= bins[9];

      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w1t)
	{
	  hW1heL2->Fill(WHEL2);	
	  hQ21heL2->Fill(Q2HEL2);	      	  
	  ht1heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w2t)
	{
	  hW2heL2->Fill(WHEL2);	
	  hQ22heL2->Fill(Q2HEL2);	      	  
	  ht2heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w3t)
	{
	  hW3heL2->Fill(WHEL2);	
	  hQ23heL2->Fill(Q2HEL2);	      	  
	  ht3heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w4t)
	{
	  hW4heL2->Fill(WHEL2);	
	  hQ24heL2->Fill(Q2HEL2);	      	  
	  ht4heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w5t)
	{
	  hW5heL2->Fill(WHEL2);	
	  hQ25heL2->Fill(Q2HEL2);	      	  
	  ht5heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w6t)
	{
	  hW6heL2->Fill(WHEL2);	
	  hQ26heL2->Fill(Q2HEL2);	      	  
	  ht6heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w7t)
	{
	  hW7heL2->Fill(WHEL2);	
	  hQ27heL2->Fill(Q2HEL2);	      	  
	  ht7heL2->Fill(-theL2); 
 	}	
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w8t)
	{
	  hW8heL2->Fill(WHEL2);	
	  hQ28heL2->Fill(Q2HEL2);	      	  
	  ht8heL2->Fill(-theL2); 
 	}	
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w1t)
	
	{
	  hW1heL2R->Fill(WHEL2);	
	  hQ21heL2R->Fill(Q2HEL2);	      	  
	  ht1heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w2t)
	
	{
	  hW2heL2R->Fill(WHEL2);	
	  hQ22heL2R->Fill(Q2HEL2);	      	  
	  ht2heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w3t)
	
	{
	  hW3heL2R->Fill(WHEL2);	
	  hQ23heL2R->Fill(Q2HEL2);	      	  
	  ht3heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w4t)
	
	{
	  hW4heL2R->Fill(WHEL2);	
	  hQ24heL2R->Fill(Q2HEL2);	      	  
	  ht4heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w5t)
	
	{
	  hW5heL2R->Fill(WHEL2);	
	  hQ25heL2R->Fill(Q2HEL2);	      	  
	  ht5heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w6t)
	
	{
	  hW6heL2R->Fill(WHEL2);	
	  hQ26heL2R->Fill(Q2HEL2);	      	  
	  ht6heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w7t)
	
	{
	  hW7heL2R->Fill(WHEL2);	
	  hQ27heL2R->Fill(Q2HEL2);	      	  
	  ht7heL2R->Fill(-theL2); 
	}
      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut && w8t)
	
	{
	  hW8heL2R->Fill(WHEL2);	
	  hQ28heL2R->Fill(Q2HEL2);	      	  
	  ht8heL2R->Fill(-theL2); 
	}

      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)))
	{
	  hmmheL2R->Fill(mmheL2);
	}
    }
  
  hQ2WHECR->Scale(1.0/6.0);

  hW1heR1R->Scale(1.0/6.0);
  hW2heR1R->Scale(1.0/6.0);
  hW3heR1R->Scale(1.0/6.0);
  hW4heR1R->Scale(1.0/6.0);
  hW5heR1R->Scale(1.0/6.0);
  hW6heR1R->Scale(1.0/6.0);
  hW7heR1R->Scale(1.0/6.0);
  hW8heR1R->Scale(1.0/6.0);

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
  hmmheCR->Scale(1.0/6.0);
  hmmheL1R->Scale(1.0/6.0);
  hmmheL2R->Scale(1.0/6.0);

  htheR1R->Scale(1.0/6.0);
  htheCR->Scale(1.0/6.0);
  htheL1R->Scale(1.0/6.0);
  htheL2R->Scale(1.0/6.0);

  htheInR1R->Scale(1.0/6.0);
  htheInCR->Scale(1.0/6.0);
  htheInL1R->Scale(1.0/6.0);
  htheInL2R->Scale(1.0/6.0);


  hph_qheR1R->Scale(1.0/6.0);
  hph_qheCR->Scale(1.0/6.0);
  hph_qheL1R->Scale(1.0/6.0);
  hph_qheL2R->Scale(1.0/6.0);

  hYheR1R->Scale(1.0/6.0);
  hYheCR->Scale(1.0/6.0);
  hYheL1R->Scale(1.0/6.0);
  hYheL2R->Scale(1.0/6.0);

  Double_t SFHER1  = 1.0/132.549;
  Double_t SFHEC   = 1.0/380.856;
  Double_t SFHEL1  = 1.0/183.703;
  Double_t SFHEL2  = 1.0/196.02;
  

  TCanvas *c8 = new TCanvas("c8", " tcoin"); 
 
  htcoinheC->SetStats(0);
  htcoinheC->SetLineColor(kGreen);
  htcoinheC->Draw("Weight");

  htcoinheR1->SetStats(0);
  htcoinheR1->SetLineColor(kBlue);
  htcoinheR1->Draw(" same Weight");

  htcoinheL2->SetStats(0);
  htcoinheL2->SetLineColor(kBlack);
  htcoinheL2->Draw("same Weight");

  htcoinheL1->SetStats(0);
  htcoinheL1->SetLineColor(kRed);
  htcoinheL1->Draw("same Weight ");
  
  
  TLine *l1he = new TLine(-1.0, 0.0, -1.0, 300000.0);
  TLine *l2he = new TLine(1.0, 0.0, 1.0, 300000.0);
  TLine *l3he = new TLine(-15.0, 0.0, -15.0, 300000.0);
  TLine *l4he = new TLine(-9.0, 0.0, -9.0, 300000.0);
  TLine *l5he = new TLine(7.0, 0.0, 7.0, 300000.0);
  TLine *l6he = new TLine(13.0, 0.0, 13.0, 300000.0);
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
     
  auto leghe = new TLegend(0.1,0.7,0.30,0.9);
  leghe->SetHeader("High epsilon","C");
  leghe->AddEntry(htcoinheR1,"Right1","l");
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

  /**
  ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
  ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
  ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
  ofstream outheR1("kindata/kindata.pl_375_781_+2680.dat");
  **/

  ofstream outheR1("kindata/kindata.pl_375_781_+2680.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheR1<<1.000<<endl;
      if(i==1)
	outheR1<<hW1meR1->GetMean()<<"\t"<<hW1heR1->GetMeanError()<<"\t"<<hQ21heR1->GetMean()<<"\t"<<hQ21heR1->GetMeanError()<<"\t"<<ht1heR1->GetMean()<<"\t"<<ht1heR1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheR1<<hW2meR1->GetMean()<<"\t"<<hW2heR1->GetMeanError()<<"\t"<<hQ22heR1->GetMean()<<"\t"<< hQ22heR1->GetMeanError()<<"\t"<<ht2heR1->GetMean()<<"\t"<<ht2heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheR1<<hW3meR1->GetMean()<<"\t"<<hW3heR1->GetMeanError()<<"\t"<<hQ23heR1->GetMean()<<"\t"<< hQ23heR1->GetMeanError()<<"\t"<<ht3heR1->GetMean()<<"\t"<<ht3heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheR1<<hW4meR1->GetMean()<<"\t"<<hW4heR1->GetMeanError()<<"\t"<<hQ24heR1->GetMean()<<"\t"<< hQ24heR1->GetMeanError()<<"\t"<<ht4heR1->GetMean()<<"\t"<<ht4heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheR1<<hW5meR1->GetMean()<<"\t"<<hW5heR1->GetMeanError()<<"\t"<<hQ25heR1->GetMean()<<"\t"<< hQ25heR1->GetMeanError()<<"\t"<<ht5heR1->GetMean()<<"\t"<<ht5heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheR1<<hW6meR1->GetMean()<<"\t"<<hW6heR1->GetMeanError()<<"\t"<<hQ26heR1->GetMean()<<"\t"<< hQ26heR1->GetMeanError()<<"\t"<<ht6heR1->GetMean()<<"\t"<<ht6heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheR1<<hW7meR1->GetMean()<<"\t"<<hW7heR1->GetMeanError()<<"\t"<<hQ27heR1->GetMean()<<"\t"<< hQ27heR1->GetMeanError()<<"\t"<<ht7heR1->GetMean()<<"\t"<<ht7heR1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheR1<<hW8meR1->GetMean()<<"\t"<<hW8heR1->GetMeanError()<<"\t"<<hQ28heR1->GetMean()<<"\t"<< hQ28heR1->GetMeanError()<<"\t"<<ht8heR1->GetMean()<<"\t"<<ht8heR1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheR1<<1.000000<<endl;
    } 
  outheR1.close();

  ofstream outheC("kindata/kindata.pl_375_781_+0000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheC<<1.000<<endl;
      if(i==1)
	outheC<<hW1meC->GetMean()<<"\t"<<hW1heC->GetMeanError()<<"\t"<<hQ21heC->GetMean()<<"\t"<<hQ21heC->GetMeanError()<<"\t"<<ht1heC->GetMean()<<"\t"<<ht1heC->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheC<<hW2meC->GetMean()<<"\t"<<hW2heC->GetMeanError()<<"\t"<<hQ22heC->GetMean()<<"\t"<< hQ22heC->GetMeanError()<<"\t"<<ht2heC->GetMean()<<"\t"<<ht2heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheC<<hW3meC->GetMean()<<"\t"<<hW3heC->GetMeanError()<<"\t"<<hQ23heC->GetMean()<<"\t"<< hQ23heC->GetMeanError()<<"\t"<<ht3heC->GetMean()<<"\t"<<ht3heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheC<<hW4meC->GetMean()<<"\t"<<hW4heC->GetMeanError()<<"\t"<<hQ24heC->GetMean()<<"\t"<< hQ24heC->GetMeanError()<<"\t"<<ht4heC->GetMean()<<"\t"<<ht4heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheC<<hW5meC->GetMean()<<"\t"<<hW5heC->GetMeanError()<<"\t"<<hQ25heC->GetMean()<<"\t"<< hQ25heC->GetMeanError()<<"\t"<<ht5heC->GetMean()<<"\t"<<ht5heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheC<<hW6meC->GetMean()<<"\t"<<hW6heC->GetMeanError()<<"\t"<<hQ26heC->GetMean()<<"\t"<< hQ26heC->GetMeanError()<<"\t"<<ht6heC->GetMean()<<"\t"<<ht6heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheC<<hW7meC->GetMean()<<"\t"<<hW7heC->GetMeanError()<<"\t"<<hQ27heC->GetMean()<<"\t"<< hQ27heC->GetMeanError()<<"\t"<<ht7heC->GetMean()<<"\t"<<ht7heC->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheC<<hW8meC->GetMean()<<"\t"<<hW8heC->GetMeanError()<<"\t"<<hQ28heC->GetMean()<<"\t"<< hQ28heC->GetMeanError()<<"\t"<<ht8heC->GetMean()<<"\t"<<ht8heC->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheC<<1.000000<<endl;
    } 
  outheC.close();

  ofstream outheL1("kindata/kindata.pl_375_781_-2015.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheL1<<1.000<<endl;
      if(i==1)
	outheL1<<hW1meL1->GetMean()<<"\t"<<hW1heL1->GetMeanError()<<"\t"<<hQ21heL1->GetMean()<<"\t"<<hQ21heL1->GetMeanError()<<"\t"<<ht1heL1->GetMean()<<"\t"<<ht1heL1->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheL1<<hW2meL1->GetMean()<<"\t"<<hW2heL1->GetMeanError()<<"\t"<<hQ22heL1->GetMean()<<"\t"<< hQ22heL1->GetMeanError()<<"\t"<<ht2heL1->GetMean()<<"\t"<<ht2heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheL1<<hW3meL1->GetMean()<<"\t"<<hW3heL1->GetMeanError()<<"\t"<<hQ23heL1->GetMean()<<"\t"<< hQ23heL1->GetMeanError()<<"\t"<<ht3heL1->GetMean()<<"\t"<<ht3heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheL1<<hW4meL1->GetMean()<<"\t"<<hW4heL1->GetMeanError()<<"\t"<<hQ24heL1->GetMean()<<"\t"<< hQ24heL1->GetMeanError()<<"\t"<<ht4heL1->GetMean()<<"\t"<<ht4heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheL1<<hW5meL1->GetMean()<<"\t"<<hW5heL1->GetMeanError()<<"\t"<<hQ25heL1->GetMean()<<"\t"<< hQ25heL1->GetMeanError()<<"\t"<<ht5heL1->GetMean()<<"\t"<<ht5heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheL1<<hW6meL1->GetMean()<<"\t"<<hW6heL1->GetMeanError()<<"\t"<<hQ26heL1->GetMean()<<"\t"<< hQ26heL1->GetMeanError()<<"\t"<<ht6heL1->GetMean()<<"\t"<<ht6heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheL1<<hW7meL1->GetMean()<<"\t"<<hW7heL1->GetMeanError()<<"\t"<<hQ27heL1->GetMean()<<"\t"<< hQ27heL1->GetMeanError()<<"\t"<<ht7heL1->GetMean()<<"\t"<<ht7heL1->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheL1<<hW8meL1->GetMean()<<"\t"<<hW8heL1->GetMeanError()<<"\t"<<hQ28heL1->GetMean()<<"\t"<< hQ28heL1->GetMeanError()<<"\t"<<ht8heL1->GetMean()<<"\t"<<ht8heL1->GetMeanError()<<"\t"<< i <<endl;      
      if(i==9)
	outheL1<<1.000000<<endl;
    } 
  outheL1.close();

  ofstream outheL2("kindata/kindata.pl_375_781_-4000.dat");
  for(int i=0 ; i <=9 ; ++i) 
    {
      if(i==0)
	outheL2<<1.000<<endl;
      if(i==1)
	outheL2<<hW1meL2->GetMean()<<"\t"<<hW1heL2->GetMeanError()<<"\t"<<hQ21heL2->GetMean()<<"\t"<<hQ21heL2->GetMeanError()<<"\t"<<ht1heL2->GetMean()<<"\t"<<ht1heL2->GetMeanError()<<"\t"<<i<<endl;
      if(i==2)
	outheL2<<hW2meL2->GetMean()<<"\t"<<hW2heL2->GetMeanError()<<"\t"<<hQ22heL2->GetMean()<<"\t"<< hQ22heL2->GetMeanError()<<"\t"<<ht2heL2->GetMean()<<"\t"<<ht2heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==3)
	outheL2<<hW3meL2->GetMean()<<"\t"<<hW3heL2->GetMeanError()<<"\t"<<hQ23heL2->GetMean()<<"\t"<< hQ23heL2->GetMeanError()<<"\t"<<ht3heL2->GetMean()<<"\t"<<ht3heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==4)
	outheL2<<hW4meL2->GetMean()<<"\t"<<hW4heL2->GetMeanError()<<"\t"<<hQ24heL2->GetMean()<<"\t"<< hQ24heL2->GetMeanError()<<"\t"<<ht4heL2->GetMean()<<"\t"<<ht4heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==5)
	outheL2<<hW5meL2->GetMean()<<"\t"<<hW5heL2->GetMeanError()<<"\t"<<hQ25heL2->GetMean()<<"\t"<< hQ25heL2->GetMeanError()<<"\t"<<ht5heL2->GetMean()<<"\t"<<ht5heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==6)
	outheL2<<hW6meL2->GetMean()<<"\t"<<hW6heL2->GetMeanError()<<"\t"<<hQ26heL2->GetMean()<<"\t"<< hQ26heL2->GetMeanError()<<"\t"<<ht6heL2->GetMean()<<"\t"<<ht6heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==7)
	outheL2<<hW7meL2->GetMean()<<"\t"<<hW7heL2->GetMeanError()<<"\t"<<hQ27heL2->GetMean()<<"\t"<< hQ27heL2->GetMeanError()<<"\t"<<ht7heL2->GetMean()<<"\t"<<ht7heL2->GetMeanError()<<"\t"<< i <<endl;
      if(i==8)
	outheL2<<hW8meL2->GetMean()<<"\t"<<hW8heL2->GetMeanError()<<"\t"<<hQ28heL2->GetMean()<<"\t"<< hQ28heL2->GetMeanError()<<"\t"<<ht8heL2->GetMean()<<"\t"<<ht8heL2->GetMeanError()<<"\t"<< i <<endl;      
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
  c10->Print(outputpdf);

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

  TCanvas *cYheR1 = new TCanvas("cYheR1", " cYheR1");
  hYheR1->Add(hYheR1R, -1);  
  hYheR1->Scale(SFHER1);  
  hYheR1->SetStats(0);
  hYheR1->Draw("COLZ");
  cYheR1->Print(outputpdf);

  cout<< "R1 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYheC = new TCanvas("cYheC", " cYheC");
  hYheC->Add(hYheCR, -1);  
  hYheC->Scale(SFHEC);  
  hYheC->SetStats(0);
  hYheC->Draw("COLZ");
  cYheC->Print(outputpdf);

  cout<< "C Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYheL1 = new TCanvas("cYheL1", " cYheL1");
  hYheL1->Add(hYheL1R, -1);  
  hYheL1->Scale(SFHEL1);  
  hYheL1->SetStats(0);
  hYheL1->Draw("COLZ");
  cYheL1->Print(outputpdf);

  cout<< "L1 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TCanvas *cYheL2 = new TCanvas("cYheL2", " cYheL2");
  hYheL2->Add(hYheL2R, -1);  
  hYheL2->Scale(SFHEL2);  
  hYheL2->SetStats(0);
  hYheL2->Draw("COLZ");
  cYheL2->Print(outputpdf);

  cout<< "L2 Setting Yield"<<endl;
  cout<<"..."<<endl;

  TH2D *Clonehe = (TH2D*)hYheR1->Clone("Clonehe");
  Clonehe->Add(hYheC,+1);
  Clonehe->Add(hYheL1,+1);
  Clonehe->Add(hYheL2,+1);
  Clonehe->SetTitle("Total Yield for High epsilon");
  TCanvas *cYthe = new TCanvas("cYthe", " cYthe");
  Clonehe->Draw("COLZ");
  cYthe->Print(outputpdf);

  cout<< "Total Yield for high epsilon"<<endl;
  cout<<"..."<<endl;

  cout<<"End of the high e Yield calculations "<<endl;


  // t limit for the analysis

  Double_t tmn = 0.00595;
  Double_t tmx = 0.07;

  //-t for all three e
  TCanvas *c13 = new TCanvas("c13", " c13");
  //High epsilon  
  htheC->SetStats(0);
  htheC->SetLineColor(kGreen);
  htheC->Add(htheCR,-1);
  TAxis *In1 = htheC->GetXaxis();
  Double_t heC =  htheC->Integral(In1->FindBin(tmn), In1->FindBin(tmx), "");
  cout<< " High epsilon C events: "<<heC<<endl;
  htheC->Draw("Weight");

  htheR1->SetStats(0);
  htheR1->SetLineColor(kGreen);
  htheR1->Add(htheR1R,-1); 
  TAxis *In2 = htheR1->GetXaxis();
  Double_t heR1 =  htheR1->Integral(In2->FindBin(tmn), In2->FindBin(tmx), "");
  cout<< " High epsilon R1 events: "<<heR1<<endl;
  htheR1->Draw("same Weight");

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
 
  // Mid epsilon
  
  htmeC->SetStats(0);
  htmeC->SetLineColor(kBlue);
  htmeC->Add(htmeCR,-1); 
  TAxis *In5 = htmeC->GetXaxis();
  Double_t meC =  htmeC->Integral(In5->FindBin(tmn), In5->FindBin(tmx), "");
  cout<< " Mid epsilon C events: "<<meC<<endl; 
  htmeC->Draw("same Weight");

  htmeR1->SetStats(0);
  htmeR1->SetLineColor(kBlue);
  htmeR1->Add(htmeR1R,-1); 
  TAxis *In6 = htmeR1->GetXaxis();
  Double_t meR1 =  htmeR1->Integral(In6->FindBin(tmn), In6->FindBin(tmx), "");
  cout<< " Mid epsilon R1 events: "<<meR1<<endl; 
  htmeR1->Draw("same Weight");

  htmeR2->SetStats(0);
  htmeR2->SetLineColor(kBlue);
  htmeR2->Add(htmeR2R,-1); 
  TAxis *In7 = htmeR2->GetXaxis();
  Double_t meR2 =  htmeR2->Integral(In7->FindBin(tmn), In7->FindBin(tmx), "");
  cout<< " Mid epsilon R2 events: "<<meR2<<endl; 
  htmeR2->Draw("same Weight");

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
  Double_t Events6 = 0.0;
  Double_t Events7 = 0.0;
  Double_t Events8 = 0.0;
  Double_t Events9 = 0.0;
  Double_t Events10 = 0.0;
  Double_t Events11 = 0.0;
  Double_t Events12 = 0.0;

  
  // calculations of events in each t

  /**
  for(Long64_t i = 2; i <=9; i++) 
    {
      Events1 = htheInC->GetBinContent(i);
      cout<<i<<"  = "<<Events1<<endl;
      Events2 = htheInR1->GetBinContent(i);
      cout<<i<<"  = "<<Events2<<endl;        
      Events3 = htheInL1->GetBinContent(i);
      cout<<i<<"  = "<<Events3<<endl;
      Events4 = htheInL2->GetBinContent(i);
      cout<<i<<"  = "<<Events4<<endl;

      cout<<"Total Events he"<< " i "<<" = "<<Events1+Events2+Events3+Events4<<endl;
    }

  for(Long64_t i = 2; i <=9; i++) 
    {
      
      Events5 = htmeInC->GetBinContent(i);
      cout<<i<<"  = "<<Events5<<endl;
      Events6 = htmeInR1->GetBinContent(i);
      cout<<i<<"  = "<<Events6<<endl;
      Events7 = htmeInR2->GetBinContent(i);
      cout<<i<<"  = "<<Events7<<endl;
      Events8 = htmeInL1->GetBinContent(i);
      cout<<i<<"  = "<<Events8<<endl;
      Events9 = htmeInL2->GetBinContent(i);
      cout<<i<<"  = "<<Events9<<endl;

      cout<<"Total Events me"<< " i "<<" = "<<Events5+Events6+Events7+Events8+Events9<<endl;
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

  **/

  /**  
  // if(In1->FindBin(0.005) >=41 && In1->FindBin(0.005) <= 246)  
  {
    // for(Long64_t i = 41; i < 61; i++)       // 1st t
    // for(Long64_t i = 44; i < 61; i++)       // 1st t  updated one
    // for(Long64_t i = 61; i < 70; i++)           // 2nd t
    //     for(Long64_t i = 70; i < 81; i++)     // 3rd t
    // for(Long64_t i = 81; i < 93; i++)     // 4th t
    for(Long64_t i = 93; i < 107; i++)           // 5th t
    //      for(Long64_t i = 107; i < 126; i++)           // 6th t
    //      for(Long64_t i = 126; i < 153; i++)           // 7th t
    // for(Long64_t i = 153; i < 247; i++)           // 8th t
      //for(Long64_t i = 41; i < 45; i++)           //test
	
      {
	//	Events1 += htheC->GetBinContent(i);
	//	Events2 += htheR1->GetBinContent(i);
	//	Events3 += htheL1->GetBinContent(i);
	//	Events4 += htheL2->GetBinContent(i);
	//	Events5 += htmeC->GetBinContent(i);
	//	Events6 += htmeR1->GetBinContent(i);
	//	Events7 += htmeR2->GetBinContent(i);
	//	Events8 += htmeL1->GetBinContent(i);
	//	Events9 += htmeL2->GetBinContent(i);
	Events10 += htC->GetBinContent(i);
	Events11 += htL1->GetBinContent(i);
	Events12 += htL2->GetBinContent(i);
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
    cout<< "  "<<Events10<<endl;    
    cout<< "  "<<Events11<<endl;    
    cout<< "  "<<Events12<<endl;    
  }
    
  //  cout<<" Total Events: " <<Events1 + Events2 +Events3 +Events4 +Events5 +Events6 +Events7 +Events8 +Events9 +Events10 +Events11 +Events12 <<endl;
  cout<<" Total Events: " <<Events10 +Events11 +Events12 <<endl;
  
  **/
  cout<<" Integral show is commented out "<<endl;

  //  TLine *ltlow  = new TLine(0.005, -80.0, 0.005, 1800);
  //TLine *lthigh = new TLine(0.08, -80.0, 0.08, 1800.0);
  // ltlow->SetLineColor(kBlack);
  //lthigh->SetLineColor(kBlack);
  //  ltlow->Draw("same"); 
  //lthigh->Draw("same");
    
  Double_t Ymin = -92.0;
  Double_t Ymax = 1750.0;

  Double_t fstedge = ((TAxis*)htheC->GetXaxis())->GetBinCenter(44);
  cout<<" fstedge "<<fstedge<<endl;
  TLine *fstedgel = new TLine(fstedge, Ymin, fstedge, Ymax);
  fstedgel->SetLineColor(kBlack);
  fstedgel->Draw("same");

  Double_t lastedge = ((TAxis*)htheC->GetXaxis())->GetBinCenter(219);
  cout<<" lastedge "<<lastedge<<endl;
  TLine *lastedgel = new TLine(lastedge, Ymin, lastedge, Ymax);
  lastedgel->SetLineColor(kBlack);
  lastedgel->Draw("same");

  Double_t bin_center1 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(60);
  cout<<" 1 "<<bin_center1<<endl;
  TLine *lt1  = new TLine(bin_center1, Ymin, bin_center1, Ymax);
  lt1->SetLineColor(kBlack);
  lt1->Draw("same");

  Double_t bin_center2 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(69);
  cout<<"2 "<<bin_center2<<endl;
  TLine *lt2  = new TLine(bin_center2, Ymin, bin_center2, Ymax);
  lt2->SetLineColor(kBlack);
  lt2->Draw("same");

  Double_t bin_center3 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(80);
  cout<<"3 "<<bin_center3<<endl;
  TLine *lt3  = new TLine(bin_center3, Ymin, bin_center3, Ymax);
  lt3->SetLineColor(kBlack);
  lt3->Draw("same");

  Double_t bin_center4 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(92);
  cout<<"4 "<<bin_center4<<endl;
  TLine *lt4  = new TLine(bin_center4, Ymin, bin_center4, Ymax);
  lt4->SetLineColor(kBlack);
  lt4->Draw("same");

  Double_t bin_center5 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(106);
  cout<<"5 "<<bin_center5<<endl;
  TLine *lt5  = new TLine(bin_center5, Ymin, bin_center5, Ymax);
  lt5->SetLineColor(kBlack);
  lt5->Draw("same");

  Double_t bin_center6 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(125);
  cout<<"6 "<<bin_center6<<endl;
  TLine *lt6  = new TLine(bin_center6, Ymin, bin_center6, Ymax);
  lt6->SetLineColor(kBlack);
  lt6->Draw("same");

  Double_t bin_center7 = ((TAxis*)htheC->GetXaxis())->GetBinCenter(152);
  cout<<"7 "<<bin_center7<<endl;
  TLine *lt7  = new TLine(bin_center7, Ymin, bin_center7, Ymax);
  lt7->SetLineColor(kBlack);
  lt7->Draw("same");
  
  auto legt = new TLegend(0.7,0.7,0.30,0.9); // 0.1 0.7
  legt->SetHeader("All epsilon at Q2 = 0.38 GeV2","C");
  legt->AddEntry(htheC,"High epsilon (4)","l");
  legt->AddEntry(htmeC,"Mid epsilon (5)","l");
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

  //Mid epsilon

  hph_qmeR2->SetStats(0);
  hph_qmeR2->SetLineColor(kBlue);
  hph_qmeR2->Draw("same Weight");
  
  hph_qmeC->SetStats(0);
  hph_qmeC->SetLineColor(kBlue);
  hph_qmeC->Draw("same Weight");

  hph_qmeL2->SetStats(0);
  hph_qmeL2->SetLineColor(kBlue);
  hph_qmeL2->Draw("same Weight");
  
  hph_qmeL1->SetStats(0);
  hph_qmeL1->SetLineColor(kBlue);
  hph_qmeL1->Draw("same Weight");
 
  hph_qmeR1->SetStats(0);
  hph_qmeR1->SetLineColor(kBlue);
  hph_qmeR1->Draw("same Weight");

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

  c14->Print(outputpdf + ')');
 
}
