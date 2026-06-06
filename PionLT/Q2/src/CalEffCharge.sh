#!/bin/bash

echo ""
echo "Runnning to calculate the effective charge"

REPLAYPATH="/group/c-kaonlt/USERS/${USER}/hallc_replay_lt"                         

echo ""
echo ""

echo " Calculating the effective charge"
echo " For LH2 data "

while IFS='' read -r Line || [[ -n "$Line" ]]; do
    RUNNUMBER=$Line
    echo ""
    echo "Running ${RUNNUMBER}"
    echo ""
    echo ""

    while IFS= read -r line; do
	if [[ $line == KLT_BCM1_Beam_Cut_Charge* ]] ; then
	    printline="yes"
	    charge=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_BCM1_Beam_Cut_Current* ]] ; then
	    printline="yes"
	    I=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_SHMS_Pion_COIN_TRACK_EFF* ]] ; then
	    printline="yes"
	    Piontrkeff=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_HMS_Elec_COIN_TRACK_EFF* ]] ; then
	    printline="yes"
	    Elecrontrkeff=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_SHMS_Hodo_3_of_4_EFF* ]] ; then
	    printline="yes"
	    SHMS3Y4=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_HMS_Hodo_3_of_4_EFF* ]] ; then
	    printline="yes"
	    HMS3Y4=$(echo $line | awk '{print $3}')
	fi

	if [[ $line == KLT_Non_Scaler_EDTM_Live_Time_Corr* ]] ; then
	    printline="yes"
	    edtmlive=$(echo $line | awk '{print $3}')
	fi
		
	if [[ $line == KLT_COIN_CPULT* ]] ; then
	    printline="yes"
	    CPUlive=$(echo $line | awk '{print $3}')
	fi	
	
#    done < "/group/c-kaonlt/USERS/vijay/Analysis_Framework_Sep6_2022/hallc_replay_lt/REPORT_OUTPUT/Production/Kaon_replay_production_${RUNNUMBER}_-1.report"
    done < "/cache/hallc/kaonlt/Data_Analysis_E12_19_006_Low_Q2/REPORT_OUTPUT/Production/Kaon_replay_production_${RUNNUMBER}_-1.report"
 
    if [[ -f "/u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/${RUNNUMBER}.dat" ]]; then  # check does file exist   
        cd /u/home/vijay/LTsepscripts/Analysis/Q2/CalEffCharge/
        rm "${RUNNUMBER}.dat"
        echo "An old file exist, it has now been deleted !!!"

        cd /u/home/vijay/LTsepscripts/Analysis/Q2/
        root -b -q  "CalEffCharge.C(\"${RUNNUMBER}\", ${charge}, ${I}, ${Piontrkeff}, ${Elecrontrkeff}, ${edtmlive}, ${CPUlive}, ${SHMS3Y4}, ${HMS3Y4})"
    else
        cd /u/home/vijay/LTsepscripts/Analysis/Q2/
        root -b -q  "CalEffCharge.C(\"${RUNNUMBER}\", ${charge}, ${I}, ${Piontrkeff}, ${Elecrontrkeff}, ${edtmlive},  ${CPUlive}, ${SHMS3Y4}, ${HMS3Y4})"

    fi
    
#2p7 low epsilon    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_lowe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_lowe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_lowe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_lowe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_lowe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_lowe_dummy"    

#3p6 mid epsilon

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_mide"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_mide_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_mide"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_mide_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_mide"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_mide_dummy"    

#4p5 high epsilon

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_highe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2center_highe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_highe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left1_highe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_highe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2left2_highe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2right1_highe"    
#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2right1_highe_dummy"    

#done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2right2_highe"    
done < "${REPLAYPATH}/UTIL_BATCH/InputRunLists/KaonLT_2018_2019/Q0p425W2p2right2_highe_dummy"    


echo ""
echo "Processing is completed" 
exit 0 
