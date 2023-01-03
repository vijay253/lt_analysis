

TString bin_file = "../t_bin_interval" ;

Int_t Get_t_bin(){

	Float_t Q2_set;
	Int_t t_bin, phi_bin;


	ifstream t_phi_file;
	t_phi_file.open(bin_file);


	if (t_phi_file.is_open()) {
		t_phi_file >> Q2_set >> t_bin >> phi_bin;
	} else {
		cerr << "File t_bin_interval doesn't exist! " << endl;
		exit(0);
	}

//	cout << Q2_set << "  " << t_bin << "  " << phi_bin << endl; 
	t_phi_file.close();
	return t_bin;

}

Int_t Get_phi_bin(){

	Float_t Q2_set;
	Int_t t_bin, phi_bin;

	ifstream t_phi_file;
	t_phi_file.open(bin_file);

	if (t_phi_file.is_open()) {
		t_phi_file >> Q2_set >> t_bin >> phi_bin;
	} else {
		cerr << "File t_bin_interval doesn't exist! " << endl;
		exit(0);
	}

//	cout << Q2_set << "  " << t_bin << "  " << phi_bin << endl; 
	t_phi_file.close();
	return phi_bin;

}
