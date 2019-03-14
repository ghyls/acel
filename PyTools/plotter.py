#coding: utf-8

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit



class Plotter:

    def __init__(self, size):
        self.fig1 = plt.figure(figsize=size)

    def createAxis(self, pos):
        self.ax1 = self.fig1.add_subplot(pos, aspect='auto', facecolor='aliceblue')

    def setProperties(self, title, xlabel='', ylabel='', legLoc = '',
        rmX = False, rmY = False, xlim = []):
    
        self.ax1.set_title(title)
        if xlabel != '': self.ax1.set_xlabel(xlabel, size=12)
        if ylabel != '': self.ax1.set_ylabel(ylabel, size=12)

        #remove axis ticks
        if rmX: self.ax1.tick_params(axis='x', which='both', bottom=False, \
                labelbottom=False)
                
        if rmY: self.ax1.tick_params(axis='y', which='both', left=False, \
                labelleft=False)        

        if xlim != []: self.ax1.set_xlim(xlim[0], xlim[1])

        self.ax1.grid()
        self.ax1.legend(loc=legLoc) if legLoc != '' else self.ax1.legend()

        # [ 2    1 ]
        # [ 6    5 ]
        # [ 3    4 ]   

    def getPoptAndPerr(self, func, x, y, p0):

        popt, pcov = curve_fit(func, x, y, p0=p0)
        perr = np.sqrt(np.diag(pcov))               

        return popt, perr



    def addSubplot(self, x, y, fit = False, func = '', p0 = [], \
        dataLabel='data', fitLabel = '', color='b', fitColor='c'):

        self.ax1.plot(x, y,'.',linewidth=0.5, label=dataLabel, color = color)


        if fit: 
               
            popt = self.getPoptAndPerr(func, x, y, p0=p0)[0]
              
            xAux = np.linspace(min(x), max(x), 200)
            self.ax1.plot(xAux, func(xAux, *popt), label=fitLabel, color = fitColor)



    def drawVerticalLine(self, coord, label, color = "darkorange"):
        self.ax1.axvline(x=coord, label = label, color = color)
    
    def showGraph(self):
        plt.show()

    def saveFig(self, outputName):
        self.fig1.savefig(outputName, bbox_inches = 'tight')



def getR2(func, x, y, p0):
     
    popt = curve_fit(func, x, y, p0=p0)[0]
    residuals = y - func(x, *popt)
    ss_res = np.sum(residuals**2)
    ss_tot = np.sum((y-np.mean(y))**2)
    R2 = 1-(ss_res/ss_tot)
    
    return R2


