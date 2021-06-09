// Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <numeric>
#include <functional>

// ROOT libraries
#include <TF1.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMarker.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>

// Custom libraries and headers
#define particle_tree_cxx
#include "particle_tree.h"


template<typename T>
std::vector<std::vector<T>>
react_plane_bins(std::string const &react_plane_file)
{
  // Loads in R_{2} = <cos n (Psi - Psi_{RP})> from file for different
  // centrality intervals between 0%-92%

  // In the project we're only working with a single centrality class, so we
  // have to find some clever average of the different centrality values
  std::vector<T> cent, rxnp;

  std::fstream rxnp_file;
  rxnp_file.open(react_plane_file, std::ios::in);
  if (!rxnp_file)
  {
    std::cerr << "Reaction plane datafile missing" << std::endl;
  }
  else
  {
    std::string line;
    while (std::getline(rxnp_file, line))
    {
      std::stringstream ss(line);
      double CENT, RxNP;
      // Only read lines with doubles in it
      if (ss >> CENT >> RxNP)
      {
        cent.push_back(CENT);
        rxnp.push_back(RxNP);
      }
    }
    rxnp_file.close();
  }

  return {cent, rxnp};
}


template<typename T>
std::vector<T> get_bin_widths(std::vector<T> const &cent, std::vector<T> const &rxnp)
{
  // Number of bins
  int N_bins = cent.size();

  // Array to store widths of bins
  std::vector<T> widths (N_bins);

  // I don't know why but if I comment out this line, the code breaks...
  //std::cout << "" << std::endl;

  // Calculate average width and add it to the bin width array
  auto avg_width = (cent[N_bins-1] - cent[0]) / N_bins;
  widths[0] = avg_width;
  widths[N_bins-1] = avg_width;

  // Calculate widths by iterating through [1] -> [N_bins]
  for(int i = 1; i < N_bins-1; i++)
  {
    widths[i] = (cent[i] - cent[i-1]) / 2 + (cent[i+1] - cent[i]) / 2;
  }

  return widths;
}


template<typename T>
double react_plane_res(std::string const &react_plane_file)
{
  // Get bin centers and RxNP values
  auto cent_rxnp = react_plane_bins<T>(react_plane_file);
  auto cent = cent_rxnp[0];
  auto rxnp = cent_rxnp[1];

  // Calculate bin widths and maximum elements
  auto widths = get_bin_widths<T>(cent, rxnp);
  double w_max = *max_element(widths.begin(), widths.end());
  // Normalize bin widths with max element
  transform(widths.begin(), widths.end(), widths.begin(), [w_max](T &c){ return c/w_max; });


  // Calculate weighted average of RxNP values to get an average value for R
  double acc = 0;
  int N_bins = cent.size();
  for(int i = 0; i < N_bins; i++)
  {
    acc += rxnp[i] * widths[i];
  }

  return acc / N_bins;
}


double calc_pt(particle_tree const &p, int &ipart)
{
  // Calculate the p_{T} traverse momentum for all particle tracks

  double pt = sqrt(p.px[ipart]*p.px[ipart] + p.py[ipart]*p.py[ipart]);

  return pt;
}

double calc_phi(particle_tree const &p, int &ipart)
{

  // CALCULATE PHI = PHI_0 - PSI
  double phi_0 = atan2(p.py[ipart], p.px[ipart]);
  double phi = phi_0 - p.ReactionPlane;

  // NORMALIZE PHI TO THE INTERVAL OF [-PI/2, PI/2]
  if(phi < - M_PI) { phi += 2*M_PI; }
  if(phi > M_PI) { phi -= 2*M_PI; }
  if(phi < - M_PI/2) { phi += M_PI; }
  if(phi > M_PI/2) { phi -= M_PI; }

  return phi;
}


void write_histograms(TH1 *ptdist, TH1 *phidist, TH1 *ntrackdist,
                      std::string outfile)
{
  // WRITE ALL HISTOGRAMS TO THE OUTPUT ROOT FILE
  TFile *f = new TFile(outfile.c_str(),"RECREATE");
  if(!f->IsWritable())
  {
    std::cerr << "File " << outfile.c_str() << " was not opened!" << std::endl;
  }
  else
  {
    std::cerr << "Analysis done, writing histos to " << outfile.c_str() << std::endl;
  }

  f->cd();
  ptdist->Write();
  phidist->Write();
  ntrackdist->Write();
  f->Write();
  f->Close();
}

