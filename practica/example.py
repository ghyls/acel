from Selector import Selector
from Plotter import Plotter
import ROOT

ROOT.gROOT.SetBatch(1) # To avoid opening windows

# Create a selector for ttbar sample
#mySelector = Selector('ttbar')
# Obtain the histogram of invariant mass of the muons from the selector
#h = mySelector.GetHisto('InvMass')
# Do whatever with it...
#print 'The histogram InvMass for the ttbar sample has %i entries' %h.GetEntries()


#seleccionar solo un muon y pintar histograma del numero de jets


# Create an object plotter thar contains all MC samples and uses data:
MCsamples = ['qcd', 'wjets', 'ww', 'wz', 'zz', 'dy', 'single_top', 'ttbar']
myPlot = Plotter(MCsamples, 'data')

# Set colors for each process... We can use colors defined in ROOT
colors = [ROOT.kGray, ROOT.kBlue-1, ROOT.kTeal-1, ROOT.kTeal+1, ROOT.kTeal+4, ROOT.kAzure-8, ROOT.kOrange+1, ROOT.kRed+1]
myPlot.SetColors(colors)

# Set other plotting options (Legend position, size, titles, etc...)
myPlot.SetLegendPos(0.62, 0.6, 0.88, 0.88)
myPlot.SetXtitle('p_{T}^{#mu} (GeV)')
myPlot.SetYtitle('Events')
myPlot.SetTitle('')

# Draw the stack plot for data and simulation
myPlot.Stack('MuonPt')

myPlot.SetXtitle('m_{#mu#mu} (GeV)')
myPlot.Stack('DiMuonMass')

# Print the contributions for each background and observed data
myPlot.PrintEvents('MuonPt')
