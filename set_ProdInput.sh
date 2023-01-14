#! /bin/bash

# Runs script in the ltsep python package that grabs current path enviroment
if [[ ${HOSTNAME} = *"cdaq"* ]]; then
    PATHFILE_INFO=`python3 /home/cdaq/pionLT-2021/hallc_replay_lt/UTIL_PION/bin/python/ltsep/scripts/getPathDict.py $PWD` # The output of this python script is just a comma separated string
elif [[ ${HOSTNAME} = *"farm"* ]]; then
    PATHFILE_INFO=`python3 /u/home/${USER}/.local/lib/python3.4/site-packages/ltsep/scripts/getPathDict.py $PWD` # The output of this python script is just a comma separated string
fi

# Split the string we get to individual variables, easier for printing and use later
VOLATILEPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f1` # Cut the string on , delimitter, select field (f) 1, set variable to output of command
ANALYSISPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f2`
HCANAPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f3`
REPLAYPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f4`
UTILPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f5`
PACKAGEPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f6`
OUTPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f7`
ROOTPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f8`
REPORTPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f9`
CUTPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f10`
PARAMPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f11`
SCRIPTPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f12`
ANATYPE=`echo ${PATHFILE_INFO} | cut -d ','  -f13`
USER=`echo ${PATHFILE_INFO} | cut -d ','  -f14`
HOST=`echo ${PATHFILE_INFO} | cut -d ','  -f15`
SIMCPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f16`

# Flag definitions (flags: h, c, a, s)
while getopts 'h' flag; do
    case "${flag}" in
        h) 
        echo "----------------------------------------------------------"
        echo "./set_ProdInput.sh -{flags} {variable arguments, see help}"
	echo
        echo "Description: Runs simc and updates simc input files"
        echo "----------------------------------------------------------"
        echo
        echo "The following flags can be called for the heep analysis..."
	echo "    KIN=arg1"
        echo "    -h, help"
        exit 0
        ;;
        *) print_usage
        exit 1 ;;
    esac
done

EPSILON=$1
Q2=$2
W=$3
echo "Epsilon must be - high - low - Case Sensitive!"
echo "Q2 must be one of - [5p5 - 4p4 - 3p0 - 2p1 - 0p5]"
echo "W must be one of - [3p02 - 2p74 - 3p14 - 2p32 - 2p95 - 2p40]"
if [[ -z "$1" || ! "$EPSILON" =~ high|low ]]; then # Check the 1st argument was provided and that it's one of the valid options
    echo ""
    echo "I need a valid epsilon..."
    while true; do
	echo ""
	read -p "Epsilon must be - high - low - Case Sensitive! - or press ctrl-c to exit : " EPSILON
	case $EPSILON in
	    '');; # If blank, prompt again
	    'high'|'low') break;; # If a valid option, break the loop and continue
	esac
    done
fi
if [[ -z "$2" || ! "$Q2" =~ 5p5|4p4|3p0|2p1|0p5 ]]; then # Check the 2nd argument was provided and that it's one of the valid options
    echo ""
    echo "I need a valid Q2..."
    while true; do
	echo ""
	read -p "Q2 must be one of - [5p5 - 4p4 - 3p0 - 2p1 - 0p5] - or press ctrl-c to exit : " Q2
	case $Q2 in
	    '');; # If blank, prompt again
	    '5p5'|'4p4'|'3p0'|'2p1'|'0p5') break;; # If a valid option, break the loop and continue
	esac
    done
fi
if [[ -z "$3" || ! "$W" =~ 3p02|2p74|3p14|2p32|2p95|2p40 ]]; then # Check the 3rd argument was provided and that it's one of the valid options
    echo ""
    echo "I need a valid W..."
    while true; do
	echo ""
	read -p "W must be one of - [3p02 - 2p74 - 3p14 - 2p32 - 2p95 - 2p40] - or press ctrl-c to exit : " W
	case $W in
	    '');; # If blank, prompt again
	    '3p02'|'2p74'|'3p14'|'2p32'|'2p95'|'2p40') break;; # If a valid option, break the loop and continue
	esac
    done
fi

InputSIMC_right="Prod_Coin_Q${Q2}W${W}right_${EPSILON}e"
InputSIMC_left="Prod_Coin_Q${Q2}W${W}left_${EPSILON}e"
InputSIMC_center="Prod_Coin_Q${Q2}W${W}center_${EPSILON}e"

cd ${SIMCPATH}/scripts
if [[ -f "$InputSIMC_right" ]]; then
    echo
    echo 
    echo "Running simc analysis for ${InputSIMC_right}..."
    echo
    ./run_simc_tree "${InputSIMC_right}"
fi

if [[ -f "$InputSIMC_left" ]]; then
    echo
    echo 
    echo "Running simc analysis for ${InputSIMC_left}..."
    echo
    ./run_simc_tree "${InputSIMC_left}"
fi

if [[ -f "$InputSIMC_center" ]]; then
    echo
    echo 
    echo "Running simc analysis for ${InputSIMC_center}..."
    echo
    ./run_simc_tree "${InputSIMC_center}"
fi
