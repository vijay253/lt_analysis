import os
import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# -------------------------------------------------
# Constants
# -------------------------------------------------
Q2 = 0.375
mp = 0.938272

# -------------------------------------------------
# Parameter files
# -------------------------------------------------
base_dir = os.path.dirname(os.path.abspath(__file__))

par_file_in  = os.path.join(base_dir, "/group/c-kaonlt/USERS/vijay/simc_gfortran/par.pl")
par_file_out = os.path.join(base_dir, "next_it_par/par.pl")

# -------------------------------------------------
# Read one column
# -------------------------------------------------
def read_column(filename, col):
    return np.loadtxt(filename, usecols=[col])

# -------------------------------------------------
# Read parameter file
# 14 rows total:
# rows 0-3   sigma_T   (4 params)
# rows 4-7   sigma_L   (4 params)
# rows 8-10  sigma_LT  (3 params)
# rows 11-13 sigma_TT  (3 params)
#
# col0 = parameter
# col1 = error
# col2 = integer flag/value (kept unchanged)
# -------------------------------------------------
def read_par_file(fname):
    data = np.loadtxt(fname)

    pT  = data[0:4,0]
    pL  = data[4:8,0]
    pLT = data[8:11,0]
    pTT = data[11:14,0]

    return data, pT, pL, pLT, pTT

# -------------------------------------------------
# Save updated parameter file
# -------------------------------------------------
def save_par_file(fname, old_data,
                  popt_T, perr_T,
                  popt_L, perr_L,
                  popt_LT, perr_LT,
                  popt_TT, perr_TT):

    out = old_data.copy()

    # sigma_T
    out[0:4,0] = popt_T
    out[0:4,1] = perr_T

    # sigma_L
    out[4:8,0] = popt_L
    out[4:8,1] = perr_L

    # sigma_LT
    out[8:11,0] = popt_LT
    out[8:11,1] = perr_LT

    # sigma_TT
    out[11:14,0] = popt_TT
    out[11:14,1] = perr_TT

    np.savetxt(fname, out, fmt="%18.10e %18.10e %6d")

# -------------------------------------------------
# Helpers
# -------------------------------------------------
def compute_ftav(xx):
    tav = (0.0735 + 0.028*np.log(Q2)) * Q2
    return (xx - tav) / tav

def compute_Wfactor(w):
    return 1.0 / ((w**2 - mp**2)**2)

# -------------------------------------------------
# Models
# -------------------------------------------------
def model_sigT(x, a0, a1, a2, a3):
    xx, w, q2 = x
    ftav = compute_ftav(xx)
    wf = compute_Wfactor(w)

#    return (a0 + a1*np.exp(a2*(-ftav)) + a3*np.exp(-(ftav**2))) \
#           * (1/q2**2) * wf
#    return (a0 + a1*np.exp(a2*(-ftav)) + a3*np.exp(-(ftav**2))) \
#           * wf
#    return (a0 + a1*np.exp(a2*(ftav)) + a3*np.exp((1-ftav**2))) \
#           * wf
#    return (a0 + a1 * ftav + (a2 + a3 * ftav) * ftav) \
#           * wf
#    return (a0 + a1 * ftav) * wf
    return ((a0 + a1 * ftav**1 + a2 * ftav**2) * wf) * (1-0.0)

def model_sigL(x, a0, a1, a2, a3):
    xx, w, q2 = x
    wf = compute_Wfactor(w)

#    return (a0*np.exp(a1*xx)*(1-a2*xx*np.exp(a3*xx))**4) \
#           * (1/q2**2) * wf
#    return (a0*np.exp(a1*xx)*(1-a2*xx*np.exp(a3*xx))**4) \
#           * wf
#    return (a0*np.exp(a1*xx)*(1-a2*xx*np.exp(a3*xx))**1) \
#           * wf
#    return ((a0 + a1*xx) * np.exp((a2 + a3*xx) *(xx -0.2))) \
#           * wf
    return ((a0 * np.exp(a1 * xx)) * (xx/((-xx - 0.13957**2)**2 * (0.44**2 - xx)**2)) * wf) * (1-0.0)
#  * (xx/((-xx - 0.13957**2)**2 * (0.44**2 - xx)))
#+ a2 * np.exp(a3 * xx)
#    return (a0 * np.exp(a1 * xx)) * wf
#    return ((a0 + a1 * np.log(q2**2)) * np.exp((a2 + a3 * np.log(q2**2))*(xx -0.2))) * wf
#    return ((a0 + a1 * q2) * np.exp((a2 + a3 * q2)*(xx -0.2))) * wf

def wrapper_sigTT(x, a0, a1, a2):
    xx, theta, w = x
    wf = compute_Wfactor(w)

    return ((a0/(xx**2)*np.exp(a1*xx) + a2/xx**4) *
            (np.sin(np.radians(theta))**2)) * wf

def wrapper_sigLT(x, a0, a1, a2):
    xx, theta, w = x
    wf = compute_Wfactor(w)

    return ((a0/(xx**2)*np.exp(a1/xx) + a2/xx) *
            np.sin(np.radians(theta))) * wf

