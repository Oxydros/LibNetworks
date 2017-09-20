#include <memory>
#include "TCPServer.h"

using namespace Network;

Network::TCPServer::TCPServer(std::string const &ip, std::string const &serverPort,
	PacketObserver &observer) : _serverIp(ip), _serverPort(serverPort),
	_packetObserver(observer),
	_io_service(), _serverSocket(_io_service), _signalRegister(_io_service),
	_acceptor(_io_service)
{
	handleAsynWait();

	boost::asio::ip::tcp::resolver	resolver(_io_service);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ ip, serverPort });

	_acceptor.open(endpoint.protocol());
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_acceptor.bind(endpoint);
	_acceptor.listen();

	processAccept();
}

Network::TCPServer::~TCPServer()
{
	_acceptor.close();
	_connectionManager.stop_all();
}

bool Network::TCPServer::run()
{
	_io_service.run();
	return (false);
}

void Network::TCPServer::handleAsynWait()
{
	//Register signals to be handle in async_wait
	//Allow us to exit the server correctly
	_signalRegister.add(SIGINT);
	_signalRegister.add(SIGTERM);

	//Process the lambda if one of the signals is received
	_signalRegister.async_wait(
		[this](boost::system::error_code ec, int sigNbr)
	{
		//Close the acceptor socket
		_acceptor.close();
		//Close all opened connections
		_connectionManager.stop_all();
	});
}

void Network::TCPServer::processAccept()
{
	_acceptor.async_accept(_serverSocket,
		[this](boost::system::error_code ec)
	{
		if (!_acceptor.is_open())
			return;
		if (!ec)
		{
			//Take the socket ownership
			//Instantiate a connection with it
			_connectionManager.add(
				std::make_shared<TCPConnection>(
					std::move(_serverSocket),
					_connectionManager,
					_packetObserver
					));
		}
		processAccept();
	});
}