#!/bin/bash
echo ""
echo "Runnning physics analysis..."

echo ""
echo ""
echo ""
	
if [[ -f "/u/home/vijay/LTsepscripts/Analysis/Q1/yield/Analysed_Qp375W2p2.pdf" ]]; then  # check does file exist
    echo "Old file exist, they have now been deleted !!!"
    cd /u/home/vijay/LTsepscripts/Analysis/Q1/
    rm /u/home/vijay/LTsepscripts/Analysis/Q1/yield/*
    root -b -q  "Q1Analysis.C"
else 
    cd /u/home/vijay/LTsepscripts/Analysis/Q1/
    root -b -q  "Q1Analysis.C"
fi

echo "Processing is completed" 
exit 0 
