
#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"





Plotter::Plotter(std::vector<std::string> bkgs, std::string pathToFiles,  
std::string prefixForHistos, std::string data_)
{

  std::cout <<"[plotter] NOTE. Hello!" << std::endl;

  LegendTextSize  = 0.030;
  fLegX1 = 0.70, fLegY1 = 0.55, fLegX2 = 0.92, fLegY2 = 0.92;


  data = data_;
  listOfSelectors = {}; // initialization!

  std::cout <<"HELLO " << data_ << " " << pathToFiles << std::endl;
  for (unsigned int i = 0; i < bkgs.size(); i++)
  {
    Selector sel(pathToFiles, bkgs[i], prefixForHistos);
    listOfSelectors.push_back(sel); 
    listOfColors.push_back(i+1);
  }
  
  if (data_ != "")
  {

    std::cout <<"hi :D" << std::endl;
    //Selector *dataS = new Selector(pathToFiles, data_, prefixForHistos);
    dataSelector = new Selector(pathToFiles, data_, prefixForHistos);
    //Selector dataSelector(pathToFiles, data_, prefixForHistos);
  }
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
    int numberOfHistos = listOfSelectors[0].GetNumberOfHistos();

    for (int i = 0; i < numberOfSelectors; i++)
    {
      for (int j = 0; i < numberOfHistos; i++)
      {
        TString n = listOfSelectors[i].histograms[j]->GetName(); //full name

        if (!std::string(n).find(std::string(name))){continue;}
        //si no encuentras el histo, sigue adelante
        return listOfSelectors[i].histograms[j];
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
  std::cout << "---------------------------------------------" << std::endl;

  Int_t totalEvts = 0;

  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = listOfSelectors[i].GetHisto(name);

    std::cout << name << ": " << h->Integral();
    totalEvts += h->Integral();
  }

  std::cout << "Total expected: " << totalEvts << std::endl;
  std::cout << "--------------------" << std::endl;

  if (data != "")
  {
    TH1F* h = dataSelector->GetHisto(name);
    std::cout << "Observed: " << h->Integral() << std::endl;
    std::cout << "--------------------" << std::endl;
  }
}


void Plotter::Stack(TString name)
{
  // name is NOT a list
      
  TCanvas *c = new TCanvas("c", "canvas", 800, 800);
  TLegend leg = TLegend(fLegX1, fLegY1, fLegX2, fLegY2); 
  leg.SetTextSize(LegendTextSize);
  leg.SetBorderSize(0);
  leg.SetFillColor(10);

  THStack *hs = new THStack("hstack_" + name, "hstack");
      
  // fill hstack >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = GetHisto(listOfSelectors[i].histoName); // TODO: DON'T MAKE ME LAUGH
    std::cout << h->Integral() << std::endl;

    h->SetFillColor(listOfColors[i]);
    h->SetLineColor(0);
    hs->Add(h);
    leg.AddEntry(h, listOfSelectors[i].histoName, "hstack");
  }

  hs->Draw("hist");

  if (title  != "") {hs->SetTitle(title);}
  if (xtitle != "") {hs->GetXaxis()->SetTitle(xtitle);}
  if (ytitle != "") {hs->GetYaxis()->SetTitle(ytitle);}
  hs->GetYaxis()->SetTitleOffset(1.35);

  TH1 *aux = ((TH1*)(hs->GetStack()->Last()));
  Float_t max = aux->GetMaximum(); // why do I have to do this in 2 lines?
  /*
  if (data != "")
  {
    TH1F* hdata = dataSelector->GetHisto(name); 
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerColor(kTeal+1); // 1
    hdata->Draw("pesame");                      //これは何か 笑

    Float_t maxData = hdata->GetMaximum();
    if (max < maxData) {max = maxData;}

    leg.AddEntry(hdata, dataSelector->prefix, "p");
  }
  */
  leg.Draw("same");
  hs->SetMaximum(max*1.1);
  c->Print(name + ".png", "png");
}

