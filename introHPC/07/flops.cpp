#include "python.h"
#include <stdint.h>
#include <chrono>
#include <iostream>
#include <new>

auto benchmark(uint32_t n) -> uint64_t
{
    float *w = new float[n];
    float *x = new float[n];
    float *y = new float[n];
    float *z = new float[n];

    for (uint64_t i=0; i<n; i++) {
        x[i] = 2.0;
        y[i] = 3.0;
        z[i] = 4.0;
    }

    auto stime = std::chrono::high_resolution_clock::now();
    for (uint64_t i=0; i<n; i++) {
        w[i] = x[i] * y[i] + z[i];
    }
    auto etime = std::chrono::high_resolution_clock::now();
    delete[] w;
    delete[] x;
    delete[] y;
    delete[] z;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(etime - stime).count();
}

const uint64_t r = 100000;
auto main() -> int
{
   /* Py_Initialize();
    PyRun_SimpleString("print('Hello, World')");
    Py_Exit(0);*/

    for (uint64_t i =1, n=10; i<=7; i++) {
        uint64_t ss = 0;
        for (int j=0; j<r; j++) {
            uint64_t s = benchmark(n);
            ss += s;
        }
        double gflops = (n * 3.0 * r) / ss;
        std::cout << "n = " <<  n << " : " << gflops << " GFlops" << std::endl;
        n *= 10;
    }
    return 0;
}
