#include "main.hpp"

void	cmd_join(Message &message)
{
	User *						user = message.get_user();
	Channel *					newChannel;
	std::vector<std::string>	channelNames;
	pairAddChannel				pair;

	if (message.get_params().empty())
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "JOIN"));
	if (message.get_params()[0][0] == '0')
		return cmd_part(message);
	channelNames = stringDelimiter(message.get_params()[0], ",");
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		if (channelNames[i].size() < 2 || channelNames[i].size() > CHANNELNAMELEN || channelNames[i][0] != '#' || channelNames[i].find_first_of(" ,\x07") != std::string::npos)
			message.send_numeric(" 476 ", Message::numericsMap[ERR_BADCHANMASK]);
		else if (user->get_channels().size() < MAXCHANNELS)
		{
			newChannel = new Channel(message.get_server(), user, channelNames[i]);
			pair = message.get_server().addChannel(newChannel);
			if (pair.second == false)
				delete newChannel;
			if (pair.second == true || pair.first->second->add_user(user) == true)
			{
				user->add_channel(pair.first->second);
				pair.first->second->send(":" + user->get_mask() + " JOIN :" + channelNames[i] + "\r\n", -1);
				cmd_names_inChannel(message, channelNames[i]);
			}
		}
		else if ((user->get_channels().size() >= MAXCHANNELS))
			message.send_numeric(" 405 ", findAndReplace(Message::numericsMap[ERR_TOOMANYCHANNELS], "<channel name>", channelNames[i]));
	}
}