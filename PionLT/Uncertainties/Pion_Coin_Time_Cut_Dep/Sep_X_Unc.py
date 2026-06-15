import numpy as np
import matplotlib.pyplot as plt
import os
'''
standard_file = "Standard/x_sep.pl_375"
wider_file    = "10%Wider/x_sep.pl_375"
narrow_file   = "10%Narrower/x_sep.pl_375"
'''
#after 2-2 iterations
standard_file = "Standard/x_sep.pl_375"
wider_file    = "10%Wider/withit/x_sep.pl_375"
narrow_file   = "10%Narrower/withit/x_sep.pl_375"

output_dir = "output"
os.makedirs(output_dir, exist_ok=True)

std_data    = np.loadtxt(standard_file)[:, 2]
wider_data  = np.loadtxt(wider_file)[:, 2]
narrow_data = np.loadtxt(narrow_file)[:, 2]

if not (len(std_data) == len(wider_data) == len(narrow_data) == 7):
    raise RuntimeError("Expected exactly 7 t bins")

delta_wider  = std_data - wider_data
delta_narrow = std_data - narrow_data

# combine ALL points for mean & std
combined = np.concatenate([delta_wider, delta_narrow])
mean = combined.mean()
std  = combined.std(ddof=1)

x = np.arange(1, 8)

plt.figure(figsize=(6, 4))

plt.plot(x, delta_wider,  'o', markersize=5, label="Standard − 10% Wider")
plt.plot(x, delta_narrow, '*', markersize=7, label="Standard − 10% Narrower")

plt.axhline(0, color='green', linestyle='--', linewidth=1)

plt.xlabel("t bin")
plt.ylabel("Difference (Standard - Cut)")
plt.legend()

plt.text(
    0.02, 0.95,
    f"mean = {mean:.4e}\nstd = {std:.4e}",
    transform=plt.gca().transAxes,
    verticalalignment='top'
)

plt.tight_layout()

pdf_path = os.path.join(output_dir, "Sep_unc.pdf")
plt.savefig(pdf_path)
plt.close()

print(f"Saved PDF: {pdf_path}")
