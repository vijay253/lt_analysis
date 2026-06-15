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
# READ + WEIGHTED INTEGRATION
# -----------------------------
def read_and_integrate(filepath):
    data = np.loadtxt(filepath)

    values = data[:, 0]
    errors = data[:, 1]

    if len(values) != 128:
        raise ValueError(f"{filepath}: Expected 128 entries, got {len(values)}")

    mask = errors > 0
    values = values[mask]
    errors = errors[mask]

    weights = 1.0 / (errors**2)

    weighted_mean = np.sum(values * weights) / np.sum(weights)
    weighted_err = np.sqrt(1.0 / np.sum(weights))

    return weighted_mean, weighted_err

# -----------------------------
# MAIN LOOP
# -----------------------------
results = {eps: [] for eps in files}
errors_out = {eps: [] for eps in files}

for folder in cut_folders:
    for eps, fname in files.items():
        path = os.path.join(folder, fname)

        avg, err = read_and_integrate(path)

        results[eps].append(avg)
        errors_out[eps].append(err)

# -----------------------------
# NORMALIZATION TO 0.980
# -----------------------------
ref_index = cut_folders.index("0p98")

percent_results = {}
percent_errors = {}

for eps in files:
    ref_val = results[eps][ref_index]
    ref_err = errors_out[eps][ref_index]

    percent_results[eps] = []
    percent_errors[eps] = []

    for val, err in zip(results[eps], errors_out[eps]):
        # Percentage variation
        pct = (val / ref_val - 1.0) * 100.0

        # Error propagation
        pct_err = 100.0 * np.sqrt(
            (err / ref_val)**2 +
            (val * ref_err / ref_val**2)**2
        )

        percent_results[eps].append(pct)
        percent_errors[eps].append(pct_err)

# -----------------------------
# PLOTTING
# -----------------------------
plt.figure(figsize=(8,6))

for eps in files:
    plt.errorbar(
        x_values,
        percent_results[eps],
        yerr=percent_errors[eps],
        fmt='o-',
        color=colors[eps],
        label=f"{eps} ε"
    )

plt.xlabel("Missing Mass Cut (GeV)")
plt.ylabel("Variation w.r.t 0.980 (%)")
plt.title("Percentage Variation of Yield Ratio vs Missing Mass Cut")
plt.axhline(0, linestyle='--')  # reference line
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig("yield_ratio_percentage_variation.pdf", dpi=300)

plt.close()
