#include "main.hpp"

int		cmd_pass(Message &message)
{
	User		&user = message.get_user();
	std::string	numericStr;
	
	if (user.get_password() == 1)
		return message.send_numeric(" 462 ", Message::numericsMap[ERR_ALREADYREGISTRED]);
	if (message.get_params().size() < 1)
	{
		numericStr = Message::numericsMap[ERR_NEEDMOREPARAMS];
		numericStr.replace(numericStr.find("<command>"), 9, "USER");
		return message.send_numeric(" 461 ", numericStr);
	}
	if (message.get_params()[0] == message.get_server().getPassword())
		user.set_password(1);
	return 0;
}
