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

TGraphErrors* q2_g;
TGraphErrors* q2_ratio_lo;
TGraphErrors* q2_ratio_mi;
TGraphErrors* q2_ratio_hi;


TGraphErrors* q2_t_set;

TMultiGraph *mg;

TString dir = "OUTPUT/";


void plot_R() {
    
  TString file_name;

  file_name =   dir + "Center_highe" + ".dat";

  TNtuple* n1 = new TNtuple("n1", "n1", "R:eff:err");
  n1->ReadFile(file_name);
      
  TGraphErrors* t_g = new TGraphErrors();
  
  TCanvas *c1 = new TCanvas("c1", "c1");
      
  TString t_bin_limit;
  //  t_bin_limit.Form( "t_bin == %i && ratio != 0", in);	

  // n1->Draw("ratio:phi_bin", t_bin_limit, "*");
  n1->Draw("eff:R:err");
    
  TGraphErrors* g = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
		
  g->SetTitle("HMS Cal Eff");
	
  g->GetXaxis()->SetTitle("Run");		
  g->GetXaxis()->CenterTitle();		
	
  g->GetYaxis()->SetTitle("Efficiency");		
  g->GetYaxis()->SetTitleOffset(1.0);		
  g->GetYaxis()->CenterTitle();		
	       
  g->SetMarkerStyle(29);
  g->GetYaxis()->SetRangeUser(0.92, 1.0);
  g->Draw("AP");
  c1->Print(dir + "Eff.png");

}     


