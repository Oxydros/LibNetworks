#include <iostream>
#include <boost/filesystem.hpp>
#include <Debug.h>
#include <FileExchanger.h>

#define TEST_FILE   ("test.txt")

int main()
{
	Network::FileExchanger client{};

    std::ifstream infile(TEST_FILE, std::ios::in | std::ios::binary);

    std::cout << "Sending file " << TEST_FILE
              << " with size " << boost::filesystem::file_size(TEST_FILE)
              << std::endl;

    Network::ByteBuffer buffer(boost::filesystem::file_size(TEST_FILE));

    infile.read(buffer.data(), boost::filesystem::file_size(TEST_FILE));


	client.sendFile("127.0.0.1", "4242", buffer);
	client.waitTransactions();
	dout << "Leaving client ..." << std::endl;
    return 0;
}
