from Selector import Selector
from ROOT import TH1F, THStack, TCanvas, TLegend

class Plotter:
  ''' Class to draw histograms and get info from Selector '''
  listOfSelectors = []
  dataSelector = Selector()
  data = ''
  colors = []

  # Default parameters: 
  fLegX1, fLegY1, fLegX2, fLegY2 = 0.70, 0.55, 0.92, 0.92
  LegendTextSize  = 0.065
  xtitle = ''
  ytitle = ''
  title  = ''

  def SetLegendPos(self, x1, y1, x2, y2):
    ''' Change the default position of the legend '''
    self.fLegX1 = x1
    self.fLegY1 = y1
    self.fLegX2 = x2
    self.fLegY2 = y2

  def SetLegendSize(self, t = 0.065):
    ''' Change the default size of the text in the legend '''
    self.LegendTextSize = t

  def SetColors(self, col):
    ''' Set the colors for each MC sample '''
    self.colors = col

  def SetTitle(self, tit):
    ''' Set title of the plot '''
    self.title = tit

  def SetXtitle(self, tit):
    ''' Set title of X axis '''
    self.xtitle = tit

  def SetYtitle(self, tit):
    ''' Set title of Y axis '''
    self.ytitle = tit

  def GetHisto(self, process, name):
    ''' Returns histogram 'name' for a given process '''
    for s in self.listOfSelectors:
       if name not in s.name: continue
       h = s.GetHisto(name)
       return h
    print('WARNING: histo %s for process %s not found!' %(name, process))
    return TH1F()

  def GetEvents(self, process, name):
    ''' Returns the integral of a histogram '''
    return self.GetHisto(process,name).Integral()
  
  def __init__(self, backgrounds, data = ''):
    ''' Initialize a new plotter... give a list with all names of MC samples and the name of the data sample '''
    self.data = data
    self.backgrounds = backgrounds
    counter = 0
    for p in self.backgrounds:
      self.listOfSelectors.append(Selector(p))
      self.colors.append(counter+1)
      counter += 1
    if(self.data != ''): self.dataSelector = Selector(self.data)

  def Stack(self, name):
    ''' Produce a stack plot for a variable given '''
    if(isinstance(name, list)):
      for nam in name:
        self.Stack(nam)
      return
    c = TCanvas('c_'+name, 'c', 10,10,800,600)  
    l = TLegend(self.fLegX1, self.fLegY1, self.fLegX2, self.fLegY2)
    l.SetTextSize(self.LegendTextSize)
    l.SetBorderSize(0)
    l.SetFillColor(10)
    hstack = THStack('hstack_' + name, "hstack")
    counter = 0
    for s in self.listOfSelectors:
       h = s.GetHisto(name)
       h.SetFillColor(self.colors[counter])
       h.SetLineColor(0)
       hstack.Add(h)
       l.AddEntry(h, s.name, "f")
       counter += 1
    hstack.Draw("hist")
    if self.title  != '': hstack.SetTitle(self.title)
    if self.xtitle != '': hstack.GetXaxis().SetTitle(self.xtitle)
    if self.ytitle != '': hstack.GetYaxis().SetTitle(self.ytitle)
    hstack.GetYaxis().SetTitleOffset(1.35)
    Max = hstack.GetStack().Last().GetMaximum()
    if self.data != '':
      hdata = self.dataSelector.GetHisto(name)
      hdata.SetMarkerStyle(20)
      hdata.SetMarkerColor(1)
      hdata.Draw("pesame")
      MaxData = hdata.GetMaximum()
      if(Max < MaxData): Max = MaxData
      l.AddEntry(hdata, self.dataSelector.name, 'p')
    l.Draw("same")
    hstack.SetMaximum(Max*1.1)
    c.Print(name + '.png', 'png')

  def PrintEvents(self, name):
    ''' Print the number of events for each sample in a given histogram '''
    if(isinstance(name, list)):
      for nam in name:
        self.PrintEvents(nam)
      return
    counter = 0
    print '\nPrinting number of events for histogram %s:' %name
    print '----------------------------------------------------'
    total = 0.
    for s in self.listOfSelectors:
       h = s.GetHisto(name)
       print '%s: %f' %(s.name, h.Integral())
       total += h.Integral()
    print 'Total expected: %f' %total
    print '------------------------------'
    if self.data != '':
      hdata = self.dataSelector.GetHisto(name)
      print 'Observed: %i' %hdata.Integral()
      print '------------------------------'

