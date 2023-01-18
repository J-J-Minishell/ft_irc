#include "main.hpp"

	//que empiezan con '&' o '#') de
   //hasta 200 caracteres. Aparte del requisito de que el primer carácter
   //sea un '&' o un '#', la única restricción es que no puede contener
   //espacios en blanco (' '), control G (^G o ASCII 7), o una coma (','

int		cmd_join(Message &message)
{
	User *user = message.get_user();
	Server &server = message.get_server();
	Channel *newChannel;
	pairAddChannel pair;
	std::string line;
	std::string channelName = message.get_params()[0]; //hola adios

	if (channelName.empty())
		return message.send_numeric(" 461 ", Message::numericsMap[ERR_NEEDMOREPARAMS]);
	if (channelName.size() > 200 || (channelName[0] != '&' && channelName[0] != '#'))
		return message.send_numeric(" 476 ", Message::numericsMap[ERR_BADCHANMASK]);
	for (size_t i = 1; i < channelName.size(); i++)
		if (channelName[i] == ' ' || (int)channelName[i] == 7 || channelName[i] == ',')
			return message.send_numeric(" 476 ", Message::numericsMap[ERR_BADCHANMASK]);
	if (user->get_channels().size() < MAXCHANNELS)
	{
		newChannel = new Channel(server, user, message.get_params()[0]);
		pair = server.addChannel(newChannel);
		if (pair.second == false)
		{
			delete newChannel;
			if (pair.first->second->add_user(user) == false)
				return -1;
		}
		user->add_channel(pair.first->second);
	}
	else
		return message.send_numeric(" 405 ", findAndReplace(Message::numericsMap[ERR_TOOMANYCHANNELS], "<channel name>", message.get_params()[0]));

	line = user->get_mask() + " JOIN :" + pair.first->second->get_name() + "\n";
	pair.first->second->send(line, -1);

	cmd_names_inChannel(message, pair.first->second->get_name());

	return -1;
}