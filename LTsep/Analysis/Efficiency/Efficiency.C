// .... Created Date: July 28, 2023 ....
// .... Author: VK ....
// .... This script is made for Efficiency ..........  
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
#include <fstream>
#include <iostream>

void Efficiency(string RunNum = "", Double_t SHMStreff = 0, Double_t SHMStrefferr = 0,  Double_t HMStreff = 0, Double_t HMStrefferr = 0, Double_t SHMS_S1X_Rate =0, Double_t HMS_S1X_Rate =0)
{
  TString Hostname = gSystem->HostName();
  TString User = (gSystem->GetUserInfo())->fUser;
  TString Replaypath;
  TString Analysispath;
  TString ROOTfilePath;
  TString OutPath;
  TString rootFile;

  gStyle->SetPalette(55);


  // Set paths depending on system you're running on
  if(Hostname.Contains("farm")){
    Analysispath = "/group/c-kaonlt/USERS/"+User+"/lt_analysis/LTsep/Analysis/Efficiency";
    ROOTfilePath = "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/ROOTfiles/";
    OutPath      = "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/OUTPUT/";
  }
  else if(Hostname.Contains("qcd")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    OutPath = Replaypath+"/UTIL_KAONLT/scripts/CoinTimePeak/OUTPUT";
  }
  else if (Hostname.Contains("phys.uregina.ca")){
    ROOTfilePath = "/home/vijay/work/HeepCoinStudy/";
    OutPath = "/home/vijay/work/HeepCoinStudy/";
  }

  rootFile = ROOTfilePath+"/"+"Kaon_coin_replay_production_"+RunNum+"_-1.root";
  if (gSystem->AccessPathName(rootFile) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  // Add more as needed for your own envrionment
  if(RunNum == "") {
    cout << "Enter RunNum: ";
    cin >> RunNum;
  }  
  
  if(SHMStreff == 0) {
    cout << "Enter SHMStreff: ";
    cin >> SHMStreff;    
  }
  /*
  if(SHMStrefferr == 0) {
    cout << "Enter SHMStrefferr: ";
    cin >> SHMStrefferr;    
  }
  */
  if(HMStreff == 0) {
    cout << "Enter HMStreff: ";
    cin >> HMStreff;    
  }
  /*
  if(HMStrefferr == 0) {
    cout << "Enter HMStrefferr: ";
    cin >> HMStrefferr;    
  }
  */
  ofstream file("OUTPUT/"+RunNum+".dat");;
  file<< RunNum << "\t"<< SHMStreff << "\t" << SHMStrefferr << "\t" << HMStreff << "\t" << HMStrefferr << "\t"<< SHMS_S1X_Rate << "\t" << HMS_S1X_Rate << endl;
  file.close();
}
