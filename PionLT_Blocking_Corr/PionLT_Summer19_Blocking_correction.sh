#!/bin/bash

# ---------------------------
# Run Blocking Correction Script
# ---------------------------

# Exit on any error
set -e

# Python script and log file
PY_SCRIPT="PionLT_Summer19_Blocking_correction.py"
LOG_FILE="PionLT_Summer19_Blocking_correction.log"

echo "--------------------------------------------"
echo " Running PionLT Summer 2019 Blocking Correction Calculation"
echo "--------------------------------------------"
echo "Script : $PY_SCRIPT"
echo "Started: $(date)"
echo "--------------------------------------------"

# Check if Python script exists
if [ ! -f "$PY_SCRIPT" ]; then
  echo "Error: Python script '$PY_SCRIPT' not found!"
  exit 1
fi

# Run the Python script
python3 "$PY_SCRIPT" | tee "output/$LOG_FILE"

# Check exit status
if [ $? -eq 0 ]; then
  echo "--------------------------------------------"
  echo " Run completed successfully at $(date)"
  echo " Results saved in blocking_correction.csv"
  echo " Log file: $LOG_FILE"
  echo "--------------------------------------------"
else
  echo "--------------------------------------------"
  echo " Error: Blocking correction script failed!"
  echo " Check $LOG_FILE for details."
  echo "--------------------------------------------"
fi
