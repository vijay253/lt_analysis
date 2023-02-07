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

  //Output file names 
  TString foutname   = OutPath+"/" + "Analysed_Qp375W2p2" + ".root";
  TString fouttxt    = OutPath+"/" + "Analysed_Qp375W2p2" + ".txt";
  TString outputpdf  = OutPath+"/" + "Analysed_Qp375W2p2" + ".pdf";

  TTree* TBRANCHC  = (TTree*)InFile_DATAC->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHC  = (Long64_t)TBRANCHC->GetEntries();  
  TTree* TBRANCHL1 = (TTree*)InFile_DATAL1->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL1  = (Long64_t)TBRANCHL1->GetEntries();  
  TTree* TBRANCHL2 = (TTree*)InFile_DATAL2->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCHL2  = (Long64_t)TBRANCHL2->GetEntries();  
  
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

  TH2D *hQ2WC  = new TH2D("hQ2WC","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1  = new TH2D("hQ2WC1","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      

  TH2D *hQ2WCR  = new TH2D("hQ2WCR","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      
  TH2D *hQ2WC1R  = new TH2D("hQ2WC1R","W; Q2; W", 300, 0.2, 0.6, 300, 2.1, 2.3);      

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

  TCutG *Diamond = new TCutG("Diamond",5);
  Diamond->SetVarX("Q2");
  Diamond->SetVarY("W");
  Diamond->SetPoint(0,0.363783,2.18318);Diamond->SetPoint(1,0.311469,2.22694); 
  Diamond->SetPoint(2,0.37837,2.21236); Diamond->SetPoint(3,0.452314,2.16293);
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
  
  for(Long64_t i = 0; i < nEntries_TBRANCHC; i++)

    {
      TBRANCHC->GetEntry(i);
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.96)
	{
	  htC->Fill(-tC);
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
      
      if(tcoinC>=-1.0 && tcoinC <= 1.0 && mmC>= 0.92 && mmC <=0.96 && Diamond_cut)
	{
	  hQ2WC1->Fill(Q2C, WC);	      	  
	}	      
      
      if(((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)) && mmC>= 0.92 && mmC <=0.96)
	{
	  htCR->Fill(-tC);
	  hQ2WCR->Fill(Q2C, WC);
	}
      
      if (((tcoinC>=-15.0 && tcoinC <= -9.0) || (tcoinC>=7.0 && tcoinC <=13.0)))
	
	{
	  hmmCR->Fill(mmC);
	}	
    }  

  for(Long64_t i = 0; i < nEntries_TBRANCHL1; i++)
  
    {
      TBRANCHL1->GetEntry(i);
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0 && mmL1>= 0.92 && mmL1 <=0.96)
	{
	  htL1->Fill(-tL1);
	  hph_qL1->Fill(ph_qL1);	  
	}
      
      if(tcoinL1>=-1.0 && tcoinL1 <= 1.0)	
	{
	  hmmL1->Fill(mmL1);
	}           
      
      {
	htcoinL1->Fill(tcoinL1);
      }
      
      if(((tcoinL1>=-15.0 && tcoinL1 <= -9.0) || (tcoinL1>=7.0 && tcoinL1 <=13.0)) && mmL1>= 0.92 && mmL1 <=0.96)	
	
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
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0 && mmL2>= 0.92 && mmL2 <=0.96)
	{
	  htL2->Fill(-tL2);
	  hph_qL2->Fill(ph_qL2);
	}
      
      if(tcoinL2>=-1.0 && tcoinL2 <= 1.0)
	
	{
	  hmmL2->Fill(mmL2);
	}   
      
      {
	htcoinL2->Fill(tcoinL2);
      }

      if(((tcoinL2>=-15.0 && tcoinL2 <= -9.0) || (tcoinL2>=7.0 && tcoinL2 <=13.0)) && mmL2>= 0.92 && mmL2 <=0.96)
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
  TLine *l8 = new TLine(0.96, -100.0, 0.96, 2200.0);
  l7->SetLineColor(kBlue);
  l8->SetLineColor(kBlue);
  l7->Draw("same"); 
  l8->Draw("same");

  c5->Print(outputpdf);

  TCanvas *c6 = new TCanvas("c6", " c6");

  c6->Divide(2);
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
  c6->Print(outputpdf);

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

  TH2D *hQ2WHEC  = new TH2D("hQ2WHERC","W; Q2; W", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WHEC1  = new TH2D("hQ2WHERC1","W; Q2; W", 300, 0.2, 0.6, 300, 2.05, 2.4);      

  TH2D *hQ2WHECR  = new TH2D("hQ2WHECR","W; Q2; W", 300, 0.2, 0.6, 300, 2.05, 2.4);      
  TH2D *hQ2WHEC1R  = new TH2D("hQ2WHEC1R","W; Q2; W", 300, 0.2, 0.6, 300, 2.05, 2.4);      

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
      
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0 && mmheR1>= 0.92 && mmheR1 <=0.96)
	{
	  htheR1->Fill(-theR1);
	  hph_qheR1->Fill(ph_qheR1);
	}
      if(tcoinheR1>=-1.0 && tcoinheR1 <= 1.0)    
	{
	  hmmheR1->Fill(mmheR1);	  
	}	      
      
      {
	htcoinheR1->Fill(tcoinheR1);
      }
           
      if(((tcoinheR1>=-15.0 && tcoinheR1 <= -9.0) || (tcoinheR1>=7.0 && tcoinheR1 <=13.0)) && mmheR1>= 0.92 && mmheR1 <=0.96)
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
      
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.96)
	{
	  htheC->Fill(-theC);
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
      
      if(tcoinheC>=-1.0 && tcoinheC <= 1.0 && mmheC>= 0.92 && mmheC <=0.96 && Diamond_cut)
	{
	  hQ2WHEC1->Fill(Q2HEC, WHEC);	      	  
	}	      

           
      if(((tcoinheC>=-15.0 && tcoinheC <= -9.0) || (tcoinheC>=7.0 && tcoinheC <=13.0)) && mmheC>= 0.92 && mmheC <=0.96)
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
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0 && mmheL1>= 0.92 && mmheL1 <=0.96)
	{
	  htheL1->Fill(-theL1);
	  hph_qheL1->Fill(ph_qheL1);	  
	}
      
      if(tcoinheL1>=-1.0 && tcoinheL1 <= 1.0)    
	{
	  hmmheL1->Fill(mmheL1);
	}           
      
      {
	htcoinheL1->Fill(tcoinheL1);
      }
      
      if(((tcoinheL1>=-15.0 && tcoinheL1 <= -9.0) || (tcoinheL1>=7.0 && tcoinheL1 <=13.0)) && mmheL1>= 0.92 && mmheL1 <=0.96)
 	
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
      
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0 && mmheL2>= 0.92 && mmheL2 <=0.96)
  
	{
	  htheL2->Fill(-theL2);
	  hph_qheL2->Fill(ph_qheL2);
	}
      
      if(tcoinheL2>=-1.0 && tcoinheL2 <= 1.0)
	
	{
	  hmmheL2->Fill(mmheL2);
	}   
      
      {
	htcoinheL2->Fill(tcoinheL2);
      }

      if(((tcoinheL2>=-15.0 && tcoinheL2 <= -9.0) || (tcoinheL2>=7.0 && tcoinheL2 <=13.0)) && mmheL2>= 0.92 && mmheL2 <=0.96)
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
  TLine *l8he = new TLine(0.96, -5000.0, 0.96, 100000.0);
  l7he->SetLineColor(kBlue);
  l8he->SetLineColor(kBlue);
  l7he->Draw("same"); 
  l8he->Draw("same");
  c11->Print(outputpdf);
	    	    
  TCanvas *c12 = new TCanvas("c12", " c12");
  c12->Divide(2);
  c12->cd(1); 
  hQ2WHEC->Draw("COL");
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12->cd(2); 
  hQ2WHEC1->Draw("COL");
  Diamond->SetLineColor(kRed);
  Diamond->Draw("same");

  c12->Print(outputpdf + ')');
}
