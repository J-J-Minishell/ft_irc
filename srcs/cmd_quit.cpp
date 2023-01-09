#include "main.hpp"

int		cmd_quit(Message &message)
{
	User &user = message.get_user();
	Server &server = message.get_server();
	std::string line;

	line = "ERROR :Closing link: (" + user.get_username() + "@" + user.get_host() + ") [Client exited]\n";
	send_all(user.get_fd(), line.c_str());
	server.quitUser(user);

	return -1;
}