#!/usr/bin/env python3

"""
Systematic uncertainty calculator

This script:

1. Reads the standard file
2. Computes differences wrt standard
3. Computes quadrature sum
4. Computes percentage uncertainties
5. Saves CSV in output/
6. Saves plots as PDF in output/

Expected structure:

.
├── Standard/
│   └── x_sep.pl_375
├── delta/
│   ├── M10p/
│   │   └── x_sep.pl_375
│   └── P10p/
│       └── x_sep.pl_375
├── xptar/
│   ├── M10p/
│   │   └── x_sep.pl_375
│   └── P10p/
│       └── x_sep.pl_375
├── yptar/
│   ├── M10p/
│   │   └── x_sep.pl_375
│   └── P10p/
│       └── x_sep.pl_375
└── systematic_uncertainty.py
"""

import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ============================================================
# CONFIGURATION
# ============================================================

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

OUTPUT_DIR = os.path.join(BASE_DIR, "output")

os.makedirs(OUTPUT_DIR, exist_ok=True)

STANDARD_FILE = os.path.join(
    BASE_DIR,
    "Standard",
    "x_sep.pl_375"
)

SYSTEMATIC_FOLDERS = [
    "delta",
    "xptar",
    "yptar",
]

#VARIATIONS = ["M10p", "P10p"]
VARIATIONS = ["M10p"]

FILE_NAME = "x_sep.pl_375"

# Columns:
# sigT, sigT_err, sigL, sigL_err,
# sigLT, sigLT_err, sigTT, sigTT_err

CROSS_SECTIONS = {
    "sigT": 0,
    "sigL": 2,
    "sigLT": 4,
    "sigTT": 6,
}

# ============================================================
# LOAD STANDARD FILE
# ============================================================

if not os.path.exists(STANDARD_FILE):
    raise FileNotFoundError(
        f"Standard file not found:\n{STANDARD_FILE}"
    )

standard_data = np.loadtxt(STANDARD_FILE)

n_tbins = standard_data.shape[0]

t_bins = np.arange(1, n_tbins + 1)

# ============================================================
# STORE DIFFERENCES
# ============================================================

diff_store = {
    key: [] for key in CROSS_SECTIONS.keys()
}

# ============================================================
# LOOP OVER ALL SYSTEMATICS
# ============================================================

for syst in SYSTEMATIC_FOLDERS:

    for var in VARIATIONS:

        current_file = os.path.join(
            BASE_DIR,
            syst,
            var,
            FILE_NAME
        )

        if not os.path.exists(current_file):
            print(f"WARNING: Missing file -> {current_file}")
            continue

        varied_data = np.loadtxt(current_file)

        for xs_name, col in CROSS_SECTIONS.items():

            standard = standard_data[:, col]
            varied = varied_data[:, col]

            difference = varied - standard

            diff_store[xs_name].append(difference)

# ============================================================
# QUADRATURE SUM
# ============================================================

quadrature_unc = {}

for xs_name in CROSS_SECTIONS.keys():

    diffs = np.array(diff_store[xs_name])

    quad = np.sqrt(np.sum(diffs**2, axis=0))

    quadrature_unc[xs_name] = quad

# ============================================================
# PERCENTAGE UNCERTAINTY
# ============================================================

percentage_unc = {}

for xs_name, col in CROSS_SECTIONS.items():

    standard = standard_data[:, col]

    percent = np.where(
        standard != 0,
        100.0 * quadrature_unc[xs_name] / np.abs(standard),
        0.0
    )

    percentage_unc[xs_name] = percent

# ============================================================
# SAVE CSV
# ============================================================

csv_dict = {
    "t_bin": t_bins
}

for xs_name in CROSS_SECTIONS.keys():

    csv_dict[f"{xs_name}_percent_unc"] = percentage_unc[xs_name]

df = pd.DataFrame(csv_dict)

csv_output = os.path.join(
    OUTPUT_DIR,
    "systematic_percentage_uncertainties.csv"
)

df.to_csv(csv_output, index=False, float_format="%0.6f")

print(f"\nSaved CSV file:")
print(csv_output)

# ============================================================
# PLOTS
# ============================================================

for xs_name in CROSS_SECTIONS.keys():

    plt.figure(figsize=(7, 5))

    plt.plot(
        t_bins,
        percentage_unc[xs_name],
        marker='o',
        linewidth=2
    )

    plt.xlabel("t-bin")
    plt.ylabel("Percentage Uncertainty (%)")
    plt.title(f"{xs_name} Systematic Uncertainty")

    plt.grid(True)

    pdf_name = os.path.join(
        OUTPUT_DIR,
        f"{xs_name}_systematic_uncertainty.pdf"
    )

    plt.savefig(
        pdf_name,
        bbox_inches="tight"
    )

    print(f"Saved plot:")
    print(pdf_name)

    plt.close()

print("\nDone.")
