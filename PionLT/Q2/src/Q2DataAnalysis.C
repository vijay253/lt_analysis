// .... Created Date: Jan 14, 2024 ....
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

void Q2DataAnalysis(string InDATAFilename = "", string OutFilename = "")
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
    //  Replaypath = "/group/c-kaonlt/USERS/"+User+"/hallc_replay_lt";
    // Output path for root file
    ROOTfilePath = "/cache/hallc/kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/";        // ROOT FILES PATH
    OutPath = "/volatile/hallc/c-kaonlt/vijay/OnlyQ2DataAnalysis/";              // OUTPUT FILES PATH
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
  // Establish the names of our output files quickly
  TString foutname = OutPath+"/" + TOutFilename + ".root";
  TString fouttxt  = OutPath+"/" + TOutFilename + ".txt";
  TString outputpdf  = OutPath+"/" + TOutFilename + ".pdf";
   
  //#################################################################### 

  TTree* TBRANCH  = (TTree*)InFile->Get("Cut_Kaon_Events_prompt_noRF");Long64_t nEntries_TBRANCH  = (Long64_t)TBRANCH->GetEntries();   // InFile
  TBRANCH->SetMakeClass(1);
  //List of variables
  Double_t tcoinC;TBRANCH->SetBranchAddress("CTime_ROC1", &tcoinC);  

  //##############################################################################
  TH1D *H_Picoin    = new TH1D("H_Picoin","CTime_ePiCoinTime_ROC1; CTime_ePiCoinTime_ROC1;", 300, -100.0, 100.0);
  TH1D *H_PicoinB    = new TH1D("H_PicoinB","CTime_ePiCoinTime_ROC1; CTime_ePiCoinTime_ROC1;", 300, -100.0, 100.0);
  TH1D *H_P_cal    = new TH1D("H_P_cal","P_cal_etottracknorm; P_cal_etottracknorm;", 300, 0.0, 2.0);
  TH1D *H_P_HGC    = new TH1D("H_P_HGC","P_hgcer_npeSum; P_hgcer_npeSum;", 300, 0.0, 20.0);
  TH1D *H_P_Aero    = new TH1D("H_P_Aero","P_aero_npeSum; P_aero_npeSum;", 300, 0.0, 30.0);
  TH1D *H_H_cal    = new TH1D("H_H_cal","H_cal_etottracknorm; H_cal_etottracknorm;", 300, 0.0, 2.0);
  TH1D *H_H_cer    = new TH1D("H_H_cer","H_cer_npeSum; H_cer_npeSum;", 300, 0.0, 30.0);
  TH2D *H_P_HGC_Aer    = new TH2D("H_P_HGC_Aer","HGC vs  Aero; P_hgcer_npeSum; P_aero_npeSum", 300, 0.0, 20.0, 300, 0, 30);
  TH2D *H_Picoin_MMpi    = new TH2D("H_Picoin_MMpi","epcoin vs MMpi; CTime_ePiCoinTime_ROC1; MMpi", 300, -20.0, 20.0, 300, 0, 2.0);

  //SHMS 
  TH1D *H_ssxfp    = new TH1D("H_ssxfp","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1D *H_ssyfp    = new TH1D("H_ssyfp","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1D *H_ssxfpB    = new TH1D("H_ssxfpB","SHMS xfp; ssxfp;", 300, -20.0, 20.0);
  TH1D *H_ssyfpB    = new TH1D("H_ssyfpB","SHMS yfp; ssyfp;", 300, -20.0, 20.0);
  TH1D *H_ssxpfp   = new TH1D("H_ssxpfp","SHMS xpfp; ssxpfp;", 300, -0.09, 0.05);
  TH1D *H_ssypfp   = new TH1D("H_ssypfp","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1D *H_ssxpfpB   = new TH1D("H_ssxpfpB","SHMS xpfp; ssxpfp;", 300, -0.09, 0.05);
  TH1D *H_ssypfpB   = new TH1D("H_ssypfpB","SHMS ypfp; ssypfp;", 300, -0.05, 0.04);
  TH1D *H_ssxptar  = new TH1D("H_ssxptar","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptar  = new TH1D("H_ssyptar","SHMS yptar; ssyptar;", 300, -0.04, 0.04);
  TH1D *H_ssxptarB  = new TH1D("H_ssxptarB","SHMS xptar; ssxptar;", 300, -0.06, 0.06);
  TH1D *H_ssyptarB  = new TH1D("H_ssyptarB","SHMS yptar; ssyptar;", 300, -0.04, 0.04);
  TH1D *H_ssdelta  = new TH1D("H_ssdelta","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  TH1D *H_ssdeltaB  = new TH1D("H_ssdeltaB","SHMS delta; ssdelta;", 300, -20.0, 20.0);
  //HMS
  TH1D *H_hsxfp    = new TH1D("H_hsxfp","HMS xfp; hsxfp;", 300, -40.0, 40.0);
  TH1D *H_hsyfp    = new TH1D("H_hsyfp","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1D *H_hsxfpB    = new TH1D("H_hsxfpB","HMS xfp; hsxfp;", 300, -40.0, 40.0);
  TH1D *H_hsyfpB    = new TH1D("H_hsyfpB","HMS yfp; hsyfp;", 300, -20.0, 20.0);
  TH1D *H_hsxpfp   = new TH1D("H_hsxpfp","HMS xpfp; hsxpfp;", 300, -0.09, 0.05); 
  TH1D *H_hsypfp   = new TH1D("H_hsypfp","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1D *H_hsxpfpB   = new TH1D("H_hsxpfpB","HMS xpfp; hsxpfp;", 300, -0.09, 0.05); 
  TH1D *H_hsypfpB   = new TH1D("H_hsypfpB","HMS ypfp; hsypfp;", 300, -0.05, 0.04);
  TH1D *H_hsxptar  = new TH1D("H_hsxptar","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptar  = new TH1D("H_hsyptar","HMS yptar; hsyptar;", 300, -0.05, 0.05);
  TH1D *H_hsxptarB  = new TH1D("H_hsxptarB","HMS xptar; hsxptar;", 300, -0.1, 0.1);
  TH1D *H_hsyptarB  = new TH1D("H_hsyptarB","HMS yptar; hsyptar;", 300, -0.05, 0.05);
  TH1D *H_hsdelta  = new TH1D("H_hsdelta","HMS Delta; hsdelta;", 300, -20.0, 20.0);
  TH1D *H_hsdeltaB  = new TH1D("H_hsdeltaB","HMS Delta; hsdelta;", 300, -20.0, 20.0);

  TH1D *H_q        = new TH1D("H_q","q; q;", 300, 5.0, 7.0);      
  TH1D *H_Q2       = new TH1D("H_Q2","Q2; Q2;", 300, 0.0, 1.0); 
  TH1D *H_Q2B       = new TH1D("H_Q2B","Q2; Q2;", 300, 0.0, 1.0); 
  TH1D *H_epsilon  = new TH1D("H_epsilon","epsilon; epsilon;", 300, 0.2, 0.4);  // 2p7 GeV   
  TH1D *H_epsilonB  = new TH1D("H_epsilonB","epsilon; epsilon;", 300, 0.2, 0.4); // 2p7 GeV 
  //TH1D *H_epsilon  = new TH1D("H_epsilon","epsilon; epsilon;", 300, 0.4, 0.9);     //3p6 and 4p5 GeV
  //TH1D *H_epsilonB  = new TH1D("H_epsilonB","epsilon; epsilon;", 300, 0.4, 0.9);   // 3p6 and 4p5 GeV
  TH1D *H_MMPi  = new TH1D("H_MMPi","MMPi ; MMPi;", 300, 0.9, 1.0); 
  TH1D *H_MMPiB = new TH1D("H_MMPiB"," MMPi (BACK); MMPi;", 300, 0.9, 1.0);
  TH1D *H_MMpi  = new TH1D("H_MMpi","MMpi ; MMpi;", 300, 0.6, 1.2); 
  TH1D *H_MMpiB = new TH1D("H_MMpiB"," MMpi (BACK); MMpi;", 300, 0.6, 1.2);
  TH1D *H_th  = new TH1D("H_th","X' tar; P_gtr_xp;", 300, -0.1, 0.1);
  TH1D *H_ph  = new TH1D("H_ph","Y' tar; P_gtr_yp;", 300, -0.1, 0.1);
  TH1D *H_pmiss  = new TH1D("H_pmiss","pmiss; Pm;", 300, 0.0, 0.3);
  TH1D *H_pmissB  = new TH1D("H_pmissB","pmiss; Pm;", 300, 0.0, 0.3);
  TH1D *H_emiss  = new TH1D("H_emiss","emiss; emiss;", 300, 0.8, 1.2); 
  TH1D *H_emissB  = new TH1D("H_emissB","emiss; emiss;", 300, 0.8, 1.2); 
  TH1D *H_pmx  = new TH1D("H_pmx","Pmx; Pmx;", 300, -0.5, 0.5);
  TH1D *H_pmxB  = new TH1D("H_pmxB","Pmx; Pmx;", 300, -0.5, 0.5);
  TH1D *H_pmy  = new TH1D("H_pmy","Pmy ; Pmy;", 300, -0.5, 0.5);
  TH1D *H_pmyB  = new TH1D("H_pmyB","Pmy ; Pmy;", 300, -0.5, 0.5);
  TH1D *H_pmz  = new TH1D("H_pmz","Pmz; Pmz;", 300, -0.7, -0.2);
  TH1D *H_pmzB  = new TH1D("H_pmzB","Pmz; Pmz;", 300, -0.7, -0.2);
  TH1D *H_W  = new TH1D("H_W","W ; W;", 300, 2.0, 2.3);
  TH1D *H_WB  = new TH1D("H_WB","W ; W;", 300, 2.0, 2.3);
  TH1D *H_t  = new TH1D("H_t","t ; t;", 8, 0.0, 0.1);    // old  value 0.06 2p7 center, left1
  TH1D *H_tB  = new TH1D("H_tB","t ; t;", 8, 0.0, 0.1);
  //TH1D *H_t  = new TH1D("H_t","t ; t;", 300, 0.0, 0.1);        // 2p7 left2
  //TH1D *H_tB  = new TH1D("H_tB","t ; t;", 300, 0.0, 0.1);
  TH1D *H_ph_q  = new TH1D("H_ph_q","ph_q ; ph_q;", 16, 0*TMath::Pi()/2, 2*TMath::Pi());    
  TH1D *H_ph_qB  = new TH1D("H_ph_qB","ph_qB ; ph_qB;", 16, 0*TMath::Pi()/2, 2*TMath::Pi());    

  TH2D* polar_hist = new TH2D("polar_hist","polar_hist",16, -TMath::Pi(), TMath::Pi(), 8, 0.0, 0.1); 

  for(Long64_t i = 0; i < nEntries_TBRANCH; i++)
    {
      TBRANCH->GetEntry(i);
    
      Double_t SHMS_FixCut;             
      Double_t SHMS_Acceptance;             
      Double_t HMS_Acceptance;
      Double_t epicointime; 
      Double_t epicointimeBack;            
      Double_t Pion_PID;            
      Double_t MMPiCUT;          
 
      //CUTs Definations 

      /*
      SHMS_FixCut = P_hod_goodscinhit == 1 && P_hod_goodstarttime == 1&&P_dc_InsideDipoleExit == 1; // && P_hod_betanotrack > 0.5 && P_hod_betanotrack < 1.4;
      SHMS_Acceptance = P_gtr_dp>=-10.0 && P_gtr_dp<=20.0&&P_gtr_xptar>=-0.06&&P_gtr_xptar<=0.06&&P_gtr_yptar>=-0.04&&P_gtr_yptar<=0.04;       
      HMS_Acceptance = H_gtr_dp>=-8.0&&H_gtr_dp<=8.0&&H_gtr_xptar>=-0.08&&H_gtr_xptar<=0.08&&H_gtr_yptar>=-0.045&&H_gtr_yptar<=0.045;       
      //  epicointime =  CTime_ePiCoinTime_ROC1>= -4.0 && CTime_ePiCoinTime_ROC1 <= -1.8;    //2.41 checked
      epicointime =  CTime_ePiCoinTime_ROC1>= -1.0 && CTime_ePiCoinTime_ROC1 <= 1;    //2.41 checked
      epicointimeBack = ((CTime_ePiCoinTime_ROC1 >= -11.0 && CTime_ePiCoinTime_ROC1 <= -5.0) || (CTime_ePiCoinTime_ROC1 >= 5.0 && CTime_ePiCoinTime_ROC1 <= 11.0)) ; 
      //   Pion_PID = P_hgcer_npeSum >= 0.1 && P_aero_npeSum >= 0.1 && P_cal_etottracknorm <= 0.7;   
      Pion_PID = P_cal_etottracknorm <= 0.7;   
      MMPiCUT = MMpi >= 0.92 && MMpi <= 0.988;
      */
      //Fill SHMS variables
             
	  H_Picoin->Fill(tcoinC);

      
      /*      if (SHMS_FixCut && SHMS_Acceptance && HMS_Acceptance && Pion_PID && epicointimeBack)  
	{
	  H_PicoinB->Fill(CTime_ePiCoinTime_ROC1);
	}
      
      if (SHMS_Acceptance && HMS_Acceptance)   
	{
	  H_Picoin->Fill(CTime_ePiCoinTime_ROC1);
	}          

      if (SHMS_Acceptance && HMS_Acceptance && epicointime && Pion_PID)   
	{
	  H_P_HGC_Aer->Fill(P_hgcer_npeSum, P_aero_npeSum);
	}          

      if (SHMS_Acceptance && HMS_Acceptance && epicointime && Pion_PID)  
	{
	  H_MMpi->Fill(MMpi);    
	}
      if (SHMS_Acceptance && HMS_Acceptance && epicointimeBack && Pion_PID)  
	{
	  H_MMpiB->Fill(MMpi);   
	}
            
      if (SHMS_Acceptance && HMS_Acceptance && epicointime && MMPiCUT && Pion_PID)   
	{
	  H_P_cal->Fill(P_cal_etottracknorm);
	  H_P_HGC->Fill(P_hgcer_npeSum);
	  H_P_Aero->Fill(P_aero_npeSum);
	  H_H_cal->Fill(H_cal_etottracknorm);
	  H_H_cer->Fill(H_cer_npeSum);
	  H_ssxfp->Fill(P_dc_x_fp);
	  H_ssyfp->Fill(P_dc_y_fp);
	  H_ssxpfp->Fill(P_dc_xp_fp);
	  H_ssypfp->Fill(P_dc_yp_fp);
	  H_ssxptar->Fill(P_gtr_xptar);
	  H_ssyptar->Fill(P_gtr_yptar);
	  H_ssdelta->Fill(P_gtr_dp);
	  H_hsxfp->Fill(H_dc_x_fp);
	  H_hsyfp->Fill(H_dc_y_fp);
	  H_hsxpfp->Fill(H_dc_xp_fp);
	  H_hsypfp->Fill(H_dc_yp_fp);
	  H_hsxptar->Fill(H_gtr_xptar);	
	  H_hsyptar->Fill(H_gtr_yptar);	
	  H_hsdelta->Fill(H_gtr_dp);	
	  H_MMPi->Fill(MMpi);    
	  H_Q2->Fill(Q2);
	  H_epsilon->Fill(epsilon);
	  H_pmiss->Fill(pmiss);
	  H_emiss->Fill(emiss);	
	  H_pmx->Fill(pmiss_x); 
	  H_pmy->Fill(pmiss_y); 
	  H_pmz->Fill(pmiss_z); 
	  H_W->Fill(W);
	  H_t->Fill(-1*MandelT);
	  H_ph_q->Fill(ph_q+3.17);  //3.17 was shift par
	  polar_hist->Fill(ph_q,-1*MandelT);
	}     
      
      if (SHMS_Acceptance && HMS_Acceptance && epicointimeBack && MMPiCUT && Pion_PID)  
	{
	  H_ssxfpB->Fill(P_dc_x_fp);
	  H_ssyfpB->Fill(P_dc_y_fp);
	  H_ssxpfpB->Fill(P_dc_xp_fp);
	  H_ssypfpB->Fill(P_dc_yp_fp);
	  H_ssxptarB->Fill(P_gtr_xptar);	
	  H_ssyptarB->Fill(P_gtr_yptar);	
	  H_ssdeltaB->Fill(P_gtr_dp);
	  H_hsxfpB->Fill(H_dc_x_fp);
	  H_hsyfpB->Fill(H_dc_y_fp);
	  H_hsxpfpB->Fill(H_dc_xp_fp);
	  H_hsypfpB->Fill(H_dc_yp_fp);
	  H_hsxptarB->Fill(H_gtr_xptar);	
	  H_hsyptarB->Fill(H_gtr_yptar);	
	  H_hsdeltaB->Fill(H_gtr_dp);	
	  H_Q2B->Fill(Q2);
	  H_MMPiB->Fill(MMpi);  
	  H_epsilonB->Fill(epsilon);
	  H_pmissB->Fill(pmiss);
	  H_emissB->Fill(emiss);	
	  H_pmxB->Fill(pmiss_x); 
	  H_pmyB->Fill(pmiss_y); 
	  H_pmzB->Fill(pmiss_z); 
	  H_WB->Fill(W);	
	  H_tB->Fill(-1*MandelT);
	  H_ph_qB->Fill(ph_q+3.17);
	}
      
      */
    }
  /*
  Double_t RandomScale = 1.0/6.0;   //#6 random beam buckets

  H_ssxfpB->Scale(RandomScale);
  H_ssyfpB->Scale(RandomScale);
  H_ssxpfpB->Scale(RandomScale);
  H_ssypfpB->Scale(RandomScale);
  H_ssxptarB->Scale(RandomScale);	
  H_ssyptarB->Scale(RandomScale);	
  H_ssdeltaB->Scale(RandomScale);
  H_hsxfpB->Scale(RandomScale);
  H_hsyfpB->Scale(RandomScale);
  H_hsxpfpB->Scale(RandomScale);
  H_hsypfpB->Scale(RandomScale);
  H_hsxptarB->Scale(RandomScale);	
  H_hsyptarB->Scale(RandomScale);	
  H_hsdeltaB->Scale(RandomScale);	
  H_Q2B->Scale(RandomScale);
  H_MMPiB->Scale(RandomScale);  
  H_MMpiB->Scale(RandomScale);  
  H_epsilonB->Scale(RandomScale);
  H_pmissB->Scale(RandomScale);
  H_emissB->Scale(RandomScale);	
  H_pmxB->Scale(RandomScale); 
  H_pmyB->Scale(RandomScale); 
  H_pmzB->Scale(RandomScale); 
  H_WB->Scale(RandomScale);	
  H_tB->Scale(RandomScale);
  H_ph_qB->Scale(RandomScale);

 
 //Background subtractions

  H_MMPi->Add(H_MMPiB, -1);
  H_MMpi->Add(H_MMpiB, -1); 
  H_ssxfp->Add(H_ssxfpB, -1);
  H_ssyfp->Add(H_ssyfpB, -1);
  H_ssxpfp->Add(H_ssxpfpB, -1);
  H_ssypfp->Add( H_ssypfpB, -1);
  H_ssxptar->Add(H_ssxptarB, -1);
  H_ssyptar->Add(H_ssyptarB, -1);
  H_ssdelta->Add(H_ssdeltaB, -1);
  H_hsxfp->Add(H_hsxfpB, -1);
  H_hsyfp->Add(H_hsyfpB, -1);
  H_hsxpfp->Add(H_hsxpfpB, -1);
  H_hsypfp->Add(H_hsypfpB, -1);
  H_hsxptar->Add(H_hsxptarB, -1);	
  H_hsyptar->Add(H_hsyptarB, -1);	
  H_hsdelta->Add(H_hsdeltaB, -1);	
  H_Q2->Add(H_Q2B, -1);
  H_epsilon->Add(H_epsilonB, -1);
  H_pmiss->Add(H_pmissB, -1);
  H_emiss->Add(H_emissB, -1);	
  H_pmx->Add(H_pmxB, -1); 
  H_pmy->Add(H_pmyB, -1); 
  H_pmz->Add( H_pmzB, -1); 
  H_W->Add(H_WB, -1);
  H_t->Add(H_tB, -1);
  H_ph_q->Add(H_ph_qB, -1);
      */
      
  // TF1 *pol3 = new TF1("pol3", "pol3(0)", 0.80, 1.40);
  // pol3->SetLineColor(kGreen);
  // H_MMK->Fit("pol3", "R");
  // gauss->Integral(0.84, 1.01);
  //cout<<" =" << gauss <<endl;
  /*  TH1D *h3 = new TH1D("h3", "Pol3(green), gaus(red) and pol3+gaus(yellow); Channel; Events", 300 ,0, 2.0);
  for (int i=0;i<300;i++)
  {
  Double_t x = H_MMpi->GetBinCenter(i);
  Double_t y=gauss->Eval(x);
  h3->Fill(y);
  }*/

  /* 
     This part for subtraction of background
     TH1F *htemp = (TH1F*)H_MMK->Clone("htemp");
  htemp->Reset();
  htemp->SetMinimum(-50.);
  TH1F *htemp1 = (TH1F*)H_MMK->Clone("htemp1");
  htemp1->Reset();
  // TH1F *htemp2 = (TH1F*)H_MMK->Clone("htemp");
  // htemp2->Reset();
  TH1F *h_ssxptar = (TH1F*)H_ssxptar->Clone("h_ssxptar");
  h_ssxptar->Reset();
  TH1F *h_ssyptar = (TH1F*)H_ssyptar->Clone("h_ssyptar");
  h_ssyptar->Reset();
  // TF1 *eback = H_MMpi->GetFunction("pol3");

    {
    for (Int_t bin=0;bin< 300;bin++) 
      {
	Float_t x = H_MMK->GetBinCenter(bin);
	Float_t x1 = h_ssxptar->GetBinCenter(bin);
	Float_t x2 = h_ssyptar->GetBinCenter(bin);
	//	Double_t fval = eback->Eval(x);
	//	Double_t fval1 = eback->Eval(x1);	
	
	if(H_MMpi->GetBinContent(bin) <= H_MMK->GetBinContent(bin))
	  {	
	    Double_t diff = TMath::Abs(H_MMK->GetBinContent(bin)- H_MMpi->GetBinContent(bin));
	    htemp->Fill(x,diff);
	  }

	//htemp->Add(htemp1, +1);
	
	//	Double_t diff1 = (h_ssxptar->GetBinContent(bin)- H_MMpi->GetBinContent(bin));
	//	Double_t diff2 = (h_ssyptar->GetBinContent(bin)- H_MMpi->GetBinContent(bin));
	//htemp->Fill(x,diff);
	//	h_ssxptar->Fill(x1,diff);
	//	h_ssyptar->Fill(x2,diff);
      }
    
    }
    TAxis *x2 = H_MMK->GetXaxis();
    
    Double_t INT1 =  htemp->Integral(x1->FindBin(1.10), x1->FindBin(1.140), "");
    Double_t INT2 =  H_MMK->Integral(x2->FindBin(1.10), x2->FindBin(1.140), "");
    cout<<" INT1 "<<INT1<<" INT2 "<<INT2<<endl;
    */


  //  H_MMK->Add(H_MMpi, -1);
  /*   TCanvas *c1 = new TCanvas("c1","Fitting Demo of Histogram in File",1500,1500,700,500);
    H_Kcoin->SetLineColor(kRed); 
    H_Kcoin->Draw("HIST");
    H_Kcoin_MMK->SetLineColor(kGreen);
    H_Kcoin_MMK->Draw("same HIST");  
    H_Kcoin_RF->SetLineColor(kBlue);
    H_Kcoin_RF->Draw("same HIST");
    c1->SaveAs();  
  */
  // Write root file

  TFile *OutHisto_file = new TFile(foutname,"RECREATE");
  TDirectory *hist = OutHisto_file->mkdir("hist");
  hist->cd();
  H_Picoin->Write();
  /*
  H_Picoin->Write();
  H_PicoinB->Write();   
  H_MMpi->Write();      
  H_MMPi->Write();     

  H_P_cal->Write();
  H_P_HGC->Write();
  H_P_Aero->Write();
  H_H_cal->Write();
  H_H_cer->Write();
  H_P_HGC_Aer->Write();
  H_ssxfp->Write(); 
  H_ssyfp->Write(); 
  H_ssxpfp->Write(); 
  H_ssypfp->Write(); 
  H_hsxfp->Write(); 
  H_hsyfp->Write(); 
  H_hsxpfp->Write(); 
  H_hsypfp->Write(); 
  H_ssxptar->Write();
  H_ssyptar->Write();
  H_hsxptar->Write();
  H_hsyptar->Write();
  H_ssdelta->Write();
  H_hsdelta->Write();
  H_Q2->Write();
  H_epsilon->Write();
  H_pmiss->Write();
  H_emiss->Write();
  H_pmx->Write();
  H_pmy->Write();
  H_pmz->Write();
  H_W->Write();
  H_t->Write(); 
  H_ph_q->Write();
  polar_hist->Write(); 
 */
  OutHisto_file->Close();
}
