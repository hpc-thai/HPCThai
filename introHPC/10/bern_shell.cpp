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
#include <mutex>

using namespace std;

class PrimeTable
{
public:
    PrimeTable(const long &size) : _size {size}, _table (size, true) {
        _table[0] = _table[1] = false;
        const long limit = (long)sqrt(size);
        for (long i=2; i<=limit; i++) {
            if (_table[i] == false) continue;
            for (long r=i+i; r<=size; r+=i) _table[r] = false;
        }
    }

    long nextPrime(long pos) {
        for (long i=pos+1; i<=_size; i++) {
            if (_table[i] == true) return i;
        }
        return -1;
    }
private:
    long _size;
    std::vector<bool> _table;
};

auto computeDk(const int &k, PrimeTable& primeTable) -> mpz_class
{
    long p {2};
    mpz_class dk {1};
    while (p != -1) {
        if (k % (p-1) == 0) dk *= p;
        p = primeTable.nextPrime(p);
    }
    return std::move(dk);
}

auto norm(mpz_class b, mpz_class p) -> mpz_class
{
    while (true) {
        if (b < 0) b += p;
        else if (b > p) b -= p;
        else break;
    }
    return b;
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

auto congruent(const mpz_class &a, const mpz_class &b, const mpz_class &m) -> mpz_class
{
    if (a == 1 && b == 1) return mpz_class {1};
    mpz_class inv = computeInvertMod(a, m);
    return (b * inv) % m;
}

auto computeCRT(vector<tuple<mpz_class, long>> &residue) -> mpz_class
{
    mpz_class mul_m = 1;
    for (auto top : residue) {
        mul_m *= get<1>(top);
    }

    mpz_class z = 0, M, X, b, m;
    for (auto top : residue) {
        b = get<0>(top);
        m = get<1>(top);
        M = mul_m / m;
        X = congruent(M, 1, m);
        z += M * X * b;
    }
    while (z > 0) {
        z -= mul_m;
    }
    return z + mul_m;
}

using crt_t = tuple<mpz_class, long>;
using primeList_t = deque<long>;

auto worker(mutex &m1, mutex &m2, vector<crt_t> &rp, long k, primeList_t &primeList, int threadNo) -> void
{
    auto p_stime = chrono::high_resolution_clock::now();
    while (true) {
        unique_lock<mutex>lck1 {m1};
        if (primeList.empty()) break;
        long pp = primeList.front();
        primeList.pop_front();
        lck1.unlock();

        crt_t tt = make_tuple(computeBkModP(pp, k), pp);
        unique_lock<mutex>lck2 {m2};
        rp.push_back(tt);
        lck2.unlock();
    }
    auto p_etime = chrono::high_resolution_clock::now();
    cout << "\nThread[" <<  threadNo << "] = " <<
            chrono::duration_cast<chrono::milliseconds>(p_etime - p_stime).count() << " msecs" << endl;
}
auto distribute(long k, primeList_t primeList) -> vector<crt_t>
{
    vector<crt_t> rp;
    mutex m1, m2;
    auto nthreads = thread::hardware_concurrency();
    thread threads[nthreads];

    for (int i=0; i<nthreads; i++) {
        threads[i] = thread(worker, ref(m1), ref(m2), ref(rp), k, ref(primeList), i);
    }
    for (int i=0; i<nthreads; i++) {
        threads[i].join();
    }
    return rp;
}

auto B(long k) -> mpq_class
{
    mpq_class bern_base[] {mpq_class {1, 1}, mpq_class {1, 2}, mpq_class {1, 6} };
    if (k < 3) return bern_base[k];

    if (k & 1) return mpq_class {0, 1};
    long Y = long(ceil((k+0.5) * log2(k)));
    if (Y < 37) Y = 37;
    PrimeTable primeTable(Y);
    mpz_class dk = computeDk(k, primeTable);
    long beta = long(ceil( (k+0.5) * log2(k) - 4.094 * k + 2.470 + log2(dk.get_d())));
    long p = 3;
    mpz_class M_ {1};
    mpz_class twoPowerBetaPlusOne;
    mpz_ui_pow_ui(twoPowerBetaPlusOne.get_mpz_t(), 2, beta+1);

    while (M_ < twoPowerBetaPlusOne) {
        p = primeTable.nextPrime(p);
        if (k % (p-1) != 0) {
            M_ *= p;
        }
    }

    long X = p;
    mpz_class M {1};
    p = 2;

    primeList_t primeList;

    while (p <= X) {
        if (k % (p-1) != 0) {
            primeList.push_back(p);
            M *= p;
        }
        p = primeTable.nextPrime(p);
    }
    auto rp = distribute(k, primeList);

    mpz_class R = computeCRT(rp);
    mpz_class N_ = norm((dk * R) % M, M);
    mpz_class nk = (k % 4 == 2)? N_ : N_ - M;
    return mpq_class {nk, dk};
}

auto main(int argc, char *argv[]) -> int
{
    if (argc != 2) {
        cout << "usage: " << argv[0] << " m" << endl;
        return 0;
    }
    auto t_stime = chrono::high_resolution_clock::now();
    cout << "B(" << argv[1] << ") = " << B(atoi(argv[1])) << endl;
    auto t_etime = chrono::high_resolution_clock::now();
    cout << "Total Time used: " << chrono::duration_cast<chrono::milliseconds>(t_etime - t_stime).count() << " msecs" << endl;
    return 0;
}
