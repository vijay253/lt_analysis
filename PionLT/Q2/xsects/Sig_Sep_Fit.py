import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# -----------------------
# Constants
# -----------------------
Q2 = 0.425
mp = 0.938272  # Proton mass in GeV (can adjust precision)
# -----------------------
# Function to read data from file
# -----------------------
def read_column(filename, col_index):
    '''
    Reads a single column from a text file (0-based indexing)

    '''
    return np.loadtxt(filename, usecols=[col_index])


# Function to compute ftav for each xx
def compute_ftav(xx):
    tav = (0.0735 + 0.028 * np.log(Q2)) * Q2
    return (xx - tav) / tav

# Function to compute Wfactor
def compute_Wfactor(w):
    return 1.0 / ((w**2 - mp**2)**2)

# -----------------------
# Define models
# -----------------------

# 1) σ_T = (a0 + a1 * ftav) * Wfactor
def model_sigT(x_tuple, a0, a1, a2, a3):
    xx, w, q2 = x_tuple
    ftav = compute_ftav(xx)
    Wfactor = compute_Wfactor(w)
    #return (a0 + a1 * ftav) * (1/q2**2) * Wfactor
   # return (a0 + a1 * np.exp(-ftav)) * (1/q2**2) * Wfactor
    return (a0 + a1 * np.exp(a2*-ftav) + a3*np.exp(-ftav**2)) * (1/q2**2) * Wfactor

# 2) σ_L = (a0 * exp(a1 * xx)) * Wfactor
def model_sigL(x_tuple, a0, a1, a2, a3):
    xx, w, q2 = x_tuple
    Wfactor = compute_Wfactor(w)
#    return (a0 * np.exp(a1 * xx**1)) * (1/q2**2) * Wfactor
#    return (a0 * np.exp(a1 * xx)*(1-a1*xx*np.exp(a1 * xx))**4) * (1/q2**2) * Wfactor
    return (a0 * np.exp(a1 * xx)*(1-a2*xx*np.exp(a3 * xx))**4) * (1/q2**2) * Wfactor

# 3) σ_TT = [(a0/xx^3 * exp(a1*xx) + a2/xx) * sin²(theta_deg)] * Wfactor
def wrapper_sigTT(x_tuple, a0, a1, a2):
    xx, theta, w = x_tuple
#    theta_deg = np.degrees(theta)  # rad → deg
    theta_deg = theta  # rad → deg
    Wfactor = compute_Wfactor(w)
    return (a0 / (xx**3) * np.exp(a1 * xx) + a2 / xx**4) * (np.sin(np.radians(theta_deg))**2) * Wfactor

# 4) σ_LT = [(a0/xx^2 * exp(a1/xx) + a2/xx) * sin(theta_deg)] * Wfactor
def wrapper_sigLT(x_tuple, a0, a1, a2):
    xx, theta, w = x_tuple
   # theta_deg = np.degrees(theta)  # rad → deg
    theta_deg = theta  # rad → deg
    Wfactor = compute_Wfactor(w)
    return (a0 / (xx**2) * np.exp(a1 / xx) + a2 / xx) * np.sin(np.radians(theta_deg)) * Wfactor

# -----------------------
# Example experimental data (replace with your own!)
# -----------------------
xx_data    = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 4)
q2_data    = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 2)
theta_data = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 6)  # radians
w_data     = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 0)   # example W values

# Separate y and errors for each model
yT, yT_err   = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 0),read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 1)
yL, yL_err   = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 2),read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 3)
yLT, yLT_err   = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 4),read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 5)
yTT, yTT_err   = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 6),read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src//xsects/x_sep.pl_425", 7)

# -----------------------
# Fitting
# -----------------------

# σ_T
popt_T, pcov_T = curve_fit(model_sigT, (xx_data, w_data, q2_data), yT, p0=[20.53867119, 10.42947234, 1.28146019, 3.3690712], sigma=yT_err, absolute_sigma=True)
perr_T = np.sqrt(np.diag(pcov_T))

# σ_L
popt_L, pcov_L = curve_fit(model_sigL, (xx_data, w_data, q2_data), yL, p0=[19.58317071, -16.66034077, -18.37201786, -23.52293292], sigma=yL_err, absolute_sigma=True)
perr_L = np.sqrt(np.diag(pcov_L))

# σ_TT
popt_TT, pcov_TT = curve_fit(wrapper_sigTT, (xx_data, theta_data, w_data), yTT, p0=[-4.77079121e-02, 2.43569556e+01, 8.27884325e-04],
                             sigma=yTT_err, absolute_sigma=True)
perr_TT = np.sqrt(np.diag(pcov_TT))

# σ_LT
popt_LT, pcov_LT = curve_fit(wrapper_sigLT, (xx_data, theta_data, w_data), yLT, p0=[0.11722934, 0.0022152, -0.42492742],
                             sigma=yLT_err, absolute_sigma=True)
perr_LT = np.sqrt(np.diag(pcov_LT))

# -----------------------
# Print results
# -----------------------
print("\nFit results:")
print("σ_T  params:", popt_T, "±", perr_T)
print("σ_L  params:", popt_L, "±", perr_L)
print("σ_LT params:", popt_LT, "±", perr_LT)
print("σ_TT params:", popt_TT, "±", perr_TT)

# -----------------------
# Plot fits
# -----------------------
xx_fit = np.linspace(min(xx_data), max(xx_data), 200)
w_fit  = np.interp(xx_fit, xx_data, w_data)
q2_fit  = np.interp(xx_fit, xx_data, q2_data)
theta_fit = np.interp(xx_fit, xx_data, theta_data)

fig, axs = plt.subplots(2, 2, figsize=(10, 8))

# σ_L
axs[0, 0].errorbar(xx_data, yL, yerr=yL_err, fmt='o', color='red', label="Data")
axs[0, 0].plot(xx_fit, model_sigL((xx_fit, w_fit, q2_fit), *popt_L), label="Fit")
axs[0, 0].set_title("σ_L")
axs[0, 0].set_ylim([5.0, 16.0])
axs[0, 0].legend()

# σ_T
axs[0, 1].errorbar(xx_data, yT, yerr=yT_err, fmt='o', color='red', label="Data")
axs[0, 1].plot(xx_fit, model_sigT((xx_fit, w_fit, q2_fit), *popt_T), label="Fit")
axs[0, 1].set_title("σ_T")
axs[0, 1].set_ylim([5.0, 16.0])
axs[0, 1].legend()

# σ_LT
axs[1, 0].errorbar(xx_data, yLT, yerr=yLT_err, fmt='o', color='red', label="Data")
axs[1, 0].plot(xx_fit, wrapper_sigLT((xx_fit, theta_fit, w_fit), *popt_LT), label="Fit")
axs[1, 0].set_title("σ_LT")
axs[1, 0].set_ylim([-5.0, 4.0])
axs[1, 0].legend()

# σ_TT
axs[1, 1].errorbar(xx_data, yTT, yerr=yTT_err, fmt='o', color='red', label="Data")
axs[1, 1].plot(xx_fit, wrapper_sigTT((xx_fit, theta_fit, w_fit), *popt_TT), label="Fit")
axs[1, 1].set_title("σ_TT")
axs[1, 1].set_ylim([-5.0, 4.0])
axs[1, 1].legend()

plt.tight_layout()
plt.savefig("Sep_Fit.pdf")
#plt.show()
