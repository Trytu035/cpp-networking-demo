#include <iostream>
#include <boost/asio.hpp>
#include "../communication.h"
#include <map>

#define STORAGE_PASSWORD "tajneHaslo123"

std::map<int, std::string> storage;

int main(int argc, char **argv){
    // std::socket s;
    storage.insert({123456789, "tajneHaslo123"});

    try{
        boost::asio::io_context io_ctx; //async execution context
        boost::asio::ip::tcp::acceptor server_socket(
            io_ctx,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(),
                2011    //port
            )
        );
        // boost::asio::ip::tcp::socket socket;
        std::string message = "";
        std::array<char, 128> buf;
        while (true){
            boost::asio::ip::tcp::socket connection(io_ctx);
            server_socket.accept(connection);
            boost::system::error_code ignored_error;    // temporarily
            boost::system::error_code timeout_error;

            size_t len = connection.read_some(boost::asio::buffer(buf), timeout_error);
            nlohmann::json result = nlohmann::json::parse(std::string(buf.data(), len));
            query q = result.get<query>();
            std::cout << q.action << " " << q.key << std::endl;
            std::cout.write(buf.data(), len) << std::endl;
            
            nlohmann::json send{};

            switch (q.action){
                case query::action_type::GET:
                    if (q.key != 0){
                        auto found = storage.find(q.key);
                        if (found != storage.end()){
                            answer packet(answer::status_type::OK, q.key, true);
                            send = packet;
                        }else{
                            answer packet(answer::status_type::OK, q.key, false);
                            send = packet;
                        }
                        boost::asio::write(connection, boost::asio::buffer(send.dump()), ignored_error);
                    } else{
                        //TODO: send error packet
                    }
                    break;
                case query::action_type::SET:
                    if (q.key != 0){
                        if (q.password == STORAGE_PASSWORD){
                            answer packet(answer::status_type::CREATED, q.key, false, "Key added successfully");
                            send = packet;
                        }else{
                            answer packet(answer::status_type::ERR, 0, false, "Authentication failed");
                            send = packet;
                        }
                        boost::asio::write(connection, boost::asio::buffer(send.dump()), ignored_error);
                    } else{
                        //TODO: send error packet
                    }
            }
            
            std::cout << "end connection\n";
        }
        // s.connect()
        // boost::asio::ip::tcp;  
        // boost::asio::basic_stream_socket socket;
        
    } catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}