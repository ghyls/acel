
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
  TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), "", 20, 10, 120);
  TH1F* h2 = new TH1F(prefix + TString("_Jet_btag"), "", 80, -2, 10);
  TH1F* h3 = new TH1F(prefix + TString("_MuonPt_raw"), "", 20, 10, 120);
  TH1F* h4 = new TH1F(prefix + TString("_Acep_gen"), "", 20, -1, 2);
  TH1F* h5 = new TH1F(prefix + TString("_Acep_obs"), "", 20, -1, 2);
  
  histograms.push_back(h1); 
  histograms.push_back(h2); 
  histograms.push_back(h3); 
  histograms.push_back(h4); 
  histograms.push_back(h5); 

}

void Selector::Loop()
{ 
  TFile f(filePath + process + ".root");
  TTree * tree = (TTree *) f.Get("events");

  Int_t numEvents = tree->GetEntries();

  // Set Adresses to branches >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Int_t NMuon;
  Int_t NJet;
  Bool_t triggerIsoMu24;
  Float_t Muon_Px[3]; // just in case... memory is not a problem here
  Float_t Muon_Py[3];
  Float_t Muon_Pz[3];
  Float_t Muon_E[3];
  Float_t Jet_btag[10]; 
  Float_t EventWeight;

  Float_t MClepton_px[3], MClepton_py[3], MClepton_pz[3];



  tree->SetBranchAddress("NMuon", &NMuon); //
  tree->SetBranchAddress("Muon_Px", &Muon_Px);
  tree->SetBranchAddress("Muon_Py", &Muon_Py);
  tree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  tree->SetBranchAddress("Muon_E", &Muon_E);
  tree->SetBranchAddress("triggerIsoMu24", &triggerIsoMu24);
  tree->SetBranchAddress("EventWeight", &EventWeight);
  tree->SetBranchAddress("NJet", &NJet);
  tree->SetBranchAddress("Jet_btag", &Jet_btag);
  tree->SetBranchAddress("MClepton_px", &MClepton_px);
  tree->SetBranchAddress("MClepton_py", &MClepton_py);
  tree->SetBranchAddress("MClepton_pz", &MClepton_pz);
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Loop over every event >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  std::cout << "[Selector::loop]: " << process << ", " << numEvents  << \
                " events." << std::endl;


  for (int i = 0; i < numEvents; i++)
  {

    tree->GetEntry(i);
    if (NMuon != 1) {continue;} 

    TLorentzVector muon;
    muon.SetPxPyPzE(Muon_Px[0], Muon_Py[0], Muon_Pz[0], Muon_E[0]);

    TLorentzVector muonGen;
    muonGen.SetPxPyPzE(MClepton_px[0], MClepton_py[0], MClepton_pz[0], 0);
    

    Float_t weight;
    if (process != "data") {weight = EventWeight;} else weight = 1;
 

    

    // full analysis >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  


    GetHisto("MuonPt_raw")->Fill(muon.Pt(), weight);


    // Aceptancia >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    //ojo con el numero de muones... con el ID del PDG! 
    if (triggerIsoMu24) 
    {
      if (muon.Pt() > 26.7)
      {
        GetHisto("Acep_obs")->Fill(0);
      }
      if (muonGen.Pt() > 26.7)
      {
        GetHisto("Acep_gen")->Fill(0);
      }


      GetHisto("MuonPt")->Fill(muon.Pt(), weight); 
      if (NJet > 0 && NJet < 10)
      {
        for (int j = 0; j < NJet; j++)
          {
            GetHisto("Jet_btag")->Fill(Jet_btag[j], weight);
          }
      }
      
      if (muon.Pt() > 26.7)
      {
        static int bJets = 0;      
        if (NJet > 0 && NJet << 10)  
        {
          for (int j = 0; j < NJet; j++)
          {
            if (Jet_btag[j] > 1) {bJets ++;}
          }
          if (bJets > 0)
          {
            // TODO: something here
          }  
        }    
      }
    }



    // <<<<<<<<<<<<<<<<<<<<<<<<>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }



}

