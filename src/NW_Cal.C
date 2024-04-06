
const Float_t m_p     = 0.93827;
const Float_t m_omega = 0.78259;

const Float_t Mpi0    = 134.9766;
const Float_t Mpi02   = pow(Mpi0,2);
const Float_t pi      = 3.141592653589793; 

Int_t neg_count;


TString q2_set_str;

Float_t sig_summer19_mod(Float_t thetacm, Float_t phicm, Float_t t_gev, Float_t tprime_gev, Float_t q2_gev, Float_t w_gev, Float_t eps);

Float_t sig_gmh_mod_u_two_model(Float_t thetacm, Float_t phicm, Float_t u_gev, Float_t q2_gev, Float_t w_gev, Float_t eps);

/*--------------------------------------------------*/
/*--------------------------------------------------*/

void wt_test() 
{
  
  TString q2;
	TString eps;
	TString theta;

	TString q2_setting[2]     = {"375", "245"};
	
	TString eps_375[3]        = {"286", "629", "781"};

	TString hms_angle_375_l[3]  = {"-4.005", "-1.995", "+0.000"};
	TString hms_angle_375_m[5]  = {"-4.000", "-2.000", "+0.000", "+2.000", "+3.120"};
	TString hms_angle_375_h[4]  = {"-4.000",  "-2.015", "+0.000", "+2.680"};

	TString dir_str;
	TString file_name;

}

void NW_Cal() 

