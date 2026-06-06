#!/bin/bash
echo ""
echo "Runnning physics analysis..."

echo ""
echo ""
echo ""
	
if [[ -f "/u/home/vijay/LTsepscripts/Analysis/Q2/yield/Analysed_Qp425W2p2.pdf" ]]; then  # check does file exist
    echo "Old file exist, they have now been deleted !!!"
    cd /u/home/vijay/LTsepscripts/Analysis/Q2/
    rm /u/home/vijay/LTsepscripts/Analysis/Q2/yield/*
    root -b -q  "Q2Analysis.C"
else 
    cd /u/home/vijay/LTsepscripts/Analysis/Q2/
    root -b -q  "Q2Analysis.C"
fi

echo "Processing is completed" 
exit 0 
