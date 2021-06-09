//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 22 09:11:30 2017 by ROOT version 5.30/03
// from TTree particle_tree/Tree of particles
// found on file: measure_createtree.root
//////////////////////////////////////////////////////////

#ifndef particle_tree_h
#define particle_tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class particle_tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Nevents;
   Float_t         Zvertex;
   Int_t           Centrality;
   Float_t         ReactionPlane;
   Int_t           Ntracks;
   Float_t         px[65];   //[Ntracks]
   Float_t         py[65];   //[Ntracks]
   Float_t         pz[65];   //[Ntracks]
   Float_t         E[65];   //[Ntracks]
   Int_t           ch[65];   //[Ntracks]
   Int_t           Mch[65];   //[Ntracks]
   Float_t         isPi[65];   //[Ntracks]
   Float_t         detp[65];   //[Ntracks]
   Float_t         detz[65];   //[Ntracks]

   // List of branches
   TBranch        *b_Nevents;   //!
   TBranch        *b_Zvertex;   //!
   TBranch        *b_Centrality;   //!
   TBranch        *b_ReactionPlane;   //!
   TBranch        *b_Ntracks;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_E;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_Mch;   //!
   TBranch        *b_isPi;   //!
   TBranch        *b_detp;   //!
   TBranch        *b_detz;   //!

   particle_tree(const char *filename="measure_createtree.root");
   virtual ~particle_tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

