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

#    if [[ ! -f "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/ROOTfiles/Kaon_replay_luminosity_${RUNNUMBER}_-1.root" ]]; then  # check does file exist
    if [[ ! -f "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/ROOTfiles/ROOTfiles_OLD/Kaon_replay_luminosity_${RUNNUMBER}_-1.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/UTIL_BATCH/Analysis_Scripts/	
	
	echo "Running Replay !!!"
	
	./Batch_Analysis_Script.sh ${RUNNUMBER} -1
	
    else
	
	echo "Skipped Replay for ${RUNNUMBER}"

    fi
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/tmp"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Lumi_2p7_plot"
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Carbon_Lumi_2p7_plot"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Carbon_Lumi_2p7"

while IFS='' read -r Line || [[ -n "$Line" ]]; do
    RUNNUMBER=$Line
    echo ""
    echo "Running ${RUNNUMBER}"
    echo ""		
    echo ""
    
    while IFS= read -r line; do

	if [[ RUNNUMBER -eq  8469 || 8470 || 8480 ]] ; then

	    if [[ $line == SW_BCM1_Charge* ]] ; then
		printline="yes"
		charge=$(echo $line | awk '{print $3}')
		echo $charge	    
	    fi
	    
	    if [[ $line == SW_BCM1_Current* ]] ; then
		printline="yes"
		I=$(echo $line | awk '{print $3}')
		echo $I	    
	    fi
	    
	else
	    
	    if [[ $line == SW_BCM1_Beam_Cut_Charge* ]] ; then
		printline="yes"
		charge=$(echo $line | awk '{print $3}')
		echo $charge	    
	    fi
	    
	    if [[ $line == SW_BCM1_Beam_Cut_Current* ]] ; then
		printline="yes"
		I=$(echo $line | awk '{print $3}')
		echo $I	    
	    fi

	fi

	if [[ $line == SW_Ps2_factor* ]] ; then
	    printline="yes"
	    PSVh=$(echo $line | awk '{print $3}')
	    echo $PSVh
	fi	    

	if [[ $line == SW_Ps3_factor* ]] ; then
	    printline="yes"
	    PSV=$(echo $line | awk '{print $3}')
	    echo $PSV
	fi	    

	if [[ $line == EDTM_H_EDTM_scaler* ]] ; then
	    printline="yes"
	    hedtmsent=$(echo $line | awk '{print $3}')
	    echo $hedtmsent
	fi	    

	if [[ $line == SW_HMS_Electron_Singles_TRACK_EFF* ]] ; then
	    printline="yes"
	    HMStreff=$(echo $line | awk '{print $3}')
	    echo $HMStreff
	fi	    

    done < "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/REPORT_OUTPUT/Lumi/Kaon_replay_luminosity_${RUNNUMBER}_-1.report" 
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/OUTPUT/	
	rm "${RUNNUMBER}.root"
	echo "An old file exist, now it has been deleted !!!"
	
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/
	root -b -q  "Analysed.C(\"${RUNNUMBER}\", ${PSV}, ${PSVh}, ${charge}, ${I}, ${hedtmsent}, ${HMStreff})"
    else
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/	
	root -b -q "Analysed.C(\"${RUNNUMBER}\", ${PSV}, ${PSVh}, ${charge}, ${I}, ${hedtmsent}, ${HMStreff})"
    fi

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/tmp"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Lumi_2p7_plot"
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Carbon_Lumi_2p7"
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/Pion_Carbon_Lumi_2p7_plot"

#while IFS='' read -r line || [[ -n "$line" ]]; do
#    runNum=$line
#    RootName1+="{runNum}.root "
# Change this path to match where your script is trying to look for files
#if [ ! -f "${UTILPATH}/OUTPUT/Analysis/ProtonLT/${runNum}_-1_Analysed_Data.root" ]; then
# Change this warning to be relevant/useful for you
#   echo "Proton analysis not found for run $runNum in ${UTILPATH}/OUTPUT/Analysis/ProtonLT/"
#  TestingVar=$((TestingVar+1))
#cd ${REPLAYPATH}/UTIL_KAONLT/scripts/HeepSingleStudy/SHMS_Heep/

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/tmp"

#if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/Lumi/OUTPUT/Q375.root" ]]; then  # check does file exist
#    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/
#    rm "Q375.root"
#    echo "An old file exist, now it has been deleted !!!"
#    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/
#    hadd "Q375.root" ${RootName1}
#else
#    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/OUTPUT/
#    hadd "Q375.root" ${RootName1}
#fi  

echo ""

echo ""
echo "Processing is completed" 
exit 0 
