import ROOT
ROOT.gROOT.SetBatch(1) # Para evitar que se abran ventanas mientras se ejecuta


# energia faltante de DY y WW,
# histograma de las dos variables
# histograma con la division

 


# Vamos a comparar el pT suma de dos muones en  ttbar y Drel-Yan
path = './practica/files/'

# Creamos los histogramas de DY y ttbar
DY_MET = ROOT.TH1F("dimu pT DYw", "", 16, 0, 160) 
WW_MET = ROOT.TH1F("dimu pT tt", "", 16, 0, 160)
#comillas vacias -> titulo
#16 celdas entre 0 y 160

# Vamos a hacer un bucle sobre los sucesos de DY
fDY = ROOT.TFile.Open(path + 'dy.root')

for DYevent in fDY.events:
  # Seleccionamos el suceso solo si tiene dos muones

  MET = (DYevent.MET_px**2 + DYevent.MET_py**2)**0.5
  # Creamos el cuadrimomento de los muones
  #muon1 = ROOT.TLorentzVector()
  #muon2 = ROOT.TLorentzVector()

  # obs: hacemos esto tras haber restringido a sucesos con solo 2 muones
  #muon1.SetPxPyPzE(DYevent.Muon_Px[0], DYevent.Muon_Py[0], DYevent.Muon_Pz[0], DYevent.Muon_E[0])
  #muon2.SetPxPyPzE(DYevent.Muon_Px[1], DYevent.Muon_Py[1], DYevent.Muon_Pz[1], DYevent.Muon_E[1])
  
  # Calculamos el pT de la suma de los muones
  #MET = (muon1 + muon2).met()

  # Llenamos el histograma
  DY_MET.Fill(MET)

# Lo mismo pero para ttbar
WW = ROOT.TFile.Open(path + 'ttbar.root')
for wwevent in WW.events:

  MET = (wwevent.MET_px**2 + wwevent.MET_py**2)**0.5


  WW_MET.Fill(MET)




# Creamos un TCanvas para pintar los dos histogramas
c = ROOT.TCanvas('c', 'c', 10, 10, 800, 600)
# 'c' son nombres. Lo demas dimensiones

# Normalizamos los histogramas a 1
DY_MET.Scale(1/DY_MET.Integral())
WW_MET.Scale(1/WW_MET.Integral())


'''
 TH1F *h3 = (TH1F*)h1->Clone("h3");
   h3->SetLineColor(kBlack);
   h3->SetMinimum(0.8);  // Define Y ..
   h3->SetMaximum(1.35); // .. range
   h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the ratio plot


TCanvas *c1=new TCanvas();
c1->Divide(3,3); // divides the canvas into three rows and three columns
c1->cd(1);
myGraph1->Draw("apl");
c1->cd(2);
myGraph2->Draw("apl");
'''

ratios = DY_MET.Clone("ratios")
ratios.Divide(WW_MET)
ratios.Draw("ratios")



# Cosmetics
DY_MET.SetLineColor(ROOT.kAzure+2)
WW_MET.SetLineColor(ROOT.kRed+1)
DY_MET.SetLineWidth(2)
WW_MET.SetLineWidth(2)

# Pintamos con la option "hist"

DY_MET.Draw("hist")
WW_MET.Draw("hist,same") # En el mismo TCanvas
c.Divide(2, 1)
c.cd(1)
WW_MET.Draw() # En el mismo TCanvas


# Ejes y otras opciones... (solo en el primer histograma que pintamos)
DY_MET.SetStats(0)
DY_MET.GetXaxis().SetTitle("Energia faltante")
DY_MET.GetYaxis().SetTitle("Events")

# Leyenda
leg = ROOT.TLegend(0.7, 0.7, 0.9, 0.9) # x0, y0, x1, y1
leg.AddEntry(DY_MET, "DY", "WW")
leg.AddEntry(WW_MET, "t#bar{t}", "WW")
leg.Draw()

# Guardamos la figura como .png y .pdf
c.Print("DiMuPt.png", "png")
c.Print("DiMuPt.pdf", "pdf")
c.Print("ratios.png", "png")
