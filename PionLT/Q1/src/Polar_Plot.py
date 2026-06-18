###### Polar plot script
###### Dr. Kumar, June 17, 2026
###### PRL-quality PDF output

import ROOT
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.colors import to_rgba

mpl.rcParams.update({
    "font.family": "serif",
    "font.size": 10,
    "axes.labelsize": 10,
    "axes.titlesize": 10,
    "legend.fontsize": 8,
    "xtick.labelsize": 8,
    "ytick.labelsize": 8,
    "axes.linewidth": 1.0,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "pdf.fonttype": 42,   
    "ps.fonttype": 42
})

def extract_data(file_path,
                 tree_name,
                 radius_branch,
                 angle_branch,
                 cut_var,
                 cut_value1,
                 cut_value2):

    file = ROOT.TFile.Open(file_path)

    if not file or file.IsZombie():
        print(f"Error opening {file_path}")
        return np.array([]), np.array([])

    tree = file.Get(tree_name)

    radius = []
    angle = []

    for entry in tree:

        # Example timing cut (currently commented out)
        # if (getattr(entry, cut_var) > cut_value1 and
        #     getattr(entry, cut_var) < cut_value2):

        if getattr(entry, radius_branch) <= 0.0:
            radius.append(getattr(entry, radius_branch))
            angle.append(getattr(entry, angle_branch))

    file.Close()

    return np.array(radius), np.array(angle)


root_files = [
    "/lustre24/expphy/volatile/hallc/c-kaonlt/vijay/Analysis/Q1/Analysed_Data_Q0p375W2p2_mide_Right2.root",
    "/lustre24/expphy/volatile/hallc/c-kaonlt/vijay/Analysis/Q1/Analysed_Data_Q0p375W2p2_mide_Right1.root",
    "/lustre24/expphy/volatile/hallc/c-kaonlt/vijay/Analysis/Q1/Analysed_Data_Q0p375W2p2_mide_Left2.root",
    "/lustre24/expphy/volatile/hallc/c-kaonlt/vijay/Analysis/Q1/Analysed_Data_Q0p375W2p2_mide_Left1.root",
    "/lustre24/expphy/volatile/hallc/c-kaonlt/vijay/Analysis/Q1/Analysed_Data_Q0p375W2p2_mide_Center.root"
]

tree_name = "Cut_Kaon_Events_prompt_noRF"

radius_branch = "MandelT"
angle_branch  = "ph_q"

cut_var    = "CTime_ROC1"
cut_value1 = 43
cut_value2 = 45

colors = ["red", "black", "blue", "magenta", "green"]
settings = ["Right2", "Right1", "Left2", "Left1", "Center"]

# PRL single-column width (3.4 inches)
fig, ax = plt.subplots(
    figsize=(3.4, 3.4),
    subplot_kw={'projection': 'polar'}
)

max_radius = 0

for i, root_file in enumerate(root_files):

    radius, angle = extract_data(
        root_file,
        tree_name,
        radius_branch,
        angle_branch,
        cut_var,
        cut_value1,
        cut_value2
    )

    if len(radius) == 0:
        continue

    rplot = -1.0 * radius

    max_radius = max(max_radius, np.max(rplot))

    ax.scatter(
        angle,
        rplot,
        s=0.2,                     # publication-quality point size
        color=to_rgba(colors[i], alpha=0.7),
        rasterized=True,           # keeps PDF size manageable
        label=settings[i]
    )

ax.grid(True, linewidth=0.8)

# Remove title for PRL submission
# Put kinematics in caption instead
# ax.set_title(
#     r"$Q^2 = 0.375~\mathrm{GeV}^2,\ \epsilon = 0.629$",
#     pad=15
# )

# Radial grid labels
radii = np.linspace(0.0, max_radius, 6)

labels = [''] * len(radii)
labels[0] = f"{radii[0]:.2f}"
labels[-1] = f"{radii[-1]:.2f}"

ax.set_rgrids(radii, labels=labels, angle=0)

# Theta labels
ax.tick_params(axis='both', which='major', labelsize=8)

legend = ax.legend(
    loc='upper right',
    bbox_to_anchor=(1.25, 1.15),
    frameon=False,
    markerscale=8
)

plt.savefig(
    "Polar_PRL.pdf",
    format="pdf",
    bbox_inches="tight"
)

plt.savefig(
    "Polar_PRL.png",
    dpi=600,
    bbox_inches="tight"
)

print("Saved:")
print("  Polar_PRL.pdf")
print("  Polar_PRL.png")

#plt.show()
