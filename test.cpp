#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    boost::asio::io_service io_service;

    io_service.run();

    std::cout << "boost asio test" << std::endl;

    return 0;
}
