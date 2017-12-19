#include <memory>
#include "Debug.h"
#include "TCPServer.h"

using namespace Network;

Network::TCPServer::TCPServer(std::string const &ip, std::string const &serverPort)
        : _serverIp(ip), _serverPort(serverPort),
        _callBack(), _io_service(), _serverSocket(_io_service),
          _signalRegister(_io_service),_acceptor(_io_service)
{
	handleAsyncWait();

	boost::asio::ip::tcp::resolver	resolver(_io_service);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ ip, serverPort });

	dout << "Launching server on " << ip << ":" << serverPort << std::endl;

	_acceptor.open(endpoint.protocol());
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_acceptor.bind(endpoint);
	_acceptor.listen();

	dout << "Launched" << std::endl;

	processAccept();
}

Network::TCPServer::~TCPServer()
{
	dout << "Stopping server..." << std::endl;
	_acceptor.close();
	_connectionManager.stop_all();
}

bool Network::TCPServer::run()
{
	dout << "Launching io_service" << std::endl;
	_io_service.run();
	return (false);
}

void Network::TCPServer::handleAsyncWait()
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
					_callBack,
					&_connectionManager
					));
		}
		processAccept();
	});
}

void Network::TCPServer::setCallback(PacketObserver &callback)
{
    _callBack = std::move(callback);
}

void Network::TCPServer::setCallback(PacketObserver &&callback)
{
    _callBack = callback;
}