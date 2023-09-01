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
	    cointrigg_rate=$(echo $line | awk '{print $3}')
	    echo $cointrigg_rate	    
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
	    
	if [[ $line == KLT_COIN_CPULT* ]] ; then
	    printline="yes"
	    cpulive=$(echo $line | awk '{print $3}')
	    echo $cpulive
	fi	    
	
	if [[ $line == KLT_COIN_CPULT* ]] ; then
	    printline="yes"
	    cpuliveerr=$(echo $line | awk '{print $5}')
	    echo $cpuliveerr
	fi	    

	if [[ $line == KLT_BCM1_Beam_Cut_Current* ]] ; then
	    printline="yes"
	    I=$(echo $line | awk '{print $3}')
	    echo $I    
	fi	

	if [[ $line == KLT_SHMS_Hodo_3_of_4_EFF* ]] ; then
	    printline="yes"
	    SHMS3Y4=$(echo $line | awk '{print $3}')
	    echo $SHMS3Y4    
	fi	

	if [[ $line == KLT_SHMS_Hodo_Plane_1* ]] ; then
	    printline="yes"
	    SHMSHOD1=$(echo $line | awk '{print $3}')
	    echo $SHMSHOD1    
	fi	

	if [[ $line == KLT_SHMS_Hodo_Plane_2* ]] ; then
	    printline="yes"
	    SHMSHOD2=$(echo $line | awk '{print $3}')
	    echo $SHMSHOD2    
	fi	

	if [[ $line == KLT_SHMS_Hodo_Plane_3* ]] ; then
	    printline="yes"
	    SHMSHOD3=$(echo $line | awk '{print $3}')
	    echo $SHMSHOD3    
	fi	

	if [[ $line == KLT_SHMS_Hodo_Plane_4* ]] ; then
	    printline="yes"
	    SHMSHOD4=$(echo $line | awk '{print $3}')
	    echo $SHMSHOD4    
	fi		

    done < "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/REPORT_OUTPUT/Production/Kaon_replay_production_${RUNNUMBER}_-1.report" 
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
    	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/OUTPUT/	
    	rm "${RUNNUMBER}.root"
    	echo "An old file exist, now it has been deleted !!!"
    	
    	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/
    	root -b -q  "Analysed.C(\"${RUNNUMBER}\", ${cointrigg_rate}, ${edtmlive}, ${edtmliveerr}, ${cpulive}, ${cpuliveerr}, ${I}, ${SHMS3Y4}, ${SHMSHOD1}, ${SHMSHOD2}, ${SHMSHOD3}, ${SHMSHOD4})"
    else
    	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Pro_Livetime/	
   	root -b -q "Analysed.C(\"${RUNNUMBER}\", ${cointrigg_rate}, ${edtmlive}, ${edtmliveerr}, ${cpulive}, ${cpuliveerr}, ${I}, ${SHMS3Y4}, ${SHMSHOD1}, ${SHMSHOD2}, ${SHMSHOD3}, ${SHMSHOD4})"
    fi
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/PionLT_Production_Q1Q2"

echo ""
echo "Processing is completed" 
exit 0 
