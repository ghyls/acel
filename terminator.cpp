

//R__LOAD_LIBRARY(libTreePlayer)

#include "plotter.h"
#include "selector.h"

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
    gROOT->LoadMacro("plotter.cpp");
    gROOT->LoadMacro("selector.h");
    gROOT->LoadMacro("selector.cpp");
    gROOT->LoadMacro("plotter.h");

    std::string pathToFiles = "./practica/files/";
    std::string dataFile = "data";
    std::string prefix = "hello";    

    std::vector<std::string> MCsamples;
    //MCsamples = {TString("qcd")};//, "wjets", "ww", "wz", "zz", "dy", "single_top", "ttbar"};

    MCsamples.push_back("qcd");

    Plotter plot(MCsamples, pathToFiles, prefix, "data");
}