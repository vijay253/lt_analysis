import pandas as pd
import numpy as np

file_path = "output/PionCoinTimeCut.ods"
df = pd.read_excel(file_path, engine="odf")

columns = {
    "Low": {
        "loose": "Loose Syst at 286",
        "tight": "Tight Syst at 286",
    },
    "Mid": {
        "loose": "Loose Syst at 629",
        "tight": "Tight Syst at 629",
    },
    "High": {
        "loose": "Loose Syst at 781",
        "tight": "Tight Syst at 781",
    }
}

# -------- CALCULATE & PRINT --------
for eps, cols in columns.items():
    loose = pd.to_numeric(df[cols["loose"]], errors="coerce")
    tight = pd.to_numeric(df[cols["tight"]], errors="coerce")

    # Combine Loose + Tight into ONE array
    combined = pd.concat([loose, tight]).dropna()

    mean = combined.mean()
    std  = combined.std(ddof=1)   # sample std

    print(f"{eps} epsilon: mean = {mean:.6e}, std = {std:.6e}")
