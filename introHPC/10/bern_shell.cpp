#include <iostream>
#include <vector>
#include <cmath>
#include <gmpxx.h>
#include <tuple>
#include <stack>
#include <utility>
#include <deque>
#include <chrono>
#include <sstream>
#include <zmqpp/zmqpp.hpp>

using namespace std;

zmqpp::context context;
zmqpp::socket sender(context, zmqpp::socket_type::push);
zmqpp::socket receiver(context, zmqpp::socket_type::pull);

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

auto distribute(long k, primeList_t primeList) -> vector<crt_t>
{
    for (long p : primeList) {
        stringstream ssOut;
        zmqpp::message messageOut;
        ssOut.str(string());
        ssOut << k << "/" << p;
        messageOut << ssOut.str();
        sender.send(messageOut);
    }
    vector<crt_t> rp;
    for (int i=0; i< primeList.size(); i++) {
        stringstream ssIn;
        zmqpp::message messageIn;
        string str;
        long p, t;
        char slash;
        receiver.receive(messageIn);
        messageIn >> str;
        ssIn.str(str);
        ssIn >> t >> slash >> p;
        crt_t tt = make_tuple(t, p);
        rp.push_back(tt);
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
    sender.bind("tcp://*:6666");
    receiver.bind("tcp://*:7777");

    while (1) {
        long k;
        cout << ">>> ";
        cin >> k;
        auto t_stime = chrono::high_resolution_clock::now();
        cout << "B(" << k << ") = " << B(k) << endl;
        auto t_etime = chrono::high_resolution_clock::now();
        cout << "elapsed time: " << chrono::duration_cast<chrono::milliseconds>(t_etime - t_stime).count() << " msecs" << endl;
    }
    return 0;
}
