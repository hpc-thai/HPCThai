#include <zmqpp/zmqpp.hpp>
#include <iostream>

auto main() -> int
{
    zmqpp::context context;

    zmqpp::socket sender(context, zmqpp::socket_type::push);
    sender.bind("tcp://*:6666");

    zmqpp::socket receiver(context, zmqpp::socket_type::pull);
    receiver.bind("tcp://*:7777");

    while (1) {
        int sum = 0;
        int max;
        std::cout << ">>>";
        std::cin >> max;
        for (int i=1; i<=max; i++) {
            zmqpp::message message;
            message << std::to_string(i);
            sender.send(message);
            std::cout << "Sent: " << i << std::endl;
        }
        std::cout << "Data sent." << std::endl;
        for (int i=1; i<=max; i++) {
            zmqpp::message message;
            std::string str;
            receiver.receive(message);
            message >> str;
            int num = std::stoi(str);
            sum += num;
            std::cout << "Received: " << num << std::endl;
        }
        std::cout << std::endl << "Answer is " << sum << std::endl;

    }
    return 0;
}
