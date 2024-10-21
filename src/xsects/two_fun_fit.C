#include <vector>
#include <iostream>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"

#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/Chi2FCN.h"
#include "TH1.h"
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

using namespace std;

//Float_t pi = 3.1415926;
Float_t pi = 3.1415926;

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




TGraphErrors* g_sig_l;
TGraphErrors* g_sig_t;

void single_setting(TString q2_set);
Double_t fun_exp(Double_t *x, Double_t *par);

Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par);
Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par);


void two_fun_fit() {


	g_sig_l = new TGraphErrors();
	g_sig_t = new TGraphErrors();

	single_setting("160");
//	single_setting("245");
// 
// 	TCanvas* c2 = new TCanvas();
// 
// 	c2->Update();
// 	
// 	g_sig_l->Draw("A*");
// 
// 	gStyle->SetOptFit(1);
// 
// 	TF1* f1 = new TF1("f1", fun_exp, 0, 0.5, 3);
// 
// //	f1->SetParameter();
// 
// 	g_sig_l->Fit("f1", "R");
// 
// 	c2->Print("sig_L.png");
// 
// 	c2->Clear();
// 	
// 	g_sig_t->Draw("A*");
// 	g_sig_t->Fit("f1", "R");
// 
// 	c2->Print("sig_T.png");
// 


}


///*--------------------------------------------------*/

