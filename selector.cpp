
#include "selector.h"

#include <iostream>
#include <string>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

// Selector corre una vez por muestra.

// TODO: pintar todos los MC, uno encima del otro
// TODO: pintar los datos en el mismo histo
// TODO: crear histograma cociente
// TODO: aplicar cortes


#define JET_MIN_PT 0 // 30
#define JET_MAX_PT 999
#define DR_MAX_JETS 0.2
#define MUON_MIN_PT 26.7
#define BTAG_LIM 0



Selector::Selector(std::string _filePath, std::string _fileName)
{

  if (!fileExist(_filePath + _fileName + ".root"))
  {
    std::cout << "[Selector] ERROR. File '" << _filePath + _fileName << "' "
    "does not exist!" << std::endl;

    throw;
  }

  process = _fileName;
  filePath = TString(_filePath);  // including last '/'

  CreateHistograms(process);
  Loop();

}

Selector::~Selector()
{
  for (int i = 0; i < histograms.size(); i++)
  {
    delete histograms[i];

  }

  histograms.clear();
  histograms = {};
}

TH1F* Selector::GetHisto(TString name) //name is the name of the histo
{
    int len = histograms.size();
    for (int i = 0; i < len; i++)
    {
      TString n = histograms[i]->GetName(); //full name

      if ((process + '_' + name) == n)
        return histograms[i];
    }

    std::cout << "[Selector::GetHisto] WARNING: not found histo named " + \
    name + ". Keep up the good work!" << std::endl;

    throw;
}

int Selector::GetNumberOfHistos()
{
  return histograms.size();
}


  
bool Selector::fileExist(const std::string name)
{
  return (access(name.c_str(), F_OK) != -1);
}

void Selector::CreateHistograms(TString prefix)
{

  //there variables may be called from other cpps.
  TH1F* h1 = new TH1F(prefix + TString("_MuonPt"), "", 8, 20, 130);
  TH1F* h2 = new TH1F(prefix + TString("_Jet_btag"), "", 80, -2, 6);
  TH1F* h3 = new TH1F(prefix + TString("_MuonPt_raw"), "", 40, 10, 60);
  TH1F* h4 = new TH1F(prefix + TString("_Acep_gen"), "", 20, -1, 2);
  TH1F* h5 = new TH1F(prefix + TString("_Acep_obs"), "", 20, -1, 2);
  TH1F* h6 = new TH1F(prefix + TString("_MuMuMass"), "", 40, 60, 120);
  TH1F* h7 = new TH1F(prefix + TString("_Muon_Iso"), "", 60, 0, 0.15);
  TH1F* h8 = new TH1F(prefix + TString("_MuonPt_TriggOnly"), "", 40, 10, 60);

  TH1F* h9 = new TH1F(prefix + TString("_BJet_Pt"), "", 20, 20, 160);
  TH1F* h10 = new TH1F(prefix + TString("_JetBTaggedRECO"), "", 11, -1, 10);
  TH1F* h11 = new TH1F(prefix + TString("_JetMatchedRECO"), "", 11, -1, 10);
  TH1F* h12 = new TH1F(prefix + TString("_Jets_GEN_Pt"), "", 20, 20, 160);
  TH1F* h13 = new TH1F(prefix + TString("_TempXSec"), "", 20, 0, 2);

  TH1F* h14 = new TH1F(prefix + TString("_MCMassHadrW"), "", 100, 60, 100);
  TH1F* h15 = new TH1F(prefix + TString("_MCMassLeptW"), "", 100, 60, 100);
  TH1F* h16 = new TH1F(prefix + TString("_MCMassHadrT"), "", 100, 171.8, 173);
  TH1F* h17 = new TH1F(prefix + TString("_MCMassLeptT"), "", 100, 171.8, 173);
  TH1F* h18 = new TH1F(prefix + TString("_JetMatchSuccess"), "", 12, -1, 3);
  TH1F* h19 = new TH1F(prefix + TString("_MassHadrW"), "", 12, 60, 100);
  TH1F* h20 = new TH1F(prefix + TString("_MassLeptW"), "", 12, 60, 100);


  histograms.push_back(h1); 
  histograms.push_back(h2); 
  histograms.push_back(h3); 
  histograms.push_back(h4); 
  histograms.push_back(h5); 
  histograms.push_back(h6); 
  histograms.push_back(h7); 
  histograms.push_back(h8); 
  histograms.push_back(h9); 
  histograms.push_back(h10); 
  histograms.push_back(h11); 
  histograms.push_back(h12); 
  histograms.push_back(h13); 
  histograms.push_back(h14); 
  histograms.push_back(h15); 
  histograms.push_back(h16); 
  histograms.push_back(h17); 
  histograms.push_back(h18); 
  histograms.push_back(h19); 
  histograms.push_back(h20); 
}

