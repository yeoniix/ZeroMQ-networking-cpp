#include <zmq.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

class ClientTask {
public:
    explicit ClientTask(const std::string& id)
        : identity_(id), ctx_(1) {}


    void ioThread() {
        zmq::socket_t dealer(ctx_, zmq::socket_type::dealer);
        dealer.setsockopt(ZMQ_IDENTITY, identity_.data(), identity_.size());
        dealer.connect("tcp://localhost:5570");

        zmq::socket_t pull(ctx_, zmq::socket_type::pull);
        pull.bind("inproc://tx");             

        std::cout << "Client " << identity_ << " started" << std::endl;

        while (true) {
            zmq::pollitem_t items[] = {
                { dealer, 0, ZMQ_POLLIN, 0 }, 
                { pull,   0, ZMQ_POLLIN, 0 }  
            };
            zmq::poll(items, 2, 1000);


            if (items[0].revents & ZMQ_POLLIN) {
                zmq::message_t msg;
                dealer.recv(msg, zmq::recv_flags::none);
                std::string m(static_cast<char*>(msg.data()), msg.size());
                std::cout << identity_ << " received: " << m << std::endl;
            }

            if (items[1].revents & ZMQ_POLLIN) {
                zmq::message_t outbound;
                pull.recv(outbound, zmq::recv_flags::none);
                dealer.send(outbound, zmq::send_flags::none);
            }
        }
    }

    void run() {
        std::thread io(&ClientTask::ioThread, this);
        io.detach();  


        zmq::socket_t push(ctx_, zmq::socket_type::push);
        push.connect("inproc://tx");

        int reqs = 0;
        while (true) {
            ++reqs;
            std::cout << "Req #" << reqs << " sent.." << std::endl;

            std::string payload = "request #" + std::to_string(reqs);
            push.send(zmq::buffer(payload), zmq::send_flags::none);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

private:
    std::string identity_;
    zmq::context_t ctx_;
};

int main(int argc, char* argv[]) {
    ClientTask client(argv[1]);
    client.run();
    return 0;
}