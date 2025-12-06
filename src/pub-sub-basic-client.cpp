#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
  
    zmq::context_t context{1};
    zmq::socket_t subscriber{context, zmq::socket_type::sub};

    std::string zip_filter=(argc>1)? argv[1] : "10001";

  
    subscriber.connect("tcp://localhost:5556");
    std::cout << "Collecting updates from weather server..." << std::endl;

    subscriber.set(zmq::sockopt::subscribe, zip_filter);

    int total_temp=0;


    for(int i=0; i<20; ++i){
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);
        std::string msg(static_cast<char*>(message.data()), message.size());

        std::istringstream iss(msg);
        std::string zipcode;
        int temperature;
        int relhumidity;
        iss >> zipcode >> temperature >> relhumidity;

        total_temp += temperature;

        std::cout << "Received temperature for zipcode '" << zip_filter << "' was " << temperature
                  << "F" << std::endl;
    }

    std::cout << "Average temperature for zipcode '" << zip_filter << "' was "
              << (total_temp / 20.0) << "°F" << std::endl;

    return 0;
}