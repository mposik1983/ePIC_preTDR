// Code to draw average number of hits vs eta at the generated level
// Shyam Kumar; shyam055119@gmail.com; shyam.kumar@ba.infn.it
void NhitsvsEta_ePIC_noMPGDs(Double_t mom=5.0)
  {
  
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(1.0,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.04,"X");gStyle->SetLabelSize(.04,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   
     // MC Track Properties
    TString InDIR = "/gpfs02/eic/mposik/ePIC/hits/pre-TDRv2/npsim/noMPGDs/rootfiles";
    TFile* file = new TFile(Form("%s/npsim_25.10.0_pi-_%0.1fGeV_3deg_177deg_0.0deg_360.0deg_1.edm4eic.root",InDIR.Data(),mom)); // Tree with tracks and hits

    TFile * ofile = new TFile(Form("./Output/Nhitsvsmom_%.1f_noMPGD_hist.root",mom),"recreate");
    ofile->cd();
    // auto file = TFile::Open("root://dtn-eic.jlab.org//volatile/eic/EPIC/RECO/25.10.0/epic_craterlake/DIS/NC/18x275/minQ2=10/pythia8NCDIS_18x275_minQ2=10_beamEffects_xAngle=-0.025_hiDiv_4.0247.eicrecon.edm4eic.root");
    TTreeReader myReader("events", file); // name of tree and file
		
   TTreeReaderArray<Float_t> charge(myReader, "MCParticles.charge"); 
   TTreeReaderArray<Double_t> vx_mc(myReader, "MCParticles.vertex.x"); 
   TTreeReaderArray<Double_t> vy_mc(myReader, "MCParticles.vertex.y"); 
   TTreeReaderArray<Double_t> vz_mc(myReader, "MCParticles.vertex.z"); 
   TTreeReaderArray<Double_t> px_mc(myReader, "MCParticles.momentum.x"); 
   TTreeReaderArray<Double_t> py_mc(myReader, "MCParticles.momentum.y"); 
   TTreeReaderArray<Double_t> pz_mc(myReader, "MCParticles.momentum.z"); 
   TTreeReaderArray<Int_t> status(myReader, "MCParticles.generatorStatus"); 
   TTreeReaderArray<Int_t> pdg(myReader, "MCParticles.PDG"); 

  TTreeReaderArray<Double_t> *vtx_si_x, *vtx_si_y, *vtx_si_z;
  TTreeReaderArray<Double_t> *barrel_si_x, *barrel_si_y, *barrel_si_z; 
  TTreeReaderArray<Double_t> *disks_si_x, *disks_si_y, *disks_si_z; 
  TTreeReaderArray<Double_t> *endcap_etof_x, *endcap_etof_y, *endcap_etof_z;
  TTreeReaderArray<Double_t> *barrel_tof_x, *barrel_tof_y, *barrel_tof_z;

  // check the quality flag for hits from primary tracks
  TTreeReaderArray<Int_t> *vtx_si_quality, *barrel_si_quality, *disks_si_quality, *endcap_etof_quality, *barrel_tof_quality; 	


   // Hits on detectors
   // SVT IB
   vtx_si_x = new TTreeReaderArray<Double_t>(myReader, "VertexBarrelHits.position.x"); 
   vtx_si_y = new TTreeReaderArray<Double_t>(myReader, "VertexBarrelHits.position.y"); 
   vtx_si_z = new TTreeReaderArray<Double_t>(myReader, "VertexBarrelHits.position.z"); 
   vtx_si_quality = new TTreeReaderArray<Int_t>(myReader, "VertexBarrelHits.quality");
   
   // SVT OB
   barrel_si_x = new TTreeReaderArray<Double_t>(myReader, "SiBarrelHits.position.x"); 
   barrel_si_y = new TTreeReaderArray<Double_t>(myReader, "SiBarrelHits.position.y"); 
   barrel_si_z = new TTreeReaderArray<Double_t>(myReader, "SiBarrelHits.position.z");
   barrel_si_quality = new TTreeReaderArray<Int_t>(myReader, "SiBarrelHits.quality");
   
   // SVT Disks
   disks_si_x = new TTreeReaderArray<Double_t>(myReader, "TrackerEndcapHits.position.x"); 
   disks_si_y = new TTreeReaderArray<Double_t>(myReader, "TrackerEndcapHits.position.y"); 
   disks_si_z = new TTreeReaderArray<Double_t>(myReader, "TrackerEndcapHits.position.z");
   disks_si_quality = new TTreeReaderArray<Int_t>(myReader, "TrackerEndcapHits.quality");
   
   // ETOF Hits
   endcap_etof_x = new TTreeReaderArray<Double_t>(myReader, "TOFEndcapHits.position.x"); 
   endcap_etof_y = new TTreeReaderArray<Double_t>(myReader, "TOFEndcapHits.position.y"); 
   endcap_etof_z = new TTreeReaderArray<Double_t>(myReader, "TOFEndcapHits.position.z");
   endcap_etof_quality = new TTreeReaderArray<Int_t>(myReader, "TOFEndcapHits.quality"); 
    
   // BarrelTOF
   barrel_tof_x = new TTreeReaderArray<Double_t>(myReader, "TOFBarrelHits.position.x"); 
   barrel_tof_y = new TTreeReaderArray<Double_t>(myReader, "TOFBarrelHits.position.y"); 
   barrel_tof_z = new TTreeReaderArray<Double_t>(myReader, "TOFBarrelHits.position.z");
   barrel_tof_quality = new TTreeReaderArray<Int_t>(myReader, "TOFBarrelHits.quality");  
   

   Double_t etamc = 0.; 
   int iEvent=-1; 
   
   TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
   c1->SetMargin(0.10, 0.05 ,0.1,0.07);
   c1->SetGridx();
   c1->SetGridy();
  
  TProfile* hits = new TProfile("hits","Nhits (#theta)",70,-3.5,3.5); 
  hits->SetTitle(Form("p = %1.1f GeV/c;#eta_{mc};Nhits",mom));
  hits->GetXaxis()->CenterTitle();
  hits->GetYaxis()->CenterTitle();
  hits->SetMinimum(0.);
  
    std::vector<TVector3> hitPos; // The ePIC tracker
    double epsilon = 10e-5;
    bool debug = true;
    int nhits_SVTIB, nhits_SVTOB, nhits_BTOF, nhits_SVTDisks, nhits_ETOF;
    
     while (myReader.Next()) 
     {
      hitPos.clear(); 
      iEvent++;
      printf("<--------------------Event No. %d---------------------> \n",iEvent);      
      // Generated primary track
      //cout << "charge.GetSize(): " << charge.GetSize() << endl;
      if (charge.GetSize()>1) continue; // skip event for larger than 1 tracks
      Double_t eta_Track = 100.; // set it ouside from -3.5 to 3.5      
      Double_t pmc = 0.;
      for (int j = 0; j < charge.GetSize(); ++j){
      
      if (status[j] !=1) continue;
      pmc = sqrt(px_mc[j]*px_mc[j]+py_mc[j]*py_mc[j]+pz_mc[j]*pz_mc[j]);   
      Double_t pzmc = pz_mc[j];
      Double_t etamc = -1.0*TMath::Log(TMath::Tan((TMath::ACos(pzmc/pmc))/2));
      eta_Track = etamc;
      Double_t particle = pdg[j];
      }
      if (fabs(eta_Track)>4.0) continue; //outside tracker acceptance
      //if (fabs(eta_Track)>3.5) continue; //outside tracker acceptance
      // Associated hits with the primary track of momentum (mom)
      if (fabs(pmc-mom)> epsilon){
	     cout << "pmc: " << pmc << endl;  
	     cout << "mom: " << mom << endl;  
	     cout << "fabs(pmc-mom): " << fabs(pmc-mom) << ", epsilon: " << epsilon << endl;  
	     continue; 
      }
    //  if (eta_Track<3.4) continue; // Looks for the hits in a given eta range
      printf("Eta of the generated track: %f, Momentum (GeV/c): %f \n",eta_Track,pmc);
     // ePIC SVT IB Tracker
     for (int j = 0; j < vtx_si_x->GetSize(); ++j){
       Double_t xhit = vtx_si_x->At(j); Double_t yhit = vtx_si_y->At(j); Double_t zhit = vtx_si_z->At(j);
       Int_t quality = vtx_si_quality->At(j);
       if (quality==0) hitPos.push_back(TVector3(xhit,yhit,zhit));
     }
     nhits_SVTIB =  hitPos.size(); hitPos.clear();
     if (debug)  printf("SVT IB Associated hits: %d \n",nhits_SVTIB);
     
     // ePIC SVT OB Tracker
     for (int j = 0; j < barrel_si_x->GetSize(); ++j){
       Double_t xhit = barrel_si_x->At(j); Double_t yhit = barrel_si_y->At(j); Double_t zhit = barrel_si_z->At(j);
       Int_t quality = barrel_si_quality->At(j); 
       if (quality==0) hitPos.push_back(TVector3(xhit,yhit,zhit));
     }
     nhits_SVTOB =  hitPos.size(); hitPos.clear();
     if (debug)  printf("SVT OB Associated hits: %d \n",nhits_SVTOB);  
    
         
    // BTOF Tracker
    for (int j = 0; j < barrel_tof_x->GetSize(); ++j){
      Double_t xhit = barrel_tof_x->At(j); Double_t yhit = barrel_tof_y->At(j); Double_t zhit = barrel_tof_z->At(j);
      Int_t quality = barrel_tof_quality->At(j); 
      if (quality==0) hitPos.push_back(TVector3(xhit,yhit,zhit)); 
    }
    
     nhits_BTOF =  hitPos.size(); hitPos.clear();
     if (debug)  printf("BTOF Associated hits: %d \n",nhits_BTOF);
   
    // ePIC SVT Disks Hits
    for (int j = 0; j < disks_si_x->GetSize(); ++j){
      Int_t quality = disks_si_quality->At(j); 
      Double_t xhit = disks_si_x->At(j); Double_t yhit = disks_si_y->At(j); Double_t zhit = disks_si_z->At(j);
      if (quality==0 && zhit>0) hitPos.push_back(TVector3(xhit,yhit,zhit));
      else if (quality==0 && zhit<0) hitPos.push_back(TVector3(xhit,yhit,zhit));      
    }
    
     nhits_SVTDisks =  hitPos.size(); hitPos.clear();
     if (debug)  printf("SVT Disks Associated hits: %d \n",nhits_SVTDisks);
    
     // ETOF Tracker (Forward)
    for (int j = 0; j < endcap_etof_x->GetSize(); ++j){
    Double_t xhit = endcap_etof_x->At(j); Double_t yhit = endcap_etof_y->At(j); Double_t zhit = endcap_etof_z->At(j);
    Int_t quality = endcap_etof_quality->At(j);    
    if (quality==0) hitPos.push_back(TVector3(xhit,yhit,zhit));
    }  
    
     nhits_ETOF =  hitPos.size(); hitPos.clear();
     if (debug)  printf("ETOF Associated hits: %d \n",nhits_ETOF);
     
    int nhits = nhits_SVTIB + nhits_SVTOB + nhits_BTOF + nhits_SVTDisks + nhits_ETOF;
    if (nhits>0) hits->Fill(eta_Track,nhits); 
    printf("Total Associated hits: %d \n",nhits);

 }
 
  c1->cd();
  gPad->SetTicks(1,1);
  hits->SetLineWidth(2);
  hits->Draw("hist");
  hits->Write();
  c1->SaveAs(Form("./Output/Nhitsvsmom%1.1f_NoMPGD.png",mom));
  c1->SaveAs(Form("./Output/Nhitsvsmom%1.1f_NoMPGD.root",mom));
}


