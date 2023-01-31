#include "main.hpp"

void	cmd_user(Message &message)
{
	User		&user = *message.get_user();
	std::string	numericStr;

	if (user.get_username() == "unknown")
	{
		if (message.get_params().size() < 4)
			return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "USER"));
		user.set_username(message.get_params()[0]);
		if (!user.isRegistered() && user.get_nick() != "*")
			send_all(&user, "PING :irc-serv\r\n");
	}
	else
		return message.send_numeric(" 462 ", Message::numericsMap[ERR_ALREADYREGISTRED]);

	user.updateMask();
}