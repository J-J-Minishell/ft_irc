#include "main.hpp"

int		cmd_kick(Message &message)
{
	std::vector<std::string>	&params = message.get_params();
	Server						&server = message.get_server();
	User						*user = message.get_user();
	UserMap::iterator 			kickUserIt;
	std::string					line;
	std::string					messageParam;

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "KICK")) ;

	for(channelsMap::iterator it = server.getChannelsMap().begin(); it != server.getChannelsMap().end(); it++)
	{
		if (it->first == message.get_params()[0])
		{
			for (kickUserIt = server.getUserMap().begin(); 1; kickUserIt++)
			{
				if (kickUserIt == server.getUserMap().end())
					return message.send_numeric(" 401 ", findAndReplace(Message::numericsMap[ERR_NOSUCHNICK], "<nickname>", params[1]));
				if (kickUserIt->second->get_nick() == params[1])
					break;
			}

			if (it->second->is_user_in_chan(user))
			{
				if (it->second->userPrivileges(user) == 0)
					return message.send_numeric(" 482 ", findAndReplace(Message::numericsMap[ERR_CHANOPRIVSNEEDED], "<channel>", params[0]));
				if (it->second->is_user_in_chan(kickUserIt->second) == false)
					return message.send_numeric(" 441 ", findAndReplace(Message::numericsMap[ERR_USERNOTINCHANNEL], "<nick> <channel>", params[1] + " " + params[0]));
				messageParam = vectorToString(std::vector<std::string>(message.get_params().begin() + 2, message.get_params().end()));
				line = user->get_mask() + " KICK " + it->first + " " + kickUserIt->second->get_nick() + " :";	
				line += (messageParam.empty() ? user->get_nick() : messageParam) + "\n";
				it->second->send(line, -1);
				it->second->delete_user(kickUserIt->second);
			}
			else
				message.send_numeric(" 442 ", findAndReplace(Message::numericsMap[ERR_NOTONCHANNEL], "<channel>", message.get_params()[0]));			
			return 0;
		}
	}

	return message.send_numeric(" 403 ", findAndReplace(Message::numericsMap[ERR_NOSUCHCHANNEL], "<channel name>", params[0]));
}
