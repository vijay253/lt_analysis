import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

base_dir = Path("/volatile/hallc/c-kaonlt/vijay/Analysis/Uncertainties/Model_Systematics")
std_dir  = base_dir / "Standard"

#sigL
#plus_dir = base_dir / "sigL_variations" / "plus_10percent"
#minus_dir = base_dir / "sigL_variations" / "minus_10percent"
#sigT
plus_dir = base_dir / "sigT_variations" / "plus_10percent"
minus_dir = base_dir / "sigT_variations" / "minus_10percent"
#sigLT
#plus_dir = base_dir / "sigLT_variations" / "plus_0p14"
#minus_dir = base_dir / "sigLT_variations" / "minus_0p14"
#sigTT
#plus_dir = base_dir / "sigTT_variations" / "plus_0p36"
#minus_dir = base_dir / "sigTT_variations" / "minus_0p36"

filename = "x_sep.pl_375"
#sigL
#output_pdf = "output/sigL_variation.pdf"
#output_csv = "output/sigL_variation.csv"
#sigT
output_pdf = "output/sigT_variation.pdf"
output_csv = "output/sigT_variation.csv"
#sigLT
#output_pdf = "output/sigLT_variation.pdf"
#output_csv = "output/sigLT_variation.csv"
#sigTT
#output_pdf = "output/sigTT_variation.pdf"
#output_csv = "output/sigTT_variation.csv"

std_data   = np.loadtxt(std_dir / filename)
plus_data  = np.loadtxt(plus_dir / filename)
minus_data = np.loadtxt(minus_dir / filename)

sigT_std = std_data[:, 0]
sigL_std = std_data[:, 2]

sigT_plus = plus_data[:, 0]
sigL_plus = plus_data[:, 2]

sigT_minus = minus_data[:, 0]
sigL_minus = minus_data[:, 2]

# -----------------------------
# Differences
# -----------------------------
d_sigT_plus  = ((sigT_plus  - sigT_std) / sigT_std ) * 100
d_sigT_minus = ((sigT_minus - sigT_std) / sigT_std ) * 100

d_sigL_plus  = ((sigL_plus  - sigL_std) / sigL_std) * 100
d_sigL_minus = ((sigL_minus - sigL_std) / sigL_std) * 100

# -----------------------------
# % Differences
# -----------------------------
#d_sigT_plus  = ((sigT_std - sigT_plus) / sigT_std ) * 100 
#d_sigT_minus = ((sigT_std - sigT_minus) / sigT_std ) * 100
 
#d_sigL_plus  = ((sigL_std - sigL_plus) / sigL_std) * 100
#d_sigL_minus = ((sigL_std - sigL_minus) / sigL_std) * 100

# X-axis (bin index)
x = np.arange(len(sigT_std))

# -----------------------------
# Write to CSV
# -----------------------------
csv_data = np.column_stack((
    x.astype(int),
    d_sigT_plus,
    d_sigT_minus,
    d_sigL_plus,
    d_sigL_minus
))

header = (
    "bin_index,"
    "d_sigT_plus,"
    "d_sigT_minus,"
    "d_sigL_plus,"
    "d_sigL_minus"
)

np.savetxt(
    output_csv,
    csv_data,
    delimiter=",",
    header=header,
    comments="",
    fmt=["%d", "%.5f", "%.5f", "%.5f", "%.5f"]
)

print(f"Saved CSV to: {output_csv}")

# -----------------------------
# Plot
# -----------------------------
plt.figure(figsize=(8, 6))

# sigL
plt.plot(x, d_sigL_plus,  marker="o", linestyle="-",
#         label="sigL: (sigL +10%) − standard")
         label="sigL: (sigT +10%) − standard")
#         label="sigL: (sigLT + 0.14) − standard")
#         label="sigL: (sigTT + 0.36) − standard")
plt.plot(x, d_sigL_minus, marker="o", linestyle="--",
#         label="sigL: (sigL −10%) − standard")
         label="sigL: (sigT −10%) − standard")
#         label="sigL: (sigLT − 0.14) − standard")
#         label="sigL: (sigTT − 0.36) − standard")

# sigT
plt.plot(x, d_sigT_plus,  marker="s", linestyle="-",
#         label="sigT: (sigL +10%) − standard")
         label="sigT: (sigT +10%) − standard")
#         label="sigT: (sigLT + 0.14) − standard")
#         label="sigT: (sigTT + 0.36) − standard")
plt.plot(x, d_sigT_minus, marker="s", linestyle="--",
#         label="sigT: (sigL −10%) − standard")
         label="sigT: (sigT −10%) − standard")
#         label="sigT: (sigLT − 0.14) − standard")
#         label="sigT: (sigTT − 0.36) − standard")

plt.axhline(0.0, linewidth=1)

plt.xlabel("t Bin")
plt.ylabel("Difference in sep. cross section")
#plt.title("±10% sigL Model variation")
plt.title("±10% sigT Model variation")
#plt.title("sigLT Model variation")
#plt.title("sigTT Model variation")
#plt.ylim(-0.005, 0.002)
plt.legend()
plt.tight_layout()

# -----------------------------
# Save and close
# -----------------------------
plt.savefig(output_pdf)
plt.close()

print(f"Saved plot to: {output_pdf}")
