
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







m = sp.Symbol('m')
V = sp.Symbol('V')


variables = [[m, Data(3, 0.8)], [V, Data(4.5, 0.4)]]   # [m, V]

func = m/V

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

print("RESULT =  " + str(value) + " pm " + str(error))



