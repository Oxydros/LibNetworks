#pragma once

#include "Common.h"
#include "IServer.h"
#include "UDPConnectionManager.h"
#include "UDPConnection.h"

namespace Network
{
    //! UDPServer
    /*!
     * Implementation of a UDPServer
     */
	class UDPServer : public IServer
	{
	private:
        short           				_serverPort;
		PacketCallback					_callBack;
		boost::asio::io_service			_io_service;
        boost::asio::io_service::strand             _strand;
		boost::asio::ip::udp::socket	_serverSocket;
		boost::asio::signal_set			_signalRegister;
		UDPConnectionManager    		_connectionManager;

	public:
		explicit UDPServer(short serverPort);
		~UDPServer() override ;

		//Can't copy a server
		UDPServer(UDPServer const &) = delete;
		UDPServer &operator=(UDPServer const &) = delete;

	public:
		bool    run() override;
        void    setCallback(PacketCallback &callback) override;
        void    setCallback(PacketCallback &&callback) override;

	private:
		void	handleAsyncWait();
		void	processAccept();
	};
}