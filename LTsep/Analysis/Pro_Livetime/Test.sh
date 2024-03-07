#!/bin/bash
echo " "

selected_line=$(sed -n "${50}p" "/lustre19/expphy/volatile/hallc/c-kaonlt/vijay/REPORT_OUTPUT/Production/Kaon_replay_production_8604_-1.report")

echo ${selected_line}

exit 0 
