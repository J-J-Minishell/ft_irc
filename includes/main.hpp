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
# define MAXBUFFER		255

# define MAXNICKLENGTH	9

# define RESET_COLOR	"\033[0m"
# define MASK_BLUE		"\033[1;34m"

# define SERVER_VERSION "irc-0.1"

class Server;
class User;
class Message;

	typedef std::map<int, User*>		UserMap;
	typedef UserMap::iterator			UserMapIterator;
	typedef std::map<std::string, int (*)(Message &)>		cmdMap;


#include "Server.hpp"
#include "User.hpp"
#include "numerics.hpp"
#include "Message.hpp"
#include "utils.hpp"

int				nick_cmd(Message &message);
int				user_cmd(Message &message);


#endif