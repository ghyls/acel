
import sympy as sp
import numpy as np



# ------------------------------------------------------------------------------

class Data:
    
    def __init__(self, value, error):

        self.value = value
        self.error = error
        

    value = 0
    error = 0

# ------------------------------------------------------------------------------

M = np.loadtxt("xsec.dat")

RAW_bTAgEff = M[0]      # medidos           | generados
RAW_trigEff = M[1]      # pasan Trigger     | no lo pasan
RAW_acep = M[2]            # los que veo       | los totales   

totalData = M[3][0]
totalBkg = M[3][1]

# designaremos con NUM al numerador (ej. RAW_bTagEff[0]) y con DEN al
# denominador (ej. RAW_bTagEff[1]) del cociente que de vuelve cada valor (ej. la
# eficiencia en b tag).

NUM_bTAgEff = RAW_bTAgEff[0]
DEN_bTAgEff = RAW_bTAgEff[1]

NUM_trigEff = RAW_trigEff[0]
DEN_trigEff = RAW_trigEff[1]

NUM_acep = RAW_acep[0]
DEN_acep = RAW_acep[1]

# inicializamos todas las variables >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

NUM_bTAgEff = sp.Symbol("NUM_bTAgEff")
DEN_bTAgEff = sp.Symbol("MAN_bTAgEff")

NUM_trigEff = sp.Symbol("NUM_trigEff")
DEN_trigEff = sp.Symbol("MAN_trigEff")

NUM_acep = sp.Symbol("NUM_acep")
DEN_acep = sp.Symbol("MAN_acep")

N = sp.Symbol("N")
B = sp.Symbol("B")

muonEff = sp.Symbol("muonEff")
lumi = sp.Symbol("lumi")


# y las creamos como objetos de Data, con su error poissoniano >>>>>>>>>>>>>>>>>

variables =    [[NUM_bTAgEff, Data(RAW_bTAgEff[0], RAW_bTAgEff[0]**0.5)], 
                [DEN_bTAgEff, Data(RAW_bTAgEff[1], RAW_bTAgEff[1]**0.5)],
                [NUM_trigEff, Data(RAW_trigEff[0], RAW_trigEff[0]**0.5)],
                [DEN_trigEff, Data(RAW_trigEff[1], RAW_trigEff[1]**0.5)],
                [NUM_acep, Data(RAW_acep[0], RAW_acep[0]**0.5)],
                [DEN_acep, Data(RAW_acep[1], RAW_acep[1]**0.5)],
                [N, Data(totalData, totalData**0.5)],
                [B, Data(totalBkg, totalBkg**0.5)],
                [muonEff, Data(0.99, 0.01)],
                [lumi, Data(50, 5)]]

# qué son las incertidumbres de normalización? 
# definimos algunos valores más, sin error asociado. >>>>>>>>>>>>>>>>>>>>>>>>>>>

BRTToMuB = 0.134
BRTToTauB = 0.71
BRTauToMu = 0.1739
BRTTo2JB = 0.665

# CALCULA EL NUMERO DE EVENTOS ANTES DE PESAR!

BRTTbarToMu = (BRTToMuB + BRTToTauB * BRTauToMu) * BRTTo2JB * 2

# definimos las cantidades que introducimos en la fórmula de Xsec >>>>>>>>>>>>>>

bTagEff = NUM_bTAgEff / DEN_bTAgEff   # en datos
trigEff = NUM_trigEff / DEN_trigEff
acep = NUM_acep / DEN_acep

print(bTagEff, trigEff, acep)

# y las incertidumbres de las eficiencias?
#   trigger -> (1-eff)/2
#   bt -> eff datos - eff MC

# definimos la formula que devolverá la Xsec >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

func = (N-B) / (lumi * acep * trigEff * muonEff * bTagEff * BRTTbarToMu)

# ------------------------------------------------------------------------------


def computeError(func, variables):

    sum = 0

    for i in range(len(variables)):
        
        partSum = sp.diff(func, variables[i][0])
        
        for elem in variables:
            partSum = partSum.subs(elem[0], elem[1].value)
        
        sum += (partSum * variables[i][1].error)**2

    return sum**0.5

def computeValue(func, variables):

    for elem in variables:
        func = func.subs(elem[0], elem[1].value)

    return func

error = computeError(func, variables)
value = computeValue(func, variables)

print("RESULT = %.2f pm %.1f" % (value, error))



