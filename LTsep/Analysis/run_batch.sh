#! /bin/bash

echo "Running as ${USER}"

if [[ $1 -eq "" ]]; then
    MAXEVENTS=-1
else
    MAXEVENTS=$1
fi

##Output history file##
historyfile=hist.$( date "+%Y-%m-%d_%H-%M-%S" ).log

##Output batch script##
batch="${USER}_Job.txt"

Workflow="LTSep_${USER}"

while true; do
    read -p "Do you wish to begin a new batch submission for physics analysis? (Please answer yes or no) " yn
    case $yn in
        [Yy]* )
            i=-1
            cp /dev/null ${batch}
            ##Creation of batch script for submission##
            echo "PROJECT: c-kaonlt" >> ${batch}
            echo "TRACK: analysis" >> ${batch}
            echo "JOBNAME:physics" >> ${batch}
            # Request disk space depending upon raw file size
            
	    # echo "DISK_SPACE: "$(( $TapeFileSize * 3 ))" GB" >> ${batch}
	    #  if [[ $TapeFileSize -le 45 ]]; then
	    #	echo "MEMORY: 4000 MB" >> ${batch}
	    #    elif [[ $TapeFileSize -ge 45 ]]; then
	    #		echo "MEMORY: 6000 MB" >> ${batch}
	    #	    fi

	    echo "OS: centos7" >> ${batch}
	    echo "OS: general" >> ${batch}
            echo "CPU: 1" >> ${batch} ### hcana single core, setting CPU higher will lower priority!              
	    #	    echo "INPUT_FILES: ${tape_file}" >> ${batch}
	    #echo "TIME: 1" >> ${batch} 
	    echo "COMMAND:/group/c-kaonlt/USERS/vijay/lt_analysis/LTsep/Analysis/RunScript.sh" >> ${batch}
	    echo "MAIL: ${USER}@jlab.org" >> ${batch}
            echo "Submitting batch"
	    #                eval "jsub ${batch} 2>/dev/null"
	    eval "/site/bin/swif2 add-jsub ${Workflow} -script ${batch} 2>/dev/null"
            echo " "
            i=$(( $i + 1 ))
	    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	    echo " "
	    echo "###############################################################################################################"
	    echo "############################################ END OF THE JOB SUBMISSION ###########################################"
	    echo "###############################################################################################################"
	    echo " "
	    
	    eval 'swif2 run ${Workflow}'
	    break;;
        [Nn]* ) 
	    exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
