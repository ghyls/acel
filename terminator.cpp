


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

    std::cout <<"[terminator] NOTE. Hello!" << std::endl;

    gROOT->LoadMacro("selector.cpp+");
    gROOT->LoadMacro("plotter.cpp+");

    R__LOAD_LIBRARY(selector_cpp.so);
    R__LOAD_LIBRARY(plotter_cpp.so);

    std::string pathToFiles = "./practica/files/";
    std::string dataFile = "data";
    std::string prefix = "hello";    

    std::vector<std::string> MCsamples;
    MCsamples = {"qcd", "wjets", "ww", "wz", "zz", "dy", "single_top", "ttbar"};

    std::vector<Int_t> colors = {kGray, kBlue-1, kTeal-1, kTeal+1, kTeal+4, kAzure-8, kOrange+1, kRed+1};

    Plotter plot(MCsamples, pathToFiles, prefix, "data");

    plot.SetColors(colors);
    plot.SetLegendPos(0.62, 0.6, 0.88, 0.88);
    plot.SetYTitle("Events");
    plot.SetTitle("HELLO");
    plot.SetXTitle("mumuM");

    //plot.Stack("MuonPt");
    plot.Stack("DiMuonMass");


    plot.PrintEvents("MuonPt");
}