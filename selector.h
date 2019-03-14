#pragma once

#include <string>
#include <mpi.h>

#include "TH1.h"
#include "TLorentzVector.h"




class Selector
{
  public:

    std::vector<TH1F*> histograms; 
    TString process;                // without .root

    Selector();
    ~Selector();       

    Selector(std::string _filePath, std::string _fileName);
    
    TH1F* GetHisto(TString name);

    int GetNumberOfHistos();
    float Module(float x, float y, float z);
    float DR(TLorentzVector v1, TLorentzVector v2);

    void CreateHistograms(TString prefix);


  private:

    TString filePath;               // path of the root file

    bool fileExist(const std::string name);


    void Loop();
};