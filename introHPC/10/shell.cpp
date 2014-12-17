#include <zmq.hpp>
#include <iotream>

auto main() -> int
{
    zmq::context_t context(1);

    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.bind("tcp://*:6666");

    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.bind("tcp://*:7777");

    while (1) {
        int sum = 0;
        int max;
        cout << ">>>";
        std::cin >> max;
        for (int i=1; i<=max; i++) {
            sender.send_string("%s")
        }

    }
    return 0;
}
