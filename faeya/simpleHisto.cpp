

#include <iostream>

#include "Rtypes.h"
#include "TH1.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TLegend.h"

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

  Int_t numEvents = tree->GetEntries();
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

    diPt = (muon1 + muon2).Pt();
    histo->Fill(diPt);
  }
}


void plotWithRatio(TH1F * h1, TH1F * h2)
{
  //do Plots
  TCanvas *c = new TCanvas("c", "canvas", 800, 800);

  // Normalizamos los histogramas a 1
  h1->Scale(1/h1->Integral());
  h2->Scale(1/h2->Integral());


  // Define Pads >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // See https://root.cern.ch/root/html/tutorials/hist/ratioplot.C.html

  // Upper plot will be in pad1. 
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0.05); 
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  h1->SetStats(0);          // No statistics on upper plot
  h1->Draw("hist");         // Draw h1
  h2->Draw("same, hist");   // Draw h2 on top of h1
  
  // lower plot will be in pad2
  c->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.2);
  pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();       // pad2 becomes the current pad
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  // Define the ratio plot >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  TH1F *h3 = (TH1F*)h1->Clone("h3");
  h3->SetLineColor(kBlack);
  h3->SetMinimum(0);  // Define Y ..
  h3->SetMaximum(3); // .. range
  h3->Sumw2();
  h3->SetStats(0);      // No statistics on lower plot
  h3->Divide(h2);       // ~ h1/h2
  h3->SetMarkerStyle(21);
  h3->Draw("ep");       // Draw the ratio plot
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  
  // Cosmetics >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  h1->SetLineColor(kAzure+2);
  h2->SetLineColor(kRed+1);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);

  // Ejes y otras opciones... (solo en el primer histograma que pintamos)
  h1->SetStats(0);
  h1->GetYaxis()->SetTitle("Events");
  h1->GetYaxis()->SetTitleOffset(1.2);
  h1->GetYaxis()->SetLabelFont(43);
  h1->GetYaxis()->SetLabelSize(15);
  h1->GetXaxis()->SetLabelFont(43);
  h1->GetXaxis()->SetLabelSize(15);

  // Leyenda
  pad1->cd();  
  TLegend leg = TLegend(0.8, 0.8, 0.9, 0.9); // x0, y0, x1, y1
  leg.AddEntry(h1, "DY", "l");
  leg.AddEntry(h2, "t#bar{t}", "l");
  leg.SetTextSize(0.043);
  leg.Draw();

  // Ratio Plot settings -------------------------------------------------------
  h3->SetTitle(""); // Remove the ratio title

  // Y axis
  h3->GetYaxis()->SetTitle("ratio h1/h2 ");
  h3->GetYaxis()->SetTitleSize(20);
  h3->GetYaxis()->SetTitleFont(43);
  h3->GetYaxis()->SetTitleOffset(1.55);
  h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetYaxis()->SetLabelSize(15);

  // X axis
  h3->GetXaxis()->SetTitle("p_{T}^{#mu#mu} (GeV)");
  h3->GetXaxis()->SetTitleSize(20);
  h3->GetXaxis()->SetTitleFont(43);
  h3->GetXaxis()->SetTitleOffset(4.);
  h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetXaxis()->SetLabelSize(15);
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  


  // Guardamos la figura como .png y .pdf
  c->Print("DiMuPt.png", "png");
}


void simpleHisto()
{

  TString path = "./practica/files/";

  // init Histos
  TH1F * hDY = new TH1F("dimu pT DYw", "dos histogramas potentes", 16, 0, 160); 
  TH1F * htt = new TH1F("dimu pT tt", "", 16, 0, 160);


  // Fill DY tree
  TFile f(path + "dy.root");
  TTree * dyTree = (TTree *) f.Get("events");
  fillHistosFromTree(dyTree, hDY);


  // Fill tt tree
  TFile f2(path + "ttbar.root");
  TTree * ttTree = (TTree *) f2.Get("events");
  fillHistosFromTree(ttTree, htt);

  // Plot Everything
  plotWithRatio(hDY, htt);
}
