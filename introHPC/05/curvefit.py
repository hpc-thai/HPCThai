import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
    print('usage: python curvefit.py outfile max_estimate')
    sys.exit()
  
x, t = np.loadtxt(sys.argv[1])
max_estimate = int(sys.argv[2])

r = np.polyfit(x, t, 10)
te = np.poly1d(r)
print('Estimate B(22) used %.2f secs' % te(22))
xx = np.arange(0,x[-1]+.5,.5)
plt.plot(x, t, 'rx', xx, te(xx), 'b-')
plt.title('Curve Fitting Polynomail Degree 10')
plt.xlabel("Bm")
plt.ylabel("Time used (secs)")
plt.grid()

plt.figure(2)
plt.title('Estimate time used to B(%s)' % max_estimate)
xx = np.arange(0, max_estimate+0.5, .5)
tt = te(xx)
plt.plot(xx, tt, 'r-')
plt.xlabel("Bm")
plt.ylabel("Time used (secs)")
plt.grid()
plt.show()

