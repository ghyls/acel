

#include <iostream>
#include <vector>
#include "TH1.h"


class Selector
{

  public:
    
    TString prefix; //prefix for ALL histos
    std::vector<TH1F*> histograms;  
    
    void CreateHistograms()
    {
      TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), ";p_{T}^{#mu} (GeV);Events", 20, 0, 200);
      TH1F* h2 = new TH1F(prefix + TString("_DiMuonMass"), ";m^{#mu#mu} (GeV);Events", 20, 0, 200);

  	  histograms.push_back(h1); 
  	  histograms.push_back(h2);    

      //add here more histograms   
    }

    TH1F GetHisto(TString name) //name is the name of the histo
    {
        int len = histograms.size();
        for (int i = 0; i < len; i++)
        {
            TString n = histograms[i]->GetName(); //full name
            
            if ((prefix + '_' + name) == n)
                return *histograms[i];
        }

        std::cout << "[Selector::GetHisto] WARNING: not found histo named " + \
        name + ". Returning an empty one" << std::endl;
        return TH1F();
    }

};


void selector()
{
    Selector obj;
    obj.prefix = "hello";
    obj.CreateHistograms();
    obj.GetHisto("MuonPst");
}