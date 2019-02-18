

import ROOT as r

from ROOT.TMath import pi


vec1 = r.TLorentzVector()
vec2 = r.TLorentzVector()

vec1.SetPtEtaPhiM(20,2,pi/3,0.15)
vec1.SetPtEtaPhiM(45,1.2,pi/3,0.15)

z = vec1 + vec2

print(z.M())
