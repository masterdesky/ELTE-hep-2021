using namespace std;

void Plot_analyzetree(const char* filename="analyzetree.root", const char* figdir="figs")
{
  TFile *f = new TFile(filename);

  TCanvas *c1 = new TCanvas();

  TH1 *ptdist = (TH1F*)f->Get("ptdist");
  ptdist->SetTitle("N(p_{T})");
  ptdist->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  ptdist->GetYaxis()->SetTitle("N(p_{T}) [c/GeV]");
  ptdist->Draw("e");
  c1->Print(Form("%s/ptdist.png",figdir));
}
