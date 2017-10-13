#include "TCPClient.h"

using namespace Network;

Network::TCPClient::TCPClient(PacketObserver &o)
	: Client(o), _io_service(), _signalRegister(_io_service)
{
 //   handleAsyncWait();
}

Network::TCPClient::~TCPClient()
{}

bool Network::TCPClient::connect(std::string const &ip, std::string const &port)
{
	boost::asio::ip::tcp::resolver	resolver(_io_service);
    boost::asio::ip::tcp::socket	socket(_io_service);

	boost::asio::connect(socket, resolver.resolve({ ip, port }));
    _tcpConnection = std::make_shared<TCPConnection>(
            std::move(socket),
            _packetObserver
    );
	return (true);
}

void Network::TCPClient::sendPacket(IPacket const &p)
{
    _tcpConnection->sendPacket(p);
}

void Network::TCPClient::disconnect()
{
    _tcpConnection->stop();
}

void Network::TCPClient::run()
{
    _tcpConnection->start();
	_io_service.run();
}

void Network::TCPClient::handleAsyncWait()
{
    dout << "Configuring signalHandler" << std::endl;

    //Register signals to be handle in async_wait
    //Allow us to exit the server correctly
    _signalRegister.add(SIGINT);
    _signalRegister.add(SIGTERM);

    //Process the lambda if one of the signals is received
    _signalRegister.async_wait(
            [this](boost::system::error_code ec, int sigNbr)
            {
                dout << "Received signal " << sigNbr << std::endl;
                _tcpConnection->stop();
            });
}