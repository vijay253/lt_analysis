import ROOT
import numpy as np
import uproot

input_file = "/cache/hallc/kaonlt/Data_Analysis_E12_19_006_Low_Q2/ROOTfiles/Kaon_coin_replay_production_8769_-1.root"
output_file = "output/8769_he_l2.root"

tree_name = "T"
branches = ["CTime.CoinTime_RAW_ROC1", "CTime.CoinTime_RAW_ROC2"]

# Histogram parameters
hist_params = {
    "CTime.CoinTime_RAW_ROC1": {"bins": 3000, "min": -750, "max": 1000},
    "CTime.CoinTime_RAW_ROC2": {"bins": 3000, "min": -750, "max": 1000},
}

# Open input ROOT file and read branches using uproot
with uproot.open(input_file) as f:
    tree = f[tree_name]
    data = tree.arrays(branches, library="np")  # NumPy arrays

# Create ROOT file for output
outfile = ROOT.TFile(output_file, "RECREATE")

# Create and fill histograms
histograms = {}
for branch in branches:
    bins = hist_params[branch]["bins"]
    min_, max_ = hist_params[branch]["min"], hist_params[branch]["max"]
    
    # Create ROOT histogram
    hist = ROOT.TH1F(branch, f"{branch} histogram", bins, min_, max_)
    
    # Fill histogram from NumPy array
    for value in data[branch]:
        hist.Fill(value)
    
    # Write histogram to file
    hist.Write()
    histograms[branch] = hist

# Close output file
outfile.Close()

print(f"✅ Histograms written to {output_file}")
