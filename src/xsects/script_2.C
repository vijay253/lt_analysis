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



// // definition of shared parameter
// // background function
// int iparB1[4] = { 0,   
//                   1,   
// 				  2,
// 				  3
// };
// 
// // signal + background function
// int iparB2[4] = { 0, 
//                   1, 
//                   2, 
//                   3  
// };


// struct GlobalChi2 { 
//    GlobalChi2(  ROOT::Math::IMultiGenFunction & f1,  ROOT::Math::IMultiGenFunction & f2) : 
//       fChi2_1(f1), fChi2_2(f2) {}
// 
//    // parameter vector is first background (in common 1 and 2) and then is signal (only in 2)
//    double operator() (const double *par) const {
//       double p1[4]; 
//       p1[0] = par[0]; // exp amplitude in B histo
//       p1[1] = par[1]; // exp common parameter 
//       p1[2] = par[2]; // exp common parameter 
//       p1[3] = par[3]; // exp common parameter 
// 
//       double p2[4]; 
//       p2[0] = par[0]; // exp amplitude in S+B histo
//       p2[1] = par[1]; // exp common parameter
//       p2[2] = par[2]; // gaussian amplitude
//       p2[3] = par[3]; // gaussian mean
// 
//       return fChi2_1(p1) + fChi2_2(p2);
//    } 
// 
//    const  ROOT::Math::IMultiGenFunction & fChi2_1;
//    const  ROOT::Math::IMultiGenFunction & fChi2_2;
// };


struct GlobalChi2 { 
   GlobalChi2(  ROOT::Math::IMultiGenFunction & f1,  ROOT::Math::IMultiGenFunction & f2) : 
      fChi2_1(f1), fChi2_2(f2) {}

