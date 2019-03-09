


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

      plot->SetTitles("Muon Pt", "muonPt", "Events", "muonPt");
      plot->SetLegendPos(0.62, 0.6, 0.85, 0.85);
      plot->Stack("MuonPt");
  
      plot->SetTitles("MuMuMass", "MMMass", "Events", "muMuMass");
      plot->SetLegendPos(0.6, 0.6, 0.9, 0.9);
      plot->Stack("MuMuMass", true);

      plot->SetTitles("Me gusta el miso", "MIso", "Events", "muon_Iso");
      plot->SetLegendPos(0.6, 0.6, 0.9, 0.9);
      plot->Stack("Muon_Iso", false, 20000);



      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Eficiencia del trigger", "Ptmuon", "Events", "triggEff");
      plot->plotWithRatio("ttbar", "MuonPt_TriggOnly", "MuonPt_raw", "RATIO", 0, 1.1);

      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Aceptancia", "xda", "Events", "aceptancia");
      plot->plotWithRatio("ttbar", "Acep_obs", "Acep_gen", "RATIO", 0, 1, false);

      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("BJet_Pt", "BJet pt", "Events", "deleteme");
      plot->plotWithRatio("ttbar", "Hadr_BJet_Pt_GEN", "Lept_BJet_Pt_GEN", "RATIO", 0, 1, false);
      
      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Bjets gen vs jets med.", "BJet pt", "Events", "eff B");
      plot->plotWithRatio("ttbar", "BJet_Pt", "Jets_GEN_Pt", "RATIO", 0.3, 1.5, false);



      plot->PrintEvents("MuonPt");
      plot->PrintXSecData();
      delete plot;
      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
/*
    if(rank==RANKS[1])
    {
    // Jet btag >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"wjets", "ttbar"}, pathToFiles, "");

    plot->SetTitles("Jet btag","Jet_btag", "Events", "jet_btag");
    plot->SetLegendPos(0.7, 0.8, 0.88, 0.88);
    plot->Stack("Jet_btag", 0, 200);

    plot->PrintEvents("Jet_btag");
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
*/



//    if(rank==RANKS[2])
//    {
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    plot = new Plotter({"ttbar"}, pathToFiles, "");
//
//    //plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
//    //plot->SetTitles("Eficiencia del trigger", "Ptmuon", "Events", "triggEff");
//    //plot->plotWithRatio("ttbar", "MuonPt_TriggOnly", "MuonPt_raw", "RATIO", 0, 1.1);
//
//
//    //plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
//    //plot->SetTitles("BJet_Pt", "BJet pt", "Events", "CCCCCC");
//    //plot->plotWithRatio("ttbar", "Hadr_BJet_Pt_GEN", "Lept_BJet_Pt_GEN", "RATIO", 0, 1, false);
//
//    //plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
//    //plot->SetTitles("Bjets gen vs jets med.", "BJet pt", "Events", "deleteme");
//    //plot->plotWithRatio("ttbar", "BJet_Pt", "Jets_GEN_Pt", "RATIO", 0.3, 1.5, false);
//
//
//
//    //plot->PrintEvents("Acep_obs");
//    //plot->GetTriggerEff();
//    delete plot;
//    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//    }
    MPI_Finalize();

}





int main()
{
  terminator();
}


