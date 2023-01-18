#ifndef ROOT_ANA_PL_H
#define ROOT_ANA_PL_H


#include <iostream>
#include <string>
#include <algorithm>

#include "TTree.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"

#include "print_data.h"
#include "read_setting.h"
#include "analysis.h"


void Initialization();
//void Initialization(TString , TString );

ReadFile::kinematics_profile kin_setting;
//ReadFile::efficiency_profile eff_setting;
ReadFile::center_profile cen_setting;

ReadFile::eff_pro* eff_setting;

ReadFile::eff_pro* eff_setting_dummy;
ReadFile::eff_pro* eff_setting_simc;
ReadFile::eff_pro* eff_setting_target;


Int_t num_runs;
Int_t set_runs;

TString Dummy_Name(int);
TString Simc_Name(int);
TString Target_Name(int);
TString List_Name(int);


TFile **output_file;
TFile **output_file_dummy;
TFile **output_file_simc;
TFile **output_file_target;

int Return_Setting(int, int, int);
int Return_Setting_dummy(int, int, int);
int Return_Setting_simc(int, int, int);
int Return_Setting_target(int, int, int);

void Fill_Setting_lst();



std::vector<TString> Q2_vec, Q2_vec_mod;
std::vector<TString>::iterator Q2_vec_mod_itt;

std::vector< std::vector<int> > setting_run_list;
std::vector< std::vector<int> > setting_run_list_dummy;
std::vector< std::vector<int> > setting_run_list_simc;
std::vector< std::vector<int> > setting_run_list_target;

std::vector<TString>  lists_vec, setting_vec;

std::vector<TString>  setting_vec_target, setting_vec_dummy, setting_vec_simc;

std::vector<TString>::iterator lists_vec_itt;

std::vector<TString>  lists_vec_target, lists_vec_dummy, lists_vec_simc;

std::vector<TString> simc_vec;

TString list_name;
TString simc_ana_name;

Analysis **ana;
Analysis **ana_target;
Analysis **ana_dummy;
Analysis **ana_simc;


// Analysis_heep **ana;
// Analysis_heep **ana_target;
// Analysis_heep **ana_dummy;
// 


ReadFile* rf;
ReadFile* sim_rf;

File_Output *fo; 


TString target_name;
bool dummy_tar;
bool simc_tar;

TString list_dir;

std::vector<int> run_list_vec;
std::vector<int> run_list_vec_target, run_list_vec_dummy, run_list_vec_simc;

void Cout_out_lists();

int setting_num;

/// Loading Functions
void Target_Load();
void Dummy_Load();
void Simc_Load();

/// Setting-up Functions
void Target_Setup();
void Dummy_Setup();
void Simc_Setup();

/// Analysis Functions]
void Target_Analysis();
void Dummy_Analysis();
void Simc_Analysis();

void Cleanup();


TString simc_file_inc;

#endif
