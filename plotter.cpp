
#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TLine.h"
#include "TLatex.h"




Plotter::Plotter(std::vector<std::string> bkgs, std::string _pathToFiles,  
                 std::string _data)
{

  LegendTextSize  = 0.030;
  fLegX1 = 0.70, fLegY1 = 0.55, fLegX2 = 0.82, fLegY2 = 0.92;


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
      if (name == listOfSelectors[i]->process)
      {
        return listOfSelectors[i]->GetHisto(name);
      }
    }
    std::cout << "What a Terrible Failure. Kill yourself or I will do the job.";
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

    //std::cout << listOfSelectors[i]->process <<": "<< h->Integral() << std::endl;
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
  float totalBkgNormUnc = 0;
  float totalBkgStatUnc = 0;
  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {
    TH1F* h = listOfSelectors[i]->GetHisto("TempXSec");

    // fill totalMC
    totalMC +=h->Integral();
    
    // fill Stat uncertainity (only for bKg)
    if (listOfSelectors[i]->process != "ttbar"){
      totalBkgStatUnc += h->GetBinError(1);
      //std::cout <<"bkg unc stat" << totalBkgStatUnc << std::endl;
      //std::cout <<listOfSelectors[i]->process << h->GetBinError(1) << std::endl;
    }

    // fill Norm Unc ( only for Bkg)                             Python Style xd
    if (listOfSelectors[i]->process == "qcd")                                  {
      totalBkgNormUnc += h->Integral() * 1                                    ;} 
    if (listOfSelectors[i]->process == "wjets")                                {
      totalBkgNormUnc += h->Integral() * 0.5                                  ;} 
    if (listOfSelectors[i]->process == "ww")                                   {
      totalBkgNormUnc += h->Integral() * 0.5                                  ;} 
    if (listOfSelectors[i]->process == "wz")                                   {
      totalBkgNormUnc += h->Integral() * 0.5                                  ;} 
    if (listOfSelectors[i]->process == "zz")                                   {
      totalBkgNormUnc += h->Integral() * 0.5                                  ;} 
    if (listOfSelectors[i]->process == "dy")                                   {
      totalBkgNormUnc += h->Integral() * 0.15                                 ;} 
    if (listOfSelectors[i]->process == "single_top")                           {
      totalBkgNormUnc += h->Integral() * 0.3                                  ;}    

    // fill ttbar histo (sigmal) 
    if (listOfSelectors[i]->process == "ttbar")
    {
      totalTTbar += h->Integral();
    }
  }

  TH1F* h = dataSelector->GetHisto("TempXSec");
  totalData += h->Integral();

  //std::cout << totalMC << ' ' << totalData << ' ' << totalTTbar << std::endl;

  float totalSignal = totalData - (totalMC - totalTTbar);

  //std::cout << totalSignal << ' ' << totalTTbar << ' ' << (totalMC - totalTTbar) << std::endl;

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
  std::cout << totalSignal << std::endl;
  float sigma = totalSignal / (lumi * acep2 * eff * BR);
  std::cout << "Cross Section: " << sigma << std::endl;        
  std::cout << "================================" << std::endl;
  

  std::ofstream f;
  f.open("./PyTools/xsec.dat");
  f << "# this is an auto-generated file. You better don't edit it \n";
  f << "# [BTag0    BTag1]" << std::endl;
  f << "# [Teff0    Teff1]" << std::endl;
  f << "# [Acep0    Acep1]" << std::endl;
  f << "# [Ndata    Nbkg ]" << std::endl;
  f << "# [bkgNormUnc    bkgStatUnc]\n" << std::endl;
  f << "# [AcepCentral   0]\n" << std::endl;

  f << integralsBTag[0] << ' ' << integralsBTag[1] << std::endl;
  f << integralsTEff[0] << ' ' << integralsTEff[1] << std::endl;
  f << integralsAcep[0] << ' ' << integralsAcep[1] << std::endl;
  f << totalData << ' ' << totalMC - totalTTbar << std::endl;
  f << totalBkgNormUnc << " " << totalBkgStatUnc << std::endl;
  f << acep2 << " 0" << std::endl;
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
  
  for (int i = 1; i < nbins+1; i++)
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
      float err1 = h1->GetBinError(1);
      std::cout << h1->Integral() <<" hi "<< err1 << std::endl;
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
      num = listOfSelectors[i]->bIdentAndMatched * 0.9;
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
  TCanvas *c = new TCanvas("c", "canvas", 1000, 1000);
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
  leg.AddEntry(h1, nameH1 + Form(": %1.0f", h1->Integral()), "f");
  leg.AddEntry(h2, nameH2 + Form(": %1.0f", h2->Integral()), "f");
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

