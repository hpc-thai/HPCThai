from math import log

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
    
def norm(b, p):
    while (True):
        if (b < 0):
            b += p
        elif (b > p):
            b -= p
        else:
            break
    return b

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
    
def worker(threadNo):
    import zmq
       
    context = zmq.Context()
    receiver = context.socket(zmq.PULL)
    receiver.connect("tcp://localhost:6666")
    sender = context.socket(zmq.PUSH)
    sender.connect("tcp://localhost:7777")
    
    print("worker %s ready." % threadNo)
    while True:
        s = receiver.recv_string()
        list1 = s.split('/')
        k   = int(list1[0])
        pp  = int(list1[1])
        sender.send_string("%s/%s" % (computeBkModP(pp, k), pp))
   
if __name__ == "__main__":
    import multiprocessing as mp
    lock1 = mp.Lock()
    nthreads = mp.cpu_count()
    threads = [mp.Process(target=worker, args=(threadNo,))
        for threadNo in range(nthreads)]
    [t.start() for t in threads ]
    [t.join() for t in threads]
    