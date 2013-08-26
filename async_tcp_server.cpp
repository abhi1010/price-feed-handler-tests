//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session
: public std::enable_shared_from_this<Session>
{
    public:
        Session(tcp::socket socket)
            : mSocket(std::move(socket))
        {
        }

        void start()
        {
            do_read();
        }

    private:
        void do_read()
        {
            auto self(shared_from_this());
            mSocket.async_read_some(boost::asio::buffer(mData, max_length),
                    [this, self](boost::system::error_code ec, std::size_t length)
                    {
                        if (!ec)
                        {
                            std::cout << "Received data:" << mData << std::endl;
                            do_write(length);
                        }
                    });
        }

        void do_write(std::size_t length)
        {
            auto self(shared_from_this());
            boost::asio::async_write(mSocket, boost::asio::buffer(mData, length),
                    [this, self](boost::system::error_code ec, std::size_t /*length*/)
                    {
                    if (!ec)
                        {
                            do_read();
                        }
                    });
        }

        tcp::socket mSocket;
        enum { max_length = 1024 };
        char mData[max_length];
};

class Server
{
    public:
        Server(boost::asio::io_service& io_service, short port)
            : mAcceptor(io_service, tcp::endpoint(tcp::v4(), port)),
            mSocket(io_service)
    {
        do_accept();
    }

    private:
        void do_accept()
        {
            mAcceptor.async_accept(mSocket,
                    [this](boost::system::error_code ec)
                    {
                        if (!ec)
                        {
                            std::make_shared<Session>(std::move(mSocket))->start();
                        }

                        do_accept();
                        std::cout << "Accepting a Socket Connection" << std::endl;
                    });
        }

        tcp::acceptor mAcceptor;
        tcp::socket mSocket;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_Server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        Server s(io_service, std::atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
