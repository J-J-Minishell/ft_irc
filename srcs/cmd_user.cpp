#include "main.hpp"

int		cmd_user(Message &message)
{
	User		&user = message.get_user();
	std::string	numericStr;

	if (user.get_username() == "unknown")
	{
		if (message.get_params().size() < 4)
		{
			numericStr = Message::numericsMap[ERR_NEEDMOREPARAMS];
			numericStr.replace(numericStr.find("<command>"), 9, "USER");
			return message.send_numeric(" 461 ", numericStr);
		}
		user.set_username(message.get_params()[0]);
		if (!user.isRegistered() && user.get_nick() != "*")  // && user.getPassword()
			send_all(user.get_fd(), "PING :irc-serv\n");
	}
	else
		return message.send_numeric(" 462 ", Message::numericsMap[ERR_ALREADYREGISTRED]);

	user.updateMask();
	return 0;
}