#include <zmq.hpp>
#include <thread>
#include <vector>
#include <string>
#include <iostream>


class ServerWorker {
public:
    ServerWorker(zmq::context_t& ctx, int id) : ctx_(ctx), id_(id) {}

    void operator()() {
        zmq::socket_t worker(ctx_, zmq::socket_type::dealer);
        worker.connect("inproc://backend");

        std::cout << "Worker#" << id_ << " started" << std::endl;

        while (true) {
            zmq::message_t ident, msg;

            if (!worker.recv(ident, zmq::recv_flags::none)) continue;
            worker.recv(msg, zmq::recv_flags::none);

            std::string ident_str(static_cast<char*>(ident.data()), ident.size());
            std::string msg_str(static_cast<char*>(msg.data()), msg.size());

            std::cout << "Worker#" << id_<< " received " << msg_str<< " from " << ident_str<< std::endl; 
            worker.send(ident, zmq::send_flags::sndmore);
            worker.send(msg, zmq::send_flags::none);
        }
    }

private:
    zmq::context_t& ctx_;
    int id_;
};


class ServerTask {
public:
    ServerTask(int num_workers)
        : ctx_(1),
          num_workers_(num_workers),
          frontend_(ctx_, zmq::socket_type::router),
          backend_(ctx_, zmq::socket_type::dealer)
    {}

    void run() {
        frontend_.bind("tcp://*:5570");
        backend_.bind("inproc://backend");

     
        workers_.reserve(num_workers_);
        for (int i = 0; i < num_workers_; ++i)
            workers_.emplace_back(ServerWorker(ctx_, i));

        
        zmq::proxy(frontend_, backend_);

        for (auto& th : workers_) th.join();
    }

private:
    zmq::context_t ctx_;
    int num_workers_;
    zmq::socket_t frontend_;
    zmq::socket_t backend_;
    std::vector<std::thread> workers_;
};


int main(int argc, char* argv[]) {
    int n = std::stoi(argv[1]);
    ServerTask server(n);
    server.run();
    return 0;
}