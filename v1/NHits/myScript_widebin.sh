#!/bin/bash
# Script; shyam kumar; INFN Bari, Italy
# shyam.kumar@ba.infn.it; shyam055119@gmail.com
rm -rf Output/ && mkdir Output/
mom_array=(0.5 1.0 2.0 5.0 10.0 15.0)
particle_array=("pi-")
nevents=50000

# run the simulation
source ../epic/install/bin/thisepic.sh 
#dd_web_display --export -o epic_craterlake_tracking_only.root ../epic/install/share/epic/epic_craterlake_tracking_only.xml
for ((i=0; i<${#mom_array[@]}; i++)); do
root -b -l -q NhitsvsEta_ePIC.C'('${mom_array[i]}')'
done


