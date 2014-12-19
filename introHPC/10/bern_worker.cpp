#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>
#include <tuple>
#include <stack>
#include <utility>
#include <deque>
#include <chrono>
#include <thread>
#include <zmqpp/zmqpp.hpp>
#include <sstream>

using namespace std;

auto norm(mpz_class b, mpz_class p) -> mpz_class
{
    while (true) {
        if (b < 0) b += p;
        else if (b > p) b -= p;
        else break;
    }
    return b;
}

auto computePowerMod(const mpz_class &a, const long &k, const mpz_class &m) -> mpz_class
{
    int index = log2(k);
    mpz_class powmod = 1, powa = a, pow2 = 1;

    for (int i=0; i<=index; i++) {
        if ( (k & pow2) != 0) powmod *= powa;
        powa = (powa * powa) % m;
        pow2 <<= 1;
    }
    return norm(powmod % m, m);
}

auto computeInvertMod(const mpz_class &n, mpz_class p) -> mpz_class
{
    stack<tuple<mpz_class, mpz_class, mpz_class>> v;
    mpz_class a = p;
    mpz_class b = n;
    if (n > p) {
        a = n;
        b = p;
    }
    mpz_class d = a / b;
    mpz_class r = a % b;

    while (r > 1) {
        v.push(make_tuple(a, b, -d));
        a = b;
        b = r;
        d = a / b;
        r = a % b;
    }
    mpz_class m1 = 1, m2 = -d;
    mpz_class aa = a, bb = b;

    while (!v.empty()) {
        auto t = v.top();
        v.pop();
        a = get<0>(t);
        b = get<1>(t);
        d = get<2>(t);
        m1 += (m2 * d);
        bb = a;
        swap(m1, m2);
        swap(aa, bb);
    }
    mpz_class rr = (n == aa)? m1 : m2;
    return norm(rr, p);
}

auto computeOrderEp(const long &a, const long &p) -> long
{
    for (long k=1; k<p; k++) {
        if (computePowerMod(a, k, p) == 1) return k;
    }
    throw "computeOrderEp error";
}

auto computePrimitiveRoot(const long &p) -> long
{
    if (p == 2) return 1;
    for (int g=2; g<p; g++) {
        if (computeOrderEp(g, p) == (p - 1)) return g;
    }
    throw "computePrimitiveRoot error";
}

auto computeBkModP(const long &p, const long &k) -> mpz_class
{
    if (k == 0) return 1;
    if (k == 1) return (p == 2)? -1 : (p-1)/2;
    if (k &  1) return 0;
    if (p <= 3) return -1;

    long m = k % (p-1);
    if (m == 0) return -1;

    mpz_class g = computePrimitiveRoot(p);
    mpz_class r = computePowerMod(g, k-1, p);
    mpz_class u = (g + ((g % 2 == 1)? 0 : p) - 1) / 2;
    mpz_class S = 0, X = 1, Y = r;


    mpz_class hp = (p-1)/2, q, h, t;
    for (long i=1; i<=hp; i++) {
        q = norm( (g*X)/p, p);
        h = norm( (q-u)%p, p);
        t = norm( (h*Y)%p, p);
        S = norm( (S-t)%p, p);
        X = norm( (g*X)%p, p);
        Y = norm( (r*Y)%p, p);
    }
    mpz_class ans = computePowerMod(g, k, p);

    ans = computeInvertMod(p+1-ans, p);

    ans = norm( (S * ans) % p, p);
    ans = norm( (2 * ans) % p, p);
    ans = norm( (k * ans) % p, p);

    return ans;
}

using crt_t = tuple<mpz_class, long>;
using primeList_t = deque<long>;

auto worker(int threadNo) -> void
{
    zmqpp::context context;
    zmqpp::socket receiver(context, zmqpp::socket_type::pull);
    receiver.connect("tcp://localhost:6666");

    zmqpp::socket sender(context, zmqpp::socket_type::push);
    sender.connect("tcp://localhost:7777");

    cout << "worker " << threadNo << " ready." << endl;

    while (1) {
        string str;
        receiver.receive(str);
        stringstream ssIn;
        ssIn.str(str);
        long k, p;
        char slash;
        ssIn >> k >> slash >> p;
        mpz_class bkModP = computeBkModP(p, k);
        stringstream ssOut;
        ssOut << bkModP << "/" << p;
        sender.send(ssOut.str());
    }
}

auto main(int argc, char *argv[]) -> int
{
    auto nthreads = thread::hardware_concurrency();
    thread threads[nthreads];

    for (int i=0; i<nthreads; i++) {
        threads[i] = thread(worker, i);
    }
    for (int i=0; i<nthreads; i++) {
        threads[i].join();
    }
    return 0;
}
