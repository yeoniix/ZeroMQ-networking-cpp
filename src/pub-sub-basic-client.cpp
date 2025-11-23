#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
  
    zmq::context_t context{1};

 
    zmq::socket_t subscriber{context, zmq::socket_type::sub};

  
    subscriber.connect("tcp://127.0.0.1:5556");
    std::cout << "Subscriber connected to tcp://127.0.0.1:5556" << std::endl;

  
    subscriber.set(zmq::sockopt::subscribe, "");

    while (true) {

        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        
        std::string msg(static_cast<char*>(message.data()), message.size());

        std::cout << "Received: " << msg << std::endl;
    }

    return 0;
}