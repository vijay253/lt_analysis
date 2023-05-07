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

  file_name =   dir + "tmp" + ".dat";

  TNtuple* n1 = new TNtuple("n1", "n1", "R:eff");
  n1->ReadFile(file_name);
      
  TGraphErrors* t_g = new TGraphErrors();
  
  TCanvas *c1 = new TCanvas("c1", "c1");
      
  TString t_bin_limit;
  //  t_bin_limit.Form( "t_bin == %i && ratio != 0", in);	

  // n1->Draw("ratio:phi_bin", t_bin_limit, "*");
  n1->Draw("eff:R");
    
  TGraphErrors* g = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, 0);
		
  g->SetTitle("Yield Ratio: Yexp/Ysim");
	
  g->GetXaxis()->SetTitle("#phi angle");		
  g->GetXaxis()->CenterTitle();		
	
  g->GetYaxis()->SetTitle("Yield Ratio");		
  g->GetYaxis()->SetTitleOffset(1.8);		
  g->GetYaxis()->CenterTitle();		
	       
  g->SetMarkerStyle(29);
  g->GetYaxis()->SetRangeUser(0.0, 1.0);
  g->Draw("AP");
  c1->Print(dir + "Eff.png");

}     


