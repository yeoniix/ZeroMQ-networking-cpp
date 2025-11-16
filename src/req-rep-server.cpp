#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    //1. Create a ZeroMQ context with a single IO thread(ZMQ가 돌아가는 환경)
    zmq::context_t context{1};

    //2. Create a REP (reply) socket
    zmq::socket_t socket{context, zmq::socket_type::rep};

    //3. Bind the socket to a TCP endpoint(소켓을 특정 주소에 바인딩)
    socket.bind("tcp://*:5555");
    
    while(true){
        zmq::message_t request;

        //4. Wait for the next request from client
        socket.recv(request, zmq::recv_flags::none);
        std::string text
    }

}