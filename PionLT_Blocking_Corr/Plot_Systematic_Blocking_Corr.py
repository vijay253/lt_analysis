import pandas as pd
import matplotlib.pyplot as plt
import os

# File names
#standard_file = "output/PionLT_Summer19_Q1_Blocking_corr_V2.csv"
#wider_file = "output/PionLT_Summer19_Q1_Blocking_corr_Systematic_wider.csv"
#narrow_file = "output/PionLT_Summer19_Q1_Blocking_corr_Systematic_narrow.csv"

standard_file = "output/PionLT_Summer19_Q2_Blocking_corr_V2.csv"
wider_file = "output/PionLT_Summer19_Q2_Blocking_corr_Systematic_wider.csv"
narrow_file = "output/PionLT_Summer19_Q2_Blocking_corr_Systematic_narrow.csv"

#output_file = "output/blocking_diff_Q1.png"
output_file = "output/blocking_diff_Q2.png"

# Load data
df_std = pd.read_csv(standard_file)
df_wider = pd.read_csv(wider_file)
df_narrow = pd.read_csv(narrow_file)

# Make sure they are aligned by Run number
df_std = df_std.sort_values("Run")
df_wider = df_wider.sort_values("Run")
df_narrow = df_narrow.sort_values("Run")

# Compute differences in BlockingCorrection
diff_wider = df_wider["BlockingCorrection"] - df_std["BlockingCorrection"]
diff_narrow = df_narrow["BlockingCorrection"] - df_std["BlockingCorrection"]
runs = df_std["Run"]

# Plot
plt.figure(figsize=(10, 6))

plt.plot(runs, diff_wider,marker='^', linestyle='none', label="4 ns Wider - Standard", markersize=1)
plt.plot(runs, diff_narrow,marker='^', linestyle='none', label="4 ns Narrow - Standard", markersize=1)

plt.xlabel("Run Number")
plt.ylabel("Diff. in BlockingCorrection")
#plt.title("Systematic Check (Blocking Corr. Differences) For Q\u00b2 = 0.375 GeV\u00b2")
plt.title("Systematic Check (Blocking Corr. Differences) For Q\u00b2 = 0.425 GeV\u00b2")
plt.ylim(-0.02, 0.02)
plt.legend()
#plt.grid(True)

plt.tight_layout()

# Create folder if it does not exist
os.makedirs(os.path.dirname(output_file), exist_ok=True)

plt.savefig(output_file, dpi=300)
plt.close()

print(f"Saved figure to {output_file}")

#plt.show()
