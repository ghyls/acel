


#include "selector.h"
#include "plotter.h"

#include <iostream>
#include <vector>
#include <string>
//#include <mpi.h>
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TSystem.h"
#include "TROOT.h"


void terminator()
{

  //MPI_Comm comm;
  //MPI_Status status;
  int size, rank = 0;
  
  //comm = MPI_COMM_WORLD;
  
  //MPI_Init(NULL, NULL);
  
  //MPI_Comm_size(comm, &size);
  //MPI_Comm_rank(comm, &rank);
  
  std::cout << "Hello from rank " << rank << '!' << std::endl;
  if (rank==0) {
    std::cout << "Running on " << size << " process(es)!\n" << std::endl;}

  std::string pathToFiles = "./practica/files/";
  std::string dataFile = "data";
  std::string prefix = "hello";    

  std::vector<std::string> MCsamples;
  std::vector<std::string> sampleType;


  std::vector<Int_t> colors = {kGray, kBlue-1, kTeal-1, kTeal+1, kTeal+4, \
                              kPink+7, kOrange+1, kRed-6};

  MCsamples = {"qcd", "wjets", "ww", "wz", "zz", "dy", "single_top", "ttbar"};
  sampleType= {"bkg", "bkg", "bkg", "bkg", "bkg"};
  static Plotter * plot;

  int RANKS[3] = {0, 0, 0};

    plot = new Plotter(MCsamples, pathToFiles, "data");
    std::vector<TString> histos;
    plot->SetColors(colors);

    if(rank==RANKS[0])
    {
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("W Mass", "M_W", "Events", "leptWMass");
      histos = {"MCMassHadrW", "MCMassLeptW"};
      plot->Stack("", "ttbar", false, "", histos, false);

      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("T Mass", "M_T", "Events", "leptTMass");
      histos = {"MCMassHadrT", "MCMassLeptT"};
      plot->Stack("", "ttbar", false, "", histos, false);

      plot->SetTitles("", "muonPt", "Events", "muonPt");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("MuonPt", "", true, "");

      plot->SetTitles("", "TempXSec", "Events", "TempXSec");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("TempXSec", "", true, "");

      plot->SetTitles("", "EvWeight", "Events", "EvWeight");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("evtWeight", "", true, "");

      plot->SetTitles("MuMuMass", "MMMass", "Events", "muMuMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MuMuMass", "", true, "", {}, true);

      plot->SetTitles("Me_gusta_el_miso", "MIso", "Events", "muon_Iso");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("Muon_Iso", "", true, "", {}, false, 20000);

      
      plot->SetTitles("Hadr W mass", "HadrW", "Events", "dataHadrWMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MassHadrW", "", true, "gauss");

      plot->SetTitles("lep W mass", "lepW", "Events", "datalepWMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MassLeptW", "", true, "gauss");

      plot->SetTitles("hadr T mass", "hadrT", "Events", "dataHadrTMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MassHadrT", "", true, "gauss");

      plot->SetTitles("Lept T mass", "LeptT", "Events", "dataLeptTMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MassLeptT", "", true, "gauss");   

      plot->SetTitles("Best T mass", "BestT", "Events", "dataBestTMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MassBestT", "", true, "gauss");   
    }


    if(rank==RANKS[1])
    {
      
      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Eficiencia del trigger", "Ptmuon", "Events", "triggEff");
      plot->plotWithRatio("ttbar", "MuonPt_TriggOnly", "MuonPt_raw", "RATIO", 
                          0, 1.1);

      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Aceptancia", "xda", "Events", "aceptancia");
      plot->plotWithRatio("ttbar", "Acep_obs", "Acep_gen", "RATIO", 0, 1, 
                          false);

      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("bTag eff", "uff", "Events", "bTagEff");
      plot->plotWithRatio("ttbar", "JetBTaggedRECO", "JetMatchedRECO", 
                          "RATIO", 0, 1, false);
      
      plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
      plot->SetTitles("Bjets gen vs jets med.", "BJet pt", "Events", "effB");
      plot->plotWithRatio("ttbar", "BJet_Pt", "Jets_GEN_Pt", "RATIO", 0.3, 1.5, 
                          false);

      plot->SetLegendPos(0.62, 0.65, 0.88, 0.88);
      plot->SetTitles("","Jet_btag", "Events", "jet_btag");
      plot->Stack("Jet_btag", "", false, "", {}, false, 600);

      //plot->PrintEvents("MuonPt");
      plot->PrintXSecData();
      
    }
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    

    if(rank==RANKS[1])
    {
    // Jet btag >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }




    if(rank==RANKS[2])
    {
    /*
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    plot = new Plotter({"ttbar"}, pathToFiles, "");

    //plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
    //plot->SetTitles("Eficiencia del trigger", "Ptmuon", "Events", "triggEff");
    //plot->plotWithRatio("ttbar", "MuonPt_TriggOnly", "MuonPt_raw", "RATIO", 0, 1.1);

    plot->SetLegendPos(0.6, 0.75, 0.9, 0.9);
    plot->SetTitles("Bjets gen vs jets med.", "BJet pt", "Events", "deleteme");
    plot->plotWithRatio("ttbar", "BJet_Pt", "Jets_GEN_Pt", "RATIO", 0.3, 1.5, false);

    //plot->PrintEvents("Acep_obs");
    //plot->GetTriggerEff();
    delete plot;
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    */
    }
    //MPI_Finalize();

}





int main()
{
  terminator();
}


