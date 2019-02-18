import ROOT
ROOT.gROOT.SetBatch(1) # Para evitar que se abran ventanas mientras se ejecuta


# energia faltante de DY y WW,
# histograma de las dos variables
# histograma con la division

 


# Vamos a comparar el pT suma de dos muones en  ttbar y Drel-Yan
path = './practica/files/'

# Creamos los histogramas de DY y ttbar
hDY = ROOT.TH1F("dimu pT DYw", "", 16, 0, 160) 
htt = ROOT.TH1F("dimu pT tt", "", 16, 0, 160)
#comillas vacias -> titulo
#16 celdas entre 0 y 160

# Vamos a hacer un bucle sobre los sucesos de DY
fDY = ROOT.TFile.Open(path + 'dy.root')

for DYevent in fDY.events:
  # Seleccionamos el suceso solo si tiene dos muones
  if not DYevent.NMuon == 2: continue
  
  # Creamos el cuadrimomento de los muones
  muon1 = ROOT.TLorentzVector()
  muon2 = ROOT.TLorentzVector()

  # obs: hacemos esto tras haber restringido a sucesos con solo 2 muones
  muon1.SetPxPyPzE(DYevent.Muon_Px[0], DYevent.Muon_Py[0], DYevent.Muon_Pz[0], DYevent.Muon_E[0])
  muon2.SetPxPyPzE(DYevent.Muon_Px[1], DYevent.Muon_Py[1], DYevent.Muon_Pz[1], DYevent.Muon_E[1])
  
  # Calculamos el pT de la suma de los muones
  diPt = (muon1 + muon2).Pt()

  # Llenamos el histograma
  hDY.Fill(diPt)

# Lo mismo pero para ttbar
ftt = ROOT.TFile.Open(path + 'ttbar.root')
for ttevent in ftt.events:
  if not ttevent.NMuon == 2: continue
  muon1 = ROOT.TLorentzVector()
  muon2 = ROOT.TLorentzVector()

  muon1.SetPxPyPzE(ttevent.Muon_Px[0], ttevent.Muon_Py[0], ttevent.Muon_Pz[0], ttevent.Muon_E[0])
  muon2.SetPxPyPzE(ttevent.Muon_Px[1], ttevent.Muon_Py[1], ttevent.Muon_Pz[1], ttevent.Muon_E[1])
  diPt = (muon1 + muon2).Pt()
  htt.Fill(diPt)

# Creamos un TCanvas para pintar los dos histogramas
c = ROOT.TCanvas('c', 'c', 10, 10, 800, 600)
# 'c' son nombres. Lo demas dimensiones


# Normalizamos los histogramas a 1
hDY.Scale(1/hDY.Integral())
htt.Scale(1/htt.Integral())

# Cosmetics
hDY.SetLineColor(ROOT.kAzure+2)
htt.SetLineColor(ROOT.kRed+1)
hDY.SetLineWidth(2)
htt.SetLineWidth(2)

# Pintamos con la option "hist"
hDY.Draw("hist")
htt.Draw("hist,same") # En el mismo TCanvas

# Ejes y otras opciones... (solo en el primer histograma que pintamos)
hDY.SetStats(0)
hDY.GetXaxis().SetTitle("p_{T}^{#mu#mu} (GeV)")
hDY.GetYaxis().SetTitle("Events")

# Leyenda
leg = ROOT.TLegend(0.7, 0.7, 0.9, 0.9) # x0, y0, x1, y1
leg.AddEntry(hDY, "DY", "l")
leg.AddEntry(htt, "t#bar{t}", "l")
leg.Draw()

# Guardamos la figura como .png y .pdf
c.Print("DiMuPt.png", "png")
c.Print("DiMuPt.pdf", "pdf")
