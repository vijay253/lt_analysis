#ifndef CIRCLEDRAWER_H
#define CIRCLEDRAWER_H

#include "TH2F.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMath.h"

class CircleDrawer {
 public:
  // Constructor with default values
  CircleDrawer(TH2D* hist, int nCircles, double maxRadius, double angle)
    : fHist(hist), fNCircles(nCircles), fMaxRadius(maxRadius), fAngle(angle) {
    for (int i = 0; i < fNCircles; i++) {
      fRadii[i] = (i+1) * fMaxRadius / fNCircles;
      fColors[i] = kRed + i;
      
    }
        
  }

  // Main drawing function
  void DrawCirclesWithRadii() {
    if (!fHist) return;

    fHist->Draw("colz");
    DrawConcentricCircles();
    DrawRadialLine();
    DrawRadiusMarkers();
        
  }

  // Set custom radii
  void SetRadii(const double* radii, int n) {
    fNCircles = (n < 10) ? n : 8; // Limit to 10 circles
    for (int i = 0; i < fNCircles; i++) {
      fRadii[i] = radii[i];
              
    }
        
  }

  // Set custom colors
  void SetColors(const int* colors, int n) {
    for (int i = 0; i < n && i < fNCircles; i++) {
      fColors[i] = colors[i];
              
    }
        
  }

  // Set angle
  void SetAngle(double angle) { fAngle = angle; }

 private:
  TH2D* fHist;
  int fNCircles;
  double fMaxRadius;
  double fAngle;
  double fRadii[8]; // Maximum 10 circles
  int fColors[8];   // Corresponding colors

  void DrawConcentricCircles() {
    for (int i = 0; i < fNCircles; i++) {
      TEllipse* circle = new TEllipse(0, 0, fRadii[i]);
      circle->SetFillStyle(0);
      circle->SetLineColor(kRed);
      circle->SetLineWidth(2);
      circle->Draw("same");
              
    }
        
  }

  void DrawRadialLine() {
    double angle_rad = fAngle * TMath::Pi() / 180.0;
    // double angle_rad = fAngle * TMath::Pi() / 180.0;
    double x_end = fRadii[fNCircles-1] * TMath::Cos(angle_rad);
    double y_end = fRadii[fNCircles-1] * TMath::Sin(angle_rad);

    TLine* radialLine = new TLine(fRadii[0], 0, fRadii[fNCircles-1], 0);
    radialLine->SetLineColor(kRed);
    radialLine->SetLineWidth(2);
    radialLine->Draw("same");
        
  }

  void DrawRadiusMarkers() {
    double angle_rad = fAngle * TMath::Pi() / 180.0;
    TLatex latex;
    latex.SetTextSize(0.01);
    latex.SetTextAlign(22);

    for (int i = 0; i < fNCircles; i++) {
      // Position along radial line (90% of radius to avoid edge)

      /*
      double x_pos = fRadii[i] * TMath::Cos(angle_rad) * 0.9;
      double y_pos = fRadii[i] * TMath::Sin(angle_rad) * 0.9;
      */
      double x_pos = fRadii[i];
      double y_pos = 0.0;

      
      // Draw perpendicular tick mark
      double tick_length = 0.002;
      double x_tick1 = x_pos + tick_length * TMath::Sin(angle_rad);
      double y_tick1 = y_pos - tick_length * TMath::Cos(angle_rad);
      double x_tick2 = x_pos - tick_length * TMath::Sin(angle_rad);
      double y_tick2 = y_pos + tick_length * TMath::Cos(angle_rad);

      TLine* tick = new TLine(x_tick1, y_tick1, x_tick2, y_tick2);
      tick->SetLineColor(kBlack);
      tick->SetLineWidth(2);
      //      tick->Draw("same");
    
      // Add radius label (offset perpendicularly)
      latex.DrawLatex(x_pos + 0.5*TMath::Sin(angle_rad),
		      y_pos - 0.5*TMath::Cos(angle_rad),
		      Form("%.1f", fRadii[i]));

      // Add radius label above the tick
      //            double bins[] = {0.00705, 0.0132833, 0.0173167, 0.0220833, 0.0275833, 0.0341833, 0.0440833, 0.06315};      
      double bins[] = {0.00705, 0.0132833, 0.0173167, 0.0220833, 0.0275833, 0.0341833, 0.0440833, 0.050};      
      latex.DrawLatex(bins[i], tick_length, Form("%0.3f", bins[i]));

      
    }
  
    // Add angle label
    double x_label = 0.7 * fRadii[fNCircles-1] * TMath::Cos(angle_rad);
    double y_label = 0.7 * fRadii[fNCircles-1] * TMath::Sin(angle_rad);
    latex.SetTextSize(0.035);
    //  latex.DrawLatex(x_label, y_label, Form("%.0f#circ", fAngle));

  }
  
};

#endif
