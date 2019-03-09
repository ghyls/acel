# pragma once

#include "selector.h"

#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"


class Plotter
{
    public:

        Selector* dataSelector;
        Selector* bkgSelector;

        Float_t fLegX1, fLegY1, fLegX2, fLegY2;

        Float_t LegendTextSize;
        TString xtitle = "";
        TString ytitle = "";
        TString title = ""; 
        TString outName = "out"; 

        Plotter();
        Plotter(std::vector<std::string> bkgs, std::string pathToFiles, 
                    std::string data);

        ~Plotter();       

        void SetLegendPos(Float_t x1, Float_t y1, Float_t x2, Float_t y2);
        void SetLegendSize(Float_t size = 0.065);
        void SetColors(std::vector<Int_t> col);
        void SetTitle(TString _title);
        void SetXTitle(TString _xtitle);
        void SetYTitle(TString _ytitle);
        void SetTitles(TString _title, TString _xtitle, TString _ytitle, 
                        TString _outName);
        TH1F* GetHisto(TString name);
        int GetEvents(TString name);
        void PrintEvents(TString name);
        void Stack(TString name,  bool doLogY = 0, Float_t maxY = -1);
        void plotWithRatio(TString process, TString nameH1, TString nameH2, \
        TString rLabel, float rMin, float rMax, bool doLogY = 0, float max = 999);
        void DrawOverflowBin(TH1F* h);
        std::vector<double> GetTriggerEff();
        std::vector<double> GetBTagEff();
        std::vector<double> GetAcceptance();
        void PrintXSecData();

    private:

        std::vector <Selector*> listOfSelectors;
        TString data;
        std::string pathToFiles;
        std::vector <Int_t> listOfColors;
};