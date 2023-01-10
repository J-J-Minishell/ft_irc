#include "main.hpp"

int main(int argc, char **argv)
{
	Server*			server;
	int				serverPort;
	std::string 	passwd;

	if (argc != 3)
	{
		std::cerr << "Introduce port and password as arguments" << std::endl;
		return 1;
	}

	serverPort = atoi(argv[1]);
	if (!serverPort && serverPort >= 65536)
	{
		std::cerr << "Error: Invalid port number" << std::endl;
		return 1;
	}
	passwd.assign(argv[2]);
	if (passwd.empty())
	{
		std::cerr << "Error: Invalid password size" << std::endl;
		return 1;
	}
	
	server = Server::createInstance(argv[1], passwd);
	server->run();

	Server::deleteInstance();
	return 0;
}
