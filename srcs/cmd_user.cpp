#include "main.hpp"

int		user_cmd(Message &message)
{
	User		&user = message.get_user();
	std::string	numericStr;

	if (user.get_username() == "unknown")
	{
		if (message.get_params().size() < 4)
		{
			numericStr = Message::numericsMap[ERR_NEEDMOREPARAMS];
			numericStr.replace(numericStr.find("<command>"), 9, "USER");
			return message.error(" 461 ", numericStr);
		}
		user.set_username(message.get_params()[0]);
	}
	else
		return message.error(" 462 ", Message::numericsMap[ERR_ALREADYREGISTRED]);

	user.updateMask();
	return 0;
}