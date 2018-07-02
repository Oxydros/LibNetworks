
#define CUBZ_NETWORK_DEBUG
#include <iostream>
#include "Debug.h"
#include "FullClient.h"

CubZPacket::EntityDescription *newEntity()
{
	CubZPacket::EntityDescription *entity = new CubZPacket::EntityDescription();

	entity->set_type(CubZPacket::EntityDescription_Type::EntityDescription_Type_CUBE);
	return (entity);
}

int main()
{
	FullClient client{};
	std::string line;

	while (std::getline(std::cin, line))
	{
		if (!line.compare("auth"))
		{
			client.connect("172.16.80.1", "4242");
			client.runAsync();
		}
		else if (!line.compare("file"))
			client.listServerFiles(".");
		else if (!line.compare("users"))
			client.listServerUsers();
		else if (!line.compare("entity"))
			client.createEntity(newEntity());
		else if (!line.compare("deleteEntity"))
		{
			std::cout << "Enter id: " << std::endl;
			std::string id;
			if (!std::getline(std::cin, id))
				return (0);
			int intId = std::atoi(id.c_str());
			CubZPacket::EntityDescription *entity = new CubZPacket::EntityDescription();
			entity->set_id(intId);
			client.deleteEntity(entity);
		}
	}
    return 0;
}
