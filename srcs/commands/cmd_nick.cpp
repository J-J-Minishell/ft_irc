#include "main.hpp"

int		cmd_nick(Message &message)
{
	User &user = *message.get_user();
	std::string &newNickname = message.get_params()[0];
	std::string line;
	std::string specialChar = SPECIALCHAR;

	if (message.get_params().empty())
		return message.send_numeric(" 431 ", Message::numericsMap[ERR_NONICKNAMEGIVEN]);

	if (newNickname.size() > 9 || (!isalpha(newNickname[0]) && specialChar.find(newNickname[0]) == std::string::npos))
		return message.send_numeric(" 432 ", Message::numericsMap[ERR_ERRONEUSNICKNAME]);

	for (size_t i = 1; i < newNickname.size(); i++)
		if (!std::isalpha(newNickname[i]) && !std::isdigit(newNickname[i]) && specialChar.find(newNickname[i]) == std::string::npos)
			return message.send_numeric(" 432 ", Message::numericsMap[ERR_ERRONEUSNICKNAME]);

	for (UserMapIterator it = message.get_server().getUserMap().begin(); it != message.get_server().getUserMap().end(); it++)
		if (it->second->get_nick() == newNickname)
			return message.send_numeric(" 433 ", Message::numericsMap[ERR_NICKNAMEINUSE]);

	line = user.get_mask() + " NICK :" + newNickname + "\n";
	user.set_nick(newNickname);
	user.updateMask();

	if (user.isRegistered())
	{
		send_all(user.get_fd(), line.c_str());
		message.send_message(user.get_users_from_channels(), line);
	}

	if (!user.isRegistered() && user.get_username() != "unknown" && user.get_nick() != "*")
		send_all(user.get_fd(), "PING :irc-serv\n");
	return 0;
}
   