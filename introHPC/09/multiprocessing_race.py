import multiprocessing as mp

MAX = 10000
nthreads = 2
  
def worker(d, threadNo, sumWorkers):
    print(d.qsize())
    sumLocal = 0
  
    while d.empty() == False:
        sumLocal += d.get(block=False)
    sumWorkers.put(sumLocal)
    print(sumLocal)
if __name__ == '__main__':
    d = mp.Queue()
    sumWorkers = mp.Queue()
    [d.put(i) for i in range (1, MAX+1)]

    threads = [mp.Process(target=worker, args=(d, i, sumWorkers)) 
        for i in range(0, nthreads)]
    [t.start() for t in threads]
    [t.join() for t in threads]
    sum = 0
    while sumWorkers.empty() == False:
        sum += sumWorkers.get()
    
    print("Sum should be: %d" % (MAX / 2.0 * (MAX+1)))
    print("Sum calculate: %d" % sum)