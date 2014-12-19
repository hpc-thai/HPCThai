#include <zmqpp/zmqpp.hpp>
#include <iostream>

auto main() -> int
{
    zmqpp::context context;
    zmqpp::socket receiver(context, zmqpp::socket_type::pull);
    receiver.connect("tcp://localhost:6666");

    zmqpp::socket sender(context, zmqpp::socket_type::push);
    sender.connect("tcp://localhost:7777");


    while (1) {
        zmqpp::message messageIn, messageOut;
        std::string str;
        receiver.receive(messageIn);
        messageIn >> str;
        int num = stoi(str);
        std::cout << "Received: " << num << std::endl;
        num *= 10;
        messageOut << std::to_string(num);
        sender.send(messageOut);
        std::cout << "Sent: " << num << std::endl;
    }
    return 0;
}