void fill_histograms(particle_tree &p, int const &Nevents, int const &N_bins,
                     std::string outfile)
{
  // CREATE HISTOGRAMS TO BE FILLED BY LOOPING THROUGH ALL EVENTS
  TH1 *ptdist = new TH1F("ptdist", "pt distribution", N_bins, 0, 2);
  TH1 *phidist = new TH1F("phidist", "phi distribution", N_bins, -M_PI/2, M_PI/2);
  TH1 *ntrackdist = new TH1F("ntrackdist", "track number distribution", 51, 0, 50);

  // LOOP THROUGH EVENTS IN THE GIVEN DATASET
  for(int ievent = 0; ievent < Nevents; ievent++)
  {
    // MONITOR PROGRESS THROUGH STDERR OUTPUT
    if(ievent > 0 && (ievent+1)%1000==0) { std::cerr << "."; }
    if(ievent > 0 && (ievent+1)%10000==0) { std::cerr << "Analyzing event #" << ievent+1 << std::endl; }

    // LOAD EVENT DATA INTO particle_tree OBJECT
    p.GetEntry(ievent);

    // Add event-level data to histogram
    int N_tracks = p.Ntracks;
    ntrackdist->Fill(N_tracks);

    // LOOP THROUGH ALL PARTICLES OF THE GIVEN EVENT
    for(int ipart = 0; ipart < p.Ntracks; ipart++)
    {
      double pt = calc_pt(p, ipart);
      ptdist->Fill(pt);
      double phi = calc_phi(p, ipart);
      phidist->Fill(phi);
    }
  }

  write_histograms(ptdist, phidist, ntrackdist,
                   outfile);
}
           

std::vector<std::vector<double>>
phi_and_pt(particle_tree &p, int const &Nevents, int const &N_bins)
{
  // Loops through all events, tracks and particles and fills histograms with
  // specific values

  // Create N bins of p_{T} to store phi values in
  std::vector<std::vector<double>> phi_per_pt (N_bins);

  // LOOP THROUGH EVENTS IN THE GIVEN DATASET
  for(int ievent = 0; ievent < Nevents; ievent++)
  {
    // MONITOR PROGRESS THROUGH STDERR OUTPUT
    if(ievent > 0 && (ievent+1)%1000==0) { std::cerr << "."; }
    if(ievent > 0 && (ievent+1)%10000==0) { std::cerr << "Analyzing event #" << ievent+1 << std::endl; }

    // LOAD EVENT DATA INTO particle_tree OBJECT
    p.GetEntry(ievent);

    // LOOP THROUGH ALL PARTICLES OF THE GIVEN EVENT
    for(int ipart = 0; ipart < p.Ntracks; ipart++)
    {
      double pt = calc_pt(p, ipart);
      double phi = calc_phi(p, ipart);

      // Put phi values into pt bins
      int bin = pt / (2.0 / N_bins);
      if(bin >= N_bins) { bin = N_bins-1; }
      phi_per_pt[bin].push_back(phi);
    }
  }

  return phi_per_pt;
}


Double_t fit_func(Double_t *x, Double_t *par)
{
  return par[0] + 2*par[1]*cos(2*x[0]);
}


std::vector<double> fit_v2(TH1 *angdist, int i)
{
  // Fitting function
  TF1 *fit = new TF1("fit",fit_func, 0, 2, 2);
  fit->SetLineColor(kBlue);
  fit->SetLineWidth(10);
  fit->SetNpx(500);
  angdist->Fit("fit");

  double a = fit->GetParameter(0);
  double b = fit->GetParameter(1);
  double ea = fit->GetParError(0);
  double eb = fit->GetParError(1);

  double v2 = b / (2*a);
  double ev2 = (eb/b + 2*ea/a) * v2;

  if(i == 4 || i == 7 || i == 36 || i == 38)
  {
    // Plot fit
    TCanvas *cf = new TCanvas("c", "", 2200, 1400);
    cf->SetGrid();

    angdist->SetTitle("");
    angdist->GetXaxis()->SetTitle("phi [rad]");
    angdist->GetYaxis()->SetTitle("N(phi)");
    angdist->Draw("bar2");

    cf->Print(Form("../figs/fit_%i.png",i));
  }

  return { v2, ev2 };
}

