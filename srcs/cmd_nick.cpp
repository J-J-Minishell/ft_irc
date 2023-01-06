#include "main.hpp"

int		cmd_nick(Message &message)
{
	User &user = message.get_user();
	std::string line;

	line = message.get_user().get_nick() + " has change his NICK to: " + message.get_params()[0] + "\n";

	message.set_lineToSend(line);
	user.set_nick(message.get_params().front());
	
	user.updateMask();
	return 0;
}