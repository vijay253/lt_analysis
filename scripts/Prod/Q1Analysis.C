// .... Created Date: Feb 01, 2023 ....
// .... Author: VK ....
// .... This script was prepared to analyse the summer 2019 data....
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

void Q1Analysis()
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
    ROOTfilePath = "/cache/hallc/kaonlt/vijay/Analysis/Q1";
    OutPath =  "/cache/hallc/kaonlt/vijay/";
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

  cout<<" "<<endl;
  cout<<" Physics Analysis..." <<endl;
  cout<<" "<<endl;
  //Data  
  TString TInDATAFilenameC = "Analysed_Data_Q0p375W2p2_lowe_Center.root";
  TString rootFile_DATAC  = ROOTfilePath+"/"+TInDATAFilenameC;
  //Dummy
  TString TInDATAFilenamedC = "Analysed_Data_Q0p375W2p2_lowe_dummy_Center.root";
  TString rootFile_DATADC  = ROOTfilePath+"/"+TInDATAFilenamedC;
  //SIMC
  TString TInSIMCFilenameC  = "Pion_2p7_Q1_center.root";
  TString rootFile_SIMCC   = ROOTfilePath+"/"+TInSIMCFilenameC;
  
  if (gSystem->AccessPathName(rootFile_DATAC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATAC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  if (gSystem->AccessPathName(rootFile_SIMCC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_SIMCC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  //Data
  TFile *InFile_DATAC = new TFile(rootFile_DATAC, "READ");
  //Dummy
  TFile *InFile_DATADC = new TFile(rootFile_DATADC, "READ");
  //SIMC
  TFile *InFile_SIMCC = new TFile(rootFile_SIMCC, "READ");
  //Output file names 
  TString foutname   = OutPath+"/" + "Analysed_Qp375W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Analysed_Qp375W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Analysed_Qp375W2p2" + ".pdf";
  //Data
  TTree* TBRANCHC  = (TTree*)InFile_DATAC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHC  = (Long64_t)TBRANCHC->GetEntries();  
  //Dummy
  TTree* TBRANCHDC  = (TTree*)InFile_DATADC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHDC  = (Long64_t)TBRANCHDC->GetEntries();  
  //SIMC  
  TTree* TSIMCC  = (TTree*)InFile_SIMCC->Get("h10");Long64_t nEntries_TSIMCC  = (Long64_t)TSIMCC->GetEntries();  
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

  Double_t tC;TBRANCHC->SetBranchAddress("MandelT", &tC);
  //dummy
  Double_t tdC;TBRANCHDC->SetBranchAddress("MandelT", &tdC);
  //dummy
  Double_t ph_qC;TBRANCHC->SetBranchAddress("ph_q", &ph_qC);
  //dummy
  Double_t ph_qdC;TBRANCHDC->SetBranchAddress("ph_q", &ph_qdC);
  //dummy
  Double_t th_qC;TBRANCHC->SetBranchAddress("th_q", &th_qC);
  Double_t mmC;TBRANCHC->SetBranchAddress("MM", &mmC);
  //dummy
  Double_t mmdC;TBRANCHDC->SetBranchAddress("MM", &mmdC);
  //dummy
  Double_t tcoinC;TBRANCHC->SetBranchAddress("CTime_ROC1", &tcoinC);
  //dummy
  Double_t tcoindC;TBRANCHDC->SetBranchAddress("CTime_ROC1", &tcoindC);

  //SIMC VARIABLES
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
  //*************************
  TH1D *MPiC   = new TH1D("MPiC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPiCR   = new TH1D("MPiCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Dummy
  TH1D *MPidC   = new TH1D("MPidC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //Random
  TH1D *MPidCR   = new TH1D("MPidCR"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //SIMC
  TH1D *MPiSC   = new TH1D("MPiSC"," ; Missing Mass (GeV/c^{2}); Yield (events/mC)", 150, 0.9, 1.0);      
  //t
  TH1D *hthtC   = new TH1D("hthtC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtCR   = new TH1D("hthtCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Dummy
  TH1D *hthtdC   = new TH1D("hthtdC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //Random
  TH1D *hthtdCR   = new TH1D("hthtdCR"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //SIMC
  TH1D *hthtSC   = new TH1D("hthtSC"," ;t (GeV/c)^{2}; Yield (events/mC)",   150, 0.00, 0.071);      
  //*************************
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
  //W Random
  TH1D *hW1CR   = new TH1D("hW1CR","W 1 t C; W;", 300, 2.1, 2.3);
  TH1D *hW2CR   = new TH1D("hW2CR","W 2 t C; W;", 300, 2.1, 2.3);
  TH1D *hW3CR   = new TH1D("hW3CR","W 3 t C; W;", 300, 2.1, 2.3);
  TH1D *hW4CR   = new TH1D("hW4CR","W 4 t C; W;", 300, 2.1, 2.3);
  TH1D *hW5CR   = new TH1D("hW5CR","W 5 t C; W;", 300, 2.1, 2.3);
  TH1D *hW6CR   = new TH1D("hW6CR","W 6 t C; W;", 300, 2.1, 2.3);
  TH1D *hW7CR   = new TH1D("hW7CR","W 7 t C; W;", 300, 2.1, 2.3);
  TH1D *hW8CR   = new TH1D("hW8CR","W 8 t C; W;", 300, 2.1, 2.3);
  //Q2
  TH1D *hQ21C   = new TH1D("hQ21C","Q2 1 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22C   = new TH1D("hQ22C","Q2 2 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23C   = new TH1D("hQ23C","Q2 3 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24C   = new TH1D("hQ24C","Q2 4 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25C   = new TH1D("hQ25C","Q2 5 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26C   = new TH1D("hQ26C","Q2 6 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27C   = new TH1D("hQ27C","Q2 7 t C; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28C   = new TH1D("hQ28C","Q2 8 t C; Q2;", 300, 0.2, 0.6);      
  //Q2 Random
  TH1D *hQ21CR   = new TH1D("hQ21CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ22CR   = new TH1D("hQ22CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ23CR   = new TH1D("hQ23CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ24CR   = new TH1D("hQ24CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ25CR   = new TH1D("hQ25CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ26CR   = new TH1D("hQ26CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ27CR   = new TH1D("hQ27CR","Q2; Q2;", 300, 0.2, 0.6);
  TH1D *hQ28CR   = new TH1D("hQ28CR","Q2; Q2;", 300, 0.2, 0.6);
  //t
  TH1D *ht1C  = new TH1D("ht1C","t 1 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2C  = new TH1D("ht2C","t 2 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3C  = new TH1D("ht3C","t 3 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4C  = new TH1D("ht4C","t 4 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5C  = new TH1D("ht5C","t 5 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6C  = new TH1D("ht6C","t 6 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7C  = new TH1D("ht7C","t 7 t C; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8C  = new TH1D("ht8C","t 8 t C; MandelT;",   300, -0.01, 0.1);      
  //t Random
  TH1D *ht1CR  = new TH1D("ht1CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht2CR  = new TH1D("ht2CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht3CR  = new TH1D("ht3CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht4CR  = new TH1D("ht4CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht5CR  = new TH1D("ht5CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht6CR  = new TH1D("ht6CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht7CR  = new TH1D("ht7CR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *ht8CR  = new TH1D("ht8CR","MandelT; MandelT;",   300, -0.01, 0.1);      

  Double_t bins[] = {-0.01, 0.00595, 0.0118167, 0.0151167, 0.01915, 0.02355, 0.0286833, 0.03565, 0.04555, 0.0741167, 0.1};
  Double_t PhiBinLow = 0.932-0.003;
  //  Double_t PhiBinLow = 0.920-0.003;
  Double_t PhiBinHigh = 0.980+0.003;
  //PhiBinW = (0.980-0.932)/NoPhiBIn=0.003
  TH1D *htC  = new TH1D("htC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htInC  = new TH1D("htInC","MandelT; MandelT;",   10, bins);      
  TH1D *htCR  = new TH1D("htCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htInCR  = new TH1D("htInCR","MandelT; MandelT;",   10, bins);      
  //DATA
  TH1D *hph_qC  = new TH1D("hph_qC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hph_qCR  = new TH1D("hph_qCR","ph_q; ph_q;",  18, -22.5, 382.5);      
  //SIMC
  TH1D *hph_qSC  = new TH1D("hph_qSC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH1D *hmmC  = new TH1D("hmmC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmCR  = new TH1D("hmmCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *htcoinC   = new TH1D("htcoinC","; ;",  300, -20.0, 20.0);      
  TH2D *hYC   = new TH2D("hYC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYCR  = new TH2D("hYCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  //dummy
  TH2D *hYdC   = new TH2D("hYdC"," Yield C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYdCR  = new TH2D("hYdCR"," Yield in Bins; t-Bin; Phi-Bin", 10, bins, 18, -22.5, 382.5);      
  TH2D *hYSC   = new TH2D("hYSC"," Yield SIMC C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  //SIMC and Data HISTOGRAMS
  TH1D *hphC  = new TH1D("hphC","ph_q; ph_q;",50, -10, 370);      
  TH1D *hphCR  = new TH1D("hphCR","ph_q; ph_q;",  50, -10, 370);      
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",50, -10, 370);      
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
  TCutG *Dcut = new TCutG("Dcut",5);
  Dcut->SetVarX("Q2");
  Dcut->SetVarY("W");
  Dcut->SetPoint(0,0.363783,2.18318);Dcut->SetPoint(1,0.311069,2.22644);  
  Dcut->SetPoint(2,0.37737,2.21136); Dcut->SetPoint(3,0.452314,2.16293);
  Dcut->SetPoint(4,0.363783,2.18318); 
  //////////////////////////////
  Double_t MMPICUTL = 0.91; 
  Double_t MMPICUTH = 0.98; 
  /////////////////////////////
  //DATA 
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);
      
      if(tcoinC -TimmingOffset >=-1.0 && tcoinC -TimmingOffset <= 1.0 && mmC>= MMPICUTL && mmC <=MMPICUTH )
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
      Double_t MMpiC = mmC>= MMPICUTL && mmC <=MMPICUTH;
      Double_t CoinPionC = tcoinC-TimmingOffset>=-1.0 && tcoinC-TimmingOffset <= 1.0;      
      Double_t CoinPionCR = ((tcoinC-TimmingOffset>=-15.0 && tcoinC-TimmingOffset<= -9.0) || (tcoinC-TimmingOffset>=7.0 && tcoinC-TimmingOffset<=13.0));      
      if(CoinPionC && DiamondC)
	{      
	  MPiC->Fill(mmC);
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
	  hthtC->Fill(-tC); 
 	  htC->Fill(-tC);
	  htInC->Fill(-tC);	
	  hph_qC->Fill(ph_qC*57.2958 + 180);
	  hphC->Fill(ph_qC*57.2958 + 180);
	  hYC->Fill(-tC, ph_qC*57.2958 + 180);
	  hthC->Fill(th_qC*57.2958);
	}	
      //Random    
      if(CoinPionCR && DiamondC)
	{
	  MPiCR->Fill(mmC);
	}
      if(CoinPionCR && MMpiC && DiamondC)
	{
	  H_ssdelta_DCR->Fill(P_gtr_dp);
	  H_ssxptar_DCR->Fill(P_gtr_xptar);
	  H_ssyptar_DCR->Fill(P_gtr_yptar);
	  H_hsdelta_DCR->Fill(H_gtr_dp);
	  H_hsxptar_DCR->Fill(H_gtr_xptar);
	  H_hsyptar_DCR->Fill(H_gtr_yptar);
	  hthtCR->Fill(-tC); 
	  htCR->Fill(-tC);
	  htInCR->Fill(-tC);	
	  hph_qCR->Fill(ph_qC*57.2958 + 180);
	  hphCR->Fill(ph_qC*57.2958 + 180);
	  hQ2WCR->Fill(Q2C, WC);
	  hYCR->Fill(-tC, ph_qC*57.2958 + 180);
	  hthCR->Fill(th_qC*57.2958);
	}
      Double_t w1t = -tC >= bins[1] && -tC <= bins[2];
      Double_t w2t = -tC >= bins[2] && -tC <= bins[3];
      Double_t w3t = -tC >= bins[3] && -tC <= bins[4];
      Double_t w4t = -tC >= bins[4] && -tC <= bins[5];
      Double_t w5t = -tC >= bins[5] && -tC <= bins[6];
      Double_t w6t = -tC >= bins[6] && -tC <= bins[7];
      Double_t w7t = -tC >= bins[7] && -tC <= bins[8];
      Double_t w8t = -tC >= bins[8] && -tC <= bins[9];

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
  
  //DATA dummy
  for(Long64_t i = 0; i < nEntries_TBRANCHDC; i++)
    {
      TBRANCHDC->GetEntry(i);
            
      Double_t DiamonddC = (Dcut->IsInside(Q2dC, WdC));  
      Double_t MMpidC = mmdC>= MMPICUTL && mmdC <=MMPICUTH;
      Double_t CoinPiondC = tcoindC-TimmingOffset>=-1.0 && tcoindC-TimmingOffset <= 1.0;      
      Double_t CoinPiondCR = ((tcoindC-TimmingOffset>=-15.0 && tcoindC-TimmingOffset<= -9.0) || (tcoindC-TimmingOffset>=7.0 && tcoindC-TimmingOffset<=13.0));      
      
      if(CoinPiondC && DiamonddC)
	{
	  MPidC->Fill(mmdC);
	}
      if(CoinPiondC && MMpidC && DiamonddC)
	{
	  hYdC->Fill(-tdC, ph_qdC*57.2958 + 180);
	  hthtdC->Fill(-tdC); 
	  H_hsdelta_DummyC->Fill(H_gtr_dpd);

	}	
      //Random
      Double_t CUTCCR = ((tcoindC-TimmingOffset>=-15.0 && tcoindC-TimmingOffset <= -9.0) || (tcoindC-TimmingOffset>=7.0 && tcoindC-TimmingOffset <=13.0));      

      if(CoinPiondCR && DiamonddC)
	{
	  MPidCR->Fill(mmdC);
	}
      if(CoinPiondCR && MMpidC && DiamonddC)
	{
	  hYdCR->Fill(-tdC, ph_qdC*57.2958 + 180);
	  hthtdCR->Fill(-tdC); 
	  H_hsdelta_DummyCR->Fill(H_gtr_dpd);

	}
    }  
  //SIMC
  for(Long64_t i = 0; i < nEntries_TSIMCC; i++)

    {
      TSIMCC->GetEntry(i);

      Double_t Diamond_cut = (Dcut->IsInside(Q2_simc, W_simc));  
      Double_t fact = 9428650.0/400000.0;
      Double_t Sw1t = t_simc >= bins[1] && t_simc <= bins[2];
      Double_t Sw2t = t_simc >= bins[2] && t_simc <= bins[3];
      Double_t Sw3t = t_simc >= bins[3] && t_simc <= bins[4];
      Double_t Sw4t = t_simc >= bins[4] && t_simc <= bins[5];
      Double_t Sw5t = t_simc >= bins[5] && t_simc <= bins[6];
      Double_t Sw6t = t_simc >= bins[6] && t_simc <= bins[7];
      Double_t Sw7t = t_simc >= bins[7] && t_simc <= bins[8];
      Double_t Sw8t = t_simc >= bins[8] && t_simc <= bins[9];
      
      Double_t CUTSIMCC = (hsdelta > SIMC_Delta_Low_H && hsdelta < SIMC_Delta_High_H) && (hsxptar > SIMC_Xptar_Low_H && hsxptar < SIMC_Xptar_High_H) && (hsyptar > SIMC_Yptar_Low_H && hsyptar < SIMC_Yptar_High_H) && (ssdelta > SIMC_Delta_Low_P && ssdelta < SIMC_Delta_High_P) && (ssxptar > SIMC_Xptar_Low_P && ssxptar < SIMC_Xptar_High_P) && (ssyptar > SIMC_Yptar_Low_P && ssyptar < SIMC_Yptar_High_P) && (missmass >=MMPICUTL && missmass <=MMPICUTH) && Diamond_cut;      

      if(CUTSIMCC)
	{
	  hthSC->Fill(thetapq*57.2958, fact*Weight);
	  hph_qSC->Fill(phipq*57.2958, fact*Weight);
	  hphSC->Fill(phipq*57.2958, fact*Weight);
	  hYSC->Fill(t_simc, phipq*57.2958, fact*Weight);
	  H_ssdelta_SC->Fill(ssdelta, fact*Weight);
	  H_ssxptar_SC->Fill(ssxptar, fact*Weight);
	  H_ssyptar_SC->Fill(ssyptar, fact*Weight);
	  H_hsdelta_SC->Fill(hsdelta, fact*Weight);
	  H_hsxptar_SC->Fill(hsxptar, fact*Weight);
	  H_hsyptar_SC->Fill(hsyptar, fact*Weight);
	  H_t_RE->Fill((t_simc-ti_simc), fact*Weight);
	  H_t_RE_t->Fill((t_simc-ti_simc), t_simc, fact*Weight);
	  hthtSC->Fill(t_simc, fact*Weight); 

	}    
    }  
  //////////////////////////
  Double_t RTSCALE = 1.0/6.0;   
  //////////////////////////
  MPiCR->Scale(RTSCALE);
  MPidCR->Scale(RTSCALE);
  hthtCR->Scale(RTSCALE);
  hthtdCR->Scale(RTSCALE);
  //Yield
  hYCR->Scale(RTSCALE);
  hYdCR->Scale(RTSCALE);
  H_ssdelta_DCR->Scale(RTSCALE);
  H_ssxptar_DCR->Scale(RTSCALE);
  H_ssyptar_DCR->Scale(RTSCALE);
  H_hsdelta_DCR->Scale(RTSCALE);
  H_hsdelta_DummyCR->Scale(RTSCALE);
  H_hsxptar_DCR->Scale(RTSCALE);
  H_hsyptar_DCR->Scale(RTSCALE);
  hphCR->Scale(RTSCALE);
  hthCR->Scale(RTSCALE);
  hW1CR->Scale(RTSCALE);  
  hW2CR->Scale(RTSCALE);  
  hW3CR->Scale(RTSCALE);  
  hW4CR->Scale(RTSCALE);  
  hW5CR->Scale(RTSCALE);  
  hW6CR->Scale(RTSCALE);  
  hW7CR->Scale(RTSCALE);  
  hW8CR->Scale(RTSCALE);  
  hQ21CR->Scale(RTSCALE);
  hQ22CR->Scale(RTSCALE);
  hQ23CR->Scale(RTSCALE);
  hQ24CR->Scale(RTSCALE);
  hQ25CR->Scale(RTSCALE);
  hQ26CR->Scale(RTSCALE);
  hQ27CR->Scale(RTSCALE);
  hQ28CR->Scale(RTSCALE);
  ht1CR->Scale(RTSCALE); 
  ht2CR->Scale(RTSCALE); 
  ht3CR->Scale(RTSCALE); 
  ht4CR->Scale(RTSCALE); 
  ht5CR->Scale(RTSCALE); 
  ht6CR->Scale(RTSCALE); 
  ht7CR->Scale(RTSCALE); 
  ht8CR->Scale(RTSCALE); 
  hQ2WCR->Scale(RTSCALE);
  hYCR->Scale(RTSCALE);
  hmmCR->Scale(RTSCALE);
  htCR->Scale(RTSCALE);
  htInCR->Scale(RTSCALE);
  hph_qCR->Scale(RTSCALE);
  //////////////////////////////////////////
  //Effective charge 
  Double_t ECLEC   = 1.0/447.01427000000007;
  Double_t ECLEDC  = 1.0/40.4086;
  //SIMC SCAL
  Double_t SCFLEC  = 1.0;  
  Double_t SCFLEL1 = 1.0; 
  Double_t SCFLEL2 = 1.0; 
  //////////////////////////////////////////
  hW1C->Add(hW1CR, -1);
  hW2C->Add(hW2CR, -1);
  hW3C->Add(hW3CR, -1);
  hW4C->Add(hW4CR, -1);
  hW5C->Add(hW5CR, -1);
  hW6C->Add(hW6CR, -1);
  hW7C->Add(hW7CR, -1);
  hW8C->Add(hW8CR, -1);

  hQ21C->Add(hQ21CR, -1); 
  hQ22C->Add(hQ22CR, -1); 
  hQ23C->Add(hQ23CR, -1); 
  hQ24C->Add(hQ24CR, -1); 
  hQ25C->Add(hQ25CR, -1); 
  hQ26C->Add(hQ26CR, -1); 
  hQ27C->Add(hQ27CR, -1); 
  hQ28C->Add(hQ28CR, -1); 

  ht1C->Add(ht1CR, -1);
  ht2C->Add(ht2CR, -1);
  ht3C->Add(ht3CR, -1);
  ht4C->Add(ht4CR, -1);
  ht5C->Add(ht5CR, -1);
  ht6C->Add(ht6CR, -1);
  ht7C->Add(ht7CR, -1);
  ht8C->Add(ht8CR, -1);

  ofstream outleC("kindata/kindata.pl_375_286_setting.dat");
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
  hYC->Add(hYCR, -1);  
  hYC->Scale(ECLEC);  
  hYSC->Scale(SCFLEC);
  ofstream YleC("yield/yields.pl_375_286_setting.target");
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
	  Double_t err;
	  Double_t TotNoEvent = hYC->IntegralAndError(j, j, i, i, err, "");
	  Double_t Yield = TotNoEvent*ECLEC;
	  YleC<<hYC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;	  
	}
    } 
  YleC.close();
  hYdC->Add(hYdCR, -1);  
  hYdC->Scale(ECLEDC);   
  hYdC->Scale(THCF);
  ofstream YdleC("yield/yields.pl_375_286_setting.dummy");
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
	  YdleC<<hYdC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;
	}
    }
  YdleC.close();
  ofstream YSleC("yield/yields.pl_375_286_setting.simc");
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
	  YSleC<<hYSC->IntegralAndError(j, j, i, i, err, "")<<"\t"<<err<<"\t"<<j-1<<"\t"<<i-1 <<endl;
	}
      
    }
  YSleC.close();  
}
