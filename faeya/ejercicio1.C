// Abre el root file
// root -l
//
// EJECUTA ESTO EN UNA SESION INTERACTIVA DE ROOT!!
//
TString path = "./practica/files/ttbar.root";

TFile* f = TFile::Open(path);

// Explora las variables del tree desde la terminal:
events->Print();

// O solo las variables de los muones
events->Print("*Muon*");

// Pinta el momento transverso de los muones en un histograma
events->Draw("sqrt(Muon_Px*Muon_Px + Muon_Py*Muon_Py)");

// Explora el tree y sus variables usando un explorador
TBrowser t

// Para salir de ROOT:
// .q
