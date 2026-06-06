import ROOT

def plot_histograms():
    # File names and tree variables
    #Low epsilon
    '''
    file1_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_lowe_Center.root"
    file2_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_lowe_Left1.root"
    file3_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_lowe_Left2.root"
    '''
    #Mid epsilon

    ''''
    file1_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_mide_Center.root"
    file2_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_mide_Left1.root"
    file3_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_mide_Left2.root"
    '''

    #High epsilon    
    file1_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_highe_Center.root"
    file2_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_highe_Left1.root"
    file3_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_highe_Left2.root"
    file4_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_highe_Right1.root"
    file5_name = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Analysed_Data_Q0p425W2p2_highe_Right2.root"

    tree_name = "Cut_Kaon_Events_prompt_noRF"  # Replace with the name of your TTree
    variable1 = "CTime_ROC1"  # Variable in file1
    variable2 = "CTime_ROC1"  # Variable in file2
    variable3 = "CTime_ROC1"  # Variable in file3
    variable4 = "CTime_ROC1"  # Variable in file3
    variable5 = "CTime_ROC1"  # Variable in file3

    # Open ROOT files
    file1 = ROOT.TFile.Open(file1_name)
    file2 = ROOT.TFile.Open(file2_name)
    file3 = ROOT.TFile.Open(file3_name)
    file4 = ROOT.TFile.Open(file4_name)
    file5 = ROOT.TFile.Open(file5_name)

    # Access TTrees
    tree1 = file1.Get(tree_name)
    tree2 = file2.Get(tree_name)
    tree3 = file3.Get(tree_name)
    tree4 = file4.Get(tree_name)
    tree5 = file5.Get(tree_name)

    # Create histograms
    #Low eps
   # hist1 = ROOT.TH1F("hist1", "$Q^2=0.42 GeV^2$, Low $\epsilon$", 300, -20, 20)
    #Mid and High eps
    hist1 = ROOT.TH1F("hist1", "$Q^2=0.42 GeV^2$, High $\epsilon$", 300, -20, 20)
    hist2 = ROOT.TH1F("hist2", " ", 300, -20, 20)
    hist3 = ROOT.TH1F("hist3", " ", 300, -20, 20)
    hist4 = ROOT.TH1F("hist4", " ", 300, -20, 20)
    hist5 = ROOT.TH1F("hist5", " ", 300, -20, 20)

    TimmingOffset = -44.1;
    # Fill histograms from tree variables
    tree1.Draw(f"{variable1}+{TimmingOffset}>>hist1")
    tree2.Draw(f"{variable2}+{TimmingOffset}>>hist2")
    tree3.Draw(f"{variable3}+{TimmingOffset}>>hist3")
    tree4.Draw(f"{variable4}+{TimmingOffset}>>hist4")
    tree5.Draw(f"{variable5}+{TimmingOffset}>>hist5")

    # Style settings
    hist1.SetLineColor(ROOT.kRed)
    hist2.SetLineColor(ROOT.kBlue)
    hist3.SetLineColor(ROOT.kGreen)
    hist4.SetLineColor(ROOT.kYellow)
    hist5.SetLineColor(ROOT.kBlack)

    hist1.SetLineWidth(2)
    hist2.SetLineWidth(2)
    hist3.SetLineWidth(2)
    hist4.SetLineWidth(2)
    hist5.SetLineWidth(2)

    # Create canvas and draw histograms
    canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
    hist5.Draw("HIST")
    hist1.Draw("HIST SAME")
    hist2.Draw("HIST SAME")
    hist3.Draw("HIST SAME")
    hist4.Draw("HIST SAME")

    # Add legend
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend.AddEntry(hist3, "Left2", "l")
    legend.AddEntry(hist2, "Left1", "l")
    legend.AddEntry(hist1, "Center", "l")
    legend.AddEntry(hist4, "Right1", "l")
    legend.AddEntry(hist5, "Right2", "l")
    legend.Draw()

    # Display canvas
    canvas.Update()
    #Low eps
    #  canvas.SaveAs("/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/Low_eps_Timing.png")
    #Mid and High eps
    canvas.SaveAs("/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/High_eps_Timing.png")
    # Close files
    file1.Close()
    file2.Close()
    file3.Close()
    file4.Close()
    file5.Close()

if __name__ == "__main__":
    plot_histograms()
