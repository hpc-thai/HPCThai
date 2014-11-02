import sys
import time
import numpy as np
import matplotlib.pyplot as plt

algors = {
    'r1' : ( 'b_recur_v1', 'Bernoulli Recursive v1'),
    'r2' : ( 'b_recur_v2', 'Bernoulli Recursive v2'),
}
def calcBm(module, max_m):
    t = []
    for m in range(0, max_m+1, 2):
        stime = time.time()
        module.B(m)
        etime = time.time() - stime
        t =  t + [etime]
        print("B(%3d) = %7.2f secs" % (m, etime))
    return t
        
def drawGraph(t, filename, title):     
    x = np.arange(0, max_m+1, 2)
    y = np.asarray(t)

    plt.plot(x, y, linewidth=2, color='r')
    plt.xlabel("Bm")
    plt.ylabel("Time used (secs)")
    plt.title(title)
    plt.grid()
    plt.savefig(filename + '.png')   # plot.show()
    
    
if len(sys.argv) != 3 :
    print("usage: python %s algor number" % sys.argv[0])
    quit()
    
algorString = sys.argv[1]
max_m = int(sys.argv[2])

if algorString not in algors:
    print('unknown algor: %s' % algorString)
    quit()
(moduleString, title) = algors[algorString]
module = __import__(moduleString)

t = calcBm(module, max_m)
drawGraph(t, moduleString, title)

