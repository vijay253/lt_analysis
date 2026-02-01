import ROOT
import csv

#runlist_file = "/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/tmp"    
runlist_file = "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1"    
#runlist_file = "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q2"    
#root_dir = "/volatile/hallc/c-kaonlt/vijay/ROOTfiles/"

root_dir = "/cache/hallc/kaonlt/vijay/CoinBlockingStudyROOTfiles/"       
#root_dir = "/cache/hallc/kaonlt/vijay/ROOTfiles/"       

tree_name = "T"               
timing_var = "CTime.CoinTime_RAW_ROC1"           
#cut for deno for only Good Coint Events
lower_cutd = -600                 
upper_cutd = 900
#cut for num
lower_cut = -35                 
upper_cut = 100

#systematic study
#lower_cut = -37                 
#upper_cut = 102

output_csv = "output/PionLT_Summer19_Blocking_corr.csv"
#output_csv = "output/PionLT_Summer19_Blocking_corr_Systematic_wider.csv"
#output_csv = "output/PionLT_Summer19_Blocking_corr_Systematic_narrow.csv"

results = []

with open(runlist_file, "r") as f:
    run_numbers = [line.strip() for line in f if line.strip()]

# Loop over runs
for run in run_numbers:
    filename = f"{root_dir}/Kaon_coin_replay_production_{run}_-1.root"
    try:
        file = ROOT.TFile.Open(filename)
        if not file or file.IsZombie():
            print(f"Could not open {filename}")
            continue

        tree = file.Get(tree_name)
        if not tree:
            print(f"Tree '{tree_name}' not found in {filename}")
            file.Close()
            continue

        # Denominator = all good events
        cut_strd = f"{timing_var} > {lower_cutd} && {timing_var} < {upper_cutd}"
        denom = tree.GetEntries(cut_strd)

        # Numerator = events within timing cuts
        cut_str = f"{timing_var} > {lower_cut} && {timing_var} < {upper_cut}"
        num = tree.GetEntries(cut_str)

        # Blocking correction
        correction = num / denom if denom > 0 else 0

        results.append((run, num, denom, correction))

        print(f"Run {run}: Num={num}, Den={denom}, Corr={correction:.4f}")

        file.Close()

    except Exception as e:
        print(f"Error processing run {run}: {e}")

# Save to CSV
with open(output_csv, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Run", "Numerator", "Denominator", "BlockingCorrection"])
    writer.writerows(results)

print(f"\nResults saved to {output_csv}")
