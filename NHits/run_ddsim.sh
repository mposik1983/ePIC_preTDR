#!/usr/bin/env bash
cd /direct/eic+u/mposik/Detector1/EPIC
export EPIC_SOURCE=install/bin/thisepic.sh
export DETECTOR_PATH=/eic/u/mposik/Detector1/EPIC/epic/install/share/epic
export ePICDIR=epic
export myDETECTOR_CONFIG=epic_craterlake_tracking_only
export BEAMLINE_CONFIG=ip6       # ip6 is the default
export N_EVENTS=${10}
export batch=${1}
export momMin=${2}
export momMax=${3}
export thMin=${4}
export thMax=${5}
export phiMin=${6}
export phiMax=${7}
export OUTFILE=${8}/rootfiles
export FILENAME=${9}${batch}.edm4eic.root
export PART=${11}

echo "Running batch: ${1} with ${EVENTS_PER_FILE} per job"

./eic-shell<<EOF
echo in eic shell
cd ${ePICDIR}
source ${EPIC_SOURCE}

echo EPIC_SOURCE: ${EPIC_SOURCE}
echo DETECTOR_CONFIG: ${myDETECTOR_CONFIG}
echo DETECTOR_PATH: ${DETECTOR_PATH}

echo "npsim --compactFile ${DETECTOR_PATH}/${myDETECTOR_CONFIG}.xml --enableGun --gun.thetaMin ${thMin}*deg --gun.thetaMax ${thMax}*deg --gun.phiMin ${phiMin}*deg --gun.phiMax ${phiMax}*deg --gun.particle ${PART} --gun.momentumMin ${momMin}*GeV --gun.momentumMax ${momMax}*GeV --gun.multiplicity 1  --gun.distribution eta --numberOfEvents ${N_EVENTS} --outputFile ${OUTFILE}/${FILENAME}"

npsim --compactFile ${DETECTOR_PATH}/${myDETECTOR_CONFIG}.xml --enableGun --gun.thetaMin ${thMin}*deg --gun.thetaMax ${thMax}*deg --gun.phiMin ${phiMin}*deg --gun.phiMax ${phiMax}*deg --gun.particle ${PART} --gun.momentumMin ${momMin}*GeV --gun.momentumMax ${momMax}*GeV --gun.multiplicity 1  --gun.distribution eta --numberOfEvents ${N_EVENTS} --outputFile ${OUTFILE}/${FILENAME}

exit
EOF
