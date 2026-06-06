import os
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# -----------------------
# Constants
# -----------------------
Q2 = 0.425
mp = 0.938272   # Proton mass in GeV

# -----------------------
# File paths
# -----------------------
base_dir = os.path.dirname(os.path.abspath(__file__))

par_file_in  = os.path.join(base_dir, "/group/c-kaonlt/USERS/vijay/simc_gfortran/par.pl")
par_file_out = os.path.join(base_dir, "next_it_par/par.pl")

# -----------------------
# Function to read data
# -----------------------
def read_column(filename, col_index):
    """
    Reads a single column from a text file (0-based indexing)
    """
    return np.loadtxt(filename, usecols=[col_index])

# -------------------------------------------------
# Read par.pl
#
# Format:
# total 14 rows
# col1 = parameter
# col2 = error
# col3 = number (copied back unchanged)
#
# rows:
# 0-3 sigma_T
# 4-7 sigma_L
# 8-10 sigma_LT
# 11-13 sigma_TT
# -------------------------------------------------
def read_parameters(fname):
    data = np.loadtxt(fname)
    
    pT = data[0:4, 0]
    eT = data[0:4, 1]
    
    pL = data[4:8, 0]
    eL = data[4:8, 1]
    
    pLT = data[8:11, 0]
    eLT = data[8:11, 1]
    
    pTT = data[11:14, 0]
    eTT = data[11:14, 1]
    
    return data, pT, eT, pL, eL, pLT, eLT, pTT, eTT

# -------------------------------------------------
# Save updated parameters
# -------------------------------------------------
def save_parameters(fname, old_data,
                  popt_T, perr_T,
                  popt_L, perr_L,
                  popt_LT, perr_LT,
                  popt_TT, perr_TT):

    new_data = old_data.copy()

    # sigma_T
    new_data[0:len(popt_T), 0] = popt_T
    new_data[0:len(perr_T), 1] = perr_T

    # sigma_L
    new_data[4:4+len(popt_L), 0] = popt_L
    new_data[4:4+len(perr_L), 1] = perr_L

    # sigma_LT
    new_data[8:8+len(popt_LT), 0] = popt_LT
    new_data[8:8+len(perr_LT), 1] = perr_LT

    # sigma_TT
    new_data[11:11+len(popt_TT), 0] = popt_TT
    new_data[11:11+len(perr_TT), 1] = perr_TT

    np.savetxt(fname, new_data, fmt="%18.10e %18.10e %6d")

# -----------------------
# Physics functions
# -----------------------
def compute_ftav(xx):
    tav = (0.0735 + 0.028 * np.log(Q2)) * Q2
    return (xx - tav) / tav

def compute_Wfactor(w):
    return 1.0 / ((w**2 - mp**2)**2)

# -----------------------
# Fit models
# -----------------------

# sigma_T
def model_sigT(x_tuple, a0, a1, a2, a3):
    xx, w, q2 = x_tuple
    ftav = compute_ftav(xx)
    Wfactor = compute_Wfactor(w)
#    return (a0 + a1 * np.exp(a2*-ftav) + a3*np.exp(-ftav**2)) * (1/q2**2) * Wfactor
    return (a0 + a1 * ftav**1 + a2 * ftav**2) * Wfactor

# sigma_L
def model_sigL(x_tuple, a0, a1, a2, a3):
    xx, w, q2 = x_tuple
    Wfactor = compute_Wfactor(w)
#    return (a0 * np.exp(a1 * xx)*(1-a2*xx*np.exp(a3 * xx))**4) * (1/q2**2) * Wfactor
    return (a0 * np.exp(a1 * xx)) * (xx/((-xx - 0.13957**2)**2 * (0.44**2 - xx)**2)) * Wfactor

# sigma_TT
def wrapper_sigTT(x_tuple, a0, a1, a2):
    xx, theta, w = x_tuple
    Wfactor = compute_Wfactor(w)
    return (a0 / xx**3 * np.exp(a1 * xx) + a2 / xx**4) * \
           (np.sin(np.radians(theta))**2) * Wfactor

# sigma_LT
def wrapper_sigLT(x_tuple, a0, a1, a2):
    xx, theta, w = x_tuple
    Wfactor = compute_Wfactor(w)
    return (a0 / xx**2 * np.exp(a1 / xx) + a2 / xx) * \
           np.sin(np.radians(theta)) * Wfactor

# -----------------------
# Read initial parameters
# -----------------------

par_data, p0_T, eT, p0_L, eL, p0_LT, eLT, p0_TT, eTT = read_parameters(par_file_in)

# -----------------------
# Experimental data
# -----------------------
xx_data    = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 4)
q2_data    = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 2)
theta_data = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 6)
w_data     = read_column("/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/averages/avek.425.dat", 0)

