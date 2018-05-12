#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

void sendPing(Network::TCPClient &client)
{
    Network::TCPPacket  packet;

    packet.setType(Network::TCPPacket::Type::PacketTCP_Type_PING);
    packet.getMutablePingMessage()->set_action(CubZPacket::PingMessage_Action::PingMessage_Action_PING);
    packet.getMutablePingMessage()->set_pingid(1);
    client.sendPacket(packet.shared_from_this());
    packet.getMutablePingMessage()->set_pingid(2);
    client.sendPacket(packet.shared_from_this());
}

void sendAuth(Network::TCPClient &client)
{
    Network::TCPPacket  packet;

    packet.setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
    packet.getMutableAuthMessage()->set_action(CubZPacket::AuthMessage_Action::AuthMessage_Action_LOGIN);
    packet.getMutableAuthMessage()->mutable_user()->set_username("Oxydros");
    client.sendPacket(packet.shared_from_this());
}

int main()
{
    std::string         input;
    Network::TCPClient  client;

    client.setPacketCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
                             {
                                 auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);

                                 std::cout << "Received " << *tcpPacket << std::endl;
                             });
    client.connect("127.0.0.1", "4242");

    boost::thread       clientThread([&](){
        client.run();
    });

    std::cout << ">";
    while(std::getline(std::cin, input))
    {
        std::map<std::string, decltype(&sendPing)> map = {
                {"ping", sendPing},
                {"auth", sendAuth}
        };
        auto find = map.find(input);
        if (find != map.end())
            find->second(client);
        else
            std::cout << "Cmd not found" << std::endl;
        std::cout << ">";
    }

    dout << "Leaving client ..." << std::endl;
    client.disconnect();
    clientThread.join();
    return 0;
}