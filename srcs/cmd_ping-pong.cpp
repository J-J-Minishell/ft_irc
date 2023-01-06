#include "main.hpp"

int		cmd_ping(Message &message)
{
	User		&user = message.get_user();
	std::string	line;


	user.updateMask();
	return 0;
}

int		cmd_pong(Message &message)
{
	User		&user = message.get_user();
	std::string	line;

	user.set_registered(true);
	user.reset_time();
	user.set_timeout(0);

	user.updateMask();
	return 0;
}