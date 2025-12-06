#include <zmq.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>

int main(int argc, char* argv[]){
    std::string clientID = (argc>1)? argv[1] : "0";
    zmq::context_t context{1};
    zmq::socket_t subscriber{context, zmq::socket_type::sub};
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "");
    subscriber.connect("tcp://localhost:5557");
    zmq::socket_t publisher{context, zmq::socket_type::push};
    publisher.connect("tcp://localhost:5558");

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1,100);

    while(true){
        zmq::pollitem_t items[] = {
            { static_cast<void*>(subscriber), 0, ZMQ_POLLIN, 0 }
        };
        zmq::poll(items,1,100);

        if(items[0].revents & ZMQ_POLLIN){
            zmq::message_t message;
            subscriber.recv(message);

            std::string s(static_cast<char*>(message.data()), message.size());
            std::cout<<"{"<<clientID<<"}: receive status => {"<<message<<"}"<<s<<std::endl;
        }
        else{
            int r=dist(gen);
            if(r<10){
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::string m="("+clientID+": ON)";
                publisher.send(zmq::buffer(m), zmq::send_flags::none);
                std::cout<<clientID<<": send status - activated"<<std::endl;
            }
            else if(r>90){
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::string m="("+clientID+": OFF)";
                publisher.send(zmq::buffer(m), zmq::send_flags::none);
                std::cout<<clientID<<": send status - deactivated"<<std::endl;
            }

            }
        }
    }