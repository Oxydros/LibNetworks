#pragma once

#include <boost/thread/mutex.hpp>
#include "Common.h"
#include "IConnection.h"

namespace Network
{
    //Forward declaration
    class UDPConnectionManager;

    //! UDPConnection
    /*!
     * Implementation of a UDPConnection
     */
	class UDPConnection : public IConnection, public std::enable_shared_from_this<UDPConnection>
	{
    public:
        typedef boost::asio::ip::udp::endpoint  endpoint;
        typedef std::shared_ptr<UDPConnection>  SharedPtr;
	private:
        boost::asio::strand                 &_strand;
        boost::asio::ip::udp::socket        &_socket;
        UDPConnectionManager		    	*_connectionManager;
        endpoint                            _remoteEndpoint;
        boost::mutex                        _ioMutex;
        //Use in sending
        std::vector<unsigned char>			_toSendBuffer;

	public:
		explicit UDPConnection(boost::asio::strand &strand,
                               boost::asio::ip::udp::socket &socket,
                               endpoint &remote,
                               UDPConnectionManager *manager = nullptr);
		virtual ~UDPConnection();

	public:
		virtual void start();
		virtual void stop();
		virtual bool sendPacket(IPacket const &packet);

    public:
        Network::UDPConnection::endpoint const &getEndpoint() const noexcept;

    private:
        void        checkWrite();
        void        handleWrite(boost::system::error_code ec);
        void 	    processWrite(boost::system::error_code &ec);
	};
}