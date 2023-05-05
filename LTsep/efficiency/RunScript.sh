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
    
    cd /group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/efficiency/
    
    root -b -q "Analysed.C(\"${RUNNUMBER}\")"
    
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/RunListHMSCalEff"


echo ""
echo "Processing is completed" 
exit 0 
