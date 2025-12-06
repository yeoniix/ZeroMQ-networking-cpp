#include <zmq.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    std::string identity = argv[1];
    zmq::context_t ctx{1};
    zmq::socket_t socket(ctx, zmq::socket_type::dealer);


    socket.setsockopt(ZMQ_IDENTITY, identity.data(), static_cast<int>(identity.size()));
    socket.connect("tcp://localhost:5570");
    std::cout << "Client " << identity << " started" << std::endl;
    int reqs = 0;

    while (true) {
        reqs=reqs+1;
        std::cout << "Req #" << reqs << " sent.." << std::endl;
        std::string out = "request #" + std::to_string(reqs);
        socket.send(zmq::buffer(out), zmq::send_flags::none);


        std::this_thread::sleep_for(std::chrono::seconds(1));

        zmq::pollitem_t items[] = { { static_cast<void*>(socket), 0, ZMQ_POLLIN, 0 } };
        zmq::poll(items, 1, 1000);

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            socket.recv(msg, zmq::recv_flags::none);

            std::string msg_str(static_cast<char*>(msg.data()), msg.size());


            std::cout << identity << " received: " << msg_str << std::endl;
        }
    }

    return 0;
}