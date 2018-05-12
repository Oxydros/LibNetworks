#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>

int main()
{
	Network::TCPServer server("0.0.0.0", "4242");

    server.setCallback([](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet){
        auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);

        std::cout << "Received " << *tcpPacket << std::endl;

        if (tcpPacket->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
        {
            std::cout << "Login request for " << tcpPacket->getAuthMessage().user().username() << std::endl;
            //Confirm connection by notfying the client with no error
            tcpPacket->getMutableAuthMessage()->set_code(0);
            co->sendPacket(packet);
            std::cout << "User logged in" << std::endl;
        }
    });
	server.run();
    return 0;
}