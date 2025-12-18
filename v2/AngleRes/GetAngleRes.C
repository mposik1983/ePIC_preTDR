//Create and fit angular resolutions at the pid surface
// angle_res is width of proj_trk_angl - pid_hit_angle from gauss fit
#include <fstream>
#include <vector>

TString Type = "tracking_only_10umBOT";
TString AnaType = "MomVector";

TString InDir  = "/Users/tua88437/Dropbox/EIC/EPIC/Simulation/PID_AngularResolutions/Craterlake/25.07.0/rootfiles/DIRCScan";
TString OutDir = "/Users/tua88437/Dropbox/EIC/EPIC/Simulation/PID_AngularResolutions/Craterlake/25.07.0/results/DIRCScan";

double Deg2Rad(double iangle){return TMath::Pi()/180.0 * iangle;}
  double GetEta(double theta1, double theta2){
  double eta1 = -1*TMath::Log(TMath::Tan(Deg2Rad(theta1)/2.0));
  double eta2 = -1*TMath::Log(TMath::Tan(Deg2Rad(theta2)/2.0));
  return 0.5*(eta1+eta2);
}

void GetAngleRes()
{
  gStyle->SetOptFit(1111);
  bool useCUT = true;
  double nsigma = 3.0;
//Variables and strings
  TString epic_ver = "25.07.0";
  TString eicrecon_ver = "v1.27.1";
  TString part = "pi-";
  TString det = "DIRC";
  //double ProjR = 755.0; //[mm] radial (or z) location trajectory is propagated to
  double ProjR = 770.5; //[mm] radial (or z) location trajectory is propagated to
  const int pBins = 7;
  //const int thetaBins = 3;
  double dtheta = 2; //2 degree bins
  //double pBin[pBins] = {6.0};
  double pBin[pBins] = {1.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0};
  //double thetaBin[thetaBins] = {52, 92, 140};

//loop to setup theta array
  double theta_start = 16.0;
  double theta_end = 164.0;
  double my_theta = theta_start;
  vector<double> thetaV;
  while(my_theta <= theta_end){
      thetaV.push_back(my_theta);
      my_theta += dtheta;
  }   
  cout << "thetaV.size():" << thetaV.size() << endl;
  const int thetaBins = thetaV.size();
  double thetaBin[thetaBins];
  for(int i=0; i<thetaV.size();i++){
    thetaBin[i] = thetaV[i];
    cout << Form("thetaBin2[%d] = %.0f \n",i,thetaBin[i]);
  }   
//end theta loop array
  
  double hetaBin[2] = {-4.0,4.0};

  double myEta[thetaBins][pBins];
  double myMom[thetaBins][pBins];
  double myZero[thetaBins][pBins];

  double myTruthMom[thetaBins][pBins];
  double myTruthEta[thetaBins][pBins];
  double myTruthThetaRes[thetaBins][pBins];
  double myTruthThetaResErr[thetaBins][pBins];
  double myTruthPhiRes[thetaBins][pBins];
  double myTruthPhiResErr[thetaBins][pBins];
  double myTruthZRes[thetaBins][pBins];
  double myTruthZResErr[thetaBins][pBins];

  double mySeedMom[thetaBins][pBins];
  double mySeedEta[thetaBins][pBins];
  double mySeedThetaRes[thetaBins][pBins];
  double mySeedThetaResErr[thetaBins][pBins];
  double mySeedPhiRes[thetaBins][pBins];
  double mySeedPhiResErr[thetaBins][pBins];
  double mySeedZRes[thetaBins][pBins];
  double mySeedZResErr[thetaBins][pBins];

//histograms
//--Truth Seeded--
  //resolutions
  TH1F* h_dth[thetaBins][pBins];
  TH1F* h_dphi[thetaBins][pBins];
  TH1F* h_dz[thetaBins][pBins];
  //Kinematics
  TH1F* h_Truep[thetaBins][pBins]; //True = hit, Rec = projected
  TH1F* h_Recp[thetaBins][pBins];
  TH1F* h_TrueEta[thetaBins][pBins];
  TH1F* h_RecEta[thetaBins][pBins];

//Fits
  TF1 *f_dth[thetaBins][pBins];
  TF1 *f_dphi[thetaBins][pBins];
  TF1 *f_dz[thetaBins][pBins];

//--Real Seeded--
//histograms
  //pulls
  TH1F* h_dth_seed_pull[thetaBins][pBins];
  TH1F* h_dphi_seed_pull[thetaBins][pBins];
  //resolutions
  TH1F* h_dth_seed[thetaBins][pBins];
  TH1F* h_dphi_seed[thetaBins][pBins];
  TH1F* h_dz_seed[thetaBins][pBins];
  //Kinematics
  TH1F* h_Truep_seed[thetaBins][pBins];
  TH1F* h_Recp_seed[thetaBins][pBins];
  TH1F* h_TrueEta_seed[thetaBins][pBins];
  TH1F* h_RecEta_seed[thetaBins][pBins];

//Fits
  TF1 *f_dth_seed[thetaBins][pBins];
  TF1 *f_dphi_seed[thetaBins][pBins];
  TF1 *f_dz_seed[thetaBins][pBins];
  TF1 *f_dth_seed_pull[thetaBins][pBins];
  TF1 *f_dphi_seed_pull[thetaBins][pBins];

//cuts
  TString cut, cut_seed;
  if(useCUT)
    {
      cout << "using CUT\n";
      cut = "";//Form("((%sres.dR>0) && (%sres.dR < 100))",det.Data(),det.Data()); 
      cut_seed = "DIRCSeedres.nmeas >= 7";//Form("((%sSeedres.dR>0) && (%sSeedres.dR < 100))",det.Data(),det.Data());
    }else{
      cout << "using no CUT\n";
      cut = "";//Form("((%sres.dR>0) && (%sres.dR < 100))",det.Data(),det.Data()); 
      cut_seed = "";//Form("((%sSeedres.dR>0) && (%sSeedres.dR < 100))",det.Data(),det.Data());
    }
  
//Output Directory:
  TString outFilePath = Form("%s/%s/dat",OutDir.Data(),Type.Data());

  fstream Outfile_dth[thetaBins];
   TString ofile_dth = "";

  fstream Outfile_dphi[thetaBins];
   TString ofile_dphi = "";

  fstream Outfile_dz[thetaBins];
   TString ofile_dz = "";

  fstream Outfile_dth_seed[thetaBins];
   TString ofile_dth_seed = "";

  fstream Outfile_dphi_seed[thetaBins];
   TString ofile_dphi_seed = "";

  fstream Outfile_dz_seed[thetaBins];
   TString ofile_dz_seed = "";

  fstream Outfile_dth_seed_pull[thetaBins];
   TString ofile_dth_seed_pull = "";

  fstream Outfile_dphi_seed_pull[thetaBins];
   TString ofile_dphi_seed_pull = "";

  fstream Outfile_dth_p[pBins];
   TString ofile_dth_p = "";

  fstream Outfile_dphi_p[pBins];
   TString ofile_dphi_p = "";

  fstream Outfile_dz_p[pBins];
   TString ofile_dz_p = "";

  fstream Outfile_dth_p_seed[pBins];
   TString ofile_dth_p_seed = "";

  fstream Outfile_dphi_p_seed[pBins];
   TString ofile_dphi_p_seed = "";

  fstream Outfile_dth_p_seed_pull[pBins];
   TString ofile_dth_p_seed_pull = "";

  fstream Outfile_dphi_p_seed_pull[pBins];
   TString ofile_dphi_p_seed_pull = "";

  fstream Outfile_dz_p_seed[pBins];
   TString ofile_dz_p_seed = "";

  TFile *f = new TFile(Form("%s/%s/rootfiles/%s_AngleRes_%s_%s_%s_%s.root",OutDir.Data(),Type.Data(),det.Data(),AnaType.Data(),Type.Data(),epic_ver.Data(),eicrecon_ver.Data()),"recreate");
  f->cd();
  gDirectory->mkdir("dtheta");
  gDirectory->mkdir("dphi");
  gDirectory->mkdir("dz");

//Inputs
  TString inFile;
  for(auto i=0; i < thetaBins; i++){ //loop over eta bins
    double my_Avgeta = GetEta(thetaBin[i],thetaBin[i]+dtheta);
   
    ofile_dth  = Form("%s/ThetaRes/Truth/EtaBin/%s/ThetaRes_Truth_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dth[i].open(ofile_dth,ios::out);
    ofile_dphi = Form("%s/PhiRes/Truth/EtaBin/%s/PhiRes_Truth_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dphi[i].open(ofile_dphi,ios::out);
    ofile_dz   = Form("%s/ZRes/Truth/EtaBin/%s/ZRes_Truth_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dz[i].open(ofile_dz,ios::out);

    ofile_dth_seed  = Form("%s/ThetaRes/Seed/EtaBin/%s/ThetaRes_Seed_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dth_seed[i].open(ofile_dth_seed,ios::out);
    ofile_dphi_seed = Form("%s/PhiRes/Seed/EtaBin/%s/PhiRes_Seed_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dphi_seed[i].open(ofile_dphi_seed,ios::out);
    ofile_dz_seed   = Form("%s/ZRes/Seed/EtaBin/%s/ZRes_Seed_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dz_seed[i].open(ofile_dz_seed,ios::out);

    ofile_dth_seed_pull  = Form("%s/ThetaRes/Seed/EtaBin/%s/ThetaRes_Seed_Pull_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dth_seed_pull[i].open(ofile_dth_seed_pull,ios::out);
    ofile_dphi_seed_pull = Form("%s/PhiRes/Seed/EtaBin/%s/PhiRes_Seed_Pull_%.0fdeg_%.0fdeg_EtaBin_%.2f.dat",outFilePath.Data(),AnaType.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta);
      Outfile_dphi_seed_pull[i].open(ofile_dphi_seed_pull,ios::out);

    for(auto j=0; j < pBins; j++){//loop over p bins
      ofile_dth_p  = Form("%s/ThetaRes/Truth/MomBin/%s/ThetaRes_Truth_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dth_p[j].open(ofile_dth_p,ios::out | ios::app);
      ofile_dphi_p = Form("%s/PhiRes/Truth/MomBin/%s/PhiRes_Truth_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dphi_p[j].open(ofile_dphi_p,ios::out | ios::app);
      ofile_dz_p   = Form("%s/ZRes/Truth/MomBin/%s/ZRes_Truth_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dz_p[j].open(ofile_dz_p,ios::out | ios::app);

      ofile_dth_p_seed  = Form("%s/ThetaRes/Seed/MomBin/%s/ThetaRes_Seed_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dth_p_seed[j].open(ofile_dth_p_seed,ios::out | ios::app);
      ofile_dphi_p_seed = Form("%s/PhiRes/Seed/MomBin/%s/PhiRes_Seed_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dphi_p_seed[j].open(ofile_dphi_p_seed,ios::out | ios::app );
      ofile_dz_p_seed   = Form("%s/ZRes/Seed/MomBin/%s/ZRes_Seed_MomBin_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dz_p_seed[j].open(ofile_dz_p_seed,ios::out | ios::app);
      
      ofile_dth_p_seed_pull  = Form("%s/ThetaRes/Seed/MomBin/%s/ThetaRes_Seed_MomBin_Pull_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dth_p_seed_pull[j].open(ofile_dth_p_seed_pull,ios::out | ios::app);
      ofile_dphi_p_seed_pull = Form("%s/PhiRes/Seed/MomBin/%s/PhiRes_Seed_MomBin_Pull_%.2fGeV.dat",outFilePath.Data(),AnaType.Data(),pBin[j]);
        Outfile_dphi_p_seed_pull[j].open(ofile_dphi_p_seed_pull,ios::out | ios::app );

      inFile = Form("%s/%s/eicrecon_%s_%s_%s_%.1fGeV_%.0fdeg_%.0fdeg_0.0deg_360.0deg_%s_1.ana.root",InDir.Data(),Type.Data(),eicrecon_ver.Data(),epic_ver.Data(),part.Data(),pBin[j],thetaBin[i],thetaBin[i]+dtheta,Type.Data());
      cout << Form("inFile: %s \n",inFile.Data());
      
      //chain root files together
      TChain mychain("TTruth");
      mychain.Add(inFile);
      TTreeReader tree_reader_mychain(&mychain);
       
      TChain mychain_seed("TSeed");
      mychain_seed.Add(inFile);
      TTreeReader tree_reader_mychain_seed(&mychain_seed);

      cout << "mychain entries: " << mychain.GetEntries() << endl;
      cout << "mychain_seed entries: " << mychain_seed.GetEntries() << endl;
      //define histos
      h_TrueEta[i][j] = new TH1F(Form("h_TrueEta_th%02d_p%02d",i,j),
                                 Form("Hit: #eta, %.1f < #theta [deg] < %.1f",thetaBin[i],thetaBin[i]+dtheta),
                                 200,hetaBin[0],hetaBin[1]);
      h_RecEta[i][j] = new TH1F(Form("h_RecEta_th%02d_p%02d",i,j),
                                 Form("Proj. to %s: #eta, %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 200,hetaBin[0],hetaBin[1]);
      h_Truep[i][j]    = new TH1F(Form("h_Truep_th%02d_p%02d",i,j),
                                 Form("%s Hit: p [GeV], %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 250,0,25);
      h_Recp[i][j]     = new TH1F(Form("h_Recp_th%02d_p%02d",i,j),
                                 Form("Proj. to %s: p [GeV], %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 250,0,25);
      h_dth[i][j]        = new TH1F(Form("h_dth_th%02d_p%02d",i,j),
                                 Form("%s: dth [mrad], %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      h_dphi[i][j]       = new TH1F(Form("h_dphi_th%02d_p%02d",i,j),
                                 Form("%s: dphi [mrad], %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      h_dz[i][j]         = new TH1F(Form("h_dz_th%02d_p%02d",i,j),
                                 Form("%s: dz [mm], %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      //Fill histos
      mychain.Draw(Form("%sTruthhit.eta>>h_TrueEta_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("%sTruthproj.eta>>h_RecEta_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("%sTruthhit.p>>h_Truep_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("%sTruthproj.p>>h_Recp_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("1000.0*%sTruthres.dtheta>>h_dth_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("1000.0*%sTruthres.dphi>>h_dphi_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");
      mychain.Draw(Form("%sTruthres.dz>>h_dz_th%02d_p%02d",det.Data(),i,j),Form("%s",cut.Data()),"goff");

     //Make Fits
     f_dth[i][j] = new TF1(Form("f_dth_th%02d_p%02d",i,j),"gaus");
       f_dth[i][j]->SetLineColor(kRed);
       f_dth[i][j]->SetParameter(0,h_dth[i][j]->GetEntries());
       f_dth[i][j]->SetParameter(1,h_dth[i][j]->GetMean());
       f_dth[i][j]->SetParameter(2,h_dth[i][j]->GetRMS());
       f_dth[i][j]->SetRange(h_dth[i][j]->GetMean() - nsigma*h_dth[i][j]->GetRMS(),
                            h_dth[i][j]->GetMean() + nsigma*h_dth[i][j]->GetRMS());
     h_dth[i][j]->Fit(Form("f_dth_th%02d_p%02d",i,j),"MEqr");
 
     f_dphi[i][j] = new TF1(Form("f_dphi_th%02d_p%02d",i,j),"gaus");
       f_dphi[i][j]->SetLineColor(kRed);
       f_dphi[i][j]->SetParameter(0,h_dphi[i][j]->GetEntries());
       f_dphi[i][j]->SetParameter(1,h_dphi[i][j]->GetMean());
       f_dphi[i][j]->SetParameter(2,h_dphi[i][j]->GetRMS());
       f_dphi[i][j]->SetRange(h_dphi[i][j]->GetMean() - nsigma*h_dphi[i][j]->GetRMS(),
                             h_dphi[i][j]->GetMean() +  nsigma*h_dphi[i][j]->GetRMS());
     h_dphi[i][j]->Fit(Form("f_dphi_th%02d_p%02d",i,j),"MEqr");

     f_dz[i][j] = new TF1(Form("f_dz_th%02d_p%02d",i,j),"gaus");
       f_dz[i][j]->SetLineColor(kRed);
       f_dz[i][j]->SetParameter(0,h_dz[i][j]->GetEntries());
       f_dz[i][j]->SetParameter(1,h_dz[i][j]->GetMean());
       f_dz[i][j]->SetParameter(2,h_dz[i][j]->GetRMS());
       f_dz[i][j]->SetRange(h_dz[i][j]->GetMean() - nsigma*h_dz[i][j]->GetRMS(),
                            h_dz[i][j]->GetMean() + nsigma*h_dz[i][j]->GetRMS());
     h_dz[i][j]->Fit(Form("f_dz_th%02d_p%02d",i,j),"MEqr");

     f->cd("dtheta"); h_dth[i][j]->Write();
     f->cd("dphi");   h_dphi[i][j]->Write();
     f->cd("dz");     h_dz[i][j]->Write();

     double true_eta = h_TrueEta[i][j]->GetMean();
     double true_p = h_Truep[i][j]->GetMean();
     double rec_eta = h_RecEta[i][j]->GetMean();
     double rec_p = h_Recp[i][j]->GetMean();
     double dth_res = f_dth[i][j]->GetParameter(2);
     double dth_res_er = f_dth[i][j]->GetParError(2);

     double dphi_res = f_dphi[i][j]->GetParameter(2);
     double dphi_res_er = f_dphi[i][j]->GetParError(2);

     double dz_res = f_dz[i][j]->GetParameter(2);
     double dz_res_er = f_dz[i][j]->GetParError(2);

     cout << Form("eta: %.2f, p: %.2f GeV,  dth: %.2f mrad  +/- %.2f mrad \n",true_eta,true_p,dth_res,dth_res_er);
     cout << Form("                         dphi: %.2f mrad +/- %.2f mrad \n",dphi_res,dphi_res_er);
     
     cout << Form("%.2f %.2f %.2f %.2f  \n",rec_p,dth_res,0.00,dth_res_er);
     cout << Form("%.2f %.2f %.2f %.2f  \n",rec_p,dphi_res,0.00,dphi_res_er);

     cout << Form("%.2f %.4f %.2f %.4f  \n",rec_eta,dth_res,0.00,dth_res_er);
     cout << Form("%.2f %.4f %.2f %.4f  \n",rec_eta,dphi_res,0.00,dphi_res_er);
     cout << Form("%.2f %.4f %.2f %.4f  \n",rec_eta,dz_res,0.00,dz_res_er);

     Outfile_dth[i]   << Form("%.2f %.5f %.2f %.5f\n",rec_p,dth_res,0.00,dth_res_er);
     Outfile_dphi[i]  << Form("%.2f %.5f %.2f %.5f\n",rec_p,dphi_res,0.00,dphi_res_er);
     Outfile_dz[i]    << Form("%.2f %.5f %.2f %.5f\n",rec_p,dz_res,0.00,dz_res_er);

     Outfile_dth_p[j]   << Form("%.2f %.5f %.2f %.5f\n",rec_eta,dth_res,0.00,dth_res_er);
     Outfile_dphi_p[j]  << Form("%.2f %.5f %.2f %.5f\n",rec_eta,dphi_res,0.00,dphi_res_er);
     Outfile_dz_p[j]    << Form("%.2f %.5f %.2f %.5f\n",rec_eta,dz_res,0.00,dz_res_er);

//Do seeded
      h_TrueEta_seed[i][j]    = new TH1F(Form("h_TrueEta_seed_th%02d_p%02d",i,j),
                                 Form("%s Hit: #eta, %.1f < #theta [deg]< %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 200,hetaBin[0],hetaBin[1]);
      h_RecEta_seed[i][j]     = new TH1F(Form("h_RecEta_seed_th%02d_p%02d",i,j),
                                 Form("Proj. to %s: #eta (Seeded), %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 200,hetaBin[0],hetaBin[1]);
      h_Truep_seed[i][j]      = new TH1F(Form("h_Truep_seed_th%02d_p%02d",i,j),
                                 Form("%s Hit: p [GeV], %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 250,0,25);
      h_Recp_seed[i][j]       = new TH1F(Form("h_Recp_seed_th%02d_p%02d",i,j),
                                 Form("Proj. to %s: p (Seeded) [GeV], %.1f < #theta [deg] < %.1f",det.Data(),thetaBin[i],thetaBin[i]+dtheta),
                                 250,0,25);
      h_dth_seed_pull[i][j]   = new TH1F(Form("h_dth_seed_pull_th%02d_p%02d",i,j),
                                 Form("%s: dth Pull (Seeded), %.1f < #theta [deg] < %.1f, <#eta> =  %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-10,10);
      h_dphi_seed_pull[i][j]  = new TH1F(Form("h_dphi_seed_pull_th%02d_p%02d",i,j),
                                 Form("%s: dphi Pull (Seeded), %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-10,10);
      h_dth_seed[i][j]        = new TH1F(Form("h_dth_seed_th%02d_p%02d",i,j),
                                 Form("%s: dth (Seeded) [mrad], %.1f < #theta [deg] < %.1f, <#eta> =  %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      h_dphi_seed[i][j]       = new TH1F(Form("h_dphi_seed_th%02d_p%02d",i,j),
                                 Form("%s: dphi (Seeded) [mrad], %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      h_dz_seed[i][j]         = new TH1F(Form("h_dz_seed_th%02d_p%02d",i,j),
                                 Form("%s: dz (Seeded) [mm], %.1f < #theta [deg] < %.1f, <#eta> = %.2f, p = %.2f GeV",det.Data(),thetaBin[i],thetaBin[i]+dtheta,my_Avgeta,pBin[j]),
                                 1000,-100,100);
      //Fill histos
      mychain_seed.Draw(Form("%sSeedhit.eta>>h_TrueEta_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedproj.eta>>h_RecEta_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedhit.p>>h_Truep_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedproj.p>>h_Recp_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedres.dtheta/TMath::Sqrt(%sSeedpoint.err_theta)>>h_dth_seed_pull_th%02d_p%02d",det.Data(),det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedres.dphi/TMath::Sqrt(%sSeedpoint.err_phi)>>h_dphi_seed_pull_th%02d_p%02d",det.Data(),det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("1000.0*%sSeedres.dtheta>>h_dth_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("1000.0*%sSeedres.dphi>>h_dphi_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");
      mychain_seed.Draw(Form("%sSeedres.dz>>h_dz_seed_th%02d_p%02d",det.Data(),i,j),Form("%s",cut_seed.Data()),"goff");

     //Make Fits
     //Pulls
     f_dth_seed_pull[i][j] = new TF1(Form("f_dth_seed_pull_th%02d_p%02d",i,j),"gaus");
       f_dth_seed_pull[i][j]->SetLineColor(kRed);
       f_dth_seed_pull[i][j]->SetParameter(0,h_dth_seed_pull[i][j]->GetEntries());
       f_dth_seed_pull[i][j]->SetParameter(1,h_dth_seed_pull[i][j]->GetMean());
       f_dth_seed_pull[i][j]->SetParameter(2,h_dth_seed_pull[i][j]->GetRMS());
       f_dth_seed_pull[i][j]->SetRange(h_dth_seed_pull[i][j]->GetMean() - nsigma*h_dth_seed_pull[i][j]->GetRMS(),
                            h_dth_seed_pull[i][j]->GetMean() + nsigma*h_dth_seed_pull[i][j]->GetRMS());
     h_dth_seed_pull[i][j]->Fit(Form("f_dth_seed_pull_th%02d_p%02d",i,j),"MEqr");
 
     f_dphi_seed_pull[i][j] = new TF1(Form("f_dphi_seed_pull_th%02d_p%02d",i,j),"gaus");
       f_dphi_seed_pull[i][j]->SetLineColor(kRed);
       f_dphi_seed_pull[i][j]->SetParameter(0,h_dphi_seed_pull[i][j]->GetEntries());
       f_dphi_seed_pull[i][j]->SetParameter(1,h_dphi_seed_pull[i][j]->GetMean());
       f_dphi_seed_pull[i][j]->SetParameter(2,h_dphi_seed_pull[i][j]->GetRMS());
       f_dphi_seed_pull[i][j]->SetRange(h_dphi_seed_pull[i][j]->GetMean() - nsigma*h_dphi_seed_pull[i][j]->GetRMS(),
                             h_dphi_seed_pull[i][j]->GetMean() + nsigma*h_dphi_seed_pull[i][j]->GetRMS());
     h_dphi_seed_pull[i][j]->Fit(Form("f_dphi_seed_pull_th%02d_p%02d",i,j),"MEqr");

     //Resolutions
     f_dth_seed[i][j] = new TF1(Form("f_dth_seed_th%02d_p%02d",i,j),"gaus");
       f_dth_seed[i][j]->SetLineColor(kRed);
       f_dth_seed[i][j]->SetParameter(0,h_dth_seed[i][j]->GetEntries());
       f_dth_seed[i][j]->SetParameter(1,h_dth_seed[i][j]->GetMean());
       f_dth_seed[i][j]->SetParameter(2,h_dth_seed[i][j]->GetRMS());
       f_dth_seed[i][j]->SetRange(h_dth_seed[i][j]->GetMean() - nsigma*h_dth_seed[i][j]->GetRMS(),
                            h_dth_seed[i][j]->GetMean() + nsigma*h_dth_seed[i][j]->GetRMS());
     h_dth_seed[i][j]->Fit(Form("f_dth_seed_th%02d_p%02d",i,j),"MEqr");
 
     f_dphi_seed[i][j] = new TF1(Form("f_dphi_seed_th%02d_p%02d",i,j),"gaus");
       f_dphi_seed[i][j]->SetLineColor(kRed);
       f_dphi_seed[i][j]->SetParameter(0,h_dphi_seed[i][j]->GetEntries());
       f_dphi_seed[i][j]->SetParameter(1,h_dphi_seed[i][j]->GetMean());
       f_dphi_seed[i][j]->SetParameter(2,h_dphi_seed[i][j]->GetRMS());
       f_dphi_seed[i][j]->SetRange(h_dphi_seed[i][j]->GetMean() - nsigma*h_dphi_seed[i][j]->GetRMS(),
                             h_dphi_seed[i][j]->GetMean() + nsigma*h_dphi_seed[i][j]->GetRMS());
     h_dphi_seed[i][j]->Fit(Form("f_dphi_seed_th%02d_p%02d",i,j),"MEqr");

     f_dz_seed[i][j] = new TF1(Form("f_dz_seed_th%02d_p%02d",i,j),"gaus");
       f_dz_seed[i][j]->SetLineColor(kRed);
       f_dz_seed[i][j]->SetParameter(0,h_dz_seed[i][j]->GetEntries());
       f_dz_seed[i][j]->SetParameter(1,h_dz_seed[i][j]->GetMean());
       f_dz_seed[i][j]->SetParameter(2,h_dz_seed[i][j]->GetRMS());
       f_dz_seed[i][j]->SetRange(h_dz_seed[i][j]->GetMean() - nsigma*h_dz_seed[i][j]->GetRMS(),
                                 h_dz_seed[i][j]->GetMean() + nsigma*h_dz_seed[i][j]->GetRMS());
     h_dz_seed[i][j]->Fit(Form("f_dz_seed_th%02d_p%02d",i,j),"MEqr");

     f->cd("dtheta"); 
       h_dth_seed[i][j]->Write(); 
       h_dth_seed_pull[i][j]->Write(); 
     f->cd("dphi");   
       h_dphi_seed[i][j]->Write();
       h_dphi_seed_pull[i][j]->Write();
     f->cd("dz");     
       h_dz_seed[i][j]->Write();

     //pulls
     double dth_pull_seed_sigma   = f_dth_seed_pull[i][j]->GetParameter(2);
     double dth_pull_seed_sigma_er   = f_dth_seed_pull[i][j]->GetParError(2);
     double dth_pull_seed_mean    = f_dth_seed_pull[i][j]->GetParameter(1);
     double dth_pull_seed_mean_er    = f_dth_seed_pull[i][j]->GetParError(1);
     double dphi_pull_seed_sigma  = f_dphi_seed_pull[i][j]->GetParameter(2);
     double dphi_pull_seed_sigma_er  = f_dphi_seed_pull[i][j]->GetParError(2);
     double dphi_pull_seed_mean   = f_dphi_seed_pull[i][j]->GetParameter(1);
     double dphi_pull_seed_mean_er   = f_dphi_seed_pull[i][j]->GetParError(1);

     double true_eta_seed = h_TrueEta_seed[i][j]->GetMean();
     double true_p_seed = h_Truep_seed[i][j]->GetMean();
     double rec_eta_seed = h_RecEta_seed[i][j]->GetMean();
     double rec_p_seed = h_Recp_seed[i][j]->GetMean();
     double dth_res_seed = f_dth_seed[i][j]->GetParameter(2);
     double dth_res_er_seed = f_dth_seed[i][j]->GetParError(2);

     double dphi_res_seed = f_dphi_seed[i][j]->GetParameter(2);
     double dphi_res_er_seed = f_dphi_seed[i][j]->GetParError(2);

     double dz_res_seed    = f_dz_seed[i][j]->GetParameter(2);
     double dz_res_er_seed = f_dz_seed[i][j]->GetParError(2);

     cout << "--- Seeded Resutls ---\n";
     cout << Form("eta: %.2f, p: %.2f GeV,  dth: %.2f mrad  +/- %.2f mrad \n",true_eta_seed,true_p_seed,dth_res_seed,dth_res_er_seed);
     cout << Form("                         dphi: %.2f mrad +/- %.2f mrad \n",dphi_res_seed,dphi_res_er_seed);
     
     cout << Form("%.2f %.2f %.2f %.2f  \n",rec_p_seed,dth_res_seed,0.00,dth_res_er_seed);
     cout << Form("%.2f %.2f %.2f %.2f  \n",rec_p_seed,dphi_res_seed,0.00,dphi_res_er_seed);

     Outfile_dth_seed[i]    << Form("%.2f %.5f %.2f %.5f\n",rec_p_seed,dth_res_seed,0.00,dth_res_er_seed);
     Outfile_dphi_seed[i]   << Form("%.2f %.5f %.2f %.5f\n",rec_p_seed,dphi_res_seed,0.00,dphi_res_er_seed);
     Outfile_dz_seed[i]     << Form("%.2f %.5f %.2f %.5f\n",rec_p_seed,dz_res_seed,0.00,dz_res_er_seed);

     Outfile_dth_seed_pull[i]    << Form("%.2f %.4f %.4f %.4f %.4f\n",rec_p_seed,dth_pull_seed_mean,dth_pull_seed_sigma,dth_pull_seed_mean_er,dth_pull_seed_sigma_er);
     Outfile_dphi_seed_pull[i]   << Form("%.2f %.4f %.4f %.4f %.4f\n",rec_p_seed,dphi_pull_seed_mean,dphi_pull_seed_sigma,dphi_pull_seed_mean_er,dphi_pull_seed_sigma_er);

     cout << Form("%.2f %.2f %.2f %.2f \n",rec_eta_seed,dth_res_seed,0.00,dth_res_er_seed);
     cout << Form("%.2f %.2f %.2f %.2f \n",rec_eta_seed,dphi_res_seed,0.00,dphi_res_er_seed);
     cout << Form("%.2f %.4f %.2f %.4f \n",rec_eta_seed,dz_res_seed,0.00,dz_res_er_seed);
     
     Outfile_dth_p_seed[j]    << Form("%.2f %.5f %.2f %.5f\n",rec_eta_seed,dth_res_seed,0.00,dth_res_er_seed);
     Outfile_dphi_p_seed[j]    << Form("%.2f %.5f %.2f %.5f\n",rec_eta_seed,dphi_res_seed,0.00,dphi_res_er_seed);
     Outfile_dz_p_seed[j]     << Form("%.2f %.5f %.2f %.5f\n",rec_eta_seed,dz_res_seed,0.00,dz_res_er_seed);

     Outfile_dth_p_seed_pull[j]    << Form("%.2f %.4f %.4f %.4f %.4f\n",rec_eta,dth_pull_seed_mean,dth_pull_seed_sigma,dth_pull_seed_mean_er,dth_pull_seed_sigma_er);
     Outfile_dphi_p_seed_pull[j]   << Form("%.2f %.4f %.4f %.4f %.4f\n",rec_eta,dphi_pull_seed_mean,dphi_pull_seed_sigma,dphi_pull_seed_mean_er,dphi_pull_seed_sigma_er);

     Outfile_dth_p[j].close();
     Outfile_dphi_p[j].close();
     Outfile_dz_p[j].close();
     Outfile_dth_p_seed[j].close();
     Outfile_dphi_p_seed[j].close();
     Outfile_dz_p_seed[j].close();
     Outfile_dth_p_seed_pull[j].close();
     Outfile_dphi_p_seed_pull[j].close();

    }
  Outfile_dth[i].close();
  Outfile_dphi[i].close();
  Outfile_dz[i].close();
  Outfile_dth_seed[i].close();
  Outfile_dphi_seed[i].close();
  Outfile_dz_seed[i].close();
  Outfile_dth_seed_pull[i].close();
  Outfile_dphi_seed_pull[i].close();
  }

}//end program loop
