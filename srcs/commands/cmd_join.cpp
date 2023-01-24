#include "main.hpp"

int		cmd_join(Message &message)
{
	User *			user = message.get_user();
	Channel *		newChannel;
	std::string		channelName;
	pairAddChannel	pair;

	if (message.get_params().empty())
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "JOIN"));
	channelName = message.get_params()[0];
	if (channelName.size() < 2 || channelName.size() > CHANNELNAMELEN || channelName[0] != '#' || channelName.find_first_of(" ,\x07") != std::string::npos)
		return message.send_numeric(" 476 ", Message::numericsMap[ERR_BADCHANMASK]);

	if (user->get_channels().size() < MAXCHANNELS)
	{
		newChannel = new Channel(message.get_server(), user, channelName);
		pair = message.get_server().addChannel(newChannel);
		if (pair.second == false)
		{
			delete newChannel;
			if (pair.first->second->add_user(user) == false)
				return -1;
		}
		user->add_channel(pair.first->second);
	}
	else
		return message.send_numeric(" 405 ", findAndReplace(Message::numericsMap[ERR_TOOMANYCHANNELS], "<channel name>", channelName));

	pair.first->second->send(user->get_mask() + " JOIN :" + channelName + "\r\n", -1);
	cmd_names_inChannel(message, channelName);

	return -1;
}