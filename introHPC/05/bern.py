import sys
import time
import numpy as np
import matplotlib.pyplot as plt

def calcBm(module, max_m):
    t = []
    x = range(0, max_m+1, 2)
    for m in x:
        stime = time.time()
        module.B(m)
        etime = time.time() - stime
        t =  t + [etime]
        print("B(%3d) = %7.2f secs" % (m, etime))
    return (x, t)
        
def drawGraph(x, t, filename, title):     
    x = np.asarray(x)
    y = np.asarray(t)

    plt.plot(x, y, linewidth=2, color='r')
    plt.xlabel("Bm")
    plt.ylabel("Time used (secs)")
    plt.title(title)
    plt.grid()
    pngFile = filename + '.png'
    plt.savefig(pngFile)   
    print("file %s is generated" % pngFile)
    #plt.show()

def saveData(x, t, filename):
    outFile = filename + '.out'
    np.savetxt(outFile, (x, t))
    print("file %s is generated" % outFile)
algors = {
    'r1' : ( 'b_recur_v1', 'Bernoulli Recursive v1'),
    'r2' : ( 'b_recur_v2', 'Bernoulli Recursive v2'),
    'r3' : ( 'b_recur_v3', 'Bernoulli Recursive v3'),
} 
    
if len(sys.argv) != 3 :
    print("usage: python bern.py algor number")
    sys.exit()
    
algorString = sys.argv[1]
max_m = int(sys.argv[2])

if algorString not in algors:
    print('unknown algor: %s' % algorString)
    sys.exit()
    
(moduleString, title) = algors[algorString]
module = __import__(moduleString)

(x, t) = calcBm(module, max_m)
drawGraph(x, t, moduleString, title)
saveData(x, t, moduleString)

