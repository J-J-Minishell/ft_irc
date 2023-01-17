#include "main.hpp"

void	cmd_names_inChannel(Message &message, std::string channelName)
{
	channelUsersMap &channelUsersMap = *message.get_server().getChannelUsersMap(channelName);
	channelUsersMap::iterator it;
	std::string	line;

	it = channelUsersMap.begin();
	line = "= :" + it->first->get_nick();
	for (it++; it != channelUsersMap.end(); it++)
		line += " " + it->first->get_nick();

	message.send_numeric(" 353 ", line);
	message.send_numeric(" 366 ", findAndReplace(Message::numericsMap[RPL_ENDOFNAMES], "<channel>", channelName));
}

int		cmd_names(Message &message)
{
	UserMap			&userMap = message.get_server().getUserMap();
	UserMapIterator	it;
	std::string		line;

	if (message.get_params().empty())
	{
		it = userMap.begin();
		line = "= :" + it->second->get_nick();
		for (it++ ; it != userMap.end(); it++)
			line += " " + it->second->get_nick();
		message.send_numeric(" 353 ", line);
		message.send_numeric(" 366 ", Message::numericsMap[RPL_ENDOFNAMES]);
	}
	else
		cmd_names_inChannel(message, message.get_params()[0]);
	return 0;
}
