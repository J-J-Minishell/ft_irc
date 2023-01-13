#include "main.hpp"

int		cmd_nick(Message &message)
{
	User &user = *message.get_user();
	std::string line;
	std::string specialChar = SPECIALCHAR;

	if (message.get_params().empty())
		return message.send_numeric(" 431 ", Message::numericsMap[ERR_NONICKNAMEGIVEN]);

	if (message.get_params()[0].size() > 9)
		return message.send_numeric(" 432 ", Message::numericsMap[ERR_ERRONEUSNICKNAME]);

	for (size_t i=0; i < message.get_params()[0].size(); i++)
	{
		if ((i == 0) && !isalpha(message.get_params()[0][i]) && specialChar.find(message.get_params()[0][i]) == std::string::npos)
			return message.send_numeric(" 432 ", Message::numericsMap[ERR_ERRONEUSNICKNAME]);
		else if (std::isalpha(message.get_params()[0][i]) || std::isdigit(message.get_params()[0][i]) || specialChar.find(message.get_params()[0][i]) != std::string::npos)
			std::cout << user.get_mask() + " NICK :" + message.get_params()[0] << std::endl;
		else
			return message.send_numeric(" 432 ", Message::numericsMap[ERR_ERRONEUSNICKNAME]);
    }

	for (UserMapIterator it = message.get_server().getUserMap().begin(); it != message.get_server().getUserMap().end(); it++)
	{
		if (it->second->get_nick() == message.get_params()[0])
			return message.send_numeric(" 433 ", Message::numericsMap[ERR_NICKNAMEINUSE]);
	}

	line = user.get_mask() + " NICK :" + message.get_params()[0] + "\n";

	message.set_lineToSend(line);
	user.set_nick(message.get_params().front());
	
	user.updateMask();

	if (!user.isRegistered() && user.get_username() != "unknown" && user.get_nick() != "*")
		send_all(user.get_fd(), "PING :irc-serv\n");
	return 0;
}
   