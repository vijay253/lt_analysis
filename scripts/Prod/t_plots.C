// .... Created Date: Dec 28, 2021 ....
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

  // Add more as needed for your own envrionment
 
  TString TInDATAFilenameC = "Analysed_Data_Q0p375W2p2_lowe_Center.root";
  TString TInDATAFilenameL1 = "Analysed_Data_Q0p375W2p2_lowe_Left1.root";
  TString TInDATAFilenameL2 = "Analysed_Data_Q0p375W2p2_lowe_Left2.root";
 
  TString rootFile_DATAC  = ROOTfilePath+"/"+TInDATAFilenameC;
  TString rootFile_DATAL1 = ROOTfilePath+"/"+TInDATAFilenameL1;
  TString rootFile_DATAL2 = ROOTfilePath+"/"+TInDATAFilenameL2;

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
  

  TFile *InFile_DATAC = new TFile(rootFile_DATAC, "READ");
  TFile *InFile_DATAL1 = new TFile(rootFile_DATAL1, "READ");
  TFile *InFile_DATAL2 = new TFile(rootFile_DATAL2, "READ");
  TString TOutFilename = "Analysed_Q0p375W2p2_lowe";
  //Output file names 
  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
 
  TTree* TBRANCHC  = (TTree*)InFile_DATAC->Get("Uncut_Kaon_Events");Long64_t nEntries_TBRANCHC  = (Long64_t)TBRANCHC->GetEntries();  
  TTree* TBRANCHL1 = (TTree*)InFile_DATAL1->Get("Uncut_Kaon_Events");Long64_t nEntries_TBRANCHL1  = (Long64_t)TBRANCHL1->GetEntries();  
  TTree* TBRANCHL2 = (TTree*)InFile_DATAL2->Get("Uncut_Kaon_Events");Long64_t nEntries_TBRANCHL2  = (Long64_t)TBRANCHL2->GetEntries();  
  
  Double_t W;TBRANCHC->SetBranchAddress("W", &W);
  Double_t Q2;TBRANCHC->SetBranchAddress("Q2", &Q2);
  
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

  TH2D *hQ2WC  = new TH2D("hQ2WC","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH1D *htC  = new TH1D("htC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htL1  = new TH1D("htL1","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htL2  = new TH1D("htL2","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *htdC  = new TH1D("htdC","MandelT; MandelT;",   300, -0.01, 0.1);      
  TH1D *htdL1  = new TH1D("htdL1","MandelT; MandelT;", 300, -0.01, 0.1);      
  TH1D *htdL2  = new TH1D("htdL2","MandelT; MandelT;", 300, -0.01, 0.1);      

  TH1D *hph_qC  = new TH1D("hph_qC","ph_q; ph_q;",   300, -5.0, 5.0);      
  TH1D *hph_qL1  = new TH1D("hph_qL1","ph_q; ph_q;", 300, -5.0, 5.0);      
  TH1D *hph_qL2  = new TH1D("hph_qL2","ph_q; ph_q;", 300, -5.0, 5.0);      

  TH1D *hmmC  = new TH1D("hmmC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL1  = new TH1D("hmmL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmL2  = new TH1D("hmmL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *hmmdC  = new TH1D("hmmdC","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmdL1  = new TH1D("hmmdL1","MM; MM;", 300, 0.8, 1.2);      
  TH1D *hmmdL2  = new TH1D("hmmdL2","MM; MM;", 300, 0.8, 1.2);      

  TH1D *htcoinC   = new TH1D("htcoinC","CTime_ROC1; CTime_ROC1;",  300, -20.0, 20.0);      
  TH1D *htcoinL1  = new TH1D("htcoinL1","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      
  TH1D *htcoinL2  = new TH1D("htcoinL2","CTime_ROC1; CTime_ROC1;", 300, -20.0, 20.0);      

  TCutG *Diamond = new TCutG("Diamond",5);
  Diamond->SetVarX("Q2");
  Diamond->SetVarY("W");
  Diamond->SetPoint(0,0.362029,2.18059);Diamond->SetPoint(1,0.294241,2.23339); 
  Diamond->SetPoint(2,0.411468,2.20738); Diamond->SetPoint(3,0.464985,2.1573);
  Diamond->SetPoint(4,0.362029,2.18059); 

  double Q21[nEntries_TBRANCHC];
  double W1[nEntries_TBRANCHC]; 

  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);
    
      {
	htcoinC->Fill(tcoinC);
	hQ2WC->Fill(Q2, W);
	Q21[i] = Q2;
	W1[i] = W;
      }
      
    } 

  auto mg1 = new TMultiGraph("mg1","mg1");
  auto gr1 = new TGraph( nEntries_TBRANCHC, Q21, W1);
  gr1->SetName("gr1");
  gr1->SetTitle("low epsilon");
  gr1->SetMarkerStyle(6);
  gr1->SetDrawOption("AP");
  gr1->SetMarkerColor(3);
  mg1->Add(gr1);
  mg1->GetXaxis()->SetLimits(0.2,0.6);
  mg1->GetYaxis()->SetRangeUser(2.1, 2.3);
 
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
	  {
	    TBRANCHC->GetEntry(i);
	    
	    Double_t Diamond_cut = (Diamond->IsInside(Q2, W));  
	    Double_t picoitimecuts = tcoinC>=-2.0 && tcoinC <= 0.0; 
  	    
	    if(Diamond_cut)
	      {
		hQ2WC1->Fill(Q2, W);	      
	    
	      }	      

	  }

  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
    {
      TBRANCHL1->GetEntry(i);

      {
	htcoinL1->Fill(tcoinL1);
      }
	
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);

      {
	htcoinL2->Fill(tcoinL2);
      }
	
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {

      TBRANCHC->GetEntry(i);
      Double_t picoitimecuts = tcoinC>=-2.3 && tcoinC <= -0.3; //&& mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecuts)
	{
	  hph_qC->Fill(ph_qC);
	  hmmC->Fill(mmC);
	}	
 
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {

      TBRANCHC->GetEntry(i);
      Double_t picoitimecuts = tcoinC>=-2.3 && tcoinC <= -0.3 && mmC>= 0.85 && mmC <=0.91; 
      
      if(picoitimecuts)
	{
	  htC->Fill(-tC);
	}
    }

  
  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
    {
      TBRANCHL1->GetEntry(i);
      Double_t picoitimecutsL1 = tcoinL1>=-2.3 && tcoinL1 <= -0.3;// && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsL1)
	{
	  hph_qL1->Fill(ph_qL1);
	  hmmL1->Fill(mmL1);
	}
	
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
    {
      TBRANCHL1->GetEntry(i);
      Double_t picoitimecutsL1 = tcoinL1>=-2.3 && tcoinL1 <= -0.3 && mmL1>= 0.85 && mmL1 <=0.91; 
      
      if(picoitimecutsL1)
	{
	  htL1->Fill(-tL1);
	}
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      Double_t picoitimecutsL2 = tcoinL2>=-2.3 && tcoinL2 <= -0.3;// && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsL2)

	{
	  hph_qL2->Fill(ph_qL2);
	  hmmL2->Fill(mmL2);
	}
	
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      Double_t picoitimecutsL2 = tcoinL2>=-2.3 && tcoinL2 <= -0.3  && mmL2>= 0.85 && mmL2 <=0.91; 

      if(picoitimecutsL2)

	{
	  htL2->Fill(-tL2);
	}
    }


  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);
      Double_t picoitimecutsd = ((tcoinC>=-14.0 && tcoinC <= -8.0) || (tcoinC>=6.0 && tcoinC <=12.0));// && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsd)
	{
	  hmmdC->Fill(mmC);
	}	
 
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)
    {
      TBRANCHC->GetEntry(i);
      Double_t picoitimecutsd = ((tcoinC>=-14.0 && tcoinC <= -8.0) || (tcoinC>=6.0 && tcoinC <=12.0)) && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsd)
	{
	  htdC->Fill(-tC);
	}	
 
    }

  
  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
    {
      TBRANCHL1->GetEntry(i);
      Double_t picoitimecutsdL1 = ((tcoinL1>=-14.0 && tcoinL1 <= -8.0) || (tcoinL1>=6.0 && tcoinL1 <=12.0));// && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsdL1)
	{
	  hmmdL1->Fill(mmL1);
	}
	
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
    {
      TBRANCHL1->GetEntry(i);
      Double_t picoitimecutsdL1 = ((tcoinL1>=-14.0 && tcoinL1 <= -8.0) || (tcoinL1>=6.0 && tcoinL1 <=12.0)) && mmL1>= 0.85 && mmL1 <=0.91; 

      if(picoitimecutsdL1)
	{
	  htdL1->Fill(-tL1);
	}
    }



  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      Double_t picoitimecutsdL2 = ((tcoinL2>=-14.0 && tcoinL2 <= -8.0) || (tcoinL2>=6.0 && tcoinL2 <=12.0));// && mmC>= 0.85 && mmC <=0.91; 

      if(picoitimecutsdL2)

	{
	  hmmdL2->Fill(mmL2);
	}
    }

  for(Long64_t i = 0; i < nEntries_TBRANCHL2; i++)
    {
      TBRANCHL2->GetEntry(i);
      Double_t picoitimecutsdL2 = ((tcoinL2>=-14.0 && tcoinL2 <= -8.0) || (tcoinL2>=6.0 && tcoinL2 <=12.0)) && mmL2>= 0.85 && mmL2 <=0.91; 

      if(picoitimecutsdL2)

	{
	  htdL2->Fill(-tL2);
	}
    }

  hmmdC->Scale(1.0/6.0);
  hmmdL1->Scale(1.0/6.0);
  hmmdL2->Scale(1.0/6.0);

  htdC->Scale(1.0/6.0);
  htdL1->Scale(1.0/6.0);
  htdL2->Scale(1.0/6.0);

  TCanvas *c1 = new TCanvas("c1", " pion"); 
  htcoinL2->SetStats(0);
  htcoinL2->GetYaxis()->SetRangeUser(0.0,4000);
  htcoinL2->SetLineColor(kBlack);
  htcoinL2->Draw("Weight");
  htcoinL1->SetStats(0);
  htcoinL1->SetLineColor(kRed);
  htcoinL1->Draw("same Weight ");
  htcoinC->SetStats(0);
  htcoinC->SetLineColor(kGreen);
  htcoinC->Draw("same Weight");
  TLine *l1 = new TLine(-2.3, 0.0, -2.3, 4000.0);
  TLine *l2 = new TLine(-0.3, 0.0, -0.3, 4000.0);
  TLine *l3 = new TLine(-14.0, 0.0, -14.0, 4000.0);
  TLine *l4 = new TLine(-8.0, 0.0, -8.0, 4000.0);
  TLine *l5 = new TLine(6.0, 0.0, 6.0, 4000.0);
  TLine *l6 = new TLine(12.0, 0.0, 12.0, 4000.0);
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
  leg->AddEntry(htcoinC,"Center","l");
  leg->AddEntry(htcoinL1,"Left1","l");
  leg->AddEntry(htcoinL2,"Left2","l");
  leg->Draw("same");
  c1->Print(outputpdf + '(');

  TCanvas *c3 = new TCanvas("c3", " c3"); 
  htC->SetStats(0);
  htC->SetLineColor(kGreen);
  htC->Add(htdC,-1); 
  htC->Draw("Weight");
  htL1->SetStats(0);
  htL1->SetLineColor(kRed);
  htL1->Add(htdL1, -1); 
  htL1->Draw("same Weight");
  htL2->SetStats(0);
  htL2->SetLineColor(kBlack);
  htL2->Add(htdL2, -1); 
  htL2->Draw(" same Weight");
  //  TLine *l1 = new TLine(-0.15, 0.0, -0.15, 1300.0);
  l1->SetLineColor(kRed);
  // l1->Draw("same");
  // TLine *l2 = new TLine(0.1, 0.0, 0.1, 1300.0);
  l2->SetLineColor(kRed);
  // l2->Draw("same");
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
  hmmL2->Add(hmmdL2, -1);
  hmmL2->Draw(" Weight");
  hmmL1->SetStats(0);
  hmmL1->SetLineColor(kRed);
  hmmL1->Add(hmmdL1, -1);
  hmmL1->Draw("same Weight");
  hmmC->SetStats(0);
  hmmC->SetLineColor(kGreen);
  hmmL1->Add(hmmdL1, -1);
  hmmC->Draw("same Weight");
  c5->Print(outputpdf);

  TCanvas *c6 = new TCanvas("c6", " c6");
  c6->Divide(2);
  c6->cd(1);
  hQ2WC->SetStats(0);
  Diamond->SetLineColor(kRed);
  hQ2WC->Draw("COL");
  Diamond->Draw("same");  
  c6->cd(2);
  hQ2WC1->SetStats(0);
  Diamond->SetLineColor(kRed);
  hQ2WC1->Draw("COL");
  Diamond->Draw("same");   
  c6->Print(outputpdf);

  TCanvas *c7 = new TCanvas("c7", " c7");
  mg1->Draw("AP");
  Diamond->Draw("same");  
  c7->Print(outputpdf + ')');

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *Di = OutHisto_file->mkdir("Di");
  Di->cd();  
  hQ2WC->Write();
  OutHisto_file->Close();

}
