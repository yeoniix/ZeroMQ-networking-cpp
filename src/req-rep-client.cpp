#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    zmq::context_t context{1};
    zmq::socket_t socket{context, zmq::socket_type::req};

    socket.connect("tcp://localhost:5555");
    std::cout<<"Connecting to hello world server..."<<std::endl;

    std::string message_text = "Hello";
    zmq::message_t message(message_text.begin(), message_text.end());
    std::cout<<"Client: sending '"<< message_text<<"'"<<std::endl;
    socket.send(message, zmq::send_flags::none);

    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    std::string reply_text(static_cast<char*>(reply.data()), reply.size());
    std::cout<<"Received reply '"<< reply_text<<"'"<<std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;

}