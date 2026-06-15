import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# -----------------------------
# Directories
# -----------------------------
base_dir = Path("/volatile/hallc/c-kaonlt/vijay/Analysis/Uncertainties/Radiative")

std_dir = base_dir / "Standard"
rad_off_dir = base_dir / "Radiative_Off"

filename = "x_sep.pl_375"

output_pdf = "output/sigL_sigT_rad_diff.pdf"
output_csv = "output/sigL_sigT_rad_diff.csv"

# -----------------------------
# Load data
# -----------------------------
std_data = np.loadtxt(std_dir / filename)
rad_data = np.loadtxt(rad_off_dir / filename)

# Columns
sigT_std = std_data[:, 0]
sigL_std = std_data[:, 2]

sigT_rad = rad_data[:, 0]
sigL_rad = rad_data[:, 2]

# -----------------------------
# Differences (Standard - Radiative_off)
# -----------------------------
d_sigT = ((sigT_std - sigT_rad) / sigT_std ) * 100    # percentage
d_sigL = ((sigL_std - sigL_rad) / sigL_std ) * 100    # percentage

# X-axis (bin index)
x = np.arange(len(sigT_std))

# -----------------------------
# Save CSV
# -----------------------------
csv_data = np.column_stack((
    x.astype(int),
    d_sigT,
    d_sigL
))

header = "bin_index,d_sigT,d_sigL"

np.savetxt(
    output_csv,
    csv_data,
    delimiter=",",
    header=header,
    comments="",
    fmt=["%d", "%.6f", "%.6f"]
)

print(f"Saved CSV to: {output_csv}")

# -----------------------------
# Plot
# -----------------------------
plt.figure(figsize=(8, 6))

# sigL
plt.plot(x, d_sigL, marker="o", linestyle="-",
         label="sigL: ((standard − radiative_off) / standard) * 100 ")

# sigT
plt.plot(x, d_sigT, marker="s", linestyle="-",
         label="sigT: ((standard − radiative_off) / standard) * 100")

plt.axhline(0.0, linewidth=1)

plt.xlabel("t Bin")
plt.ylabel("Δ cross section (%)")
plt.title("Radiative Correction Effect (Standard − Radiative_off)")

# Optional: adjust based on your scale
# plt.ylim(-0.005, 0.002)

plt.legend()
plt.tight_layout()

# -----------------------------
# Save and close
# -----------------------------
plt.savefig(output_pdf)
plt.close()

print(f"Saved plot to: {output_pdf}")
