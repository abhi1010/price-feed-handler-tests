#pragma once


#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

const short multicast_port = 30001;
const int max_message_count = 10;


class mc_sender
{
public:
    mc_sender(boost::asio::io_service& io_service, const boost::asio::ip::address& multicast_address);
    
    void handle_send_to(const boost::system::error_code& error);
    void handle_timeout(const boost::system::error_code& error);
    void sendData();

private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::deadline_timer timer_;
    int message_count_;
    std::string message_;
};


class McPub
{
public:
    McPub(ulong rate);
    ~McPub(void);

    void test();
    void initTimer();
    
    void handle_timeout(const boost::system::error_code&);
    void handle_deadline_timeout(const boost::system::error_code&);
private:
    mc_sender*                  mMulticastSender;
    boost::asio::io_service     mIoService;
    const std::string           mIpAddress;
    boost::asio::deadline_timer t;
    boost::asio::deadline_timer::duration_type dt;
    ulong                       mRate;
};



