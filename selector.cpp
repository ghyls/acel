
#include "selector.h"

#include <iostream>
#include <string>
#include <mpi.h>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

// Selector corre una vez por muestra.

// TODO: pintar todos los MC, uno encima del otro
// TODO: pintar los datos en el mismo histo
// TODO: crear histograma cociente
// TODO: aplicar cortes




Selector::Selector(std::string _filePath, std::string _fileName)
{

  if (!fileExist(_filePath + _fileName + ".root"))
  {
    std::cout << "[Selector] ERROR. File '" << _filePath + _fileName << "' "
    "does not exist!" << std::endl;

    throw;
  }

  process = _fileName;
  filePath = TString(_filePath);  // including last '/'

  CreateHistograms(process);
  Loop();

}

Selector::~Selector()
{
  for (int i = 0; i < histograms.size(); i++)
  {
    delete histograms[i];

  }

  histograms.clear();
  histograms = {};
}

TH1F* Selector::GetHisto(TString name) //name is the name of the histo
{
    int len = histograms.size();
    for (int i = 0; i < len; i++)
    {
      TString n = histograms[i]->GetName(); //full name

      if ((process + '_' + name) == n)
        return histograms[i];
    }

    std::cout << "[Selector::GetHisto] WARNING: not found histo named " + \
    name + ". Keep up the good work!" << std::endl;

    throw;
}

int Selector::GetNumberOfHistos()
{
  return histograms.size();
}


  
bool Selector::fileExist(const std::string name)
{
  return (access(name.c_str(), F_OK) != -1);
}

void Selector::CreateHistograms(TString prefix)
{

  //there variables may be called from other cpps.
  TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), "", 20, 20, 120);
  TH1F* h2 = new TH1F(prefix + TString("_Jet_btag"), "", 80, -2, 6);
  TH1F* h3 = new TH1F(prefix + TString("_MuonPt_raw"), "", 20, 10, 120);
  TH1F* h4 = new TH1F(prefix + TString("_Acep_gen"), "", 20, -1, 2);
  TH1F* h5 = new TH1F(prefix + TString("_Acep_obs"), "", 20, -1, 2);
  TH1F* h6 = new TH1F(prefix + TString("_MuMuMass"), "", 60, 60, 120);
  TH1F* h7 = new TH1F(prefix + TString("_Muon_Iso"), "", 60, 0, 0.2);
  TH1F* h8 = new TH1F(prefix + TString("_MuonPt_TriggOnly"), "", 20, 10, 120);

  histograms.push_back(h1); 
  histograms.push_back(h2); 
  histograms.push_back(h3); 
  histograms.push_back(h4); 
  histograms.push_back(h5); 
  histograms.push_back(h6); 
  histograms.push_back(h7); 
  histograms.push_back(h8); 
}

