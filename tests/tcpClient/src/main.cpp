#include <iostream>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
    Network::TCPClient client{};
	Network::TCPPacket::SharedPtr packet = std::make_shared<Network::TCPPacket>();

    client.setPacketCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr genericPacket)
                             {
                                 auto packet = std::static_pointer_cast<Network::TCPPacket>(genericPacket);

                                 std::cout << "Received " << *packet << std::endl;
                                 if (packet->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
                                 {
                                     if (!packet->getTCPPacket().authmessage().user().username().compare("Oxydros"))
                                     {
                                         std::cout << "Logged in" << std::endl;
                                     }
                                 }
                             });
	client.connect("127.0.0.1", "4242");
    //Login packet for "Oxydros"
    packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
    packet->getTCPPacket().mutable_authmessage()->set_code(0);
    packet->getTCPPacket().mutable_authmessage()->mutable_user()->set_username("Oxydros");

    client.sendPacket(packet);
	client.async_run();
	client.wait();
	TRACE("Leaving client ..." << std::endl);
    return 0;
}
