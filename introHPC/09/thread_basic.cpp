#include <iostream>
#include <thread>

using namespace std;

auto f(int id) -> void
{
    for (int i=0; i<10; i++) {
        cout << "Hello #" << id << " : " << id << endl;
        this_thread::yield();
    }
}

auto main() -> int
{
    thread f1 {f, 1};
    thread f2 (f, 2);

    cin.get();
    f1.join();
    f2.join();

}
