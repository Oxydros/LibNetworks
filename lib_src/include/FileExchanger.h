//
// Created by Louis VENTRE (aka Oxydros) on 5/11/18.
//

#ifndef LIBNETWORKS_FILEEXCHANGER_H
#define LIBNETWORKS_FILEEXCHANGER_H

#include <boost/thread.hpp>
#include "Common.h"
#include "TCPRawConnection.h"

namespace Network
{
    class FileExchanger
    {
	public:
		struct FileExchange : public std::enable_shared_from_this<FileExchange>
		{
			std::shared_ptr<TCPRawConnection>					tcpConnection;
			std::unique_ptr<boost::asio::ip::tcp::acceptor>		acceptor;
		};

    private:
        typedef std::pair<std::shared_ptr<FileExchange>, Network::RawCallback>  FileConnection;

    private:
        boost::asio::io_service                     _io_service;
        boost::asio::io_service::strand             _strand;
        boost::mutex                                _fileMutex;
        std::vector<FileConnection>                 _tcpFileConnections;
        std::vector<boost::thread>                  _threads;

    public:
        FileExchanger();
        ~FileExchanger();

        void        launchService();

        void        sendFile(std::string const &ip, std::string const &port, ByteBuffer bytes);

		std::shared_ptr<FileExchange>	prepareReception();

        void        receiveFile(std::shared_ptr<FileExchange> fileExchange, size_t expectedSize,
                                Network::RawCallback callback);

        void        fileTransferFinished(std::shared_ptr<IRawConnection> connection, ByteBuffer bytes);

        void        waitTransactions();
    };
}

#endif //LIBNETWORKS_FILEEXCHANGER_H
