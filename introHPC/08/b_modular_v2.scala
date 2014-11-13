import util.control.Breaks._
import scala.annotation.tailrec
import scala.collection.mutable.Stack

object b_modular_v2 {
    class PrimeTable(size : Int)
	{
	    var _size = size
	    var _table = Array.fill[Boolean](size+1)(true)
	    val limit = math.sqrt(size).toInt
	    for (i <- 2 to limit) {
	        breakable {
		        if (_table(i) == false) break
		        for (r <- i+i to size by i) _table(r) = false
	        }
	    }
	 
	    final def nextPrime(pos : Int) : Int = {
	    	for (i <- pos+1 to size) {
	    	    if (_table(i) == true) return i;
	    	}
	    	return -1;
	    }
	}
    
    def computeDk(k : Int, primeTable : PrimeTable) : BigInt = {
	    var p = 2;
	    var dk = BigInt(1)
	    while (p != -1) {
	        if (k % (p-1) == 0) dk = dk * p
	        p = primeTable.nextPrime(p)
	    }
	    return dk
	}
	
	def norm(b : BigInt, p : BigInt) : BigInt = {
	    var bb = b 
	   
	    breakable {
		    while (true) {
		        if (bb<BigInt(0)) bb = bb + p
		        else if (b > p) bb = bb - p
		        else break
		    }
	    }
	    return bb
	}
	
		def computePowerMod(a : BigInt, k : Int, m : BigInt) : BigInt = {
	    val index = (math.log(k) / math.log(2)).toInt
	    var powmod = BigInt(1)
	    var powa = a
	    var pow2 = BigInt(1)
	    for (i <- 0 to index) {
	        if ( (k & pow2) != 0) powmod = powmod * powa
	        powa = (powa * powa) % m
	        pow2 = pow2 << 1
	    }
	   
	    return norm(powmod %m, m)
	}
	
	def computeInvertMod(n : BigInt, p : BigInt) : BigInt = {
	    var v : Stack[(BigInt, BigInt, BigInt)] = Stack()
	    var a = p
	    var b = n
	    
	    if (n > p) {
	        a = n
	        b = p
	    }
	    var d = a / b
	    var r = a % b
	    
	    while (r > 1) {
	        v.push((a, b, -d))
	        a = b
	        b = r
	        d = a / b
	        r = a % b
	    }
	    var m1 = BigInt(1)
	    var m2 = -d
	    var aa = a
	    var bb = b
	    
	    while (v.isEmpty == false) {
	       var (a, b, d) = v.pop
	     
	        m1 = m1 + (m2 * d)
	        bb = a
	        var temp = m1
	        m1 = m2
	        m2 = temp
	        
	        temp = aa
	        aa = bb
	        bb = temp
	    }
	    var rr = if (n == aa) m1 else m2
	    norm(rr, p)
	}
	
	def computeOrderEp(a : Int, p : Int) : Int = {
	    
	    for (k <- 1 until p) {
	    	if (computePowerMod(a, k, p) == 1) return k
	    }
	     
	    throw new Exception("computerOrderEp error")
	}
	
	def computePrimitiveRoot(p : Int) : Int = {
	    if (p == 2) return 1
	    for (g <- 2 until p) {
	        if (computeOrderEp(g, p) == (p - 1)) return g;
	    }
	     
	    throw new Exception("computePrimitiveRoot error")
	}
	
	def computeBkModP(p : Int, k : Int) : BigInt = {
	    if (k == 0) return BigInt(1)
	    if (k == 1) return if (p == 2) BigInt(-1) else BigInt((p-1)/2)
	    if (k % 2 == 1) return BigInt(0)
	    if (p <= 3) return -1
	    
	    var m = k % (p-1)
	    if (m == 0) return -1
	    
	    var g = computePrimitiveRoot(p)
	    var r = computePowerMod(g, k-1, p)
	   
	    var u = ((g + (if (g % 2 == 1) 0 else p)) - 1) / 2
	    var S = BigInt(0)
	    var X = BigInt(1)
	    var Y = r
	    
	    var hp = (p-1)/2
	    var q = BigInt(0)
	    var h = BigInt(0)
	    var t = BigInt(0)
	    
	    for (i <- 1 to hp) {
            q = norm( (g*X)/p, p)
	        h = norm( (q-u)%p, p)
	        t = norm( (h*Y)%p, p)
	        S = norm( (S-t)%p, p)
	        X = norm( (g*X)%p, p)
	        Y = norm( (r*Y)%p, p)
	    }
	    
	    var ans = computePowerMod(g, k, p)
	    ans = computeInvertMod(p+1-ans, p)
        ans = norm( (S * ans) % p, p)
	    ans = norm( (2 * ans) % p, p)
	    ans = norm( (k * ans) % p, p)

	    return ans;
	}

	def congruent(a : BigInt, b : BigInt, m : BigInt) : BigInt = {
	    if (a == BigInt(1) && b == BigInt(1)) return BigInt(1)
	    var inv = computeInvertMod(a, m)
	    return (b * inv) % m
	}
	
	def computeCRT(residue : List[(BigInt, Int)]) : BigInt = {
	    var mul_m = BigInt(1)
	    for ( (b, m) <- residue) {
	    	mul_m = mul_m * m
	    }
	    var z = BigInt(0)
	    var M = BigInt(0)
	    var X = BigInt(0)
	    var b = BigInt(0)
	    var m = BigInt(0)
	     
	    for ((b, m) <- residue) {
	        M = mul_m / m
	        X = congruent(M, 1, m)
	        z = z + (M * X * b)
	    }
	   
	    while (z > 0) {
	        z = z - mul_m
	    }
	    return z + mul_m
	}
	
	def log2(k : Double) =  math.log(k) / math.log(2)  
	
	def B(k : Int) : (BigInt, BigInt) = {
	    
	    val bernBase = Array((BigInt(1), BigInt(1)), (BigInt(1), BigInt(2)), (BigInt(1), BigInt(6)))
	    if (k < 3) return bernBase(k)
	    if (k % 2 == 1) return (BigInt(0), BigInt(1))
	    var Y = (math.ceil( (k+0.5) * log2(k))).toInt
	    Y = if (Y < 37) 37 else Y
	   
	    var primeTable = new PrimeTable(Y)
	    val dk = computeDk(k, primeTable)
	    
	    val beta = (math.ceil ((k + 0.5) * log2(k) - 4.094 * k + 2.470 + log2(dk.toDouble) )).toInt
	    
	    var p = 3
	    var M_ = BigInt(1)
	    var twoPowerBetaPlusOne = BigInt(2).pow(beta+1)
	    
	    while (M_ < twoPowerBetaPlusOne) {
	        p = primeTable.nextPrime(p)
	        if (k % (p-1) != 0) M_ = M_ * p
	    }
	    var X = p
	    var M = BigInt(1)
	    p = 2
	    var rp = List[(BigInt, Int)]()
	    
	
	    while (p <= X) {
	        if (k % (p-1) != 0) {
	            var bk = computeBkModP(p, k) 
	            rp = rp :+ (bk, p)
	            M = M * p
	        }
	        p = primeTable.nextPrime(p)
	    }
	   
	    val R = computeCRT(rp)
	    
	    val N_ = norm((dk * R) % M, M)
	    val nk = if (k % 4 == 2) N_ else N_ - M
	    return (nk, dk)
	    
	}
	
	def main(args : Array[String]): Unit = {
		println(B(1000))
	}
}

