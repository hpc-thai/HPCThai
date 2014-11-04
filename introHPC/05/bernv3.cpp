#include <iostream>
#include <sstream>
#include <gmpxx.h>
#include <unordered_map>
#include <utility>
#include <chrono>

using namespace std;
using binoMap_t = unordered_map< string, mpq_class>;
using bernMap_t = unordered_map< int, mpq_class>;

auto factorial(int n) -> mpq_class
{
    mpq_class ans(1, 1);
    if (n < 2) return ans;
    for (int i=2; i<=n; i++) ans *= i;
    return ans;
}

auto binomial(int n, int r) -> mpq_class
{
    static binoMap_t binoMap;
    ostringstream ss;
    ss << n << "," << r;
    string nr = ss.str();

    if (binoMap.find(nr) != binoMap.end()) {
        return binoMap[nr];
    }
    mpq_class ans = factorial(n) / (factorial(r) * factorial(n-r));
    binoMap[nr] = ans;
    return ans;
}

auto B(int m) -> mpq_class
{
    static bernMap_t bernMap;

    if (m == 0) return mpq_class(1, 1);
    if (m == 1) return mpq_class(1, 2);
    if (m %  2 == 1) return mpq_class(0, 1);

    if (bernMap.find(m) != bernMap.end()) {
        return bernMap[m];
    }
    mpq_class sigma = mpq_class(0, 1);
    for (int k=0; k<m; k++) {
        sigma += (binomial(m, k) * B(k) / (m - k + 1));
    }
    mpq_class ans = mpq_class(1, 1) - sigma;
    bernMap[m] = ans;
    return ans;
}

auto main() -> int
{
    int m;

    for (int i=0; i<=20; i+=2) {
        mpq_class b = B(i);
        b.canonicalize();
        cout << i << "   " << b << endl;
    }
    cout << "calculating B(1000)..." << endl;
    auto start = chrono::system_clock::now();
    mpq_class b1000 = B(1000);
    b1000.canonicalize();
    auto stop = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "B(1000) = " << b1000 << endl;
    cout << "Time used B(1000) =  " << duration.count() << " secs."<< endl;
    return 0;
}
