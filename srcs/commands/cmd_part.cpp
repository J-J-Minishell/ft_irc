#include "main.hpp"

int		cmd_part(Message &message)
{
	channelsMap					&channelsMap = message.get_server().getChannelsMap();
	User						*user = message.get_user();
	std::string					line;
	std::string					messageParam;
	std::vector<std::string>	channelNames;
	channelsMap::iterator		it;

	if (message.get_params().empty())
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "PART"));

	if (message.get_params()[0][0] == '0')
		channelNames = user->get_channelsNames();
	else
		channelNames = stringDelimiter(message.get_params()[0], ",");
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		for(it = channelsMap.begin(); it != channelsMap.end(); it++)
		{
			if (it->first == channelNames[i])
			{
				if (it->second->is_user_in_chan(user))
				{
					messageParam = vectorToString(std::vector<std::string>(message.get_params().begin() + 1, message.get_params().end()));
					line = ":" + user->get_mask() + " PART ";
					line += messageParam.empty() ? (":" + it->first + "\r\n") : (it->first + " :" + messageParam + "\r\n");
					it->second->send(line, -1);
					it->second->delete_user(user);
				}
				else
					message.send_numeric(" 442 ", findAndReplace(Message::numericsMap[ERR_NOTONCHANNEL], "<channel>", message.get_params()[0]));
				break;
			}
		}
		if (it == channelsMap.end())
			message.send_numeric(" 403 ", findAndReplace(Message::numericsMap[ERR_NOSUCHCHANNEL], "<channel name>", message.get_params()[0]));
	}

	return 0;
}
