from math import factorial

def binomial(m, r):
    return factorial(m) / factorial(m-r) / factorial(r)
    
m = 20

from pyspark import SparkContext
sc = SparkContext(appName="solve")

from sympy import symbols, Poly, summation



blist = range(m+1, -1, -1)
b_rdd = sc.parallelize(blist).map(lambda k : binomial(m+1, m+1-k) )

i, n = symbols('i, n', integer=True)

plist = Poly(summation(i**m, (i, 1, n)), n).all_coeffs()
p_rdd = sc.parallelize(plist).map(lambda num : num / plist[0])

bernoulli = p_rdd.zip(b_rdd).map(lambda (x, y) : x / y).collect()
print(bernoulli)


