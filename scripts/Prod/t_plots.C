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

  TString rootFile_SIMCC  = ROOTfilePath+"/"+TInSIMCFilenameC;


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

  //DATA HISTOGRAMS

  TH2D *hQ2WC  = new TH2D("hQ2WC","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH2D *hQ2WCR  = new TH2D("hQ2WCR","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1R  = new TH2D("hQ2WC1R","; ; ", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH1D *htC  = new TH1D("htC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htL1  = new TH1D("htL1","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htL2  = new TH1D("htL2","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *htCR  = new TH1D("htCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htL1R  = new TH1D("htL1R","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htL2R  = new TH1D("htL2R","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *hph_qC  = new TH1D("hph_qC","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qL1  = new TH1D("hph_qL1","ph_q; ph_q;", 300, -5.0, 5.0);      
  TH1D *hph_qL2  = new TH1D("hph_qL2","ph_q; ph_q;", 300, -5.0, 5.0);      

  TH1D *hmmC  = new TH1D("hmmC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1  = new TH1D("hmmL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2  = new TH1D("hmmL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmCR  = new TH1D("hmmCR","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1R  = new TH1D("hmmL1R","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2R  = new TH1D("hmmL2R","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinC   = new TH1D("htcoinC","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinL1  = new TH1D("htcoinL1","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      
  TH1D *htcoinL2  = new TH1D("htcoinL2","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      

  //SIMC HISTOGRAMS

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

  TCutG *Diamond = new TCutG("Diamond",5);
  Diamond->SetVarX("Q2");
  Diamond->SetVarY("W");
  Diamond->SetPoint(0,0.363783,2.18318);Diamond->SetPoint(1,0.311069,2.22644); //Diamond->SetPoint(1,0.311469,2.22694); Diamond->SetPoint(2,0.37837,2.21236); 
  Diamond->SetPoint(2,0.37737,2.21136); Diamond->SetPoint(3,0.452314,2.16293);
  Diamond->SetPoint(4,0.363783,2.18318); 

  // ULong64_t Q2C1[60000000];
  //ULong64_t WC1[600000000];
  // Q2C1 = new ULong64_t[nEntries_TBRANCHC];
  // WC1 = new ULong64_t[nEntries_TBRANCHC];
  //TVectorD Q2C1(nEntries_TBRANCHC);
  //TVectorD WC1(nEntries_TBRANCHC);
  //  std::vector<Double_t> Q2C1(nEntries_TBRANCHC);
  // std::vector<Double_t> WC1(nEntries_TBRANCHC);

  Double_t Q2C1[nEntries_TBRANCHC];
  Double_t WC1[nEntries_TBRANCHC]; 
  
  Double_t qL1[nEntries_TBRANCHL1];
  Double_t wL1[nEntries_TBRANCHL1]; 
  

  //DATA CENTER
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)

    {
      TBRANCHC->GetEntry(i);
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98)
	{
	  hQ2WC->Fill(Q2C, WC);
	  hph_qC->Fill(ph_qC);
	}
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0)    
	{
	  hmmC->Fill(mmC);	  
	}	      
      
      {
	htcoinC->Fill(tcoinC);
      }
      
      Double_t Diamond_cut = (Diamond->IsInside(Q2C, WC));  
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.98 && Diamond_cut)
	{
	  hQ2WC1->Fill(Q2C, WC);	      	  
  	  htC->Fill(-tC);
	}	      
      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.98 && Diamond_cut)
	{
	  htCR->Fill(-tC);
	  hQ2WCR->Fill(Q2C, WC);
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
      Double_t fact = 10136000.0/20000.0;

      if(hsdelta >=-8.0 && hsdelta <=8.0 && hsxptar >=-0.08 && hsxpfp <=0.08 && hsyptar >=-0.045 && hsypfp <=0.045 && ssdelta >=-10.0 && hsdelta <=20.0 && ssxptar >=-0.06 && hsxpfp <=0.06 && hsyptar >=-0.04 && hsypfp <=0.04 && missmass >= 0.92 && missmass <= 0.98)
	
	{
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
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98)
	{
	  hph_qL1->Fill(ph_qL1);	  
	}
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0)	
	{
	  hmmL1->Fill(mmL1);
	}           
      {
	htcoinL1->Fill(tcoinL1);
      }

      Double_t Diamond_cut = (Diamond->IsInside(Q2L1, WL1));  
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut)
	{
	  htL1->Fill(-tL1);
	}      
      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.98 && Diamond_cut)	
	
	{
	  htL1R->Fill(-tL1);
	}
      
      if (((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)))
	
	{	 
	  hmmL1R->Fill(mmL1);
	}     
    }
  
  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98)
	{
	  hph_qL2->Fill(ph_qL2);
	}
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0)
	
	{
	  hmmL2->Fill(mmL2);
	}   
      
      {
	htcoinL2->Fill(tcoinL2);
      }

      Double_t Diamond_cut = (Diamond->IsInside(Q2L2, WL2));  
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut)
	{
	  htL2->Fill(-tL2);
	}

      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.98 && Diamond_cut)
	{
	  htL2R->Fill(-tL2);
	}
      if (((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)))
	{
	  hmmL2R->Fill(mmL2);
	}
    }
  
  hQ2WCR->Scale(1.0/6.0);

  hmmCR->Scale(1.0/6.0);
  hmmL1R->Scale(1.0/6.0);
  hmmL2R->Scale(1.0/6.0);

  htCR->Scale(1.0/6.0);
  htL1R->Scale(1.0/6.0);
  htL2R->Scale(1.0/6.0);

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

  TCanvas *c4 = new TCanvas("c4", " c4"); 
  hph_qL2->SetStats(0);
  hph_qL2->SetLineColor(kBlack);
  hph_qL2->Draw(" Weight");

  hph_qL1->SetStats(0);
  hph_qL1->SetLineColor(kRed);
  hph_qL1->Draw("same Weight");

  hph_qC->SetStats(0);
  hph_qC->SetLineColor(kGreen);
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
  Diamond->SetLineColor(kRed);
  // hQ2WC->Add(hQ2WCR, -1);
  // hQ2WC->SetStats(0);
  hQ2WC->Draw("COL");
  Diamond->Draw("same");  
  c6->cd(2);
  // hQ2WC1->SetStats(0);
  Diamond->SetLineColor(kRed);
  hQ2WC1->Draw("COL");
  Diamond->Draw("same");
  //c6->SetLeftMargin(0.15);   
  c6->Print(outputpdf);

  TCanvas *cRE = new TCanvas("cRE", " cRE");

  TF1 *Gauss = new TF1("Gauss","gaus(0)",-0.004,0.006);
  Gauss->SetLineColor(kRed);
  H_t_RE->Fit("Gauss", "RQ"); 
  H_t_RE->SetStats(0);
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
  hQ2WC->GetListOfFunctions()->Add(Diamond,"L");  
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

  TH1D *htmeR1  = new TH1D("htmeR1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeR2  = new TH1D("htmeR2","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeC   = new TH1D("htmeC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1  = new TH1D("htmeL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2  = new TH1D("htmeL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htmeR1R  = new TH1D("htmeR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeR2R  = new TH1D("htmeR2R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeCR   = new TH1D("htmeCR","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL1R  = new TH1D("htmeL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htmeL2R  = new TH1D("htmeL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *hph_qmeR1  = new TH1D("hph_qmeR1","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qmeR2  = new TH1D("hph_qmeR2","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qmeC   = new TH1D("hph_qmeC","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qmeL1  = new TH1D("hph_qmeL1","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qmeL2  = new TH1D("hph_qmeL2","ph_q; ph_q;",   300, -5.0, 5.0);      

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
      
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98)
	{
	  hph_qmeR1->Fill(ph_qmeR1);
	}
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0)    
	{
	  hmmmeR1->Fill(mmmeR1);	  
	}	      
      
      {
	htcoinmeR1->Fill(tcoinmeR1);
      }

      Double_t Diamond_cut = (Diamond->IsInside(Q2MER1, WMER1));  
      
      if(tcoinmeR1>=-1.0 && tcoinmeR1 <= 1.0 && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut)
	{
	  htmeR1->Fill(-tmeR1);
	}

      if(((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)) && mmmeR1>= 0.92 && mmmeR1 <=0.98 && Diamond_cut)
	{
	  htmeR1R->Fill(-tmeR1);
	}
      
      if (((tcoinmeR1>=-15.0 && tcoinmeR1 <= -9.0) || (tcoinmeR1>=7.0 && tcoinmeR1 <=13.0)))
	
	{
	  hmmmeR1R->Fill(mmmeR1);
	}	
    }  

  for(Long64_t i = 0; i < nEntries_TBRANCHMIDER2; i++)

    {
      TBRANCHMIDER2->GetEntry(i);
      
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98)
	{
	  hph_qmeR2->Fill(ph_qmeR2);
	}
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0)    
	{
	  hmmmeR2->Fill(mmmeR2);	  
	}	      
      
      {
	htcoinmeR2->Fill(tcoinmeR2);
      }
           
      Double_t Diamond_cut = (Diamond->IsInside(Q2MER2, WMER2));  
     
      if(tcoinmeR2>=-1.0 && tcoinmeR2 <= 1.0 && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut)
	{
	  htmeR2->Fill(-tmeR2);
	}

      if(((tcoinmeR2>=-15.0 && tcoinmeR2 <= -9.0) || (tcoinmeR2>=7.0 && tcoinmeR2 <=13.0)) && mmmeR2>= 0.92 && mmmeR2 <=0.98 && Diamond_cut)
	{
	  htmeR2R->Fill(-tmeR2);
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
	  hph_qmeC->Fill(ph_qmeC);
	}
 
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0)    
 	{
	  hmmmeC->Fill(mmmeC);	  
	}	      
      
      {
	htcoinmeC->Fill(tcoinmeC);
      }
      
      Double_t Diamond_cut = (Diamond->IsInside(Q2MEC, WMEC));  
      
      if(tcoinmeC>=-1.0 && tcoinmeC <= 1.0 && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut)
	{
	  hQ2WMEC1->Fill(Q2MEC, WMEC);	      	  
	  htmeC->Fill(-tmeC);
	}	      

           
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)) && mmmeC>= 0.92 && mmmeC <=0.98 && Diamond_cut)
  	{
	  htmeCR->Fill(-tmeC);
	  hQ2WMECR->Fill(Q2MEC, WMEC);
	}
      
      if(((tcoinmeC>=-15.0 && tcoinmeC <= -9.0) || (tcoinmeC>=7.0 && tcoinmeC <=13.0)))
	
	{
	  hmmmeCR->Fill(mmmeC);
	}	
    }  


  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL1; i++)
  
    {
      TBRANCHMIDEL1->GetEntry(i);
      
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98)
	{
	  hph_qmeL1->Fill(ph_qmeL1);	  
	}
      
      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0)    
	{
	  hmmmeL1->Fill(mmmeL1);
	}           
      
      {
	htcoinmeL1->Fill(tcoinmeL1);
      }
      
      Double_t Diamond_cut = (Diamond->IsInside(Q2MEL1, WMEL1));  

      if(tcoinmeL1>=-1.0 && tcoinmeL1 <= 1.0 && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut)
	{
	  htmeL1->Fill(-tmeL1);
	}

      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)) && mmmeL1>= 0.92 && mmmeL1 <=0.98 && Diamond_cut)
 	
	{
	  htmeL1R->Fill(-tmeL1);
	}
      
      if(((tcoinmeL1>=-15.0 && tcoinmeL1 <= -9.0) || (tcoinmeL1>=7.0 && tcoinmeL1 <=13.0)))
	
	{	 
	  hmmmeL1R->Fill(mmmeL1);
	}     
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHMIDEL2; i++)
    {
      TBRANCHMIDEL2->GetEntry(i);
      
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98)
  
	{
	  hph_qmeL2->Fill(ph_qmeL2);
	}
      
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0)
	
	{
	  hmmmeL2->Fill(mmmeL2);
	}   
      
      {
	htcoinmeL2->Fill(tcoinmeL2);
      }
      
      Double_t Diamond_cut = (Diamond->IsInside(Q2MEL2, WMEL2));  
      
      if(tcoinmeL2>=-1.0 && tcoinmeL2 <= 1.0 && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut)
	
	{
	  htmeL2->Fill(-tmeL2);
	}

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)) && mmmeL2>= 0.92 && mmmeL2 <=0.98 && Diamond_cut)
	{
	  htmeL2R->Fill(-tmeL2);
	}

      if(((tcoinmeL2>=-15.0 && tcoinmeL2 <= -9.0) || (tcoinmeL2>=7.0 && tcoinmeL2 <=13.0)))
	{
	  hmmmeL2R->Fill(mmmeL2);
	}
    }
  
  hQ2WMECR->Scale(1.0/6.0);

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
  
  TCanvas *c10me = new TCanvas("c10me", " c10me"); 
  hph_qmeR2->SetStats(0);
  hph_qmeR2->SetLineColor(kYellow);
  hph_qmeR2->Draw(" Weight");
  
  hph_qmeC->SetStats(0);
  hph_qmeC->SetLineColor(kGreen);
  hph_qmeC->Draw("same Weight");

  hph_qmeL2->SetStats(0);
  hph_qmeL2->SetLineColor(kBlack);
  hph_qmeL2->Draw("same Weight");
  
  hph_qmeL1->SetStats(0);
  hph_qmeL1->SetLineColor(kRed);
  hph_qmeL1->Draw("same Weight");
 
  hph_qmeR1->SetStats(0);
  hph_qmeR1->SetLineColor(kBlue);
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
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12me->cd(2); 
  hQ2WMEC1->Draw("COL");
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12me->Print(outputpdf);


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

  TH1D *htheR1  = new TH1D("htheR1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheC   = new TH1D("htheC","MandelT; MandelT;",    300, -0.01, 0.1);      
  TH1D *htheL1  = new TH1D("htheL1","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2  = new TH1D("htheL2","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *htheR1R  = new TH1D("htheR1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheCR   = new TH1D("htheCR","MandelT; MandelT;",     300, -0.01, 0.1);      
  TH1D *htheL1R  = new TH1D("htheL1R","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htheL2R  = new TH1D("htheL2R","MandelT; MandelT;",   300, -0.01, 0.1);      

  TH1D *hph_qheR1  = new TH1D("hph_qheR1","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qheC   = new TH1D("hph_qheC","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qheL1  = new TH1D("hph_qheL1","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qheL2  = new TH1D("hph_qheL2","ph_q; ph_q;",   300, -5.0, 5.0);      

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
      
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98)
	{
	  hph_qheR1->Fill(ph_qheR1);
	}
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0)    
	{
	  hmmheR1->Fill(mmheR1);	  
	}	      
      
      {
	htcoinheR1->Fill(tcoinheR1);
      }
      Double_t Diamond_cut = (Diamond->IsInside(Q2HER1, WHER1));  
      
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut)
	{
	  htheR1->Fill(-theR1);
	}
      
      if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.98 && Diamond_cut)
	{
	  htheR1R->Fill(-theR1);
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
	  hph_qheC->Fill(ph_qheC);
	}
 
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0)    
 	{
	  hmmheC->Fill(mmheC);	  
	}	      
      
      {
	htcoinheC->Fill(tcoinheC);
      }
      
      Double_t Diamond_cut = (Diamond->IsInside(Q2HEC, WHEC));  
      
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut)
	{
	  hQ2WHEC1->Fill(Q2HEC, WHEC);	      	  
	  htheC->Fill(-theC);
	}	      

           
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.98 && Diamond_cut)
  	{
	  htheCR->Fill(-theC);
	  hQ2WHECR->Fill(Q2HEC, WHEC);
	}
      
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)))
	
	{
	  hmmheCR->Fill(mmheC);
	}	
    }  


  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL1; i++)
  
    {
      TBRANCHHIGHEL1->GetEntry(i);
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98)
	{
	  hph_qheL1->Fill(ph_qheL1);	  
	}
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0)    
	{
	  hmmheL1->Fill(mmheL1);
	}           
      
      {
	htcoinheL1->Fill(tcoinheL1);
      }

      Double_t Diamond_cut = (Diamond->IsInside(Q2HEL1, WHEL1));  

      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut)
	{
	  htheL1->Fill(-theL1);
	}      

      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.98 && Diamond_cut)
 	
	{
	  htheL1R->Fill(-theL1);
	}
      
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)))
	
	{	 
	  hmmheL1R->Fill(mmheL1);
	}     
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHHIGHEL2; i++)
    {
      TBRANCHHIGHEL2->GetEntry(i);
      
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98)
  
	{
	  hph_qheL2->Fill(ph_qheL2);
	}
      
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0)
	
	{
	  hmmheL2->Fill(mmheL2);
	}   
      
      {
	htcoinheL2->Fill(tcoinheL2);
      }

      Double_t Diamond_cut = (Diamond->IsInside(Q2HEL2, WHEL2));  

      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut)
  
	{
	  htheL2->Fill(-theL2);
	}

      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.98 && Diamond_cut)
	{
	  htheL2R->Fill(-theL2);
	}

      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)))
	{
	  hmmheL2R->Fill(mmheL2);
	}
    }
  
  hQ2WHECR->Scale(1.0/6.0);

  hmmheR1R->Scale(1.0/6.0);
  hmmheCR->Scale(1.0/6.0);
  hmmheL1R->Scale(1.0/6.0);
  hmmheL2R->Scale(1.0/6.0);

  htheR1R->Scale(1.0/6.0);
  htheCR->Scale(1.0/6.0);
  htheL1R->Scale(1.0/6.0);
  htheL2R->Scale(1.0/6.0);

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
  
  TCanvas *c10 = new TCanvas("c10", " c10"); 
  
  hph_qheC->SetStats(0);
  hph_qheC->SetLineColor(kGreen);
  hph_qheC->Draw(" Weight");

  hph_qheL2->SetStats(0);
  hph_qheL2->SetLineColor(kBlack);
  hph_qheL2->Draw("same Weight");
  
  hph_qheL1->SetStats(0);
  hph_qheL1->SetLineColor(kRed);
  hph_qheL1->Draw("same Weight");
 
  hph_qheR1->SetStats(0);
  hph_qheR1->SetLineColor(kBlue);
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
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12->cd(2); 
  hQ2WHEC1->Draw("COL");
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12->Print(outputpdf);

  //-t for all three e
  TCanvas *c13 = new TCanvas("c13", " c13");
  //High epsilon  
  htheC->SetStats(0);
  htheC->SetLineColor(kGreen);
  htheC->Add(htheCR,-1);
  TAxis *In1 = htheC->GetXaxis();
  Double_t heC =  htheC->Integral(In1->FindBin(0.005), In1->FindBin(0.08), "");
  cout<< " High epsilon C events: "<<heC<<endl;
  htheC->Draw("Weight");

  htheR1->SetStats(0);
  htheR1->SetLineColor(kGreen);
  htheR1->Add(htheR1R,-1); 
  TAxis *In2 = htheR1->GetXaxis();
  Double_t heR1 =  htheR1->Integral(In2->FindBin(0.005), In2->FindBin(0.08), "");
  cout<< " High epsilon R1 events: "<<heR1<<endl;
  htheR1->Draw("same Weight");

  htheL1->SetStats(0);
  htheL1->SetLineColor(kGreen);
  htheL1->Add(htheL1R, -1); 
  TAxis *In3 = htheL1->GetXaxis();
  Double_t heL1 =  htheL1->Integral(In3->FindBin(0.005), In3->FindBin(0.08), "");
  cout<< " High epsilon L1 events: "<<heL1<<endl;
  htheL1->Draw("same Weight");

  htheL2->SetStats(0);
  htheL2->SetLineColor(kGreen);
  htheL2->Add(htheL2R, -1); 
  TAxis *In4 = htheL2->GetXaxis();
  Double_t heL2 =  htheL2->Integral(In4->FindBin(0.005), In4->FindBin(0.08), "");
  cout<< " High epsilon L2 events: "<<heL2<<endl; 
  htheL2->Draw("same Weight");
 
  // Mid epsilon
  
  htmeC->SetStats(0);
  htmeC->SetLineColor(kBlue);
  htmeC->Add(htmeCR,-1); 
  TAxis *In5 = htmeC->GetXaxis();
  Double_t meC =  htmeC->Integral(In5->FindBin(0.005), In5->FindBin(0.08), "");
  cout<< " Mid epsilon C events: "<<meC<<endl; 
  htmeC->Draw("same Weight");

  htmeR1->SetStats(0);
  htmeR1->SetLineColor(kBlue);
  htmeR1->Add(htmeR1R,-1); 
  TAxis *In6 = htmeR1->GetXaxis();
  Double_t meR1 =  htmeR1->Integral(In6->FindBin(0.005), In6->FindBin(0.08), "");
  cout<< " Mid epsilon R1 events: "<<meR1<<endl; 
  htmeR1->Draw("same Weight");

  htmeR2->SetStats(0);
  htmeR2->SetLineColor(kBlue);
  htmeR2->Add(htmeR2R,-1); 
  TAxis *In7 = htmeR2->GetXaxis();
  Double_t meR2 =  htmeR2->Integral(In7->FindBin(0.005), In7->FindBin(0.08), "");
  cout<< " Mid epsilon R2 events: "<<meR2<<endl; 
  htmeR2->Draw("same Weight");

  htmeL1->SetStats(0);
  htmeL1->SetLineColor(kBlue);
  htmeL1->Add(htmeL1R, -1); 
  TAxis *In8 = htmeL1->GetXaxis();
  Double_t meL1 =  htmeL1->Integral(In8->FindBin(0.005), In8->FindBin(0.08), "");
  cout<< " Mid epsilon L1 events: "<<meL1<<endl; 
  htmeL1->Draw("same Weight");

  htmeL2->SetStats(0);
  htmeL2->SetLineColor(kBlue);
  htmeL2->Add(htmeL2R, -1); 
  TAxis *In9 = htmeL2->GetXaxis();
  Double_t meL2 =  htmeL2->Integral(In9->FindBin(0.005), In9->FindBin(0.08), "");
  cout<< " Mid epsilon L2 events: "<<meL2<<endl; 
  htmeL2->Draw("same Weight");

  //Low epsilon

  htC->SetStats(0);
  htC->SetLineColor(kRed);
  htC->Add(htCR,-1); 
  TAxis *In10 = htC->GetXaxis();
  Double_t leC =  htC->Integral(In10->FindBin(0.005), In10->FindBin(0.08), "");
  cout<< " Low epsilon C events: "<<leC<<endl; 
  htC->Draw("same Weight");

  htL1->SetStats(0);
  htL1->SetLineColor(kRed);
  htL1->Add(htL1R, -1); 
  TAxis *In11 = htL1->GetXaxis();
  Double_t leL1 =  htL1->Integral(In11->FindBin(0.005), In11->FindBin(0.08), "");
  cout<< " Low epsilon L1 events: "<<leL1<<endl; 
  htL1->Draw("same Weight");
 
  htL2->SetStats(0);
  htL2->SetLineColor(kRed);
  htL2->Add(htL2R, -1); 
  TAxis *In12 = htL2->GetXaxis();
  Double_t leL2 =  htL2->Integral(In12->FindBin(0.005), In12->FindBin(0.08), "");
  cout<< " Low epsilon L2 events: "<<leL2<<endl; 
  htL2->Draw(" same Weight");
  //choose -t
  
  cout<<In1->FindBin(0.005)<<endl;
  cout<<In1->FindBin(0.08)<<endl;
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

  // if(In1->FindBin(0.005) >=41 && In1->FindBin(0.005) <= 246)  
    {
      //      for(Long64_t i = 41; i < 61; i++)       // 1st t
      //      for(Long64_t i = 44; i < 61; i++)       // 1st t  updated one
      // for(Long64_t i = 61; i < 70; i++)           // 2nd t
      //      for(Long64_t i = 70; i < 81; i++)     // 3rd t
      //     for(Long64_t i = 81; i < 93; i++)     // 4th t
      // for(Long64_t i = 93; i < 107; i++)           // 5th t
      //      for(Long64_t i = 107; i < 126; i++)           // 6th t
      //      for(Long64_t i = 126; i < 153; i++)           // 7th t
      for(Long64_t i = 153; i < 247; i++)           // 8th t
	//for(Long64_t i = 41; i < 45; i++)           //test
	
	{
	  Events1 += htheC->GetBinContent(i);
	  Events2 += htheR1->GetBinContent(i);
	  Events3 += htheL1->GetBinContent(i);
	  Events4 += htheL2->GetBinContent(i);
	  Events5 += htmeC->GetBinContent(i);
	  Events6 += htmeR1->GetBinContent(i);
	  Events7 += htmeR2->GetBinContent(i);
	  Events8 += htmeL1->GetBinContent(i);
	  Events9 += htmeL2->GetBinContent(i);
	  Events10 += htC->GetBinContent(i);
	  Events11 += htL1->GetBinContent(i);
	  Events12 += htL2->GetBinContent(i);
	}   
      
      cout<< "  "<<Events1<<endl;    
      cout<< "  "<<Events2<<endl;    
      cout<< "  "<<Events3<<endl;    
      cout<< "  "<<Events4<<endl;    
      cout<< "  "<<Events5<<endl;    
      cout<< "  "<<Events6<<endl;    
      cout<< "  "<<Events7<<endl;    
      cout<< "  "<<Events8<<endl;    
      cout<< "  "<<Events9<<endl;    
      cout<< "  "<<Events10<<endl;    
      cout<< "  "<<Events11<<endl;    
      cout<< "  "<<Events12<<endl;    
    }
    
    cout<<" Total Events: " <<Events1 + Events2 +Events3 +Events4 +Events5 +Events6 +Events7 +Events8 +Events9 +Events10 +Events11 +Events12 <<endl;
  

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

  Double_t lastedge = ((TAxis*)htheC->GetXaxis())->GetBinCenter(246);
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

 c13->Print(outputpdf + ')');
}
