#include <iostream>
#include <Debug.h>
#include <TCPClient.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
    Network::TCPClient client{};
	Network::TCPPacket::SharedPtr packet = std::make_shared<Network::TCPPacket>();
	CubZPacket::UserDescription *user = new CubZPacket::UserDescription();

    client.setPacketCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
                             {
                                 auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);

                                 std::cout << "Received " << *tcpPacket << std::endl;
                                 if (tcpPacket->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
                                 {
                                     if (!tcpPacket->getAuthMessage().user().username().compare("Oxydros"))
                                     {
                                         std::cout << "Logged in" << std::endl;
                                     }
                                 }
                             });
	client.connect("127.0.0.1", "4242");
    //Login packet for "Oxydros"
    packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
    packet->getMutableAuthMessage()->set_code(0);
    user->set_username("Oxydros");
    packet->getMutableAuthMessage()->set_allocated_user(user);

    client.sendPacket(packet);
	client.async_run();
	client.wait();
	dout << "Leaving client ..." << std::endl;
    return 0;
}
