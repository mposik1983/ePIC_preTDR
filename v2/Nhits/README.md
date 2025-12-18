Scripts used to produce ePIC pre-TDR NHit vs. eta plots.

* Simulation Details:
    * particle gun: pi-
    * theta range: 3deg - 177deg
    * phi range: 0deg - 360deg
    * Fixed momentum values: 0.5, 1.0, 2.0, 5.0, 10.0 GeV
    * Nevents: 10,000

1) This analysis runs in eic-shell and assumes that the epic software repository (https://github.com/eic/epic.git) is installed for sim hits and eicrecon installed for rec hits.
2) Two detector configuration files are used with npsim:
    * craterlake_tracking_only.yml for analysis with the Silicon + ToF + MPGD 
    * craterlake_tracking_only_noMPGDs.yml for analysis excluding MPGD hits
3) Scripts to produce plots are
    * myScript_widebin.sh (for Sim Nhits)
        * script excecutes NhitsvsEta_ePIC.C and NhitsvsEta_ePIC_noMPGD.C for Sim Nhits includeing and excluding MPGDs
    * myScript_widebin_rec.sh (for Rec Nhits)
        * script excecutes RecNhitsvsEta_ePIC.C and RecNhitsvsEta_ePIC_noMPGD.C for Rec Nhits includeing and excluding MPGDs
4) CompareSimHits.C and CompareRecHits.C compares the histograms for hits with and without MPGDs for Sim hits and Rec hits, respectivly.    

Results from analysis are shown in MPGD_NHitsPerformance_epic25.10.0.pdf included in ePIC pre-TDR v2
