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
# φ WEIGHTED INTEGRATION
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
    t_errors = []

    for v_phi, e_phi in zip(values, errors):

        mask = e_phi > 0
        v_phi = v_phi[mask]
        e_phi = e_phi[mask]

        weights = 1.0 / (e_phi**2)

        mean = np.sum(v_phi * weights) / np.sum(weights)
        err  = np.sqrt(1.0 / np.sum(weights))

        t_means.append(mean)
        t_errors.append(err)

    return np.array(t_means), np.array(t_errors)

# -----------------------------
# READ DATA
# -----------------------------
results = {eps: [] for eps in files}
errors  = {eps: [] for eps in files}

for folder in cut_folders:
    for eps, fname in files.items():

        path = os.path.join(folder, fname)

        vals, errs = read_t_bins(path)

        results[eps].append(vals)
        errors[eps].append(errs)

for eps in files:
    results[eps] = np.array(results[eps])
    errors[eps]  = np.array(errors[eps])

# -----------------------------
# REFERENCE INDEX
# -----------------------------
ref_index = cut_folders.index("0p98")

# -----------------------------
# RMS OF % VARIATION
# -----------------------------
rms_percent = {}

for eps in files:

    vals = results[eps]
    errs = errors[eps]

    ref_vals = vals[ref_index]

    rms_list = []

    for t in range(N_T):

        pct_vals = []

        for i in range(len(cut_folders)):

            if i == ref_index:
                continue

            v = vals[i, t]
            v0 = ref_vals[t]

            if v0 == 0:
                continue

            pct = (v / v0 - 1.0) * 100.0
            pct_vals.append(pct)

        pct_vals = np.array(pct_vals)

        rms = np.sqrt(np.mean(pct_vals**2))
        rms_list.append(rms)

    rms_percent[eps] = np.array(rms_list)

# -----------------------------
# PLOT
# -----------------------------
fig, axes = plt.subplots(3, 3, figsize=(14, 10), sharex=True)

axes = axes.flatten()

for t in range(N_T):

    ax = axes[t]

    for eps in files:

        ax.plot(
            x_values,
            results[eps][:, t] / results[eps][ref_index, t] * 100 - 100,
            'o-',
            color=colors[eps],
            label=eps if t == 0 else None
        )

        rms_val = rms_percent[eps][t]

        ax.text(
            0.70, 0.40 - 0.08 * list(files.keys()).index(eps),
            f"{eps}: σ={rms_val:.4f}%",
            transform=ax.transAxes,
            color=colors[eps],
            fontsize=8,
            verticalalignment='top'
        )

    ax.axhline(0, linestyle='--', linewidth=1)
    ax.set_title(f"t bin {t+1}", fontsize=10)
    ax.grid(True)

# turn off empty panels
for i in range(N_T, 9):
    axes[i].axis('off')

# labels
fig.supxlabel("Missing Mass Cut (GeV)")
fig.supylabel("Variation w.r.t 0.980 (%)")

# legend
handles, labels = axes[0].get_legend_handles_labels()
fig.legend(handles, labels, loc="lower right")

plt.tight_layout()
plt.savefig("Radiative_Syst_With_MM_Cut_Percentage_per_tbin_with_RMS_averages.pdf")
plt.close()
