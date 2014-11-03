from math import factorial
from fractions import Fraction
    
def B(m):
    def binomial(n, r): 		
        return Fraction(factorial(n) //  (factorial(r) * factorial(n-r)))

    if m == 0:
        return Fraction(1, 1)
    if m == 1:
        return Fraction(1, 2)
    if m % 2 == 1:
        return 0
    sigma = Fraction(0, 1)
    for k in range(m):
        sigma += binomial(m, k) * B(k) / (m - k + 1)
    return Fraction(1,1) - sigma
