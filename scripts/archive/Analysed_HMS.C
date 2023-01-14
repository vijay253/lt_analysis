// .... This script has created to analyse heep sinlge data ....
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

void Analysed_HMS(string InDATAFilename = "", string OutFilename = "")
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
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/scripts/HeepSingleStudy/HMS_Heep/ROOTfiles";
    OutPath = Replaypath+"/UTIL_KAONLT/scripts/HeepSingleStudy/HMS_Heep/OUTPUT";
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

  TTree* TBRANCH  = (TTree*)InFile->Get("T"); Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();     
  Double_t  H_dc_InsideDipoleExit;TBRANCH->SetBranchAddress("H.dc.InsideDipoleExit", &H_dc_InsideDipoleExit);
  //Double_t  H_hod_betanotrack;TBRANCH->SetBranchAddress("H_hod_betanotrack", &H_hod_betanotrack);

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
  Double_t  Q2;TBRANCH->SetBranchAddress("H.kin.Q2", &Q2);   
  Double_t  W;TBRANCH->SetBranchAddress("H.kin.W", &W);   
  Double_t  W2;TBRANCH->SetBranchAddress("H.kin.W2", &W2);   
  Double_t  epsilon;TBRANCH->SetBranchAddress("H.kin.epsilon", &epsilon);   
  
  //##############################################################################
  // Define Histograms that need to compare data and SIMC

  TH1D *H_hsxfp    = new TH1D("H_hsxfp","HMS xfp; hsxfp;", 300, -20.0, 20.0);
  TH1D *H_hsyfp    = new TH1D("H_hsyfp","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1D *H_hsxpfp   = new TH1D("H_hsxpfp","HMS xpfp; hsxpfp;", 300, -0.09, 0.05);
  TH1D *H_hsypfp   = new TH1D("H_hsypfp","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1D *H_hsdelta  = new TH1D("H_hsdelta","HMS delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsxptar  = new TH1D("H_hsxptar","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar  = new TH1D("H_hsyptar","HMS yptar; hsyptar;", 300, -0.04, 0.04);
  TH1D *H_Q2       = new TH1D("H_Q2","Q2; Q2;", 300, 0.0, 4.0);      
  TH1D *H_epsilon  = new TH1D("H_epsilon","epsilon; epsilon;", 300, 0.80, 1.0);        
  TH1D *H_W  = new TH1D("H_W","W ; W;", 300, 0.6, 1.2);
  TH1D *H_W2  = new TH1D("H_W2","W2 ; W2;", 300, 0.0, 1.5);
  TH1D *H_etottrnorm  = new TH1D("H_etottrnorm"," H_cal_etottracknorm ; H_cal_etottracknorm;", 300, 0.0, 2);
  TH1D *H_etotnorm  = new TH1D("H_etotnorm"," H_cal_etotnorm ; H_cal_etotnorm;", 300, 0.0, 2);
  TH1D *H_npeSum  = new TH1D("H_npeSum","H_cer_npeSum ; H_cer_npeSum;", 300, 0.0, 30);
  TH1D *h_gtr_p  = new TH1D("h_gtr_p"," h_gtr_p ; H_gtr_p;", 300, 0.0, 10);

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
      Double_t HMS_FixCut;             
      Double_t HMS_Acceptance;
      Double_t ELECTRON_PID;          
    
      //CUTs Definations 
      HMS_FixCut = H_hod_goodscinhit == 1 && H_hod_goodstarttime == 1&&H_dc_InsideDipoleExit == 1;
      HMS_Acceptance = H_gtr_dp>=-8.0 && H_gtr_dp<=8.0&&H_gtr_xptar>=-0.08&&H_gtr_xptar<=0.08&&H_gtr_yptar>=-0.045&&H_gtr_yptar<=0.045;       
      ELECTRON_PID = H_cer_npeSum >=0.5 && H_cal_etotnorm >=0.8 && H_cal_etotnorm <=1.2;   
       
      if(HMS_FixCut && HMS_Acceptance && ELECTRON_PID && W <=1.0)   
	{
	  H_hsxfp->Fill(H_dc_x_fp);
	  H_hsyfp->Fill(H_dc_y_fp);
	  H_hsxpfp->Fill(H_dc_xp_fp);
	  H_hsypfp->Fill(H_dc_yp_fp);
	  H_hsxptar->Fill(H_gtr_xptar);
	  H_hsyptar->Fill(H_gtr_yptar);
	  H_hsdelta->Fill(H_gtr_dp);
	  H_Q2->Fill(Q2);
	  H_epsilon->Fill(epsilon);
	  H_W->Fill(W);
	  H_W2->Fill(W2);
	  H_etottrnorm->Fill(H_cal_etottracknorm);
	  H_etotnorm->Fill(H_cal_etotnorm);
	  H_npeSum->Fill(H_cer_npeSum);
	  h_gtr_p->Fill(H_gtr_p);
	  H_etottrnorm->Fill(H_cal_etottracknorm);	
	  H_npeSum->Fill(H_cer_npeSum);
	}     
    }      
    
  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *hist = OutHisto_file->mkdir("hist");
  hist->cd();
  H_hsxfp->Write();
  H_hsyfp->Write();
  H_hsxpfp->Write();
  H_hsypfp->Write();
  H_hsxptar->Write();
  H_hsyptar->Write();
  H_hsdelta->Write();
  H_Q2->Write();
  H_epsilon->Write();
  H_W->Write();
  H_W2->Write();
  H_etottrnorm->Write();
  H_etotnorm->Write();
  H_npeSum->Write();
  h_gtr_p->Write();
  H_etottrnorm->Write(); 
  H_npeSum->Write();  
  OutHisto_file->Close();  
}
