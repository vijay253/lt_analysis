// .... Created Date: Feb 2, 2023 ....
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
//done

void diamond_cut(string Setting = "")
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
  if(Setting == "") {
    cout << "Enter Run Number: ";
    cin >> Setting;
  }  
 
  TString TInDATAFilename = "Add_Analysed_Data_"+Setting+"_mide.root" ;
  rootFile_DATA = ROOTfilePath+"/"+TInDATAFilename;

  if (gSystem->AccessPathName(rootFile_DATA) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATA <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  

  TFile *InFile_DATA = new TFile(rootFile_DATA, "READ");
  TString TOutFilename = "Diamond_"+Setting;
  //Output file names 
  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
 
  TTree* TBRANCH = (TTree*)InFile_DATA->Get("Uncut_Kaon_Events");Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();  
  //SHMS SIMC variables
  
  Double_t PI_COIN_TIME;TBRANCH->SetBranchAddress("CTime_ROC1", &PI_COIN_TIME);
  Double_t Q2;TBRANCH->SetBranchAddress("Q2", &Q2);
  Double_t W;TBRANCH->SetBranchAddress("W", &W);
   
  TH1D *PiCoinTime  = new TH1D("PiCoinTime","CTime_ROC1; PiCoinTime;", 300, -20, 20);      
  TH2D *Q2W         = new TH2D("Q2W","Diamond; Q2 (GeV2); W (GeV);", 50, 0.2, 0.6, 50, 2.0, 2.6);  
  for(Long64_t i = 0; i < nEntries_TBRANCH; i++)
    {
      TBRANCH->GetEntry(i);

      {
        PiCoinTime->Fill(PI_COIN_TIME);
	Q2W->Fill(Q2,W);
      }
    
    }

  TCanvas *c = new TCanvas("c", " Pion coin time");
  PiCoinTime->Draw("");
  c->Print(outputpdf + '(');
  Q2W->Draw("COL");
  c->Print(outputpdf + ')');
  
  //xfp->Print(outputpdf + '(');
  //tmp->Print(outputpdf + ')');

}