{	
  TString root_file_str = "/volatile/hallc/c-kaonlt/vijay/ROOTfiles/KaonLT_Analysis/PhysicsAnalysis/PionLT/Analysis/KaonLT/Pion_2p7_Q1_center.root";

 	TFile* f1     = new TFile(root_file_str, "READ");	
	TTree* t1 = (TTree*)f1->Get("h10");

	cout << root_file_str << "    " << root_file_str.ReplaceAll(".root", "") << endl; 
	
	TFile* f1_mod = new TFile(root_file_str.ReplaceAll(".root", "") + "_mod.root", "RECREATE");
	neg_count = 0;	

//	exit(0);
//	t1->Draw("Weight");
	
	Float_t weight_mod;
	Float_t sig_mod;
	Float_t weight;
	Float_t sigcm;

	Float_t thetacm, thetapq, phicm, t, w, Q2, eps;
	Float_t wi, ti, Q2i, thetacmi, phipqi, epsiloni, nu, q;
	Float_t phicmi, wcmi, tprimei, ui;

//	TBranch* w_mod = t1->Branch("mod", &weight_mod, "mod/F");

	TBranch *b = t1->GetBranch("Weight_mod");
	t1->GetListOfBranches()->Remove(b);
//	b->Delete();


	TBranch* w_mod = t1->Branch("Weight_mod", &weight_mod, "Weight_mod");

	t1->SetBranchAddress("Weight",  &weight  );
	t1->SetBranchAddress("sigcm",   &sigcm   );

	t1->SetBranchAddress("thetapq", &thetacm );

	t1->SetBranchAddress("phipq",   &phicm   );
	t1->SetBranchAddress("t",       &t       );
	t1->SetBranchAddress("W",       &w       );

	t1->SetBranchAddress("Q2",      &Q2      );
	t1->SetBranchAddress("epsilon", &eps     );

	t1->SetBranchAddress("Wi",       &wi       );
	t1->SetBranchAddress("Q2i",      &Q2i      );
	t1->SetBranchAddress("thetapq", &thetacmi );
	t1->SetBranchAddress("phipqi",   &phipqi   );

	t1->SetBranchAddress("nu",       &nu       );
	t1->SetBranchAddress("q",        &q        );


	t1->SetBranchAddress("epsilon", &epsiloni );
	t1->SetBranchAddress("phipqi",   &phicmi   );
	t1->SetBranchAddress("Wi",       &wcmi     );
	t1->SetBranchAddress("ti",  &tprimei  );

	t1->SetBranchAddress("ti",       &ti  );
	t1->SetBranchAddress("ti",       &ui  );


	t1->SetBranchAddress("sigcm", &sigcm);
	//	t1->SetBranchAddress("sigcm", &sigcm);
//	t1->SetBranchAddress("sigcm", &sigcm);
//	t1->SetBranchAddress("sigcm", &sigcm);

    Long64_t nentries = t1->GetEntries(); // read the number of entries in the t3


//	t1->SetBranchStatus("mod", 1);

    for (Long64_t i = 0; i < nentries; i++) {
//    for (Long64_t i = 0; i < 1; i++) {

		
//		t1->SetBranchStatus("Weight", 1);

		t1->GetEntry(i);

		Float_t Wsq, qsq, m_psq, invm, tt, uu, theta, phi;
		Float_t mass;

		
		/*--------------------------------------------------*/

// 		invm  = w;
// 		tt    = t;
// //		qsq   = Q2;
// 		theta = thetacm;
// 	 	phi   = phicm;

// 		/*--------------------------------------------------*/
 

		invm  = wcmi;
		tt    = ti;
		qsq   = Q2i;
		theta = thetacmi;
		phi   = phicmi;
		eps   = epsiloni;
		uu    = ui;

// 

//		invm = sqrt((nu+m_p)**2-q**2);
	
		/*--------------------------------------------------*/
		/// Constant		
		Wsq   = invm*invm;
		m_psq = m_p*m_p;
		mass  = m_omega;



		Float_t e_photCM = (Wsq - qsq - m_psq)/invm/2.;
		Float_t e_omCM   = (Wsq + pow(mass,2) - m_psq)/invm/2.;
      
		Float_t t_min    = -qsq + pow(mass, 2) -2*(e_omCM*e_photCM-sqrt((pow(e_omCM, 2)-pow(mass,2)*(pow(e_photCM,2)+qsq))));

//		Float_t tprime   = abs(tt)-abs(t_min);

		Float_t tprime   = tprimei;


//		cout<< "Check tprime:  " << -tt << "     " << tprime << endl;


		/*--------------------------------------------------*/	
		// Cal_New_Mod(thetacm, phicm, t_gev, tprime_gev, q2_gev, w_gev, eps);		/// Calculate the new Cross section
//		 sig_mod = Cal_New_Mod(theta, phi, -tt, tprime, qsq, invm, eps);
//		 sig_mod = sig_gmh_mod(theta, phi, -tt, tprime, qsq, invm, eps);



 		if (root_file_str.Contains("Q1")) {
 
			q2_set_str = "Q1";

 		} else if (root_file_str.Contains("Q2")) {

			q2_set_str = "Q2";
 
 		} else {
 
// 			sig_mod = 0.0;
 			exit(0);
// 	
 		}

//     /*--------------------------------------------------*/
//     //  Itt_20 Fitted parameterization
// 
// 	if( q2_set_str == "160") {
// 		cout << "Q2 = 1.6 Parameterization is used ! " << endl; 
// 	} else if (q2_set_str == "245") {
// 		cout << "Q2 = 2.45 Parameterization is used !" << endl; 
// 	} else {
// 		cout << "No parameterization found !" << endl;
// 		exit(0);
// 	}
//		sig_mod = sig_gmh_mod_u_two_model(theta, phi, -uu, qsq, invm, eps);

		sig_mod = sig_summer19_mod(theta, phi, -tt, tprime, qsq, invm, eps);

		if (sig_mod < 0.0) {

// 			cout << "The sig_mod calculated is below 0 !!!!!   " << sig_mod << endl;
// 			cout << "If you see this message, go back to your fit_in_t and then refit!" << endl;
// 			cout << "Parameterization should not yield sig < 0" << endl;
//			exit(0);

			neg_count++;

//			sig_mod = 0.0;

		}

//		sig_mod = sig_gmh_mod_u(theta, phi, -uu, qsq, invm, eps);

		/*--------------------------------------------------*/	
		/// Calculate the new weight
	

		if( weight*sig_mod/sigcm != weight*sig_mod/sigcm) {

			weight_mod = weight;			
//			cout << "!!! " << weight_mod << endl;	

		} else {

			weight_mod = weight * sig_mod/sigcm;

		}

//		weight_mod = sig_mod / sigcm;

// 		cout << weight << "     " << weight_mod << "    " << sigcm << "    " << sig_mod<< endl;

//		exit(0);

// 		if (Float_t(sigcm/sig_mod) != Float_t(1.0)) {
// 
//  			cout << "  Sig: " << sigcm << "    Sig_mod:" << sig_mod << "  "  << sigcm / sig_mod << endl;
// 			
// 
// 		} 
// 	

//		exit();

//		weight_mod = 1.1;

//		t1->SetBranchStatus("*", 0);
//		t1->SetBranchStatus("mod", 1);

		w_mod->Fill();

//		t1->SetBranchStatus("*", 1);


    }






// 	t1->SetBranchAddress("Weight_mod/F", &weight);
// 
//     for (Long64_t i = 0; i < nentries; i++) {
// 
// 		t1->GetEntry(i);
// 
// 		cout << weight<< "     " << weight_mod << endl;
// 
//     }
//	t1->SetBranchStatus("*",1);
//	t1->Print();

 	cout << "The total number of negtive counts:  " << neg_count << endl;

	/*--------------------------------------------------*/	
	/// Print into a new file 
	
	TTree* t11 = t1->CloneTree();

//	f1->Close();
//	t1->Delete();
//	f1->Clear();


	f1_mod->cd();
//	f1->cd();
//	f1->DeleteAll();
//	gDirectory->Delete("h666;*");
//	f1->ReOpen("RECREATE");

// 	t11->Print();
 	t11->Write();

	f1_mod->Close();
	f1->Close();

}

