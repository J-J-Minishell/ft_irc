#include "main.hpp"

int		cmd_die(Message &message)
{
	User	*user = message.get_user();
	Server	&server = message.get_server();

	if (!user->isOper())
		return message.send_numeric(" 481 ", Message::numericsMap[ERR_NOPRIVILEGES]);

	server.shutdown();
	message.delete_user();
	return -1;
}