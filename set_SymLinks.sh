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
LTANAPATH=`echo ${PATHFILE_INFO} | cut -d ','  -f17`


# Check symlinks and create/fix if bad
if [ ! -L "${LTANAPATH}/simc_gfortran" ]; then
    echo ${LTANAPATH}
    ln -s "${SIMCPATH}" "${LTANAPATH}/simc_gfortran"
elif [ -L "${LTANAPATH}/simc_gfortran" ]; then
    if [ ! -e "${LTANAPATH}/simc_gfortran" ]; then
	echo "${LTANAPATH}/simc_gfortran sym link exits but is broken, replacing"
	rm "${LTANAPATH}/simc_gfortran"
	ln -s "${SIMCPATH}" "${LTANAPATH}/simc_gfortran"
    else 
	echo "${LTANAPATH}/simc_gfortran sym link already exists and not broken"
	echo "             ${LTANAPATH}/simc_gfortran-->${SIMCPATH}"
	echo
	echo
    fi
fi

if [ ! -L "${LTANAPATH}/OUTPUT" ]; then
    ln -s "${VOLATILEPATH}/OUTPUT/" "${LTANAPATH}/OUTPUT"
elif [ -L "${LTANAPATH}/OUTPUT" ]; then
    if [ ! -e "${LTANAPATH}/OUTPUT" ]; then
	echo "${LTANAPATH}/OUTPUT sym link exits but is broken, replacing"
	rm "${LTANAPATH}/OUTPUT"
	ln -s "${VOLATILEPATH}/OUTPUT/" "${LTANAPATH}/OUTPUT"
    else 
	echo "${LTANAPATH}/OUTPUT sym link already exists and not broken"
	echo "             ${LTANAPATH}/OUTPUT-->${VOLATILEPATH}/OUTPUT/"
	echo
	echo
    fi
fi

if [ ! -L "${LTANAPATH}/src/data" ]; then
    ln -s "${VOLATILEPATH}/OUTPUT/Analysis/${ANATYPE}LT/" "${LTANAPATH}/src/data"
elif [ -L "${LTANAPATH}/src/data" ]; then
    if [ ! -e "${LTANAPATH}/src/data" ]; then
	echo "${LTANAPATH}/src/data sym link exits but is broken, replacing"
	rm "${LTANAPATH}/src/data"
	ln -s "${VOLATILEPATH}/OUTPUT/Analysis/${ANATYPE}LT/" "${LTANAPATH}/src/data"
    else 
	echo "${LTANAPATH}/src/data sym link already exists and not broken"
	echo "             ${LTANAPATH}/src/data-->${VOLATILEPATH}/OUTPUT/Analysis/${ANATYPE}LT/"
	echo
	echo
    fi
fi

if [ ! -L "${LTANAPATH}/simc_gfortran/OUTPUTS" ]; then
    ln -s "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT/" "${LTANAPATH}/simc_gfortran/OUTPUTS"
elif [ -L "${LTANAPATH}/simc_gfortran/OUTPUTS" ]; then
    if [ ! -e "${LTANAPATH}/simc_gfortran/OUTPUTS" ]; then
	echo "${LTANAPATH}/simc_gfortran/OUTPUTS sym link exits but is broken, replacing"
	rm "${LTANAPATH}/simc_gfortran/OUTPUTS"
	ln -s "${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT/" "${LTANAPATH}/simc_gfortran/OUTPUTS"
    else 
	echo "${LTANAPATH}/simc_gfortran/OUTPUTS sym link already exists and not broken"
	echo "             ${LTANAPATH}/simc_gfortran/OUTPUTS-->${LTANAPATH}/OUTPUT/Analysis/${ANATYPE}LT/"
	echo
	echo
    fi
fi

if [ ! -L "${LTANAPATH}/simc_gfortran/input" ]; then
    ln -s "${LTANAPATH}/input/" "${LTANAPATH}/simc_gfortran/input"
elif [ -L "${LTANAPATH}/simc_gfortran/input" ]; then
    if [ ! -e "${LTANAPATH}/simc_gfortran/input" ]; then
	echo "${LTANAPATH}/simc_gfortran/input sym link exits but is broken, replacing"
	rm "${LTANAPATH}/simc_gfortran/input"
	ln -s "${LTANAPATH}/input/" "${LTANAPATH}/simc_gfortran/input"
    else 
	echo "${LTANAPATH}/simc_gfortran/input sym link already exists and not broken"
	echo "             ${LTANAPATH}/simc_gfortran/input-->${LTANAPATH}/input/"
	echo
	echo
    fi
fi

if [ ! -L "${LTANAPATH}/simc_gfortran/worksim" ]; then
    ln -s "${VOLATILEPATH}/worksim/" "${LTANAPATH}/simc_gfortran/worksim"
elif [ -L "${LTANAPATH}/simc_gfortran/worksim" ]; then
    if [ ! -e "${LTANAPATH}/simc_gfortran/worksim" ]; then
	echo "${LTANAPATH}/simc_gfortran/worksim sym link exits but is broken, replacing"
	rm "${LTANAPATH}/simc_gfortran/worksim"
	ln -s "${VOLATILEPATH}/worksim/" "${LTANAPATH}/simc_gfortran/worksim"
    else 
	echo "${LTANAPATH}/simc_gfortran/worksim sym link already exists and not broken"
	echo "             ${LTANAPATH}/simc_gfortran/worksim-->${VOLATILEPATH}/worksim/"
	echo
	echo
    fi
fi

echo 
echo "Directories and sym links for ${LTANAPATH} now setup"

exit 0
