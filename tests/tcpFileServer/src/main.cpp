#include <iostream>
#include <Debug.h>
#include <FileExchanger.h>
#include <boost/filesystem.hpp>

#define TEST_FILE   ("test.txt")

int main()
{
	Network::FileExchanger client{};

	client.receiveFile("0.0.0.0", "4242", boost::filesystem::file_size(TEST_FILE),
    [](Network::ByteBuffer bytes){
	    std::cout << "Received " << bytes.size() << " bytes" << std::endl;
	});
	client.waitTransactions();
    return 0;
}