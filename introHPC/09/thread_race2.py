from threading import Thread
from functools import reduce

MAX = 10000
nthreads = 2

d = []
def worker(threadNo, sumWorkers):
    global d
    while len(d) != 0:
        num = d[0]
        d = d[1:]
        sumWorkers[threadNo] += num

if __name__ == '__main__':
    d = [i for i in range(1, MAX+1)]
    sumWorkers = [0] * nthreads
    threads = [Thread(target=worker, args=(i, sumWorkers)) 
        for i in range(0, nthreads)]
    [t.start() for t in threads]
    [t.join() for t in threads]
    sum = reduce(lambda a, b : a+b, sumWorkers)
    print("Sum should be: %d" % (MAX / 2.0 * (MAX+1)))
    print("Sum calculate: %d" % sum)