#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <array>
#include "../communication.h"

int main(int argc, char **argv){
    try{
        boost::asio::io_context io_ctx;
        boost::asio::ip::tcp::socket connection(io_ctx);

        connection.connect(
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address(
                    boost::asio::ip::make_address_v4("127.0.0.1")
                ),
                2011
            )
        );

        query q;
        if (argc == 1){
            q = {query::action_type::GET, 123456789};
        } else if (argc == 2){
            q = {query::action_type::GET, atoi(argv[1])};
        } else if (argc == 3){
            nlohmann::json this_action_type = argv[1];
            q = {this_action_type.get<query::action_type>(), atoi(argv[2])};
        }else {
            throw std::exception("skladnia: Lab1_client.exe [[get | set] key]\n"
                 "np.: Lab1_client.exe 123456789\n"
                 "Lab1_client.exe set 20\n"
                "klucz 0 jest zarezerwowany\n");
        }

        nlohmann::json data = q;
        std::array<char, 128> buf{};

        boost::system::error_code error{};
        
        boost::asio::write(connection, boost::asio::buffer(data.dump()));
        size_t len = connection.read_some(boost::asio::buffer(buf), error);
        nlohmann::json result = nlohmann::json::parse(std::string(buf.data(), len));
        auto res = result.get<answer>();

        if (error == boost::asio::error::eof){
            std::cout << "eof" << std::endl;
            // break;
        }
        else if (error){
            throw boost::system::system_error(error); // Some other error.
        }
        std::cout.write(buf.data(), len);
        std::cout << std::endl;

        std::cout << "closing\n" << std::endl;
        connection.close();
    } catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}