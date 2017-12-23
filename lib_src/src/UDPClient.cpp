#include "UDPClient.h"

using namespace Network;

Network::UDPClient::UDPClient()
	: Client(), _io_service(), _strand(_io_service),
      _socket(_io_service,
              boost::asio::ip::udp::endpoint(
                      boost::asio::ip::udp::v4(), 0)
              ),
      _manager(_socket, _strand, _callBack),_signalRegister(_io_service)
{
 //   handleAsyncWait();
}

Network::UDPClient::~UDPClient()
{}

bool Network::UDPClient::connect(std::string const &ip, std::string const &port)
{
	boost::asio::ip::udp::resolver	resolver{_io_service};
    boost::asio::ip::udp::endpoint  endpoint{*resolver.resolve(
            {
                    boost::asio::ip::udp::v4(),
                    ip,
                    port
            })};
    _mainCo = std::make_shared<UDPConnection>(
            _strand,
            _socket,
            endpoint,
            &_manager
    );
    _endpoint = endpoint;
	return (true);
}

void Network::UDPClient::sendPacket(IPacket const &p)
{
    _mainCo->sendPacket(p);
}

void Network::UDPClient::disconnect()
{
    _manager.stop_all();
}

void Network::UDPClient::run()
{
    _manager.run();
	_io_service.run();
}

void Network::UDPClient::handleAsyncWait()
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
                _manager.stop_all();
            });
}