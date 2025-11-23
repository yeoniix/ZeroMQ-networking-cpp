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
    std::cout << "Publishing updates at weather server..." << std::endl;


    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> zip_dist(1, 100000);
    std::uniform_int_distribution<> temp_dist(-80, 135);
    std::uniform_int_distribution<> humid_dist(10, 60);

    while (true) {
      
        int zipcode = zip_dist(gen);
        int temperature = temp_dist(gen);
        int relhumidity = humid_dist(gen);

        std::string update_msg =
            std::to_string(zipcode) + " " +
            std::to_string(temperature) + " " +
            std::to_string(relhumidity);

    
        zmq::message_t message(update_msg.size());
        memcpy(message.data(), update_msg.data(), update_msg.size());

   
        publisher.send(message, zmq::send_flags::none);

        std::cout << "PUB: " << update_msg << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}