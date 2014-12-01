#include <iostream>
#include <iomanip>
#include <thread>
#include <deque>
#include <vector>
#include <mutex>

using namespace std;

const long MAX = 10000;
const int nthreads = 2;

void worker(mutex &m, deque<long> &d, int threadNo, vector<long> &sumWorkers)
{
    while(1) {
	unique_lock<mutex> lck {m};
	if (d.empty()) break;
        long num = d.front();
        d.pop_front();
	lck.unlock();
        sumWorkers[threadNo] += num;
    }
}

auto main() -> int
{
    mutex m;
    deque<long> d(MAX);
    vector<long> sumWorkers(nthreads);
    thread threads[nthreads];
    for (long i=1; i<=MAX; i++) {
        d.push_back(i);
    }

    for (int i=0; i<nthreads; i++) {
	sumWorkers[i] = 0;
        threads[i] = thread(worker, ref(m), ref(d), i, ref(sumWorkers));
    }

    long sum = 0;
    for (int i=0; i<nthreads; i++) {
        threads[i].join();
        sum += sumWorkers[i];
    }
    cout << "Sum should be: " << setiosflags(ios::fixed) 
		<< setprecision(0) << MAX / 2.0 * (MAX +1) << endl;
    cout << "Sum calculate: " << sum << endl;

    return 0;
}
