// .... Created Date: May 5, 2023 ....
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

void Analysed(string RunNum = "")
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
    Analysispath = "/group/c-kaonlt/USERS/"+User+"/lt_analysis/LTsep/efficiency";
    // Output path for root file
    ROOTfilePath = "/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/ROOTfiles/Analysis/KaonLT/";
    OutPath = "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT";
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
  
  rootFile = ROOTfilePath+"/"+"Kaon_coin_replay_production_"+RunNum+"_-1.root";

  if (gSystem->AccessPathName(rootFile) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  TFile *InFile = new TFile(rootFile, "READ");
  TString TOutFilename;
  // Establish the names of our output files quickly
  TString foutname = OutPath+"/"+RunNum+ ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";

  //#################################################################### 

  TTree* TBRANCH  = (TTree*)InFile->Get("T");Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();   // InFile
  TBRANCH->SetMakeClass(1);
  
  //Timing Info
  Double_t  RFTime_SHMS_RFtimeDist ;TBRANCH->SetBranchAddress("RFTime.SHMS_RFtimeDist", &RFTime_SHMS_RFtimeDist);
  Double_t  CTime_ePiCoinTime_ROC1;TBRANCH->SetBranchAddress("CTime.ePiCoinTime_ROC1", &CTime_ePiCoinTime_ROC1);
  Double_t  CTime_eKCoinTime_ROC1;TBRANCH->SetBranchAddress("CTime.eKCoinTime_ROC1", &CTime_eKCoinTime_ROC1);
  Double_t  CTime_epCoinTime_ROC1;TBRANCH->SetBranchAddress("CTime.epCoinTime_ROC1", &CTime_epCoinTime_ROC1);
  Double_t  P_RF_tdcTime;TBRANCH->SetBranchAddress("T.coin.pRF_tdcTime", &P_RF_tdcTime);
  Double_t  P_hod_fpHitsTime;TBRANCH->SetBranchAddress("P.hod.fpHitsTime", &P_hod_fpHitsTime);
  // Double_t  H_RF_Dist;TBRANCH->SetBranchAddress("RFTime.HMS_RFtimeDist", &H_RF_Dist);
  //  Double_t  P_RF_Dist;TBRANCH->SetBranchAddress("RFTime.SHMS_RFtimeDist", &P_RF_Dist);
  Double_t  P_dc_InsideDipoleExit;TBRANCH->SetBranchAddress("P.dc.InsideDipoleExit", &P_dc_InsideDipoleExit);
  Double_t  P_hod_betanotrack;TBRANCH->SetBranchAddress("P.hod.betanotrack", &P_hod_betanotrack);
 
  //HMS Info 
  Double_t  H_hod_goodscinhit;TBRANCH->SetBranchAddress("H.hod.goodscinhit", &H_hod_goodscinhit);
  Double_t  H_hod_goodstarttime;TBRANCH->SetBranchAddress("H.hod.goodstarttime", &H_hod_goodstarttime);
  Double_t  H_dc_x_fp;TBRANCH->SetBranchAddress("H.dc.x_fp", &H_dc_x_fp);
  Double_t  H_dc_y_fp;TBRANCH->SetBranchAddress("H.dc.y_fp", &H_dc_y_fp);
  Double_t  H_dc_xp_fp;TBRANCH->SetBranchAddress("H.dc.xp_fp", &H_dc_xp_fp);
  Double_t  H_dc_yp_fp;TBRANCH->SetBranchAddress("H.dc.yp_fp", &H_dc_yp_fp);
  Double_t  H_gtr_beta;TBRANCH->SetBranchAddress("H.gtr.beta", &H_gtr_beta);
  Double_t  H_gtr_xptar;TBRANCH->SetBranchAddress("H.gtr.th", &H_gtr_xptar);
  Double_t  H_gtr_yptar;TBRANCH->SetBranchAddress("H.gtr.ph", &H_gtr_yptar);
  Double_t  H_gtr_p;TBRANCH->SetBranchAddress("H.gtr.p", &H_gtr_p);
  Double_t  H_gtr_dp;TBRANCH->SetBranchAddress("H.gtr.dp", &H_gtr_dp);
  Double_t  H_cal_etotnorm;TBRANCH->SetBranchAddress("H.cal.etotnorm", &H_cal_etotnorm);
  Double_t  H_cal_etottracknorm;TBRANCH->SetBranchAddress("H.cal.etottracknorm", &H_cal_etottracknorm);
  Double_t  H_cer_npeSum;TBRANCH->SetBranchAddress("H.cer.npeSum", &H_cer_npeSum);
  //SHMS Info
  Double_t  P_hod_goodscinhit;TBRANCH->SetBranchAddress("P.hod.goodscinhit", &P_hod_goodscinhit);
  Double_t  P_hod_goodstarttime;TBRANCH->SetBranchAddress("P.hod.goodstarttime", &P_hod_goodstarttime);
  Double_t  P_dc_x_fp;TBRANCH->SetBranchAddress("P.dc.x_fp", &P_dc_x_fp);
  Double_t  P_dc_y_fp;TBRANCH->SetBranchAddress("P.dc.y_fp", &P_dc_y_fp);
  Double_t  P_dc_xp_fp;TBRANCH->SetBranchAddress("P.dc.xp_fp", &P_dc_xp_fp);
  Double_t  P_dc_yp_fp;TBRANCH->SetBranchAddress("P.dc.yp_fp", &P_dc_yp_fp);
  Double_t  P_gtr_beta;TBRANCH->SetBranchAddress("P.gtr.beta", &P_gtr_beta);
  Double_t  P_gtr_xptar;TBRANCH->SetBranchAddress("P.gtr.th", &P_gtr_xptar);
  Double_t  P_gtr_yptar;TBRANCH->SetBranchAddress("P.gtr.ph", &P_gtr_yptar);
  Double_t  P_gtr_p;TBRANCH->SetBranchAddress("P.gtr.p", &P_gtr_p);
  Double_t  P_gtr_dp;TBRANCH->SetBranchAddress("P.gtr.dp", &P_gtr_dp);
  Double_t  P_cal_etotnorm;TBRANCH->SetBranchAddress("P.cal.etotnorm", &P_cal_etotnorm);
  Double_t  P_cal_etottracknorm;TBRANCH->SetBranchAddress("P.cal.etottracknorm", &P_cal_etottracknorm);
  Double_t  P_hgcer_npeSum;TBRANCH->SetBranchAddress("P.hgcer.npeSum", &P_hgcer_npeSum);
  Double_t  P_aero_npeSum;TBRANCH->SetBranchAddress("P.aero.npeSum", &P_aero_npeSum);

  // Kinematic quantitites 
  Double_t  Q2;TBRANCH->SetBranchAddress("H.kin.primary.Q2", &Q2);   
  Double_t  W;TBRANCH->SetBranchAddress("H.kin.primary.W", &W);   
  Double_t  epsilon;TBRANCH->SetBranchAddress("H.kin.primary.epsilon", &epsilon);   
  Double_t  ph_q;TBRANCH->SetBranchAddress("P.kin.secondary.ph_bq", &ph_q);   
  Double_t  emiss;TBRANCH->SetBranchAddress("P.kin.secondary.emiss", &emiss);   
  Double_t  pmiss;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss", &pmiss);   
  Double_t  MMpi;TBRANCH->SetBranchAddress("P.kin.secondary.MMpi", &MMpi);   
  Double_t  MMK;TBRANCH->SetBranchAddress("P.kin.secondary.MMK", &MMK);   
  Double_t  MMp;TBRANCH->SetBranchAddress("P.kin.secondary.MMp", &MMp);   
  Double_t  MandelT;TBRANCH->SetBranchAddress("P.kin.secondary.MandelT", &MandelT);   
  Double_t  pmiss_x;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_x", &pmiss_x);   
  Double_t  pmiss_y;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_y", &pmiss_y);   
  Double_t  pmiss_z;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_z", &pmiss_z);   


  //##############################################################################
  TH1D *H_H_cal    = new TH1D("H_H_cal","H_cal_etottracknorm; H_cal_etottracknorm;", 300, 0.0, 2.0);

  for(Long64_t i = 0; i < nEntries_TBRANCH; i++)
    {
      TBRANCH->GetEntry(i);
    
      Double_t FixCut;             
      Double_t SHMS_Acceptance;             
      Double_t HMS_Acceptance;
      Double_t epicointime; 
      Double_t epicointimeBack;            
      Double_t Pion_PID;            
      Double_t MMPiCUT;          
 
      FixCut = P_hod_goodstarttime == 1 && H_hod_goodstarttime == 1; 
      SHMS_Acceptance = P_gtr_dp > -10.0 && P_gtr_dp < 20.0 && P_gtr_xptar > -0.06 && P_gtr_xptar <=0.06 && P_gtr_yptar > -0.04 && P_gtr_yptar < 0.04;       
      HMS_Acceptance = H_gtr_dp > -8.0 && H_gtr_dp < 8.0 && H_gtr_xptar > -0.08 && H_gtr_xptar < 0.08 && H_gtr_yptar > -0.045 && H_gtr_yptar < 0.045;       
      epicointime =  CTime_ePiCoinTime_ROC1 >= -1.0 && CTime_ePiCoinTime_ROC1 <= 1;
            
      if (FixCut && SHMS_Acceptance && HMS_Acceptance && epicointime)   
	{
	  H_H_cal->Fill(H_cal_etottracknorm);
	}     
    }

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *hist = OutHisto_file->mkdir("hist");
  hist->cd();
  H_H_cal->Write();

  OutHisto_file->Close();
}
