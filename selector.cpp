
#include "selector.h"

#include <iostream>
#include <string>
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

// Selector corre una vez por muestra.

// TODO: pintar todos los MC, uno encima del otro
// TODO: pintar los datos en el mismo histo
// TODO: crear histograma cociente
// TODO: aplicar cortes




Selector::Selector(std::string _filePath, std::string _fileName, std::string _prefix)
{


  if (!fileExist(_filePath + _fileName + ".root"))
  {
    std::cout << "[Selector] ERROR. File '" << _filePath + _fileName << "' "
    "does not exist!" << std::endl;

    throw;
  }

  histoName = _fileName;
  filePath = TString(_filePath);  // including last '/'

  prefix = TString(_prefix);      //

  CreateHistograms();
  Loop();
}

TH1F* Selector::GetHisto(TString name) //name is the name of the histo
{
    int len = histograms.size();
    for (int i = 0; i < len; i++)
    {
      TString n = histograms[i]->GetName(); //full name
            
      if ((prefix + '_' + name) == n)
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
  //std::cout << name << std::endl;
  return (access(name.c_str(), F_OK) != -1);
}

void Selector::CreateHistograms()
{
  TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), ";p_{T}^{#mu} (GeV);Events", 20, 0, 200);
  TH1F* h2 = new TH1F(prefix + TString("_DiMuonMass"), ";m^{#mu#mu} (GeV);Events", 20, 0, 200);

  histograms.push_back(h1); 
  histograms.push_back(h2);    

  //add here more histograms   
}

void Selector::Loop()
{ 
  //TFile f(filePath + fileName);
  TFile f(filePath + histoName + ".root");
  TTree * tree = (TTree *) f.Get("events");

  Int_t numEvents = tree->GetEntries();

  // Set Adresses to branches >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Int_t NMuon;
  Bool_t triggerIsoMu24;
  Float_t Muon_Px[3]; // just in case... memory is not a problem here
  Float_t Muon_Py[3];
  Float_t Muon_Pz[3];
  Float_t Muon_E[3];
  Float_t EventWeight;

  tree->SetBranchAddress("NMuon", &NMuon); //
  tree->SetBranchAddress("Muon_Px", &Muon_Px);
  tree->SetBranchAddress("Muon_Py", &Muon_Py);
  tree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  tree->SetBranchAddress("Muon_E", &Muon_E);
  tree->SetBranchAddress("triggerIsoMu24", &triggerIsoMu24);
  tree->SetBranchAddress("EventWeight", &EventWeight);
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Loop over every event >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  for (int i = 0; i < numEvents; i++)
  {
    tree->GetEntry(i);

    if (!triggerIsoMu24) {continue;}
    if (NMuon != 1) {continue;} 

    TLorentzVector muon;
    muon.SetPxPyPzE(Muon_Px[0], Muon_Py[0], Muon_Pz[0], Muon_E[0]);
        
    Float_t weight;
    if (histoName != "data") {weight = EventWeight;} else weight = 1;

    // full analysis >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // fill histos >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    GetHisto("MuonPt")->Fill(muon.Pt()); //C'mon Mario, you can do it better 

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }


}




void selector()
{
  std::string path = "./practica/files/";
  std::string name = "dy.root";
  std::string prefix = "hello";
  
  Selector obj (path, name, prefix);
  obj.GetHisto("MuonPt");
}