#include "TCPClient.h"

using namespace Network;

Network::TCPClient::TCPClient()
	: Client(), _io_service(), _strand(_io_service), _signalRegister(_io_service)
{
 //   handleAsyncWait();
}

Network::TCPClient::~TCPClient()
{}

bool Network::TCPClient::connect(std::string const &ip, std::string const &port)
{
	boost::asio::ip::tcp::resolver	resolver{_io_service};
    boost::asio::ip::tcp::socket	socket{_io_service};

	boost::asio::connect(socket, resolver.resolve({ ip, port }));
    _tcpPacketConnection = std::make_shared<TCPPacketConnection>(
            _io_service,
            std::move(socket),
            _packetCallBack
    );
	return (true);
}

void Network::TCPClient::sendPacket(IPacket::SharedPtr p)
{
    _tcpPacketConnection->sendPacket(p);
}

void Network::TCPClient::sendFile(std::string const &ip, std::string const &port, Network::ByteBuffer bytes)
{
    _fileExchanger.sendFile(ip, port, bytes);
}

std::shared_ptr<FileExchanger::FileExchange>	Network::TCPClient::prepareFileReception()
{
	return (_fileExchanger.prepareReception());
}

void TCPClient::receiveFile(std::shared_ptr<FileExchanger::FileExchange> fileExchange, size_t expectedSize,
                            Network::RawCallback callback)
{
    _fileExchanger.receiveFile(fileExchange, expectedSize, callback);
}

void Network::TCPClient::disconnect()
{
    _tcpPacketConnection->stop();
}

void Network::TCPClient::run()
{
    _tcpPacketConnection->start();
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
                _tcpPacketConnection->stop();
            });
}