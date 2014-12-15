from threading import Thread, Lock
from functools import reduce

MAX = 10000
nthreads = 2

d = []
def worker(lock1, threadNo, sumWorkers):
    global d
    sumLocal = 0
    while True:
        with lock1:
            if  len(d) == 0: 
                break
            sumLocal += d[0]
            d = d[1:]
        sumWorkers[threadNo] = sumLocal
    print("Thread[%d]: %d" %(threadNo, sumWorkers[threadNo]))

if __name__ == '__main__':
    lock1 = Lock()
    d = [i for i in range(1, MAX+1)]
    sumWorkers = [0] * nthreads
    threads = [Thread(target=worker, args=(lock1, i, sumWorkers)) 
        for i in range(0, nthreads)]
    [t.start() for t in threads]
    [t.join() for t in threads]
    sum = reduce(lambda a, b : a+b, sumWorkers)
    print("Sum should be: %d" % (MAX / 2.0 * (MAX+1)))
    print("Sum calculate: %d" % sum)