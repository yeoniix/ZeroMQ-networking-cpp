#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>



int main() {
    //1. Create a ZeroMQ context with a single I/O thread(ZMQ가 돌아가는 환경)
    zmq::context_t context{1}; //1은 I/O 스레드의 개수를 나타냄 

    //2. Create a REP (reply) socket
    zmq::socket_t socket{context, zmq::socket_type::rep};

    //3. Bind the socket to a TCP endpoint(소켓을 특정 주소에 바인딩)
    socket.bind("tcp://*:5555");//5555 포트에서 모든 네트워크 인터페이스 대기 시작 


    
    while(true){
        zmq::message_t request;

        //4. Wait for the next request from client
        socket.recv(request, zmq::recv_flags::none);
        std::string text(static_cast<char*>(request.data()), request.size());
        std::cout<<"Received request: "<<text<<std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        //5. reply message
        std::string reply_text = "World";
        zmq::message_t reply(reply_text.begin(), reply_text.end());

        socket.send(reply, zmq::send_flags::none);
    }

    return 0; 

}