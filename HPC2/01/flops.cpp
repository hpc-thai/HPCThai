#include "python.h"
#include <cstdint>
#include <chrono>
#include <iostream>
#include <new>
#include <sstream>
#include <string>

auto benchmark(uint32_t N, uint32_t R) -> uint64_t
{
    float *w = new float[N];
    float *x = new float[N];
    float *y = new float[N];
    float *z = new float[N];

    for (uint64_t n=0; n<N; n++) {
        x[n] = 2.0;
        y[n] = 3.0;
        z[n] = 4.0;
    }

    auto stime = std::chrono::high_resolution_clock::now();
    for (uint32_t r=0; r < R; r++) {
        for (uint32_t n=0; n<N; n++) {
            w[n] = x[n] * y[n] + z[n];
        }
    }
    auto etime = std::chrono::high_resolution_clock::now();
    delete[] w;
    delete[] x;
    delete[] y;
    delete[] z;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(etime - stime).count();
}

auto genGraph(std::string strN, std::string strGFlops) -> void
{
    Py_Initialize();
    PyRun_SimpleString("from pylab import *");
    PyRun_SimpleString(strN.c_str());
    PyRun_SimpleString(strGFlops.c_str());
    PyRun_SimpleString("plot(n, gflops, 'g*-')");
    PyRun_SimpleString("title('GFlops Benchmark')");
    PyRun_SimpleString("xlabel('N')");
    PyRun_SimpleString("ylabel('GFlops')");
    PyRun_SimpleString("grid()");
    PyRun_SimpleString("savefig('flops.png')");
    Py_Finalize();
    std:: cout << "Generated flops.png" << std::endl;
}

const uint32_t R = 100;
auto main() -> int
{
   /* Py_Initialize();
    PyRun_SimpleString("print('Hello, World')");
    Py_Exit(0);*/
    std::stringstream ssN, ssGFlops;
    ssN << "n = [";
    ssGFlops << "gflops = [";

    for (uint32_t N=200; N<=20000000; N*=2) {
        uint64_t s = benchmark(N, R);
        double gflops = (N * 3.0 * R) / s;
        ssN  << N  << ", ";
        ssGFlops  <<gflops << ", ";
        std::cout << "N = " <<  N << " : " << gflops << " GFlops" << std::endl;
    }
    ssN << "]";
    ssGFlops << "]";
    genGraph(ssN.str(), ssGFlops.str());
    return 0;
}
