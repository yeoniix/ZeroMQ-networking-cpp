#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    // 실행 시 ID를 넘길 수 있게 (기본값 "client-1")
    std::string my_id = (argc > 1) ? argv[1] : "client-1";

    zmq::context_t ctx{1};

    // DEALER 소켓
    zmq::socket_t dealer{ctx, zmq::socket_type::dealer};

    // 내 routing id(식별자) 설정 — ROUTER가 첫 프레임으로 이 값을 받게 된다
    dealer.set(zmq::sockopt::routing_id, my_id);

    dealer.connect("tcp://127.0.0.1:5560");
    std::cout << "[DEALER " << my_id << "] connected to tcp://127.0.0.1:5560\n";

    // 간단히 5번 요청/응답
    for (int i = 0; i < 5; ++i) {
        std::string text = "hello-" + std::to_string(i);

        zmq::message_t msg(text.size());
        memcpy(msg.data(), text.data(), text.size());

        // DEALER는 payload만 보냄 (routing id는 소켓 옵션으로 붙는다)
        dealer.send(msg, zmq::send_flags::none);
        std::cout << "[DEALER " << my_id << "] sent \"" << text << "\"\n";

        // 응답 수신
        zmq::message_t reply;
        dealer.recv(reply, zmq::recv_flags::none);
        std::string r(static_cast<char*>(reply.data()), reply.size());
        std::cout << "[DEALER " << my_id << "] got reply \"" << r << "\"\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    std::cout << "[DEALER " << my_id << "] done.\n";
    return 0;
}