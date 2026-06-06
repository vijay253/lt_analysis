import ROOT
import numpy as np
from array import array
import os
import glob

def process_file(FILENAME):
    # Output PDF name
    base, _ = os.path.splitext(FILENAME)
    OUTFILE = base + ".pdf"

    # Load data: format y, y_err, phi, t
    data = np.loadtxt(FILENAME)
    if data.ndim == 1:
        data = data.reshape(1, -1)

    y_vals   = data[:, 0]
    y_errs   = data[:, 1]
    phi_vals = data[:, 2]
    t_bins   = data[:, 3]

    unique_t = np.unique(t_bins)

    # Canvas with 7 plots (4x2 pads)
    c = ROOT.TCanvas("c", "t-bin plots", 1400, 900)
    c.Divide(4, 2)   # 8 pads, 7 used

    graphs = []
    lines  = []

    for i, t in enumerate(unique_t):
        mask = (t_bins == t)
        phi  = phi_vals[mask].astype(float)
        y    = y_vals[mask].astype(float)
        err  = y_errs[mask].astype(float)

        if len(phi) == 0:
            continue

        # sort by phi
        order = np.argsort(phi)
        phi = phi[order]
        y   = y[order]
        err = err[order]

        n = len(phi)
        xa = array('d', phi.tolist())
        ya = array('d', y.tolist())
        xea = array('d', [0.0]*n)
        yea = array('d', err.tolist())

        gr = ROOT.TGraphErrors(n, xa, ya, xea, yea)
        gr.SetTitle(f"t-bin {i+1};Phi;Yield Ratio (Exp/MC)")
        gr.SetMarkerStyle(20)
        gr.SetMarkerSize(0.9)
        gr.SetMarkerColor(ROOT.kBlue+1)
        gr.SetLineColor(ROOT.kBlue+1)

        graphs.append(gr)

        # fixed y-axis range
        y_min = 0.5
        y_max = 1.5

        c.cd(i+1)
        pad = ROOT.gPad
        pad.SetLeftMargin(0.15)
        pad.SetRightMargin(0.05)
        pad.SetTopMargin(0.08)
        pad.SetBottomMargin(0.12)

        gr.Draw("AP")
        ROOT.gPad.Update()

        hist = gr.GetHistogram()
        if hist:
            hist.SetMinimum(y_min)
            hist.SetMaximum(y_max)
            hist.GetYaxis().SetTitle("Yield Ratio (Exp/MC)")
            hist.GetYaxis().SetTitleOffset(1.2)
            hist.GetXaxis().SetTitle("Phi")

        # horizontal red dashed line at y=1
        x_min = float(np.min(phi))
        x_max = float(np.max(phi))
        line = ROOT.TLine(x_min, 1.0, x_max, 1.0)
        line.SetLineColor(ROOT.kRed)
        line.SetLineStyle(2)
        line.SetLineWidth(2)
        line.Draw("SAME")

        lines.append(line)

    c.Update()
    c.SaveAs(OUTFILE)
    print(f"Saved: {OUTFILE}")


# --- Main program ---
files_input = input("Enter .dat files (space-separated or wildcard like *.dat): ").strip()

# split by spaces and expand wildcards
file_patterns = files_input.split()
file_list = []
for pattern in file_patterns:
    expanded = glob.glob(pattern)
    if expanded:
        file_list.extend(expanded)
    else:
        if os.path.isfile(pattern):
            file_list.append(pattern)
        else:
            print(f"Warning: '{pattern}' not found.")

if not file_list:
    print("No matching files found!")
else:
    for f in file_list:
        process_file(f)
