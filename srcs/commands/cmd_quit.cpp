#include "main.hpp"

int		cmd_quit(Message &message)
{
	User *user = message.get_user();
	Server &server = message.get_server();
	std::string line;

	line = "ERROR :Closing link: (" + user->get_username() + "@" + user->get_host() + ") ";
	if (!message.get_params().empty())
		line += "[" + message.get_params()[0] + "]\n";
	else
		line += "[Client exited]\n";
	
	send_all(user, line.c_str());

	if (!message.get_params().empty())
		line = ":" + user->get_mask() + "QUIT :" + message.get_params()[0] + "\n";
	else
		line = ":" + user->get_mask() + "QUIT :Client exited\n";

	message.send_message(user->get_users_from_channels(), line);
	server.quitUser(user);
	message.delete_user();
	return -1;
}