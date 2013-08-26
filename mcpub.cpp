#include "mcpub.h"


mc_sender::mc_sender(boost::asio::io_service& io_service, const boost::asio::ip::address& multicast_address)
    : endpoint_(multicast_address, multicast_port)
    ,  socket_(io_service, endpoint_.protocol())
    ,  timer_(io_service)
      ,  message_count_(0)
{
    sendData();
}


void mc_sender::sendData()
{
    std::ostringstream os;
    os << "Message " << message_count_++;
    message_ = os.str();
    socket_.async_send_to (boost::asio::buffer(message_), endpoint_,
            boost::bind(&mc_sender::handle_send_to, this, boost::asio::placeholders::error));
}

void mc_sender::handle_send_to(const boost::system::error_code& error)
{
    if (!error && message_count_ < max_message_count)
    {
        timer_.expires_from_now(boost::posix_time::seconds(1));
        timer_.async_wait( boost::bind(&mc_sender::handle_timeout, this,
                    boost::asio::placeholders::error));
    }
}

void mc_sender::handle_timeout(const boost::system::error_code& error)
{
    if (!error)
    {
        std::ostringstream os;
        os << "Message " << message_count_++;
        message_ = os.str();

        socket_.async_send_to(boost::asio::buffer(message_),
                endpoint_, boost::bind(&mc_sender::handle_send_to,
                    this, boost::asio::placeholders::error));
    }
}


McPub::McPub(ulong rate)
    : mMulticastSender      (NULL)
      , mIoService            ()
    , mIpAddress            ("239.255.0.1")
    , t                     (mIoService)
    , dt                    (boost::posix_time::milliseconds(100))
      , mRate                 (rate)
{
    initTimer();

    mMulticastSender = new mc_sender(mIoService, boost::asio::ip::address::from_string(mIpAddress));
    mIoService.run();

}


void McPub::handle_timeout(const boost::system::error_code&)
{
    std::cout << "McPub::handle_timeout\n";
}

void McPub::handle_deadline_timeout(const boost::system::error_code&)
{
    static int counter = 0;
    ulong msgToSend = mRate / 100;
    std::cout << "McPub::handle deadline timeout:: " << ++counter << "; Msgs to send=" <<
        msgToSend << "\n";

    t.expires_from_now(dt);
    t.async_wait( boost::bind (&McPub::handle_deadline_timeout, this,
                boost::asio::placeholders::error));
    mMulticastSender->sendData();
}

void McPub::initTimer()
{
    t.expires_from_now(dt);
    std::cout << "Starting asynchronous wait ........ 2... \n";
    t.async_wait( boost::bind (&McPub::handle_deadline_timeout, this,
                boost::asio::placeholders::error));
    //io.run();
}


McPub::~McPub(void)
{
    delete mMulticastSender;
}

void McPub::test()
{
    try
    {
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}



int main()
{
    McPub mcPub(900);
    mcPub.test();
}
