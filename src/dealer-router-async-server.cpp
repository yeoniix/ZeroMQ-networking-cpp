#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    // 1) ZMQ 컨텍스트
    zmq::context_t ctx{1};

    // 2) ROUTER 소켓
    zmq::socket_t router{ctx, zmq::socket_type::router};
    router.bind("tcp://*:5560");
    std::cout << "ROUTER listening on tcp://*:5560\n";

    while (true) {
        // ROUTER는 항상 "보낸쪽 식별자(routing id) → payload" 순서로 프레임들을 받는다.
        // (DEALER는 기본적으로 빈 프레임을 안 보낸다)
        zmq::message_t from;     // routing id (클라이언트 식별자)
        zmq::message_t payload;  // 실제 메시지

        // 1프레임: routing id
        router.recv(from, zmq::recv_flags::none);

        // 다음 프레임이 곧 payload라고 가정 (단순 예제)
        router.recv(payload, zmq::recv_flags::none);

        std::string from_id(static_cast<char*>(from.data()), from.size());
        std::string data(static_cast<char*>(payload.data()), payload.size());

        std::cout << "[ROUTER] from=" << from_id << " msg=\"" << data << "\"\n";

        // 응답 만들기
        std::string reply_text = "OK:" + data;
        zmq::message_t reply(reply_text.size());
        memcpy(reply.data(), reply_text.data(), reply_text.size());

        // ROUTER가 특정 클라이언트에게 답하려면 "routing id 프레임 → 데이터 프레임"으로 보내야 함
        router.send(from, zmq::send_flags::sndmore);   // 그대로 재사용 가능 (move 아님)
        router.send(reply, zmq::send_flags::none);
    }
    return 0;
}