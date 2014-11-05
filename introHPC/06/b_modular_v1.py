from fractions import Fraction, gcd
from math import log, ceil, sqrt

def B(k):
    class PrimeTable:
        def __init__(self, size):
            self.size = size
            self.table = [True] * (size+1)
            self.table[0] = False
            self.table[1] = False
          
            limit = int(sqrt(size))
             
            for i in range(2, limit+1):
                if self.table[i] == False:
                    continue
                for r in range(i+i, size+1, i):
                    self.table[r] = False
        def __getitem__(self, k):
            return self.isPrime(k)
            
        def isPrime(self, n):
            return self.table[n]
        
        def nextPrime(self, n):
            for i in range(n+1, self.size+1):
                if self.isPrime(i):
                    return i
            return -1
            
    def computeDk(k, primeTable):
        p = 2
        dk = 1
        while (p != -1):
            if (k % (p-1) == 0):
                dk *= p
            p = primeTable.nextPrime(p)
        return dk
    
    def norm(b, p):
        while (True):
            if (b < 0):
                b += p
            elif (b > p):
                b -= p
            else:
                break
        return b
        
    def computePowerMod(a, k, m): 
        index = int(log(k, 2)) 
        powmod = 1 
        powa = a
        pow2 = 1
    
        for i in range(0, index+1):
            if k & pow2 != 0:
                powmod *= powa
            powa = (powa * powa) % m
            pow2 = pow2 * 2
        return norm(powmod % m, m)
    
    #n x === 1 (mod p)
    def computeInvertMod(n, p):
        (a, b) = (n, p) if n > p else (p, n)
        d = a // b
        r = a % b   
        v = []
        
        while r > 1:
            v = [(a, b, -d)] + v
            (a, b) = (b, r)
            d = a // b
            r = a % b
        
        (m1, aa, m2, bb) = (1, a, -d, b)
      
        for (a, b, d) in v:
            m1 += (m2 * d)
            bb = a
            (m1, aa, m2, bb) = (m2, bb, m1, aa)
        rr = m1 if n == aa else m2
        return norm(rr, p)     
        
    def computeOrderEp(a, p):
        for k in range(1, p):
            if computePowerMod(a, k, p) == 1:
                return k
        raise 
        
    def computePrimitiveRoot(p):
        if p == 2:
            return 1
        for g in range(2, p):
            if computeOrderEp(g, p) == p - 1:
                return g
        raise
    
    def computeBkModP(p, k):
        if k == 0:
            return 1
        if k == 1:
            return -1 if p == 2 else (p-1)/2
        if (k & 1):
            return 0
        if (p <= 3):
            return -1
        m = k % (p-1)
        if m == 0:
            return -1
        g = computePrimitiveRoot(p)
        r = computePowerMod(g, k-1, p);
        u = (g + (0 if g & 1 else p) - 1) // 2
        S = 0 ; X = 1 ; Y = r
        for i in range(1, (p-1)//2 + 1):
            q = norm( (g * X) // p, p)
            h = norm( (q-u) % p, p)
            t = norm( (h * Y) % p, p)
            S = norm( (S - t) % p, p)
        
            X = norm( (g * X) % p, p)
            Y = norm( (r * Y) % p, p)
        ans = computePowerMod(g, k, p)
        ans = computeInvertMod(p + 1 - ans, p)
        ans = norm( (S * ans) % p, p)
        ans = norm( (2 * ans) % p, p)
        ans = norm( (k * ans) % p, p)
        return ans
     
    def congruent(a, b, m): 
        g = gcd(a, m)
        if b % g != 0: 
            return []
        a_ = a // g
        b_ = b // g
        m_ = m // g
    
        while True:
            if b_ % a_ == 0:
                break
            b_ = b_ + m_
        return b_ // (a_)
        
    def computeCRT(residue):
        mul_m = 1
        for (_, m) in residue:
            mul_m *= m
        z = 0
        for (b, m) in residue:
            M = (mul_m) // m
            X = congruent(M, 1, m)
            z += M * X * b
        while z > 0:
            z = z - mul_m
        return z + mul_m
#B(m) code
    bernn_base = [Fraction(1,1), Fraction(-1, 2), Fraction(1, 6)]
    if k < 3:
        return bernn_base[k]
    
    if k & 1:
        return Fraction (0, 1)
    Y = int(max(37.0, ceil((k + 0.5)*log(k, 2))))
    primeTable = PrimeTable(Y)
    dk = computeDk(k, primeTable)
    beta = ceil( (k+0.5) * log(k, 2) - 4.094 * k + 2.470 + log(dk, 2))
  
    p = 3 ; M_ = 1
    TwoPowerBetaPlusOne = 2 ** (beta + 1)
    while M_ < TwoPowerBetaPlusOne:
        p = primeTable.nextPrime(p)
        if k % (p - 1) != 0:
            M_ *= p
    X = p
    p = 2
    rp = []
    M = 1
    while (p <= X):
        if k % (p-1) != 0:
            rp = rp + [ (computeBkModP(p, k), p) ]
            M = M * p
        p = primeTable.nextPrime(p)
    R = computeCRT(rp)
   
    N_ = norm( dk * R % M, M)
    if k % 4 == 2:
        nk = N_
    else:
        nk = N_ - M
   
    return(Fraction(nk, dk))
