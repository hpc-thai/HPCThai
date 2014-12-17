from fractions import Fraction
from math import log, ceil, sqrt
import zmq

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

def norm(b, p):
    while (True):
        if (b < 0):
            b += p
        elif (b > p):
            b -= p
        else:
            break
    return b
    
def computeDk(k, primeTable):
    p = 2
    dk = 1
    while (p != -1):
        if (k % (p-1) == 0):
            dk *= p
        p = primeTable.nextPrime(p)
    return dk      

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
    
def congruent(a, b, m):
    if (a, b) == (1, 1):
        return 1
    inv = computeInvertMod(a, m)
    return  (b * inv) % m
        
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

def distribute(k, primeList):
    global sender, receiver
    for p in primeList:
        s = "%s/%s" % (k, p)
        sender.send_string(s)
    rp = []
    count = 0
    for _ in primeList:
        r = receiver.recv_string();
        count = count + 1
        list1 = r.split("/")
        tt = int(list1[0])
        pp = int(list1[1])
        rp.append((tt, pp))
    return rp
        
def B(k):
    bernn_base = [Fraction(1,1), Fraction(-1, 2), Fraction(1, 6)]
    if k < 3:
        return bernn_base[k]
    
    if k & 1:
        return Fraction (0, 1)
    Y = int(max(37.0, ceil((k + 0.5)*log(k, 2))))
    primeTable = PrimeTable(Y)
    dk = computeDk(k, primeTable)
    beta = int(ceil( (k+0.5) * log(k, 2) - 4.094 * k + 2.470 + log(dk, 2)))
  
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
    primeList = []
    while (p <= X):
        if k % (p-1) != 0:
            primeList = primeList + [p]
            M = M * p
        p = primeTable.nextPrime(p)
    
    rp = distribute(k, primeList)
    R = computeCRT(rp)
    N_ = norm( dk * R % M, M)
    
    if k % 4 == 2:
        nk = N_
    else:
        nk = N_ - M
    return(Fraction(nk, dk))

if __name__ == "__main__": 
    import time
    global sender, receiver
    
    context = zmq.Context()    
    sender = context.socket(zmq.PUSH)
    sender.bind("tcp://*:6666")
    
    receiver = context.socket(zmq.PULL)
    receiver.bind("tcp://*:7777")
    
    while (True):
        sum = 0
        k = int(input(">>> "))       
        print("B(%s) = " % k)
        stime = time.time()
        bk = B(k)          
        print(B(k))
        etime = time.time()      
        print("elapsed time : %.2f secs" % (etime - stime))
        