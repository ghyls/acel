
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
acept = M[2][0]         

totalData = M[3][0]
totalBkg = M[3][1]
print(acept, totalBkg, totalData, RAW_bTAgEff)

# designaremos con MIN al numerador (ej. RAW_bTagEff[0]) y con MAX al
# denominador (ej. RAW_bTagEff[1]) del cociente que de vuelve cada valor (ej. la
# eficiencia en b tag).


MIN_bTAgEff = RAW_bTAgEff[0]
MAX_bTAgEff = RAW_bTAgEff[1]

MIN_trigEff = RAW_trigEff[0]
MAX_trigEff = RAW_trigEff[1]



# inicializamos todas las variables >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

MIN_bTAgEff = sp.Symbol("MIN_bTAgEff")
MAX_bTAgEff = sp.Symbol("MAN_bTAgEff")

MIN_trigEff = sp.Symbol("MIN_trigEff")
MAX_trigEff = sp.Symbol("MAN_trigEff")


N = sp.Symbol("N")
B = sp.Symbol("B")

muonEff = sp.Symbol("muonEff")
lumi = sp.Symbol("lumi")


# y las creamos como objetos de Data, con su error poissoniano >>>>>>>>>>>>>>>>>

variables =    [[MIN_bTAgEff, Data(RAW_bTAgEff[0], RAW_bTAgEff[0]**0.5)], 
                [MAX_bTAgEff, Data(RAW_bTAgEff[1], RAW_bTAgEff[1]**0.5)],
                [MIN_trigEff, Data(RAW_trigEff[0], RAW_trigEff[0]**0.5)],
                [MAX_trigEff, Data(RAW_trigEff[1], RAW_trigEff[1]**0.5)],
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



BRTTbarToMu = (BRTToMuB + BRTToTauB * BRTauToMu) * BRTTo2JB

# definimos las cantidades que introducimos en la fórmula de Xsec >>>>>>>>>>>>>>

bTagEff = MIN_bTAgEff / MAX_bTAgEff   # en datos
bTagEff = 0.59925

trigEff = MIN_trigEff / MAX_trigEff


# y las incertidumbres de las eficiencias?
#   trigger -> (1-eff)/2
#   bt -> eff datos - eff MC

# definimos la formula que devolverá la Xsec >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

func = (N-B) / (lumi * acept * trigEff * muonEff * bTagEff * BRTTbarToMu)

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



