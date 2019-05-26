from plotter import Plotter
import numpy as np


# trigger >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
M = np.loadtxt("eff.dat")  # el archivo con los datos

outputName = "eff.png"

x = np.linspace(10, 60, len(M))
y = [elem for elem in M] # eficiencia

xlabel = r"$p_{T, min}$ (GeV)"
ylabel = r"Eficiencia"
title = u"Eficiencia  del trigger en función del corte en el $p_T$ del muón"

eff = Plotter((8, 5))
eff.createAxis(111)

eff.addSubplot(x, y, 0, color="crimson", dataLabel=r"$Trigg_{eff}$")
eff.drawVerticalLine(26, u"$p_T$ = 26 GeV")


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
ylabel = r"Eficiencia"
title = u"bTag eff en función del corte en $g$"
title = u""

plot = Plotter((13, 5))
plot.createAxis(121)



#plot.addSubplot(xM, yM, 0, color="crimson", dataLabel="miss TAG")
#plot.addSubplot(xB, yB, 0, color="b", dataLabel="b TAG")

plot.addSubplot(xM, yM, 0, color="crimson", dataLabel="miss TAG")

plot.drawVerticalLine(1, "g = 1, loose", "red")
plot.drawVerticalLine(3.3, "g = 3.3, normal", "green")
plot.drawVerticalLine(4.3, "g = 4.3, tight", "blue")

plot.setProperties(title, xlabel=xlabel, ylabel=ylabel, legLoc=1)

plot.createAxis(122)
#plot.addSubplot(yM, yB, 0, color="darkolivegreen", dataLabel="bTag Eff / missTag Eff")
plot.addSubplot(xM, yB/yM, 0, color="darkolivegreen", dataLabel="bTag Eff / missTag Eff")



title = "bTag Eff / missTag Eff"
title = ""
#xlabel = "missTagEff"
xlabel = "discriminante"
ylabel = "bTagEff"
ylabel = ""
plot.setProperties(title, xlabel=xlabel, ylabel=ylabel, legLoc=2)

#plot.showGraph()
plot.saveFig("bTagEffvsG")



# t -> g = 1
# normal g = 3.3
# loose - 4.4