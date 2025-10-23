//compare hist with and without MPGDs turned on
void CompareSimHistos(){
 TFile *f_MPGD_0   = TFile::Open("./Output/RecHits/Nhitsvsmom_0.5_hist.root");
 TFile *f_noMPGD_0 = TFile::Open("./Output/RecHits/Nhitsvsmom_0.5_noMPGD_hist.root");
 TFile *f_MPGD_1   = TFile::Open("./Output/RecHits/Nhitsvsmom_1.0_hist.root");
 TFile *f_noMPGD_1 = TFile::Open("./Output/RecHits/Nhitsvsmom_1.0_noMPGD_hist.root");
 TFile *f_MPGD_2   = TFile::Open("./Output/RecHits/Nhitsvsmom_2.0_hist.root");
 TFile *f_noMPGD_2 = TFile::Open("./Output/RecHits/Nhitsvsmom_2.0_noMPGD_hist.root");
 TFile *f_MPGD_3   = TFile::Open("./Output/RecHits/Nhitsvsmom_5.0_hist.root");
 TFile *f_noMPGD_3 = TFile::Open("./Output/RecHits/Nhitsvsmom_5.0_noMPGD_hist.root");
 TFile *f_MPGD_4   = TFile::Open("./Output/RecHits/Nhitsvsmom_10.0_hist.root");
 TFile *f_noMPGD_4 = TFile::Open("./Output/RecHits/Nhitsvsmom_10.0_noMPGD_hist.root");

 TH1F *h_MPGD_0 = (TH1F*)f_MPGD_0->Get("hits");
 TH1F *h_noMPGD_0 = (TH1F*)f_noMPGD_0->Get("hits");
 if(!h_MPGD_0 || !h_noMPGD_0){
	 std::cerr << "Error (0): Could not load both histograms\n";
	 return;
 }
 TH1F *h_MPGD_1 = (TH1F*)f_MPGD_1->Get("hits");
 TH1F *h_noMPGD_1 = (TH1F*)f_noMPGD_1->Get("hits");
 if(!h_MPGD_1 || !h_noMPGD_1){
	 std::cerr << "Error (1): Could not load both histograms\n";
	 return;
 }
 TH1F *h_MPGD_2 = (TH1F*)f_MPGD_2->Get("hits");
 TH1F *h_noMPGD_2 = (TH1F*)f_noMPGD_2->Get("hits");
 if(!h_MPGD_2 || !h_noMPGD_2){
	 std::cerr << "Error (2): Could not load both histograms\n";
	 return;
 }
 TH1F *h_MPGD_3 = (TH1F*)f_MPGD_3->Get("hits");
 TH1F *h_noMPGD_3 = (TH1F*)f_noMPGD_3->Get("hits");
 if(!h_MPGD_3 || !h_noMPGD_3){
	 std::cerr << "Error (3): Could not load both histograms\n";
	 return;
 }
 TH1F *h_MPGD_4 = (TH1F*)f_MPGD_4->Get("hits");
 TH1F *h_noMPGD_4 = (TH1F*)f_noMPGD_4->Get("hits");
 if(!h_MPGD_4 || !h_noMPGD_4){
	 std::cerr << "Error (4): Could not load both histograms\n";
	 return;
 }

 TLegend *leg = new TLegend(0.0,0.4,0.6,0.6);
 leg->SetFillStyle(0);
 leg->SetBorderSize(0);
 leg->SetLineColor(10);
 leg->SetFillColor(10);
 leg->SetTextSize(0.09);
 leg->AddEntry(h_MPGD_0,"with MPGDs","l");
 leg->AddEntry(h_noMPGD_0,"without MPGDs","l");

 gStyle->SetOptStat(0);
 TCanvas *c1 = new TCanvas();
 c1->Divide(3,2);
 c1->SetGrid();
 c1->cd(1);
   c1->SetGrid(1);
   h_MPGD_0->SetLineColor(kBlue);
   h_MPGD_0->GetYaxis()->SetTitle("<Sim Nhits>");
   h_noMPGD_0->GetYaxis()->SetTitle("<Sim Nhits>");
   h_MPGD_0->Draw("hist");   
   h_noMPGD_0->SetLineColor(kRed);
   h_noMPGD_0->SetLineStyle(2);
   h_noMPGD_0->Draw("hist same");   
 c1->cd(2);
   c1->SetGrid(1);
   h_MPGD_1->SetLineColor(kBlue);
   h_MPGD_1->GetYaxis()->SetTitle("<Sim Nhits>");
   h_noMPGD_1->GetYaxis()->SetTitle("<Sim Nhits>");
   h_MPGD_1->Draw("hist");   
   h_noMPGD_1->SetLineColor(kRed);
   h_noMPGD_1->SetLineStyle(2);
   h_noMPGD_1->Draw("hist same");   
 c1->cd(3);
   c1->SetGrid(1);
   h_MPGD_2->SetLineColor(kBlue);
   h_MPGD_2->GetYaxis()->SetTitle("<Sim Nhits>");
   h_noMPGD_2->GetYaxis()->SetTitle("<Sim Nhits>");
   h_MPGD_2->Draw("hist");   
   h_noMPGD_2->SetLineColor(kRed);
   h_noMPGD_2->SetLineStyle(2);
   h_noMPGD_2->Draw("hist same");   
 c1->cd(4);
   c1->SetGrid(1);
   h_MPGD_3->SetLineColor(kBlue);
   h_MPGD_3->GetYaxis()->SetTitle("<Sim Nhits>");
   h_noMPGD_3->GetYaxis()->SetTitle("<Sim Nhits>");
   h_MPGD_3->Draw("hist");   
   h_noMPGD_3->SetLineColor(kRed);
   h_noMPGD_3->SetLineStyle(2);
   h_noMPGD_3->Draw("hist same");   
 c1->cd(5);
   c1->SetGrid();
   c1->SetGrid();
   h_MPGD_4->SetLineColor(kBlue);
   h_MPGD_4->GetYaxis()->SetTitle("<Sim Nhits>");
   h_noMPGD_4->GetYaxis()->SetTitle("<Sim Nhits>");
   h_MPGD_4->Draw("hist");   
   h_noMPGD_4->SetLineColor(kRed);
   h_noMPGD_4->SetLineStyle(2);
   h_noMPGD_4->Draw("hist same");   
   c1->Update();
 c1->cd(6);
   leg->Draw();
 c1->Update();
}
