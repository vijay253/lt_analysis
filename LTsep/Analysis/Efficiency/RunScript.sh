#!/bin/bash
echo ""
echo "Runnning for Lumi study"

if [[ "${HOSTNAME}" = *"ifarm"* ]]; then  
    REPLAYPATH="/group/c-kaonlt/USERS/${USER}/Analysis_Framework_Sep6_2022/hallc_replay_lt"                         
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
    
    while IFS= read -r line; do

	if [[ $line == Pion_COIN_TRACK_EFF* ]] ; then
	    printline="yes"
	    SHMStreff=$(echo $line | awk '{print $3}')
	    SHMStrefferr=$(echo $line | awk '{print $5}')
	fi	    

	if [[ $line == KLT_SHMS_Hodoscope_S1X_Rate* ]] ; then
	    printline="yes"
	    SHMS_S1X_Rate=$(echo $line | awk '{print $3}')
	fi	    

	if [[ $line == Elec_COIN_TRACK_EFF* ]] ; then
	    printline="yes"
	    HMStreff=$(echo $line | awk '{print $3}')
	    HMStrefferr=$(echo $line | awk '{print $5}')
	fi	    

	if [[ $line == KLT_HMS_Hodoscope_S1X_Rate* ]] ; then
	    printline="yes"
	    HMS_S1X_Rate=$(echo $line | awk '{print $3}')
	fi	    

    done < "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/REPORT_OUTPUT/Production/Kaon_replay_production_${RUNNUMBER}_-1.report" 
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/OUTPUT/	
	rm "${RUNNUMBER}.root"
	echo "An old file exist, now it has been deleted !!!"
	
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/
	root -b -q  "Effiency.C(\"${RUNNUMBER}\", ${SHMStreff}, ${SHMStrefferr}, ${HMStreff}, ${HMStrefferr}, ${SHMS_S1X_Rate}, ${HMS_S1X_Rate})"
    else
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Efficiency/	
	root -b -q "Efficiency.C(\"${RUNNUMBER}\", ${SHMStreff}, ${SHMStrefferr}, ${HMStreff}, ${HMStrefferr}, ${SHMS_S1X_Rate}, ${HMS_S1X_Rate})"
    fi
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/EffRunList"
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1Q2"

echo ""
echo "Processing is completed" 
exit 0 
