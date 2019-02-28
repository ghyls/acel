#pragma once

#include <string>
#include "TH1.h"




class Selector
{
  public:

    std::vector<TH1F*> histograms; 
    TString prefix;                 // prefix for ALL histos
    TString histoName;              // without .root

    Selector();
    Selector(std::string _filePath, std::string _fileName, std::string _prefix);
    
    TH1F* GetHisto(TString name);

    int GetNumberOfHistos();

  private:

    TString filePath;               // path of the root file

    bool fileExist(const std::string name);

    void CreateHistograms();

    void Loop();
};