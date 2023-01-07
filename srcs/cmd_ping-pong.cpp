#include "main.hpp"

int		cmd_ping(Message &message)
{
	User		&user = message.get_user();
	std::string	line;

	if (message.get_params().empty())
	{
		line = Message::numericsMap[ERR_NEEDMOREPARAMS];
		line.replace(line.find("<command>"), 9, "PING");
		return message.send_numeric(" 461 ", line);
	}
	line = message.prefix + " PONG " + message.get_server().getServerName() + " :" + message.get_params()[0] + "\n";
	send_all(user.get_fd(), line.c_str());

	return 0;
}

int		cmd_pong(Message &message)
{
	User		&user = message.get_user();
	std::string	line;

	if (!user.isRegistered())  // && user.getPassword() && user.get_nick() != "*" && user.get_username() != "*"
		message.welcome_user();

	user.reset_time();
	user.set_timeout(0);

	return 0;
}