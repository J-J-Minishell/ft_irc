#include "main.hpp"

int		cmd_part(Message &message)
{
	channelsMap	&channelsMap = message.get_server().getChannelsMap();
	User		*user = message.get_user();
	std::string	line;
	std::string	messageParam;

	if (message.get_params().empty())
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "PART"));

	for(channelsMap::iterator it = channelsMap.begin(); it != channelsMap.end(); it++)
	{
		if (it->first == message.get_params()[0])
		{
			if (it->second->is_user_in_chan(user))
			{
				messageParam = vectorToString(std::vector<std::string>(message.get_params().begin() + 1, message.get_params().end()));
				line = user->get_mask() + " PART :" + it->first;
				line += messageParam.empty() ? "\r\n" : (":" + messageParam + "\r\n");
				it->second->send(line, -1);
				it->second->delete_user(user);
			}
			else
				message.send_numeric(" 442 ", findAndReplace(Message::numericsMap[ERR_NOTONCHANNEL], "<channel>", message.get_params()[0]));
			return 0;
		}
	}

	return message.send_numeric(" 403 ", findAndReplace(Message::numericsMap[ERR_NOSUCHCHANNEL], "<channel name>", message.get_params()[0]));
}
