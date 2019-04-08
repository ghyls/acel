from plotter import Plotter
import numpy as np


# trigger >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
#eff.showGraph()
eff.saveFig(outputName)

# B Tag >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

BT = np.loadtxt("bTag.dat")  # el archivo con los datos
MT = np.loadtxt("missTag.dat")  # el archivo con los datos

xB = np.array([elem[0] for elem in BT])
yB = np.array([elem[1] for elem in BT])
xM = np.array([elem[0] for elem in MT])
yM = np.array([elem[1] for elem in MT])

xlabel = r"discriminante"
ylabel = r"BTag Eff"
title = u"bTag eff en función del corte en $g$"

plot = Plotter((11, 5))
plot.createAxis(121)

plot.addSubplot(xM, yM, 0, color="crimson", dataLabel="miss TAG")
plot.addSubplot(xB, yB, 0, color="b", dataLabel="b TAG")

plot.setProperties(title, xlabel=xlabel, ylabel=ylabel, legLoc=1)

plot.createAxis(122)
plot.addSubplot(xB, yB/yM, 0, color="darkolivegreen", dataLabel="data")

title = "bTag Eff / missTAG"
xlabel = "bTAgEff"
ylabel = "missTAGEff"
plot.setProperties(title, xlabel=xlabel, ylabel=ylabel, legLoc=2)


plot.saveFig("bTagEffvsG")
