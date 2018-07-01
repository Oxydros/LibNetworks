#include <iostream>
#include <Debug.h>
#include <TCPServer.h>
#include <TCPPacket.h>
#include <FileExchanger.h>
#include <boost/filesystem.hpp>

#define TEST_FILE   ("test.txt")

int main()
{
	std::ifstream infile(TEST_FILE, std::ios::in | std::ios::binary);

	std::cout << "rECEIVING file " << TEST_FILE
		<< " with size " << boost::filesystem::file_size(TEST_FILE)
		<< std::endl;

	Network::ByteBuffer buffer(boost::filesystem::file_size(TEST_FILE));

	infile.read(buffer.data(), boost::filesystem::file_size(TEST_FILE));

	Network::FileExchanger exchanger{};
	std::shared_ptr<Network::FileExchanger::FileExchange> fExchange{};

	Network::TCPServer server{ "0.0.0.0", "4242" };

	server.setCallback([&server, &fExchange, &exchanger, &buffer](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr genericPacket) {
		auto packet = std::static_pointer_cast<Network::TCPPacket>(genericPacket);

		std::cout << "Received " << *packet << std::endl;

		if (packet->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_FILE)
		{
			fExchange = exchanger.prepareReception();

			std::cout << "Prepared file exchange on port " << fExchange->acceptor->local_endpoint().port() << std::endl;

			exchanger.receiveFile(fExchange, boost::filesystem::file_size(TEST_FILE),
				[&buffer](Network::ByteBuffer bytes) {
				std::cout << "Received " << bytes.size() << " bytes" << std::endl;

				for (size_t i = 0; i < buffer.size(); i++)
				{
					if (buffer[i] != bytes[i])
					{
						std::cout << "DIFFERENCE IN BYTES AT ADDR " << i << std::endl;
						std::cout << "FAILED TEST" << std::endl;
						return;
					}						
				}
				std::cout << "SUCCESS" << std::endl;
			});

			packet->getTCPPacket().mutable_filemessage()->set_port(fExchange->acceptor->local_endpoint().port());
			co->sendPacket(packet);
		}
	});
	server.run();
	return 0;
}