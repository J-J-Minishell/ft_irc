#include "main.hpp"

void	cmd_kick(Message &message)
{
	std::vector<std::string>	&params = message.get_params();
	Server						&server = message.get_server();
	User						*user = message.get_user();
	UserMap::iterator 			kickUserIt;
	std::vector<std::string>	userNames;
	std::string					line;
	std::string					messageParam;

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "KICK")) ;

	for(channelsMap::iterator it = server.getChannelsMap().begin(); it != server.getChannelsMap().end(); it++)
	{
		if (it->first == message.get_params()[0])
		{
			userNames = stringDelimiter(message.get_params()[1], ",");
			for (size_t i = 0; i < userNames.size(); i++)
			{
				kickUserIt = server.getUserMap().begin();
				while (kickUserIt != server.getUserMap().end() && kickUserIt->second->get_nick() != userNames[i])
					kickUserIt++;
				if (kickUserIt == server.getUserMap().end())
					message.send_numeric(" 401 ", findAndReplace(Message::numericsMap[ERR_NOSUCHNICK], "<nickname>", userNames[i]));
				else if (it->second->is_user_in_chan(user))
				{
					if (it->second->userPrivileges(user) == 0)
						message.send_numeric(" 482 ", findAndReplace(Message::numericsMap[ERR_CHANOPRIVSNEEDED], "<channel>", params[0]));
					else if (it->second->is_user_in_chan(kickUserIt->second) == false)
						message.send_numeric(" 441 ", findAndReplace(Message::numericsMap[ERR_USERNOTINCHANNEL], "<nick> <channel>", userNames[i] + " " + params[0]));
					else
					{
						messageParam = vectorToString(std::vector<std::string>(message.get_params().begin() + 2, message.get_params().end()));
						line = ":" + user->get_mask() + " KICK " + it->first + " " + kickUserIt->second->get_nick() + " :";	
						line += (messageParam.empty() ? user->get_nick() : messageParam) + "\r\n";
						it->second->send(line, -1);
						it->second->delete_user(kickUserIt->second);
					}
				}
				else
					message.send_numeric(" 442 ", findAndReplace(Message::numericsMap[ERR_NOTONCHANNEL], "<channel>", message.get_params()[0]));			
			}
			return ;
		}
	}

	return message.send_numeric(" 403 ", findAndReplace(Message::numericsMap[ERR_NOSUCHCHANNEL], "<channel name>", params[0]));
}
