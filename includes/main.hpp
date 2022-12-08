#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <string>
# include <map>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# include <poll.h>
# include <netinet/in.h>
# include <netdb.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/socket.h>


# define MAXUSERS			10
# define MAXBUFFER			255

class Server;
class User;

	typedef std::map<int, User*>		UserMap;
	typedef UserMap::iterator			UserMapIterator;

#include "Server.hpp"
#include "User.hpp"
#include "utils.hpp"

std::string	to_string(long nb);


#endif