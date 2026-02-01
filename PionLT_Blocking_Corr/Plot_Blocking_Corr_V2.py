import pandas as pd
import matplotlib.pyplot as plt
import re
import os

#csv_file = "output/PionLT_Summer19_Blocking_corr.csv"       
#csv_file = "output/PionLT_Summer19_Q1_Blocking_corr_V2.csv"       
csv_file = "output/PionLT_Summer19_Q2_Blocking_corr_V2.csv"       
coinrate_folder = "/cache/hallc/kaonlt/vijay/CoinBlockingStudyREPORT_OUTPUT/Production/"   

save_plot = True
#output_file = "output/PionLT_Summer19_Blocking_Corr_V2.png"
#output_file = "output/PionLT_Summer19_Q1_Blocking_Corr_V2.png"
output_file = "output/PionLT_Summer19_Q2_Blocking_Corr_V2.png"

df = pd.read_csv(csv_file)

if not {'Run', 'BlockingCorrection'}.issubset(df.columns):
    raise ValueError("CSV must contain 'Run' and 'BlockingCorrection' columns.")

df['Run'] = df['Run'].astype(str)

def extract_coin_rate(file_path):
    pattern = re.compile(r"KLT_COIN_Trigger_Rate\s*:\s*([-+]?\d*\.\d+|\d+)\s*kHz", re.IGNORECASE)
    with open(file_path, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                return float(match.group(1))
    return None  # Return None if not found

coin_rates = []
missing_runs = []
unmatched_runs = []

for run in df['Run']:
    file_path = os.path.join(coinrate_folder, f"Kaon_replay_production_{run}_-1.report")
    if os.path.exists(file_path):
        rate = extract_coin_rate(file_path)
        if rate is not None:
            coin_rates.append(rate)
        else:
            coin_rates.append(None)
            unmatched_runs.append(run)
    else:
        coin_rates.append(None)
        missing_runs.append(run)

# Add rates to DataFrame
df['CoinRate_kHz'] = coin_rates

# Report missing or unmatched files
if missing_runs:
    print("Warning: Missing text files for runs:", ", ".join(missing_runs))
if unmatched_runs:
    print("Warning: 'KLT_COIN_Trigger_Rate' line not found in files for runs:", ", ".join(unmatched_runs))

# Drop rows without valid Coin Rate
df = df.dropna(subset=['CoinRate_kHz'])

df["Run"] = df["Run"].astype(int)
df = df.sort_values("Run")

# ============================
# Plotting
# ============================
plt.figure(figsize=(12, 5))

# --- Plot 1: Run Number vs Blocking Correction ---
plt.subplot(1, 2, 1)
plt.plot(df['Run'], df['BlockingCorrection'], marker='^', linestyle='none', color='b',  markersize=1)
plt.title("PionLT_Summer19_Blocking Corr. vs. Run Number")
plt.xlabel("Run Number")
plt.ylabel("Blocking Corr.")
plt.ylim(0.7, 1.1)
#plt.grid(True, linestyle='--', alpha=0.7)

# --- Plot 2: Coin Rate vs Blocking Correction ---
plt.subplot(1, 2, 2)
plt.plot(df['CoinRate_kHz'], df['BlockingCorrection'], marker='^', linestyle='none', color='r', markersize=1)
plt.title("PionLT_Summer19_Blocking Corr. vs. Coin Rate")
plt.xlabel("Coin Rate (kHz)")
plt.ylabel("Blocking Corr.")
plt.ylim(0.7, 1.1)
#plt.grid(True, linestyle='--', alpha=0.7)

plt.tight_layout()

if save_plot:
    plt.savefig(output_file, dpi=300)
    print(f"Plot saved as {output_file}")

#plt.show()
