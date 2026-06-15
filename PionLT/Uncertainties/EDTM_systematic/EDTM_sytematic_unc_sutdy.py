import numpy as np
import matplotlib.pyplot as plt

# -----------------------------
# INPUT DATA (replace with yours)
# -----------------------------

# EDTM-corrected yields (10 runs)
Y = np.array([
    1.002, 0.998, 1.001, 0.999, 1.003,
    1.000, 0.997, 1.002, 1.001, 0.998
])

# Coincidence trigger rates (kHz or Hz, consistent units)
rate = np.array([
    2.1, 2.5, 3.0, 3.5, 4.0,
    4.5, 5.0, 5.5, 6.0, 6.5
])

# -----------------------------
# STEP 1: Mean yield
# -----------------------------
Y_mean = np.mean(Y)

# -----------------------------
# STEP 2: Normalized yield
# -----------------------------
R = Y / Y_mean

# -----------------------------
# STEP 3: RMS calculation
# -----------------------------
RMS = np.sqrt(np.mean((R - 1)**2))

# convert to percent
RMS_percent = RMS * 100

# -----------------------------
# STEP 4: Plot
# -----------------------------
plt.figure(figsize=(7,5))
plt.scatter(rate, R, color='blue', label='Runs')

# horizontal line at 1
plt.axhline(1.0, color='red', linestyle='--', label='Mean = 1')

# labels
plt.xlabel('Coincidence Trigger Rate')
plt.ylabel(r'$Y_i / \langle Y \rangle$')
plt.title('EDTM Stability Check')

# show RMS on plot
plt.text(
    0.05, 0.95,
    f'RMS = {RMS_percent:.3f} %',
    transform=plt.gca().transAxes,
    fontsize=12,
    verticalalignment='top',
    bbox=dict(boxstyle="round", facecolor="white", alpha=0.8)
)

plt.legend()
plt.grid(True)

plt.tight_layout()
#plt.show()
plt.savefig("EDTM_stability.pdf", bbox_inches='tight')
plt.close()

# -----------------------------
# STEP 5: Print result
# -----------------------------
print(f"Mean yield = {Y_mean:.6f}")
print(f"RMS (fraction) = {RMS:.6f}")
print(f"EDTM systematic uncertainty ≈ {RMS_percent:.3f} %")
