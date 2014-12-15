import multiprocessing as mp

MAX = 10000
nthreads = 2
  
def worker(lock1, d, threadNo, sumWorkers):
    sumLocal = 0
    while True:
        with lock1:
            if  d.empty() == True: 
                break
            sumLocal += d.get(block=False)
    sumWorkers.put(sumLocal)
    print("Thread[%d]: %d" %(threadNo, sumLocal))
    
if __name__ == '__main__':
    d = mp.Queue()
    lock1 = mp.Lock()
    sumWorkers = mp.Queue()
    [d.put(i) for i in range (1, MAX+1)]

    threads = [mp.Process(target=worker, args=(lock1, d, i, sumWorkers)) 
        for i in range(0, nthreads)]
    [t.start() for t in threads]
    [t.join() for t in threads]
    sum = 0
    while sumWorkers.empty() == False:
        sum += sumWorkers.get()
    
    print("Sum should be: %d" % (MAX / 2.0 * (MAX+1)))
    print("Sum calculate: %d" % sum)