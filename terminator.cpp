


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

    MPI_Comm comm;
    MPI_Status status;
    int size, rank;
  
    comm = MPI_COMM_WORLD;
  
    MPI_Init(NULL, NULL);
  
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
  
    std::cout << "Hello from rank " << rank << '!' << std::endl;
    if (rank==0) {
      std::cout << "Running on " << size << " process(es)!\n" << std::endl;}
    //int rank = 0;  

    std::string pathToFiles = "./practica/files/";
    std::string dataFile = "data";
    std::string prefix = "hello";    

    std::vector<std::string> MCsamples;


    std::vector<Int_t> colors = {kGray, kBlue-1, kTeal-1, kTeal+1, kTeal+4, \
                                kAzure-8, kOrange+1, kRed+1};

    MCsamples = {"qcd", "wjets", "ww", "wz", "zz", "dy", "single_top", "ttbar"};
    static Plotter * plot;

    int RANKS[3] = {0, 0, 0};

    if(rank==RANKS[0])
    {
      // mu mu Pt >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      plot = new Plotter(MCsamples, pathToFiles, "data");
      //plot.SetColors(colors);
      plot->SetTitle("Muon Pt");
      plot->SetXTitle("muonPt");
      plot->SetLegendPos(0.62, 0.6, 0.85, 0.85); plot->SetYTitle("Events");
      plot->Stack("MuonPt");
  
      plot->PrintEvents("MuonPt");
      delete plot;
      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }

    if(rank==RANKS[1])
    {
    // Jet btag >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"wjets", "ttbar"}, pathToFiles, "");
    //plot = new Plotter(MCsamples, pathToFiles, "");
    plot->SetTitle("Jet btag");
    plot->SetXTitle("Jet_btag");
    plot->SetLegendPos(0.7, 0.8, 0.88, 0.88); plot->SetYTitle("Events");
    plot->Stack("Jet_btag", 200);

    plot->PrintEvents("Jet_btag");
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }

    /*
    */

    if(rank==RANKS[2])
    {
    // Trigger eff. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"ttbar"}, pathToFiles, "");
    //plot = new Plotter(MCsamples, pathToFiles, "");
    plot->SetTitle("Eficiencia del trigger");
    plot->SetXTitle("Ptmuon");
    plot->SetLegendPos(0.6, 0.75, 0.9, 0.9); plot->SetYTitle("Events");

    plot->plotWithRatio("MuonPt", "MuonPt_raw", "diMuPt", "RATIO", 0, 1.1);

    plot->PrintEvents("MuonPt_raw");
    plot->PrintEvents("MuonPt");

    // plot->GetTriggerEff();
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }

    if(rank==RANKS[2])
    {
    // Trigger eff. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"ttbar"}, pathToFiles, "");
    //plot = new Plotter(MCsamples, pathToFiles, "");
    plot->SetTitle("Aceptancia");
    plot->SetXTitle("asd");
    plot->SetLegendPos(0.6, 0.75, 0.9, 0.9); plot->SetYTitle("Events");

    plot->plotWithRatio("Acep_obs", "Acep_gen", "acept", "RATIO", 0, 2);

    plot->PrintEvents("Acep_obs");


    // plot->GetTriggerEff();
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
    MPI_Finalize();


}





int main()
{
    terminator();
}


