import multiprocessing as mp
from time import sleep

def f(id):
    for i in range(10):
        print("Hello %d: %d" %(id, i))
        sleep(0.1)

if __name__ == '__main__':
    t1 = mp.Process(target=f, args=(1,))
    t2 = mp.Process(target=f, args=(2,))
    
    t1.start();
    t2.start();
    t1.join();
    t2.join();
    print("Done")