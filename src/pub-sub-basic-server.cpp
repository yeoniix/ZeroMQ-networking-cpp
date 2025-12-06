#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

int main() {

    zmq::context_t context{1};
    zmq::socket_t publisher{context, zmq::socket_type::pub};
    publisher.bind("tcp://*:5556");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_zip(1, 100000);
    std::uniform_int_distribution<> dist_temperature(-80,135);
    std::uniform_int_distribution<> dist_relhumidity(10, 60);

    std::cout << "Publishing updates at weather server..." << std::endl;

    while(true){
        int zipcode= dist_zip(gen);
        int temperature= dist_temperature(gen);
        int relhumidity= dist_relhumidity(gen);

        publisher.send(
            zmq::buffer(
                std::to_string(zipcode) + " " +
                std::to_string(temperature) + " " +
                std::to_string(relhumidity)
            ),
        zmq::send_flags::none
        );
    }

    return 0;
}