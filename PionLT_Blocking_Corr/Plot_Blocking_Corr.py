import pandas as pd
import matplotlib.pyplot as plt

csv_file = "output/PionLT_Summer19_Blocking_corr.csv"

df = pd.read_csv(csv_file)

run_numbers = df["Run"]
blocking_corr = df["BlockingCorrection"]

plt.figure(figsize=(10, 6))
#plt.plot(run_numbers, blocking_corr, marker='o', linestyle='-', color='blue')
plt.plot(run_numbers, blocking_corr, marker='^', linestyle='None', color='blue', markersize=1)

plt.xlabel("Run Number")
plt.ylabel("Blocking Corr.")
plt.title("PionLT_Summer19_Blocking Corr. vs. Run Number")
#plt.grid(True)

plt.ylim(0.7, 1.1)

plt.savefig("output/PionLT_Summer19_Blocking_Corr.png", dpi=300)

#plt.show()
