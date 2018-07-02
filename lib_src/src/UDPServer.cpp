#include <memory>
#include "Debug.h"
#include "UDPServer.h"

using namespace boost::asio::ip;
using namespace Network;

Network::UDPServer::UDPServer(short serverPort)
        : _serverPort(serverPort),
        _callBack(), _io_service(), _strand(_io_service),
          _serverSocket(_io_service, udp::endpoint(udp::v4(), serverPort)),
          _signalRegister(_io_service), _connectionManager(_serverSocket, _strand, _callBack)
{
	handleAsyncWait();

	TRACE("Launched UDP server on port " << serverPort << std::endl);
}

Network::UDPServer::~UDPServer()
{
	TRACE("Stopping server..." << std::endl);
	_connectionManager.stop_all();
}

bool Network::UDPServer::run()
{
	TRACE("Launching io_service" << std::endl);
    _connectionManager.run();
	_io_service.run();
	return (false);
}

void Network::UDPServer::handleAsyncWait()
{
	TCPMSG("Configuring signalHandler" << std::endl);

	//Register signals to be handle in async_wait
	//Allow us to exit the server correctly
	_signalRegister.add(SIGINT);
	_signalRegister.add(SIGTERM);

	//Process the lambda if one of the signals is received
	_signalRegister.async_wait(
		[this](boost::system::error_code ec, int sigNbr)
	{
		TCPMSG("Received signal " << sigNbr << std::endl);
		//Close all opened connections
		_connectionManager.stop_all();
        _serverSocket.close();
	});
}

void Network::UDPServer::setCallback(PacketCallback &callback)
{
    _callBack = std::move(callback);
}

void Network::UDPServer::setCallback(PacketCallback &&callback)
{
    _callBack = callback;
}