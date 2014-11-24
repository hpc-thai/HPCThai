#include <iostream>
#include <chrono>
#include <new>
#include <cstdint>

const int N = 15000;
auto main() -> int
{
    uint64_t **a = new uint64_t*[N];

    uint64_t num = 1;
    for (uint32_t i=0; i<N; i++) {
        a[i] = new uint64_t[N];
        for (uint32_t j=0; j<N; j++) {
            a[i][j] = num++;
        }
    }

    double sum = 0.0;

    auto stime = std::chrono::high_resolution_clock::now();
    for (uint32_t i=0; i<N; i++) {
        for (uint32_t j=0; j<N; j++) {
            sum += a[j][i];
        }
    }
    auto etime = std::chrono::high_resolution_clock::now();
    std::cout << "Answer is : " << sum << std::endl;
    std::cout << "Time Used: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(etime - stime).count() << std::endl;
    return 0;
}
