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

# Flag definitions (flags: h, a, o, s)
while getopts 'haos' flag; do
    case "${flag}" in
        h) 
        echo "--------------------------------------------------------------"
        echo "./run_HeeP_Analysis.sh -{flags} {variable arguments, see help}"
	echo
        echo "Description: Plots data vs simc"
        echo "--------------------------------------------------------------"
        echo
        echo "The following flags can be called for the heep analysis..."
        echo "    -h, help"
        echo "    -a, analyze"
	echo "        coin -> KIN=arg1"
	echo "        sing -> SPEC=arg1 KIN=arg2 (requires -s flag)"
	echo "    -s, single arm"
	echo "    -o, offset to replay applied "
	echo "        (this is a label, offsets must be applied explicitly in replay before this step)"
        exit 0
        ;;
        a) a_flag='true' ;;
        o) o_flag='true' ;;
	s) s_flag='true' ;;
        *) print_usage
        exit 1 ;;
    esac
done

# When any flag is used then the user input changes argument order
if [[ $a_flag = "true" || $o_flag = "true" || $s_flag = "true" ]]; then
    if [[ $s_flag = "true" ]]; then
	spec=$(echo "$2" | tr '[:upper:]' '[:lower:]')
	SPEC=$(echo "$spec" | tr '[:lower:]' '[:upper:]')
	KIN=$3
	ROOTPREFIX=replay_${spec}_heep
    else
	KIN=$2
	ROOTPREFIX=replay_coin_heep
    fi
else
    KIN=$1
    ROOTPREFIX=replay_coin_heep
fi

# When just single flag used...
if [[ $s_flag = "true" ]]; then
    ##############
    # HARD CODED #
    ##############
    # Defines efficiency table to use
    if [[ $SPEC = "HMS" ]]; then
	EffData="hms_heep_HeePSing_efficiency_data_2022_07_28.csv"
    else
	EffData="shms_heep_HeePSing_efficiency_data_2022_07_28.csv"
    fi
    InDATAFilename="Raw_Data_${SPEC}_${KIN}.root"
    InDUMMYFilename="Raw_DummyData_${SPEC}_${KIN}.root"
    InSIMCFilename="Heep_${SPEC}_${KIN}.root"
    OutDATAFilename="Analysed_Data_${SPEC}_${KIN}"
    OutDUMMYFilename="Analysed_DummyData_${SPEC}_${KIN}"
    if [[ $o_flag = "true" ]]; then
	OutFullAnalysisFilename="FullAnalysis_Offset_${SPEC}_${KIN}"
    else
	OutFullAnalysisFilename="FullAnalysis_${SPEC}_${KIN}"
    fi
else
    ##############
    # HARD CODED #
    ##############
    # Defines efficiency table to use
    #EffData="coin_heep_HeePCoin_efficiency_data_2022_09_09.csv"
    EffData="coin_heep_HeePCoin_efficiency_data_2022_12_02.csv"
    InDATAFilename="Raw_Data_${KIN}.root"
    InDUMMYFilename="Raw_DummyData_${KIN}.root"
    InSIMCFilename="Heep_Coin_${KIN}.root"
    OutDATAFilename="Analysed_Data_${KIN}"
    OutDUMMYFilename="Analysed_DummyData_${KIN}"
    if [[ $o_flag = "true" ]]; then
	OutFullAnalysisFilename="FullAnalysis_Offset_${KIN}"
    else
	OutFullAnalysisFilename="FullAnalysis_${KIN}"
    fi
fi

##############
# HARD CODED #
##############
# Define heep run numbers for a particular setting
if [[ $KIN = "10p6" && $s_flag != "true" ]]; then
    declare -a data=(4827 4828 4855 4856 4857 4858 4859 4860 4862 4863) # All heep coin 10p6 runs
    #    declare -a data=(4827) # Just one test run
    declare -a dummydata=(4864)