xsec_file = "/group/c-kaonlt/USERS/vijay/Analysis/Q2/src/xsects/x_sep.pl_425"

yT,  yT_err  = read_column(xsec_file, 0), read_column(xsec_file, 1)
yL,  yL_err  = read_column(xsec_file, 2), read_column(xsec_file, 3)
yLT, yLT_err = read_column(xsec_file, 4), read_column(xsec_file, 5)
yTT, yTT_err = read_column(xsec_file, 6), read_column(xsec_file, 7)

# -----------------------
# Perform fits
# -----------------------
popt_T,  pcov_T  = curve_fit(model_sigT,
                             (xx_data, w_data, q2_data),
                             yT,
                             p0=p0_T,
                             sigma=yT_err,
                             absolute_sigma=True)

popt_L,  pcov_L  = curve_fit(model_sigL,
                             (xx_data, w_data, q2_data),
                             yL,
                             p0=p0_L,
                             sigma=yL_err,
                             absolute_sigma=True)

popt_LT, pcov_LT = curve_fit(wrapper_sigLT,
                             (xx_data, theta_data, w_data),
                             yLT,
                             p0=p0_LT,
                             sigma=yLT_err,
                             absolute_sigma=True)

popt_TT, pcov_TT = curve_fit(wrapper_sigTT,
                             (xx_data, theta_data, w_data),
                             yTT,
                             p0=p0_TT,
                             sigma=yTT_err,
                             absolute_sigma=True)

# Errors
perr_T  = np.sqrt(np.diag(pcov_T))
perr_L  = np.sqrt(np.diag(pcov_L))
perr_LT = np.sqrt(np.diag(pcov_LT))
perr_TT = np.sqrt(np.diag(pcov_TT))

# -----------------------
# Print results
# -----------------------
print("\nFit results:")
print("σ_T :",  popt_T,  "±", perr_T)
print("σ_L :",  popt_L,  "±", perr_L)
print("σ_LT:", popt_LT, "±", perr_LT)
print("σ_TT:", popt_TT, "±", perr_TT)

# -----------------------
# Save new parameters
# -----------------------
save_parameters(par_file_out,
              par_data,
              popt_T, perr_T,
              popt_L, perr_L,
              popt_LT, perr_LT,
              popt_TT, perr_TT)

print("Updated parameters written to:", par_file_out)

# -----------------------
# Plot fits
# -----------------------
xx_fit    = np.linspace(min(xx_data), max(xx_data), 200)
w_fit     = np.interp(xx_fit, xx_data, w_data)
q2_fit    = np.interp(xx_fit, xx_data, q2_data)
theta_fit = np.interp(xx_fit, xx_data, theta_data)

fig, axs = plt.subplots(2, 2, figsize=(10, 8))

# sigma_L
axs[0,0].errorbar(xx_data, yL, yerr=yL_err, fmt='o', color='red', label="Data")
axs[0,0].plot(xx_fit, model_sigL((xx_fit, w_fit, q2_fit), *popt_L),label="Fit")
axs[0,0].set_title(r'$\sigma_{L}$')
axs[0,0].set_ylim([5.0, 16.0])
axs[0,0].set_xlim([0.01, 0.062])
axs[0,0].legend()

# sigma_T
axs[0,1].errorbar(xx_data, yT, yerr=yT_err, fmt='o', color='red',label="Data")
axs[0,1].plot(xx_fit, model_sigT((xx_fit, w_fit, q2_fit), *popt_T),label="Fit")
axs[0,1].set_title(r'$\sigma_{T}$')
axs[0,1].set_ylim([5.0, 16.0])
axs[0,1].set_xlim([0.01, 0.062])
axs[0,1].legend()

# sigma_LT
axs[1,0].errorbar(xx_data, yLT, yerr=yLT_err, fmt='o', color='red',label="Data")
axs[1,0].plot(xx_fit, wrapper_sigLT((xx_fit, theta_fit, w_fit), *popt_LT),label="Fit")
axs[1,0].set_title(r'$\sigma_{LT}$')
axs[1,0].set_ylim([-5.0, 4.0])
axs[1,0].set_xlim([0.01, 0.062])
axs[1,0].legend()

# sigma_TT
axs[1,1].errorbar(xx_data, yTT, yerr=yTT_err, fmt='o', color='red',label="Data")
axs[1,1].plot(xx_fit, wrapper_sigTT((xx_fit, theta_fit, w_fit), *popt_TT),label="Fit")
axs[1,1].set_title(r'$\sigma_{TT}$')
axs[1,1].set_ylim([-5.0, 4.0])
axs[1,1].set_xlim([0.01, 0.062])
axs[1,1].legend()
plt.tight_layout()
plt.savefig("Sep_Fit.pdf")
