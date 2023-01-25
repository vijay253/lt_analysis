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
#LTANAPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f17`
LTANAPATH=`/u/group/c-kaonlt/USERS/vijay/lt_analysis/`

# Flag definitions (flags: h, a, o, s)
#while getopts 'hdat' flag; do
while getopts "hdat" flag; do
    case ${flag} in
        h) 
        echo "--------------------------------------------------------------"
        echo "./run_Prod_Analysis.sh -{flags} {variable arguments, see help}"
	echo
        echo "Description: Plots data vs simc"
        echo "--------------------------------------------------------------"
        echo
        echo "The following flags can be called for the heep analysis..."
        echo "    -h, help"
	echo "    -d, debug"	
        echo "    -a, analyze"	
        echo "    -t, set t-bin (!!!required for script!!!)"
	echo "        EPSILON=arg1, Q2=arg2, W=arg3"	
        exit 0
        ;;
	d) d_flag='true' ;;
	a) a_flag='true' ;;
        t) t_flag='true' ;;
        *) print_usage
        exit 1 ;;
    esac
done

# When any flag is used then the user input changes argument order
if [[ $t_flag = "true" || $a_flag = "true" ]]; then

    EPSILON=$(echo "$2" | tr '[:upper:]' '[:lower:]')
    Q2=$3
    W=$4
    TargetType=$(echo "$5" | tr '[:upper:]' '[:lower:]')
    echo "Epsilon must be - high - low - Case Sensitive!"
    echo "Q2 must be one of - [5p5 - 4p4 - 3p0 - 2p1 - 0p5 - 0p375 - 0p425]"
    echo "W must be one of - [3p02 - 2p74 - 3p14 - 2p32 - 2p95 - 2p40 - 2p2]"
    if [[ -z "$2" || ! "$EPSILON" =~ high|low|mid ]]; then # Check the 1st argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid epsilon..."
	while true; do
	    echo ""
	    read -p "Epsilon must be - high - low - Case Sensitive! - or press ctrl-c to exit : " EPSILON
	    case $EPSILON in
		'');; # If blank, prompt again
		'high'|'low'|'mid') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$3" || ! "$Q2" =~ 0p375|0p425 ]]; then # Check the 2nd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid Q2..."
	while true; do
	    echo ""
	    read -p "Q2 must be one of - [0p375 - 0p425] - or press ctrl-c to exit : " Q2
	    case $Q2 in
		'');; # If blank, prompt again
		'0p375'|'0p425') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$4" || ! "$W" =~ 2p2 ]]; then # Check the 3rd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid W..."
	while true; do
	    echo ""
	    read -p "W must be one of - [2p2] - or press ctrl-c to exit : " W
	    case $W in
		'');; # If blank, prompt again
		'2p2') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$5" || ! "$TargetType" =~ lh2|dummy|simc ]]; then # Check the 3rd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid target type..."
	while true; do
	    echo ""
	    read -p "Target type must be one of - [lh2 - dummy - simc] - or press ctrl-c to exit : " TargetType
	    case $TargetType in
		'');; # If blank, prompt again
		'lh2'|'dummy'|'simc') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi    
    
else
    
    EPSILON=$(echo "$1" | tr '[:upper:]' '[:lower:]' ':midle:')
    Q2=$2
    W=$3
    TargetType=$(echo "$4" | tr '[:upper:]' '[:lower:]')
 
   if [[ -z "$1" || ! "$EPSILON" =~ high|low|mid ]]; then # Check the 1st argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid epsilon..."
	while true; do
	    echo ""
	    read -p "Epsilon must be - high - low - mid- Case Sensitive! - or press ctrl-c to exit : " EPSILON
	    case $EPSILON in
		'');; # If blank, prompt again
		'high'|'low'|'mid') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$2" || ! "$Q2" =~ 0p375|0p425 ]]; then # Check the 2nd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid Q2..."
	while true; do
	    echo ""
	    read -p "Q2 must be one of - [0p375 - 0p425] - or press ctrl-c to exit : " Q2
	    case $Q2 in
		'');; # If blank, prompt again
		'0p375'|'0p425') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$3" || ! "$W" =~ 2p2 ]]; then # Check the 3rd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid W..."
	while true; do
	    echo ""
	    read -p "W must be one of - [2p2] - or press ctrl-c to exit : " W
	    case $W in
		'');; # If blank, prompt again
		'2p2') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
    if [[ -z "$4" || ! "$TargetType" =~ lh2|dummy|simc ]]; then # Check the 3rd argument was provided and that it's one of the valid options
	echo ""
	echo "I need a valid target type..."
	while true; do
	    echo ""
	    read -p "Target type must be one of - [lh2 - dummy - simc] - or press ctrl-c to exit : " TargetType
	    case $TargetType in
		'');; # If blank, prompt again
		'lh2'|'dummy'|'simc') break;; # If a valid option, break the loop and continue
	    esac
	done
    fi
