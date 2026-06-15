import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# =========================================================
# Directories
# =========================================================
base_dir = Path("/volatile/hallc/c-kaonlt/vijay/Analysis/Uncertainties/Model_Systematics")
std_dir  = base_dir / "Standard"

filename = "x_sep.pl_375"

output_dir = Path("output")
output_dir.mkdir(exist_ok=True)

# =========================================================
# Load standard data
# =========================================================
std_data = np.loadtxt(std_dir / filename)

# x-axis (1 to 7)
x = np.arange(1, len(std_data) + 1)

# =========================================================
# What is extracted (panels)
# =========================================================
separated_xsec = {
    "sigL":  {"col": 2, "title": r"Effect on extracted $\sigma_L$"},
    "sigT":  {"col": 0, "title": r"Effect on extracted $\sigma_T$"},
    "sigLT": {"col": 4, "title": r"Effect on extracted $\sigma_{LT}$"},
    "sigTT": {"col": 6, "title": r"Effect on extracted $\sigma_{TT}$"},
}

# =========================================================
# Model variations applied in input
# =========================================================
variations = {
    "sigL":  ("sigL_variations",  "plus_10percent", "minus_10percent"),
    "sigT":  ("sigT_variations",  "plus_10percent", "minus_10percent"),
    "sigLT": ("sigLT_variations", "plus_0p14",      "minus_0p14"),
    "sigTT": ("sigTT_variations", "plus_0p36",      "minus_0p36"),
}

# =========================================================
# Labels for legend (clean physics format)
# =========================================================
plus_label = {
    "sigL":  "+10%",
    "sigT":  "+10%",
    "sigLT": "+0.14",
    "sigTT": "+0.36"
}

minus_label = {
    "sigL":  "-10%",
    "sigT":  "-10%",
    "sigLT": "-0.14",
    "sigTT": "-0.36"
}

# =========================================================
# Plot setup
# =========================================================
fig, axes = plt.subplots(2, 2, figsize=(16, 12))
axes = axes.flatten()

# =========================================================
# Loop over extracted cross sections
# =========================================================
for ax, (name, cfg) in zip(axes, separated_xsec.items()):

    col = cfg["col"]
    sigma_std = std_data[:, col]

    all_curves = []

    # =====================================================
    # CSV storage (UNCHANGED FORMAT)
    # =====================================================
    csv_columns = [x.astype(int)]
    csv_header = ["bin"]

    # =====================================================
    # Loop over all model variations
    # =====================================================
    for var_name, (vdir, plus_sub, minus_sub) in variations.items():

        plus_path  = base_dir / vdir / plus_sub / filename
        minus_path = base_dir / vdir / minus_sub / filename

        plus_data  = np.loadtxt(plus_path)
        minus_data = np.loadtxt(minus_path)

        sigma_plus  = plus_data[:, col]
        sigma_minus = minus_data[:, col]

        # % variation
        d_plus  = ((sigma_plus  - sigma_std) / sigma_std) * 100.0
        d_minus = ((sigma_minus - sigma_std) / sigma_std) * 100.0

        all_curves.append(d_plus)
        all_curves.append(d_minus)

        # =================================================
        # Plot WITH proper labels + markers
        # =================================================
        ax.plot(
            x, d_plus,
            marker='o',
            linestyle='-',
            linewidth=1,
            markersize=5,
            label=f"{var_name} {plus_label[var_name]}"
        )

        ax.plot(
            x, d_minus,
            marker='s',
            linestyle='--',
            linewidth=1,
            markersize=5,
            label=f"{var_name} {minus_label[var_name]}"
        )

        # =================================================
        # CSV (UNCHANGED FORMAT)
        # =================================================
        csv_columns.append(d_plus)
        csv_columns.append(d_minus)
        csv_header.append(f"{var_name}_plus_percent")
        csv_header.append(f"{var_name}_minus_percent")

    # =====================================================
    # RMS calculation
    # =====================================================
    all_curves = np.array(all_curves)   # shape (8, 7)

    rms_per_bin = np.sqrt(np.mean(all_curves**2, axis=0))
    total_rms   = np.sqrt(np.mean(rms_per_bin**2))

    # =====================================================
    # RMS annotation
    # =====================================================
    ax.text(
        0.3, 0.15,
        f"RMS = {total_rms:.4f}%",
        transform=ax.transAxes,
        fontsize=12,
        bbox=dict(facecolor='white', alpha=0.7, edgecolor='none')
    )

    # =====================================================
    # Styling
    # =====================================================
    ax.axhline(0.0, color='black', linewidth=1)
    ax.set_title(cfg["title"])
    ax.set_xlabel("t-bin")
    ax.set_ylabel("% Variation")
    ax.grid(True)

    ax.legend(fontsize=8, ncol=2)

    # =====================================================
    # Save CSV (UNCHANGED)
    # =====================================================
    csv_file = output_dir / f"{name}_model_variations.csv"

    np.savetxt(
        csv_file,
        np.column_stack(csv_columns),
        delimiter=",",
        header=",".join(csv_header),
        comments="",
        fmt="%.6f"
    )

    print(f"Saved CSV: {csv_file}")

# =========================================================
# Save figure
# =========================================================
plt.tight_layout()

pdf_file = output_dir / "all_model_variation_effects_RMS.pdf"
plt.savefig(pdf_file)

plt.close()

print(f"Saved PDF: {pdf_file}")
