#define particle_tree_cxx
#include "particle_tree.h"
#include <iostream>
#include <cmath>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;

int main(int argc, const char** argv)
{
  if(argc<2)
  {
    cerr << "Usage: " << argv[0] << " <input file name> <output file name> <max events=-1>" << endl;
    return -1;
  }
  string infilename(argv[1]);
  string outfilename(argv[2]);
  cerr << "Writing to " << outfilename.c_str() << endl;
  int Nmaxevt = -1;
  if(argc>=4) Nmaxevt = atoi(argv[3]);
  if(Nmaxevt<1) Nmaxevt = -1;

  //CREATE A HISTOGRAM TO BE FILED BY LOOPING THROUGH ALL EVENTS
  TH1 *ptdist = new TH1F("ptdist","pt distribution",100,0,2);

  //INITIALIZE particle_tree OBJECT
  particle_tree p(infilename.c_str());
  if(p.fChain) cerr << "Tree initialized" << endl;
  else { cerr << "No tree found." << endl; return 1; }

  //DETERMINE HOW MANY EVENTS TO RUN ON
  long unsigned int Nevents = p.fChain->GetEntries();
  if(Nmaxevt>0 && Nmaxevt<(int)Nevents) Nevents=Nmaxevt;
  cerr << "Will run on " << Nevents << " events (out of " << p.fChain->GetEntries()  << ")." << endl;

  //LOOP THROUGH EVENTS IN THE GIVEN DATASET
  for(long unsigned int ievent=0;ievent<Nevents;ievent++)
  {
    //MONITOR PROGRESS THROUGH STDERR OUTPUT
    if(ievent>0&&ievent%1000==0) cerr << ".";
    if(ievent>0&&ievent%10000==0) cerr << "Analyzing event #" << ievent << endl;

    //LOAD EVENT DATA INTO particle_tree OBJECT
    p.GetEntry(ievent);

    //LOOP THROUGH ALL PARTICLES OF THE GIVEN EVENT
    for(int ipart=0;ipart<p.Ntracks;ipart++)
    {
      //CALCULATE A VARIABLE AND FILL IT INTO A HISTOGRAM
      double pt = sqrt(p.px[ipart]*p.px[ipart]+p.py[ipart]*p.py[ipart]);
      ptdist->Fill(pt);
    }
  }
  cerr << endl;

  //WRITE ALL HISTOGRAMS TO THE OUTPUT ROOT FILE
  TFile *f = new TFile(outfilename.c_str(),"RECREATE");
  if(!f->IsWritable()) cerr << "File " << outfilename.c_str() << " was not opened!" << endl;
  else cerr << "Analysis done, writing histos to " << outfilename.c_str() << endl;
  f->cd();
  ptdist->Write();
  f->Write();
  f->Close();

  //BELOW ARE SOME EXAMPLES TO DRAW STUFF WITHOUT THE NEED TO LOOP THROUGH EVENTS
  //p.fChain->Draw("ReactionPlane","ReactionPlane>-9000");
  //c1->Print("figs/ReactionPlane.png");
  //p.fChain->Draw("Centrality","Centrality>-9000");
  //c1->Print("figs/Centrality.png");
  //p.fChain->Draw("Zvertex","Zvertex>-9000");
  //c1->Print("figs/Zvertex.png");
  //p.fChain->Draw("Ntracks");
  //c1->Print("figs/Ntracks.png");
  return 0; 
}
