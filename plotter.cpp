
#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"




Plotter::Plotter(std::vector<std::string> bkgs, std::string pathToFiles,  
                 std::string data_)
{

  LegendTextSize  = 0.030;
  fLegX1 = 0.70, fLegY1 = 0.55, fLegX2 = 0.92, fLegY2 = 0.92;


  data = data_;
  listOfSelectors = {}; // initialization!

  //bkgSelector = new Selector(pathToFiles, bkgs[0]);
  for (unsigned int i = 0; i < bkgs.size(); i++)
  {
    bkgSelector = new Selector(pathToFiles, bkgs[i]);
    listOfSelectors.push_back(bkgSelector); 
    listOfColors.push_back(i+1);
  }
  
  if (data_ != "")
  {
    dataSelector = new Selector(pathToFiles, data_);
  }
}

Plotter::~Plotter()
{
  if (data != "") {delete dataSelector;} //else: it does not exist
  delete bkgSelector;
  for (int i = 0; i < listOfSelectors.size()-1; i++)
  {
    delete listOfSelectors[i];
  }

  std::cout << "destroyed Plotter\n" << std::endl;
}

void Plotter::SetLegendPos(Float_t x1, Float_t y1, Float_t x2, Float_t y2)
{
  fLegX1 = x1; 
  fLegX2 = x2; 
  fLegY1 = y1; 
  fLegY2 = y2; 
}

    
void Plotter::SetLegendSize(Float_t size)
{
  LegendTextSize = size;
}


void Plotter::SetColors(std::vector<Int_t> col)
{
  listOfColors = col;
}


void Plotter::SetTitle(TString _title)
{
  title = _title;
}

    
void Plotter::SetXTitle(TString _xtitle)
{
  xtitle = _xtitle;
}


void Plotter::SetYTitle(TString _ytitle)
{
  ytitle = _ytitle;
}


TH1F* Plotter::GetHisto(TString name) //name is the name of the histo
{
    //TODO: I don't work for a specific process!
    //TODO: Number of histos could depend on the process (?)
    //FIXME: Algo me dice que esto solo devuelve la primera coincidencia.
        
    int numberOfSelectors = listOfSelectors.size();
    std::cout <<"plott, gethisto, N_SEL: " << numberOfSelectors << std::endl;
    int numberOfHistos = listOfSelectors[0]->GetNumberOfHistos();

    for (int i = 0; i < numberOfSelectors; i++)
    {
      for (int j = 0; j < numberOfHistos; j++)
      {
        TString n = listOfSelectors[i]->histograms[j]->GetName(); //full name
        std::cout << "name: " << n << std::endl;
        if (!std::string(n).find(std::string(name))){continue;}
        //si no encuentras el histo, sigue adelante
        return listOfSelectors[i]->histograms[j];
      }
    }
    throw;
}

int Plotter::GetEvents(TString name)
{
  // Returns the integral of a histogram
  return GetHisto(name)->Integral();
}

void Plotter::PrintEvents(TString name)
{
  // Name is a name. Not a list or any other weird thing.

  std::cout << "\nPrinting number of events for histogram " << name << "\n";
  std::cout << "-------------------------------------------------" << std::endl;

  Int_t totalEvts = 0;

  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = listOfSelectors[i]->GetHisto(name);

    std::cout << listOfSelectors[i]->process <<": "<< h->Integral() << std::endl;
    totalEvts += h->Integral();
  }

  std::cout << "Total expected: " << totalEvts << std::endl;
  std::cout << "------------------------" << std::endl;

  if (data != "")
  {
    TH1F* h = dataSelector->GetHisto(name);
    std::cout << "Observed: " << h->Integral() << std::endl;
    std::cout << "------------------------" << std::endl;
  }
}


void Plotter::GetTriggerEff()
{
  TH1F* h1 = listOfSelectors[0]->GetHisto("MuonPt");
  TH1F* h2 = listOfSelectors[0]->GetHisto("MuonPt_raw");

  Int_t nbins = h1->GetNbinsX();
  float eff[nbins];
  
  for (int i = 0; i < nbins; i++)
  {
    eff[i] = h1->Integral(i, nbins)/h2->Integral(i, nbins);
    std::cout << eff[i] << std::endl;
  }
}

