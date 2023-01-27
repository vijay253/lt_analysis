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

void PlotScript(string RunNum = "")
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
  if(RunNum == "") {
    cout << "Enter Run Number: ";
    cin >> RunNum;
  }  
 
  TString TInDATAFilename = RunNum+"_-1_Proc_Data.root" ;
  rootFile_DATA = ROOTfilePath+"/"+TInDATAFilename;

  if (gSystem->AccessPathName(rootFile_DATA) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DATA <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }
  

  TFile *InFile_DATA = new TFile(rootFile_DATA, "READ");
  TString TOutFilename = RunNum+"_PiCoinTime";
  //Output file names 
  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
 
  TTree* TBRANCH = (TTree*)InFile_DATA->Get("Uncut_Kaon_Events");Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();  
 //SHMS SIMC variables
  
  Double_t PI_COIN_TIME;TBRANCH->SetBranchAddress("CTime_ROC1", &PI_COIN_TIME);
   
  TH1D *PiCoinTime  = new TH1D("PiCoinTime","CTime_ROC1; PiCoinTime;", 300, -20, 20);      
 
  for(Long64_t i = 0; i < nEntries_TBRANCH; i++)
    {
      TBRANCH->GetEntry(i);

        {
	  PiCoinTime->Fill(PI_COIN_TIME);
	}
	
    }

  TCanvas *c = new TCanvas("c", " Pion coin time");
  PiCoinTime->Draw("");
  c->Print(outputpdf);
  
  //xfp->Print(outputpdf + '(');
  //tmp->Print(outputpdf + ')');

}
