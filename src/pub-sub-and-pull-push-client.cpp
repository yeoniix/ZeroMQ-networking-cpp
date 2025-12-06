#include <zmq.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <random>
#include <chrono>

int main() {
    zmq::context_t context{1};


    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.connect("tcp://localhost:5557");

 
    zmq::socket_t publisher(context, zmq::socket_type::push);
    publisher.connect("tcp://localhost:5558");


    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 100);

    while (true) {
        zmq::pollitem_t items[] = {
            { static_cast<void*>(subscriber), 0, ZMQ_POLLIN, 0 }
        };

    
        zmq::poll(items, 1, 100);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            subscriber.recv(message);

            std::string s(static_cast<char*>(message.data()), message.size());
            std::cout << "I: received message " << s << std::endl;
        } else {
            int r= dist(gen);
            if (r < 10) {
                std::string msg = std::to_string(r);
                zmq::message_t out(msg);
                publisher.send(out, zmq::send_flags::none);

                std::cout << "I: sending message " << r << std::endl;
            }
        }
    }

    return 0;
}