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
    _threads.push_back(boost::thread([this, ip, port, bytes](){
        tcpMsg << "FileExchanger receive command to send file of size " << bytes.size() << std::endl;

        boost::asio::ip::tcp::resolver	resolver{_io_service};
        boost::asio::ip::tcp::socket	socket{_io_service};

        boost::asio::connect(socket, resolver.resolve({ ip, port }));

        auto newConnection = std::make_shared<TCPRawConnection>(_strand, std::move(socket),
                                                                std::bind(&FileExchanger::fileTransferFinished, this,
                                                                          std::placeholders::_1, std::placeholders::_2));
        newConnection->sendRawBytes(bytes);

        auto newPair = std::make_pair(newConnection, nullptr);
        _fileMutex.lock();
        _tcpFileConnections.push_back(newPair);
        _fileMutex.unlock();

        this->launchService();
    }));
}

void FileExchanger::receiveFile(std::string const &ip, std::string const &port, size_t expectedSize,
                                Network::RawCallback callback)
{
    _threads.push_back(boost::thread([this, ip, port, expectedSize, callback](){
        boost::asio::ip::tcp::resolver	resolver{_io_service};
        boost::asio::ip::tcp::endpoint  endpoint{*resolver.resolve({ ip, port })};
        boost::asio::ip::tcp::socket	_serverSocket{_io_service};
        boost::asio::ip::tcp::acceptor	_acceptor{_io_service};

        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen();

        tcpMsg << "Waiting connection for file reception " << std::endl;
        _acceptor.accept(_serverSocket);


        auto newConnection = std::make_shared<TCPRawConnection>(_strand, std::move(_serverSocket),
                                                                std::bind(&FileExchanger::fileTransferFinished, this,
                                                                          std::placeholders::_1, std::placeholders::_2));

        _acceptor.close();
        _serverSocket.close();

        tcpMsg << "EXPECTED " << expectedSize << std::endl;
        newConnection->readRawBytes(expectedSize);

        auto newPair = std::make_pair(newConnection, callback);

        _fileMutex.lock();
        _tcpFileConnections.push_back(newPair);
        _fileMutex.unlock();

        this->launchService();
    }));
}

void FileExchanger::fileTransferFinished(std::shared_ptr<Network::IRawConnection> connection, Network::ByteBuffer bytes)
{
    boost::mutex::scoped_lock   lock{_fileMutex};

    auto it = std::find_if(_tcpFileConnections.begin(), _tcpFileConnections.end(),
                           [connection](auto &co) -> bool {
                               return (co.first.get() == connection.get());
                           });

    tcpMsg << "Finished raw co" << std::endl;
    if (it->second)
        it->second(bytes);
    _tcpFileConnections.erase(it);
}