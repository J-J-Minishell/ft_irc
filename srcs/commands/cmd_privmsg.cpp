#include "main.hpp"

int		find_nick(Message &message, std::string line)
{
	std::vector<std::string>	&params = message.get_params();
	UserMap						&userMap = message.get_server().getUserMap();

	for (UserMapIterator it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it->second->get_nick() == params[0])
		{
			send_all(it->second->get_fd(), line.c_str());
			return 1;
		}
	}
	return 0;
}

int		find_channel(Message &message, std::string line)
{
	std::vector<std::string>	&params = message.get_params();
	channelsMap					&channelsMap = message.get_server().getChannelsMap();

	for (channelsMap::iterator it = channelsMap.begin(); it != channelsMap.end(); it++)
	{
		if (it->second->get_name() == params[0])
		{
			if (message.get_user()->is_in_channel(it->second))
				it->second->send(line.c_str(), message.get_user()->get_fd());
			else
				message.send_numeric(" 404 ", findAndReplace(Message::numericsMap[ERR_CANNOTSENDTOCHAN], "<channel name>", params[0]));
			return 1;
		}
	}
	return 0;
}

int		cmd_privmsg(Message &message)
{
	std::vector<std::string>	&params = message.get_params();
	std::string					line;

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "PRIVMSG"));

	line = vectorToString(params);
	extractWord(line);
	line = ":" + message.get_user()->get_mask() + " PRIVMSG " + params[0] + " :" + line + "\n";

	if (!find_nick(message, line) && !find_channel(message, line))
		return message.send_numeric(" 401 ", findAndReplace(Message::numericsMap[ERR_NOSUCHNICK], "<nickname>", params[0]));

	return 0;
}