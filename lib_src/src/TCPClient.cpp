#include "TCPClient.h"

using namespace Network;

Network::TCPClient::TCPClient(PacketObserver &o)
	: Client(o), _io_service(), _socket(_io_service)
{
}

Network::TCPClient::~TCPClient()
{}

bool Network::TCPClient::connect(std::string const &ip, std::string const &port)
{
	boost::asio::ip::tcp::resolver	resolver(_io_service);

	boost::asio::connect(_socket, resolver.resolve({ ip, port }));
	return (true);
}

void Network::TCPClient::disconnect()
{
	_socket.close();
}
void Network::TCPClient::run()
{
	_io_service.run();
}
