#include <vector>

void x_sep_plot() {

	single_setting("160");
	single_setting("245");

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
//	c2->Print("sig_T.png");



}


///*--------------------------------------------------*/

void single_setting(TString q2_set){


	TCanvas* c1 =  new TCanvas("c1", "c1", 800, 800); 

	c1->Divide(2, 2, 0.003, 0.003);

 	TString file_name_tot;
 	file_name_tot  = "x_sep.pl_" + q2_set;
 
  	TString file_sigl_fit;
  	file_sigl_fit  = "sigl_fit.pl_" + q2_set;

  	TString file_sigt_fit;
  	file_sigt_fit  = "sigt_fit.pl_" + q2_set;

 	TString file_siglt_fit;
  	file_siglt_fit = "siglt_fit.pl_" + q2_set;

 	TString file_sigtt_fit;
  	file_sigtt_fit = "sigtt_fit.pl_" + q2_set;


 
 	TNtuple* n_tot  = new TNtuple("n1", "n1", "sigt:dsigt:sigl:dsigl:sigtt:dsigtt:siglt:dsiglt:chi2:u:w:Q2");
 	n_tot->ReadFile(file_name_tot);
  
 	TNtuple* n_sigl = new TNtuple("sigl",   "sigl", "sig:u:prv_sig");
 	n_sigl->ReadFile(file_sigl_fit);

 	TNtuple* n_sigt = new TNtuple("sigt",   "sigt", "sig:u:prv_sig");
 	n_sigt->ReadFile(file_sigt_fit);

 	TNtuple* n_siglt = new TNtuple("siglt", "siglt", "sig:u:prv_sig");
 	n_siglt->ReadFile(file_siglt_fit);
 
	TNtuple* n_sigtt = new TNtuple("sigtt", "sigtt", "sig:u:prv_sig");
 	n_sigtt->ReadFile(file_sigtt_fit);


// 	TGraphErrors* g_sigl = new TGraphErrors(); 
// 	TGraphErrors* g_sigt = new TGraphErrors(); 
// 	TGraphErrors* g_siglt = new TGraphErrors(); 
// 	TGraphErrors* g_sigtt = new TGraphErrors(); 
// 
// 
// 	Int_t entries = n_tot->GetEntries(); 
// 
// 	Float_t sigl, sigt, siglt, sigtt;
// 
// 	n_sigl->SetBranchAddress("sig", &sigl);
// 	n_sigt->SetBranchAddress("sig", &sigt);
// 	n_siglt->SetBranchAddress("sig", &siglt);
// 	n_sigtt->SetBranchAddress("sig", &sigtt);



    n_tot->Draw("sigt:u:dsigt", "", "goff");
 	TGraphErrors* g1_tmp = new TGraphErrors(n_tot->GetSelectedRows(), n_tot->GetV2(), n_tot->GetV1(), 0, n_tot->GetV3());
  	TGraphErrors* g_sigt = (TGraphErrors*) g1_tmp->Clone("g1");

	
    n_sigt->Draw("sig:u:prv_sig", "", "goff");
 	TGraphErrors* g2_tmp = new TGraphErrors(n_sigt->GetSelectedRows(), n_sigt->GetV2(), n_sigt->GetV1(), 0, 0);
  	TGraphErrors* g_sigt_fit = (TGraphErrors*) g2_tmp->Clone("g2");

	TGraphErrors* g3_tmp = new TGraphErrors(n_sigt->GetSelectedRows(), n_sigt->GetV2(), n_sigt->GetV3(), 0, 0);
  	TGraphErrors* g_sigt_prv = (TGraphErrors*) g3_tmp->Clone("g3");


	c1->cd(1);

	gPad->SetLeftMargin(0.1);
	gPad->SetRightMargin(0.03);


	g_sigt->SetMaximum(1);
	g_sigt->SetMinimum(0);


 	g_sigt_fit->SetMarkerColor(2);
 	g_sigt_fit->SetLineColor(2);
//	g_sigt_fit->Draw("same*l");

	g_sigt_prv->SetMarkerColor(4);
	g_sigt_prv->SetMarkerStyle(21);
//	g_sigt_prv->Draw("samep");


	TMultiGraph *mg_sigt = new TMultiGraph();

	mg_sigt->Add(g_sigt, "*");
	mg_sigt->Add(g_sigt_fit, "*l");
	mg_sigt->Add(g_sigt_prv, "p");

	mg_sigt->Draw("a");

	mg_sigt->SetTitle("Sig T");
// 	g_sigt->Draw("A*");
	
	mg_sigt->GetXaxis()->SetTitle("-u");
	mg_sigt->GetXaxis()->CenterTitle();
 




	c1->cd(2);
	gPad->SetLeftMargin(0.1);
	gPad->SetRightMargin(0.03);

    n_tot->Draw("sigl:u:dsigl", "", "goff");
 	TGraphErrors* g1_tmp = new TGraphErrors(n_tot->GetSelectedRows(), n_tot->GetV2(), n_tot->GetV1(), 0, n_tot->GetV3());
  	TGraphErrors* g_sigl = (TGraphErrors*) g1_tmp->Clone("g1");

	
    n_sigl->Draw("sig:u:prv_sig", "", "goff");
 	TGraphErrors* g2_tmp = new TGraphErrors(n_sigl->GetSelectedRows(), n_sigl->GetV2(), n_sigl->GetV1(), 0, 0);
  	TGraphErrors* g_sigl_fit = (TGraphErrors*) g2_tmp->Clone("g2");

	TGraphErrors* g3_tmp = new TGraphErrors(n_sigl->GetSelectedRows(), n_sigl->GetV2(), n_sigl->GetV3(), 0, 0);
  	TGraphErrors* g_sigl_prv = (TGraphErrors*) g3_tmp->Clone("g3");


// 	g_sigl->SetTitle("Sig L");
//  
// 
// 	g_sigl->SetMaximum(1);
// 	g_sigl->SetMinimum(0);
// 


//	g_sigl->Draw("A*");

//	g_sigl->GetXaxis()->SetTitle("-u");
//	g_sigl->GetXaxis()->CenterTitle();

// 	g_sigl->SetMaximum(0.2);
 
 	g_sigl_fit->SetMarkerColor(2);
 	g_sigl_fit->SetLineColor(2);
//	g_sigl_fit->Draw("same*l");

	g_sigl_prv->SetMarkerColor(4);
	g_sigl_prv->SetMarkerStyle(21);
//	g_sigl_prv->Draw("samep");


	TMultiGraph *mg_sigl = new TMultiGraph();

	mg_sigl->Add(g_sigl, "*");
	mg_sigl->Add(g_sigl_fit, "*l");
	mg_sigl->Add(g_sigl_prv, "p");

	mg_sigl->Draw("a");

	mg_sigl->SetTitle("Sig L");
	
	mg_sigl->GetXaxis()->SetTitle("-u");
	mg_sigl->GetXaxis()->CenterTitle();



	c1->cd(3);
	gPad->SetLeftMargin(0.1);
	gPad->SetRightMargin(0.03);

    n_tot->Draw("sigtt:u:dsigtt", "", "goff");
 	TGraphErrors* g1_tmp = new TGraphErrors(n_tot->GetSelectedRows(), n_tot->GetV2(), n_tot->GetV1(), 0, n_tot->GetV3());
  	TGraphErrors* g_sigtt = (TGraphErrors*) g1_tmp->Clone("g1");

	
    n_sigtt->Draw("sig:u:prv_sig", "", "goff");
 	TGraphErrors* g2_tmp = new TGraphErrors(n_sigtt->GetSelectedRows(), n_sigtt->GetV2(), n_sigtt->GetV1(), 0, 0);
  	TGraphErrors* g_sigtt_fit = (TGraphErrors*) g2_tmp->Clone("g2");

	TGraphErrors* g3_tmp = new TGraphErrors(n_sigtt->GetSelectedRows(), n_sigtt->GetV2(), n_sigtt->GetV3(), 0, 0);
  	TGraphErrors* g_sigtt_prv = (TGraphErrors*) g3_tmp->Clone("g3");


// 	g_sigtt->SetTitle("Sig TT");
// 	g_sigtt->GetXaxis()->CenterTitle();
// 
// 
// 	g_sigtt->SetMaximum(0.2);
//  	g_sigtt->Draw("A*");
// 	
// 	g_sigtt->GetXaxis()->SetTitle("-u");
// 
//   	g_sigtt->SetMaximum(0.3);
//  	g_sigtt->SetMinimum(-0.3);
// 


 	g_sigtt_fit->SetMarkerColor(2);
 	g_sigtt_fit->SetLineColor(2);
//	g_sigtt_fit->Draw("same*l");


	g_sigtt_prv->SetMarkerColor(4);
	g_sigtt_prv->SetMarkerStyle(21);
//	g_sigtt_prv->Draw("samep");


	TMultiGraph *mg_sigtt = new TMultiGraph();

	mg_sigtt->Add(g_sigtt, "*");
	mg_sigtt->Add(g_sigtt_fit, "*l");
	mg_sigtt->Add(g_sigtt_prv, "p");

	mg_sigtt->Draw("a");

	mg_sigtt->SetTitle("Sig TT");
	
	mg_sigtt->GetXaxis()->SetTitle("-u");
	mg_sigtt->GetXaxis()->CenterTitle();






	c1->cd(4);
	gPad->SetLeftMargin(0.1);
	gPad->SetRightMargin(0.03);


    n_tot->Draw("siglt:u:dsiglt", "", "goff");
 	TGraphErrors* g1_tmp = new TGraphErrors(n_tot->GetSelectedRows(), n_tot->GetV2(), n_tot->GetV1(), 0, n_tot->GetV3());
  	TGraphErrors* g_siglt = (TGraphErrors*) g1_tmp->Clone("g1");

	
    n_siglt->Draw("sig:u:prv_sig", "", "goff");
 	TGraphErrors* g2_tmp = new TGraphErrors(n_siglt->GetSelectedRows(), n_siglt->GetV2(), n_siglt->GetV1(), 0, 0);
  	TGraphErrors* g_siglt_fit = (TGraphErrors*) g2_tmp->Clone("g2");
 
	TGraphErrors* g3_tmp = new TGraphErrors(n_siglt->GetSelectedRows(), n_siglt->GetV2(), n_siglt->GetV3(), 0, 0);
  	TGraphErrors* g_siglt_prv = (TGraphErrors*) g3_tmp->Clone("g3");


// 	g_siglt->SetTitle("Sig LT");
// 	g_siglt->GetXaxis()->CenterTitle();
// 
//  	g_siglt->Draw("A*");
//  
//  	g_siglt->GetXaxis()->SetTitle("-u");
// 	
// 
//  	g_siglt->SetMaximum(0.3);
//  	g_siglt->SetMinimum(-0.3);

 	g_siglt_fit->SetMarkerColor(2);
 	g_siglt_fit->SetLineColor(2);
//	g_siglt_fit->Draw("same*l");


	g_siglt_prv->SetMarkerColor(4);
	g_siglt_prv->SetMarkerStyle(21);
//	g_siglt_fit_prv->Draw("samep");



	TMultiGraph *mg_siglt = new TMultiGraph();

	mg_siglt->Add(g_siglt, "*");
	mg_siglt->Add(g_siglt_fit, "*l");
	mg_siglt->Add(g_siglt_prv, "p");

	mg_siglt->Draw("a");

	mg_siglt->SetTitle("Sig LT");
	
	mg_siglt->GetXaxis()->SetTitle("-u");
	mg_siglt->GetXaxis()->CenterTitle();





	c1->Print( "seperated_"+ q2_set + ".png");


// // 	TString eps_lo;
// // 	TString eps_hi;
// // 
// // 	TGraphErrors* sig_u_lo = new TGraphErrors();
// // 	TGraphErrors* sig_u_hi = new TGraphErrors();
// // 	TGraphErrors* sig_u_diff = new TGraphErrors();
// // 
// 
// 
// 	TString file_name_1;
// 	file_name_1 = "x_sep.pl_" + q2_set;
// 
//  	TString file_name_2;
//  	file_name_2 = "x_sep.pl_" + q2_set;
// 
// 	TNtuple* n1 = new TNtuple("n1", "n1", "sigl/F:dsigl:sigt:dsigt:siglt:dsiglt:sigtt:dsigtt:chi2:u:w:Q2");
// 	n1->ReadFile(file_name_1);
//  
//  	TNtuple* n2 = new TNtuple("n2", "n2", "sigl/F:dsigl:sigt:dsigt:siglt:dsiglt:sigtt:dsigtt:chi2:u:w:Q2");
//  	n2->ReadFile(file_name_2);
// 
// 
// //	n1->Draw("t");
// 
// 	Float_t uu;
// 
// 
// 	n1->SetBranchAddress("u", &uu);
// 
// 	Int_t N = n1->GetEntries();
// 	
// 	vector<Float_t> t_list;
// 
// 
// 
// 	for (Int_t i = 0; i < N; ++i) {
// 
//     	n1->GetEntry(i);
// 		
//   		if(!binary_search(t_list.begin(), t_list.end(), uu)) {
// 
// 			cout << uu << endl << endl;
// 
// 			t_list.push_back(uu);
// 
// 
// 
// 		}
// 
// 	}
// 
// 	
// 	for (Int_t i = 0; i < t_list.size(); i++) {
// 
//   	 	c1->cd(i+1);
// 
//         n1->Draw("sigl:u:dsigl", "", "goff");
//     	TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
//  	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
// 
// 		g1->Draw("A*");
// 
//  	}
// 
// 	c1->Print("Sigma_L.png");
// 
// 
// 	for (Int_t i = 0; i < t_list.size(); i++) {
// 
//   	 	c1->cd(i+1);
// 
//         n1->Draw("sigt:u:dsigt", "", "goff");
//     	TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
//  	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
// 
// 		g1->Draw("A*");
// 
//  	}
// 
// 	c1->Print("Sigma_T.png");
// 
// 	for (Int_t i = 0; i < t_list.size(); i++) {
// 
//   	 	c1->cd(i+1);
// 
//         n1->Draw("siglt:u:dsiglt", "", "goff");
//     	TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
//  	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
// 
// 		g1->Draw("A*");
// 
//  	}
// 
// 	c1->Print("Sigma_LT.png");
// 
// 	for (Int_t i = 0; i < t_list.size(); i++) {
// 
//   	 	c1->cd(i+1);
// 
//         n1->Draw("sigtt:u:dsigtt", "", "goff");
//     	TGraphErrors* g1_tmp = new TGraphErrors(n1->GetSelectedRows(), n1->GetV2(), n1->GetV1(), 0, n1->GetV3());
//  	  	TGraphErrors* g1 = (TGraphErrors*) g1_tmp->Clone("g1");
// 
// 		g1->Draw("A*");
// 
//  	}
// 
// 	c1->Print("Sigma_TT.png");
// 
// 



}