void Selector::Loop()
{ 
  TFile f(filePath + process + ".root");
  TTree * tree = (TTree *) f.Get("events");

  Int_t numEvents = tree->GetEntries();

  // Set Adresses to branches >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Int_t NMuon;
  Int_t NJet;
  Float_t Jet_E[20]; 
  Float_t Jet_btag[20]; 
  Float_t Jet_Px[20], Jet_Py[20], Jet_Pz[20];
  Bool_t triggerIsoMu24;
  Float_t Muon_Px[5]; // just in case... memory is not a problem here
  Float_t Muon_Py[5];
  Float_t Muon_Pz[5];
  Float_t Muon_E[5];
  Float_t Muon_Iso[5];
  Float_t EventWeight;
  Float_t MClepton_px[5], MClepton_py[5], MClepton_pz[5];
  Int_t MCleptonPDGid[5]; // 11 electrones, 13 muones, 15 taus



  tree->SetBranchAddress("NMuon", &NMuon); //
  tree->SetBranchAddress("Muon_Px", &Muon_Px);
  tree->SetBranchAddress("Muon_Py", &Muon_Py);
  tree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  tree->SetBranchAddress("Muon_E", &Muon_E);
  tree->SetBranchAddress("Muon_Iso", &Muon_Iso);
  tree->SetBranchAddress("triggerIsoMu24", &triggerIsoMu24);
  tree->SetBranchAddress("EventWeight", &EventWeight);
  tree->SetBranchAddress("NJet", &NJet);
  tree->SetBranchAddress("Jet_E", &Jet_E);
  tree->SetBranchAddress("Jet_Px", &Jet_Px);
  tree->SetBranchAddress("Jet_Py", &Jet_Py);
  tree->SetBranchAddress("Jet_Pz", &Jet_Pz);

  tree->SetBranchAddress("Jet_btag", &Jet_btag);
  tree->SetBranchAddress("MClepton_px", &MClepton_px);
  tree->SetBranchAddress("MClepton_py", &MClepton_py);
  tree->SetBranchAddress("MClepton_pz", &MClepton_pz);
  tree->SetBranchAddress("MCleptonPDGid", &MCleptonPDGid);
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Loop over every event >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  std::cout << "[Selector::loop]: " << process << ", " << numEvents  << \
                " events." << std::endl;

  for (int i = 0; i < numEvents; i++)
  {
    tree->GetEntry(i);

    Int_t NIsoMuon = 0;

    TLorentzVector leadMuon; 
    TLorentzVector muon2; 
    int leadMuIndex;      // index of the leading muon, inside the Muon tuple.

    TLorentzVector muon;  // aux variables
    TLorentzVector jet;   // aux variables


    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // count NIsoMuon and store the 2 with larger pt
    for (int j = 0; j < NMuon; j++)
    {
      muon.SetPxPyPzE(Muon_Px[j], Muon_Py[j], Muon_Pz[j], Muon_E[j]); 
      
      GetHisto("Muon_Iso")->Fill(Muon_Iso[j]/muon.Pt());
      
      if (Muon_Iso[j]/muon.Pt() < 0.1)
      {
        NIsoMuon ++;
        if (NIsoMuon == 1) {leadMuon = muon; leadMuIndex = j;} // bigger pt
        if (NIsoMuon == 2) {muon2 = muon;}                     // lower pt
      }
      
    } // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonIsolation 
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    // Z mass >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    if (NIsoMuon > 1 && triggerIsoMu24)
    {
      if(leadMuon.Pt() > 25)//|| muon2.Pt() > 20)
      {
        GetHisto("MuMuMass")->Fill((leadMuon + muon2).M(), EventWeight);
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




    // =========================================================================
    if (NIsoMuon != 1) {continue;} //  TTBAR ANALYSIS 
    // =========================================================================

    // Aceptancia, GEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    if (TMath::Abs(MCleptonPDGid[leadMuIndex]) == 13) // solo los muones
    {
      muon.SetPxPyPzE(MClepton_px[leadMuIndex], MClepton_py[leadMuIndex], MClepton_pz[leadMuIndex], 0);
  
      if (muon.Pt() > 26.7)
      {
        GetHisto("Acep_gen")->Fill(0);
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    Float_t weight;
    if (process != "data") {weight = EventWeight;} else weight = 1;
    

    GetHisto("MuonPt_raw")->Fill(leadMuon.Pt(), weight);


    if (triggerIsoMu24) 
    {
      GetHisto("MuonPt_TriggOnly")->Fill(leadMuon.Pt(), weight);

      // Aceptancia >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (leadMuon.Pt() > 26.7)
      {
        GetHisto("Acep_obs")->Fill(0);
      }
      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      if (NJet > 0 && NJet < 10)
      {
        for (int j = 0; j < NJet; j++)
          {
            GetHisto("Jet_btag")->Fill(Jet_btag[j], weight);
          }
      }
      
      if (leadMuon.Pt() > 26.7)
      {
        static int bJets = 0;      
        static int nJetWithBigPt = 0;    // useless  
        if (NJet > 3)  
        {
          for (int j = 0; j < NJet; j++)
          {
            jet.SetPxPyPzE(Jet_Px[j], Jet_Py[j], Jet_Pz[j], Jet_E[j]);
            
            if (Jet_btag[j] > 2 && jet.Pt() > 20)
            {
              bJets ++;
            }
            
          }
          if (bJets > 0)// && nJetWithBigPt > 0)
          {
            GetHisto("MuonPt")->Fill(leadMuon.Pt(), weight); 
          }  
        }    
      }
    }



    // <<<<<<<<<<<<<<<<<<<<<<<<>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }



}