void Plotter::plotWithRatio(TString nameH1, TString nameH2, TString rLabel, \
                            float rMin, float rMax, float max)
{

  TH1F* h1 = listOfSelectors[0]->GetHisto(nameH1);
  TH1F* h2 = listOfSelectors[0]->GetHisto(nameH2);

  //do Plots
  TCanvas *c = new TCanvas("c", "canvas", 800, 800);

  // Normalizamos los histogramas a 1
  // h1->Scale(1/h1->Integral());
  // h2->Scale(1/h2->Integral());


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

  Float_t maxData = std::max(h1->GetMaximum(), h2->GetMaximum());
  Float_t minData = std::min(h1->GetMinimum(), h2->GetMinimum());
  h1->SetMaximum(maxData *1.1);
  h1->SetMinimum(0);  
  if (max != 999) {h1->SetMaximum(max);}    // .. range
  
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
  h3->SetMinimum(rMin);    // Define Y ..
  h3->SetMaximum(rMax);    // .. range
  //h3->Sumw2();
  h3->SetStats(0);      // No statistics on lower plot
  h3->Divide(h2);       // ~ h1/h2
  h3->SetMarkerStyle(21);
  h3->Draw("ep");       // Draw the ratio plot

  TLine *l = new TLine(10, 1.0, 120, 1.0);  // horizontal line
  l->SetLineColor(kBlue);
  l->Draw();
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  
  // Cosmetics >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  h1->SetLineColor(kAzure+2);
  h2->SetLineColor(kRed+1);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);

  // Ejes y otras opciones... (solo en el primer histograma que pintamos)
  h1->SetStats(0);
  h1->SetTitle(title);
  h1->GetYaxis()->SetTitle("Events");
  h1->GetYaxis()->SetTitleOffset(1.2);
  h1->GetYaxis()->SetLabelFont(43);
  h1->GetYaxis()->SetLabelSize(15);
  h1->GetXaxis()->SetLabelFont(43);
  h1->GetXaxis()->SetLabelSize(15);

  DrawOverflowBin(h1);
  DrawOverflowBin(h2);




  // Leyenda
  pad1->cd();  
  TLegend leg = TLegend(fLegX1, fLegY1, fLegX2, fLegY2); // x0, y0, x1, y1
  leg.AddEntry(h1, nameH1 + Form(": %1.0f", h1->Integral()), "l");
  leg.AddEntry(h2, nameH2 + Form(": %1.0f", h2->Integral()), "l");
  leg.SetTextSize(0.043);
  leg.Draw();

  // Ratio Plot settings -------------------------------------------------------
  h3->SetTitle(""); // Remove the ratio title

  // Y axis
  h3->GetYaxis()->SetTitle(rLabel);
  h3->GetYaxis()->SetTitleSize(20);
  h3->GetYaxis()->SetTitleFont(43);
  h3->GetYaxis()->SetTitleOffset(1.55);
  h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetYaxis()->SetLabelSize(15);

  // X axis
  h3->GetXaxis()->SetTitle(xtitle);
  h3->GetXaxis()->SetTitleSize(20);
  h3->GetXaxis()->SetTitleFont(43);
  h3->GetXaxis()->SetTitleOffset(4.);
  h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  h3->GetXaxis()->SetLabelSize(15);
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  


  // Guardamos la figura como .png y .pdf
  c->Print("DiMuPt.png", "png");
}

void Plotter::DrawOverflowBin(TH1F* h)
{
  Float_t overflow = h->GetBinContent(h->GetNbinsX()) + \
                    h->GetBinContent(h->GetNbinsX() + 1);
  h->SetBinContent(h->GetNbinsX(), overflow);
}

void Plotter::Stack(TString name, Float_t maxY)
{
  // name is NOT a list
      
  TCanvas *c = new TCanvas("c", "canvas", 1000, 800);
  TLegend leg = TLegend(fLegX1, fLegY1, fLegX2, fLegY2); 
  leg.SetTextSize(LegendTextSize);
  leg.SetBorderSize(0);
  leg.SetFillColor(10);

  THStack *hs = new THStack("hstack_" + name, "hstack");
      
  // fill hstack >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = listOfSelectors[i]->GetHisto(name); // TODO: DON'T MAKE ME LAUGH
    
    //std::cout << "name: " << h->GetName() << std::endl;
    //std::cout << "integral " << h->Integral() << std::endl;
  
    h->SetFillColor(listOfColors[i]);
    h->SetLineColor(kBlack);
    DrawOverflowBin(h);
    hs->Add(h);
    leg.AddEntry(h, listOfSelectors[i]->process + Form(": %1.0f", \
                  h->Integral()), "f");
  }

  hs->Draw("hist");
  if (title  != "") {hs->SetTitle(title);}
  if (xtitle != "") {hs->GetXaxis()->SetTitle(xtitle);}
  if (ytitle != "") {hs->GetYaxis()->SetTitle(ytitle);}
  hs->GetYaxis()->SetTitleOffset(1.5);

  TH1 *aux = ((TH1*)(hs->GetStack()->Last()));
  Float_t max = aux->GetMaximum(); // why do I have to do this in 2 lines?
  
  if (data != "")
  {
    TH1F* hdata = dataSelector->GetHisto(name); 
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerColor(kTeal+1); // 1
    hdata->Draw("pesame");

    Float_t maxData = hdata->GetMaximum();
    if (max < maxData) {max = maxData;}

    leg.AddEntry(hdata, dataSelector->process + Form(": %1.0f", \
                  hdata->Integral()), "p");
  }
  
  leg.Draw("same");
  if (maxY == -1){hs->SetMaximum(max*1.1);}
  else{hs->SetMaximum(maxY);}
  c->Print(name + ".png", "png");

  delete c, hs;
}

