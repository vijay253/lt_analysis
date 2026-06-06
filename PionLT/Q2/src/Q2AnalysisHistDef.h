#ifndef HISTOGRAM_DEFINITIONS_H
#define HISTOGRAM_DEFINITIONS_H

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

TH2D* CreateHistogram(const char* name, const char* title) {
  TH2D *h = new TH2D(name," ; ;  ",  100, -0.1, 0.1, 100, -0.1, 0.1);
  // Configure your histogram here
  h->GetXaxis()->SetTitle(" tcos(#phi)");
  h->GetYaxis()->SetTitle(" tsin(#phi)");
  return h;
  
}

#endif