//=========================================================================
/// Model for Summer 2019 data Jun 7, 2024
/// Analyzed V.K.
//==========================================================================

Float_t sig_summer19_mod(Float_t thetacm, Float_t phicm, Float_t t_gev, Float_t tprime_gev, Float_t q2_gev, Float_t w_gev, Float_t eps) {

	Float_t sig_summer19_mod;

//	sig_gmh_mod = 100.0;

	Float_t sig, wfactor;
	Float_t sigt,sigl,siglt,sigtt;
//	Float_t thetacm,phicm,t_gev,tprime_gev,q2_gev,w_gev,eps,tp;
	Float_t lambda0_sq,lambdapi_sq,alphapi_t,alphapi_0;
	Float_t a,b,c,d,e,f,fpi,fpi235;
	Float_t m_pi0sq, tp;
	Float_t mp = m_p*1000;

//	cout << endl << "Theta: "<< thetacm << "   phi: " << phicm << "   t_gev: " << t_gev << "   t_prime: " << tprime_gev << "   q2_gev: " << q2_gev  <<  "   w_gev: "<< w_gev  << "  eps: " << eps << endl << endl;   

    m_pi0sq= Mpi02/1.e6;

	tp = fabs(tprime_gev);

	lambda0_sq= 0.462;



//	cout << m_pi0sq << endl;

	if (t_gev>m_pi0sq) { 
	   alphapi_t=0.7*(t_gev-m_pi0sq);
	} else {
	   alphapi_t=tanh(0.7*(t_gev-m_pi0sq));
	}
	
	alphapi_0=0.7*(0.-m_pi0sq);
	lambdapi_sq=lambda0_sq*pow((1.+alphapi_t)/(1.+alphapi_0),2);
	fpi    = 1./(1.+q2_gev/lambdapi_sq);
	fpi235 = 1./(1.+0.375/lambdapi_sq);
	
	
	a = 0.16675;
	b = 0.89524;
	c = 3.5991;
	d = 6.4562;
	e = -9.6199;
	f = 5.8319;
	sigl = a*exp(-b*tp)+c*exp(-d*(pow(tp,0.5)))+e*exp(-f*pow(tp,0.25));
	sigl = sigl *pow(fpi/fpi235,2);
	

//	cout << "Sigma L: "<< fpi <<  "  " << fpi235 << "   "<< tp << "  " << sigl << endl;;


	a = -0.12286;
	b = 0.56383;
	c = 1.4673;
	d = 2.1988;
	e = 0.65170;
	f = 18.501;
	sigt = a*exp(-b*tp)+c*exp(-d*(pow(tp,0.5)))+e*exp(-f*pow(tp,2));
	sigt = sigt *pow(fpi/fpi235,2);

//	cout << "Sigma T: "<< sigt << endl;;

	
	a = 0.46397;
	b = 4.9179;
	c = 3.3023;
	d = 3.1741;
	siglt = a*exp(-b*tp)+c*exp(-d*pow(tp,0.5));
	siglt = siglt*pow(fpi/fpi235,2)*sin(thetacm);
	siglt = -siglt/sqrt(2.);
	
//	cout << "Sigma LT: "<< siglt << endl;;



	a = -0.26497;
	b = 2.7655;
	c = 2.8034;
	d = 3.8586;
	sigtt = a*exp(-b*tp)+c*exp(-d*pow(tp,0.5));
	sigtt= sigtt*pow(fpi/fpi235,2)*pow(sin(thetacm),2);
	
//	cout << "Sigma TT: " << sigtt << endl;;

	wfactor=pow((pow(2.47,2)-pow(mp,2)),2)/pow((pow(w_gev,2)-pow(mp,2)),2);
	
//	cout << "wfactor: " << mp << "   " << wfactor << endl;;


	sigl = sigl*wfactor;
	sigt = sigt*wfactor;
	siglt= siglt*wfactor;
	sigtt= sigtt*wfactor;
	
	sig = sigt +eps*sigl +eps*cos(2.*phicm)*sigtt +sqrt(2.*eps*(1.+eps))*cos(phicm)*siglt;
	
	sig = sig/2./pi/1.e06;

	sig_summer19_mod = sig;
	
	
	
	return sig_summer19_mod;
}

