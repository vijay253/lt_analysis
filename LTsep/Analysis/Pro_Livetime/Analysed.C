// .... Created Date: Aug 11, 2023 ....
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
#include <fstream>
#include <iostream>

void Analysed(string RunNum = "", Double_t Coin_trig_rate = 0, Double_t EDTMLIVE = 0, Double_t EDTMLIVEERR = 0)
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
    Analysispath = "/group/c-kaonlt/USERS/"+User+"/lt_analysis/LTsep/Analysis/Pro_Livetime";
    // Output path for root file
    ROOTfilePath = "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/ROOTfiles/";
    OutPath = "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/";
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
    cout << "Enter RunNum: ";
    cin >> RunNum;
  }  
  
  ofstream file("OUTPUT/"+RunNum+".dat");;
  file<< RunNum <<"\t"<< EDTMLIVE <<"\t"<< EDTMLIVEERR << "\t"<< Coin_trig_rate <<endl;
  file.close();
}
