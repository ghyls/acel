
#include "selector.h"

#include <iostream>
#include <vector>
#include <string>
#include "TH1.h"


class Plotter
{

  public:

    Float_t fLegX1 = 0.70, fLegY1 = 0.55, fLegX2 = 0.92, fLegY2 = 0.92;
    Float_t LegendTextSize  = 0.065;
    TString xtitle = "";
    TString ytitle = "";
    TString title  = "";    
    Selector* dataSelector;

    Plotter(std::vector<std::string> bkgs, std::string pathToFiles,  
    std::string prefixForHistos, std::string data = "")
    {
      data = data;

      for (int i = 0; i < bkgs.size(); i++)
      {
        Selector sel(pathToFiles, bkgs[i], prefixForHistos);

        listOfSelectors.push_back(sel); 
        listOfColors.push_back(i+1);
      }

      if (data != "")
        dataSelector = new Selector(pathToFiles, data, prefixForHistos);
    }

    void SetLegendPos(Float_t x1, Float_t y1, Float_t x2, Float_t y2)
    {
      fLegX1 = x1; 
      fLegX2 = x2; 
      fLegY1 = y1; 
      fLegY2 = y2; 
    }
    
    void SetLegendSize(Float_t size = 0.065)
    {
      LegendTextSize = size;
    }

    void SetColors(std::vector<Int_t> col)
    {
      listOfColors = col;
    }

    void SetTitle(TString _title)
    {
      title = _title;
    }
    
    void SetXTitle(TString _xtitle)
    {
      xtitle = _xtitle;
    }
    void SetYTitle(TString _ytitle)
    {
      ytitle = _ytitle;
    }

    TH1F* GetHisto(TString name) //name is the name of the histo
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

    int GetEvents(TString name)
    {
      // Returns the integral of a histogram
      return GetHisto(name)->Integral();
    }

    void PrintEvents(TString name)
    {
      // Name is a name. Not a list or any other weird thing.

      std::cout << "\nPrinting number of events for histogram " << name << "\n";
      std::cout << "---------------------------------------------" << std::endl;

      Int_t totalEvts = 0;

      for (int i = 0; i < listOfSelectors.size(); i++)
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


    void Stack(TString name)
    {
      //TODO: C'mon dude, that should be easy 
    }


  private:

    TString data;
    std::vector <Selector> listOfSelectors;
    //Selector ds();
    std::vector <Int_t> listOfColors;


}
