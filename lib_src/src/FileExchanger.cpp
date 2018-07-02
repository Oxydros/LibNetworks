//
// Created by Louis VENTRE (aka Oxydros) on 5/11/18.
//

#include <string>
#include <Common.h>
#include <FileExchanger.h>

using namespace Network;

FileExchanger::FileExchanger() : _io_service(), _strand(_io_service)
{

}

FileExchanger::~FileExchanger()
{
}

void FileExchanger::waitTransactions()
{
    for(auto it = _threads.begin(); it != _threads.end(); ++it)
    {
        it->join();
    }
}

void FileExchanger::launchService()
{
    _io_service.run();
}

void FileExchanger::sendFile(const std::string &ip, const std::string &port, ByteBuffer bytes)
{
	boost::mutex::scoped_lock   lock{ _fileMutex };

    _threads.push_back(boost::thread([this, ip, port, bytes](){
        TCPMSG("FileExchanger receive command to send file of size " << bytes.size() << std::endl);

        boost::asio::ip::tcp::resolver	resolver{_io_service};
        boost::asio::ip::tcp::socket	socket{_io_service};

        boost::asio::connect(socket, resolver.resolve({ ip, port }));

        auto newConnection = std::make_shared<TCPRawConnection>(_io_service, std::move(socket),
                                                                std::bind(&FileExchanger::fileTransferFinished, this,
                                                                          std::placeholders::_1, std::placeholders::_2));
        newConnection->sendRawBytes(bytes);

		auto fileExchange = std::make_shared<FileExchange>();

		fileExchange->tcpConnection = newConnection;

        auto newPair = std::make_pair(fileExchange, nullptr);
        _fileMutex.lock();
        _tcpFileConnections.push_back(newPair);
        _fileMutex.unlock();

		_io_service.reset();
		_io_service.run();

		TCPMSG("END THREAD SEND FILE" << std::endl);
    }));
}

std::shared_ptr<FileExchanger::FileExchange>	FileExchanger::prepareReception()
{
	boost::asio::ip::tcp::acceptor	acceptor{ _io_service };
	boost::asio::ip::tcp::resolver	resolver{ _io_service };
	boost::asio::ip::tcp::endpoint  endpoint{ *resolver.resolve({ "127.0.0.1", "0" }) };
	auto fileExchange = std::make_shared<FileExchange>();

	fileExchange->acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(std::move(acceptor));

	fileExchange->acceptor->open(endpoint.protocol());
	fileExchange->acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	fileExchange->acceptor->bind(endpoint);

	return (fileExchange);
}

void FileExchanger::receiveFile(std::shared_ptr<FileExchange> fileExchange, size_t expectedSize,
                                Network::RawCallback callback)
{
	boost::mutex::scoped_lock   lock{ _fileMutex };

    _threads.push_back(boost::thread([this, fileExchange, expectedSize, callback](){
        boost::asio::ip::tcp::socket	_serverSocket{_io_service};

		fileExchange->acceptor->listen();

        TCPMSG("Waiting connection for file reception " << std::endl);
		fileExchange->acceptor->accept(_serverSocket);


        auto newConnection = std::make_shared<TCPRawConnection>(_io_service, std::move(_serverSocket),
                                                                std::bind(&FileExchanger::fileTransferFinished, this,
                                                                          std::placeholders::_1, std::placeholders::_2));

		fileExchange->acceptor->close();

		TCPMSG("EXPECTED " << expectedSize << std::endl);
        newConnection->readRawBytes(expectedSize);

		fileExchange->tcpConnection = newConnection;

        auto newPair = std::make_pair(fileExchange, callback);

        _fileMutex.lock();
        _tcpFileConnections.push_back(newPair);
        _fileMutex.unlock();

		_io_service.reset();
		_io_service.run();

		TCPMSG("END THREAD RECEIVE FILE" << std::endl);
    }));
}

void FileExchanger::fileTransferFinished(std::shared_ptr<Network::IRawConnection> connection, Network::ByteBuffer bytes)
{
    boost::mutex::scoped_lock   lock{_fileMutex};

    auto it = std::find_if(_tcpFileConnections.begin(), _tcpFileConnections.end(),
                           [connection](auto &co) -> bool {
                               return (co.first->tcpConnection.get() == connection.get());
                           });

	TCPMSG("Finished raw co" << std::endl);
    if (it->second)
        it->second(bytes);
    _tcpFileConnections.erase(it);
}