void Plotter::PrintGaussianFit(TH1F * histo)
{
  TF1 * fit = new TF1("f1", "gaus");
  
  //TFitResultPtr fitData = histo->Fit(fit, "N");
  
  //TFitResultPtr fitData = histo->Fit(fit, "Q, N");
  TFitResultPtr fitData = histo->Fit(fit, "N");
  Double_t p1 = fit->GetParameter(1);
  Double_t e1 = fit->GetParError(1);
  std::cout << p1 << " pm " << e1 << std::endl;
}

void Plotter::AddNormUnc(TH1F * hTarget, TString name, TString proc, 
                                                    float perc, float * hBinSys)
{
  // hTarget is a histo with the same bining as hSource
  TH1F* hSource;
  for (unsigned int i = 0; i < listOfSelectors.size(); i++)
  {    
    if (listOfSelectors[i]->process == proc)
      {hSource = listOfSelectors[i]->GetHisto(name);}
  }

  for (int j = 1; j < hTarget->GetNbinsX() + 1; j++) 
  { // all bins except the last
  //std::cout << "I'm here " <<hTarget->GetBinContent(j) <<  std::endl;
    if (j == hTarget->GetNbinsX())
    {
      Float_t overflow = hSource->GetBinContent(hSource->GetNbinsX()) + \
          hSource->GetBinContent(hSource->GetNbinsX() + 1);
      hBinSys[j-1] += overflow * perc;
    }
    else hBinSys[j-1] += hSource->GetBinContent(j) * perc;
  }
}

void Plotter::AddToLegend(TLegend * leg, TH1F * h, TString name, float Integral)
{

  //  M A X I M U M   H A R D C O D I N G

  if ((int)Integral > 0)
  {

    if (name != "data")
    {
      if (name == "wjets") {name = "W + jets";}
      if (name == "ttbar") {name = "TTbar";}
      if (name == "single_top") {name = "Single Top";}
      if (name == "dy") {name = "Drell Yan";}
      
      // define some groups here...
      if (name == "wjets") {name = "W + jets";}
      if (name == "ttbar") {name = "TTbar";}
      if (name == "single_top") {name = "Single Top";}
      if (name == "dy") {name = "Drell Yan";}
      leg->AddEntry(h, name + Form(": %1.0f", Integral), "f");
    }
    else
    {
      leg->AddEntry(h, name + Form(": %1.0f", Integral), "p");
    }
  }
  else if (Integral > 0)
  {
    leg->AddEntry(h, name + ": < 1", "f");
  }
  
}