elif [[ $KIN = "8p2" ]]; then
    declare -a data=(7974 7975 7976)
    #    declare -a data=(7974) # Just one test run
    declare -a dummydata=(7977)
elif [[ $KIN = "6p2" ]]; then
    declare -a data=(7866 7867)
    declare -a dummydata=(7868)
elif [[ $KIN = "4p9" ]]; then
    declare -a data=(6881 6882)
    declare -a dummydata=(6883)
elif [[ $KIN = "3p8" ]]; then
    declare -a data=(6634 6635)
    declare -a dummydata=(6637)
elif [[ $KIN = "10p6" && $s_flag = "true" ]]; then
    declare -a data=(7974 7975 7976)
    #    declare -a data=(7974) # Just one test run
    declare -a dummydata=(7977)    
elif [[ $KIN = "8p2" && $s_flag != "true" ]]; then
    declare -a data=(111)
    declare -a dummydata=(111)
elif [[ $KIN = "4p9" && $s_flag = "true" ]]; then
    declare -a data=(111)
    declare -a dummydata=(111)
elif [[ $KIN = "3p8" && $s_flag = "true" ]]; then
    declare -a data=(111)
    declare -a dummydata=(111)
else
    echo "Invalid kinematic setting, ${KIN}"
    exit 128
fi

# When analysis flag is used then the analysis script (Analysed_{SPEC}.py)
# will create a new root file per run number which are combined using hadd
if [[ $a_flag = "true" ]]; then
    if [[ $s_flag = "true" ]]; then
	cd "${SIMCPATH}/scripts/HeeP/SING"
	echo
	echo "Analysing ${SPEC} data..."
	echo

	for i in "${data[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing data run $i..."
	    echo "-----------------------------"
	    echo
	    python3 Analysed_SING.py "$i" ${SPEC} | tee ../../log/Analysed_SING_${SPEC}_$i.log
	    #root -l <<EOF 
	    #.x $SIMCPATH/Analysed_SING.C("$InDATAFilename","$OutDATAFilename")
	    #EOF
	done
	cd "${SIMCPATH}/OUTPUT/Analysis/HeeP"
	echo
	echo "Combining root files..."  
	hadd -f ${OutDATAFilename}.root *_-1_${SPEC}_Raw_Data.root
	#rm -f *_-1_${SPEC}_Raw_Data.root

	cd "${SIMCPATH}/scripts/HeeP/SING"    
	echo
	echo "Analysing ${SPEC} dummy data..."
	echo

	for i in "${dummydata[@]}"
	do
	    echo
	    echo "-----------------------------------"
	    echo "Analysing dummy data run $i..."
	    echo "-----------------------------------"
	    echo
	    python3 Analysed_SING.py "$i" ${SPEC} | tee ../../log/Analysed_SING_${SPEC}_$i.log
	    #root -l <<EOF 
	    #.x $SIMCPATH/Analysed_SING.C("$InDUMMYFilename","$OutDUMMYFilename")
	    #EOF
	done
	cd "${SIMCPATH}/OUTPUT/Analysis/HeeP"
	echo
	echo "Combining root files..."
	hadd -f ${OutDUMMYFilename}.root *_-1_${SPEC}_Raw_Data.root
	#rm -f *_-1_${SPEC}_Raw_Data.root	
    else
	cd "${SIMCPATH}/scripts/HeeP/COIN"
	echo
	echo "Analysing data..."
	echo

	for i in "${data[@]}"
	do
	    echo
	    echo "-----------------------------"
	    echo "Analysing data run $i..."
	    echo "-----------------------------"
	    echo
	    python3 Analysed_COIN.py "$i" | tee ../../log/Analysed_COIN_$i.log
	    #root -l <<EOF 
	    #.x $SIMCPATH/Analysed_COIN.C("$InDATAFilename","$OutDATAFilename")
	    #EOF
	done
	cd "${SIMCPATH}/OUTPUT/Analysis/HeeP"
	echo
	echo "Combining root files..."  
	hadd -f ${OutDATAFilename}.root *_-1_Raw_Data.root
	for i in *_-1_Raw_Data.root; do mv -- "$i" "${i%_-1_Raw_Data.root}_-1_Raw_Target.root"; done

	cd "${SIMCPATH}/scripts/HeeP/COIN"    
	echo
	echo "Analysing dummy data..."
	echo

	for i in "${dummydata[@]}"
	do
	    echo
	    echo "-----------------------------------"
	    echo "Analysing dummy data run $i..."
	    echo "-----------------------------------"
	    echo
	    python3 Analysed_COIN.py "$i" | tee ../../log/Analysed_COIN_$i.log
	    #root -l <<EOF 
	    #.x $SIMCPATH/Analysed_COIN.C("$InDUMMYFilename","$OutDUMMYFilename")
	    #EOF
	done
	cd "${SIMCPATH}/OUTPUT/Analysis/HeeP"
	echo
	echo "Combining root files..."
	hadd -f ${OutDUMMYFilename}.root *_-1_Raw_Data.root
	for i in *_-1_Raw_Data.root; do mv -- "$i" "${i%_-1_Raw_Data.root}_-1_Raw_Dummy.root"; done
    fi
