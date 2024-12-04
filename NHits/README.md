Scripts used to produce ePIC pre-TDR NHit vs. eta plots.

1) This analysis runs in eic-shell and assumes that the epic software repository (https://github.com/eic/epic.git) is installed.
2) Do_ddsim.csh: script used to define simulation parameters, paths and file names that are then passed to run_ddsim.sh 
3) run_ddsim.sh: script that runs the npsim simulation and produces output root files that are to be read by myScript_widebin.sh
4) myScript_widebin.sh: script that executes NhitsvsEta_ePIC.C for various momentum settings
5) NhitsvsEta_ePIC.C: access output npsim rootfiles to count and histogram the number of hits from each tracking detector for given momentum and eta. 

Results from analysis are shown in MPGD_NHitsPerformance_epic24.11.1.png and is included in ePIC pre-TDR v1.0
