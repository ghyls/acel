

#include <iostream>
#include <vector>
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

// Selector corre una vez por muestra.

// TODO: pintar todos los MC, uno encima del otro
// TODO: pintar los datos en el mismo histo
// TODO: crear histograma cociente
// TODO: aplicar cortes


class Selector
{

  public:
    
    TString prefix;                 // prefix for ALL histos
    TString fileName;               // name of the root file
    TString filePath;               // path of the root file
    std::vector<TH1F*> histograms;  
    
    void CreateHistograms()
    {
      TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), ";p_{T}^{#mu} (GeV);Events", 20, 0, 200);
      TH1F* h2 = new TH1F(prefix + TString("_DiMuonMass"), ";m^{#mu#mu} (GeV);Events", 20, 0, 200);

  	  histograms.push_back(h1); 
  	  histograms.push_back(h2);    

      //add here more histograms   
    }

    TH1F GetHisto(TString name) //name is the name of the histo
    {
        int len = histograms.size();
        for (int i = 0; i < len; i++)
        {
          TString n = histograms[i]->GetName(); //full name
            
          if ((prefix + '_' + name) == n)
            return *histograms[i];
        }

        std::cout << "[Selector::GetHisto] WARNING: not found histo named " + \
        name + ". Returning an empty one" << std::endl;
        return TH1F();
    }

    void Loop()
    {
      TFile f(filePath + fileName);
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

      tree->SetBranchAddress("NMuon", &NMuon);
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
        if (fileName != "data") {weight = EventWeight;} else weight = 1;

        // full analysis >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // fill histos >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        GetHisto('MuonPt').Fill(muon.Pt()); //C'mon Mario, you can do it better 

        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      }
    }

};


void selector()
{
    Selector obj;
    obj.prefix = "hello";
    obj.filePath = "./practica/files/";
    obj.fileName = "dy.root";
    obj.CreateHistograms();
    obj.Loop();
    obj.GetHisto("MuonPst");
}