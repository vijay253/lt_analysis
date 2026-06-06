###### Polar plot script 
###### Dr. Kumar, Jan 16, 2025  
import ROOT
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import to_rgba

# Function to extract data from a ROOT file
def extract_data(file_path, tree_name, radius_branch, angle_branch, cut_var, cut_value1, cut_value2):
    file = ROOT.TFile.Open(file_path)
    tree = file.Get(tree_name)
    radius = []
    angle = []

    for entry in tree:
#        if (getattr(entry, cut_var) > cut_value1 and getattr(entry, cut_var) < cut_value2):  
        if getattr(entry, radius_branch) <= 0.000:  
            radius.append(getattr(entry, radius_branch))
            angle.append(getattr(entry, angle_branch))

    file.Close()
    return np.array(radius), np.array(angle)


ROOTFILEPATH = "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/"
# File paths and tree/branch details
root_files = ["/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Right2.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Right1.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Left2.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Left1.root", "/volatile/hallc/c-kaonlt/vijay/Analysis/Q2/OUTPUT_PYTHON/Analysed_Data_Q0p425W2p2_highe_Center.root"]
tree_name = "Cut_Kaon_Events_prompt_noRF"
radius_branch = "MandelT"
angle_branch = "ph_q"
cut_var = "CTime_ROC1"
cut_value1 =43 
cut_value2 = 45

# Colors for each dataset
#colors = ["red", "blue", "magenta", "black", "green"]
colors = ["red", "black",  "blue", "magenta", "green"]
setting = ["Right2", "Right1", "Left2", "Left1", "Center"]

# Initialize the polar plot
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})

# Loop through each file, extract data, and plot
for i, root_file in enumerate(root_files):
    radius, angle = extract_data(root_file, tree_name, radius_branch, angle_branch, cut_var, cut_value1, cut_value2)
#    ax.scatter(angle, radius, s=0.005, color=to_rgba(colors[i], alpha=0.7), label=setting[i])
    ax.scatter(angle, -1*radius, s=0.01, color=to_rgba(colors[i], alpha=0.7),label=setting[i])

# Add legend and title
#ax.set_ylim(bottom=0)
#legend = ax.legend(markerscale=1)
ax.set_title("$Q^2$ = 0.42 $GeV^2$, $\epsilon$ = 0.774")
ax.grid(True)

radii = np.linspace(0.000, max(-1*radius),10)
labels= [''] * len(radii)
labels[0] = f"{radii[0]:.1f}" 
labels[-1] = f"{radii[-1]:.1f}" 
ax.set_rgrids(radii,labels, angle=0)
#ax.set_rgrids(np.linspace(-0.2, max(radius),10),labels=[], angle=0)


# Show the plot
legend = ax.legend(markerscale=20)
plt.savefig('Polar.png')  # Save plot as PNG
plt.show()
