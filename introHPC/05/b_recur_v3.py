from math import factorial
from fractions import Fraction

binoDict = {}
bernDict = {}
def B(m):

        
    def binomial(n, r):
        if (n, r) in binoDict:
            return binoDict[(n, r)]
        b = Fraction(factorial(n) //  (factorial(r) * factorial(n-r)))
        
        binoDict[(n, r)] = b
        return b
    
    if m == 0:
        return Fraction(1, 1)
    if m == 1:
        return Fraction(1, 2)
    if m % 2 == 1:
        return 0
    if m in bernDict:
        return bernDict[m]
    sigma = Fraction(0, 1)
    for k in range(m):
        sigma += binomial(m, k) * B(k) / (m - k + 1)
    ans =  Fraction(1,1) - sigma
    bernDict[m] = ans
    return ans
