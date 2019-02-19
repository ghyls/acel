

#include <iostream>


void fillHistosFromTree(TTree * tree, TH1F * histo)
{
  // solo 2 muones
  // diPt

  // ...........................................................................

  // Set Adresses to branches
  Int_t NMuon;
  Float_t Muon_Px[3];
  Float_t Muon_Py[3];
  Float_t Muon_Pz[3];
  Float_t Muon_E[3];

  tree->SetBranchAddress("NMuon", &NMuon);
  tree->SetBranchAddress("Muon_Px", &Muon_Px);
  tree->SetBranchAddress("Muon_Py", &Muon_Py);
  tree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  tree->SetBranchAddress("Muon_E", &Muon_E);

  numEvents = tree->GetEntries();
  TLorentzVector muon1;
  TLorentzVector muon2;
  Float_t diPt;


  // main loop
  for (int i = 0; i < numEvents; i++)   // DY
  {
    tree->GetEntry(i);
    
    // only events with 2 muons
    if (NMuon !=2) continue;

    muon1.SetPxPyPzE(Muon_Px[0], Muon_Py[0], Muon_Pz[0], Muon_E[0]);
    muon2.SetPxPyPzE(Muon_Px[1], Muon_Py[1], Muon_Pz[1], Muon_E[1]);

    // Por qué están todos llenos?
    //std::cout << Muon_Pz[0] <<" "<< Muon_Pz[1] <<" "<< Muon_Pz[2] << std::endl;

    diPt = (muon1 + muon2).Pt();
    histo->Fill(diPt);
  }
}

void simpleHisto()
{


  std::cout << "Hello world!" << std::endl;

  TString path = "./practica/files/";

  // init Histos
  TH1F * hDY = new TH1F("dimu pT DYw", "", 16, 0, 160); 
  TH1F * htt = new TH1F("dimu pT tt", "", 16, 0, 160);


  // Fill DY tree
  TFile f(path + "dy.root");
  TTree * dyTree = (TTree *) f.Get("events");
  fillHistosFromTree(dyTree, hDY);

  // Fill tt tree
  TFile f2(path + "ttbar.root");
  TTree * ttTree = (TTree *) f2.Get("events");
  fillHistosFromTree(ttTree, htt);


  //do Plots
  TCanvas * c = new TCanvas("c", "c", 10, 10, 800, 600);

  // Normalizamos los histogramas a 1
  hDY->Scale(1/hDY->Integral());
  htt->Scale(1/htt->Integral());

  // Cosmetics
  hDY->SetLineColor(kAzure+2);
  htt->SetLineColor(kRed+1);
  hDY->SetLineWidth(2);
  htt->SetLineWidth(2);


  hDY->Draw("hist");
  htt->Draw("hist,same"); // En el mismo TCanvas

  // Ejes y otras opciones... (solo en el primer histograma que pintamos)
  hDY->SetStats(0);
  hDY->GetXaxis()->SetTitle("p_{T}^{#mu#mu} (GeV)");
  hDY->GetYaxis()->SetTitle("Events");

  // Leyenda
  leg = TLegend(0.7, 0.7, 0.9, 0.9); // x0, y0, x1, y1
  leg.AddEntry(hDY, "DY", "l");
  leg.AddEntry(htt, "t#bar{t}", "l");
  leg.Draw();

  // Guardamos la figura como .png y .pdf
  c->Print("DiMuPt.png", "png");

}
