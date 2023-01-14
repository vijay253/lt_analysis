#! /bin/bash

while getopts 'hra' flag; do
    case "${flag}" in
        h) 
        echo "----------------------------------------------------------------"
        echo "./edit_HeepInput.sh -{flags} {kinematic setting} {offset values}"
        echo "----------------------------------------------------------------"
        echo
        echo "The following flags can be called for the heep analysis..."
        echo "    -h, help"
        echo "    -r, reset offset, if no offset given the input offsets are set to default."
        echo "    -a, analyze new offset simc"
        exit 0
        ;;
        r) r_flag='true' ;;
        a) a_flag='true' ;;
        *) print_usage
        exit 1 ;;
    esac
done

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

if [[ $r_flag = "true" ]]; then
    KIN=$2
    OFFSET=$3
    cd "${SIMCPATH}/input"
    cp "Heep_Coin_${KIN}.inp" "Heep_Coin_${KIN}_Offset.inp"
    #if [[ $3 -eq "" ]]; then
    if [[ -z "$3" ]]; then
	exit 0
    fi
elif [[ $a_flag = "true" ]]; then
    KIN=$2
    OFFSET=$3
else
    KIN=$1
    OFFSET=$2
fi

InputSIMC="Heep_Coin_${KIN}_Offset"

cd "${SIMCPATH}/scripts"
python3 setOffset.py ${InputSIMC} ${OFFSET}

if [[ $a_flag = "true" ]]; then
    echo
    echo 
    echo "Running simc analysis for ${InputSIMC}..."
    echo
    cd "${SIMCPATH}"
    ./run_simc_tree "${InputSIMC}"
fi
