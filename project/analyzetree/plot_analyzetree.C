using namespace std;

void plot_analyzetree(const char* filename, const char* figdir)
{
  TFile *f = new TFile(filename);
  gStyle->SetPadLeftMargin(0.15);

  // PT DIST
  TCanvas *c1 = new TCanvas("c1", "", 2200, 1400);
  c1->SetGrid();

  TH1 *ptdist = (TH1F*)f->Get("ptdist");
  ptdist->SetTitle("");
  ptdist->SetFillColor(kBlue);
  ptdist->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  ptdist->GetYaxis()->SetTitle("N(p_{T}) [c/GeV]");
  ptdist->Draw("bar2");
  c1->Print(Form("%s/ptdist.png",figdir));

  // PHI DIST
  TCanvas *c2 = new TCanvas("c2", "", 2200, 1400);
  c2->SetGrid();

  TH1 *phidist = (TH1F*)f->Get("phidist");
  phidist->SetTitle("");
  phidist->SetFillColor(kBlue);
  phidist->GetXaxis()->SetTitle("phi [rad]");
  phidist->GetYaxis()->SetTitle("N(phi)");
  phidist->Draw("bar2");
  c2->Print(Form("%s/phidist.png",figdir));

  // TRACK NUMBER DIST
  TCanvas *c3 = new TCanvas("c3", "", 2200, 1400);
  c3->SetGrid();

  TH1 *ntrackdist = (TH1F*)f->Get("ntrackdist");
  ntrackdist->SetTitle("");
  ntrackdist->SetFillColor(kBlue);
  ntrackdist->GetXaxis()->SetTitle("Number of tracks");
  ntrackdist->GetYaxis()->SetTitle("Number of events");
  ntrackdist->Draw("bar2");
  c3->Print(Form("%s/ntrackdist.png",figdir));
}