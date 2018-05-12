#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
	Network::TCPServer server("0.0.0.0", "4242");

    server.setCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr genericPacket){
        auto packet = std::static_pointer_cast<Network::TCPPacket>(genericPacket);

        std::cout << "Received " << *packet << std::endl;

        if (packet->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
        {
            std::cout << "Login request for " << packet->getTCPPacket().authmessage().user().username() << std::endl;
            //Confirm connection by notfying the client with no error
            packet->getTCPPacket().mutable_authmessage()->set_code(0);
            co->sendPacket(packet);
            std::cout << "User logged in" << std::endl;
        }
    });
	server.run();
    return 0;
}