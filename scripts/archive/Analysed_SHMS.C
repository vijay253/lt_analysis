// .... This script has created to analyse HMS Heep data....
// .... Created Date: Dec 5, 2021 ....
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

void Analysed_SHMS(string InDATAFilename = "", string OutFilename = "")
{
  TString Hostname = gSystem->HostName();
  TString User = (gSystem->GetUserInfo())->fUser;
  TString Replaypath;
  TString ROOTfilePath;
  TString OutPath;
  TString rootFile;

  gStyle->SetPalette(55);


  // Set paths depending on system you're running on
  if(Hostname.Contains("farm")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/scripts/HeepSingleStudy/SHMS_Heep/ROOTfiles";
    OutPath = Replaypath+"/UTIL_KAONLT/scripts/HeepSingleStudy/SHMS_Heep/OUTPUT";
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
  if(InDATAFilename == "") {
    cout << "Enter a DATA ROOT Filename to analyse: ";
    cin >> InDATAFilename;
  }  

  if(OutFilename == "") {
    cout << "Enter a Filename to output to: ";
    cin >> OutFilename;
  }
  
  TString TInDATAFilename = InDATAFilename ;
  rootFile = ROOTfilePath+"/"+TInDATAFilename;

  if (gSystem->AccessPathName(rootFile) == kTRUE){
    cerr << "!!!!! ERROR !!!!! " << endl <<rootFile <<  " not found" << endl <<  "!!!!! ERRROR !!!!!" << endl;
    exit;
  }

  TFile *InFile = new TFile(rootFile, "READ");
  TString TOutFilename = OutFilename;

  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
   
  //#################################################################### 

  TTree* TBRANCH  = (TTree*)InFile->Get("T");Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();   // InFile
  // SHMS variables

  Double_t  P_dc_InsideDipoleExit;TBRANCH->SetBranchAddress("P.dc.InsideDipoleExit", &P_dc_InsideDipoleExit);
  //  Double_t  P_hod_betanotrack;TBRANCH->SetBranchAddress("P_hod_betanotrack", &P_hod_betanotrack);

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
  Double_t  P_hgcer_xAtCer;TBRANCH->SetBranchAddress("P.hgcer.xAtCer", &P_hgcer_xAtCer);   
  Double_t  P_hgcer_yAtCer;TBRANCH->SetBranchAddress("P.hgcer.yAtCer", &P_hgcer_yAtCer);   
  Double_t  P_hgcer_npeSum;TBRANCH->SetBranchAddress("P.hgcer.npeSum", &P_hgcer_npeSum);   
  Double_t  P_aero_npeSum;TBRANCH->SetBranchAddress("P.aero.npeSum", &P_aero_npeSum);   
  Double_t  Q2;TBRANCH->SetBranchAddress("P.kin.Q2", &Q2);   
  Double_t  W;TBRANCH->SetBranchAddress("P.kin.W", &W);   
  Double_t  epsilon;TBRANCH->SetBranchAddress("P.kin.epsilon", &epsilon);   
  
  //##############################################################################
  // Define Histograms that need to compare data and SIMC

  TH1D *H_ssxfp    = new TH1D("H_ssxfp","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1D *H_ssyfp    = new TH1D("H_ssyfp","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1D *H_ssxpfp   = new TH1D("H_ssxpfp","SHMS xpfp; ssxpfp;", 300, -0.09, 0.05);
  TH1D *H_ssypfp   = new TH1D("H_ssypfp","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1D *H_ssdelta  = new TH1D("H_ssdelta","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssxptar  = new TH1D("H_ssxptar","SHMS xptar; ssxptar;", 300, -0.1, 0.1);
  TH1D *H_ssyptar  = new TH1D("H_ssyptar","SHMS yptar; ssyptar;", 300, -0.04, 0.04);
  TH1D *H_Q2       = new TH1D("H_Q2","Q2; Q2;", 300, 0.0, 4.0);                    
  TH1D *H_epsilon  = new TH1D("H_epsilon","epsilon; epsilon;", 300, 0.50, 0.90);       
  TH1D *H_W  = new TH1D("H_W","W ; W;", 300, 0.6, 1.2);
  TH1D *H_etottrnorm  = new TH1D("H_etottrnorm"," H_cal_etottracknorm ; H_cal_etottracknorm;", 300, 0.0, 2);
  TH1D *H_etotnorm  = new TH1D("H_etotnorm"," H_cal_etotnorm ; H_cal_etotnorm;", 300, 0.0, 2);
  TH1D *H_npeSum  = new TH1D("H_npeSum","H_hgcer_npeSum ; H_hgcer_npeSum;", 300, 0.0, 30);

  //for progress bar
  double progress=0.0;

  // Fill data events 
  cout << "\nApplying cuts and filling histograms...(will take a while)" << endl;
  for(Long64_t i = 0; i < nEntries_TBRANCH; i++)
    {

      // Progress bar
      if(i%1000==0) {	    
	int barWidth = 70;
	progress = ((double)i/(double)nEntries_TBRANCH);	    
	// cout<<i<<"/"<<NEvts<<endl;
	// cout << progress << endl;
	std::cout << "[";
	double pos = barWidth * progress;
	for (double i = 0.; i < barWidth; ++i) {
	  if (i < pos) std::cout << "=";
	  else if (i == pos) std::cout << ">";
	  else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0) << " %\r";
	std::cout.flush();
      }	 

      TBRANCH->GetEntry(i);
    
      //......... Define Cuts.................
      Double_t SHMS_FixCut;             
      Double_t SHMS_Acceptance;
      Double_t epcointime;             
      Double_t PION_PID;                   
      Double_t KAON_PID;            
      Double_t ELECTRON_PID;          
    
      //CUTs Definations 
      SHMS_FixCut = P_hod_goodstarttime == 1&&P_dc_InsideDipoleExit == 1; // && P_hod_betanotrack > 0.5 && P_hod_betanotrack < 1.4;
      SHMS_Acceptance = P_gtr_dp>=-10.0 && P_gtr_dp<=20.0&&P_gtr_xptar>=-0.06&&P_gtr_xptar<=0.06&&P_gtr_yptar>=-0.04&&P_gtr_yptar<=0.04;
      ELECTRON_PID = P_cal_etottracknorm >= 0.85 && P_cal_etottracknorm <= 1.2; // P_hgcer_npeSum >=0.5 && P_aero_npeSum >=0.5;

       
      if(SHMS_FixCut && SHMS_Acceptance && ELECTRON_PID && W <=1.0) 
	{
	  H_ssxfp->Fill(P_dc_x_fp);
	  H_ssyfp->Fill(P_dc_y_fp);
	  H_ssxpfp->Fill(P_dc_xp_fp);
	  H_ssypfp->Fill(P_dc_yp_fp);
	  H_ssxptar->Fill(P_gtr_xptar);
	  H_ssyptar->Fill(P_gtr_yptar);
	  H_ssdelta->Fill(P_gtr_dp);
	  H_Q2->Fill(Q2);
	  H_epsilon->Fill(epsilon);
	  H_W->Fill(W);
	  H_etottrnorm->Fill(P_cal_etottracknorm);
	  H_etotnorm->Fill(P_cal_etotnorm);
	  H_npeSum->Fill(P_hgcer_npeSum);
	}     
    }      
    
  // Write variables into root file
  
  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *hist = OutHisto_file->mkdir("hist");
  hist->cd();
  H_ssxfp->Write();
  H_ssyfp->Write();
  H_ssxpfp->Write();
  H_ssypfp->Write();
  H_ssxptar->Write();
  H_ssyptar->Write();
  H_ssdelta->Write();
  H_Q2->Write();
  H_epsilon->Write();
  H_W->Write();
  H_etottrnorm->Write();
  H_etotnorm->Write();
  H_npeSum->Write();
  OutHisto_file->Close();  

}
