#!/bin/bash
echo ""
echo "Runnning for DQA live time study"

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
#	if [[ $line == KLT_BCM1_Beam_Cut_Current* ]] ; then
	if [[ $line == KLT_COIN_Trigger_Rate* ]] ; then
	    printline="yes"
	    I=$(echo $line | awk '{print $3}')
	    echo $I	    
	fi

	if [[ $line == KLT_Non_Scaler_EDTM_Live_Time* ]] ; then
	    printline="yes"
	    edtmlive=$(echo $line | awk '{print $3}')
	    echo $edtmlive
	fi	    

	if [[ $line == KLT_Non_Scaler_EDTM_Live_Time* ]] ; then
	    printline="yes"
	    edtmliveerr=$(echo $line | awk '{print $5}')
	    echo $edtmliveerr
	fi	    


    done < "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/REPORT_OUTPUT/Production/Kaon_replay_production_${RUNNUMBER}_-1.report" 
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/	
	rm "${RUNNUMBER}.root"
	echo "An old file exist, now it has been deleted !!!"
	
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/
	root -b -q  "Analysed.C(\"${RUNNUMBER}\", ${I}, ${edtmlive}, ${edtmliveerr})"
    else
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/	
	root -b -q "Analysed.C(\"${RUNNUMBER}\", ${I}, ${edtmlive}, ${edtmliveerr})"
    fi
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1Q2"

echo ""
echo "Processing is completed" 
exit 0 