fi

cd "${SIMCPATH}/scripts"

DataChargeVal=()
DataEffVal=()
DataRunNum=()
echo
echo "Calculating data total effective charge..."
for i in "${data[@]}"
do
    # Calculates total efficiency then applies to the charge for each run number
    # to get the effective charge per run and saves as an array
    DataChargeVal+=($(python3 findEffectiveCharge.py ${EffData} ${ROOTPREFIX} "$i" -1))
    # Grabs the total effiency value per run and saves as an array
    DataEffVal+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
    DataRunNum+=("$i")
    #echo "${DataChargeVal[@]} uC"
done
#echo ${DataChargeVal[*]}
# Sums the array to get the total effective charge
# Note: this must be done as an array! This is why uC is used at this step
#       and later converted to C
DataChargeSum=$(IFS=+; echo "$((${DataChargeVal[*]}))") # Only works for integers
echo "${DataChargeSum} uC"

# Repeat the above process for dummy
DummyChargeVal=()
DummyEffVal=()
DummyRunNum=()
echo
echo "Calculating dummy total effective charge..."
for i in "${dummydata[@]}"
do
    DummyChargeVal+=($(python3 findEffectiveCharge.py ${EffData} ${ROOTPREFIX} "$i" -1))
    DummyEffVal+=($(python3 getEfficiencyValue.py "$i" ${EffData} "efficiency"))
    DummyRunNum+=($(echo "$i"))
    #echo "${DummyChargeVal[@]} uC"
done
#echo ${DummyChargeVal[*]}
DummyChargeSum=$(IFS=+; echo "$((${DummyChargeVal[*]}))") # Only works for integers
echo "${DummyChargeSum} uC"

# Finally, run the plotting script
if [[ $s_flag = "true" ]]; then
    cd "${SIMCPATH}/scripts/HeeP/SING"
    python3 HeepSing.py ${KIN} "${OutDATAFilename}.root" $DataChargeSum "${DataEffVal[*]}" "${OutDUMMYFilename}.root" $DummyChargeSum "${DummyEffVal[*]}" ${InSIMCFilename} ${OutFullAnalysisFilename} ${EffData} ${SPEC}
else
    cd "${SIMCPATH}/scripts/HeeP/COIN"
    python3 HeepCoin.py ${KIN} "${OutDATAFilename}.root" $DataChargeSum "${DataEffVal[*]}" "${DataRunNum[*]}" "${OutDUMMYFilename}.root" $DummyChargeSum "${DummyEffVal[*]}" "${DummyRunNum[*]}" ${InSIMCFilename} ${OutFullAnalysisFilename} ${EffData}
fi

cd "${SIMCPATH}"
evince "OUTPUT/Analysis/HeeP/${OutFullAnalysisFilename}.pdf"
