import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
    print('usage: python curvefit.py outfile max_estimate')
    sys.exit()
  
x, t = np.loadtxt(sys.argv[1])
max_estimate = int(sys.argv[2])
baseFileName = sys.argv[1][:-4]

degree =  min(10, len(x) - 1)
r = np.polyfit(x, t, degree)
fit = np.poly1d(r)
next = int(x[-1]+2)
print('Estimate B(%s) used %.2f secs' % (next, np.abs(fit(next))))
print('Estimate B(100) used %.2f secs' % (fit(100)))
xx = np.arange(0,x[-1]+.5,.5)
plt.plot(x, t, 'rx', xx, np.abs(fit(xx)), 'b-')
plt.title('Curve Fitting Polynomail Degree %s' % degree)
plt.xlabel("Bm")
plt.ylabel("Time used (secs)")
plt.grid()
fitFileName = baseFileName + '_fit.png'
plt.savefig(fitFileName)
print("file %s is generated" % fitFileName)

plt.figure(2)
plt.title('Estimate time used to B(%s)' % max_estimate)
xx = np.arange(0, max_estimate+0.5, .5)
tt = np.abs(fit(xx))
plt.plot(xx, tt, 'b-')
plt.xlabel("Bm")
plt.ylabel("Time used (secs)")
plt.grid()
estFileName = baseFileName + '_est.png'
plt.savefig(estFileName)
print("file %s is generated" % estFileName)

