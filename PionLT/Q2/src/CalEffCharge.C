// .... This script has created to the Coin Heep Study....
// .... Created Date: Oct 11, 2023 ....
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

void CalEffCharge(string RunNum = "", Double_t charge = 0, Double_t I = 0, Double_t Piontrkeff = 0, Double_t Electrontrkeff = 0, Double_t EDTMLIVE = 0, Double_t CPULIVE = 0, Double_t SHMS3Y4 = 0, Double_t HMS3Y4 = 0)
{

  Double_t hmscaleff = 0.9979;
  Double_t hmscalefferrr = 4.004e-05;

  if (EDTMLIVE >= 0.0)
    {
      Double_t EffCharge = charge*(1.0-0.0005296*I)*Piontrkeff*Electrontrkeff*EDTMLIVE*SHMS3Y4*HMS3Y4*hmscaleff;  
      ofstream file("CalEffCharge/"+RunNum+".dat");;
      file<< RunNum <<"\t"<< EffCharge <<endl;
      file.close();
    }
  //If EDTM tunred off
  else
    {
      Double_t EffCharge = charge*(1.0-0.0005296*I)*Piontrkeff*Electrontrkeff*CPULIVE*SHMS3Y4*HMS3Y4*hmscaleff; 
      ofstream file("CalEffCharge/"+RunNum+".dat");;
      file<< RunNum <<"\t"<< EffCharge <<endl;
      file.close();
    }

}
   

