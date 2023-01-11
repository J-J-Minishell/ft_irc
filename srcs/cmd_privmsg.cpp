#include "main.hpp"

int		cmd_privmsg(Message &message)
{
	Server &server = message.get_server();
	std::vector<std::string> &params = message.get_params();
	std::string line;

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "PRIVMSG"));

	UserMap			&userMap = server.getUserMap();
	UserMapIterator	it;
	for (it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it->second->get_nick() == params[0])
		{
			std::vector<std::string>::iterator param_it = params.begin();
			line = *++param_it;
			while (++param_it != params.end())
				line += " " + *param_it;
			line = ":" + message.get_user().get_mask() + " PRIVMSG " + it->second->get_nick() + " :" + line + "\n";
			send_all(it->second->get_fd(), line.c_str());
			return -1;
		}
	}
	if (it == userMap.end())
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NOSUCHNICK], "<nickname>", params[0]));

	return 0;
}