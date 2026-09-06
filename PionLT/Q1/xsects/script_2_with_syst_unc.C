#include <vector>
#include <iostream>
#include <fstream>

#include "TCanvas.h"

#include "TGraph.h"
#include "TGraphErrors.h"

#include "TGraph2D.h"
#include "TGraph2DErrors.h"

#include "TH3F.h"
#include "TAxis.h"

#include "TF1.h"
#include "TF2.h"

#include "TStyle.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"

#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"

#include "TH1.h"
#include "TH2.h"

#include "TList.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"

#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "TH1.h"
#include "TF1.h"
#include "TList.h"
#include "Math/WrappedMultiTF1.h"
#include "HFitInterface.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TGraph.h"
#include "TRandom.h"
#include "TMultiGraph.h"

#include "TText.h"
#include "TMinuit.h"

using namespace std;

Float_t pi = 3.1415926;

Int_t u_bin_num;
Int_t prl_tbin = 1;

struct GlobalChi2 { 
   GlobalChi2(  ROOT::Math::IMultiGenFunction & f1,  ROOT::Math::IMultiGenFunction & f2) : 
      fChi2_1(f1), fChi2_2(f2) {}

   double operator() (const double *par) const {
      double p1[2]; 
      p1[0] = par[0]; 
      p1[1] = par[1]; 

      double p2[2]; 
      p2[0] = par[0]; 
      p2[1] = par[1];

      return fChi2_1(p1) + fChi2_2(p2);
   } 

   const  ROOT::Math::IMultiGenFunction & fChi2_1;
   const  ROOT::Math::IMultiGenFunction & fChi2_2;
};

TGraphErrors* g_sig_l_total;
TGraphErrors* g_sig_t_total;
TGraphErrors* g_sig_lt_total;
TGraphErrors* g_sig_tt_total;

void single_setting(TString q2_set);
Double_t fun_exp(Double_t *x, Double_t *par);

Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par);
Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par);
Double_t LT_sep_x_mi_fun(Double_t *x, Double_t *par);

Float_t hi_eps, mi_eps, lo_eps;

Double_t sys_sigT_pct[7]  = {0.968, 1.291, 7.058, 10.996, 11.584, 9.082, 11.063};
Double_t sys_sigL_pct[7]  = {1.936, 1.957, 3.711, 6.932, 10.430, 6.617, 5.215};
Double_t sys_sigLT_pct[7] = {0.981, 1.334, 4.630, 10.309, 14.907, 13.420, 93.365};
Double_t sys_sigTT_pct[7] = {2.746, 1.912, 3.479, 12.366, 3.204, 2.980, 23.767};

/*
Double_t sys_sigT_pct[7]  = {0, 0, 0, 0, 0, 0, 0};
Double_t sys_sigL_pct[7]  = {0, 0, 0, 0, 0, 0, 0};
Double_t sys_sigLT_pct[7] = {0, 0, 0, 0, 0, 0, 0};
Double_t sys_sigTT_pct[7] = {0, 0, 0, 0, 0, 0, 0};
*/
void script_2_with_syst_unc() {
	g_sig_l_total = new TGraphErrors();
	g_sig_t_total = new TGraphErrors();
	g_sig_lt_total = new TGraphErrors();
	g_sig_tt_total = new TGraphErrors();

	single_setting("375");

	TCanvas* c4 = new TCanvas();
	c4->Update();
	g_sig_l_total->Draw("A*");
	gStyle->SetOptFit(1);
	c4->Print("lt_sep_plots/sig_L_total.png");
	c4->Clear();
	g_sig_t_total->Draw("A*");
	c4->Print("lt_sep_plots/sig_T_total.png");
	c4->Clear();
	g_sig_lt_total->Draw("A*");
	c4->Print("lt_sep_plots/sig_LT_total.png");
	c4->Clear();
	g_sig_tt_total->Draw("A*");
	c4->Print("lt_sep_plots/sig_TT_total.png");
}

