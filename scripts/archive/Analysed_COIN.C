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

void Analysed_COIN(string InDATAFilename = "", string OutFilename = "")
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
    //    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    //    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
    //    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";

    Replaypath = "/group/c-kaonlt/USERS/"+User+"/simc_gfortran";
    // Output path for root file
    ROOTfilePath = Replaypath+"/OUTPUTS/";
    OutPath = Replaypath+"/OUTPUTS/";
  }
  else if(Hostname.Contains("qcd")){
    Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    ROOTfilePath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
    OutPath = Replaypath+"/UTIL_KAONLT/OUTPUT/Analysis/HeeP";
  }
  else if (Hostname.Contains("phys.uregina.ca")){
    ROOTfilePath = "/home/vijay/work/HeepCoinStudy/";
    OutPath = "/home/vijay/work/HeepCoinStudy/";
  }

  if(InDATAFilename == "") {
    cout << "Enter a input File: ";
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

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TTree *s_tree = new TTree("scaler","Scaler Data");
  TTree *tree = new TTree("hist","Analyzed Data");
   
  //####################################################################
  TTree* TSCALER  = (TTree*)InFile->Get("TSP"); Long64_t nEntries_TSCALER  = (Long64_t)TSCALER->GetEntries();
  
  Double_t  P_BCM1_scalerCharge;TSCALER->SetBranchAddress("P.BCM1.scalerCharge", &P_BCM1_scalerCharge);
  Double_t  P_BCM2_scalerCharge;TSCALER->SetBranchAddress("P.BCM2.scalerCharge", &P_BCM2_scalerCharge);
  Double_t  P_BCM4A_scalerCharge;TSCALER->SetBranchAddress("P.BCM4A.scalerCharge", &P_BCM4A_scalerCharge);
  Double_t  P_BCM4B_scalerCharge;TSCALER->SetBranchAddress("P.BCM4B.scalerCharge", &P_BCM4B_scalerCharge);
  Double_t  P_BCM4C_scalerCharge;TSCALER->SetBranchAddress("P.BCM4C.scalerCharge", &P_BCM4C_scalerCharge);

  Double_t  P_BCM1_scalerCurrent;TSCALER->SetBranchAddress("P.BCM1.scalerCurrent", &P_BCM1_scalerCurrent);
  Double_t  P_BCM2_scalerCurrent;TSCALER->SetBranchAddress("P.BCM2.scalerCurrent", &P_BCM2_scalerCurrent);
  Double_t  P_BCM4A_scalerCurrent;TSCALER->SetBranchAddress("P.BCM4A.scalerCurrent", &P_BCM4A_scalerCurrent);
  Double_t  P_BCM4B_scalerCurrent;TSCALER->SetBranchAddress("P.BCM4B.scalerCurrent", &P_BCM4B_scalerCurrent);
  Double_t  P_BCM4C_scalerCurrent;TSCALER->SetBranchAddress("P.BCM4C.scalerCurrent", &P_BCM4C_scalerCurrent);
  //#################################################################### 

  TTree* TBRANCH  = (TTree*)InFile->Get("T"); Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();

  Double_t  CTime_epCoinTime_ROC1;TBRANCH->SetBranchAddress("CTime.epCoinTime_ROC1", &CTime_epCoinTime_ROC1);
  
  Double_t  H_dc_InsideDipoleExit;TBRANCH->SetBranchAddress("H.dc.InsideDipoleExit", &H_dc_InsideDipoleExit);
  //Double_t  H_hod_betanotrack;TBRANCH->SetBranchAddress("H_hod_betanotrack", &H_hod_betanotrack);

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
   
  Double_t  Q2;TBRANCH->SetBranchAddress("H.kin.primary.Q2", &Q2);   
  Double_t  W;TBRANCH->SetBranchAddress("H.kin.primary.W", &W);   
  Double_t  W2;TBRANCH->SetBranchAddress("H.kin.primary.W2", &W2);   
  Double_t  epsilon;TBRANCH->SetBranchAddress("H.kin.primary.epsilon", &epsilon);
  Double_t  pmx;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_x", &pmx);
  Double_t  pmy;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_y", &pmy);
  Double_t  pmz;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss_z", &pmz);
  Double_t  emiss;TBRANCH->SetBranchAddress("P.kin.secondary.emiss", &emiss);
  Double_t  pmiss;TBRANCH->SetBranchAddress("P.kin.secondary.pmiss", &pmiss);
  Double_t  MMp;TBRANCH->SetBranchAddress("P.kin.secondary.MMp", &MMp);
  
  //##############################################################################
  // Define Histograms that need to compare data and SIMC
  /*
  TH1F *H_ssxfp    = new TH1F("H_ssxfp","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1F *H_ssyfp    = new TH1F("H_ssyfp","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1F *H_ssxpfp   = new TH1F("H_ssxpfp","SHMS xpfp; ssxpfp;", 300, -0.09, 0.05);
  TH1F *H_ssypfp   = new TH1F("H_ssypfp","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1F *H_ssdelta  = new TH1F("H_ssdelta","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1F *H_ssxptar  = new TH1F("H_ssxptar","SHMS xptar; ssxptar;", 300, -0.1, 0.1);
  TH1F *H_ssyptar  = new TH1F("H_ssyptar","SHMS yptar; ssyptar;", 300, -0.04, 0.04);

  TH1F *H_hsxfp    = new TH1F("H_hsxfp","HMS xfp; hsxfp;", 300, -20.0, 20.0);
  TH1F *H_hsyfp    = new TH1F("H_hsyfp","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1F *H_hsxpfp   = new TH1F("H_hsxpfp","HMS xpfp; hsxpfp;", 300, -0.09, 0.05);
  TH1F *H_hsypfp   = new TH1F("H_hsypfp","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1F *H_hsdelta  = new TH1F("H_hsdelta","HMS delta; hsdelta;", 300, -20.0, 20.0);
  TH1F *H_hsxptar  = new TH1F("H_hsxptar","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1F *H_hsyptar  = new TH1F("H_hsyptar","HMS yptar; hsyptar;", 300, -0.04, 0.04);
  TH1F *H_etottracknorm  = new TH1F("H_etottracknorm"," H_cal_etottracknorm ; H_cal_etottracknorm;", 300, 0.0, 2);
  TH1F *H_etotnorm  = new TH1F("H_etotnorm"," H_cal_etotnorm ; H_cal_etotnorm;", 300, 0.0, 2);
  TH1F *H_npeSum  = new TH1F("H_npeSum","H_cer_npeSum ; H_cer_npeSum;", 300, 0.0, 30);
  TH1F *H_gtr_p  = new TH1F("H_gtr_p"," H_gtr_p ; H_gtr_p;", 300, 0.0, 10);

  TH1F *H_Q2       = new TH1F("H_Q2","Q2; Q2;", 300, 0.0, 4.0);      
  TH1F *H_epsilon  = new TH1F("H_epsilon","epsilon; epsilon;", 300, 0.80, 1.0);        
  TH1F *H_W  = new TH1F("H_W","W ; W;", 300, 0.6, 1.2);
  TH1F *H_W2  = new TH1F("H_W2","W2 ; W2;", 300, 0.0, 1.5);
  TH1F *H_pmx  = new TH1F("H_pmx","Pmx; Pmx;", 300, -0.2, 0.2);
  TH1F *H_pmy  = new TH1F("H_pmy","Pmy; Pmy;", 300, -0.2, 0.2);
  TH1F *H_pmz  = new TH1F("H_pmz","Pmz; Pmz;", 300, -0.2, 0.2);
  TH1F *H_emiss  = new TH1F("H_emiss","emiss; emiss;", 300, -0.1, 0.4);
  TH1F *H_pmiss  = new TH1F("H_pmiss","pmiss; Pm;", 300, -0.1, 0.4);
  TH1F *H_MMp  = new TH1F("H_MMp","MMp ; MMp;", 300, -0.01, 0.01);
  */
  tree->Branch("MMp",&MMp,"MMp/D");
  tree->Branch("Q2",&Q2,"Q2/D");
  tree->Branch("epsilon",&epsilon,"epsilon/D");
  tree->Branch("W",&W,"W/D");
  tree->Branch("W2",&W2,"W2/D");
  tree->Branch("pmx",&pmx,"pmx/D");
  tree->Branch("pmy",&pmy,"pmy/D");
  tree->Branch("pmz",&pmz,"pmz/D");
  tree->Branch("emiss",&emiss,"emiss/D");
  tree->Branch("pmiss",&pmiss,"pmiss/D");

  tree->Branch("ssxfp",&P_dc_x_fp,"P_dc_x_fp/D");
  tree->Branch("ssyfp",&P_dc_y_fp,"P_dc_y_fp/D");
  tree->Branch("ssxpfp",&P_dc_xp_fp,"P_dc_xp_fp/D");
  tree->Branch("ssypfp",&P_dc_yp_fp,"P_dc_yp_fp/D");
  tree->Branch("ssxptar",&P_gtr_xptar,"P_gtr_xptar/D");
  tree->Branch("ssyptar",&P_gtr_yptar,"P_gtr_yptar/D");
  tree->Branch("ssdelta",&P_gtr_dp,"P_gtr_dp/D");

  tree->Branch("hsxfp",&H_dc_x_fp,"H_dc_x_fp/D");
  tree->Branch("hsyfp",&H_dc_y_fp,"H_dc_y_fp/D");
  tree->Branch("hsxpfp",&H_dc_xp_fp,"H_dc_xp_fp/D");
  tree->Branch("hsypfp",&H_dc_yp_fp,"H_dc_yp_fp/D");
  tree->Branch("hsxptar",&H_gtr_xptar,"H_gtr_xptar/D");
  tree->Branch("hsyptar",&H_gtr_yptar,"H_gtr_yptar/D");
  tree->Branch("hsdelta",&H_gtr_dp,"H_gtr_dp/D");

  tree->Branch("P_hod_goodstarttime",&P_hod_goodstarttime,"P_hod_goodstarttime/D");
  tree->Branch("P_dc_InsideDipoleExit",&P_dc_InsideDipoleExit,"P_dc_InsideDipoleExit/D");
  tree->Branch("P_gtr_xptar",&P_gtr_xptar,"P_gtr_xptar/D");
  tree->Branch("P_gtr_yptar",&P_gtr_yptar,"P_gtr_yptar/D");
  tree->Branch("P_gtr_dp",&P_gtr_dp,"P_gtr_dp/D");
  tree->Branch("P_gtr_yptar",&P_gtr_yptar,"P_gtr_yptar/D");
  tree->Branch("P_cal_etottracknorm",&P_cal_etottracknorm,"P_cal_etottracknorm/D");
  tree->Branch("P_cal_etotnorm",&P_cal_etotnorm,"P_cal_etotnorm/D");
  tree->Branch("P_hgcer_npeSum",&P_hgcer_npeSum,"P_hgcer_npeSum/D");

  tree->Branch("H_gtr_p",&H_gtr_p,"H_gtr_p/D");
  tree->Branch("H_hod_goodscinhit",&H_hod_goodscinhit,"H_hod_goodscinhit/D");
  tree->Branch("H_hod_goodstarttime",&H_hod_goodstarttime,"H_hod_goodstarttime/D");
  tree->Branch("H_dc_InsideDipoleExit",&H_dc_InsideDipoleExit,"H_dc_InsideDipoleExit/D");
  tree->Branch("H_gtr_xptar",&H_gtr_xptar,"H_gtr_xptar/D");
  tree->Branch("H_gtr_yptar",&H_gtr_yptar,"H_gtr_yptar/D");
  tree->Branch("H_gtr_dp",&H_gtr_dp,"H_gtr_dp/D");
  tree->Branch("H_gtr_yptar",&H_gtr_yptar,"H_gtr_yptar/D");
  tree->Branch("H_cal_etottracknorm",&H_cal_etottracknorm,"H_cal_etottracknorm/D");
  tree->Branch("H_cal_etotnorm",&H_cal_etotnorm,"H_cal_etotnorm/D");
  tree->Branch("H_cer_npeSum",&H_cer_npeSum,"H_cer_npeSum/D");
  tree->Branch("H_cal_etottracknorm",&H_cal_etottracknorm),"H_cal_etottracknorm)/D";	

  tree->Branch("CTime_epCoinTime_ROC1",&CTime_epCoinTime_ROC1),"CTime_epCoinTime_ROC1)/D";	
  
  s_tree->Branch("bcm1_charge",&P_BCM1_scalerCharge,"P_BCM1_scalerCharge/D");
  s_tree->Branch("bcm2_charge",&P_BCM1_scalerCharge,"P_BCM2_scalerCharge/D");
  s_tree->Branch("bcm4a_charge",&P_BCM1_scalerCharge,"P_BCM4A_scalerCharge/D");
  s_tree->Branch("bcm4b_charge",&P_BCM1_scalerCharge,"P_BCM4B_scalerCharge/D");
  s_tree->Branch("bcm4c_charge",&P_BCM1_scalerCharge,"P_BCM4C_scalerCharge/D");

  s_tree->Branch("bcm1_current",&P_BCM1_scalerCurrent,"P_BCM1_scalerCurrent/D");
  s_tree->Branch("bcm2_current",&P_BCM1_scalerCurrent,"P_BCM2_scalerCurrent/D");
  s_tree->Branch("bcm4a_current",&P_BCM1_scalerCurrent,"P_BCM4A_scalerCurrent/D");
  s_tree->Branch("bcm4b_current",&P_BCM1_scalerCurrent,"P_BCM4B_scalerCurrent/D");
  s_tree->Branch("bcm4c_current",&P_BCM1_scalerCurrent,"P_BCM4C_scalerCurrent/D");
  
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

      tree->Fill();
    }      

  // Fill scaler events 
  for(Long64_t i = 0; i < nEntries_TSCALER; i++)
    {

      TSCALER->GetEntry(i);

      s_tree->Fill();
    }      
  
  cout << "Loop end\n\n" << endl;

  OutHisto_file->Write();  
  OutHisto_file->Close();  
}
