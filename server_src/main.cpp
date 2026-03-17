#include <iostream>
// #include <net   ioapi.h>
// #include <sys/types.h>
// #include <winsock.h>
// #include <winsock2.h>
// #include <net
#include <boost/asio.hpp>
// #include <boost/beast.hpp>

int main(int argc, char **argv){
    // std::socket s;
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
        while (true){
            boost::asio::ip::tcp::socket connection(io_ctx);
            server_socket.accept(connection);

            std::string message = "ack";
            boost::system::error_code ignored_error;    // temporarily
            boost::asio::write(connection, boost::asio::buffer(message), ignored_error);
            // boost::asio::ip::tcp::socket client(boost::asio::execution::any_executor());
        }
        // s.connect()
        // boost::asio::ip::tcp;  
        // boost::asio::basic_stream_socket socket;
        
    } catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}