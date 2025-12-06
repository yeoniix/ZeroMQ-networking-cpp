#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    //initialize the zmq context with a single IO thread
    zmq::context_t context{1};
    //REQ socket
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");
    std::cout<<"Connecting to hello world server..."<<std::endl;

    std::string message_text = "Hello";

    for(int request_num=0; request_num<10; ++request_num) {
        zmq::message_t message(message_text);
        std::cout<<"Sending request "<< request_num <<"'..."<<std::endl;
        socket.send(message, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);

        std::cout<<"Received reply '"<< reply.to_string()<<std::endl;
        std::cout<<"("<<request_num<<")"<<std::endl;

    }

    return 0;

}