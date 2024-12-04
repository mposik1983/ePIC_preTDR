#! /bin/env csh
set epic_ver = "24.11.1"
set ith=3.0  
set ith_max = 177
  foreach ip (0.5 1.0 2.0 5.0 10.0 15.0)
    set Nfile = 1
    set m_batch = 1
    set m_phiMin = 0.0
    set m_phiMax = 360.0
    set m_thMin = $ith
    set m_thMax = ${ith_max}
    set m_momMin =  $ip
    set m_momMax =  $ip
    set m_events = 50000
    set m_part = pi-
    
#set paths and file names
    set m_BaseDIR = /direct/eic+u/mposik/Detector1/EPIC/Condor/angres
    set m_Myoutdir = /gpfs02/eic/mposik/ePIC/NHits/npsim
    set m_Exec = ${m_BaseDIR}/run_ddsim.sh
    set m_FileName = npsim_${epic_ver}_${m_part}_${m_momMax}GeV_${m_thMin}deg_${m_thMax}deg_
    echo "m_momMin = ${m_momMin}, m_momMax = ${m_momMax}"
    echo "m_thMin = ${m_thMin}, m_thMax = ${m_thMax}"
    echo "m_phiMin = ${m_phiMin}, m_phiMax = ${m_phiMax}"
    echo "m_events = ${m_events}"

    echo "source GenerateCondorFile_ddsim.csh ${Nfile} ${m_batch} ${m_momMin} ${m_momMax} ${m_thMin} ${m_thMax} ${m_phiMin} ${m_phiMax} ${m_BaseDIR} ${m_Myoutdir} ${m_Exec} ${m_FileName} ${m_events} ${m_part}"

   source GenerateCondorFile_ddsim.csh ${Nfile} ${m_batch} ${m_momMin} ${m_momMax} ${m_thMin} ${m_thMax} ${m_phiMin} ${m_phiMax} ${m_BaseDIR} ${m_Myoutdir} ${m_Exec} ${m_FileName} ${m_events} ${m_part}
   end
end
