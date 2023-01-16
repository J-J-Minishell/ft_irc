#include "main.hpp"

int		cmd_names(Message &message)
{
	UserMap			&userMap = message.get_server().getUserMap();
	UserMapIterator it = userMap.begin();
	std::string		line;

	line = "= :" + it->second->get_nick();
	for (it++ ; it != userMap.end(); it++)
		line += " " + it->second->get_nick();

	message.send_numeric(" 353 ", line);
	message.send_numeric(" 366 ", Message::numericsMap[RPL_ENDOFNAMES]);
	return 0;
}
