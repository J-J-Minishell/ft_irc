#include "Server.hpp"
#include <stdlib.h>
#include <iostream>

int main(int argc, char **argv)
{
	Server*			server;
	int				serverPort;

	if (argc != 2)
	{
		std::cerr << "Introduce port as an argument" << std::endl;
		return 1;
	}

	serverPort = atoi(argv[1]);
	if (!serverPort && serverPort >= 65536)
	{
		std::cerr << "Error: Invalid port number" << std::endl;
		return 1;
	}
	
	server = Server::createInstance(argv[1]);
	server->run();

	Server::deleteInstance();
	return 0;
}