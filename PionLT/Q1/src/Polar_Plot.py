###### Polar plot script
###### Dr. Kumar, June 17, 2026

import ROOT
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.colors import to_rgba

### PRL format
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
        if (getattr(entry, cut_var) > cut_value1 and
            getattr(entry, cut_var) < cut_value2):

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

#### t offsets

t_offsets = [
    0.00034,  # Right2
    0.00045,  # Right1
    0.00054,  # Left2
    0.00063,  # Left1
    0.00056   # Center
]

print("\nApplying Hall C t-offsets:")
for name, offset in zip(settings, t_offsets):
    print(f"{name:7s} : {offset:.6f}")

## t range for plotting    
t_min = 0.006
t_max = 0.1

fig, ax = plt.subplots(
    figsize=(3.4, 3.4),
    subplot_kw={'projection': 'polar'}
)

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

    # -------------------------------------------------
    # MandelT is negative in tree (t < 0)
    # Apply correction:
    # t_corr = t + t_offset
    # plot |t| = -t_corr = -t - t_offset
    # -------------------------------------------------
    rplot = -radius - t_offsets[i]

    mask = (rplot >= t_min) & (rplot <= t_max)

    angle_plot = angle[mask]
    rplot_plot = rplot[mask]

    ax.scatter(
        angle_plot,
        rplot_plot,
        s=0.2,
        color=to_rgba(colors[i], alpha=0.7),
        rasterized=True,
        label=settings[i]
    )

ax.grid(True, linewidth=0.8)

radii = np.linspace(t_min, t_max, 6)

labels = [''] * len(radii)
labels[0] = f"{radii[0]:.3f}"
labels[-1] = f"{radii[-1]:.2f}"

ax.set_rgrids(radii, labels=labels, angle=0)

ax.tick_params(axis='both', which='major', labelsize=8)

ax.legend(
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

print("\nSaved:")
print("  Polar_PRL.pdf")
print("  Polar_PRL.png")

# plt.show()
