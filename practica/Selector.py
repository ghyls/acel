from ROOT import TFile, TH1F, TLorentzVector
from array import array
import os

class Selector:

  def CreateHistograms(self):
    ''' CREATE YOUR HISTOGRAMS HERE '''
    self.histograms = []
    self.histograms.append(TH1F(self.name+'_MuonPt', ';p_{T}^{#mu} (GeV);Events', 20, 0, 200))
    self.histograms.append(TH1F(self.name+'_DiMuonMass', ';m^{#mu#mu} (GeV);Events', 20, 0, 200))

  def GetHisto(self, name):
    ''' Use this method to access to any stored histogram '''
    for h in self.histograms:
      n = h.GetName()
      if self.name + '_' + name == n: return h
    print '[Selector::GetHisto] WARNING: not found histo ', name
    return TH1F()

  def Loop(self):
    ''' Main method, used to loop over all the entries '''
    f = TFile.Open(self.filename)

    nEvents = f.events.GetEntries()
    print 'Opening file %s... loopeing over %i events...' % (self.filename, nEvents)
    # Loop over the events
    for event in f.events:

      ''' IMPLEMENT YOUR ANALYSIS HERE '''
      # Select events and fill histograms

      if not event.triggerIsoMu24: continue # Events must pass the trigger, pt 24 amnd isolated
      if event.NMuon != 2: continue # Selecting events with 2 muons
      muon1 = TLorentzVector()
      muon1.SetPxPyPzE(event.Muon_Px[0], event.Muon_Py[0], event.Muon_Pz[0], event.Muon_E[0])
      muon2 = TLorentzVector()
      muon2.SetPxPyPzE(event.Muon_Px[1], event.Muon_Py[1], event.Muon_Pz[1], event.Muon_E[1])
      invariantMass = (muon1 + muon2).M()

      weight = event.EventWeight if not self.name == 'data' else 1
      self.GetHisto('MuonPt').Fill(muon1.Pt(), weight)
      self.GetHisto('DiMuonMass').Fill(invariantMass, weight)

  def __init__(self, filename = ''):
    ''' Initialize a new Selector by giving the name of a sample.root file '''
    self.name = filename
    self.filename = self.name
    if self.filename[-5:] != '.root': self.filename += '.root'
    if not os.path.exists(self.filename): self.filename = 'files/' + self.filename
    if not os.path.exists(self.filename):
      if(self.name != ''): print '[Selector] WARNING: not found file ', self.name
    else:
      self.CreateHistograms()
      self.Loop()

