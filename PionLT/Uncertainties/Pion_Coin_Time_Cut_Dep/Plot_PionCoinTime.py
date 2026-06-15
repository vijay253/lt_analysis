import matplotlib.pyplot as plt
import pandas as pd

file_path = "PionCoinTimeCut.ods"
df = pd.read_excel(file_path, engine="odf")

columns_map = {
    "Low": {
        "loose_diff": "Loose Syst at 286",
        "tight_diff": "Tight Syst at 286"
    },
    "Mid": {
        "loose_diff": "Loose Syst at 629",
        "tight_diff": "Tight Syst at 629"
    },
    "High": {
        "loose_diff": "Loose Syst at 781",
        "tight_diff": "Tight Syst at 781"
    }
}

colors = {"Low": "blue", "Mid": "green", "High": "red"}

plt.figure(figsize=(10,6))

plt.axhline(y=0, color='black', linestyle='--', linewidth=1)

for eps, cols in columns_map.items():
    y_loose = df[cols["loose_diff"]].iloc[:112]
    y_tight = df[cols["tight_diff"]].iloc[:112]

    x = range(len(y_loose))
  
    plt.plot(y_loose, color=colors[eps], marker='*', linestyle='', markersize=4, label=f"{eps} -10%")
    plt.plot(y_tight, color=colors[eps], marker='o', linestyle='', markersize=2, label=f"{eps} +10%")

plt.xlabel("Total Bin (7*16)")
plt.ylabel("Difference (Std - Cut)")
plt.title("Unsep X differences ±10% for Low, Mid, High epsilon")
plt.legend()
#plt.grid(True)
plt.tight_layout()

plt.savefig("Pion_Coin_Time_Syst.pdf", dpi=300)
print("Pion_Coin_Time_Syst.pdf")
#plt.show()
