


#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TSystem.h"
#include "TROOT.h"


void terminator()
{

    gROOT->LoadMacro("selector.cpp+");
    gROOT->LoadMacro("plotter.cpp+");

    R__LOAD_LIBRARY(selector_cpp.so);
    R__LOAD_LIBRARY(plotter_cpp.so);

    std::string pathToFiles = "./practica/files/";
    std::string dataFile = "data";
    std::string prefix = "hello";    

    std::vector<std::string> MCsamples;


    std::vector<Int_t> colors = {kGray, kBlue-1, kTeal-1, kTeal+1, kTeal+4, \
                                kAzure-8, kOrange+1, kRed+1};

    MCsamples = {"qcd", "wjets", "ww", "wz", "zz", "dy", "single_top", "ttbar"};
    Plotter * plot;


    // mu mu Pt >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter(MCsamples, pathToFiles, prefix, "data");
    //plot.SetColors(colors);
    plot->SetTitle("Muon Pt");
    plot->SetXTitle("muonPt");
    plot->SetLegendPos(0.62, 0.6, 0.85, 0.85); plot->SetYTitle("Events");
    plot->Stack("MuonPt");

    plot->PrintEvents("MuonPt");
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Jet btag >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"wjets", "ttbar"}, pathToFiles, prefix, "");
    plot->SetTitle("Muon Pt");
    plot->SetXTitle("Jet_btag");
    plot->SetLegendPos(0.7, 0.8, 0.88, 0.88); plot->SetYTitle("Events");
    plot->Stack("Jet_btag", 1000);

    plot->PrintEvents("Jet_btag");
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    // Trigger eff. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"ttbar"}, pathToFiles, prefix, "");
    plot->SetTitle("Eficiencia del trigger");
    plot->SetXTitle("Ptmuon");
    plot->SetLegendPos(0.7, 0.75, 0.9, 0.9); plot->SetYTitle("Events");

    plot ->plotWithRatio("AllMuons", "MuonPt");

    plot->PrintEvents("AllMuons");
    plot->PrintEvents("MuonPt");
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}