float Selector::Module(float x, float y, float z)
{
  return TMath::Power(x*x + y*y + z*z, 0.5);
}

float Selector::DR(TLorentzVector v1, TLorentzVector v2)
{
  float dEta2 = TMath::Power((v1.Eta()-v2.Eta()), 2);
  float dPhi2 = TMath::Power((v1.Phi()-v2.Phi()), 2);
  return TMath::Power(dEta2 + dPhi2, 0.5);  
}

void Selector::Loop()
{ 
  TFile f(filePath + process + ".root");
  TTree * tree = (TTree *) f.Get("events");

  Int_t numEvents = tree->GetEntries();

  // Set Adresses to branches >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Int_t NMuon;
  Int_t NJet;
  Float_t Jet_E[20]; 
  Float_t Jet_btag[20]; 
  Float_t Jet_Px[20], Jet_Py[20], Jet_Pz[20];

  Float_t MChadronicBottom_px; // if filled, there are 1 b tags.
  Float_t MChadronicBottom_py; // if filled, there are 1 b tags.
  Float_t MChadronicBottom_pz; // if filled, there are 1 b tags.

  Float_t MCleptonicBottom_px; // if filled, there are 2 b tags.
  Float_t MCleptonicBottom_py; // if filled, there are 2 b tags.
  Float_t MCleptonicBottom_pz; // if filled, there are 2 b tags.

  Float_t MChadronicWDecayQuark_px;
  Float_t MChadronicWDecayQuark_py;
  Float_t MChadronicWDecayQuark_pz;

  Float_t MChadronicWDecayQuarkBar_px;
  Float_t MChadronicWDecayQuarkBar_py;
  Float_t MChadronicWDecayQuarkBar_pz;
  

  Bool_t triggerIsoMu24;
  Float_t Muon_Px[5]; // just in case... memory is not a problem here
  Float_t Muon_Py[5];
  Float_t Muon_Pz[5];
  Float_t Muon_E[5];
  Float_t MET_px, MET_py;
  Float_t Muon_Iso[5];
  Float_t EventWeight;
  Float_t MClepton_px[5], MClepton_py[5], MClepton_pz[5];
  Float_t MCneutrino_px, MCneutrino_py, MCneutrino_pz;
  Int_t MCleptonPDGid[5]; // 11 electrones, 13 muones, 15 taus



  tree->SetBranchAddress("NMuon", &NMuon); //
  tree->SetBranchAddress("Muon_Px", &Muon_Px);
  tree->SetBranchAddress("Muon_Py", &Muon_Py);
  tree->SetBranchAddress("Muon_Pz", &Muon_Pz);
  tree->SetBranchAddress("Muon_E", &Muon_E);
  tree->SetBranchAddress("MET_px", &MET_px);
  tree->SetBranchAddress("MET_py", &MET_py);
  tree->SetBranchAddress("Muon_Iso", &Muon_Iso);
  tree->SetBranchAddress("triggerIsoMu24", &triggerIsoMu24);
  tree->SetBranchAddress("EventWeight", &EventWeight);
  tree->SetBranchAddress("NJet", &NJet);
  tree->SetBranchAddress("Jet_E", &Jet_E);
  tree->SetBranchAddress("Jet_Px", &Jet_Px);
  tree->SetBranchAddress("Jet_Py", &Jet_Py);
  tree->SetBranchAddress("Jet_Pz", &Jet_Pz);

  tree->SetBranchAddress("Jet_btag", &Jet_btag);
  tree->SetBranchAddress("MClepton_px", &MClepton_px);
  tree->SetBranchAddress("MClepton_py", &MClepton_py);
  tree->SetBranchAddress("MClepton_pz", &MClepton_pz);
  tree->SetBranchAddress("MCneutrino_px", &MCneutrino_px);
  tree->SetBranchAddress("MCneutrino_py", &MCneutrino_py);
  tree->SetBranchAddress("MCneutrino_pz", &MCneutrino_pz);

  tree->SetBranchAddress("MCleptonPDGid", &MCleptonPDGid);

  tree->SetBranchAddress("MChadronicBottom_px", &MChadronicBottom_px);
  tree->SetBranchAddress("MChadronicBottom_py", &MChadronicBottom_py);
  tree->SetBranchAddress("MChadronicBottom_pz", &MChadronicBottom_pz);
  tree->SetBranchAddress("MCleptonicBottom_px", &MCleptonicBottom_px);
  tree->SetBranchAddress("MCleptonicBottom_py", &MCleptonicBottom_py);
  tree->SetBranchAddress("MCleptonicBottom_pz", &MCleptonicBottom_pz);

  tree->SetBranchAddress("MChadronicWDecayQuark_px", &MChadronicWDecayQuark_px);
  tree->SetBranchAddress("MChadronicWDecayQuark_py", &MChadronicWDecayQuark_py);
  tree->SetBranchAddress("MChadronicWDecayQuark_pz", &MChadronicWDecayQuark_pz);
  
  tree->SetBranchAddress("MChadronicWDecayQuarkBar_px", &MChadronicWDecayQuarkBar_px);
  tree->SetBranchAddress("MChadronicWDecayQuarkBar_py", &MChadronicWDecayQuarkBar_py);
  tree->SetBranchAddress("MChadronicWDecayQuarkBar_pz", &MChadronicWDecayQuarkBar_pz);


  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  // Loop over every event >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  std::cout << "[Selector::loop]: " << process << ", " << numEvents  << \
                " events." << std::endl;


  for (int i = 0; i < numEvents; i++)
  {
    tree->GetEntry(i);

    if (process == "data") {EventWeight = 1;}

    TLorentzVector leadMuon; 
    TLorentzVector muon2; 

    TLorentzVector muon;  // aux variables
    TLorentzVector jet;   // aux variables
    TLorentzVector jet2;  // aux variables
    TLorentzVector nu;    // aux variables
    TLorentzVector W;
    TLorentzVector b;


    // compute TMass from MC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    if (process == "ttbar" && MCneutrino_px != 0)
    {
      // leptonic W ............................................................
      Float_t muonP = Module(MClepton_px[0], MClepton_py[0], MClepton_pz[0]);
      Float_t muonE = TMath::Power(0.1056*0.1056 + muonP*muonP, 0.5);
      Float_t nuP = Module(MCneutrino_px, MCneutrino_py, MCneutrino_pz);


      muon.SetPxPyPzE(MClepton_px[0], MClepton_py[0], MClepton_pz[0], muonE);
      nu.SetPxPyPzE(MCneutrino_px, MCneutrino_py, MCneutrino_pz, nuP);
      GetHisto("MCMassLeptW")->Fill((muon + nu).M(), EventWeight);
      
      // leptonic T ............................................................
      Float_t bP = Module(MCleptonicBottom_px, MCleptonicBottom_py,
                          MCleptonicBottom_pz);
      Float_t bE = TMath::Power(4.18*4.18 + bP*bP, 0.5);
      b.SetPxPyPzE(MCleptonicBottom_px, MCleptonicBottom_py, 
                                                      MCleptonicBottom_pz, bE);
      GetHisto("MCMassLeptT")->Fill((muon + nu + b).M(), EventWeight);


      // hadronic W ............................................................
      Float_t q1P = Module(MChadronicWDecayQuark_px, MChadronicWDecayQuark_py, 
                      MChadronicWDecayQuark_pz);
      Float_t q2P = Module(MChadronicWDecayQuarkBar_px, 
                      MChadronicWDecayQuarkBar_py, MChadronicWDecayQuarkBar_pz);


      jet.SetPxPyPzE(MChadronicWDecayQuark_px, MChadronicWDecayQuark_py,
                        MChadronicWDecayQuark_pz, q1P);
      jet2.SetPxPyPzE(MChadronicWDecayQuarkBar_px, MChadronicWDecayQuarkBar_py, 
                        MChadronicWDecayQuarkBar_pz, q2P);
      GetHisto("MCMassHadrW")->Fill((jet + jet2).M(), EventWeight);

      // hadronic T ............................................................
      bP = Module(MChadronicBottom_px, MChadronicBottom_py,
                          MChadronicBottom_pz);
      bE = TMath::Power(4.18*4.18 + bP*bP, 0.5);
      b.SetPxPyPzE(MChadronicBottom_px, MChadronicBottom_py, 
                                                      MChadronicBottom_pz, bE);
      GetHisto("MCMassHadrT")->Fill((jet + jet2 + b).M(), EventWeight);
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




    Int_t NIsoMuon = 0;

    int leadMuIndex;      // index of the leading muon, inside the Muon tuple.

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // count NIsoMuon and store the 2 with larger pt
    for (int j = 0; j < NMuon; j++)
    {
      muon.SetPxPyPzE(Muon_Px[j], Muon_Py[j], Muon_Pz[j], Muon_E[j]); 
      //jet.SetPxPyPzE(Muon_Px[0], Muon_Py[0], Muon_Pz[0], Muon_E[0]); 
      
      GetHisto("Muon_Iso")->Fill(Muon_Iso[j]/muon.Pt(), EventWeight);
      
      if (Muon_Iso[j]/muon.Pt() < 0.1)
      {
        NIsoMuon ++;
        if (NIsoMuon == 1) {leadMuon = muon; leadMuIndex = j;} // bigger pt
        if (NIsoMuon == 2) {muon2 = muon;}                     // lower pt
        if (leadMuIndex != 0)
        {
          //std::cout << jet.Pt() << ' ' << leadMuon.Pt() << std::endl;
        }
      }
      
    } // https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonIsolation 
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    // Z mass >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    if (NIsoMuon > 1 && triggerIsoMu24)
    {
      if(leadMuon.Pt() > MUON_MIN_PT)//|| muon2.Pt() > 20)
      {
        GetHisto("MuMuMass")->Fill((leadMuon + muon2).M(), EventWeight);
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    
    // BTag Eff >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    int bJets = 0;
    bool jetIsB[20]; std::fill_n(jetIsB, 20, false); //init the array with zeros

    if (NJet > 0 && process == "ttbar")
    {
      // bTags generados -------------------------------------------------------
      TLorentzVector JetHadrGEN;
      TLorentzVector JetLeptGEN;

      JetHadrGEN.SetPxPyPzE(MChadronicBottom_px, MChadronicBottom_py, \
                              MChadronicBottom_pz, INFINITY);

      JetLeptGEN.SetPxPyPzE(MCleptonicBottom_px, MCleptonicBottom_py, \
                              MCleptonicBottom_pz, INFINITY);


      if (JetHadrGEN.Pt() != 0 && JetLeptGEN.Pt() != 0) {

        //std::cout << JetLeptGEN.Eta() << std::endl;
        // only semileptonic events

        // identificamos jets en reco
        float DRHadr, auxDRHadr = 999; int indexHadr = -1;
        float DRLept, auxDRLept = 999; int indexLept = -1;
        bool matchSuccess = false;

        for (int j = 0; j < NJet; j++)
        {
          GetHisto("Jet_btag")->Fill(Jet_btag[j], EventWeight);
          if (Jet_btag[j] > BTAG_LIM && jet.Pt() > JET_MIN_PT)
          { // si es bTag
            bJets ++;
            jetIsB[j] = true;

            jet.SetPxPyPzE(Jet_Px[j], Jet_Py[j], Jet_Pz[j], Jet_E[j]);

            // macheamos reco con gen
            DRHadr = DR(JetHadrGEN, jet);
            DRLept = DR(JetLeptGEN, jet);
            if(DRHadr < DR_MAX_JETS && DRHadr < auxDRHadr && DRHadr < DRLept){
              if (auxDRHadr != 999) std::cout << "WTFFFFF Lept" << std::endl;
              auxDRHadr = DRHadr;
            }
            if(DRLept < DR_MAX_JETS && DRLept < auxDRLept && DRLept < DRHadr){
              if (auxDRLept != 999) std::cout << "WTFFFFF Hadr" << std::endl;
              auxDRLept = DRLept;
            }            
          }
        }
        // hay jets que puedo encajar tanto en hadr como en lept...

        // jets totales reco: NJet
        // jets totales reales: 2
        // jets etiquetados como b: BJet
        // jets que realmente son un b: <= 2, NRecoJets

        int NRecoJets = 0;
        if (auxDRHadr != 999) {NRecoJets ++;}
        if (auxDRLept != 999) {NRecoJets ++;}

        // si lo que pensabamos que era un BJet lo es, ponemos esta var. a true:

        GetHisto("JetMatchSuccess")->Fill(bJets - NRecoJets, EventWeight);
        

        GetHisto("JetMatchedRECO")->Fill(NRecoJets, EventWeight);
        GetHisto("JetBTaggedRECO")->Fill(bJets, EventWeight);
        


//        GetHisto("Hadr_BJet_Pt_GEN")->Fill(JetHadrGEN.Pt(), EventWeight);
//
//
//
//        GetHisto("Hadr_BJet_Pt_GEN")->Fill(JetHadrGEN.Pt(), EventWeight);
//        GetHisto("Lept_BJet_Pt_GEN")->Fill(JetLeptGEN.Pt(), EventWeight);
//
//        if (JetHadrGEN.Pt() > JET_MIN_PT && JetHadrGEN.Pt() < JET_MAX_PT){
//          GetHisto("Jets_GEN_Pt")->Fill(JetHadrGEN.Pt(), EventWeight);
//        }
//        if (JetLeptGEN.Pt() > JET_MIN_PT && JetLeptGEN.Pt() < JET_MAX_PT){
//          GetHisto("Jets_GEN_Pt")->Fill(JetLeptGEN.Pt(), EventWeight);
//        }
      }
    }

    // =========================================================================
    if (NIsoMuon != 1) {continue;} //  TTBAR ANALYSIS 
    // =========================================================================




    bJets = 0; std::fill_n(jetIsB, 20, false);
    for (int j = 0; j < NJet; j++)
    {
      if (Jet_btag[j] > BTAG_LIM)
      { // si es bTag
        //if (process != "ttbar") std::cout << Jet_btag[j] << std::endl;
        bJets ++;
        jetIsB[j] = true;
      }
    }   
    



    // Aceptancia, GEN >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    if (TMath::Abs(MCleptonPDGid[leadMuIndex]) == 13) // solo los muones
    {
      muon.SetPxPyPzE(MClepton_px[leadMuIndex], MClepton_py[leadMuIndex], 
                                                  MClepton_pz[leadMuIndex], 0);
  
      if (muon.Pt() > MUON_MIN_PT)
      {
        GetHisto("Acep_gen")->Fill(0.0, EventWeight);
      }
    }
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    

    GetHisto("MuonPt_raw")->Fill(leadMuon.Pt(), EventWeight);

     
    if (triggerIsoMu24) 
    {
      GetHisto("MuonPt_TriggOnly")->Fill(leadMuon.Pt(), EventWeight);

    
      // Aceptancia >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (leadMuon.Pt() > MUON_MIN_PT)
      {
        GetHisto("Acep_obs")->Fill(0.0, EventWeight);
      }
      // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      

      if (leadMuon.Pt() > MUON_MIN_PT)
      {

        // compute TMass from Data >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (NJet >= 4 && bJets >= 2) 
        {
          float Mjj = 0; float auxMjj = 0;

          for (int j = 0; j < NJet; j++) // hadronico
          {
            for (int k = 0; k < NJet; k++)
            {
              if (!jetIsB[j] && !jetIsB[k] && j != k) // if it is not a b Jet
              {
                jet.SetPxPyPzE(Jet_Px[j], Jet_Py[j], Jet_Pz[j], Jet_E[j]);
                jet2.SetPxPyPzE(Jet_Px[k], Jet_Py[k], Jet_Pz[k], Jet_E[k]);

                auxMjj = (jet + jet2).M();
                if ((abs(auxMjj - 81) < abs(Mjj - 81)) && abs(auxMjj - 81) < 30)
                {
                  Mjj = auxMjj;
                }
              }
            }
          }
          GetHisto("MassHadrW")->Fill(Mjj, EventWeight);


          TLorentzVector nuAux;
          nuAux.SetPx(MET_px); nuAux.SetPy(MET_py);
          nuAux.SetE(TMath::Power(MET_px*MET_px + MET_py*MET_py, 0.5));
         
          float MLeptW = (nuAux + leadMuon).Mt();
          if (abs(auxMjj - 81) < 30)
          GetHisto("MassLeptW")->Fill(MLeptW, EventWeight);
        }


        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


        bJets = 0;    
        std::vector <int> indexBJets = {}; 
        int nJetWithBigPt = 0;    // useless  
        if (NJet > 3)  
        {
          for (int j = 0; j < NJet; j++)
          {
            jet.SetPxPyPzE(Jet_Px[j], Jet_Py[j], Jet_Pz[j], Jet_E[j]);
            
            if (Jet_btag[j] > BTAG_LIM && jet.Pt() > JET_MIN_PT)
            {
              bJets ++;
              indexBJets.push_back(j);
            }
            
          }
            //std::cout << "ASDASDSASDSDSDA" << std::endl;
          if (bJets > 0)
          {
            GetHisto("MuonPt")->Fill(leadMuon.Pt(), EventWeight); 
            //GetHisto("MuonPt")->Fill(leadMuon.Pt(), weight); 
            GetHisto("TempXSec")->Fill(1., EventWeight); 
            
          }  
        } 

        for (int j = 0; j < bJets; j++)
        {
          jet.SetPxPyPzE(Jet_Px[indexBJets[j]], Jet_Py[indexBJets[j]], \
                            Jet_Pz[indexBJets[j]], Jet_E[indexBJets[j]]);
          
          if (jet.Pt() > JET_MIN_PT && jet.Pt() < JET_MAX_PT)
            GetHisto("BJet_Pt")->Fill(jet.Pt(), EventWeight); 

        }
        //std::cout << MChadronicWDecayQuark_px << std::endl;

      }
    }



    // <<<<<<<<<<<<<<<<<<<<<<<<>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  }


  //std::cout << "counted " << nObs << std::endl;
}