void single_setting(TString q2_set){


	TCanvas* c1 =  new TCanvas("c1", "c1", 1200, 400); 

	c1->Divide(3, 1);

	TString eps_lo;
	TString eps_hi;

	Float_t eps_diff;

	if (q2_set == "245" ) {

		eps_lo = "27";
		eps_hi = "55";
	
		eps_diff = 0.55-0.27;


	} if (q2_set == "160") {

		eps_lo = "32";
		eps_hi = "59";

		eps_diff = 0.59-0.32;

	}


	TGraphErrors* sig_u_lo = new TGraphErrors();
	TGraphErrors* sig_u_hi = new TGraphErrors();
	TGraphErrors* sig_u_diff = new TGraphErrors();



	TString file_name_1;
	file_name_1 = "x_unsep.pl_" + q2_set + "_" + eps_lo;

 	TString file_name_2;
 	file_name_2 = "x_unsep.pl_" + q2_set + "_" + eps_hi;

	TNtuple* n1 = new TNtuple("n1", "n1", "x/F:dx:x_mod:eps:theta:phi:t:u:w:Q2");
	n1->ReadFile(file_name_1);
 
 	TNtuple* n2 = new TNtuple("n2", "n2", "x/F:dx:x_mod:eps:theta:phi:t:u:w:Q2");
 	n2->ReadFile(file_name_2);


//	n1->Draw("t");

	Float_t uu;


	n1->SetBranchAddress("u", &uu);

	Int_t N = n1->GetEntries();
	
	vector<Float_t> t_list;



	for (Int_t i = 0; i < N; ++i) {

    	n1->GetEntry(i);
		
  		if(!binary_search(t_list.begin(), t_list.end(), uu)) {

			cout << uu << endl << endl;

			t_list.push_back(uu);



		}

	}

	
//	for (Int_t i = 0; i < t_list.size(); i++) {

//		cout << "asdasdasd "<< t_list[i] << endl;



	for (Int_t i = 0; i < 1; i++) {

		TString upp;


		if (i == 0) {

			upp.Form("u < %lf", t_list[i]+0.01); 

		} else {

			upp.Form("u> %lf && u < %lf ", t_list[i-1]+0.01, t_list[i]+0.01); 

		}



 		cout << "asdasd "<< upp << endl;
// 
// 
 	 	c1->cd(i+1);
// 
// //		n1->Draw("t");
// 
// 

        n1->Draw("x:phi:dx", upp, "goff");
   		TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());

//		TF1* f1 = new TF1("lo_eps_fit", LT_sep_x_lo_fun, 0, 360, 4); 
//		TF1* f2 = new TF1("hi_eps_fit", LT_sep_x_hi_fun, 0, 360, 4); 

		TF1* f1 = new TF1("lo_eps_fit", LT_sep_x_lo_fun, 0, 360, 2); 
		TF1* f2 = new TF1("hi_eps_fit", LT_sep_x_hi_fun, 0, 360, 2); 

	  	TGraph* g1 = (TGraph*) g1_tmp->Clone("g1");

		Float_t ave_sig_lo = g1->GetMean(2);
		Float_t err_sig_lo = g1->GetRMS(2);

		sig_u_lo->SetPoint(sig_u_lo->GetN(), t_list[i], ave_sig_lo);
		sig_u_lo->SetPointError(sig_u_lo->GetN()-1, 0, err_sig_lo);

		g_sig_t->SetPoint(g_sig_t->GetN(),        t_list[i], ave_sig_lo);
		g_sig_t->SetPointError(g_sig_t->GetN()-1,         0, err_sig_lo);


// 	 	n2->Draw("x:phi", upp, "*");
// 	 	TGraph* g2_tmp = (TGraph*) c1->GetPad(i+1)->GetListOfPrimitives()->FindObject("Graph");
// 
        n2->Draw("x:phi:dx", upp, "goff");
   		TGraphErrors* g2_tmp = new TGraphErrors(n2->GetSelectedRows(), n2->GetV2(), n2->GetV1(), 0, n2->GetV3());


	   	TGraph* g2 = (TGraph*) g2_tmp->Clone("g2");




  		ROOT::Math::WrappedMultiTF1 wfB1(*f1,1);
  		ROOT::Math::WrappedMultiTF1 wfB2(*f2,1);

  		ROOT::Fit::DataOptions opt; 

  		ROOT::Fit::DataRange rangeB1;
  		ROOT::Fit::DataRange rangeB2; 
  
  		ROOT::Fit::BinData dataB1(opt,rangeB1);
  		ROOT::Fit::BinData dataB2(opt,rangeB2);


		rangeB1.SetRange(0,360);
  		rangeB2.SetRange(0,360);

//	  	ROOT::Fit::BinData dataB1; 
  		ROOT::Fit::FillData(dataB1, g1);

//  		ROOT::Fit::BinData dataB2; 
  		ROOT::Fit::FillData(dataB2, g2);

	  	ROOT::Fit::Chi2Function chi2_B1(dataB1, wfB1);
  		ROOT::Fit::Chi2Function chi2_B2(dataB2, wfB2);

  		GlobalChi2 globalChi2(chi2_B1, chi2_B2);

  		ROOT::Fit::Fitter fitter;
 
//         const int Npar = 4; 
// 		double par0[Npar] = { 0.0, 0.0, 0.0, 0.0};
 
        const int Npar = 2; 
		double par0[Npar] = { 0.0, 0.0};


		fitter.Config().SetMinimizer("Minuit","Minimize"); 
 
		fitter.FitFCN(2,globalChi2,par0,dataB1.Size()+dataB2.Size());
  		ROOT::Fit::FitResult result = fitter.Result();
 		result.Print(std::cout);
 		f1->SetParameters(result.GetParams());
 		f2->SetParameters(result.GetParams());


  		//hSB->GetListOfFunctions()->Add(fSB);


		Float_t ave_sig_hi = g2->GetMean(2);
		Float_t err_sig_hi = g2->GetRMS(2);

		sig_u_hi->SetPoint(sig_u_hi->GetN(), t_list[i], ave_sig_hi);
		sig_u_hi->SetPointError(sig_u_hi->GetN()-1, 0, err_sig_hi);

	
		g1->Draw("A*");

//		g1->Fit("lo_eps_fit", "R");

// 
//  		f1->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
//  		f2->SetParameters(0.2582E-07, -0.1265E-08, 0.0, 0.0);
//

		g2->SetMarkerColor(2);
		g2->SetLineColor(2);
	 	g2->Draw("*");

		f1->SetLineColor(1);
		f2->SetLineColor(2);	

		
		f1->Draw("same");	
		f2->Draw("same");	

		


	
		if(TMath::MaxElement(g2->GetN(),g2->GetY()) > TMath::MaxElement(g1->GetN(),g1->GetY())) {

			cout  << "!!!!!!!!!!!!!!!  " << endl;
			g1->SetMaximum(TMath::MaxElement(g2->GetN(),g2->GetY())*1.1);

		}

		if(TMath::MinElement(g2->GetN(),g2->GetY()) < TMath::MinElement(g1->GetN(),g1->GetY())) {

			cout  << "!!!!!!!!!!!!!!!  " << endl;
			g1->SetMinimum(TMath::MinElement(g2->GetN(),g2->GetY())*0.9);

		}


		Float_t sig_l = (ave_sig_hi-ave_sig_lo)/eps_diff;

		sig_u_diff->SetPoint(sig_u_diff->GetN(), t_list[i], sig_l );

		Float_t sig_l_err =  sig_l * sqrt( pow(err_sig_hi/ave_sig_hi,2) + pow(err_sig_lo/ave_sig_lo,2));

		sig_u_diff->SetPointError(sig_u_diff->GetN()-1, 0, sig_l_err);

		g_sig_l->SetPoint(g_sig_l->GetN(), t_list[i], sig_l);
		g_sig_l->SetPointError(g_sig_l->GetN()-1, 0, sig_l_err);




	}
// 
// 
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
}





/*--------------------------------------------------*/

Double_t fun_exp(Double_t *x, Double_t *par){

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);

    Double_t sig =  par[0]*exp(par[1]*xx)+ par[2];


	return sig;
}





/*--------------------------------------------------*/

Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par){


	Float_t eps = 0.27;

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);
	Double_t xs;

//	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(2*xx) + par[3]*cos(xx);
//	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180);
	xs = par[0] +  eps*par[1];

	return xs;
}



/*--------------------------------------------------*/

Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par){


	Float_t eps = 0.55;

    Float_t xx =x[0];

//    Double_t f = par[0]*sin(xx);
	Double_t xs;

//	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))+par[2]*cos(xx*pi/180) + eps*par[3]*cos(2*xx*pi/180);
	xs = par[0] +  eps*par[1];

	return xs;
}




