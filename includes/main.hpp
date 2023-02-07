#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# include <poll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>


# define MAXUSERS		10
# define MAXBUFFER		510
# define MAXCHANNELS	5
# define MAXNICKLENGTH	9
# define CHANNELNAMELEN	50

# define REGTIMEOUT		300
# define PINGTIMEOUT	90
# define TIMEOUT		45

# define PONG_STR		"server"

# define RESET_COLOR	"\033[0m"
# define MASK_BLUE		"\033[1;34m"
# define INFO_GREEN		"\033[1;32m"
# define ERR_RED		"\033[1;31m"
# define CHANNEL_YELLOW	"\033[1;33m"

# define SERVER_VERSION "irc-0.1"

# define OPERPASSWORD		"1234"
# define OPERLOGIN			"operator"

# define SPECIALCHAR    "-[]\\`^{}"

class Server;
class User;
class Message;
class Channel;

	typedef std::map<int, User*>							UserMap;
	typedef UserMap::iterator								UserMapIterator;
	typedef std::map<std::string, void (*)(Message &)>		cmdMap;
	typedef std::map<std::string, Channel*>					channelsMap;
	typedef	std::map<User*, int>							channelUsersMap;
	typedef std::pair<channelsMap::iterator, bool>			pairAddChannel;



#include "Server.hpp"
#include "User.hpp"
#include "numerics.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "utils.hpp"

void			cmd_pass(Message &message);
void			cmd_nick(Message &message);
void			cmd_user(Message &message);
void			cmd_ping(Message &message);
void			cmd_pong(Message &message);
void			cmd_quit(Message &message);
void			cmd_privmsg(Message &message);
void			cmd_join(Message &message);
void			cmd_list(Message &message);
void			cmd_names(Message &message);
void			cmd_names_inChannel(Message &message, std::string channelName);
void			cmd_part(Message &message);
void			cmd_kick(Message &message);
void			cmd_oper(Message &message);
void			cmd_die(Message &message);

#endif