/*--------------------------------------------------*/
/*--------------------------------------------------*/
/// GMH model original 
//
/// Do Not Change!

Float_t sig_gmh_mod(Float_t thetacm, Float_t phicm, Float_t t_gev, Float_t tprime_gev, Float_t q2_gev, Float_t w_gev, Float_t eps) {

	Float_t sig_gmh_mod;

//	sig_gmh_mod = 100.0;

	Float_t sig, wfactor;
	Float_t sigt,sigl,siglt,sigtt;
//	Float_t thetacm,phicm,t_gev,tprime_gev,q2_gev,w_gev,eps,tp;
	Float_t lambda0_sq,lambdapi_sq,alphapi_t,alphapi_0;
	Float_t a,b,c,d,e,f,fpi,fpi235;
	Float_t m_pi0sq, tp;
	Float_t mp = m_p*1000;

//	cout << endl << "Theta: "<< thetacm << "   phi: " << phicm << "   t_gev: " << t_gev << "   t_prime: " << tprime_gev << "   q2_gev: " << q2_gev  <<  "   w_gev: "<< w_gev  << "  eps: " << eps << endl << endl;   



    m_pi0sq= Mpi02/1.e6;

	tp = fabs(tprime_gev);

	lambda0_sq= 0.462;



//	cout << m_pi0sq << endl;

	if (t_gev>m_pi0sq) { 
	   alphapi_t=0.7*(t_gev-m_pi0sq);
	} else {
	   alphapi_t=tanh(0.7*(t_gev-m_pi0sq));
	}
	
	alphapi_0=0.7*(0.-m_pi0sq);
	lambdapi_sq=lambda0_sq*pow((1.+alphapi_t)/(1.+alphapi_0),2);
	fpi    = 1./(1.+q2_gev/lambdapi_sq);
	fpi235 = 1./(1.+2.35/lambdapi_sq);
	
	
	a = 0.16675;
	b = 0.89524;
	c = 3.5991;
	d = 6.4562;
	e = -9.6199;
	f = 5.8319;
	sigl = a*exp(-b*tp)+c*exp(-d*(pow(tp,0.5)))+e*exp(-f*pow(tp,0.25));
	sigl = sigl *pow(fpi/fpi235,2);
	

//	cout << "Sigma L: "<< fpi <<  "  " << fpi235 << "   "<< tp << "  " << sigl << endl;;


	a = -0.12286;
	b = 0.56383;
	c = 1.4673;
	d = 2.1988;
	e = 0.65170;
	f = 18.501;
	sigt = a*exp(-b*tp)+c*exp(-d*(pow(tp,0.5)))+e*exp(-f*pow(tp,2));
	sigt = sigt *pow(fpi/fpi235,2);

//	cout << "Sigma T: "<< sigt << endl;;

	
	a = 0.46397;
	b = 4.9179;
	c = 3.3023;
	d = 3.1741;
	siglt = a*exp(-b*tp)+c*exp(-d*pow(tp,0.5));
	siglt = siglt*pow(fpi/fpi235,2)*sin(thetacm);
	siglt = -siglt/sqrt(2.);
	
//	cout << "Sigma LT: "<< siglt << endl;;



	a = -0.26497;
	b = 2.7655;
	c = 2.8034;
	d = 3.8586;
	sigtt = a*exp(-b*tp)+c*exp(-d*pow(tp,0.5));
	sigtt= sigtt*pow(fpi/fpi235,2)*pow(sin(thetacm),2);
	
//	cout << "Sigma TT: " << sigtt << endl;;

	wfactor=pow((pow(2.47,2)-pow(mp,2)),2)/pow((pow(w_gev,2)-pow(mp,2)),2);
	
//	cout << "wfactor: " << mp << "   " << wfactor << endl;;


	sigl = sigl*wfactor;
	sigt = sigt*wfactor;
	siglt= siglt*wfactor;
	sigtt= sigtt*wfactor;
	
	sig = sigt +eps*sigl +eps*cos(2.*phicm)*sigtt +sqrt(2.*eps*(1.+eps))*cos(phicm)*siglt;
	
	sig = sig/2./pi/1.e06;

	sig_gmh_mod = sig;
	
	
	
	return sig_gmh_mod;

}

