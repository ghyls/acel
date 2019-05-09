#pragma once

#include <string>


#include "TH1.h"
#include "TLorentzVector.h"
#include "TTree.h"



class Selector
{
  public:

    std::vector<TH1F*> histograms; 
    TString process;                // without .root
  
    float bIdentAndMatched = 0;
    float missBIdentAndMatched = 0;
    float totalGenB = 0;
    float totalGenMissB = 0;
    float bTagEff = 0;
    float acep = 0;
    float MET_pz[2];
    float ttbarGen;
    float ttbarReco;

    int tmp0 = 0;
    int tmp1 = 0;

    // branches ----------------------------------------------------------------
    Int_t NMuon;
    Int_t NJet;
    Float_t Jet_E[20]; 
    Float_t Jet_btag[20]; 
    Bool_t Jet_ID[20]; 
    Float_t Jet_Px[20], Jet_Py[20], Jet_Pz[20];

    Float_t MChadronicBottom_px; // if filled, there are 1 b tags.
    Float_t MChadronicBottom_py; // if filled, there are 1 b tags.
    Float_t MChadronicBottom_pz; // if filled, there are 1 b tags.

    Float_t MCleptonicBottom_px; // if filled, there are 2 b tags.
    Float_t MCleptonicBottom_py; // if filled, there are 2 b tags.
    Float_t MCleptonicBottom_pz; // if filled, there are 2 b tags.

    Float_t MChadronicWDecayQuark_px;
    Float_t MChadronicWDecayQuark_py;
    Float_t MChadronicWDecayQuark_pz;

    Float_t MChadronicWDecayQuarkBar_px;
    Float_t MChadronicWDecayQuarkBar_py;
    Float_t MChadronicWDecayQuarkBar_pz;

    Bool_t triggerIsoMu24;
    Float_t Muon_Px[5]; // just in case... memory is not a problem here
    Float_t Muon_Py[5];
    Float_t Muon_Pz[5];
    Float_t Muon_E[5];
    Float_t MET_px, MET_py;
    Float_t Muon_Iso[5];
    Float_t EventWeight;
    Float_t MClepton_px, MClepton_py, MClepton_pz;
    Float_t MCneutrino_px, MCneutrino_py, MCneutrino_pz;
    Int_t MCleptonPDGid; // 11 electrones, 13 muones, 15 taus  
    // -------------------------------------------------------------------------

    // event variables
    TLorentzVector leadMuon; 
    TLorentzVector muon2; 

    TLorentzVector muon;  // aux variables
    TLorentzVector jet;   // aux variables
    TLorentzVector jet2;  // aux variables
    TLorentzVector nu;    // aux variables
    TLorentzVector W;
    TLorentzVector b;
    TLorentzVector b2;


    int numEvents;

    // Jet Variables
    bool jetHasGoodPtEta[20];
    int NTrueJets = 0;
    bool jetIsGood[20];
    int bJets = 0;
    bool jetIsB[20];


    Selector();
    ~Selector();       

    Selector(std::string _filePath, std::string _fileName);
    
    TH1F* GetHisto(TString name);

    void ReadTrees(TTree * tree);
    int GetNumberOfHistos();
    float Module(float x, float y, float z);
    float DR(TLorentzVector v1, TLorentzVector v2);
    float * ComputeNuPz(TLorentzVector nuAux, TLorentzVector leadMuon);
    void DoJetGoodPt();
    void CreateHistograms(TString prefix);
    void JetQuality();
    void ComputeBTagEff(float discr);
    void ComputeMissTagEff(float discr);
    void DoBTagging(float discr);
    void PrintBTagEffData();
    void ClearVariables();


  private:

    TString filePath;               // path of the root file

    bool fileExist(const std::string name);


    void Loop();
};