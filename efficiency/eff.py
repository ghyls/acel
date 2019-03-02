from plotter import Plotter
import numpy as np



M = np.loadtxt("eff.dat")  # el archivo con los datos

outputName = "eff.png"

x = np.linspace(10, 60, len(M))
y = [elem for elem in M] # eficiencia

xlabel = r"$p_T$ $min$ (GeV)"
ylabel = r"Eficiencia"
title = u"Eficiencia en función del corte en $p_T$"

eff = Plotter((8, 5))
eff.createAxis(111)

eff.addSubplot(x, y, 0, color="crimson", dataLabel="eficiencia")
eff.drawVerticalLine(26.6667, u"$p_T$ = 26.7 GeV")


eff.setProperties(title, xlabel=xlabel, ylabel=ylabel, legLoc=4, xlim=[10, 60])

eff.saveFig(outputName)



