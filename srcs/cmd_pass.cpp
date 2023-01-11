#include "main.hpp"

int		cmd_pass(Message &message)
{
	User		&user = *message.get_user();

	if (user.get_password() == 1)
		return message.send_numeric(" 462 ", Message::numericsMap[ERR_ALREADYREGISTRED]);
	if (message.get_params().size() < 1)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "PASS"));
	if (message.get_params()[0] == message.get_server().getPassword())
		user.set_password(1);
	return 0;
}
