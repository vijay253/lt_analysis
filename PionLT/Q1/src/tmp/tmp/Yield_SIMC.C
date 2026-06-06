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

void Yield_SIMC()
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
  cout<<" PHYSICS SIMC ANALYSIS..." <<endl;
  cout<<" "<<endl;
  TString TInSIMCFilenameC  = "Pion_2p7_Q1_center.root";
  TString rootFile_SIMCC   = ROOTfilePath+"/"+TInSIMCFilenameC;
  
  if (gSystem->AccessPathName(rootFile_SIMCC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_SIMCC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  TFile *InFile_SIMCC = new TFile(rootFile_SIMCC, "READ");
  TTree* TSIMCC  = (TTree*)InFile_SIMCC->Get("h10");Long64_t nEntries_TSIMCC  = (Long64_t)TSIMCC->GetEntries();  
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
  TH1D *hph_qSC  = new TH1D("hph_qSC","ph_q; ph_q;",   18, -22.5, 382.5);      
  TH2D *hYSC   = new TH2D("hYSC"," Yield SIMC C;t-Bin; Phi-Bin ", 10, bins, 18, -22.5, 382.5);      
  //SIMC HISTOGRAMS
  TH1D *hphSC  = new TH1D("hphSC","ph_q; ph_q;",50, -10, 370);      
  TH1D *hthSC  = new TH1D("hth_qC","th_q; th_q;",   50, -0.01, 5.0);      
  TH1D *H_ssdelta_SC  = new TH1D("H_ssdelta_SC","SHMS delta; ssdelta;", 50, -5.0, 3.0);
  TH1D *H_ssxptar_SC  = new TH1D("H_ssxptar_SC","SHMS xptar; ssxptar;", 50, -0.06, 0.06);
  TH1D *H_ssyptar_SC  = new TH1D("H_ssyptar_SC","SHMS yptar; ssyptar;", 50, -0.04, 0.04);
  TH1D *H_hsdelta_SC  = new TH1D("H_hsdelta_SC","HMS Delta; hsdelta;", 50, -20.0, 20.0);
  TH1D *H_hsxptar_SC  = new TH1D("H_hsxptar_SC","HMS xptar; hsxptar;", 50, -0.1, 0.1);
  TH1D *H_hsyptar_SC  = new TH1D("H_hsyptar_SC","HMS yptar; hsyptar;", 50, -0.05, 0.05);
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
  ofstream YSleC("yield/yields.pl_q2_eps_setting.simc");
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
