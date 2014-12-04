import multiprocessing as mp

MAX = 10000
nthreads = 2

   
def worker(d, threadNo, sumWorkers):
    sum = 0
    while d.empty() == False:
        num = d.get(block=False)
        sum += num
    sumWorkers.put(sum)
    
if __name__ == '__main__':
   
    d = mp.Queue()
    sumWorkers = mp.Queue(nthreads)
    
    [d.put(i) for i in range(1, (MAX+1))]
    threads = [mp.Process(target=worker, args=(d, i, sumWorkers)) 
        for i in range(0, nthreads)]
    [t.start() for t in threads]
    [t.join() for t in threads]
    sum = 0
    print("Got = [", end='')
    while sumWorkers.empty() == False:
        num = sumWorkers.get()
        print("Got : %d, " % num, end='')
        sum += num
    print("]")
    print("Sum should be: %d" % (MAX / 2.0 * (MAX+1)))
    print("Sum calculate: %d" % sum)