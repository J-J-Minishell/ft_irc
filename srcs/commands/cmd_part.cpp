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
				messageParam = vectorToString(message.get_params());
				extractWord(messageParam);
				line = user->get_mask() + " PART :" + it->second->get_name();
				line += messageParam.empty() ? "\n" : ":" + messageParam + "\n";
				it->second->send(line, 0);
				it->second->delete_user(user);
			}
			else
				message.send_numeric(" 442 ", findAndReplace(Message::numericsMap[ERR_NOTONCHANNEL], "<channel>", message.get_params()[0]));
			return 0;
		}
	}

	return message.send_numeric(" 403 ", findAndReplace(Message::numericsMap[ERR_NOSUCHCHANNEL], "<channel name>", message.get_params()[0]));
}
