import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# -------------------------
# CONFIG
# -------------------------
standard_folder = "0p98"
variation_folders = [
    "0p972","0p974","0p976","0p978",
    "0p982","0p984","0p986","0p988"
]

filename = "x_sep.pl_375"
output_dir = "output"
os.makedirs(output_dir, exist_ok=True)

n_tbins = 7

# ONLY CENTRAL VALUES (ignore errors)
columns = {
    "sigT": 0,
    "sigL": 2,
    "sigLT": 4,
    "sigTT": 6
}

# -------------------------
# READ STANDARD
# -------------------------
std_path = os.path.join(standard_folder, filename)
std = np.loadtxt(std_path)

std_xs = {k: std[:, col] for k, col in columns.items()}

# -------------------------
# VARIATIONS (CORRECT)
# -------------------------
quad_sum = {k: np.zeros(n_tbins) for k in columns}

for folder in variation_folders:

    path = os.path.join(folder, filename)
    data = np.loadtxt(path)

    if data.shape[0] != n_tbins:
        raise ValueError(f"Bin mismatch in {path}")

    for k, col in columns.items():

        diff = data[:, col] - std_xs[k]
        quad_sum[k] += diff**2

# -------------------------
# FINAL SYSTEMATIC + %
# -------------------------
percent = {}

for k in columns:

    sys_unc = np.sqrt(quad_sum[k])

    percent[k] = np.where(
        std_xs[k] != 0,
        (sys_unc / std_xs[k]) * 100.0,
        0.0
    )

# -------------------------
# SAVE CSV
# -------------------------
df = pd.DataFrame({
    "tbin": np.arange(1, n_tbins + 1),
    "sigT_%": percent["sigT"],
    "sigL_%": percent["sigL"],
    "sigLT_%": percent["sigLT"],
    "sigTT_%": percent["sigTT"]
})

csv_path = os.path.join(output_dir, "systematic_percent_variation.csv")
#df.to_csv(csv_path, index=False)
df.round(6).to_csv(csv_path, index=False)

print("Saved:", csv_path)

# -------------------------
# PLOTS
# -------------------------
fig, axs = plt.subplots(2, 2, figsize=(12, 10))

for ax, key in zip(axs.flat, columns):

    ax.plot(np.arange(1, n_tbins+1), percent[key], marker="o")
    ax.set_title(f"{key} systematic (%)")
    ax.set_xlabel("t-bin")
    ax.set_ylabel("% variation")
    ax.grid(True)

plt.tight_layout()

pdf_path = os.path.join(output_dir, "systematics.pdf")
plt.savefig(pdf_path)

print("Saved:", pdf_path)
