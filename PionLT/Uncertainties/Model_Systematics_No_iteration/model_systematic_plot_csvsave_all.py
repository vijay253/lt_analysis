import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# =========================================================
# Base directories
# =========================================================
base_dir = Path("/volatile/hallc/c-kaonlt/vijay/Analysis/Uncertainties/Model_Systematics")
std_dir  = base_dir / "Standard"

filename = "x_sep.pl_375"

output_dir = Path("output")
output_dir.mkdir(exist_ok=True)

# =========================================================
# Column definitions in x_sep.pl_375
# =========================================================
#
# col 0 -> sigT
# col 1 -> sigTT
# col 2 -> sigL
# col 3 -> sigLT
#
# =========================================================

# =========================================================
# Variations to apply
# =========================================================
variations = {
    "sigL": {
        "variation_dir": "sigL_variations",
        "plus_dir": "plus_10percent",
        "minus_dir": "minus_10percent",
        "label_plus": r"$\sigma_L$ +10%",
        "label_minus": r"$\sigma_L$ -10%",
    },

    "sigT": {
        "variation_dir": "sigT_variations",
        "plus_dir": "plus_10percent",
        "minus_dir": "minus_10percent",
        "label_plus": r"$\sigma_T$ +10%",
        "label_minus": r"$\sigma_T$ -10%",
    },

    "sigLT": {
        "variation_dir": "sigLT_variations",
        "plus_dir": "plus_0p14",
        "minus_dir": "minus_0p14",
        "label_plus": r"$\sigma_{LT}$ +0.14",
        "label_minus": r"$\sigma_{LT}$ -0.14",
    },

    "sigTT": {
        "variation_dir": "sigTT_variations",
        "plus_dir": "plus_0p36",
        "minus_dir": "minus_0p36",
        "label_plus": r"$\sigma_{TT}$ +0.36",
        "label_minus": r"$\sigma_{TT}$ -0.36",
    }
}

# =========================================================
# Separated cross sections to STUDY
# =========================================================
#
# These are the extracted separated cross sections
# whose sensitivity we want to investigate.
#
# Example:
#   How does extracted sigL change when
#   sigL/sigT/sigLT/sigTT models are varied?
#
# =========================================================
separated_xsec = {
    "sigL": {
        "column": 2,
        "title": r"Effect on Extracted $\sigma_L$"
    },

    "sigT": {
        "column": 0,
        "title": r"Effect on Extracted $\sigma_T$"
    },

    "sigLT": {
        "column": 3,
        "title": r"Effect on Extracted $\sigma_{LT}$"
    },

    "sigTT": {
        "column": 1,
        "title": r"Effect on Extracted $\sigma_{TT}$"
    }
}

# =========================================================
# Load standard model data
# =========================================================
std_data = np.loadtxt(std_dir / filename)

# x-axis
#x = np.arange(len(std_data))
x = np.arange(1, len(std_data) + 1)

# =========================================================
# Create 4-panel figure
# =========================================================
fig, axes = plt.subplots(2, 2, figsize=(16, 12))
axes = axes.flatten()

# =========================================================
# Loop over separated cross sections
# =========================================================
for ax, (sep_name, sep_cfg) in zip(axes, separated_xsec.items()):

    sep_col = sep_cfg["column"]

    # Standard extracted separated cross section
    sigma_std = std_data[:, sep_col]

    # =====================================================
    # CSV storage
    # =====================================================
    csv_columns = [x.astype(int)]

    csv_header = ["bin"]

    # =====================================================
    # Loop over ALL model variations
    # =====================================================
    for var_name, var_cfg in variations.items():

        # Directories
        plus_dir = (
            base_dir
            / var_cfg["variation_dir"]
            / var_cfg["plus_dir"]
        )

        minus_dir = (
            base_dir
            / var_cfg["variation_dir"]
            / var_cfg["minus_dir"]
        )

        # Load data
        plus_data  = np.loadtxt(plus_dir / filename)
        minus_data = np.loadtxt(minus_dir / filename)

        # Extract SAME separated xsec column
        sigma_plus  = plus_data[:, sep_col]
        sigma_minus = minus_data[:, sep_col]

        # =================================================
        # Percentage variation
        # =================================================
        d_plus = (
            (sigma_plus - sigma_std)
            / sigma_std
        ) * 100.0

        d_minus = (
            (sigma_minus - sigma_std)
            / sigma_std
        ) * 100.0

        # =================================================
        # Store for CSV
        # =================================================
        csv_columns.append(d_plus)
        csv_columns.append(d_minus)

        csv_header.append(f"{var_name}_plus_percent")
        csv_header.append(f"{var_name}_minus_percent")

        # =================================================
        # Plot
        # =================================================
        ax.plot(
            x,
            d_plus,
            marker="o",
            linestyle="-",
            label=var_cfg["label_plus"]
        )

        ax.plot(
            x,
            d_minus,
            marker="s",
            linestyle="--",
            label=var_cfg["label_minus"]
        )

    # =====================================================
    # Final subplot formatting
    # =====================================================
    ax.axhline(0.0, color="black", linewidth=1)

    ax.set_title(sep_cfg["title"])

    ax.set_xlabel("t Bin")
    ax.set_ylabel("% Variation")

    ax.grid(True)

    ax.legend(fontsize=8)

    # =====================================================
    # Save CSV for this separated xsec
    # =====================================================
    csv_data = np.column_stack(csv_columns)

    csv_output = output_dir / f"{sep_name}_all_model_variations.csv"

    np.savetxt(
        csv_output,
        csv_data,
        delimiter=",",
        header=",".join(csv_header),
        comments="",
        fmt="%.6f"
    )

    print(f"Saved CSV: {csv_output}")

# =========================================================
# Final figure formatting
# =========================================================
plt.tight_layout()

combined_pdf = output_dir / "all_model_variation_effects.pdf"

plt.savefig(combined_pdf)

plt.close()

print(f"\nSaved combined PDF: {combined_pdf}")
