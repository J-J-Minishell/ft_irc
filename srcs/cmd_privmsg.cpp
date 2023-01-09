#include "main.hpp"

int		cmd_privmsg(Message &message)
{
	Server &server = message.get_server();

	std::vector<std::string> &params = message.get_params();
	std::string numericStr;

	if (params.size() < 2)
	{
		numericStr = Message::numericsMap[ERR_NEEDMOREPARAMS];
		numericStr.replace(numericStr.find("<command>"), 9, "PRIVMSG");
		return message.send_numeric(" 461 ", numericStr);
	}

	UserMap			&userMap = server.getUserMap();
	UserMapIterator	it;
	for (it = userMap.begin(); it != userMap.end(); it++)
	{
		if (it->second->get_nick() == params[0])
		{
			std::vector<std::string>::iterator param_it = params.begin();
			numericStr = *++param_it;
			while (++param_it != params.end())
				numericStr += " " + *param_it;
			numericStr = ":" + message.get_user().get_mask() + " PRIVMSG " + it->second->get_nick() + " :" + numericStr + "\n";
			send_all(it->second->get_fd(), numericStr.c_str());
			return -1;
		}
	}
	if (it == userMap.end())
	{
		numericStr = Message::numericsMap[ERR_NOSUCHNICK];
		numericStr.replace(numericStr.find("<nickname>"), 10, params[0]);
		return message.send_numeric(" 401 ", numericStr);
	}

	return 0;
}