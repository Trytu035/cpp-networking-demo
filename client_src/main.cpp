#include <iostream>
#include <boost/asio.hpp>
#include <array>

int main(int argc, char **argv){
    try{
        boost::asio::io_context io_ctx;
        // boost::asio::io::address server_address = boost::asio::ip::make_address("localhost")
        boost::asio::ip::tcp::socket connection(io_ctx);

        connection.connect(
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address(
                    boost::asio::ip::make_address_v4("127.0.0.1")
                ),
                2011
            )
        );

        std::array<char, 128> buf;

        boost::system::error_code error;
        size_t len = connection.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof){
            std::cout << "eof" << std::endl;
        }
            // break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf.data(), len);
        connection.close();
    } catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}