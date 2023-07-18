#!/bin/bash
echo ""
echo "Runnning for the HMS Calorimeter efficiency study"

if [[ "${HOSTNAME}" = *"ifarm"* ]]; then  
    REPLAYPATH="/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt"                         
    cd "$REPLAYPATH"
    source "$REPLAYPATH/setup.sh"    
fi

if [[ "${HOSTNAME}" != *"ifarm"* ]]; then
    source /site/12gev_phys/softenv.sh 2.3
    cd "/group/c-kaonlt/hcana/"
    source "/group/c-kaonlt/hcana/setup.sh"
    cd "$REPLAYPATH"
    source "$REPLAYPATH/setup.sh"
fi
echo ""
echo ""
echo ""

echo ""
echo "Processing data..."
while IFS='' read -r Line || [[ -n "$Line" ]]; do
    RUNNUMBER=$Line
    echo ""
    echo "Running ${RUNNUMBER}"
    echo ""		
    echo ""
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/	
	rm "${RUNNUMBER}.root"
	echo "An old file exist, it has now been deleted !!!"
	
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/
#	eval "root -l -q \"/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Analysed.C("${RUNNUMBER}")\""
#	root -b -q  "Analysed.C(\"${RUNNUMBER}\")"
	root -b -q  "HeepCoin.C(\"${RUNNUMBER}\")"
    else
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/
#	eval "root -l -q \"/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Analysed.C("${RUNNUMBER}")\""
#	root -b -q "Analysed.C(\"${RUNNUMBER}\")"
	root -b -q  "HeepCoin.C(\"${RUNNUMBER}\")"

    fi
    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1"
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19_All"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/tmp"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_highe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left1_highe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left1_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left2_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19_All"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_2p7"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_3p6"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_4p5"

#while IFS='' read -r line || [[ -n "$line" ]]; do
#    runNum=$line
#    RootName1+="{runNum}.root "
    # Change this path to match where your script is trying to look for files
    #if [ ! -f "${UTILPATH}/OUTPUT/Analysis/ProtonLT/${runNum}_-1_Analysed_Data.root" ]; then
    # Change this warning to be relevant/useful for you
    #   echo "Proton analysis not found for run $runNum in ${UTILPATH}/OUTPUT/Analysis/ProtonLT/"
    #  TestingVar=$((TestingVar+1))
    #cd ${REPLAYPATH}/UTIL_KAONLT/scripts/HeepSingleStudy/SHMS_Heep/

#done < "/group/c-kaonlt/USERS/${USER}/hallc_replay_lt/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/RunListHMSCalEff"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/tmp"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_highe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left1_highe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2center_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left1_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p375W2p2left2_lowe"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_Summer19_All"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_2p7"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_3p6"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/HeeP_Coin_4p5"

echo ""

echo ""
echo "Processing is completed" 
exit 0 
