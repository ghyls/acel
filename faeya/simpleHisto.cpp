

#include <iostream>




void ej2() 
{


  std::cout << "Hello world!" << std::endl;

  // Create TTree from file
  TString path = "./practica/files/";
  TFile f(path + "dy.root");
  TTree * theTree = (TTree *) f.Get("events");


  // init Histos
  TH1F * hDY = new TH1F("dimu pT DYw", "", 16, 0, 160); 
  TH1F * htt = new TH1F("dimu pT tt", "", 16, 0, 160);

  // Set Adresses to branches
  Int_t NMuon;
  Float_t Muon_Px[3];
  Float_t Muon_Py[3];
  Float_t Muon_Pz[3];
  Float_t Muon_E[3];

  theTree->SetBranchAddress("NMuon", &NMuon);
  theTree->SetBranchAddress("Muon_Px", &Muon_Px);
  theTree->SetBranchAddress("Muon_Py", &Muon_Py);
  theTree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  theTree->SetBranchAddress("Muon_E", &Muon_E);

  numEvents = theTree->GetEntries();
  std::cout << numEvents << std::endl;

  TLorentzVector muon1;
  TLorentzVector muon2;
  
  Float_t diPt;

  // main loop
  for (int i = 0; i < numEvents; i++)   // DY
  {
    theTree->GetEntry(i);
    
    // only events with 2 muons
    if (NMuon !=2) continue;

    muon1.SetPxPyPzE(Muon_Px[0], Muon_Py[0], Muon_Pz[0], Muon_E[0]);
    muon2.SetPxPyPzE(Muon_Px[1], Muon_Py[1], Muon_Pz[1], Muon_E[1]);

    // Por qué están todos llenos?
    //std::cout << Muon_Pz[0] <<" "<< Muon_Pz[1] <<" "<< Muon_Pz[2] << std::endl;

    diPt = (muon1 + muon2).Pt();
    hDY->Fill(diPt);
  }

  TCanvas * c = new TCanvas("c", "c", 10, 10, 800, 600);
  hDY->Draw("hist");
  c->Print("DiMuPt.png", "png");

}
