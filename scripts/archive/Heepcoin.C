// .... This script has created to the Coin Heep Study....
// .... Created Date: October 22, 2021 ....
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

// Input should be the input root file name (including suffix) and an output file name string (without any suffix)
void Heepcoin(string InDATAFilename = "", string InDUMMYFilename = "",string InSIMCFilename = "", string OutFilename = "")
{
  TString Hostname = gSystem->HostName();
  TString User = (gSystem->GetUserInfo())->fUser;
  TString Replaypath;
  TString ROOTfilePath;
  TString OutPath;
  TString rootFile;
  TString rootFile_DUMMY;
  TString rootFile_SIMC;

  gStyle->SetPalette(55);

  // Set paths depending on system you're running on
  if(Hostname.Contains("farm")){
    //    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    //    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
    //    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";

    Replaypath = "/group/c-kaonlt/USERS/"+User+"/simc_gfortran";
    // Output path for root filw
    ROOTfilePath = Replaypath+"/OUTPUTS";
    OutPath = Replaypath+"/OUTPUTS";
  }
  else if(Hostname.Contains("qcd")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
  }
  else if (Hostname.Contains("phys.uregina.ca")){
    ROOTfilePath = "/home/vijay/work/HeepCoinStudy";
    OutPath = "/home/vijay/work/HeepCoinStudy";
  }

  // Add more as needed for your own envrionment
  if(InDATAFilename == "") {
    cout << "Enter a DATA ROOT Filename to analyse: ";
    cin >> InDATAFilename;
  }  

  if(InDUMMYFilename == "") {
    cout << "Enter a DUMMY ROOT Filename to analyse: ";
    cin >> InDUMMYFilename;
  }  

  if(InSIMCFilename == "") {
    cout << "Enter a SIMC ROOT Filename to analyse: ";
    cin >> InSIMCFilename;
  }  

  if(OutFilename == "") {
    cout << "Enter a Filename to output to: ";
    cin >> OutFilename;
  }

  TString TInDATAFilename = InDATAFilename ;
  TString TInDUMMYFilename = InDUMMYFilename ;
  TString TInSIMCFilename = InSIMCFilename ;
  rootFile = ROOTfilePath+"/"+TInDATAFilename;
  rootFile_DUMMY = ROOTfilePath+"/"+TInDUMMYFilename;
  rootFile_SIMC = ROOTfilePath+"/"+TInSIMCFilename;

  if (gSystem->AccessPathName(rootFile) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  if (gSystem->AccessPathName(rootFile_DUMMY) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile_DUMMY <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  if (gSystem->AccessPathName(rootFile_SIMC) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl << rootFile_SIMC <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  TFile *InFile_DATA = new TFile(rootFile, "OPEN");
  InFile_DATA->GetListOfKeys()->Print();
  TFile *InFile_DUMMY = new TFile(rootFile_DUMMY, "OPEN");
  TFile *InFile_SIMC = new TFile(rootFile_SIMC, "READ");
  TString TOutFilename = OutFilename;

  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
   
  //#################################################################### 
  
  TTree* TBRANCH_DATA  = (TTree*)InFile_DATA->Get("hist");Long64_t nEntries_TBRANCH_DATA  = (Long64_t)TBRANCH_DATA->GetEntries();  
  TTree* TBRANCH_DUMMY  = (TTree*)InFile_DUMMY->Get("hist");Long64_t nEntries_TBRANCH_DUMMY  = (Long64_t)TBRANCH_DUMMY->GetEntries();  
  TTree* TBRANCH_SIMC  = (TTree*)InFile_SIMC->Get("h10");Long64_t nEntries_TBRANCH_SIMC  = (Long64_t)TBRANCH_SIMC->GetEntries();
  
  //SIMC variables
  //HMS
  Float_t hsdelta;TBRANCH_SIMC->SetBranchAddress("hsdelta", &hsdelta);
  Float_t hsxptar;TBRANCH_SIMC->SetBranchAddress("hsxptar", &hsxptar);
  Float_t hsyptar;TBRANCH_SIMC->SetBranchAddress("hsyptar", &hsyptar);
  Float_t hsxfp;TBRANCH_SIMC->SetBranchAddress("hsxfp", &hsxfp);
  Float_t hsyfp;TBRANCH_SIMC->SetBranchAddress("hsyfp", &hsyfp);
  Float_t hsxpfp;TBRANCH_SIMC->SetBranchAddress("hsxpfp", &hsxpfp);
  Float_t hsypfp;TBRANCH_SIMC->SetBranchAddress("hsypfp", &hsypfp);
  //SHMS
  Float_t ssdelta;TBRANCH_SIMC->SetBranchAddress("ssdelta", &ssdelta);
  Float_t ssxptar;TBRANCH_SIMC->SetBranchAddress("ssxptar", &ssxptar);
  Float_t ssyptar;TBRANCH_SIMC->SetBranchAddress("ssyptar", &ssyptar);
  Float_t ssxfp;TBRANCH_SIMC->SetBranchAddress("ssxfp", &ssxfp);
  Float_t ssyfp;TBRANCH_SIMC->SetBranchAddress("ssyfp", &ssyfp);
  Float_t ssxpfp;TBRANCH_SIMC->SetBranchAddress("ssxpfp", &ssxpfp);
  Float_t ssypfp;TBRANCH_SIMC->SetBranchAddress("ssypfp", &ssypfp);
 
  Float_t q;TBRANCH_SIMC->SetBranchAddress("q", &q);
  Float_t Q2_simc;TBRANCH_SIMC->SetBranchAddress("Q2", &Q2_simc);
  Float_t W_simc;TBRANCH_SIMC->SetBranchAddress("W", &W_simc);
  Float_t epsilon_simc;TBRANCH_SIMC->SetBranchAddress("epsilon", &epsilon_simc);
  Float_t Pmx;TBRANCH_SIMC->SetBranchAddress("Pmx", &Pmx);
  Float_t Pmy;TBRANCH_SIMC->SetBranchAddress("Pmy", &Pmy);
  Float_t Pmz;TBRANCH_SIMC->SetBranchAddress("Pmz", &Pmz);
  Float_t Em;TBRANCH_SIMC->SetBranchAddress("Em", &Em);
  Float_t Pm;TBRANCH_SIMC->SetBranchAddress("Pm", &Pm);
  Float_t Weight;TBRANCH_SIMC->SetBranchAddress("Weight", &Weight);

  //DATA variables
  //HMS
  Double_t hsdelta_data;TBRANCH_DATA->SetBranchAddress("hsdelta", &hsdelta_data);
  Double_t hsxptar_data;TBRANCH_DATA->SetBranchAddress("hsxptar", &hsxptar_data);
  Double_t hsyptar_data;TBRANCH_DATA->SetBranchAddress("hsyptar", &hsyptar_data);
  Double_t hsxfp_data;TBRANCH_DATA->SetBranchAddress("hsxfp", &hsxfp_data);
  Double_t hsyfp_data;TBRANCH_DATA->SetBranchAddress("hsyfp", &hsyfp_data);
  Double_t hsxpfp_data;TBRANCH_DATA->SetBranchAddress("hsxpfp", &hsxpfp_data);
  Double_t hsypfp_data;TBRANCH_DATA->SetBranchAddress("hsypfp", &hsypfp_data);
  //SHMS
  Double_t ssdelta_data;TBRANCH_DATA->SetBranchAddress("ssdelta", &ssdelta_data);
  Double_t ssxptar_data;TBRANCH_DATA->SetBranchAddress("ssxptar", &ssxptar_data);
  Double_t ssyptar_data;TBRANCH_DATA->SetBranchAddress("ssyptar", &ssyptar_data);
  Double_t ssxfp_data;TBRANCH_DATA->SetBranchAddress("ssxfp", &ssxfp_data);
  Double_t ssyfp_data;TBRANCH_DATA->SetBranchAddress("ssyfp", &ssyfp_data);
  Double_t ssxpfp_data;TBRANCH_DATA->SetBranchAddress("ssxpfp", &ssxpfp_data);
  Double_t ssypfp_data;TBRANCH_DATA->SetBranchAddress("ssypfp", &ssypfp_data);
 
  Double_t Q2_data;TBRANCH_DATA->SetBranchAddress("Q2", &Q2_data);
  Double_t W_data;TBRANCH_DATA->SetBranchAddress("W", &W_data);
  Double_t epsilon_data;TBRANCH_DATA->SetBranchAddress("epsilon", &epsilon_data);
  Double_t pmx_data;TBRANCH_DATA->SetBranchAddress("pmx", &pmx_data);
  Double_t pmy_data;TBRANCH_DATA->SetBranchAddress("pmy", &pmy_data);
  Double_t pmz_data;TBRANCH_DATA->SetBranchAddress("pmz", &pmz_data);
  Double_t em_data;TBRANCH_DATA->SetBranchAddress("emiss", &em_data);
  Double_t pm_data;TBRANCH_DATA->SetBranchAddress("pmiss", &pm_data);
  Double_t MMp_data;TBRANCH_DATA->SetBranchAddress("MMp", &MMp_data);

  Double_t  P_hod_goodstarttime_data;TBRANCH_DATA->SetBranchAddress("P_hod_goodstarttime", &P_hod_goodstarttime_data);   
  Double_t  P_dc_InsideDipoleExit_data;TBRANCH_DATA->SetBranchAddress("P_dc_InsideDipoleExit", &P_dc_InsideDipoleExit_data);
  Double_t  P_gtr_xptar_data;TBRANCH_DATA->SetBranchAddress("P_gtr_xptar", &P_gtr_xptar_data);   
  Double_t  P_gtr_dp_data;TBRANCH_DATA->SetBranchAddress("P_gtr_dp", &P_gtr_dp_data); 
  Double_t  P_gtr_yptar_data;TBRANCH_DATA->SetBranchAddress("P_gtr_yptar", &P_gtr_yptar_data);     
  Double_t  P_cal_etottracknorm_data;TBRANCH_DATA->SetBranchAddress("P_cal_etottracknorm", &P_cal_etottracknorm_data);   

  Double_t  H_hod_goodscinhit_data;TBRANCH_DATA->SetBranchAddress("H_hod_goodscinhit", &H_hod_goodscinhit_data);   
  Double_t  H_hod_goodstarttime_data;TBRANCH_DATA->SetBranchAddress("H_hod_goodstarttime", &H_hod_goodstarttime_data);   
  Double_t  H_dc_InsideDipoleExit_data;TBRANCH_DATA->SetBranchAddress("H_dc_InsideDipoleExit", &H_dc_InsideDipoleExit_data);
  Double_t  H_gtr_dp_data;TBRANCH_DATA->SetBranchAddress("H_gtr_dp", &H_gtr_dp_data);   
  Double_t  H_gtr_xptar_data;TBRANCH_DATA->SetBranchAddress("H_gtr_xptar", &H_gtr_xptar_data);   
  Double_t  H_gtr_yptar_data;TBRANCH_DATA->SetBranchAddress("H_gtr_yptar", &H_gtr_yptar_data);   
  Double_t  H_cer_npeSum_data;TBRANCH_DATA->SetBranchAddress("H_cer_npeSum", &H_cer_npeSum_data);
  Double_t  H_cal_etotnorm_data;TBRANCH_DATA->SetBranchAddress("H_cal_etotnorm", &H_cal_etotnorm_data);   

  //DUMMY variables
  //HMS
  Double_t hsdelta_dummy;TBRANCH_DUMMY->SetBranchAddress("hsdelta", &hsdelta_dummy);
  Double_t hsxptar_dummy;TBRANCH_DUMMY->SetBranchAddress("hsxptar", &hsxptar_dummy);
  Double_t hsyptar_dummy;TBRANCH_DUMMY->SetBranchAddress("hsyptar", &hsyptar_dummy);
  Double_t hsxfp_dummy;TBRANCH_DUMMY->SetBranchAddress("hsxfp", &hsxfp_dummy);
  Double_t hsyfp_dummy;TBRANCH_DUMMY->SetBranchAddress("hsyfp", &hsyfp_dummy);
  Double_t hsxpfp_dummy;TBRANCH_DUMMY->SetBranchAddress("hsxpfp", &hsxpfp_dummy);
  Double_t hsypfp_dummy;TBRANCH_DUMMY->SetBranchAddress("hsypfp", &hsypfp_dummy);
  //SHMS
  Double_t ssdelta_dummy;TBRANCH_DUMMY->SetBranchAddress("ssdelta", &ssdelta_dummy);
  Double_t ssxptar_dummy;TBRANCH_DUMMY->SetBranchAddress("ssxptar", &ssxptar_dummy);
  Double_t ssyptar_dummy;TBRANCH_DUMMY->SetBranchAddress("ssyptar", &ssyptar_dummy);
  Double_t ssxfp_dummy;TBRANCH_DUMMY->SetBranchAddress("ssxfp", &ssxfp_dummy);
  Double_t ssyfp_dummy;TBRANCH_DUMMY->SetBranchAddress("ssyfp", &ssyfp_dummy);
  Double_t ssxpfp_dummy;TBRANCH_DUMMY->SetBranchAddress("ssxpfp", &ssxpfp_dummy);
  Double_t ssypfp_dummy;TBRANCH_DUMMY->SetBranchAddress("ssypfp", &ssypfp_dummy);
 
  Double_t Q2_dummy;TBRANCH_DUMMY->SetBranchAddress("Q2", &Q2_dummy);
  Double_t W_dummy;TBRANCH_DUMMY->SetBranchAddress("W", &W_dummy);
  Double_t epsilon_dummy;TBRANCH_DUMMY->SetBranchAddress("epsilon", &epsilon_dummy);
  Double_t pmx_dummy;TBRANCH_DUMMY->SetBranchAddress("pmx", &pmx_dummy);
  Double_t pmy_dummy;TBRANCH_DUMMY->SetBranchAddress("pmy", &pmy_dummy);
  Double_t pmz_dummy;TBRANCH_DUMMY->SetBranchAddress("pmz", &pmz_dummy);
  Double_t em_dummy;TBRANCH_DUMMY->SetBranchAddress("emiss", &em_dummy);
  Double_t pm_dummy;TBRANCH_DUMMY->SetBranchAddress("pmiss", &pm_dummy);
  Double_t MMp_dummy;TBRANCH_DUMMY->SetBranchAddress("MMp", &MMp_dummy);

  Double_t  P_hod_goodstarttime_dummy;TBRANCH_DUMMY->SetBranchAddress("P_hod_goodstarttime", &P_hod_goodstarttime_dummy);   
  Double_t  P_dc_InsideDipoleExit_dummy;TBRANCH_DUMMY->SetBranchAddress("P_dc_InsideDipoleExit", &P_dc_InsideDipoleExit_dummy);
  Double_t  P_gtr_xptar_dummy;TBRANCH_DUMMY->SetBranchAddress("P_gtr_xptar", &P_gtr_xptar_dummy);   
  Double_t  P_gtr_dp_dummy;TBRANCH_DUMMY->SetBranchAddress("P_gtr_dp", &P_gtr_dp_dummy); 
  Double_t  P_gtr_yptar_dummy;TBRANCH_DUMMY->SetBranchAddress("P_gtr_yptar", &P_gtr_yptar_dummy);     
  Double_t  P_cal_etottracknorm_dummy;TBRANCH_DUMMY->SetBranchAddress("P_cal_etottracknorm", &P_cal_etottracknorm_dummy);   

  Double_t  H_hod_goodscinhit_dummy;TBRANCH_DUMMY->SetBranchAddress("H_hod_goodscinhit", &H_hod_goodscinhit_dummy);   
  Double_t  H_hod_goodstarttime_dummy;TBRANCH_DUMMY->SetBranchAddress("H_hod_goodstarttime", &H_hod_goodstarttime_dummy);   
  Double_t  H_dc_InsideDipoleExit_dummy;TBRANCH_DUMMY->SetBranchAddress("H_dc_InsideDipoleExit", &H_dc_InsideDipoleExit_dummy);
  Double_t  H_gtr_dp_dummy;TBRANCH_DUMMY->SetBranchAddress("H_gtr_dp", &H_gtr_dp_dummy);   
  Double_t  H_gtr_xptar_dummy;TBRANCH_DUMMY->SetBranchAddress("H_gtr_xptar", &H_gtr_xptar_dummy);   
  Double_t  H_gtr_yptar_dummy;TBRANCH_DUMMY->SetBranchAddress("H_gtr_yptar", &H_gtr_yptar_dummy);   
  Double_t  H_cer_npeSum_dummy;TBRANCH_DUMMY->SetBranchAddress("H_cer_npeSum", &H_cer_npeSum_dummy);
  Double_t  H_cal_etotnorm_dummy;TBRANCH_DUMMY->SetBranchAddress("H_cal_etotnorm", &H_cal_etotnorm_dummy);   

  //##############################################################################
  
  TH1D *H_hsdelta_DATA  = new TH1D("H_hsdelta_DATA","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsdelta_DUMMY  = new TH1D("H_hsdelta_DUMMY","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsdelta_SIMC  = new TH1D("H_hsdelta_SIMC","HMS Delta; hsdelta;", 300, -20.0, 20.0);

  TH1D *H_hsxptar_DATA  = new TH1D("H_hsxptar_DATA","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsxptar_DUMMY  = new TH1D("H_hsxptar_DUMMY","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsxptar_SIMC  = new TH1D("H_hsxptar_SIMC","HMS xptar; hsxptar;", 300, -0.1, 0.1);

  TH1D *H_hsyptar_DATA  = new TH1D("H_hsyptar_DATA","HMS yptar; hsyptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_DUMMY  = new TH1D("H_hsyptar_DUMMY","HMS yptar; hsyptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar_SIMC  = new TH1D("H_hsyptar_SIMC","HMS yptar; hsyptar;", 300, -0.1, 0.1);

  TH1D *H_ssxfp_DATA    = new TH1D("H_ssxfp_DATA","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1D *H_ssxfp_DUMMY    = new TH1D("H_ssxfp_DUMMY","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1D *H_ssxfp_SIMC    = new TH1D("H_ssxfp_SIMC","SHMS xfp; ssxfp;", 300, -20.0, 20.0);

  TH1D *H_ssyfp_DATA    = new TH1D("H_ssyfp_DATA","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1D *H_ssyfp_DUMMY    = new TH1D("H_ssyfp_DUMMY","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1D *H_ssyfp_SIMC    = new TH1D("H_ssyfp_SIMC","SHMS yfp; ssyfp;", 300, -20.0, 20.0);

  TH1D *H_ssxpfp_DATA   = new TH1D("H_ssxpfp_DATA","SHMS xpfp; ssxpfp;", 300, -0.09, 0.09);
  TH1D *H_ssxpfp_DUMMY   = new TH1D("H_ssxpfp_DUMMY","SHMS xpfp; ssxpfp;", 300, -0.09, 0.09);
  TH1D *H_ssxpfp_SIMC   = new TH1D("H_ssxpfp_SIMC","SHMS xpfp; ssxpfp;", 300, -0.09, 0.09);

  TH1D *H_ssypfp_DATA   = new TH1D("H_ssypfp_DATA","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1D *H_ssypfp_DUMMY   = new TH1D("H_ssypfp_DUMMY","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1D *H_ssypfp_SIMC   = new TH1D("H_ssypfp_SIMC","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);

  TH1D *H_hsxfp_DATA    = new TH1D("H_hsxfp_DATA","HMS xfp; hsxfp;", 300, -40.0, 40.0);
  TH1D *H_hsxfp_DUMMY    = new TH1D("H_hsxfp_DUMMY","HMS xfp; hsxfp;", 300, -40.0, 40.0);
  TH1D *H_hsxfp_SIMC    = new TH1D("H_hsxfp_SIMC","HMS xfp; hsxfp;", 300, -40.0, 40.0);

  TH1D *H_hsyfp_DATA    = new TH1D("H_hsyfp_DATA","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1D *H_hsyfp_DUMMY    = new TH1D("H_hsyfp_DUMMY","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1D *H_hsyfp_SIMC    = new TH1D("H_hsyfp_SIMC","HMS yfp; hsyfp;", 300, -20.0, 20.0);

  TH1D *H_hsxpfp_DATA   = new TH1D("H_hsxpfp_DATA","HMS xpfp; hsxpfp;", 300, -0.09, 0.05);
  TH1D *H_hsxpfp_DUMMY   = new TH1D("H_hsxpfp_DUMMY","HMS xpfp; hsxpfp;", 300, -0.09, 0.05);
  TH1D *H_hsxpfp_SIMC   = new TH1D("H_hsxpfp_SIMC","HMS xpfp; hsxpfp;", 300, -0.09, 0.05);
 
  TH1D *H_hsypfp_DATA   = new TH1D("H_hsypfp_DATA","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1D *H_hsypfp_DUMMY   = new TH1D("H_hsypfp_DUMMY","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1D *H_hsypfp_SIMC   = new TH1D("H_hsypfp_SIMC","HMS ypfp; hsypfp;", 300, -0.05, 0.04);

  TH1D *H_ssdelta_DATA  = new TH1D("H_ssdelta_DATA","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssdelta_DUMMY  = new TH1D("H_ssdelta_DUMMY","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssdelta_SIMC  = new TH1D("H_ssdelta_SIMC","SHMS delta; ssdelta;", 300, -20.0, 20.0);

  TH1D *H_ssxptar_DATA  = new TH1D("H_ssxptar_DATA","SHMS xptar; ssxptar;", 300, -0.1, 0.1);
  TH1D *H_ssxptar_DUMMY  = new TH1D("H_ssxptar_DUMMY","SHMS xptar; ssxptar;", 300, -0.1, 0.1);
  TH1D *H_ssxptar_SIMC  = new TH1D("H_ssxptar_SIMC","SHMS xptar; ssxptar;", 300, -0.1, 0.1);

  TH1D *H_ssyptar_DATA  = new TH1D("H_ssyptar_DATA","SHMS yptar; ssyptar;", 300, -0.04, 0.04);
  TH1D *H_ssyptar_DUMMY  = new TH1D("H_ssyptar_DUMMY","SHMS yptar; ssyptar;", 300, -0.04, 0.04);
  TH1D *H_ssyptar_SIMC  = new TH1D("H_ssyptar_SIMC","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1D *H_q_DATA        = new TH1D("H_q_DATA","q; q;", 300, 5.0, 7.0);
  TH1D *H_q_DUMMY        = new TH1D("H_q_DUMMY","q; q;", 300, 5.0, 7.0);
  TH1D *H_q_SIMC        = new TH1D("H_q_SIMC","q; q;", 300, 5.0, 7.0);

  TH1D *H_Q2_DATA       = new TH1D("H_Q2_DATA","Q2; Q2;", 300, 1.5, 8.0);  
  TH1D *H_Q2_DUMMY       = new TH1D("H_Q2_DUMMY","Q2; Q2;", 300, 1.5, 8.0);  
  TH1D *H_Q2_SIMC       = new TH1D("H_Q2_SIMC","Q2; Q2;", 300, 1.5, 8.0);  

  TH1D *H_epsilon_DATA  = new TH1D("H_epsilon_DATA","epsilon; epsilon;", 300, 0.5, 1.0);
  TH1D *H_epsilon_DUMMY  = new TH1D("H_epsilon_DUMMY","epsilon; epsilon;", 300, 0.5, 1.0);
  TH1D *H_epsilon_SIMC  = new TH1D("H_epsilon_SIMC","epsilon; epsilon;", 300, 0.5, 1.0);

  TH1D *H_MMp_DATA  = new TH1D("H_MMp_DATA","MMp ; MMp;", 300, -0.005, 0.005);
  TH1D *H_MMp_DUMMY  = new TH1D("H_MMp_DUMMY","MMp ; MMp;", 300, -0.005, 0.005);
  TH1D *H_MMp_SIMC  = new TH1D("H_MMp_SIMC","MMp ; MMp;", 300, -0.005, 0.005);
 
  TH1D *H_th_DATA  = new TH1D("H_th_DATA","X' tar; P_gtr_xp;", 300, -0.1, 0.1);
  TH1D *H_th_DUMMY  = new TH1D("H_th_DUMMY","X' tar; P_gtr_xp;", 300, -0.1, 0.1);
  TH1D *H_th_SIMC  = new TH1D("H_th_SIMC","H_th_simc; ssxptar;", 300, -0.1, 0.1);

  TH1D *H_ph_DATA  = new TH1D("H_ph_DATA","Y' tar; P_gtr_yp;", 300, -0.1, 0.1);
  TH1D *H_ph_DUMMY  = new TH1D("H_ph_DUMMY","Y' tar; P_gtr_yp;", 300, -0.1, 0.1);
  TH1D *H_ph_SIMC  = new TH1D("H_ph_SIMC","H_ph_simc; ssyptar;", 300, -0.1, 0.1);

  TH1D *H_pmiss_DATA  = new TH1D("H_pmiss_DATA","pmiss; pm;", 300, -0.1, 0.4);
  TH1D *H_pmiss_DUMMY  = new TH1D("H_pmiss_DUMMY","pmiss; pm;", 300, -0.1, 0.4);
  TH1D *H_pmiss_SIMC  = new TH1D("H_pmiss_SIMC","pmiss; pm;", 300, -0.1, 0.4);

  TH1D *H_emiss_DATA  = new TH1D("H_emiss_DATA","emiss; emiss;", 300, -0.1, 0.4);
  TH1D *H_emiss_DUMMY  = new TH1D("H_emiss_DUMMY","emiss; emiss;", 300, -0.1, 0.4);
  TH1D *H_emiss_SIMC  = new TH1D("H_emiss_SIMC","emiss; emiss;", 300, -0.1, 0.4);
 
  TH1D *H_pmx_DATA  = new TH1D("H_pmx_DATA","pmx; pmx;", 300, -0.2, 0.2);
  TH1D *H_pmx_DUMMY  = new TH1D("H_pmx_DUMMY","pmx; pmx;", 300, -0.2, 0.2);
  TH1D *H_pmx_SIMC  = new TH1D("H_pmx_SIMC","pmx; pmx;", 300, -0.2, 0.2);

  TH1D *H_pmy_DATA  = new TH1D("H_pmy_DATA","pmy ; pmy;", 300, -0.2, 0.2);
  TH1D *H_pmy_DUMMY  = new TH1D("H_pmy_DUMMY","pmy ; pmy;", 300, -0.2, 0.2);
  TH1D *H_pmy_SIMC  = new TH1D("H_pmy_SIMC","pmy; pmy;", 300, -0.2, 0.2);

  TH1D *H_pmz_DATA  = new TH1D("H_pmz_DATA","pmz; pmz;", 300, -0.2, 0.2);
  TH1D *H_pmz_DUMMY  = new TH1D("H_pmz_DUMMY","pmz; pmz;", 300, -0.2, 0.2);
  TH1D *H_pmz_SIMC  = new TH1D("H_pmz_SIMC","pmz; pmz;", 300, -0.2, 0.2);

  TH1D *H_W_DATA  = new TH1D("H_W_DATA","W ; W;", 300, 0.5, 1.5);
  TH1D *H_W_DUMMY  = new TH1D("H_W_DUMMY","W ; W;", 300, 0.5, 1.5);
  TH1D *H_W_SIMC  = new TH1D("H_W_SIMC","W; W;", 300, 0.5, 1.5);
  
  for(Long64_t i = 0; i < nEntries_TBRANCH_SIMC; i++)
    {
      TBRANCH_SIMC->GetEntry(i);
      // Define the acceptance cuts  
      
      Double_t CUT1;           //HMS Delta
      Double_t CUT2;          // HMS xptar
      Double_t CUT3;         //  HMS yptar
      Double_t CUT4;        // SHMS Delta
      Double_t CUT5;       //  SHMS xptar
      Double_t CUT6;      //   SHMS yptar
      // Select the cuts
      //HMS
      CUT1 = hsdelta >=-8.0 && hsdelta <=8.0;
      CUT2 = hsxptar >=-0.08 && hsxpfp <=0.08;
      CUT3 = hsyptar >=-0.045 && hsypfp <=0.045;
  
      //SHMS    
      CUT4 = ssdelta >=-10.0 && ssdelta <=20.0;
      CUT5 = ssxptar >=-0.06 && ssxpfp <=0.06;
      CUT6 = ssyptar >=-0.04 && ssypfp <=0.04;
 
      //........................................
      
      //Fill SIMC events

      if(CUT1 && CUT2 && CUT3 && CUT4 && CUT5 && CUT6)
	{
	  H_ssxfp_SIMC->Fill(ssxfp, Weight);
	  H_ssyfp_SIMC->Fill(ssyfp, Weight);
	  H_ssxpfp_SIMC->Fill(ssxpfp, Weight);
	  H_ssypfp_SIMC->Fill(ssypfp, Weight);
	  H_hsxfp_SIMC->Fill(hsxfp, Weight);
	  H_hsyfp_SIMC->Fill(hsyfp, Weight);
	  H_hsxpfp_SIMC->Fill(hsxpfp, Weight);
	  H_hsypfp_SIMC->Fill(hsypfp, Weight);
	  H_ssdelta_SIMC->Fill(ssdelta, Weight); 
	  H_hsdelta_SIMC->Fill(hsdelta, Weight);	
	  H_ssxptar_SIMC->Fill(ssxptar, Weight);
	  H_ssyptar_SIMC->Fill(ssyptar, Weight);
	  H_hsxptar_SIMC->Fill(hsxptar, Weight);	
	  H_hsyptar_SIMC->Fill(hsyptar, Weight);	
	  H_pmiss_SIMC->Fill(Pm, Weight);	
	  H_emiss_SIMC->Fill(Em, Weight);	
	  H_pmx_SIMC->Fill(Pmx, Weight);
	  H_pmy_SIMC->Fill(Pmy, Weight);
	  H_pmz_SIMC->Fill(Pmz, Weight);
	  H_Q2_SIMC->Fill(Q2_simc, Weight);
	  H_W_SIMC->Fill(W_simc, Weight);
	  H_epsilon_SIMC->Fill(epsilon_simc, Weight);
	  H_MMp_SIMC->Fill((pow(Em, 2) - pow(Pm, 2)), Weight);  
	}
    }

  for(Long64_t i = 0; i < nEntries_TBRANCH_DATA; i++)
    {
      TBRANCH_DATA->GetEntry(i);

      //......... Define Cuts.................
      Double_t SHMS_FixCut;             
      Double_t SHMS_Acceptance;
      Double_t epcointime;             
      Double_t PION_PID;                   
      Double_t KAON_PID;            
      Double_t SHMS_ELECTRON_PID;

      Double_t HMS_FixCut;             
      Double_t HMS_Acceptance;
      Double_t HMS_ELECTRON_PID;

      //CUTs Definations 
      SHMS_FixCut = P_hod_goodstarttime_data == 1&&P_dc_InsideDipoleExit_data == 1; // && P_hod_betanotrack_data > 0.5 && P_hod_betanotrack_data < 1.4;
      SHMS_Acceptance = P_gtr_dp_data>=-10.0 && P_gtr_dp_data<=20.0&&P_gtr_xptar_data>=-0.06&&P_gtr_xptar_data<=0.06&&P_gtr_yptar_data>=-0.04&&P_gtr_yptar_data<=0.04;
      SHMS_ELECTRON_PID = P_cal_etottracknorm_data >= 0.85 && P_cal_etottracknorm_data <= 1.2; // P_hgcer_npeSum_data >=0.5 && P_aero_npeSum_data >=0.5;

      HMS_FixCut = H_hod_goodstarttime_data == 1&&H_dc_InsideDipoleExit_data == 1; // H_hod_goodscinhit_data == 1
      HMS_Acceptance = H_gtr_dp_data>=-8.0 && H_gtr_dp_data<=8.0&&H_gtr_xptar_data>=-0.08&&H_gtr_xptar_data<=0.08&&H_gtr_yptar_data>=-0.045&&H_gtr_yptar_data<=0.045;       
      HMS_ELECTRON_PID = H_cer_npeSum_data >=0.5 && H_cal_etotnorm_data >=0.8 && H_cal_etotnorm_data <=1.2;
 
      //........................................

      //if(SHMS_FixCut && SHMS_Acceptance && SHMS_ELECTRON_PID) 
      if(SHMS_FixCut && SHMS_Acceptance && HMS_FixCut && HMS_Acceptance) 
	{
	  H_ssxfp_DATA->Fill(ssxfp_data);
	  H_ssyfp_DATA->Fill(ssyfp_data);
	  H_ssxpfp_DATA->Fill(ssxpfp_data);
	  H_ssypfp_DATA->Fill(ssypfp_data);
	  H_ssdelta_DATA->Fill(ssdelta_data);
	  H_ssxptar_DATA->Fill(ssxptar_data);
	  H_ssyptar_DATA->Fill(ssyptar_data);

      //if(HMS_FixCut && HMS_Acceptance && HMS_ELECTRON_PID)
      //if(HMS_FixCut && HMS_Acceptance)

	  H_pmiss_DATA->Fill(pm_data);	
	  H_emiss_DATA->Fill(em_data);	
	  H_pmx_DATA->Fill(pmx_data);
	  H_pmy_DATA->Fill(pmy_data);
	  H_pmz_DATA->Fill(pmz_data);
	  H_Q2_DATA->Fill(Q2_data);
	  H_W_DATA->Fill(W_data);
	  H_epsilon_DATA->Fill(epsilon_data);
	  H_MMp_DATA->Fill((pow(em_data, 2) - pow(pm_data, 2)));  
	  //	  H_MMp_DATA->Fill(MMp_data);  

	  H_hsxfp_DATA->Fill(hsxfp_data);
	  H_hsyfp_DATA->Fill(hsyfp_data);
	  H_hsxpfp_DATA->Fill(hsxpfp_data);
	  H_hsypfp_DATA->Fill(hsypfp_data);
	  H_hsdelta_DATA->Fill(hsdelta_data);
	  H_hsxptar_DATA->Fill(hsxptar_data);	
	  H_hsyptar_DATA->Fill(hsyptar_data);
	}

    }

  for(Long64_t i = 0; i < nEntries_TBRANCH_DUMMY; i++)
    {
      TBRANCH_DUMMY->GetEntry(i);

      //......... Define Cuts.................
      Double_t SHMS_FixCut;             
      Double_t SHMS_Acceptance;
      Double_t epcointime;             
      Double_t PION_PID;                   
      Double_t KAON_PID;            
      Double_t SHMS_ELECTRON_PID;

      Double_t HMS_FixCut;             
      Double_t HMS_Acceptance;
      Double_t HMS_ELECTRON_PID;

      //CUTs Definations 
      SHMS_FixCut = P_hod_goodstarttime_dummy == 1&&P_dc_InsideDipoleExit_dummy == 1; // && P_hod_betanotrack_dummy > 0.5 && P_hod_betanotrack_dummy < 1.4;
      SHMS_Acceptance = P_gtr_dp_dummy>=-10.0 && P_gtr_dp_dummy<=20.0&&P_gtr_xptar_dummy>=-0.06&&P_gtr_xptar_dummy<=0.06&&P_gtr_yptar_dummy>=-0.04&&P_gtr_yptar_dummy<=0.04;
      SHMS_ELECTRON_PID = P_cal_etottracknorm_dummy >= 0.85 && P_cal_etottracknorm_dummy <= 1.2; // P_hgcer_npeSum_dummy >=0.5 && P_aero_npeSum_dummy >=0.5;

      HMS_FixCut = H_hod_goodstarttime_dummy == 1&&H_dc_InsideDipoleExit_dummy == 1; // H_hod_goodscinhit_dummy == 1
      HMS_Acceptance = H_gtr_dp_dummy>=-8.0 && H_gtr_dp_dummy<=8.0&&H_gtr_xptar_dummy>=-0.08&&H_gtr_xptar_dummy<=0.08&&H_gtr_yptar_dummy>=-0.045&&H_gtr_yptar_dummy<=0.045;       
      HMS_ELECTRON_PID = H_cer_npeSum_dummy >=0.5 && H_cal_etotnorm_dummy >=0.8 && H_cal_etotnorm_dummy <=1.2;
 
      //........................................

      //if(SHMS_FixCut && SHMS_Acceptance && SHMS_ELECTRON_PID) 
      if(SHMS_FixCut && SHMS_Acceptance && HMS_FixCut && HMS_Acceptance) 
	{
	  H_ssxfp_DUMMY->Fill(ssxfp_dummy);
	  H_ssyfp_DUMMY->Fill(ssyfp_dummy);
	  H_ssxpfp_DUMMY->Fill(ssxpfp_dummy);
	  H_ssypfp_DUMMY->Fill(ssypfp_dummy);
	  H_ssdelta_DUMMY->Fill(ssdelta_dummy);
	  H_ssxptar_DUMMY->Fill(ssxptar_dummy);
	  H_ssyptar_DUMMY->Fill(ssyptar_dummy);

      //if(HMS_FixCut && HMS_Acceptance && HMS_ELECTRON_PID)
      //if(HMS_FixCut && HMS_Acceptance)

	  H_pmiss_DUMMY->Fill(pm_dummy);	
	  H_emiss_DUMMY->Fill(em_dummy);	
	  H_pmx_DUMMY->Fill(pmx_dummy);
	  H_pmy_DUMMY->Fill(pmy_dummy);
	  H_pmz_DUMMY->Fill(pmz_dummy);
	  H_Q2_DUMMY->Fill(Q2_dummy);
	  H_W_DUMMY->Fill(W_dummy);
	  H_epsilon_DUMMY->Fill(epsilon_dummy);
	  H_MMp_DUMMY->Fill((pow(em_dummy, 2) - pow(pm_dummy, 2)));  
	  //H_MMp_DUMMY->Fill(MMp_dummy);  

	  H_hsxfp_DUMMY->Fill(hsxfp_dummy);
	  H_hsyfp_DUMMY->Fill(hsyfp_dummy);
	  H_hsxpfp_DUMMY->Fill(hsxpfp_dummy);
	  H_hsypfp_DUMMY->Fill(hsypfp_dummy);
	  H_hsdelta_DUMMY->Fill(hsdelta_dummy);
	  H_hsxptar_DUMMY->Fill(hsxptar_dummy);	
	  H_hsyptar_DUMMY->Fill(hsyptar_dummy);
	}

    }

  //  Double_t simc_wgt = 0.131105E-04;
  Double_t simc_normfactor = 0.832520E+07;
  Double_t simc_nevents = 200000;
  Double_t normfac_simc = (simc_normfactor)/(simc_nevents);

  H_ssxfp_SIMC->Scale(normfac_simc);                                                                                                                                   
  H_ssyfp_SIMC->Scale(normfac_simc);                                                                                                                                  
  H_ssxpfp_SIMC->Scale(normfac_simc);                                                                                                                              
  H_ssypfp_SIMC->Scale(normfac_simc);                                                                                                                                      
  H_hsxfp_SIMC->Scale(normfac_simc);                                                                                                                                              
  H_hsyfp_SIMC->Scale(normfac_simc);                                                                                                                                               
  H_hsxpfp_SIMC->Scale(normfac_simc);                                                                                                                                                                    
  H_hsypfp_SIMC->Scale(normfac_simc);                                                                                                                                                                    
  H_ssdelta_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_hsdelta_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_ssxptar_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_ssyptar_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_hsxptar_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_hsyptar_SIMC->Scale(normfac_simc);                                                                                                                                                                  
  H_pmiss_SIMC->Scale(normfac_simc);                                                                                                                                        
  H_emiss_SIMC->Scale(normfac_simc);                                                                                                                                            
  H_pmx_SIMC->Scale(normfac_simc);                                                                                                                                                
  H_pmy_SIMC->Scale(normfac_simc);                                                                                                                                                
  H_pmz_SIMC->Scale(normfac_simc);                                                                                                                                                
  H_Q2_SIMC->Scale(normfac_simc);                                                                                                                                                 
  H_W_SIMC->Scale(normfac_simc);                                                                                                                                                         
  H_epsilon_SIMC->Scale(normfac_simc);                                                                                                                                                    
  H_MMp_SIMC->Scale(normfac_simc);

  Double_t dummy_charge = 42.096;
  Double_t dummy_target_corr = 4.8579;
  Double_t normfac_dummy = 1/(dummy_charge*dummy_target_corr);

  H_ssxfp_DUMMY->Scale(normfac_dummy);
  H_ssyfp_DUMMY->Scale(normfac_dummy);
  H_ssxpfp_DUMMY->Scale(normfac_dummy);
  H_ssypfp_DUMMY->Scale(normfac_dummy);
  H_hsxfp_DUMMY->Scale(normfac_dummy);
  H_hsyfp_DUMMY->Scale(normfac_dummy);
  H_hsxpfp_DUMMY->Scale(normfac_dummy);
  H_hsypfp_DUMMY->Scale(normfac_dummy);
  H_ssxptar_DUMMY->Scale(normfac_dummy);
  H_ssyptar_DUMMY->Scale(normfac_dummy);
  H_hsxptar_DUMMY->Scale(normfac_dummy);
  H_hsyptar_DUMMY->Scale(normfac_dummy);
  H_ssdelta_DUMMY->Scale(normfac_dummy);
  H_hsdelta_DUMMY->Scale(normfac_dummy);
  H_Q2_DUMMY->Scale(normfac_dummy);
  H_epsilon_DUMMY->Scale(normfac_dummy);
  H_MMp_DUMMY->Scale(normfac_dummy);
  H_pmiss_DUMMY->Scale(normfac_dummy);
  H_emiss_DUMMY->Scale(normfac_dummy);
  H_pmx_DUMMY->Scale(normfac_dummy);
  H_pmy_DUMMY->Scale(normfac_dummy);
  H_pmz_DUMMY->Scale(normfac_dummy);
  H_W_DUMMY->Scale(normfac_dummy);

  Double_t data_charge = 542.499;
  Double_t normfac_data = 1/(data_charge);

  H_ssxfp_DATA->Scale(normfac_data);
  H_ssyfp_DATA->Scale(normfac_data);
  H_ssxpfp_DATA->Scale(normfac_data);
  H_ssypfp_DATA->Scale(normfac_data);
  H_hsxfp_DATA->Scale(normfac_data);
  H_hsyfp_DATA->Scale(normfac_data);
  H_hsxpfp_DATA->Scale(normfac_data);
  H_hsypfp_DATA->Scale(normfac_data);
  H_ssxptar_DATA->Scale(normfac_data);
  H_ssyptar_DATA->Scale(normfac_data);
  H_hsxptar_DATA->Scale(normfac_data);
  H_hsyptar_DATA->Scale(normfac_data);
  H_ssdelta_DATA->Scale(normfac_data);
  H_hsdelta_DATA->Scale(normfac_data);
  H_Q2_DATA->Scale(normfac_data);
  H_epsilon_DATA->Scale(normfac_data);
  H_MMp_DATA->Scale(normfac_data);
  H_pmiss_DATA->Scale(normfac_data);
  H_emiss_DATA->Scale(normfac_data);
  H_pmx_DATA->Scale(normfac_data);
  H_pmy_DATA->Scale(normfac_data);
  H_pmz_DATA->Scale(normfac_data);
  H_W_DATA->Scale(normfac_data);

  // Dummy Subtraction
  H_ssxfp_DATA->Add(H_ssxfp_DUMMY,-1);
  H_ssyfp_DATA->Add(H_ssyfp_DUMMY,-1);
  H_ssxpfp_DATA->Add(H_ssxpfp_DUMMY,-1);
  H_ssypfp_DATA->Add(H_ssypfp_DUMMY,-1);
  H_hsxfp_DATA->Add(H_hsxfp_DUMMY,-1);
  H_hsyfp_DATA->Add(H_hsyfp_DUMMY,-1);
  H_hsxpfp_DATA->Add(H_hsxpfp_DUMMY,-1);
  H_hsypfp_DATA->Add(H_hsypfp_DUMMY,-1);
  H_ssxptar_DATA->Add(H_ssxptar_DUMMY,-1);
  H_ssyptar_DATA->Add(H_ssyptar_DUMMY,-1);
  H_hsxptar_DATA->Add(H_hsxptar_DUMMY,-1);
  H_hsyptar_DATA->Add(H_hsyptar_DUMMY,-1);
  H_ssdelta_DATA->Add(H_ssdelta_DUMMY,-1);
  H_hsdelta_DATA->Add(H_hsdelta_DUMMY,-1);
  H_Q2_DATA->Add(H_Q2_DUMMY,-1);
  H_epsilon_DATA->Add(H_epsilon_DUMMY,-1);
  H_MMp_DATA->Add(H_MMp_DUMMY,-1);
  H_pmiss_DATA->Add(H_pmiss_DUMMY,-1);
  H_emiss_DATA->Add(H_emiss_DUMMY,-1);
  H_pmx_DATA->Add(H_pmx_DUMMY,-1);
  H_pmy_DATA->Add(H_pmy_DUMMY,-1);
  H_pmz_DATA->Add(H_pmz_DUMMY,-1);
  H_W_DATA->Add(H_W_DUMMY,-1);

  //...................................................................

  // PLOT HIST..

  TCanvas *xfp = new TCanvas("xfp", "SHMS xfp");
  H_ssxfp_DATA->SetLineColor(kRed);

  //  TH1D *H_ssxfp_DATA = (TH1D)H_ssxfp_DATA_clone->Clone(“H_ssxfp_DATA”);
  //  TH1D *H_ssxfp_DATA_clone    = new TH1D("H_ssxfp_DATA_clone","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  //  H_ssxfp_DATA_clone->Clone("H_ssxfp_DATA");
  H_ssxfp_DATA->Integral(H_ssxfp_DATA->FindBin(-20.0), H_ssxfp_DATA->FindBin(20.0), "Width");
  //  cout << H_ssxfp_DATA->Integral(H_ssxfp_DATA->FindBin(-20.0), H_ssxfp_DATA->FindBin(20.0)) << endl;
  //  cout << H_ssxfp_DATA->Integral(H_ssxfp_DATA->FindBin(-20.0), H_ssxfp_DATA->FindBin(20.0), "width") << endl;

			       //  TH1D *H_ssxfp_SIMC = (TH1D)H_ssxfp_SIMC_clone->Clone(“H_ssxfp_SIMC”);

  //  TH1D *H_ssxfp_SIMC_clone    = new TH1D("H_ssxfp_SIMC_clone","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  //  H_ssxfp_SIMC_clone->Clone("H_ssxfp_SIMC");
  H_ssxfp_SIMC->Integral(H_ssxfp_SIMC->FindBin(-20.0), H_ssxfp_SIMC->FindBin(20.0), "Width");

  H_ssxfp_SIMC->Draw("");
  H_ssxfp_DATA->Draw("same");

  xfp->Print(outputpdf + '(');

  TCanvas *yfp = new TCanvas("yfp", "SHMS yfp");
  H_ssyfp_DATA->SetLineColor(kRed);

  H_ssyfp_SIMC->Draw("");
  H_ssyfp_DATA->Draw("same");

  yfp->Print(outputpdf);

  TCanvas *xpfp = new TCanvas("xpfp", "SHMS xpfp");
  H_ssxpfp_DATA->SetLineColor(kRed);

  H_ssxpfp_SIMC->Draw("");
  H_ssxpfp_DATA->Draw("same");

  xpfp->Print(outputpdf);

  TCanvas *ypfp = new TCanvas("ypfp", "SHMS ypfp");
  H_ssypfp_DATA->SetLineColor(kRed);

  H_ssypfp_SIMC->Draw("");
  H_ssypfp_DATA->Draw("same");

  ypfp->Print(outputpdf);

  TCanvas *hxfp = new TCanvas("hxfp", "HMS xfp");
  H_hsxfp_DATA->SetLineColor(kRed);

  H_hsxfp_SIMC->Draw("");
  H_hsxfp_DATA->Draw("same");

  hxfp->Print(outputpdf);

  TCanvas *hyfp = new TCanvas("hyfp", "HMS yfp");
  H_hsyfp_DATA->SetLineColor(kRed);

  H_hsyfp_SIMC->Draw("");
  H_hsyfp_DATA->Draw("same");

  hyfp->Print(outputpdf);

  TCanvas *hxpfp = new TCanvas("hxpfp", "HMS xpfp");
  H_hsxpfp_DATA->SetLineColor(kRed);

  H_hsxpfp_SIMC->Draw("");
  H_hsxpfp_DATA->Draw("same");

  hxpfp->Print(outputpdf);

  TCanvas *hypfp = new TCanvas("hypfp", "HMS ypfp");
  H_hsypfp_DATA->SetLineColor(kRed);

  H_hsypfp_SIMC->Draw("");
  H_hsypfp_DATA->Draw("same");

  hypfp->Print(outputpdf);

  TCanvas *xptar = new TCanvas("xptar", "SHMS xptar");
  H_ssxptar_DATA->SetLineColor(kRed);

  H_ssxptar_SIMC->Draw("");
  H_ssxptar_DATA->Draw("same");

  xptar->Print(outputpdf);

  TCanvas *yptar = new TCanvas("yptar", "SHMS yptar");
  H_ssyptar_DATA->SetLineColor(kRed);

  H_ssyptar_SIMC->Draw("");
  H_ssyptar_DATA->Draw("same");

  yptar->Print(outputpdf);

  TCanvas *hxptar = new TCanvas("hxptar", "HMS xptar");
  H_hsxptar_DATA->SetLineColor(kRed);

  H_hsxptar_SIMC->Draw("");
  H_hsxptar_DATA->Draw("same");

  hxptar->Print(outputpdf);

  TCanvas *hyptar = new TCanvas("hyptar", "HMS yptar");
  H_hsyptar_DATA->SetLineColor(kRed);

  H_hsyptar_SIMC->Draw("");
  H_hsyptar_DATA->Draw("same");
 
  hyptar->Print(outputpdf);

  TCanvas *Delta = new TCanvas("Delta", "SHMS Delta");
  H_ssdelta_DATA->SetLineColor(kRed);

  H_ssdelta_SIMC->Draw("");
  H_ssdelta_DATA->Draw("same");

  Delta->Print(outputpdf);

  TCanvas *hDelta = new TCanvas("hDelta", "HMS Delta");
  H_hsdelta_DATA->SetLineColor(kRed);

  H_hsdelta_SIMC->Draw("");
  H_hsdelta_DATA->Draw("same");

  hDelta->Print(outputpdf);

  TCanvas *CQ2 = new TCanvas("CQ2", "SHMS Q2");
  H_Q2_DATA->SetLineColor(kRed);

  H_Q2_SIMC->Draw("");
  H_Q2_DATA->Draw("same");

  CQ2->Print(outputpdf);

  TCanvas *Cepsilon = new TCanvas("Cepsilon", "epsilon");
  H_epsilon_DATA->SetLineColor(kRed);

  H_epsilon_SIMC->Draw("");
  H_epsilon_DATA->Draw("same");

  Cepsilon->Print(outputpdf);

  TCanvas *CMMp = new TCanvas("CMMp", "Proton missing mass");
  H_MMp_DATA->SetLineColor(kRed);

  H_MMp_SIMC->Draw("");
  H_MMp_DATA->Draw("same");

  CMMp->Print(outputpdf);

  TCanvas *Cpmiss = new TCanvas("Cpmiss", "pmiss");
  H_pmiss_DATA->SetLineColor(kRed);

  H_pmiss_SIMC->Draw("");
  H_pmiss_DATA->Draw("same");

  Cpmiss->Print(outputpdf);

  TCanvas *Cemiss = new TCanvas("Cemiss", "emiss");
  H_emiss_DATA->SetLineColor(kRed);

  H_emiss_SIMC->Draw("");
  H_emiss_DATA->Draw("same");

  Cemiss->Print(outputpdf);

  TCanvas *Cpmiss_x = new TCanvas("Cpmiss_x", "pmiss_x");
  H_pmx_DATA->SetLineColor(kRed);

  H_pmx_SIMC->Draw("");
  H_pmx_DATA->Draw("same");
  Cpmiss_x->Print(outputpdf);

  TCanvas *Cpmiss_y = new TCanvas("Cpmiss_y", "pmiss_y");
  H_pmy_DATA->SetLineColor(kRed);

  H_pmy_SIMC->Draw("");
  H_pmy_DATA->Draw("same");

  Cpmiss_y->Print(outputpdf);

  TCanvas *Cpmiss_z = new TCanvas("Cpmiss_z", "pmiss_z");
  H_pmz_DATA->SetLineColor(kRed);

  H_pmz_SIMC->Draw("");
  H_pmz_DATA->Draw("same");

  Cpmiss_z->Print(outputpdf);

  TCanvas *CW = new TCanvas("CW", "W");
  H_W_DATA->SetLineColor(kRed);

  H_W_SIMC->Draw("");
  H_W_DATA->Draw("same");

  CW->Print(outputpdf + ')');
 
}
