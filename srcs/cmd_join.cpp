#include "main.hpp"

int		cmd_join(Message &message)
{
	User *user = message.get_user();
	Server &server = message.get_server();
	Channel *newChannel;
	pairAddChannel pair;
	std::string line;

	newChannel = new Channel(server, user, message.get_params()[0]);
	pair = server.addChannel(newChannel);
	if (pair.second == false)
	{
		delete newChannel;
		if (pair.first->second->add_user(user) == false)
			return -1;
	}
	if (user->get_channels().size() < MAXCHANNELS)
		user->add_channel(pair.first->second);
	else
		return message.send_numeric(" 405 ", findAndReplace(Message::numericsMap[ERR_TOOMANYCHANNELS], "<channel name>", message.get_params()[0]));

	line = user->get_mask() + " JOIN :" + newChannel->get_name() + "\n";
	pair.first->second->send(line, -1);

	return -1;
}