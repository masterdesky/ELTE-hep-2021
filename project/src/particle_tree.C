//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 22 09:11:30 2017 by ROOT version 5.30/03
// from TTree particle_tree/Tree of particles
// found on file: measure_createtree.root
//////////////////////////////////////////////////////////

#define particle_tree_cxx
#include "particle_tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <string>
#include <iostream>
#include <fstream>

void particle_tree::Loop()
{
//   Normally, this Loop subroutine is not needed, but if one wants,
//   then in a ROOT session, the following can be done:
//      Root > .L particle_tree.C
//      Root > particle_tree t("data.root")
//      Root > t.GetEntry(12); // Fill t data members with entry (event) #12
//      Root > t.Show();       // Show values of entry #12
//      Root > t.Show(16);     // Read and show values of entry #16
//      Root > t.Loop();       // Loop on all entries
//

//   This code reads each TTree in the TChain, if needed
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

particle_tree::particle_tree(const char* filename) : fChain(0)
{
  std::string fn(filename);
  if(fn.substr(fn.size()-4)=="root")
  {
    TFile *f = new TFile(filename);
    TTree *tree;
    f->GetObject("particle_tree",tree);
    std::cerr << "Initializing root file " << filename << std::endl;
    Init(tree);
    if(fChain) std::cerr << "Obtained tree(s)" << std::endl;
  }
  else
  {
    TChain *chain = new TChain("particle_tree");
    std::ifstream infile(filename);
    std::string rootfile("");
    while(infile >> rootfile)
    {
      chain->Add(rootfile.c_str());
    }
    std::cerr << "Initializing text file " << filename << std::endl;
    Init(chain);
    if(fChain) std::cerr << "Obtained trees(s)" << std::endl;
  }
}

particle_tree::~particle_tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t particle_tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t particle_tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void particle_tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Nevents", &Nevents, &b_Nevents);
   fChain->SetBranchAddress("Zvertex", &Zvertex, &b_Zvertex);
   fChain->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
   fChain->SetBranchAddress("ReactionPlane", &ReactionPlane, &b_ReactionPlane);
   fChain->SetBranchAddress("Ntracks", &Ntracks, &b_Ntracks);
   fChain->SetBranchAddress("px", px, &b_px);
   fChain->SetBranchAddress("py", py, &b_py);
   fChain->SetBranchAddress("pz", pz, &b_pz);
   fChain->SetBranchAddress("E", E, &b_E);
   fChain->SetBranchAddress("ch", ch, &b_ch);
   fChain->SetBranchAddress("Mch", Mch, &b_Mch);
   fChain->SetBranchAddress("isPi", isPi, &b_isPi);
   fChain->SetBranchAddress("detp", detp, &b_detp);
   fChain->SetBranchAddress("detz", detz, &b_detz);
   Notify();
}

Bool_t particle_tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void particle_tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t particle_tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
