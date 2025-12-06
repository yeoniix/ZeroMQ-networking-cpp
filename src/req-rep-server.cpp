#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>



int main() {
    // ZeroMQ context with a single I/O thread(ZMQ가 돌아가는 환경)
    zmq::context_t context{1}; 
    //REP Socket
    zmq::socket_t socket{context, zmq::socket_type::rep};
    //소켓을 특정 주소에 바인딩
    socket.bind("tcp://*:5555");

    const std::string data{"World"};


    
    while(true){
        zmq::message_t request;

        //Wait for the next request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout<<"Received request: "<<request.to_string()<<std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        //5. reply message
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    return 0; 

}