std::vector<std::vector<double>>
angle_distribution(std::vector<std::vector<double>> phi_per_pt,
                   int const &N_bins, double &R)
{
  // Array to store v_{2} values
  std::vector<double> v2 (N_bins);
  std::vector<double> ev2 (N_bins);

  for(int i = 0; i < N_bins; i++)
  {
    if(phi_per_pt[i].size() < 100) { v2[i] = 0; continue; }

    // Fill histogram
    TH1 *angdist = new TH1F("angdist", "angle distribution", 250, -M_PI/2, M_PI/2);
    for(auto const& phi_i : phi_per_pt[i])
    {
      angdist->Fill(phi_i);
    }

    auto v2_fit = fit_v2(angdist, i);
    v2[i] = v2_fit[0] / R;
    ev2[i] = v2_fit[1] / R;

    delete angdist;
  }

  return { v2, ev2 };
}

void plot_v2fit(std::vector<std::vector<double>> v2_fit, int N_bins)
{
  // Plot v_{2} elliptic flow parameter
  // Create X-axis values (p_{T} bin centers)
  double dpt = 2.0 / N_bins;
  Double_t x[N_bins], y[N_bins], ex[N_bins], ey[N_bins];
  for(int i = 0; i < N_bins; i++)
  {
    x[i] = dpt/2 + i*dpt;
    y[i] = v2_fit[0][i];
    ex[i] = 0;
    ey[i] = v2_fit[1][i]; 
  }

  TCanvas *c = new TCanvas("c", "", 2200, 1400);
  c->SetGrid();
  TGraph *g = new TGraphErrors(N_bins, x, y, ex, ey);
  //g->SetTitle("p_{T} dependence of elliptic flow");
  g->SetTitle("");
  g->GetXaxis()->SetTitle("p_{T} [GeV/c]");
  g->GetYaxis()->SetTitle("v_{2}");
  g->Draw("AL");

  TMarker *m = new TMarker();
  m->SetMarkerStyle(20);
  m->SetMarkerColor(kBlue);
  for (int i = 0; i < N_bins; i++)
  {
    m->SetMarkerStyle(21);
    m->SetMarkerSize(3);
    m->DrawMarker(x[i], y[i]);
  }

  c->Print("../figs/v2_flow_per_pt.png");
}


int main(int argc, const char** argv)
{
  if(argc<2)
  {
    std::cerr << "Usage: " << argv[0] << " <input file name>"
                                      << " <output file name>"
                                      << " <reaction plane res. file>"
                                      << " <max events>"
                                      << " <number of bins>"
                                      << std::endl;
    return -1;
  }

  std::string datafile(argv[1]);
  std::string outfile(argv[2]);
  std::string react_plane_file(argv[3]);
  std::cerr << "Writing to " << outfile.c_str() << std::endl;
  int Nmaxevt = atoi(argv[4]);
  int N_bins = atoi(argv[5]);

  // INITIALIZE particle_tree OBJECT
  particle_tree p(datafile.c_str());
  if(p.fChain) { std::cerr << "Tree initialized" << std::endl; }
  else { std::cerr << "No tree found." << std::endl; return 1; }

  // DETERMINE HOW MANY EVENTS TO RUN ON
  int Nevents = p.fChain->GetEntries();
  if(Nmaxevt > 0 && Nmaxevt < Nevents) { Nevents = Nmaxevt; }
  std::cerr << "Will run on " << Nevents << " events (out of " << p.fChain->GetEntries()  << ")." << std::endl;

  // Get reaction plane correction parameter
  auto R = react_plane_res<double>(react_plane_file.c_str());
  std::cout << "R parameter : " << R << std::endl;

  // Fill histograms
  auto phi_per_pt = phi_and_pt(p, Nevents, N_bins);

  // Calculate the v_{2} values and their errors for different p_{T} bins
  auto v2_fit = angle_distribution(phi_per_pt, N_bins, R);

  // Plot the results of the v_{2} calculations
  fill_histograms(p, Nevents, N_bins, outfile);
  plot_v2fit(v2_fit, N_bins);

  return 0;
}
