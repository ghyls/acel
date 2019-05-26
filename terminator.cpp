


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
  
  //std::cout << "Hello from rank " << rank << '!' << std::endl;
  //if (rank==0) {
  //  std::cout << "Running on " << size << " process(es)!\n" << std::endl;}

  std::string pathToFiles = "./practica/files/";
  std::string dataFile = "data";
  std::string prefix = "hello";    

  std::vector<std::string> MCsamples;
  std::vector<std::string> sampleType;


  std::vector<Int_t> colors = {kGray, kBlue-1, kGreen-6, kTeal+4, kSpring+3, \
                              kPink+8, kOrange+1, kRed-6};
  //std::vector<Int_t> colors = {kRed-7, kBlue-1, kGreen-6, kTeal+4, kSpring+3, \
  //                            kPink+8, kOrange+1, kRed-6};
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
      plot->SetTitles("", "M_{W} (GeV), lept.", "Events / 0.4 GeV", "MCWMassL");
      histos = {"MCMassLeptW"};
      plot->Stack("", "ttbar", false, "", histos, 0);

      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("", "M_{W} (GeV), hadr.", "Events / 0.4 GeV", "MCWMassH");
      histos = {"MCMassHadrW"};
      plot->Stack("", "ttbar", false, "", histos, 0);

      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("", "M_{W} (GeV), hadr.", "Events", "MassHadrW");
      histos = {"MassHadrW"};
      plot->Stack("", "ttbar", false, "", histos, false);

      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("", "M_{T} (GeV), hadr.", "Events / 0.012 GeV", "MCTMassH");
      histos = {"MCMassHadrT"};
      plot->Stack("", "ttbar", false, "", histos, false);

      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->SetTitles("", "M_{T} (GeV), lept.", "Events / 0.012 GeV", "MCTMassL");
      histos = {"MCMassLeptT"};
      plot->Stack("", "ttbar", false, "", histos, false);


      plot->SetTitles("", "P_{t, #mu}", "Events / 16 GeV", "muonPt");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("MuonPt", "", true, "");

      plot->scale =300;
      plot->SetTitles("", "missing E_{T} (GeV)", "Events / 3 GeV", "met");
      plot->SetLegendPos(0.71, 0.6, 0.94, 0.88);
      plot->Stack("MET", "", true, "");
      plot->scale = 999;

      plot->SetTitles("", "Jet P_{T}", "Events / 6 GeV", "JetPt");
      plot->SetLegendPos(0.73, 0.6, 0.94, 0.88);
      plot->Stack("Jet_Pt", "", true, "", {}, 0);

      plot->SetTitles("", "TempXSec", "Events", "TempXSec");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("TempXSec", "", true, "");

      plot->SetTitles("", "EvWeight", "Events", "EvWeight");
      plot->SetLegendPos(0.76, 0.6, 0.94, 0.88);
      plot->Stack("evtWeight", "", true, "");

      plot->SetTitles("MuMuMass", "MMMass", "Events", "muMuMass");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("MuMuMass", "", true, "", {}, true);

      plot->scale = 110;
      plot->SetTitles("", "Muon iso / Muon p_{T}", "Events", "muon_Iso");
      plot->SetLegendPos(0.7, 0.6, 0.9, 0.88);
      plot->Stack("Muon_Iso", "", true, "", {}, false, 20000);
      plot->scale = 999;
      
      plot->SetTitles("", "M_{W} (GeV), hadr.", "Events / 9 GeV", "dataHadrWMass");
      plot->SetLegendPos(0.76, 0.6, 0.9, 0.88);
      plot->Stack("MassHadrW", "", true, "gauss");

      plot->SetTitles("", "M_{W} transversa (GeV)", "Events / 10 GeV", "datalepWMass");
      plot->SetLegendPos(0.76, 0.6, 0.9, 0.88);
      plot->Stack("TransMassLeptW", "", true, "gauss");

      plot->SetTitles("", "M_{T} (GeV), hadr.", "Events / 24 GeV", "dataHadrTMass");
      plot->SetLegendPos(0.76, 0.6, 0.9, 0.88);
      plot->Stack("MassHadrT", "", true, "gauss");

      plot->SetTitles("", "M_{T} (GeV), lept.", "Events / 24 GeV", "dataLeptTMass");
      plot->SetLegendPos(0.76, 0.6, 0.9, 0.88);
      plot->Stack("MassLeptT", "", true, "gauss");   

      plot->SetTitles("", "M_{T} (GeV), best", "Events / 24 GeV", "dataBestTMass");
      plot->SetLegendPos(0.76, 0.6, 0.9, 0.88);
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

      plot->SetLegendPos(0.67, 0.65, 0.88, 0.88);
      plot->SetTitles("","Discriminator, g", "Jets / 0.1", "jet_btag");
      plot->Stack("Jet_btag", "", false, "", {}, false, 650);

      plot->PrintEvents("MuonPt");
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