void single_setting(TString q2_set){
    Float_t eps_lo_setting, eps_mi_setting, eps_hi_setting;
	TString eps_lo;
	TString eps_mi;
	TString eps_hi;
	Float_t eps_diff;

	TGraphErrors* sig_L_g  = new TGraphErrors();
	TGraphErrors* sig_T_g  = new TGraphErrors();
	TGraphErrors* sig_LT_g = new TGraphErrors();
	TGraphErrors* sig_TT_g = new TGraphErrors();

	if (q2_set == "375" ) {
		eps_lo = "286";
		eps_mi = "629";
		eps_hi = "781";

		eps_lo_setting = 0.286;
		eps_mi_setting = 0.629;
		eps_hi_setting = 0.781;

		lo_eps = eps_lo_setting;
		mi_eps = eps_mi_setting;
		hi_eps = eps_hi_setting;
		
		eps_diff = eps_hi_setting - eps_lo_setting;
	}

	TGraphErrors* sig_u_lo = new TGraphErrors();
	TGraphErrors* sig_u_mi = new TGraphErrors();
	TGraphErrors* sig_u_hi = new TGraphErrors();
	TGraphErrors* sig_u_diff = new TGraphErrors();

	TString file_name_1 = "x_unsep.pl_" + q2_set + "_" + eps_lo;
 	TString file_name_2 = "x_unsep.pl_" + q2_set + "_" + eps_hi;
 	TString file_name_3 = "x_unsep.pl_" + q2_set + "_" + eps_mi;

	TNtuple* n1 = new TNtuple("n1", "n1", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
	n1->ReadFile(file_name_1);
 	TNtuple* n2 = new TNtuple("n2", "n2", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
 	n2->ReadFile(file_name_2);
 	TNtuple* n3 = new TNtuple("n3", "n3", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
 	n3->ReadFile(file_name_3);

	ofstream file_out;
	file_out.open("x_sep.pl_" + q2_set, ofstream::out);
	
	Float_t uu, qq, ww;
	n1->SetBranchAddress("t",  &uu);
	n1->SetBranchAddress("Q2", &qq);
	n1->SetBranchAddress("w",  &ww);

	Int_t N = n1->GetEntries();
	vector<Float_t> u_list, q2_list, w_list;

	for (Int_t i = 0; i < N; ++i) {
	    n1->GetEntry(i);
	    if(!binary_search(u_list.begin(), u_list.end(), uu)) {                  
			u_list.push_back(uu);
			q2_list.push_back(qq);
			w_list.push_back(ww);
	    }
	}
	
	u_bin_num = u_list.size();

	TCanvas* c1 =  new TCanvas("c1", "c1", 1800, 600); 
	TCanvas* c2 =  new TCanvas("c2", "c2", 1500, 2000); 
	TCanvas* cthesis =  new TCanvas("cthesis", "cthesis", 1500, 600); 

	c1->Divide(4, 2);
	c2->Divide(2, 4);
 
	for (Int_t i = 0; i < u_bin_num; i++) {
		c1->cd(i+1);
 		TString upp;
		if (i == 0) {
		  upp.Form("t < %lf && x!=0.0", u_list[i]+0.001); 
		} else {
		  upp.Form("(t> %lf && t < %lf) && x!=0.0", u_list[i-1]+0.001, u_list[i]+0.001); 
		}
		
		n1->Draw("x:phi:dx", upp, "goff");
		TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());

		TF1* f1 = new TF1("lo_eps_fit", LT_sep_x_lo_fun,  11.25, 348.78, 4); 
		TF1* f2 = new TF1("hi_eps_fit", LT_sep_x_hi_fun,  11.25, 348.78, 4); 
		TF1* f3 = new TF1("mi_eps_fit", LT_sep_x_mi_fun,  11.25, 348.78, 4); 

	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
		Float_t ave_sig_lo = g1->GetMean(2);
		Float_t err_sig_lo = g1->GetRMS(2);

		sig_u_lo->SetPoint(sig_u_lo->GetN(), u_list[i], ave_sig_lo);
		sig_u_lo->SetPointError(sig_u_lo->GetN()-1, 0, err_sig_lo);

 	 	n2->Draw("x:phi:dx", upp, "goff");
   		TGraphErrors* g2_tmp = new TGraphErrors(n2->GetSelectedRows(), n2->GetV2(), n2->GetV1(), 0, n2->GetV3());
	   	TGraphErrors* g2 = (TGraphErrors*) g2_tmp->Clone("g2");
	  
		Float_t ave_sig_hi = g2->GetMean(2);
		Float_t err_sig_hi = g2->GetRMS(2);

		sig_u_hi->SetPoint(sig_u_hi->GetN(), u_list[i], ave_sig_hi);
		sig_u_hi->SetPointError(sig_u_hi->GetN()-1, 0, err_sig_hi);

		n3->Draw("x:phi:dx", upp, "goff");
   		TGraphErrors* g3_tmp = new TGraphErrors(n3->GetSelectedRows(), n3->GetV2(), n3->GetV1(), 0, n3->GetV3());
	   	TGraphErrors* g3 = (TGraphErrors*) g3_tmp->Clone("g3");

		Float_t ave_sig_mi = g3->GetMean(2);
		Float_t err_sig_mi = g3->GetRMS(2);

		sig_u_mi->SetPoint(sig_u_mi->GetN(), u_list[i], ave_sig_mi);
		sig_u_mi->SetPointError(sig_u_mi->GetN()-1, 0, err_sig_mi);

		TGraph2DErrors* g_plot_err = new TGraph2DErrors();
		Double_t g_xx, g_yy, g_yy_err;
				
		for(Int_t ii = 0; ii < g1->GetN(); ii++) {
			g1->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g1->GetErrorY(ii);
			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_lo_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0, 0.0, g_yy_err);
		}
			
		for(Int_t ii = 0; ii < g2->GetN(); ii++) {
			g2->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g2->GetErrorY(ii);
			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_hi_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0,  0.0, g_yy_err);
		}
		
		for(Int_t ii = 0; ii < g3->GetN(); ii++) {
			g3->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g3->GetErrorY(ii);
			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_mi_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0,  0.0, g_yy_err);
		}
		
		g_plot_err->SetFillColor(29);
		g_plot_err->SetMarkerSize(0.8);
		g_plot_err->SetMarkerStyle(20);
		g_plot_err->SetMarkerColor(kRed);
		g_plot_err->SetLineColor(kBlue-3);
		g_plot_err->SetLineWidth(2);

		TF2 *fff2 = new TF2("fff2","([0] + y*[1] + sqrt(2*y*(1+y))*[2]*cos(x*pi/180)  + y*[3]*cos(2*x*pi/180))/(2*TMath::Pi())", 11.25, 348.75, 0.238, 0.80);
		
 		TGraphErrors * sigL_change = new TGraphErrors(); 
 		TGraphErrors * sigT_change = new TGraphErrors(); 
 		TGraphErrors * sigLT_change = new TGraphErrors(); 
 		TGraphErrors * sigTT_change = new TGraphErrors(); 
  
		g_plot_err->Fit("fff2", "EMR");
 
 		sigL_change->SetPoint(sigL_change->GetN(), sigL_change->GetN()+1, fff2->GetParameter(1));
 		sigL_change->SetPointError(sigL_change->GetN()-1, 0, fff2->GetParError(1));
    
		sigT_change->SetPoint(sigT_change->GetN(), sigT_change->GetN()+1, fff2->GetParameter(0));
		sigT_change->SetPointError(sigT_change->GetN()-1, 0, fff2->GetParError(0));
 
   		g_plot_err->Fit("fff2", "EMR");
  
 		sigL_change->SetPoint(sigL_change->GetN(), sigL_change->GetN()+1, fff2->GetParameter(1));
 		sigL_change->SetPointError(sigL_change->GetN()-1, 0, fff2->GetParError(1));
    
		sigT_change->SetPoint(sigT_change->GetN(), sigT_change->GetN()+1, fff2->GetParameter(0));
		sigT_change->SetPointError(sigT_change->GetN()-1, 0, fff2->GetParError(0));
 
 		c2->Update();
 		c2->cd(i+1);
			
		TPad* pad = (TPad*)c2->GetPad(i+1);
		pad->SetLeftMargin(0.13);
		pad->SetBottomMargin(0.06);
		pad->SetTopMargin(0.02);
		pad->SetRightMargin(0.0);
		
  		g1->SetMarkerStyle(5);
  		g2->SetMarkerColor(2);
  		g2->SetLineColor(2);
  		g2->SetMarkerStyle(4);
  		g3->SetLineColor(3);
  		g3->SetMarkerStyle(29);

   		TMultiGraph *g = new TMultiGraph();
		g->SetMinimum(0.0);
		g->Add(g1);
		g->Add(g2);
		g->Add(g3);
 
 		g->Draw("AP");
		
 		f1->FixParameter(0, fff2->GetParameter(0));
 		f1->FixParameter(1, fff2->GetParameter(1));
 		f1->FixParameter(2, fff2->GetParameter(2));
 		f1->FixParameter(3, fff2->GetParameter(3));
 		
 		f2->FixParameter(0, fff2->GetParameter(0));
 		f2->FixParameter(1, fff2->GetParameter(1));
 		f2->FixParameter(2, fff2->GetParameter(2));
 		f2->FixParameter(3, fff2->GetParameter(3));
 
		f3->FixParameter(0, fff2->GetParameter(0));
 		f3->FixParameter(1, fff2->GetParameter(1));
 		f3->FixParameter(2, fff2->GetParameter(2));
 		f3->FixParameter(3, fff2->GetParameter(3));

		Double_t sigT_value  = fff2->GetParameter(0);
		Double_t sigL_value  = fff2->GetParameter(1);
		Double_t sigLT_value = fff2->GetParameter(2);
		Double_t sigTT_value = fff2->GetParameter(3);

		Double_t dSigT  = TMath::Abs(sigT_value)  * sys_sigT_pct[i]  / 100.0;
		Double_t dSigL  = TMath::Abs(sigL_value)  * sys_sigL_pct[i]  / 100.0;
		Double_t dSigLT = TMath::Abs(sigLT_value) * sys_sigLT_pct[i] / 100.0;
		Double_t dSigTT = TMath::Abs(sigTT_value) * sys_sigTT_pct[i] / 100.0;

		// Low epsilon points
		for (Int_t jj = 0; jj < g1->GetN(); jj++) {
			Double_t phi_deg, sigma_value;
			g1->GetPoint(jj, phi_deg, sigma_value);
			Double_t phi_rad = phi_deg * TMath::Pi() / 180.0;
			Double_t stat_err = g1->GetErrorY(jj);

			Double_t sys_err = TMath::Sqrt(
				TMath::Power(dSigT, 2) +
				TMath::Power(lo_eps * dSigL, 2) +
				TMath::Power(TMath::Sqrt(2.0 * lo_eps * (1.0 + lo_eps)) * TMath::Cos(phi_rad) * dSigLT, 2) +
				TMath::Power(lo_eps * TMath::Cos(2.0 * phi_rad) * dSigTT, 2)
			);

			sys_err /= (2.0 * TMath::Pi());
			Double_t total_err = TMath::Sqrt(stat_err * stat_err + sys_err * sys_err);
			g1->SetPointError(jj, 0.0, total_err);
		}

		// High epsilon points
		for (Int_t jj = 0; jj < g2->GetN(); jj++) {
			Double_t phi_deg, sigma_value;
			g2->GetPoint(jj, phi_deg, sigma_value);
			Double_t phi_rad = phi_deg * TMath::Pi() / 180.0;
			Double_t stat_err = g2->GetErrorY(jj);

			Double_t sys_err = TMath::Sqrt(
				TMath::Power(dSigT, 2) +
				TMath::Power(hi_eps * dSigL, 2) +
				TMath::Power(TMath::Sqrt(2.0 * hi_eps * (1.0 + hi_eps)) * TMath::Cos(phi_rad) * dSigLT, 2) +
				TMath::Power(hi_eps * TMath::Cos(2.0 * phi_rad) * dSigTT, 2)
			);

			sys_err /= (2.0 * TMath::Pi());
			Double_t total_err = TMath::Sqrt(stat_err * stat_err + sys_err * sys_err);
			g2->SetPointError(jj, 0.0, total_err);
		}

		// Mid epsilon points
		for (Int_t jj = 0; jj < g3->GetN(); jj++) {
			Double_t phi_deg, sigma_value;
			g3->GetPoint(jj, phi_deg, sigma_value);
			Double_t phi_rad = phi_deg * TMath::Pi() / 180.0;
			Double_t stat_err = g3->GetErrorY(jj);

			Double_t sys_err = TMath::Sqrt(
				TMath::Power(dSigT, 2) +
				TMath::Power(mi_eps * dSigL, 2) +
				TMath::Power(TMath::Sqrt(2.0 * mi_eps * (1.0 + mi_eps)) * TMath::Cos(phi_rad) * dSigLT, 2) +
				TMath::Power(mi_eps * TMath::Cos(2.0 * phi_rad) * dSigTT, 2)
			);

			sys_err /= (2.0 * TMath::Pi());
			Double_t total_err = TMath::Sqrt(stat_err * stat_err + sys_err * sys_err);
			g3->SetPointError(jj, 0.0, total_err);
		}

		// CALCULATE SEPARATE CHI2/NDF FOR EACH EPSILON AFTER TOTAL ERRORS ARE APPLIED
		Double_t chi2_lo = 0; Int_t ndf_lo = 0;
		for (Int_t jj = 0; jj < g1->GetN(); jj++) {
			Double_t phi, sigma; g1->GetPoint(jj, phi, sigma);
			Double_t total_err = g1->GetErrorY(jj);
			if (total_err > 0) {
				Double_t theory = fff2->Eval(phi, lo_eps);
				chi2_lo += TMath::Power((sigma - theory) / total_err, 2);
				ndf_lo++;
			}
		}
		// NDF for individual graph is points minus sharing parameters if done individually, 
		// but since we evaluate the quality of the fit on this subset, ndf = points. 
		// If we treat it as an independent curve validation, we use NDF = number of points.
		Double_t chi2_per_ndf_lo = (ndf_lo > 0) ? (chi2_lo / ndf_lo) : 0.0;

		Double_t chi2_mi = 0; Int_t ndf_mi = 0;
		for (Int_t jj = 0; jj < g3->GetN(); jj++) {
			Double_t phi, sigma; g3->GetPoint(jj, phi, sigma);
			Double_t total_err = g3->GetErrorY(jj);
			if (total_err > 0) {
				Double_t theory = fff2->Eval(phi, mi_eps);
				chi2_mi += TMath::Power((sigma - theory) / total_err, 2);
				ndf_mi++;
			}
		}
		Double_t chi2_per_ndf_mi = (ndf_mi > 0) ? (chi2_mi / ndf_mi) : 0.0;

		Double_t chi2_hi = 0; Int_t ndf_hi = 0;
		for (Int_t jj = 0; jj < g2->GetN(); jj++) {
			Double_t phi, sigma; g2->GetPoint(jj, phi, sigma);
			Double_t total_err = g2->GetErrorY(jj);
			if (total_err > 0) {
				Double_t theory = fff2->Eval(phi, hi_eps);
				chi2_hi += TMath::Power((sigma - theory) / total_err, 2);
				ndf_hi++;
			}
		}
		Double_t chi2_per_ndf_hi = (ndf_hi > 0) ? (chi2_hi / ndf_hi) : 0.0;

		// WRITE SEPARATE CHI2 VALUES ON THE PANEL GRID PLOT (c2)
		TLatex latex_chi2;
		latex_chi2.SetNDC();
		latex_chi2.SetTextSize(0.035);
		latex_chi2.SetTextFont(42);
		
		latex_chi2.SetTextColor(kBlack);
		latex_chi2.DrawLatex(0.15, 0.85, Form("#chi^{2}/N_{pts} (lo) = %.2f", chi2_per_ndf_lo));
		latex_chi2.SetTextColor(kBlue+1);
		latex_chi2.DrawLatex(0.15, 0.80, Form("#chi^{2}/N_{pts} (mi) = %.2f", chi2_per_ndf_mi));
		latex_chi2.SetTextColor(kRed);
		latex_chi2.DrawLatex(0.15, 0.75, Form("#chi^{2}/N_{pts} (hi) = %.2f", chi2_per_ndf_hi));

		f1->SetLineColor(1);
		f2->SetLineColor(2);	 		
		f3->SetLineColor(3);	 		
  
		f1->Draw("same");	
		f2->Draw("same");	
		f3->Draw("same");	

		if(i == prl_tbin){
		  TCanvas *cPRL = new TCanvas("cPRL","PRL Single Bin",800,600);
		  cPRL->SetFillColor(0);
		  cPRL->SetBorderMode(0);
		  cPRL->SetFrameBorderMode(0);
		  cPRL->SetLeftMargin(0.14);
		  cPRL->SetBottomMargin(0.14);
		  cPRL->SetRightMargin(0.04);
		  cPRL->SetTopMargin(0.05);
		  gStyle->SetOptStat(0);
		  gStyle->SetOptFit(0);
		  
		  g1->SetMarkerStyle(20);
		  g1->SetMarkerSize(1.0);
		  g1->SetMarkerColor(kBlack);
		  g1->SetLineColor(kBlack);
		  
		  g2->SetMarkerStyle(24);
		  g2->SetMarkerSize(1.0);
		  g2->SetMarkerColor(kRed);
		  g2->SetLineColor(kRed);
		  
		  g3->SetMarkerStyle(25);
		  g3->SetMarkerSize(1.0);
		  g3->SetMarkerColor(kBlue+1);
		  g3->SetLineColor(kBlue+1);
		  
		  TMultiGraph *mgPRL = new TMultiGraph();
		  mgPRL->Add(g1);
		  mgPRL->Add(g2);
		  mgPRL->Add(g3);
		  
		  mgPRL->Draw("AP");
		  mgPRL->SetMinimum(0.0);
		  mgPRL->GetXaxis()->SetTitle("#phi (deg)");
		  mgPRL->GetYaxis()->SetTitle("d^{2}#sigma/dtd#phi (#mub/GeV^{2})");
		  mgPRL->GetXaxis()->CenterTitle();
		  mgPRL->GetYaxis()->CenterTitle();
		  mgPRL->GetXaxis()->SetTitleSize(0.05);
		  mgPRL->GetYaxis()->SetTitleSize(0.05);
		  mgPRL->GetXaxis()->SetLabelSize(0.045);
		  mgPRL->GetYaxis()->SetLabelSize(0.045);
		  mgPRL->GetXaxis()->SetTitleOffset(1.1);
		  mgPRL->GetYaxis()->SetTitleOffset(1.35);
		  mgPRL->GetXaxis()->SetLimits(0,360);

		  f1->SetLineColor(kBlack);
		  f2->SetLineColor(kRed);
		  f3->SetLineColor(kBlue+1);
		  f1->SetLineWidth(2);
		  f2->SetLineWidth(2);
		  f3->SetLineWidth(2);
    
		  f1->Draw("same");
		  f2->Draw("same");
		  f3->Draw("same");
		  
		  TLegend *leg = new TLegend(0.58,0.24,0.88,0.42);
		  leg->SetBorderSize(0);
		  leg->SetFillStyle(0);
		  leg->SetTextSize(0.038);
		  leg->AddEntry(g1,Form("#epsilon = %.3f",lo_eps),"pe");
		  leg->AddEntry(g3,Form("#epsilon = %.3f",mi_eps),"pe");
		  leg->AddEntry(g2,Form("#epsilon = %.3f",hi_eps),"pe");
		  leg->Draw();
		  
		  TLatex latex;
		  latex.SetNDC();
		  latex.SetTextFont(42);
		  latex.SetTextSize(0.040);
		  latex.DrawLatex(0.18,0.37,Form("Q^{2} = %.3f GeV^{2}",q2_list[i]));
		  latex.DrawLatex(0.18,0.31,Form("W = %.2f GeV",w_list[i]));
		  latex.DrawLatex(0.18,0.25,Form("-t = %.3f GeV^{2}",u_list[i]));

		  // WRITE SEPARATE CHI2 VALUES ON THE PRL PLOT AS WELL
		  TLatex latex_prl_chi2;
		  latex_prl_chi2.SetNDC();
		  latex_prl_chi2.SetTextSize(0.035);
		  latex_prl_chi2.SetTextFont(42);
		  latex_prl_chi2.SetTextColor(kBlack);
		  latex_prl_chi2.DrawLatex(0.58, 0.55, Form("#chi^{2}/N_{lo} = %.2f", chi2_per_ndf_lo));
		  latex_prl_chi2.SetTextColor(kBlue+1);
		  latex_prl_chi2.DrawLatex(0.58, 0.50, Form("#chi^{2}/N_{mi} = %.2f", chi2_per_ndf_mi));
		  latex_prl_chi2.SetTextColor(kRed);
		  latex_prl_chi2.DrawLatex(0.58, 0.45, Form("#chi^{2}/N_{hi} = %.2f", chi2_per_ndf_hi));
		  
		  cPRL->SaveAs(Form("lt_sep_plots/PRL_tbin_%d.pdf",i));
		  cPRL->SaveAs(Form("lt_sep_plots/PRL_tbin_%d.eps",i));
		}
		
 		Float_t sig_l, sig_t, sig_lt, sig_tt;
 		Float_t sig_l_err, sig_t_err, sig_lt_err, sig_tt_err;
 
 		sig_t  =  fff2->GetParameter(0);
 		sig_l  =  fff2->GetParameter(1);
 		sig_lt =  fff2->GetParameter(2);
 		sig_tt =  fff2->GetParameter(3);
 
 		sig_t_err  =  fff2->GetParError(0);
 		sig_l_err  =  fff2->GetParError(1);
 		sig_lt_err =  fff2->GetParError(2);
 		sig_tt_err =  fff2->GetParError(3);
 
 		cout << " aaa  " << sig_t  << "  " << sig_t_err  << "  "
 			 << sig_l  << "  " << sig_l_err  << "  "
              << sig_lt << "  " << sig_lt_err << "  "
              << sig_tt << "  " << sig_tt_err << "  "
 		     << fff2->GetChisquare() << "  " << u_list[i]  << "  " 
              << w_list[i] << "  " << q2_list[i] << "  " << endl;
 
 		file_out << sig_t  << "  " << sig_t_err  << "  "
 				 << sig_l  << "  " << sig_l_err  << "  "
                  << sig_lt << "  " << sig_lt_err << "  "
                  << sig_tt << "  " << sig_tt_err << "  "
 			     << fff2->GetChisquare() << "  " << u_list[i]  << "  " 
                  << w_list[i] << "  " << q2_list[i] << "  " << endl;
 
 		delete g_plot_err;
 
  		g_sig_l_total->SetPoint(g_sig_l_total->GetN(), u_list[i], sig_l);
  		g_sig_l_total->SetPointError(g_sig_l_total->GetN()-1, 0, sig_l_err);
 
  		g_sig_t_total->SetPoint(g_sig_t_total->GetN(), u_list[i], sig_t);
  		g_sig_t_total->SetPointError(g_sig_t_total->GetN()-1, 0, sig_t_err);
 
  		g_sig_lt_total->SetPoint(g_sig_lt_total->GetN(), u_list[i], sig_lt);
  		g_sig_lt_total->SetPointError(g_sig_lt_total->GetN()-1, 0, sig_lt_err);
 
  		g_sig_tt_total->SetPoint(g_sig_tt_total->GetN(), u_list[i], sig_tt);
  		g_sig_tt_total->SetPointError(g_sig_tt_total->GetN()-1, 0, sig_tt_err);
 
 		sig_L_g->SetPoint(i,  u_list[i], sig_l);
 		sig_T_g->SetPoint(i,  u_list[i], sig_t);
 		sig_LT_g->SetPoint(i, u_list[i], sig_lt);
 		sig_TT_g->SetPoint(i, u_list[i], sig_tt);
 
 		sig_L_g->SetPointError(i,  0.0, sig_l_err);
 		sig_T_g->SetPointError(i,  0.0, sig_t_err);
 		sig_LT_g->SetPointError(i, 0.0, sig_lt_err);
 		sig_TT_g->SetPointError(i, 0.0, sig_tt_err);
 
 		TCanvas* cc4 = new TCanvas();
 		TString sig_check_str;
 		sig_check_str.Form("lt_sep_plots/sigL_change_tbin_%i",i);
 		sigL_change->Draw("a*");
 		cc4->Print( sig_check_str + "_" + q2_set + ".png");
 		sig_check_str.Form("lt_sep_plots/sigT_change_tbin_%i",i);
 		sigT_change->Draw("a*");	
 		cc4->Print( sig_check_str + "_" + q2_set + ".png");
 		delete cc4;
	}

	c1->Print("lt_sep_plots/check_"+ q2_set + ".png");
	c1->Print("lt_sep_plots/check_"+ q2_set + ".root");
	
	c2->cd();
	c2->Update();
                       
	TLatex label;
	label.SetTextFont(42);
	label.SetTextSize(0.04);
	label.DrawLatex(0.5, 0.02, "#phi (deg)");
	label.DrawLatex(0.14, 0.96, "-t = 0.009");
	label.DrawLatex(0.64, 0.96, "-t = 0.014");
	label.DrawLatex(0.14, 0.71, "-t = 0.020");
	label.DrawLatex(0.64, 0.71, "-t = 0.030");
	label.DrawLatex(0.14, 0.46, "-t = 0.038");
	label.DrawLatex(0.64, 0.46, "-t = 0.048");
	label.DrawLatex(0.14, 0.21, "-t = 0.057");

	label.SetTextAngle(90);
	label.DrawLatex(0.04, 0.5, "d^{2}#sigma/dtd#phi (#mub/GeV^{2})");
	c2->Print("lt_sep_plots/money_"+q2_set + ".pdf");
	c2->Print("lt_sep_plots/money_"+q2_set + ".root");

	TCanvas* c3 = new TCanvas("c3", "c3", 700, 700);
	c3->Divide(2,2);
	c3->cd(1);
	TPad* pad1 = (TPad*)c3->GetPad(1);
	pad1->SetLeftMargin(0.13);
	pad1->SetBottomMargin(0.08);
	pad1->SetTopMargin(0.02);
	pad1->SetRightMargin(0.0);
	sig_L_g->GetYaxis()->SetRangeUser(7.0,23.0);
	sig_L_g->Draw("a*");
	c3->cd(2);
	TPad* pad2 = (TPad*)c3->GetPad(2);
	pad2->SetLeftMargin(0.13);
	pad2->SetBottomMargin(0.08);
	pad2->SetTopMargin(0.02);
	pad2->SetRightMargin(0.0);
	sig_T_g->GetYaxis()->SetRangeUser(7.0,23.0);
	sig_T_g->Draw("a*");
	c3->cd(3);
	TPad* pad3 = (TPad*)c3->GetPad(3);
	pad3->SetLeftMargin(0.13);
	pad3->SetBottomMargin(0.08);
	pad3->SetTopMargin(0.02);
	pad3->SetRightMargin(0.0);
	sig_LT_g->GetYaxis()->SetRangeUser(-5.0,4.0);
	sig_LT_g->Draw("a*");
	c3->cd(4);
	TPad* pad4 = (TPad*)c3->GetPad(4);
	pad4->SetLeftMargin(0.13);
	pad4->SetBottomMargin(0.08);
	pad4->SetTopMargin(0.02);
	pad4->SetRightMargin(0.0);
	sig_TT_g->GetYaxis()->SetRangeUser(-5.0,4.0);
	sig_TT_g->Draw("a*");
	c3->cd();
	c3->Update();
                       
	TLatex label1;
	label1.SetTextFont(42);
	label1.SetTextSize(0.04);
	label1.DrawLatex(0.45, 0.01, "-t (GeV^{2})");
	label1.DrawLatex(0.20, 0.90, "#sigma_{L}");
	label1.DrawLatex(0.70, 0.90, "#sigma_{T}");
	label1.DrawLatex(0.20, 0.40, "#sigma_{LT}");
	label1.DrawLatex(0.70, 0.40, "#sigma_{TT}");

	label1.SetTextAngle(90);
	label1.DrawLatex(0.04, 0.5, "d^{2}#sigma/dtd#phi (#mub/GeV^{2})");
	c3->Print("lt_sep_plots/Sep_X_"+ q2_set + ".pdf");

	delete c1;
	delete c2;
	delete c3;

	file_out.close();
}

Double_t fun_exp(Double_t *x, Double_t *par){
    Float_t xx =x[0];
    Double_t sig =  par[0]*exp(par[1]*xx)+ par[2];
	return sig;
}

Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par){
	Float_t eps = lo_eps;
    Float_t xx =x[0];
	Double_t xs = (par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180))/(2*TMath::Pi());
	return xs;
}

Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par){
	Float_t eps = hi_eps;
    Float_t xx =x[0];
	Double_t xs = (par[0] +  eps*par[1]  + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180))/(2*TMath::Pi());
	return xs;
}

Double_t LT_sep_x_mi_fun(Double_t *x, Double_t *par){
	Float_t eps = mi_eps;
    Float_t xx =x[0];
	Double_t xs = (par[0] +  eps*par[1]  + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180))/(2*TMath::Pi());
	return xs;
}