# -------------------------------------------------
# Read parameters from par.pl
# -------------------------------------------------
par_data, p0_T, p0_L, p0_LT, p0_TT = read_par_file(par_file_in)

# -------------------------------------------------
# Experimental data
# -------------------------------------------------
avefile = "/group/c-kaonlt/USERS/vijay/Analysis/Q1/src/averages/avek.375.dat"
xfile   = "/group/c-kaonlt/USERS/vijay/Analysis/Q1/src/xsects/x_sep.pl_375"

xx_data    = read_column(avefile, 4)
q2_data    = read_column(avefile, 2)
theta_data = read_column(avefile, 6)
w_data     = read_column(avefile, 0)

yT,  yT_err  = read_column(xfile,0), read_column(xfile,1)
yL,  yL_err  = read_column(xfile,2), read_column(xfile,3)
yLT, yLT_err = read_column(xfile,4), read_column(xfile,5)
yTT, yTT_err = read_column(xfile,6), read_column(xfile,7)

# -------------------------------------------------
# Fitting
# -------------------------------------------------
popt_T, pcov_T = curve_fit(
    model_sigT,
    (xx_data, w_data, q2_data),
    yT,
    p0=p0_T,
    sigma=yT_err,
    absolute_sigma=True
)

popt_L, pcov_L = curve_fit(
    model_sigL,
    (xx_data, w_data, q2_data),
    yL,
    p0=p0_L,
    sigma=yL_err,
    absolute_sigma=True
)
#maxfev=4000
popt_LT, pcov_LT = curve_fit(
    wrapper_sigLT,
    (xx_data, theta_data, w_data),
    yLT,
    p0=p0_LT,
    sigma=yLT_err,
    absolute_sigma=True
)

popt_TT, pcov_TT = curve_fit(
    wrapper_sigTT,
    (xx_data, theta_data, w_data),
    yTT,
    p0=p0_TT,
    sigma=yTT_err,
    absolute_sigma=True
)

perr_T  = np.sqrt(np.diag(pcov_T))
perr_L  = np.sqrt(np.diag(pcov_L))
perr_LT = np.sqrt(np.diag(pcov_LT))
perr_TT = np.sqrt(np.diag(pcov_TT))

# -------------------------------------------------
# Print
# -------------------------------------------------
print("\nFit results:")
print("sigma_T :",  popt_T,  "+/-", perr_T)
print("sigma_L :",  popt_L,  "+/-", perr_L)
print("sigma_LT:", popt_LT, "+/-", perr_LT)
print("sigma_TT:", popt_TT, "+/-", perr_TT)

# -------------------------------------------------
# Save new parameters
# -------------------------------------------------
save_par_file(
    par_file_out,
    par_data,
    popt_T,  perr_T,
    popt_L,  perr_L,
    popt_LT, perr_LT,
    popt_TT, perr_TT
)

print("\nUpdated parameters saved to:", par_file_out)

# -------------------------------------------------
# Plot
# -------------------------------------------------
xx_fit    = np.linspace(min(xx_data), max(xx_data), 200)
w_fit     = np.interp(xx_fit, xx_data, w_data)
q2_fit    = np.interp(xx_fit, xx_data, q2_data)
theta_fit = np.interp(xx_fit, xx_data, theta_data)

fig, axs = plt.subplots(2, 2, figsize=(10,8))

# sigma_L
axs[0,0].errorbar(xx_data, yL, yerr=yL_err, fmt='o', color='red',label="Data")
axs[0,0].plot(xx_fit, model_sigL((xx_fit,w_fit,q2_fit), *popt_L),label="Fit")
axs[0,0].set_title(r'$\sigma_{L}$')
axs[0,0].set_ylim([6,22])
axs[0,0].legend()

# sigma_T
axs[0,1].errorbar(xx_data, yT, yerr=yT_err, fmt='o', color='red',label="Data")
axs[0,1].plot(xx_fit, model_sigT((xx_fit,w_fit,q2_fit), *popt_T),label="Fit")
axs[0,1].set_title(r'$\sigma_{T}$')
axs[0,1].set_ylim([6,22])
axs[0,1].legend()

# sigma_LT
axs[1,0].errorbar(xx_data, yLT, yerr=yLT_err, fmt='o', color='red',label="Data")
axs[1,0].plot(xx_fit, wrapper_sigLT((xx_fit,theta_fit,w_fit), *popt_LT),label="Fit")
axs[1,0].set_title(r'$\sigma_{LT}$')
axs[1,0].set_ylim([-5,4])
axs[1,0].legend()

# sigma_TT
axs[1,1].errorbar(xx_data, yTT, yerr=yTT_err, fmt='o', color='red',label="Data")
axs[1,1].plot(xx_fit, wrapper_sigTT((xx_fit,theta_fit,w_fit), *popt_TT),label="Fit")
axs[1,1].set_title(r'$\sigma_{TT}$')
axs[1,1].set_ylim([-5,4])
axs[1,1].legend()

plt.tight_layout()
plt.savefig("Sep_Fit.pdf")