fi

##############
# HARD CODED #
##############
NumtBins=5
NumPhiBins=16

# Define global variables for lt_analysis scripts
POL="+1" # All KaonLT is positive polarity
TMIN=0.010
TMAX=0.990
KSet=1 # Arbitrary value

# Efficiency csv file
#EffData="coin_production_Prod_efficiency_data_2022_12_05.csv"
#EffData="coin_production_Prod_efficiency_data_2022_12_30.csv"
EffData="coin_production_Prod_efficiency_data_2023_01_01.csv"

# Function that calls python script to grab run numbers
grab_runs () {
    RunList=$1
    INPDIR="${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/${RunList}"
#    cd "${LTANAPATH}/scripts"
    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts"
    RunNumArr=$(python3 getRunNumbers.py $INPDIR)
    echo $RunNumArr
}

echo
echo "---------------------------------------------------------"
echo
echo "Beginning analysis for Q2=${Q2}, W=${W}, ${EPSILON} setting..."
echo
echo "                       Number of t bins: ${NumtBins}"
echo "                       Range of t: ${TMIN} - ${TMAX}"
echo "                       Number of Phi bins: ${NumPhiBins}"
echo
echo "---------------------------------------------------------"
echo

data_right=()
data_left=()
data_center=()
data_right1=()
data_right2=()
data_left1=()
data_left2=()
# Get run numbers for left, right, and, center settings
declare -a PHI0P375HE=("RIGHT1" "CENTER" "LEFT1" "LEFT2")         # Q1 & high epsilon setting
for i in "${PHI0P375HE[@]}"
do
    if [[ $Q2 = "0p375" && $W = "2p2" && $EPSILON = "high" ]]; then
	if [[ $i = "RIGHT1" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_right1="Q0p375W2p2right_${EPSILON}e_dummy"
	   
	    elif [[ $TargetType = "simc" ]]; then
		file_right1="Q0p375W2p2right_${EPSILON}e_simc"		
	    else
		file_right1="Q0p375W2p2right_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for right file ${file_right1}..."
		data_right+=(0000) 
		echo "Run Numbers: [${data_right1[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for right file ${file_right1}..."
		IFS=', ' read -r -a data_right1 <<< "$( grab_runs ${file_right1} )"		
		echo "Run Numbers: [${data_right1[@]}]"
		echo
	    fi

	elif [[ $i = "CENTER" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_simc"		
	    else
		file_center="Q0p375W2p2center_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for center file ${file_center}..."
		data_center+=(0002) # CENTER, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_center[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for center file ${file_center}..."
		IFS=', ' read -r -a data_center <<< "$( grab_runs ${file_center} )"		
		echo "Run Numbers: [${data_center[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT1" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_simc"		
	    else
		file_left1="Q0p375W2p2left1_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left1 file ${file_left1}..."
		data_left1+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left1[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left1 file ${file_left1}..."
		IFS=', ' read -r -a data_left1 <<< "$( grab_runs ${file_left1} )"		
		echo "Run Numbers: [${data_left1[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT2" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_simc"		
	    else
		file_left2="Q0p375W2p2left2_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left2 file ${file_left2}..."
		data_left2+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left2[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left2 file ${file_left2}..."
		IFS=', ' read -r -a data_left2 <<< "$( grab_runs ${file_left2} )"		 
		echo "Run Numbers: [${data_left2[@]}]"
		echo
	    fi

	fi

	if [[ ${EPSILON} == "low" ]]; then
	    EPSVAL=0.4515
	elif [[ ${EPSILON} == "mid" ]]; then
	    EPSVAL=0.523              #just test
	else
	    EPSVAL=0.6979
	fi
	if [[ $TargetType = "dummy" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_dummy"
	elif [[ $TargetType = "simc" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_simc"	    
	else
	    KIN="Q0p375W2p2_${EPSILON}e"
	fi
    fi    

done

declare -a PHI0P375ME=("RIGHT2" "RIGHT1" "CENTER" "LEFT1" "LEFT2")         # Q1 & mid epsilon setting
for i in "${PHI0P375ME[@]}"
do
    if [[ $Q2 = "0p375" && $W = "2p2"  && $EPSILON = "mid" ]]; then
	if [[ $i = "RIGHT2" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_right2="Q0p375W2p2right2_${EPSILON}e_dummy"
	   
	    elif [[ $TargetType = "simc" ]]; then
		file_right2="Q0p375W2p2right2_${EPSILON}e_simc"		
	    else
		file_right2="Q0p375W2p2right2_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for right2 file ${file_right2}..."
		data_right2+=(0000) # RIGHT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_right2[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for right2 file ${file_right2}..."
		IFS=', ' read -r -a data_right2 <<< "$( grab_runs ${file_right2} )"		 
		echo "Run Numbers: [${data_right2[@]}]"
		echo
	    fi

	elif [[ $i = "RIGHT1" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_right1="Q0p375W2p2right1_${EPSILON}e_dummy"
	   
	    elif [[ $TargetType = "simc" ]]; then
		file_right1="Q0p375W2p2right1_${EPSILON}e_simc"		
	    else
		file_right1="Q0p375W2p2right1_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for right1 file ${file_right1}..."
		data_right1+=(0000) # RIGHT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_right1[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for right1 file ${file_right1}..."
		IFS=', ' read -r -a data_right1 <<< "$( grab_runs ${file_right1} )"		 
		echo "Run Numbers: [${data_right1[@]}]"
		echo
	    fi

	elif [[ $i = "CENTER" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_simc"		
	    else
		file_center="Q0p375W2p2center_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for center file ${file_center}..."
		data_center+=(0002) # CENTER, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_center[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for center file ${file_center}..."
		IFS=', ' read -r -a data_center <<< "$( grab_runs ${file_center} )"		
		echo "Run Numbers: [${data_center[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT1" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_simc"		
	    else
		file_left1="Q0p375W2p2left1_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left1 file ${file_left1}..."
		data_left1+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left1[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left1 file ${file_left1}..."
		IFS=', ' read -r -a data_left1 <<< "$( grab_runs ${file_left1} )"	
		echo "Run Numbers: [${data_left1[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT2" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_simc"		
	    else
		file_left2="Q0p375W2p2left2_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left2 file ${file_left2}..."
		data_left2+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left2[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left2 file ${file_left2}..."
		IFS=', ' read -r -a data_left2 <<< "$( grab_runs ${file_left2} )"	 
		echo "Run Numbers: [${data_left2[@]}]"
		echo
	    fi

	fi

	if [[ ${EPSILON} == "low" ]]; then
	    EPSVAL=0.4515
	elif [[ ${EPSILON} == "mid" ]]; then
	    EPSVAL=0.523              #just test
	else
	    EPSVAL=0.6979
	fi
	if [[ $TargetType = "dummy" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_dummy"
	elif [[ $TargetType = "simc" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_simc"	    
	else
	    KIN="Q0p375W2p2_${EPSILON}e"
	fi
    fi    

done

declare -a PHI0P375ME=("CENTER" "LEFT1" "LEFT2")         # Q1 & low epsilon setting
for i in "${PHI0P375ME[@]}"
do
    if [[ $Q2 = "0p375" && $W = "2p2"  && $EPSILON = "low" ]]; then

	if [[ $i = "CENTER" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_center="Q0p375W2p2center_${EPSILON}e_simc"		
	    else
		file_center="Q0p375W2p2center_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for center file ${file_center}..."
		data_center+=(0002) # CENTER, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_center[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for center file ${file_center}..."
		IFS=', ' read -r -a data_center <<< "$( grab_runs ${file_center} )"    
		echo "Run Numbers: [${data_center[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT1" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left1="Q0p375W2p2left1_${EPSILON}e_simc"		
	    else
		file_left1="Q0p375W2p2left1_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left1 file ${file_left1}..."
		data_left1+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left1[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left1 file ${file_left1}..."
		IFS=', ' read -r -a data_left1 <<< "$( grab_runs ${file_left1} )"	  
		echo "Run Numbers: [${data_left1[@]}]"
		echo
	    fi

	elif [[ $i = "LEFT2" ]]; then
	    if [[ $TargetType = "dummy" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_dummy"
	    elif [[ $TargetType = "simc" ]]; then
		file_left2="Q0p375W2p2left2_${EPSILON}e_simc"		
	    else
		file_left2="Q0p375W2p2left2_${EPSILON}e"
	    fi
	    if [[ $TargetType = "simc" ]]; then
		echo "Reading in run numbers for left2 file ${file_left2}..."
		data_left2+=(0001) # LEFT, Q2=0p5, W=2p40
		echo "Run Numbers: [${data_left2[@]}]"
		echo	    	    	    	    
	    else
		echo "Reading in run numbers for left2 file ${file_left2}..."
		IFS=', ' read -r -a data_left2 <<< "$( grab_runs ${file_left2} )"	 
		echo "Run Numbers: [${data_left2[@]}]"
		echo
	    fi

	fi

	if [[ ${EPSILON} == "low" ]]; then
	    EPSVAL=0.4515
	elif [[ ${EPSILON} == "mid" ]]; then
	    EPSVAL=0.523              #just test
	else
	    EPSVAL=0.6979
	fi
	if [[ $TargetType = "dummy" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_dummy"
	elif [[ $TargetType = "simc" ]]; then
	    KIN="Q0p375W2p2_${EPSILON}e_simc"	    
	else
	    KIN="Q0p375W2p2_${EPSILON}e"
	fi
    fi    

done

# Define input and output file names
InDATAFilename="Proc_Data_${KIN}.root"
OutDATAFilename="Analysed_Data_${KIN}"
OutFullAnalysisFilename="FullAnalysis_${KIN}"

# When analysis flag is used then the analysis script (Analysed_Prod.py)
# will create a new root file per run number which are combined using hadd
if [[ $a_flag = "true" && $TargetType != "simc" ]]; then
    # Checks that array isn't empty
    if [ ${#data_right2[@]} -ne 0 ]; then
	echo
	echo
	for i in "${data_right2[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing right2 data run $i..."
	    echo "-----------------------------"
	    echo
#	    cd "${LTANAPATH}/scripts/Prod"
	    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	    python3 Analysed_Prod_v1.py "$i" | tee ../../log/Analysed_Prod_v1_$i.log
	    root -b -q  "PlotScript.C(\"$i\")"
#	    cd "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT"
	    cd "/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/OUTPUT/Analysis/KaonLT"
	    echo "Combining run $i with ${OutDATAFilename}_Right2.root..."  
	    hadd -f ${OutDATAFilename}_Right2.root ${i}_-1_Raw_Data.root
	    echo "Renaming Raw_Data to Proc_Data..."
	    mv ${i}_-1_Raw_Data.root ${i}_-1_Proc_Data.root
	done
	echo
    fi

    if [ ${#data_right1[@]} -ne 0 ]; then
	echo
	echo
	for i in "${data_right1[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing right1 data run $i..."
	    echo "-----------------------------"
	    echo
#	    cd "${LTANAPATH}/scripts/Prod"
	    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	    python3 Analysed_Prod_v1.py "$i" | tee ../../log/Analysed_Prod_v1_$i.log
	    root -b -q  "PlotScript.C(\"$i\")"
	    echo
#	    cd "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT"
	    cd "/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/OUTPUT/Analysis/KaonLT"
	    echo "Combining run $i with ${OutDATAFilename}_Right1.root..."  
	    hadd -f ${OutDATAFilename}_Right1.root ${i}_-1_Raw_Data.root
	    echo "Renaming Raw_Data to Proc_Data..."
	    mv ${i}_-1_Raw_Data.root ${i}_-1_Proc_Data.root

	done
	echo
    fi


    if [ ${#data_center[@]} -ne 0 ]; then
	echo
	echo "Analysing center data..."
	echo
	for i in "${data_center[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing center data run $i..."
	    echo "-----------------------------"
	    echo
#	    cd "${LTANAPATH}/scripts/Prod"
	    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	    python3 Analysed_Prod_v1.py "$i" | tee ../../log/Analysed_Prod_v1_$i.log
	    root -b -q  "PlotScript.C(\"$i\")"
	    echo
#	    cd "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT"
	    cd "/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/OUTPUT/Analysis/KaonLT"
	    echo "Combining run $i with ${OutDATAFilename}_Center.root..."  
	    hadd -f ${OutDATAFilename}_Center.root ${i}_-1_Raw_Data.root
	    echo "Renaming Raw_Data to Proc_Data..."
	    mv ${i}_-1_Raw_Data.root ${i}_-1_Proc_Data.root
	done
    fi

    # Checks that array isn't empty
    if [ ${#data_left1[@]} -ne 0 ]; then
	echo
	echo "Analysing left1 data..."
	echo
	for i in "${data_left1[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing left1 data run $i..."
	    echo "-----------------------------"
	    echo
#	    cd "${LTANAPATH}/scripts/Prod"
	    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	    python3 Analysed_Prod_v1.py "$i" | tee ../../log/Analysed_Prod_v1_$i.log
	    root -b -q  "PlotScript.C(\"$i\")"
	    echo
#	    cd "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT"
	    cd "/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/OUTPUT/Analysis/KaonLT"
	    echo "Combining run $i with ${OutDATAFilename}_Left1.root..."  
	    hadd -f ${OutDATAFilename}_Left1.root ${i}_-1_Raw_Data.root
	    echo "Renaming Raw_Data to Proc_Data..."
	    mv ${i}_-1_Raw_Data.root ${i}_-1_Proc_Data.root
	done
    fi

    # Checks that array isn't empty
    if [ ${#data_left2[@]} -ne 0 ]; then
	echo
	echo "Analysing left2 data..."
	echo
	for i in "${data_left2[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing left2 data run $i..."
	    echo "-----------------------------"
	    echo
#	    cd "${LTANAPATH}/scripts/Prod"
	    cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	    python3 Analysed_Prod_v1.py "$i" | tee ../../log/Analysed_Prod_v1_$i.log
	    root -b -q  "PlotScript.C(\"$i\")"
	    echo
#	    cd "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT"
	    cd "/u/group/c-kaonlt/USERS/vijay/hallc_replay_lt/UTIL_KAONLT/OUTPUT/Analysis/KaonLT"
	    echo "Combining run $i with ${OutDATAFilename}_Left2.root..."  
	    hadd -f ${OutDATAFilename}_Left2.root ${i}_-1_Raw_Data.root
	    echo "Renaming Raw_Data to Proc_Data..."
	    mv ${i}_-1_Raw_Data.root ${i}_-1_Proc_Data.root
	done
    fi
    
fi

#cd "${LTANAPATH}/scripts"
cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts"

# Checks that array isn't empty
if [[ ${#data_right2[@]} -ne 0 && $TargetType != "simc" ]]; then
    DataChargeValRight2=()
    DataChargeErrRight2=()
    DataEffValRight2=()
    DataEffErrRight2=()
    DatapThetaValRight2=()
    DataEbeamValRight2=()
    echo
    echo "Calculating data total effective charge right2..."
    for i in "${data_right2[@]}"
    do
	# Calculates total efficiency then applies to the charge for each run number
	# to get the effective charge per run and saves as an array
	DataChargeValRight2+=($(python3 findEffectiveCharge.py ${EffData} "replay_coin_production" "$i" -1))
	DataChargeErrRight2+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs the total effiency value per run and saves as an array
	DataEffValRight2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
	DataEffErrRight2+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs pTheta value per run
	DatapThetaValRight2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "pTheta"))
	# Grabs ebeam value per run
	DataEbeamValRight2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "ebeam"))
    done
 
    DataChargeSumRight2=$(IFS=+; echo "$((${DataChargeValRight2[*]}))") # Only works for integers
    echo "Total Charge Right2: ${DataChargeSumRight2} uC"
fi

if [[ ${#data_right1[@]} -ne 0 && $TargetType != "simc" ]]; then
    DataChargeValRight1=()
    DataChargeErrRight1=()
    DataEffValRight1=()
    DataEffErrRight1=()
    DatapThetaValRight1=()
    DataEbeamValRight1=()
    echo
    echo "Calculating data total effective charge right1..."


    for i in "${data_right1[@]}"
    do
	# Calculates total efficiency then applies to the charge for each run number
	# to get the effective charge per run and saves as an array
	DataChargeValRight1+=($(python3 findEffectiveCharge.py ${EffData} "replay_coin_production" "$i" -1))
	DataChargeErrRight1+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs the total effiency value per run and saves as an array
	DataEffValRight1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
	DataEffErrRight1+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs pTheta value per run
	DatapThetaValRight1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "pTheta"))
	# Grabs ebeam value per run
	DataEbeamValRight1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "ebeam"))
    done
 
   #       and later converted to C
    DataChargeSumRight1=$(IFS=+; echo "$((${DataChargeValRight1[*]}))") # Only works for integers
    echo "Total Charge Right1: ${DataChargeSumRight1} uC"
fi

# Checks that array isn't empty
if [[ ${#data_center[@]} -ne 0 && $TargetType != "simc" ]]; then
    DataChargeValCenter=()
    DataChargeErrCenter=()
    DataEffValCenter=()
    DataEffErrCenter=()
    DatapThetaValCenter=()
    DataEbeamValCenter=()
    echo
    echo "Calculating data total effective charge center..."
    for i in "${data_center[@]}"
    do
	# Calculates total efficiency then applies to the charge for each run number
	# to get the effective charge per run and saves as an array
	DataChargeValCenter+=($(python3 findEffectiveCharge.py ${EffData} "replay_coin_production" "$i" -1))
	DataChargeErrCenter+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs the total effiency value per run and saves as an array
	DataEffValCenter+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
	DataEffErrCenter+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs pTheta value per run
	DatapThetaValCenter+=($(python3 getEfficiencyValue.py "$i" ${EffData} "pTheta"))
	# Grabs ebeam value per run
	DataEbeamValCenter+=($(python3 getEfficiencyValue.py "$i" ${EffData} "ebeam"))
    done
    DataChargeSumCenter=$(IFS=+; echo "$((${DataChargeValCenter[*]}))") # Only works for integers
    echo "Total Charge Center: ${DataChargeSumCenter} uC"
fi

# Checks that array isn't empty
if [[ ${#data_left1[@]} -ne 0 && $TargetType != "simc" ]]; then
    DataChargeValLeft1=()
    DataChargeErrLeft1=()
    DataEffValLeft1=()
    DataEffErrLeft1=()
    DatapThetaValLeft1=()
    DataEbeamValLeft1=()
    echo
    echo "Calculating data total effective charge left1..."
    for i in "${data_left1[@]}"
    do
	# Calculates total efficiency then applies to the charge for each run number
	# to get the effective charge per run and saves as an array
	DataChargeValLeft1+=($(python3 findEffectiveCharge.py ${EffData} "replay_coin_production" "$i" -1))
	DataChargeErrLeft1+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs the total effiency value per run and saves as an array
	DataEffValLeft1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
	DataEffErrLeft1+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs pTheta value per run
	DatapThetaValLeft1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "pTheta"))
	# Grabs ebeam value per run
	DataEbeamValLeft1+=($(python3 getEfficiencyValue.py "$i" ${EffData} "ebeam"))
  done
    DataChargeSumLeft1=$(IFS=+; echo "$((${DataChargeValLeft1[*]}))") # Only works for integers
    echo "Total Charge Left1: ${DataChargeSumLeft1} uC"
fi
# Checks that array isn't empty
if [[ ${#data_left2[@]} -ne 0 && $TargetType != "simc" ]]; then
    DataChargeValLeft2=()
    DataChargeErrLeft2=()
    DataEffValLeft2=()
    DataEffErrLeft2=()
    DatapThetaValLeft2=()
    DataEbeamValLeft2=()
    echo
    echo "Calculating data total effective charge left2..."
    for i in "${data_left2[@]}"
    do
	# Calculates total efficiency then applies to the charge for each run number
	# to get the effective charge per run and saves as an array
	DataChargeValLeft2+=($(python3 findEffectiveCharge.py ${EffData} "replay_coin_production" "$i" -1))
	DataChargeErrLeft2+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs the total effiency value per run and saves as an array
	DataEffValLeft2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
	DataEffErrLeft2+=(1) # HERE! These uncertainties are set to unity until the replays are updated with proper uncertainties
	# Grabs pTheta value per run
	DatapThetaValLeft2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "pTheta"))
	# Grabs ebeam value per run
	DataEbeamValLeft2+=($(python3 getEfficiencyValue.py "$i" ${EffData} "ebeam"))
    done
    DataChargeSumLeft2=$(IFS=+; echo "$((${DataChargeValLeft2[*]}))") # Only works for integers
    echo "Total Charge Left2: ${DataChargeSumLeft2} uC"
fi

# Run the plotting script if t-flag enabled
# Checks that array isn't empty
if [[ $TargetType != "simc" ]]; then
    echo "test"
    if [[ $t_flag = "true" || $d_flag = "true" ]]; then
#	cd "${LTANAPATH}/scripts/Prod"  
	echo "test"
	cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod"
	if [ ${#data_right[@]} -eq 0 ]; then
	    python3 find_tBinRange.py ${KIN} ${W} ${Q2} ${EPSVAL} ${OutDATAFilename} ${OutFullAnalysisFilename} ${TMIN} ${TMAX} ${NumtBins} ${NumPhiBins} "0" "${data_left[*]}" "${data_center[*]}" "0" ${DataChargeSumLeft} ${DataChargeSumCenter} "0" "${DataEffValLeft[*]}" "${DataEffValCenter[*]}" ${EffData} ${TargetType}
	else
	    python3 find_tBinRange.py ${KIN} ${W} ${Q2} ${EPSVAL} ${OutDATAFilename} ${OutFullAnalysisFilename} ${TMIN} ${TMAX} ${NumtBins} ${NumPhiBins} "${data_right[*]}" "${data_left[*]}" "${data_center[*]}" ${DataChargeSumRight} ${DataChargeSumLeft} ${DataChargeSumCenter} "${DataEffValRight[*]}" "${DataEffValLeft[*]}" "${DataEffValCenter[*]}" ${EffData} ${TargetType}
	fi
    fi
fi

# Create input for lt_analysis code
#cd "${LTANAPATH}/scripts/Prod/"
cd "/u/group/c-kaonlt/USERS/vijay/lt_analysis/scripts/Prod/"
if [[ $TargetType != "simc" ]]; then
    if [ ${#data_right[@]} -eq 0 ]; then
	python3 createPhysicsList.py ${Q2} ${POL} ${EPSVAL} ${TMIN} ${TMAX} ${NumtBins} ${KSet} "0" "${data_left[*]}" "${data_center[*]}" "0" "${DatapThetaValLeft[*]}" "${DatapThetaValCenter[*]}" "0" "${DataEbeamValLeft[*]}" "${DataEbeamValCenter[*]}" "0" "${DataEffValLeft[*]}" "${DataEffValCenter[*]}" "0" "${DataEffErrLeft[*]}" "${DataEffErrCenter[*]}" "0" "${DataChargeValLeft[*]}" "${DataChargeValCenter[*]}" "0" "${DataChargeErrLeft[*]}" "${DataChargeErrCenter[*]}" ${TargetType}
    else
	python3 createPhysicsList.py ${Q2} ${POL} ${EPSVAL} ${TMIN} ${TMAX} ${NumtBins} ${KSet} "${data_right[*]}" "${data_left[*]}" "${data_center[*]}" "${DatapThetaValRight[*]}" "${DatapThetaValLeft[*]}" "${DatapThetaValCenter[*]}" "${DataEbeamValRight[*]}" "${DataEbeamValLeft[*]}" "${DataEbeamValCenter[*]}" "${DataEffValRight[*]}" "${DataEffValLeft[*]}" "${DataEffValCenter[*]}" "${DataEffErrRight[*]}" "${DataEffErrLeft[*]}" "${DataEffErrCenter[*]}" "${DataChargeValRight[*]}" "${DataChargeValLeft[*]}" "${DataChargeValCenter[*]}" "${DataChargeErrRight[*]}" "${DataChargeErrLeft[*]}" "${DataChargeErrCenter[*]}" ${TargetType}
    fi
else
    python3 createSimcList.py ${Q2} ${POL} ${EPSVAL} "${data_right[*]}" "${data_left[*]}" "${data_center[*]}" ${KIN}
fi

if [[ $t_flag = "true" || $d_flag = "true" ]]; then
  #  cd "${LTANAPATH}"
    cd "/lustre19/expphy/volatile/hallc/c-kaonlt/vijay/ROOTfiles/KaonLT_Analysis/PhysicsAnalysis/PionLT/Analysis/KaonLT"
    evince "${OutFullAnalysisFilename}.pdf"
fi