   // parameter vector is first background (in common 1 and 2) and then is signal (only in 2)
   double operator() (const double *par) const {
      double p1[2]; 
      p1[0] = par[0]; // exp amplitude in B histo
      p1[1] = par[1]; // exp common parameter 

      double p2[2]; 
      p2[0] = par[0]; // exp amplitude in S+B histo
      p2[1] = par[1]; // exp common parameter

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


void script_2() {


	g_sig_l_total = new TGraphErrors();
	g_sig_t_total = new TGraphErrors();
	g_sig_lt_total = new TGraphErrors();
	g_sig_tt_total = new TGraphErrors();

	single_setting("375");
	// 	single_setting("245");


	TCanvas* c4 = new TCanvas();

	c4->Update();
	
	g_sig_l_total->Draw("A*");

	gStyle->SetOptFit(1);

//	TF1* f1 = new TF1("f1", fun_exp, 0, 0.5, 3);

//	f1->SetParameter();

//	g_sig_l_total->Fit("f1", "R");

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


//	g_sig_t_total->Fit("f1", "R");




}


///*--------------------------------------------------*/

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
	/**if (q2_set == "160") {

		eps_lo = "32";
		eps_hi = "59";


		eps_lo_setting = 0.32;
		eps_hi_setting = 0.59;

		lo_eps = eps_lo_setting;
		hi_eps = eps_hi_setting;

		eps_diff = eps_hi_setting - eps_lo_setting;

		}**/


	TGraphErrors* sig_u_lo = new TGraphErrors();
	TGraphErrors* sig_u_mi = new TGraphErrors();
	TGraphErrors* sig_u_hi = new TGraphErrors();
	TGraphErrors* sig_u_diff = new TGraphErrors();

	TString file_name_1;
	file_name_1 = "x_unsep.pl_" + q2_set + "_" + eps_lo;

 	TString file_name_2;
 	file_name_2 = "x_unsep.pl_" + q2_set + "_" + eps_hi;

 	TString file_name_3;
 	file_name_3 = "x_unsep.pl_" + q2_set + "_" + eps_mi;

	TNtuple* n1 = new TNtuple("n1", "n1", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
	n1->ReadFile(file_name_1);
 
 	TNtuple* n2 = new TNtuple("n2", "n2", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
 	n2->ReadFile(file_name_2);

 	TNtuple* n3 = new TNtuple("n3", "n3", "x/F:dx:x_mod:eps:theta:phi:t:w:Q2");
 	n3->ReadFile(file_name_3);

	ofstream file_out;
	file_out.open("x_sep.pl_" + q2_set, ofstream::out);
	

//	n1->Draw("t");

	Float_t uu, qq, ww;


	n1->SetBranchAddress("t",  &uu);
	n1->SetBranchAddress("Q2", &qq);
	n1->SetBranchAddress("w",  &ww);

	Int_t N = n1->GetEntries();
	
	vector<Float_t> u_list, q2_list, w_list;



	for (Int_t i = 0; i < N; ++i)
	  {

	    n1->GetEntry(i);
	    
	    if(!binary_search(u_list.begin(), u_list.end(), uu))              
	      {                  
		
		cout << i << "  "<< uu << endl;
		
		u_list.push_back(uu);
		q2_list.push_back(qq);
		w_list.push_back(ww);
		
	      }
	    
	  }

//	exit(0);

	
	cout << " tc "<< u_list.size() << endl;

	u_bin_num = u_list.size();

	TCanvas* c1 =  new TCanvas("c1", "c1", 1800, 600); 
	TCanvas* c2 =  new TCanvas("c2", "c2", 1500, 1200); 
	//thesis plot
	TCanvas* cthesis =  new TCanvas("cthesis", "cthesis", 1500, 600); 


	//	c1->Divide(u_bin_num, 1, 0.003, 0.003);
	//	c2->Divide(u_bin_num, 1, 0.003, 0.003);
	c1->Divide(4, 2);
	c2->Divide(3, 3);
	//	c1->Divide(u_bin_num, 1);
	//	c2->Divide(u_bin_num, 1);
 
//	exit(0);


//	for (Int_t i = 0; i < u_list.size(); i++) {
	for (Int_t i = 0; i < u_bin_num; i++) {

		c1->cd(i+1);

 		TString upp;
// 
		if (i == 0) {

		  upp.Form("t < %lf && x!=0.0", u_list[i]+0.001); 
		  //			upp.Form("t < %lf && x!=0.0", u_list[i]); 

		} else {

		  upp.Form("(t> %lf && t < %lf) && x!=0.0", u_list[i-1]+0.001, u_list[i]+0.001); 
		  //	upp.Form("(t> %lf && t < %lf) && x!=0.0", u_list[i-1], u_list[i]); 

		}
		
		cout <<"upp"<< upp << endl;

//		cout << u_list[i]+0.01 << endl;

//		exit(0);


// 		cout << "asdasd "<< upp << endl;

		cout << i+1 << endl;

		n1->Draw("x:phi:dx", upp, "goff");
		TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
		//		TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, 0);

//		TF1* f1 = new TF1("lo_eps_fit", LT_sep_x_lo_fun, 0, 360, 4); 
//		TF1* f2 = new TF1("hi_eps_fit", LT_sep_x_hi_fun, 0, 360, 4); 

		TF1* f1 = new TF1("lo_eps_fit", LT_sep_x_lo_fun,  11.25, 348.78, 4); 
		TF1* f2 = new TF1("hi_eps_fit", LT_sep_x_hi_fun,  11.25, 348.78, 4); 
		TF1* f3 = new TF1("mi_eps_fit", LT_sep_x_mi_fun,  11.25, 348.78, 4); 

	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
		
		Float_t ave_sig_lo = g1->GetMean(2);
		Float_t err_sig_lo = g1->GetRMS(2);

		sig_u_lo->SetPoint(sig_u_lo->GetN(), u_list[i], ave_sig_lo);
		sig_u_lo->SetPointError(sig_u_lo->GetN()-1, 0, err_sig_lo);

//		g_sig_t->SetPoint(g_sig_t->GetN(),        u_list[i], ave_sig_lo);
//		g_sig_t->SetPointError(g_sig_t->GetN()-1,         0, err_sig_lo);


// 	 	n2->Draw("x:phi", upp, "*");
// 	 	TGraph* g2_tmp = (TGraph*) c1->GetPad(i+1)->GetListOfPrimitives()->FindObject("Graph");
// 
		n2->Draw("x:phi:dx", upp, "goff");
//		n2->Draw("x:phi:dx", "", "goff");

   		TGraphErrors* g2_tmp = new TGraphErrors(n2->GetSelectedRows(), n2->GetV2(), n2->GetV1(), 0, n2->GetV3());
//   		TGraphErrors* g2_tmp = new TGraphErrors(n2->GetSelectedRows(), n2->GetV2(), n2->GetV1(), 0, 0);

	   	TGraphErrors* g2 = (TGraphErrors*) g2_tmp->Clone("g2");
	  

//   		ROOT::Math::WrappedMultiTF1 wfB1(*f1,1);
//   		ROOT::Math::WrappedMultiTF1 wfB2(*f2,1);
// 
//   		ROOT::Fit::DataOptions opt; 
// 
//   		ROOT::Fit::DataRange rangeB1;
//   		ROOT::Fit::DataRange rangeB2; 
//   
//   		ROOT::Fit::BinData dataB1(opt,rangeB1);
//   		ROOT::Fit::BinData dataB2(opt,rangeB2);
// 
// 
// 		rangeB1.SetRange(0,360);
//   		rangeB2.SetRange(0,360);
// 
// //	  	ROOT::Fit::BinData dataB1; 
//   		ROOT::Fit::FillData(dataB1, g1);
// 
// //  		ROOT::Fit::BinData dataB2; 
//   		ROOT::Fit::FillData(dataB2, g2);
// 
// 	  	ROOT::Fit::Chi2Function chi2_B1(dataB1, wfB1);
//   		ROOT::Fit::Chi2Function chi2_B2(dataB2, wfB2);
// 
//   		GlobalChi2 globalChi2(chi2_B1, chi2_B2);
// 
//   		ROOT::Fit::Fitter fitter;
//  
// //         const int Npar = 4; 
// // 		double par0[Npar] = { 0.0, 0.0, 0.0, 0.0};
//  
//         const int Npar = 2; 
// 		double par0[Npar] = { 0.0, 0.0};
// 
// 
// 		fitter.Config().SetMinimizer("Minuit","Minimize"); 
//  
// 		fitter.FitFCN(2,globalChi2,par0,dataB1.Size()+dataB2.Size());
//   		ROOT::Fit::FitResult result = fitter.Result();
//  		result.Print(std::cout);
//  		f1->SetParameters(result.GetParams());
//  		f2->SetParameters(result.GetParams());


  		//hSB->GetListOfFunctions()->Add(fSB);


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


// 	
// 		g1->Draw("A*");
// 
// //		g1->Fit("lo_eps_fit", "R");
// 
// // 
// //  		f1->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
// //  		f2->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
// //
// 
// 		g2->SetMarkerColor(2);
// 		g2->SetLineColor(2);
// 	 	g2->Draw("*");
// 
// 		f1->SetLineColor(1);
// 		f2->SetLineColor(2);	
// 
// 		
// 		f1->Draw("same");	
// 		f2->Draw("same");	
// 

		cout << "I'm here'" << g1->GetN() << endl;


		TGraph2DErrors* g_plot_err = new TGraph2DErrors();

		Double_t g_xx, g_yy, g_yy_err;
				
		for(Int_t ii = 0; ii < g1->GetN(); ii++) {

			g1->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g1->GetErrorY(ii);

			cout << ii << "  " <<  g_plot_err->GetN() << "  "<< g_xx << "  " << eps_lo_setting 
				 << "  "  << g_yy <<  "  " << g_yy_err << endl;


			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_lo_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0, 0.0, g_yy_err);

			}
			
		for(Int_t ii = 0; ii < g2->GetN(); ii++) {

			g2->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g2->GetErrorY(ii);

			cout << ii << "  " << g_plot_err->GetN() << "  "<< g_xx << "  " << eps_hi_setting 
				 << "  " << g_yy <<  "  " << g_yy_err << endl;

			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_hi_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0,  0.0, g_yy_err);

		}
		
		for(Int_t ii = 0; ii < g3->GetN(); ii++) {

			g3->GetPoint(ii, g_xx, g_yy);
			g_yy_err = g3->GetErrorY(ii);

			cout << ii << "  " << g_plot_err->GetN() << "  "<< g_xx << "  " << eps_mi_setting 
				 << "  " << g_yy <<  "  " << g_yy_err << endl;

			g_plot_err->SetPoint(g_plot_err->GetN(), g_xx, eps_mi_setting, g_yy);
			g_plot_err->SetPointError(g_plot_err->GetN()-1, 0.0,  0.0, g_yy_err);
		}
		
		g_plot_err->SetFillColor(29);
		g_plot_err->SetMarkerSize(0.8);
		g_plot_err->SetMarkerStyle(20);

		g_plot_err->SetMarkerColor(kRed);
		g_plot_err->SetLineColor(kBlue-3);
		g_plot_err->SetLineWidth(2);
		

// 
// //		g_plot_err->SetDrawOption("perr");
// 
// 		g_plot_err->GetXaxis()->SetLimits(0, 360);
// 		g_plot_err->GetYaxis()->SetLimits(0.286, 0.781);
// 		g_plot_err->GetZaxis()->SetLimits(1e-6, 6e-6);
// 
//   		g_plot_err->SetMaximum(40e-9);
//   		g_plot_err->SetMinimum(10e-9);
// 
// //		g_plot_err->GetZaxis()->SetRangeUser(10e-9, 50e-9);


//         g_plot_err->GetHistogram()->SetMaximum(50e-9);           
//         g_plot_err->GetHistogram()->SetMinimum(10e-9);    
// 
//   		TH3F* h3 = new TH3F("h3", "h2", 10, 0, 360, 10, 0.25, 0.65, 10, 10e-9, 40e-9);
//   		h3->Draw();       

//		g_plot_err->Draw();


// 	 	c1->cd(i+1);
//		g_plot_err->DrawClone("perr");
// 		c1->Modified();
// 
// 
// 
//  	 	c1->Update();
  	
 
 
 // 		cout << "asdasdasd " <<  g_plot_err->GetZminE() << endl;
 // 
 //		TF2 *fff2 = new TF2("fff2","[0] + y*[1]", 0, 360, 0, 0.6);
 
 
  		///*--------------------------------------------------*/
 		// Standard LT seperation equation 
 		// Parameter assignment
 		// par 0: for sigT
 		// par 1: for sigL
 		// par 2: for sigLT
 		// par 3: for sigTT
 
		//	TF2 *fff2 = new TF2("fff2","[0] + y*[1] + sqrt(2*y*(1+y))*[2]*cos(0.017453*x)  + y*[3]*cos(0.034906*x)", 0, 360, 0.25, 0.6);
		//	TF2 *fff2 = new TF2("fff2","[0] + y*[1] + sqrt(2*y*(1+y))*[2]*cos(0.017453*x)  + y*[3]*cos(0.034906*x)", 0, 360, 0.20, 0.90);        // VK
		TF2 *fff2 = new TF2("fff2","[0] + y*[1] + sqrt(2*y*(1+y))*[2]*cos(x*pi/180)  + y*[3]*cos(2*x*pi/180)", 11.25, 348.78, 0.26, 0.80 );       // VK
		//	TF2 *fff2 = new TF2("fff2","[0] + y*[1] + sqrt(2*y*(1+y))*[2]*cos(x*pi/180)  + y*[3]*cos(2*x*pi/180)", 0, 360, 0.2, 0.7);        // VK
 	
 		TGraphErrors * sigL_change = new TGraphErrors(); 
 		TGraphErrors * sigT_change = new TGraphErrors(); 
 		TGraphErrors * sigLT_change = new TGraphErrors(); 
 		TGraphErrors * sigTT_change = new TGraphErrors(); 
  
 		
 
 
 
 		/*--------------------------------------------------*/
 		/// Fit 1
 		/// 
 
		// 		cout  << endl << "/*--------------------------------------------------*/" << endl;
		//		cout << " Fitting Step 1" << endl;
		//		cout << " Fit L and T, while Fix LT and TT" << endl;
 
 		/// Set parameter 0 and 1
 
		/**		 
		fff2->SetParameter(0, 1);
  		fff2->SetParLimits(0, 0, 5);
		
 		fff2->SetParameter(1, 0.1);	       
  		fff2->SetParLimits(1, 0, 3);
		**/
 		/// Fix parameter 2 and 3
 //		fff2->FixParameter(1, 0.0);
		

		//VK
		/*	
 		fff2->FixParameter(2, 0.0);
 		fff2->FixParameter(3, 0.0);
		*/
		g_plot_err->Fit("fff2", "MR");
 
 
 
 
 
 		sigL_change->SetPoint(sigL_change->GetN(), sigL_change->GetN()+1, fff2->GetParameter(1));
 		sigL_change->SetPointError(sigL_change->GetN()-1, 0, fff2->GetParError(1));
    
		sigT_change->SetPoint(sigT_change->GetN(), sigT_change->GetN()+1, fff2->GetParameter(0));
		sigT_change->SetPointError(sigT_change->GetN()-1, 0, fff2->GetParError(0));
 
		
 		/*--------------------------------------------------*/
 		/// Fit 2
 		///
						 
		//	cout  << endl << "/*--------------------------------------------------*/" << endl;
		//	cout << " Fitting Step 2" << endl;
		//	cout << " Fit LT, while Fix L, T and TT" << endl;
 
 		/// Fix parameter 0 and 1 and 3
		
		//VK
		/*
  		fff2->FixParameter(0, fff2->GetParameter(0));
  		fff2->FixParameter(1, fff2->GetParameter(1));
  		fff2->FixParameter(3, fff2->GetParameter(3));
		*/
 
 		/// Set parameter 2
		/*
		fff2->ReleaseParameter(2);
 		fff2->ReleaseParameter(3);
		*/
		// 		fff2->SetParameter(2, 0.0);
		/** 	
			fff2->SetParLimits(2, -0.1, 0.1);
		**/
		//		g_plot_err->Fit("fff2", "MR");
 
		
 		/*--------------------------------------------------*/
 		/// Fit 3
 		///
		
		//		cout  << endl << "/*--------------------------------------------------*/" << endl;
		//	cout << " Fitting Step 3" << endl;
		//	cout << " Fit L and T, while Fix LT and TT" << endl;
		
 		/// Set parameter 0 and 1
		//VK
		/*
 		fff2->ReleaseParameter(0);
 		fff2->ReleaseParameter(1);
		*/
		//	fff2->SetParameter(0, fff2->GetParameter(0));
		//	fff2->SetParameter(1, fff2->GetParameter(1));
 
 		/// Fix parameter 2 and 3
		/*
  		fff2->FixParameter(2, fff2->GetParameter(2));
  		fff2->FixParameter(3, fff2->GetParameter(3));
		*/
		//	g_plot_err->Fit("fff2", "MR");
		/*
 		sigL_change->SetPoint(sigL_change->GetN(), sigL_change->GetN()+1, fff2->GetParameter(1));
 		sigL_change->SetPointError(sigL_change->GetN()-1, 0, fff2->GetParError(1));
    
         sigT_change->SetPoint(sigT_change->GetN(), sigT_change->GetN()+1, fff2->GetParameter(0));
         sigT_change->SetPointError(sigT_change->GetN()-1, 0, fff2->GetParError(0));
		*/
		
 
 
 
 		/*--------------------------------------------------*/
 		/// Fit 4
 		///
		//	cout  << endl << "/*--------------------------------------------------*/" << endl;
		//	cout << " Fitting Step 4" << endl;
		//	cout << " Fit TT, while Fix T, L and LT" << endl;
 
 		// Fix parameter 0 and 1
		//VK
		/*	
  		fff2->FixParameter(0, fff2->GetParameter(0));
  		fff2->FixParameter(1, fff2->GetParameter(1));
  		fff2->FixParameter(2, fff2->GetParameter(2));
		*/
 		
 		/// Set parameter 2 and 3
 
 		// fff2->ReleaseParameter(2);
		//VK
		//	fff2->ReleaseParameter(3);
 
		//		fff2->SetParameter(3, fff2->GetParameter(3));       //Updated on May 2
		/** 
		fff2->SetParLimits(3, -0.1, 0.1);
		**/
		//	g_plot_err->Fit("fff2", "MR");
 
		
 		/*--------------------------------------------------*/
 		/// Fit 5
 		///
		//	cout  << endl << "/*--------------------------------------------------*/" << endl;
		//	cout << " Fitting Step 5" << endl;
		//	cout << " Fit T and L, while Fix LT and TT" << endl;
		 
 		/// Fix parameter 0 and 1
		//VK
		/*
		fff2->ReleaseParameter(0);
 		fff2->ReleaseParameter(1);
		
 		fff2->SetParameter(0, fff2->GetParameter(0));
 		fff2->SetParameter(1, fff2->GetParameter(1));
		*/
 		/// Fix parameter 2 and 3
 
  		//fff2->FixParameter(2, fff2->GetParameter(2));
		//	fff2->FixParameter(3, fff2->GetParameter(3));
 
		//   		g_plot_err->Fit("fff2", "MR");
		
 
 
 // 		/*--------------------------------------------------*/
 // 		/// Fit 6
 // 		///
 // 		cout  << endl << "--------------------------------------------------" << endl;
 // 		cout << " Fitting Step 4" << endl;
 // 		cout << " Fit LT and TT, while Fix L and T" << endl;
 // 
 // 		/// Fix parameter 0 and 1
 // 
		//  		fff2->FixParameter(0, fff2->GetParameter(0));
		//		fff2->FixParameter(1, fff2->GetParameter(1));
 // 
 // 
 //  		fff2->SetParLimits(0, fff2->GetParameter(0) - fff2->GetParError(0), fff2->GetParameter(0) + fff2->GetParError(0));
 //  		fff2->SetParLimits(1, fff2->GetParameter(1) - fff2->GetParError(1), fff2->GetParameter(0) + fff2->GetParError(0));
 // 
 // 
 //		fff2->ReleaseParameter(2);
		//  		fff2->ReleaseParameter(3);
 // 
 //  		fff2->SetParameter(2, fff2->GetParameter(2));
 //  		fff2->SetParameter(3, fff2->GetParameter(3));
 // 
 //   		g_plot_err->Fit("fff2", "MR");
 
 		/*--------------------------------------------------*/
 		/// Last Step
 		///
 		cout  << endl << "/*--------------------------------------------------*/" << endl;
 		cout << " Last Step" << endl;
 		cout << " Fit All" << endl;
 
 		/// Fix parameter 0 and 1
 
		/*		
 		fff2->ReleaseParameter(0);
 		fff2->ReleaseParameter(1);
 		fff2->ReleaseParameter(2);
 		fff2->ReleaseParameter(3);
		*/
		//				fff2->SetParLimits(0, 0, 1);
		//		  		fff2->SetParLimits(1, 0, 1);
		
		//		   		fff2->SetParLimits(2, -1, 1);
		//		  		fff2->SetParLimits(3, -1, 1);
		
		/*
   		fff2->FixParameter(0, fff2->GetParameter(0));
   		fff2->FixParameter(1, fff2->GetParameter(1));
   		fff2->FixParameter(2, fff2->GetParameter(2));
   		fff2->FixParameter(3, fff2->GetParameter(3));
		*/
 
 //  		fff2->SetParLimits(0, 0, 0.3);
 
 // 		fff2->FixParameter(1, 0.0);
 
		//   		fff2->SetParLimits(0, fff2->GetParameter(0) - fff2->GetParError(0), fff2->GetParameter(0) + fff2->GetParError(0));
		//    		fff2->SetParLimits(1, fff2->GetParameter(1) - fff2->GetParError(1), fff2->GetParameter(0) + fff2->GetParError(0));
  
		//   		fff2->SetParLimits(2, fff2->GetParameter(2) - fff2->GetParError(2), fff2->GetParameter(2) + fff2->GetParError(2));
		//    		fff2->SetParLimits(3, fff2->GetParameter(3) - fff2->GetParError(3), fff2->GetParameter(3) + fff2->GetParError(3));
		
   		fff2->SetParameter(0, fff2->GetParameter(0));
   		fff2->SetParameter(1, fff2->GetParameter(1));
   		fff2->SetParameter(2, fff2->GetParameter(2));
   		fff2->SetParameter(3, fff2->GetParameter(3));
		
 		
   		g_plot_err->Fit("fff2", "MR");
  
 
 
 		sigL_change->SetPoint(sigL_change->GetN(), sigL_change->GetN()+1, fff2->GetParameter(1));
 		sigL_change->SetPointError(sigL_change->GetN()-1, 0, fff2->GetParError(1));
    
		sigT_change->SetPoint(sigT_change->GetN(), sigT_change->GetN()+1, fff2->GetParameter(0));
		sigT_change->SetPointError(sigT_change->GetN()-1, 0, fff2->GetParError(0));
 
 		c2->Update();
 
		c2->cd(i+1);
		/*	
		TPad* pad = (TPad*)c2->GetPad(i+1);
		pad->SetLeftMargin(0.08);
		pad->SetBottomMargin(0.07);
		pad->SetTopMargin(0.05);
		pad->SetRightMargin(0.02);
		*/
 // 		g1->Draw("A*");
 
  		g1->SetMarkerStyle(5);

 // //		g1->Fit("lo_eps_fit", "R");
 // 
 // // 
 // //  		f1->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
 // //  		f2->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
 // //
 // 
		// 		g1->SetMarkerColor(4);
  		g2->SetMarkerColor(2);
  		g2->SetLineColor(2);
  		g2->SetMarkerStyle(4);

  		g3->SetLineColor(3);
  		g3->SetMarkerStyle(29);


   		TMultiGraph *g = new TMultiGraph();
		g->Add(g1);
		g->Add(g2);
		g->Add(g3);
 
 		g->Draw("AP");
// 		g->GetHistogram()->SetMinimum(0.0);
// 		g->GetHistogram()->SetMaximum(2e-7);
//  
// 		g->SetTitle("Unseprated #sigma");
// 
		g->GetXaxis()->SetTitle("#phi (deg)");
 		g->GetXaxis()->CenterTitle();
		// 		g->GetYaxis()->SetTitle("Unseparated X-Section");
 		g->GetYaxis()->SetTitle("d#sigma/dtd#phi (#mub/GeV^{2})");
		// 		g->GetYaxis()->SetTitleOffset(1.6);
 		g->GetYaxis()->CenterTitle();
		
		
 		f1->FixParameter(0, fff2->GetParameter(0));
 		f1->FixParameter(1, fff2->GetParameter(1));
 		f1->FixParameter(2, fff2->GetParameter(2));
 		f1->FixParameter(3, fff2->GetParameter(3));
		
		//		f1->FixParameter(2, 0.0);
		//		f1->FixParameter(3, 0.0);
 		
 		f2->FixParameter(0, fff2->GetParameter(0));
 		f2->FixParameter(1, fff2->GetParameter(1));
 		f2->FixParameter(2, fff2->GetParameter(2));
 		f2->FixParameter(3, fff2->GetParameter(3));
 
 //		f2->FixParameter(2, 0.0);
 //		f2->FixParameter(3, 0.0);
 
		f3->FixParameter(0, fff2->GetParameter(0));
 		f3->FixParameter(1, fff2->GetParameter(1));
 		f3->FixParameter(2, fff2->GetParameter(2));
 		f3->FixParameter(3, fff2->GetParameter(3));
 
		f1->SetLineColor(1);
		f2->SetLineColor(2);	 		
		f3->SetLineColor(3);	 		
  
 //		TF1* ftest = new TF1("ftest", "sin(3.14*90/180)", 0, 1);
 
 //		ftest->Draw();
 
 // 		/*--------------------------------------------------*/
 // 		///Test 
 // 
 // 		Float_t test_l, test_t;
 // 
 // 		test_l = 0.7515E-8;
 // 		test_t = 0.1082E-8;
 // 
 // 
 // 		f1->FixParameter(0, test_l);
 // 		f1->FixParameter(1, test_t);
 // 
 // 		f2->FixParameter(0, test_l);
 // 		f2->FixParameter(1, test_t);
 
		f1->Draw("same");	
		f2->Draw("same");	
		f3->Draw("same");	
 
 
 //		cout << f1->GetParameter(0) << endl;
 //		exit(0);
 /*
 		TText*fit_status = new TText();
 		fit_status->SetTextSize(0.04);
 		fit_status->DrawTextNDC(0.15, 0.85, "Q2 = " + q2_set);
		fit_status->DrawTextNDC(0.15, 0.80, "Fit Status: " + gMinuit->fCstatu);
 */
		/**
 		if(TMath::MaxElement(g2->GetN(),g2->GetY()) > TMath::MaxElement(g1->GetN(),g1->GetY())) {
 
 			cout  << "!!!!!!!!!!!!!!!  " << endl;
 			g1->SetMaximum(TMath::MaxElement(g2->GetN(),g2->GetY())*1.1);
 
 		}
 
 		if(TMath::MinElement(g2->GetN(),g2->GetY()) < TMath::MinElement(g1->GetN(),g1->GetY())) {
 
 			cout  << "!!!!!!!!!!!!!!!  " << endl;
 			g1->SetMinimum(TMath::MinElement(g2->GetN(),g2->GetY())*0.9);
 
			}**/
// 
// 
// 		Float_t sig_l = (ave_sig_hi-ave_sig_lo)/eps_diff;
// 
// 		sig_u_diff->SetPoint(sig_u_diff->GetN(), u_list[i], sig_l );
// 
// 		Float_t sig_l_err =  sig_l * sqrt( pow(err_sig_hi/ave_sig_hi,2) + pow(err_sig_lo/ave_sig_lo,2));
// 
// 		sig_u_diff->SetPointError(sig_u_diff->GetN()-1, 0, sig_l_err);

// 		fff2->GetParameter(0)
//      fff2->GetParameter(1)
//      fff2->GetParameter(2)
//      fff2->GetParameter(3)
 
 		Float_t sig_l, sig_t, sig_lt, sig_tt;
 		Float_t sig_l_err, sig_t_err, sig_lt_err, sig_tt_err;
 
 
 
 
 
 		///*--------------------------------------------------*/
 		// Standard LT seperation equation
 		// Parameter assignment
 		// par 1: for sigT
 		// par 2: for sigL
 		// par 3: for sigTT
 		// par 4: for sigLT
 
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
 
 //		exit(0);
 
 		
 
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
	
	

	c2->Print("lt_sep_plots/money_"+q2_set + ".pdf");
	c2->Print("lt_sep_plots/money_"+q2_set + ".root");

	TCanvas* c3 = new TCanvas();

	sig_L_g->GetXaxis()->SetTitle("t (GeV^{2})");
	sig_L_g->GetXaxis()->CenterTitle();
	sig_L_g->GetYaxis()->SetTitle("d#sigma/dtd#phi (#mub/GeV^{2})");
	sig_L_g->GetYaxis()->CenterTitle();
	sig_L_g->Draw("a*");
	c3->Print("lt_sep_plots/sigL_"+ q2_set + ".png");

	sig_T_g->GetXaxis()->SetTitle("t (GeV^{2})");
	sig_T_g->GetXaxis()->CenterTitle();
	sig_T_g->GetYaxis()->SetTitle("d#sigma/dtd#phi (#mub/GeV^{2})");
	sig_T_g->GetYaxis()->CenterTitle();
	sig_T_g->Draw("a*");
	c3->Print("lt_sep_plots/sigT_"+ q2_set + ".png");

	sig_LT_g->GetXaxis()->SetTitle("t (GeV^{2})");
	sig_LT_g->GetXaxis()->CenterTitle();
	sig_LT_g->GetYaxis()->SetTitle("d#sigma/dtd#phi (#mub/GeV^{2})");
	sig_LT_g->GetYaxis()->CenterTitle();
	sig_LT_g->Draw("a*");
	c3->Print("lt_sep_plots/sigLT_"+ q2_set + ".png");

	sig_TT_g->GetXaxis()->SetTitle("t (GeV^{2})");
	sig_TT_g->GetXaxis()->CenterTitle();
	sig_TT_g->GetYaxis()->SetTitle("d#sigma/dtd#phi (#mub/GeV^{2})");
	sig_TT_g->GetYaxis()->CenterTitle();
	sig_TT_g->Draw("a*");
	c3->Print("lt_sep_plots/sigTT_"+ q2_set + ".png");





	delete c1;
	delete c2;
	delete c3;



// 	c1->Print( "unsep_"+ q2_set + ".png");
// 
// 	c1->Clear();
// 
// 	sig_u_lo->Draw("A*");
// 
// 	c1->Print( "sigma_u_"+ q2_set + "_"+ eps_lo + ".png");
// 
// 	c1->Clear();
// 
// 	sig_u_hi->Draw("A*");
// 
// 	c1->Print( "sigma_u_"+ q2_set + "_"+ eps_hi + ".png");
// 
// 	c1->Clear();
// 
// 	sig_u_diff->Draw("A*");
// 
// 	c1->Print( "sigma_u_diff_"+ q2_set + "_"+ eps_hi + ".png");
//
	file_out.close();

}





/*--------------------------------------------------*/

Double_t fun_exp(Double_t *x, Double_t *par){

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);

    Double_t sig =  par[0]*exp(par[1]*xx)+ par[2];


	return sig;
}





//*--------------------------------------------------*/
/// Low epsilon drawing function

Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par){

//	cout << lo_eps << endl;
//	exit(0);

	Float_t eps = lo_eps;

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);
	Double_t xs;

//	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(2*xx) + par[3]*cos(xx);
	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180);
//	xs = par[0] +  eps*par[1];

	return xs;
}



/*--------------------------------------------------*/
/// High epsilon drawing function

Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par){


	Float_t eps = hi_eps;

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);
	Double_t xs;

	xs = par[0] +  eps*par[1]  + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180);
//	xs = par[0] +  eps*par[1];

	return xs;
}
/*--------------------------------------------------*/
/// Mid epsilon drawing function

Double_t LT_sep_x_mi_fun(Double_t *x, Double_t *par){


	Float_t eps = mi_eps;

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);
	Double_t xs;

	xs = par[0] +  eps*par[1]  + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180);
//	xs = par[0] +  eps*par[1];

	return xs;
}