// 
// 
// /*--------------------------------------------------*/
// 
// Double_t fun_exp(Double_t *x, Double_t *par){
// 
//     Float_t xx =x[0];
// 
// //    Double_t f = par[0]*sin(xx);
// 
//     Double_t sig =  par[0]*exp(par[1]*xx)+ par[2];
// 
// 
// 	return sig;
// }
// 
// 
// 
// 
// 
// /*--------------------------------------------------*/
// 
// Double_t LT_sep_x_lo_fun(Double_t *x, Double_t *par){
// 
// 
// 	Float_t eps = 0.27;
// 
//     Float_t xx =x[0];
// 
// //    Double_t f = par[0]*sin(xx);
// 	Double_t xs;
// 
// //	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(2*xx) + par[3]*cos(xx);
// 	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(xx) + eps*par[3]*cos(2*xx);
// 
// 	return xs;
// }
// 
// 
// 
// /*--------------------------------------------------*/
// 
// Double_t LT_sep_x_hi_fun(Double_t *x, Double_t *par){
// 
// 
// 	Float_t eps = 0.55;
// 
//     Float_t xx =x[0];
// 
// //    Double_t f = par[0]*sin(xx);
// 	Double_t xs;
// 
// 	xs = par[0] +  eps*par[1] + sqrt(2*eps*(1+eps))*par[2]*cos(xx) + eps*par[3]*cos(2*xx);
// 
// 	return xs;
// }
// 
// 
// 
// 
