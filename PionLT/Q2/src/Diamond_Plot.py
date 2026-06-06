import ROOT
import matplotlib.pyplot as plt

# Function to extract data from a ROOT file
def extract_leaf_data(file_name, tree_name, leaf_name):
    file = ROOT.TFile.Open(file_name)
    tree = file.Get(tree_name)
    data = []

    for entry in tree:
        data.append(getattr(entry, leaf_name))

    file.Close()
    return data

# Specify file names, tree, and leaf variables
root_files = ["/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Center.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_mide_Center.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_lowe_Center.root"]
tree_name = "Cut_Kaon_Events_prompt_noRF"  # Replace with your tree name
leaf1 = "Q2"  # Replace with your first leaf variable
leaf2 = "W"  # Replace with your second leaf variable

# Colors for each file
colors = ["red", "blue", "green"]
labels = ["$\epsilon$ = 0.774", "$\epsilon$ = 0.617", "$\epsilon$ = 0.264"]

# Initialize plots
plt.figure(figsize=(4, 4))

# Loop through files and plot data
for i, file_name in enumerate(root_files):
    data1 = extract_leaf_data(file_name, tree_name, leaf1)
    data2 = extract_leaf_data(file_name, tree_name, leaf2)

    plt.scatter(data1, data2, s=0.1,  label=labels[i], color=colors[i], alpha=0.7)

# Plot formatting
plt.xlabel(leaf1)
plt.ylabel(leaf2)
plt.title("$Q^2$ = 0.42 $GeV^2$")
plt.legend(markerscale=20)
plt.xlabel(r"$Q^2$ ($GeV^2$)")
plt.ylabel(r"$W$ (GeV)")

#plt.grid(True)

# Show plot
plt.tight_layout()
plt.savefig('Diamond_Plot.png')
plt.show()
