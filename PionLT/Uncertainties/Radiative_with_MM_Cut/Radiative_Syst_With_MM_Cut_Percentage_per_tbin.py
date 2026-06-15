import numpy as np
import matplotlib.pyplot as plt
import os

# -----------------------------
# USER INPUT
# -----------------------------
cut_folders = ["0p972","0p974","0p976","0p978",
               "0p98","0p982","0p984","0p986","0p988"]

def folder_to_float(name):
    return float(name.replace('p', '.'))

x_values = [folder_to_float(f) for f in cut_folders]

files = {
    "low":  "aver.pl_375_286.dat",
    "mid":  "aver.pl_375_629.dat",
    "high": "aver.pl_375_781.dat"
}

colors = {
    "low": "blue",
    "mid": "green",
    "high": "red"
}

# -----------------------------
# STRUCTURE
# -----------------------------
N_T = 7
N_PHI = 16
N_TOTAL = N_T * N_PHI

# -----------------------------
# φ INTEGRATION (WEIGHTED)
# -----------------------------
def read_t_bins(filepath):

    data = np.loadtxt(filepath)

    values = data[:, 0]
    errors = data[:, 1]

    if len(values) != N_TOTAL:
        raise ValueError(f"{filepath}: expected {N_TOTAL}, got {len(values)}")

    values = values.reshape(N_T, N_PHI)
    errors = errors.reshape(N_T, N_PHI)

    t_means = []

    for v_phi, e_phi in zip(values, errors):

        mask = e_phi > 0
        v_phi = v_phi[mask]
        e_phi = e_phi[mask]

        weights = 1.0 / (e_phi**2)

        mean = np.sum(v_phi * weights) / np.sum(weights)

        t_means.append(mean)

    return np.array(t_means)

# -----------------------------
# READ DATA
# -----------------------------
results = {eps: [] for eps in files}

for folder in cut_folders:
    for eps, fname in files.items():

        path = os.path.join(folder, fname)

        results[eps].append(read_t_bins(path))

for eps in files:
    results[eps] = np.array(results[eps])  # (cuts, t_bins)

# -----------------------------
# PERCENT VARIATION
# -----------------------------
ref_index = cut_folders.index("0p98")

percent_results = {}

for eps in files:

    ref_val = results[eps][ref_index]

    pct_list = []

    for i in range(len(cut_folders)):
        pct = (results[eps][i] / ref_val - 1.0) * 100.0
        pct_list.append(pct)

    percent_results[eps] = np.array(pct_list)

# -----------------------------
# PLOT (3x3 GRID)
# -----------------------------
fig, axes = plt.subplots(3, 3, figsize=(14, 10), sharex=True, sharey=False)

axes = axes.flatten()

for t in range(N_T):

    ax = axes[t]

    for eps in files:
        ax.plot(
            x_values,
            percent_results[eps][:, t],
            'o-',
            color=colors[eps],
            label=eps if t == 0 else None  # legend only once
        )

    ax.axhline(0, linestyle='--', linewidth=1)
    ax.set_title(f"t bin {t+1}", fontsize=10)
    ax.grid(True)

# turn off empty panels (8th and 9th)
for i in range(N_T, 9):
    axes[i].axis('off')

# global labels
fig.supxlabel("Missing Mass Cut (GeV)")
fig.supylabel("Variation w.r.t 0.980 (%)")

# single legend
handles, labels = axes[0].get_legend_handles_labels()
#fig.legend(handles, labels, loc="upper right")
fig.legend(handles, labels, loc="lower right")

plt.tight_layout()
plt.savefig("yield_variation_all_tbins.pdf")
plt.close()
