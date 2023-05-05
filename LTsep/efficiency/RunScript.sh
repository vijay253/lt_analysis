#!/bin/bash
echo ""
echo "Runnning for the HMS Calorimeter efficiency study"

if [[ "${HOSTNAME}" = *"ifarm"* ]]; then  
    REPLAYPATH="/group/c-kaonlt/USERS/${USER}/hallc_replay_lt"                         
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
    
    if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/${RUNNUMBER}.root" ]]; then  # check does file exist
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/	
	rm "${RUNNUMBER}.root"
	echo "An old file exist, now it has been deleted !!!"
	
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/
	root -b -q  "Analysed.C(\"${RUNNUMBER}\")"
    else
	cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/	
	root -b -q "Analysed.C(\"${RUNNUMBER}\")"
    fi
    
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/RunListHMSCalEff"

while IFS='' read -r line || [[ -n "$line" ]]; do
    runNum=$line
    RootName1+="{runNum}.root "
    # Change this path to match where your script is trying to look for files
    #if [ ! -f "${UTILPATH}/OUTPUT/Analysis/ProtonLT/${runNum}_-1_Analysed_Data.root" ]; then
    # Change this warning to be relevant/useful for you
    #   echo "Proton analysis not found for run $runNum in ${UTILPATH}/OUTPUT/Analysis/ProtonLT/"
    #  TestingVar=$((TestingVar+1))
    #cd ${REPLAYPATH}/UTIL_KAONLT/scripts/HeepSingleStudy/SHMS_Heep/

done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/RunListHMSCalEff"

if [[ -f "/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/Q375.root" ]]; then  # check does file exist
    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/
    rm "Q375.root"
    echo "An old file exist, now it has been deleted !!!"
    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/
    hadd "Q375.root" ${RootName1}
else
    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/OUTPUT/
    hadd "Q375.root" ${RootName1}
fi  

echo ""

echo ""
echo "Processing is completed" 
exit 0 
