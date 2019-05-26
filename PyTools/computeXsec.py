
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
RAW_acep = M[2]         # los que veo       | los totales   

totalData = M[3][0]
totalBkg = M[3][1]

normBkg = M[4][0]
statBkg = M[4][1]
AcepCentral = M[5][0]

# designaremos con NUM al numerador (ej. RAW_bTagEff[0]) y con DEN al
# denominador (ej. RAW_bTagEff[1]) del cociente que de vuelve cada valor (ej. la
# eficiencia en b tag).

NUM_bTAgEff = RAW_bTAgEff[0]
DEN_bTAgEff = RAW_bTAgEff[1]
NombTAgEff = RAW_bTAgEff[0]/RAW_bTAgEff[1]

NUM_trigEff = RAW_trigEff[0]
DEN_trigEff = RAW_trigEff[1]
NomTrigEff = RAW_trigEff[0]/RAW_trigEff[1]

NUM_acep = RAW_acep[0]
DEN_acep = RAW_acep[1]

# inicializamos todas las variables >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

NUM_bTAgEff = sp.Symbol("NUM_bTAgEff")
DEN_bTAgEff = sp.Symbol("DEN_bTAgEff")
bTAgEff = sp.Symbol("bTAgEff")
trigEff = sp.Symbol("trigEff")

NUM_trigEff = sp.Symbol("NUM_trigEff")
DEN_trigEff = sp.Symbol("DEN_trigEff")

NUM_acep = sp.Symbol("NUM_acep")
DEN_acep = sp.Symbol("DEN_acep")

N = sp.Symbol("N")
B = sp.Symbol("B")
Bstat = sp.Symbol("Bstat")


muonEff = sp.Symbol("muonEff")
lumi = sp.Symbol("lumi")


# y las creamos como objetos de Data, con su error poissoniano >>>>>>>>>>>>>>>>>

'''[NUM_bTAgEff, Data(RAW_bTAgEff[0], RAW_bTAgEff[0]**0.5)],''' 
'''[DEN_bTAgEff, Data(RAW_bTAgEff[1], RAW_bTAgEff[1]**0.5)],'''
variables =    [
                [bTAgEff, Data(NombTAgEff, NombTAgEff*0.1)],
                [trigEff, Data(NomTrigEff, (1-NomTrigEff)/2)],
                [NUM_acep, Data(RAW_acep[0], RAW_acep[0]**0.5)],
                [DEN_acep, Data(RAW_acep[1], RAW_acep[1]**0.5)],
                [N, Data(totalData, totalData**0.5)],
                [B, Data(totalBkg, normBkg)],      # bkg Norm Unc
                [Bstat, Data(totalBkg, statBkg)],      # bkg Stat Unc
                [muonEff, Data(0.99, 0.01)],
                [lumi, Data(50, 5)]]
                # take care before you edit me :P

print("data", totalData, totalData**0.5)
print("Bnorm", totalBkg, normBkg)
print("Bstar", totalBkg, statBkg)
print("trigg", NomTrigEff, (1-NomTrigEff)/2)
print("btag", NombTAgEff, NombTAgEff*0.1)
# que son las incertidumbres de normalizacion? 
# definimos algunos valores mas, sin error asociado. >>>>>>>>>>>>>>>>>>>>>>>>>>>

BRTToMuB = 0.134
BRTTo2JB = 0.665

# CALCULA EL NUMERO DE EVENTOS ANTES DE PESAR!

BRTTbarToMu = (BRTToMuB) * BRTTo2JB * 2


#print(BRTTbarToMu)

# definimos las cantidades que introducimos en la fórmula de Xsec >>>>>>>>>>>>>>
#print("acep,", NUM_acep/DEN_acep/BRTTbarToMu, "bteff", NUM_bTAgEff/DEN_bTAgEff,
#        "treff", NUM_trigEff/DEN_trigEff)

bTagEff = bTAgEff   # en MC
acep = NUM_acep / (DEN_acep*BRTTbarToMu)


# y las incertidumbres de las eficiencias?
#   trigger -> (1-eff)/2
#   bt -> eff datos - eff MC

# definimos la formula que devolverá la Xsec >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
func = (N-B) / (lumi * acep * trigEff * muonEff * bTagEff * BRTTbarToMu)

#print("acep, ", AcepCentral)
#print("bteff, ", NombTAgEff)
#print("tff, ", RAW_trigEff[0]/RAW_trigEff[1])
#print("nSignal, ", totalData-totalBkg)
#print("BR, ", BRTTbarToMu)
#print((totalData-totalBkg)/(50*RAW_acep[0]/RAW_acep[1]/BRTTbarToMu * RAW_trigEff[0]/RAW_trigEff[1]*0.99*NombTAgEff * BRTTbarToMu))
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

print("acep, error" , AcepCentral,  computeError(acep, variables))
error = computeError(func, variables)

# We redefine func, since we don't calculate the error of the acceptance with
# the formula that gives the central value.

func2 = (N-B) / (lumi * AcepCentral * trigEff * muonEff * bTagEff * BRTTbarToMu)
func3 = (N-Bstat) / (lumi * AcepCentral * trigEff * muonEff * bTagEff * BRTTbarToMu)
value = computeValue(func2, variables)

# REST OF ERRORS ===============================================================
from copy import deepcopy

def someError(variables, var, var2="", funcc=func): # 
    varContainer = deepcopy(variables)
    
    for elem in varContainer:

        if elem[0].name != var and elem[0].name != var2:
            elem[1].error = 0
    
    return computeError(funcc, varContainer)


# LUMI .........................................................................
errorLumi = someError(variables, "lumi")

# DATA_STAT ....................................................................
errorData = someError(variables, "N")

# BKG_NORM .....................................................................
errorNormBkg = someError(variables, "B")
errorStatBkg = someError(variables, "Bstat", funcc = func3)

# bTag .........................................................................
errorBTag = someError(variables, "bTAgEff")

# acep .........................................................................
errorAcep = someError(variables, "NUM_acep", "DEN_acep")

# trig .........................................................................
errorTrigg = someError(variables, "trigEff")

error = (errorLumi**2 + errorData**2 + errorNormBkg**2 + errorStatBkg**2 + errorBTag**2
            + errorTrigg**2 + errorAcep**2)**0.5  # do not show this to anybody

print("RESULT = %.2f pm %.1f (TOTAL)\n"
                        "                pm %.1f (lumi)\n"
                        "                pm %.1f (data stat)\n"                            
                        "                pm %.1f (bkg norm) \n"
                        "                pm %.1f (bkg stat) \n"
                        "                pm %.1f (bTag) \n"
                        "                pm %.1f (trigger) \n"
                        "                pm %.1f (acep)" % 
    (value, error, errorLumi, errorData, errorNormBkg, errorStatBkg, errorBTag,
            errorTrigg, errorAcep))



