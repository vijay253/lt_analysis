import uproot
import awkward as ak

# Input and output ROOT files
input_file = "/cache/hallc/kaonlt/Data_Analysis_E12_19_006_Low_Q2/ROOTfiles/Kaon_coin_replay_production_8759_-1.root"
output_file = "output/8759.root"

branches_to_read = ["CTime.CoinTime_RAW_ROC1", "CTime.CoinTime_RAW_ROC2"]

tree_name = "T"

# Open the ROOT file and read the tree
with uproot.open(input_file) as file:
    tree = file[tree_name]
    
    # Read desired branches (or all branches)
    data = tree.arrays(branches_to_read, library="ak")  # Reads all branches as awkward array

# Example: define your cut (e.g., 10 < time < 50)
#cut = (data["time"] > 10) & (data["time"] < 50)
#cut = data

# Apply the cut
#trimmed_data = data[cut]

# Write the trimmed tree to a new ROOT file
with uproot.recreate(output_file) as new_file:
    new_file[tree_name] = data

print(f"Trimmed tree written to {output_file}")
