#include "main.hpp"

int		cmd_list(Message &message)
{
	channelsMap &channelsMap = message.get_server().getChannelsMap();
	channelsMap::iterator it = channelsMap.begin();

	for (; it != channelsMap.end(); it++)
		message.send_numeric(" 322 ", it->first);

	message.send_numeric(" 323 ", Message::numericsMap[RPL_LISTEND]);
	return 0;
}
