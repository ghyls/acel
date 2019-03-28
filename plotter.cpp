
#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"




Plotter::Plotter(std::vector<std::string> bkgs, std::string _pathToFiles,  
                 std::string _data)
{

  LegendTextSize  = 0.030;
  fLegX1 = 0.70, fLegY1 = 0.55, fLegX2 = 0.92, fLegY2 = 0.92;


  data = _data;
  pathToFiles = _pathToFiles;
  listOfSelectors = {}; // initialization!

  //bkgSelector = new Selector(pathToFiles, bkgs[0]);
  for (unsigned int i = 0; i < bkgs.size(); i++)
  {
    bkgSelector = new Selector(pathToFiles, bkgs[i]);
    listOfSelectors.push_back(bkgSelector); 
    listOfColors.push_back(i+1);
  }
  
  if (_data != "")
  {
    dataSelector = new Selector(pathToFiles, _data);
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
/*

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
*/

void Plotter::SetTitles(TString _title, TString _xtitle, TString _ytitle, 
                        TString _outName)
{
  title = _title;
  xtitle = _xtitle;
  ytitle = _ytitle;
  outName = _outName;
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
void Plotter::PrintXSecData()
// prints every ingredient for calculating the Xsec with its uncertainity, so we
// can import it from python, an then propagate the errors. Also prints in the
// command line the central value of the cross section. Fuck yeah.
{
  float totalMC = 0;
  float totalData = 0;
  float totalTTbar = 0;

  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = listOfSelectors[i]->GetHisto("TempXSec");

    totalMC +=h->Integral();
    if (listOfSelectors[i]->process == "ttbar")
    {
      totalTTbar += h->Integral();
    }
  }

  TH1F* h = dataSelector->GetHisto("TempXSec");
  totalData += h->Integral();

  //std::cout << totalMC << ' ' << totalData << ' ' << totalTTbar << std::endl;

  float totalSignal = totalData - (totalMC - totalTTbar);

  std::cout << totalSignal << ' ' << totalTTbar << ' ' << (totalMC - totalTTbar) << std::endl;

  //std::cout << totalSignal << std::endl;
  //http://pdglive.lbl.gov/Particle.action?node=Q007&init=0

  float sigmaTeo = 165;

  std::cout << "\n\n================================" << std::endl;

  std::vector<double> integralsBTag = GetBTagEff();
  std::vector<double> integralsTEff = GetTriggerEff();
  std::vector<double> integralsAcep = GetAcceptance();

  float bTagEff = integralsBTag[0]/integralsBTag[1];
  float triggEff = integralsTEff[0]/integralsTEff[1];
  float muonEff = 0.99;   // pm 0.01
  float lumi = 50;        // pb, pm 10%

  //float acep = 2858. / 3500.;
  
  float acep = integralsAcep[0]/(integralsAcep[1]*BR);

  //std::cout << totalSignal/totalTTbar << std::endl;

  float eff = triggEff * muonEff * bTagEff;
  //acep = totalTTbar / (eff * lumi * BR * sigmaTeo);
  float acep2 = totalTTbar / (eff * lumi * BR * sigmaTeo);
  std::cout << acep2 << std::endl;
  float sigma = totalSignal / (lumi * acep * eff * BR);
  std::cout << "Cross Section: " << sigma << std::endl;        
  std::cout << "================================" << std::endl;
  

  std::ofstream f;
  f.open("./PyTools/xsec.dat");
  f << "# this is an auto-generated file. You better don't edit it \n\n";
  f << "# Btag, Teff, Acep; [num, denom]\n" << std::endl;

  f << integralsBTag[0] << ' ' << integralsBTag[1] << std::endl;
  f << integralsTEff[0] << ' ' << integralsTEff[1] << std::endl;
  f << integralsAcep[0] << ' ' << integralsAcep[1] << std::endl;
  f << totalData << ' ' << totalMC - totalTTbar << std::endl;
  f.close();
}

std::vector<double> Plotter::GetTriggerEff()
{
  TH1F* h1;
  TH1F* h2;

  for (int i = 0; i < listOfSelectors.size(); i++)
  {
    if (listOfSelectors[i]->process == "ttbar")
    {
      h1 = listOfSelectors[i]->GetHisto("MuonPt_TriggOnly");
      h2 = listOfSelectors[i]->GetHisto("MuonPt_raw");
    }
  }


  //tempSelector = new Selector(pathToFiles, "ttbar");
  //TH1F* h1 = tempSelector->GetHisto("MuonPt_TriggOnly");
  //TH1F* h2 = tempSelector->GetHisto("MuonPt_raw");

  //listOfSelectors.push_back(bkgSelector); 


  Int_t nbins = h1->GetNbinsX();
  std::vector <float> eff = {};
  
  for (int i = 0; i < nbins; i++)
  {
    eff.push_back(h1->Integral(i, nbins)/h2->Integral(i, nbins));
    //std::cout << eff[i] << std::endl; 
  }

  // find the index of the element coorresponding to the max eff.
  float max = *max_element(std::begin(eff), std::end(eff));
  for (int i = 0; i < nbins; i++)
  {
    if (eff[i] == max)
    {
      // returns the operation for the Trigg. Eff with the current Pt_min cut.
      std::cout << "Trigger eff: " << eff[i] << std::endl;  

      std::vector<double> integrals = \
                              {h1->Integral(i, nbins), h2->Integral(i, nbins)};
      return integrals;
    } 
  }
}
  

std::vector<double> Plotter::GetAcceptance()
{
  TH1F* h1;
  TH1F* h2;
  float acept;
  float ttbarReco;
  float ttbarGen;
  for (int i = 0; i < listOfSelectors.size(); i++)
  {
    if (listOfSelectors[i]->process == "ttbar")
    {
      h1 = listOfSelectors[i]->GetHisto("Acep_gen");
      h2 = listOfSelectors[i]->GetHisto("Acep_obs");
      ttbarReco = listOfSelectors[i]->ttbarReco;
      ttbarGen = listOfSelectors[i]->ttbarGen;
    }
  }

  //float acept = h2->Integral()/h1->Integral();

  std::cout << "Aceptance: " << ttbarReco/(ttbarGen*BR) << std::endl;
  
  //std::vector<double> integrals = {h1->Integral(), h2->Integral()};
  std::vector<double> integrals = {ttbarReco, ttbarGen};
  return integrals;
}

std::vector<double> Plotter::GetBTagEff()
// TODO: rename me
{
  TH1F* h1;
  TH1F* h2;
  float eff;
  float num;
  float den;
  for (int i = 0; i < listOfSelectors.size(); i++)
  {
    if (listOfSelectors[i]->process == "ttbar")
    {
      h1 = listOfSelectors[i]->GetHisto("BJet_Pt");
      h2 = listOfSelectors[i]->GetHisto("Jets_GEN_Pt");
      num = listOfSelectors[i]->bIdentAndMatched;
      den = listOfSelectors[i]->totalGenB;
    }
  }
  std::cout << "B tagging eff: " << num/den << std::endl;  

  //std::vector<double> integrals = {h1->Integral(), h2->Integral()};
  std::vector<double> integrals = {num, den};
  return integrals;
}

void Plotter::plotWithRatio(TString process, TString nameH1, TString nameH2, \
                TString rLabel, float rMin, float rMax, bool doLogY, float max)
{

  TH1F* h1;
  TH1F* h2;

  for (int i = 0; i < listOfSelectors.size(); i++)
  {
    if (listOfSelectors[i]->process == process)
    {
      h1 = listOfSelectors[i]->GetHisto(nameH1);
      h2 = listOfSelectors[i]->GetHisto(nameH2);
    }
  }

  //do Plots
  TCanvas *c = new TCanvas("c", "canvas", 800, 800);
  if (doLogY) {c->SetLogy(1);}
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
  h1->GetYaxis()->SetTitle(ytitle);
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
  c->Print(outName + ".png", "png");
  delete c;
}

void Plotter::DrawOverflowBin(TH1F* h)
{
  Float_t overflow = h->GetBinContent(h->GetNbinsX()) + \
                    h->GetBinContent(h->GetNbinsX() + 1);
  h->SetBinContent(h->GetNbinsX(), overflow);
}


template <typename S> 
TString returnFuckingName(S _name, TString process)
{
  std::cout << typeid(S).name() << std::endl;
  std::cout << typeid(process).name() << std::endl;
  
  if (typeid(S) == typeid(std::vector<TString>)) // is string
  {
    std::cout << "hassssssssssssdasd" << std::endl;
    return process;
  }
  else if (typeid(S) == typeid(const char*)) // is string
  {
    std::cout << "hasdasd" << std::endl;
    return _name;
  }
}

//template  void Plotter::Stack(char const*, TString, 
                        //std::vector<TString>, bool, Float_t);
//template  void Plotter::Stack(std::vector <TString>, TString, bool, Float_t);

//template <typename T>




void Plotter::Stack(TString name, TString process, bool drawRatios, std::vector<TString> histoNames, \
                    bool doLogY, Float_t maxY)
{
  
  float minPad1;
  if (drawRatios)
  {
    minPad1 = 0.3;
  }
  else
  {
    minPad1 = 0;
  }

  TCanvas *c = new TCanvas("c", "canvas", 1000, 800);


  //name = returnFuckingName(_name, process);


  TH1F* hdata;
  
  THStack *hs = new THStack("hstack_" + name, "hstack");


  TPad *pad1 = new TPad("pad1", "pad1", 0, minPad1, 1, 1);
  if (doLogY) {pad1->SetLogy(1);}

  pad1->SetBottomMargin(0.05); 
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad    
  TLegend leg = TLegend(fLegX1, fLegY1, fLegX2, fLegY2); 
  leg.SetTextSize(LegendTextSize);
  leg.SetBorderSize(0);
  leg.SetFillColor(10);
  



  // if we want to stack different histos from the SAME process
  if (process == "")// if we want to stack the same histo for DIFERENT processes
  {
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
  }
  else
  {
    for (unsigned int j = 0; j < listOfSelectors.size(); j++)
    {
      if (listOfSelectors[j]->process != process) {continue;}
      
      for (unsigned int i = 0; i < histoNames.size(); i++)
      {

        TH1F* h = listOfSelectors[j]->GetHisto(TString(histoNames[i]));
        h->SetFillColor(listOfColors[i]);
        h->SetLineColor(kBlack);
        DrawOverflowBin(h);
        hs->Add(h);
        leg.AddEntry(h, histoNames[i] + Form(": %1.0f", \
                      h->Integral()), "f");
      }
    }
  }

  pad1->cd();
  hs->Draw("hist");
  if (title  != "") {hs->SetTitle(title);}
  if (xtitle != "") {hs->GetXaxis()->SetTitle(xtitle);}
  if (ytitle != "") {hs->GetYaxis()->SetTitle(ytitle);}
  hs->GetYaxis()->SetTitleOffset(1.5);

  TH1 *aux = ((TH1*)(hs->GetStack()->Last()));
  Float_t max = aux->GetMaximum(); // why do I have to do this in 2 lines?
  
  if (data != "" && process == "")
  {
    hdata = dataSelector->GetHisto(name); 
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerColor(1); // 1
    DrawOverflowBin(hdata);
    hdata->Draw("pesame");

    Float_t maxData = hdata->GetMaximum();
    if (max < maxData) {max = maxData;}

    leg.AddEntry(hdata, dataSelector->process + Form(": %1.0f", \
                  hdata->Integral()), "p");
  }

  
  if (drawRatios && process == "")
  {


    c->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    TH1F *h3 = (TH1F*)hdata->Clone("h3");
    h3->SetLineColor(kBlack);
    h3->SetMinimum(0);
    h3->SetMaximum(2);

    h3->GetYaxis()->SetTitle("Data/MC");
    h3->GetYaxis()->SetTitleSize(20);
    h3->GetYaxis()->SetTitleFont(43);
    h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(15);


    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(aux);       // ~ h1/h2
    h3->SetMarkerStyle(21);
    h3->Draw("ep");       // Draw the ratio plot
  }
  
  pad1->cd(); leg.Draw("same");
  if (maxY == -1){hs->SetMaximum(max*1.1);}
  else{hs->SetMaximum(maxY);}
  c->Print(outName + ".png", "png");

  delete c, hs;
  
}