void Plotter::Stack(TString name, TString process, bool drawRatios, 
  TString options, std::vector<TString> histoNames, bool doLogY, Float_t maxY)
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

  TCanvas *c = new TCanvas("c", "canvas", 1000, 1000);


  //name = returnFuckingName(_name, process);


  TH1F* hdata;
  
  THStack *hs = new THStack("hstack_" + name, "");


  TPad *pad1 = new TPad("pad1", "pad1", 0, minPad1, 1, 1);
  if (doLogY) {pad1->SetLogy(1);}

  if (drawRatios) pad1->SetBottomMargin(0.01); 
  pad1->SetRightMargin(0.06); 
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad    
  TLegend * leg = new TLegend(fLegX1, fLegY1, fLegX2, fLegY2); 
  leg->SetTextSize(LegendTextSize);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);

  
  // ESTOS TRES HISTOGRAMAS SE usarán para calcular la normalización y ponerla
  // en el plot. Ojalá la esté calculando bien. Con el ratio plot ya te pelearás
  // luego

  TH1F* hSignal;
  TH1F* hBkg;
  TH1F* hTotal;
  float hBinSys[listOfSelectors.size()];
  std::fill_n(hBinSys, listOfSelectors.size(), 0);
  if (process == "" && data != "") //fill hBkg && hSignal
  {
    
    hdata = dataSelector->GetHisto(name); 
    hSignal = (TH1F*)hdata->Clone("hSignal");
    hBkg = (TH1F*)hdata->Clone("hBkg");

    for (int j = 1; j < hdata->GetNbinsX() +1; j++) // empty histos
    {
      hSignal->SetBinContent(j, 0);
      hBkg->SetBinContent(j, 0);
    }


    for (unsigned int i = 0; i < listOfSelectors.size(); i++)
    { // Actually fill them
      TH1F* h = listOfSelectors[i]->GetHisto(name);
      if (listOfSelectors[i]->process != "ttbar")
      {
        for (int j = 1; j < h->GetNbinsX(); j++)
        {
          hBkg->SetBinContent(j, hBkg->GetBinContent(j) + h->GetBinContent(j));
        }
        hBkg->SetBinContent(h->GetNbinsX(),hBkg->GetBinContent(hBkg->GetNbinsX()) + h->GetBinContent(h->GetNbinsX()) + \
                h->GetBinContent(h->GetNbinsX() + 1));
      }
      else
      {
        for (int j = 1; j < h->GetNbinsX(); j++)
        {
          hSignal->SetBinContent(j, hSignal->GetBinContent(j) + h->GetBinContent(j));
        }
        hSignal->SetBinContent(h->GetNbinsX(),hSignal->GetBinContent(hSignal->GetNbinsX()) + h->GetBinContent(h->GetNbinsX()) + \
          h->GetBinContent(h->GetNbinsX() + 1));
      }

    }
    hTotal = (TH1F*)hSignal->Clone("hTotal");
    hTotal->Add(hBkg);

    // Add Sys erorrs and read the statistical ones
    if (name == "MuonPt") // o si es tempXsec!
    {
      // Now we read and store the stat errors

      AddNormUnc(hTotal, name, "qcd", 1, hBinSys);
      AddNormUnc(hTotal, name, "wjets", 0.5, hBinSys);
      AddNormUnc(hTotal, name, "ww", 0.5, hBinSys);
      AddNormUnc(hTotal, name, "wz", 0.5, hBinSys);
      AddNormUnc(hTotal, name, "zz", 0.5, hBinSys);
      AddNormUnc(hTotal, name, "dy", 0.15, hBinSys);
      AddNormUnc(hTotal, name, "single_top", 0.3, hBinSys);

      for (int j = 1; j < hTotal->GetNbinsX() + 1; j++)
      {
        //std::cout << hBinSys[j-1] << std::endl;
        hTotal->SetBinError(j, hBinSys[j-1]);
      }
    }
  }


  // if we want to stack different histos from the SAME process
  if (process == "")// if we want to stack the same histo for DIFERENT processes
  {
    nEventsVV = 0;
    // create an empty auxiliar histo for the VV process
    TH1F* hVV = (TH1F*)hTotal->Clone("hVV"); 
    hVV->SetFillColor(kBlack);
    //hVV->SetLineColor(kBlack);
    hVV->SetLineWidth(0);

    for (int j = 1; j < hVV->GetNbinsX() + 1; j++)
    {
      hVV->SetBinContent(j, 0);
    }
    TH1F* hTTbar; 

    for (unsigned int i = 0; i < listOfSelectors.size(); i++)
    {
      TH1F* h = listOfSelectors[i]->GetHisto(name); // TODO: DON'T MAKE ME LAUGH
      
      //std::cout << "name: " << h->GetName() << std::endl;
      //std::cout << "integral " << h->Integral() << std::endl;
    
      h->SetFillColor(listOfColors[i]);
      h->SetLineColor(kBlack);
      h->SetLineWidth(0);
      DrawOverflowBin(h);
        
      if (listOfSelectors[i]->process == "ttbar")
      {
        hTTbar = (TH1F*)h->Clone("hTTbar");  
      }
      else if (h->Integral() < 10)
      {
        if (listOfSelectors[i]->process == "ww" || 
            listOfSelectors[i]->process == "wz" || 
            listOfSelectors[i]->process == "zz") 
        {
          hVV->Add(h);
          nEventsVV += h->Integral();
        }
        else
        {
          hs->Add(h);
          AddToLegend(leg, h, listOfSelectors[i]->process, h->Integral());
        }
        
      }
      else if (h->Integral() >= 10)
      {
        hs->Add(h);
        AddToLegend(leg, h, listOfSelectors[i]->process, h->Integral());
      } 
      
      //leg->AddEntry(h, listOfSelectors[i]->process + Form(": %1.0f", \
      //              h->Integral()), "f");
      
    }
    if (nEventsVV > 0)
    {
      hs->Add(hVV);
      AddToLegend(leg, hVV, "VV", nEventsVV);
    }
    hs->Add(hTTbar);
    AddToLegend(leg, hTTbar, "ttbar", hTTbar->Integral()); 
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
        //h->SetLineColor(kBlack);
        h->SetLineWidth(0);
        DrawOverflowBin(h);
        hs->Add(h);
        leg->AddEntry(h, histoNames[i] + Form(": %1.0f", \
                      h->Integral()), "f");
        //AddToLegend(leg, h, histoNames[i], h->Integral());
      }
    }
  }

  //h->SetFillStyle(3444);
  //h->SetBinError(0, 200);
  //h->Draw("e2");



  pad1->cd();

  if (!drawRatios)
  {
    pad1->SetLeftMargin(0.12);
    pad1->SetRightMargin(0.06);
  }



  hs->Draw("hist");
  if (process == "")
  {
    TLatex* texLumi = new TLatex(0, 0, 
                          Form("50 pb^{-1}, #sqrt{s} = 7 TeV"));
    texLumi->SetTextAlign(12);
    texLumi->SetNDC(1);
    texLumi->SetX(0.72); // 15
    texLumi->SetY(0.93); // 89
    texLumi->SetTextSize(0.04);
    texLumi->SetTextSizePixels(10); // 22
    if (!drawRatios)
    {
    texLumi->SetX(0.68); // 15
    texLumi->SetY(0.92); // 89   
    texLumi->SetTextSize(0.032);
    }
    texLumi->Draw("same");

    TLatex* texCuts = new TLatex(0, 0, 
                          Form("N_{good jets} >= 4"));
    texCuts->SetTextAlign(12);
    texCuts->SetNDC(1);
    texCuts->SetX(0.13); // 15
    texCuts->SetY(0.93); // 89
    texCuts->SetTextSize(0.04);
    texCuts->SetTextSizePixels(10); // 22
    //texCuts->Draw("same");

    hTotal->SetFillStyle(3444);
    hTotal->SetFillColor(kGray+2);
    hTotal->Draw("same,e2");
  }

  if (scale != 999 && process == "")
  {
    //TH1F* hScaled;
    TH1F *hScaled = (TH1F*)hSignal->Clone("hScaled");
    std::cout << "potato" << std::endl;
    //hScaled->Add(hSignal, 6);
    hScaled->Scale(scale);
    hScaled->SetFillStyle(3244);
    hScaled->SetLineColor(kRed-7);
    hScaled->SetLineWidth(2);
    hScaled->SetFillColor(kGray +2);
    TString label = TString("TTbar * ") + (TString)std::to_string(scale);
    leg->AddEntry(hScaled, label , "f"); 
    hScaled->Draw("same, hist");
  }



  if (title  != "") {hs->SetTitle(title);}
  if (ytitle != "") {hs->GetYaxis()->SetTitle(ytitle);}

  TH1 *aux = ((TH1*)(hs->GetStack()->Last()));
  Float_t max = aux->GetMaximum(); // why do I have to do this in 2 lines?
  
  if (maxY == -1){hs->SetMaximum(max*1.1);}
  else{hs->SetMaximum(maxY);}
  hs->SetMinimum(0.0001);
  //hs->SetMinimum(0);
  hs->GetYaxis()->SetTitleSize(25);
  hs->GetYaxis()->SetTitleFont(43);
  hs->GetYaxis()->SetTitleOffset(2.1);
  if (drawRatios) {hs->GetXaxis()->SetLabelSize(0);}
  else
  {
    hs->GetYaxis()->SetTitleSize(30);
    hs->GetYaxis()->SetTitleOffset(2.1);
    hs->GetXaxis()->SetTitleOffset(1.5);
    hs->GetXaxis()->SetTitleSize(30);
    hs->GetXaxis()->SetTitle(xtitle);
    hs->GetXaxis()->SetTitleFont(43);
    //hs->GetXaxis()->SetLabelSize(20);
  }


  if (data != "" && process == "")
  {
    hdata = dataSelector->GetHisto(name); 
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerColor(1); // 1
    DrawOverflowBin(hdata);
    hdata->Draw("pesame");

    if (options.Contains("gauss"))
    {
      PrintGaussianFit(hdata);
    }

    Float_t maxData = hdata->GetMaximum();
    if (max < maxData) {max = maxData;}

    //leg->AddEntry(hdata, dataSelector->process + Form(": %1.0f", \
    //              hdata->Integral()), "p");
    AddToLegend(leg, hdata, dataSelector->process, hdata->Integral());
  }

  
  if (drawRatios && process == "")
  {
    c->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.2);
    pad2->SetRightMargin(0.06); 
    pad2->SetGridx(); // vertical grid
    pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    TH1F *h3 = (TH1F*)hdata->Clone("h3");
    //TH1F *h3 = hSignal;
    h3->SetLineColor(kBlack);
    h3->SetMinimum(0.51);
    //h3->SetMinimum(0.51);
    h3->SetMaximum(1.49);
    //h3->SetMaximum(2.19);

    
    TH1F * hratio = (TH1F*)hdata->Clone("hratio");
    for (int j = 1; j < hratio->GetNbinsX()+1; j++)
    {
      hratio->SetBinContent(j, 1);

      float relErr = (hTotal->GetBinContent(j) + hTotal->GetBinError(j))/
                    hTotal->GetBinContent(j) -1;
      hratio->SetBinError(j, relErr);
    }

    h3->GetYaxis()->SetTitle("Data/MC");
    //h3->GetYaxis()->SetTitle("Signal / Bkg");
    h3->GetYaxis()->SetTitleSize(25);
    h3->GetYaxis()->SetTitleFont(43);
    h3->GetYaxis()->SetTitleOffset(1.3);
    h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(18);
    if (xtitle != "") {h3->GetXaxis()->SetTitle(xtitle);}
    h3->GetXaxis()->SetTitleSize(25);
    h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(3.7);
    h3->GetXaxis()->SetLabelFont(43); 
    h3->GetXaxis()->SetLabelSize(20);



    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(aux);       // ~ h1/h2
    //h3->Divide(hBkg);       // ~ h1/h2
    h3->SetMarkerStyle(21);
    h3->Draw("ep");       // Draw the ratio plot

    TLine *l = new TLine(hratio->GetXaxis()->GetXmin(), 1.0,
                         hratio->GetXaxis()->GetXmax(), 1.0);
    l->SetLineColor(kBlack);
    l->SetLineWidth(2);
    l->Draw();


    hratio->SetFillStyle(3444);
    hratio->SetFillColor(kGray+2);
    hratio->SetMarkerStyle(0);
    hratio->Draw("e2, same");
  }
  
  pad1->cd(); leg->Draw("same");

  c->Print(outName + ".png", "png");

  delete c, hs;
  
}

