#include "Server.hpp"

Server*	Server::_instance = NULL;

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

Server::Server(const char *port, std::string password) : _port(port), _numPollfds(2), _serverName("irc-server"), _run(false)
{
	this->_password = password;
	memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_getAddrinfoStruct();
	this->_prepareSocket();

	this->_fillCmdMap();
}

Server::~Server(void)
{}

// ------------------ CREATE / DELETE INSTANCE ------------------

Server*	Server::createInstance(const char *port, std::string passwd)
{
	if (Server::_instance == NULL)
		Server::_instance = new Server(port, passwd);
	return Server::_instance;
}

void	Server::deleteInstance(void)
{
	if (Server::_instance != NULL)
		delete Server::_instance;
	Server::_instance = NULL;
}

void	Server::shutdown()
{
	for (UserMapIterator it = this->_usersMap.begin(); it != this->_usersMap.end(); it++)
	{
		send_all(it->second, "**** SERVER SHUTDOWN ****\r\n");
		delete it->second;
	}
	this->_usersMap.clear();
	freeaddrinfo(this->_server_info);
	this->_run = false;
	std::cout << "************   " ERR_RED "SHUTDOWN" RESET_COLOR "   ************\n"<< std::endl;
}

// ---------------------- RUN (MAIN LOOP) ----------------------

void	Server::run(void)
{
	int	numEvents;

	this->_listen();
	this->_run = true;
	while (this->_run)
	{
		numEvents = poll(this->_pollfds, MAXUSERS + 2, 1000);
		if (numEvents == -1)
		{
			std::cerr << "Error, poll() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (numEvents > 0)
		{
			this->_checkConnection();
			this->_checkInputs();
		}
		for (UserMapIterator it = this->_usersMap.begin(); it != this->_usersMap.end(); )
			this->_checkTime(it->second)? it = this->_usersMap.begin() : it++;
		if (this->_pollfds[1].revents & POLLIN)
			this->_serverInput();
	}
}

int		Server::findPollindex(User &user)
{
	for (int i = 0; i < this->_numPollfds; i++)
		if (this->_pollfds[i].fd == user.get_fd())
			return i;
	return -1;
}

void	Server::quitUser(User *user)
{
	std::cout << *user << " is leaving." << std::endl;
	this->_relocate_poll(findPollindex(*user));
	this->_numPollfds--;
	this->_usersMap.erase(user->get_fd());
	delete user;
}

pairAddChannel	Server::addChannel(Channel *newChannel)
{
	return this->_channelsMap.insert(std::make_pair(newChannel->get_name(), newChannel));
}

channelUsersMap	*	Server::getChannelUsersMap(std::string channelName)
{
	for (channelsMap::iterator it = this->_channelsMap.begin(); it != this->_channelsMap.end(); it++)
	{
		if (it->first == channelName)
			return &it->second->get_usersMap();
	}
	return NULL;
}

void	Server::setPOLLOUT(User *user)
{
	this->_pollfds[this->findPollindex(*user)].events = POLLOUT;
}

/*  /////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////  PRIVATE METHODS  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/////////////////////////////////////////////// */

void	Server::_fillCmdMap()
{
	_cmdMap["PASS"] = &cmd_pass;
	_cmdMap["NICK"] = &cmd_nick;
	_cmdMap["USER"] = &cmd_user;
	_cmdMap["PING"] = &cmd_ping;
	_cmdMap["PONG"] = &cmd_pong;
	_cmdMap["QUIT"] = &cmd_quit;
	_cmdMap["PRIVMSG"] = &cmd_privmsg;
	_cmdMap["NOTICE"] = &cmd_privmsg;
	_cmdMap["JOIN"] = &cmd_join;
	_cmdMap["LIST"] = &cmd_list;
	_cmdMap["NAMES"] = &cmd_names;
	_cmdMap["PART"] = &cmd_part;
	_cmdMap["KICK"] = &cmd_kick;
	_cmdMap["OPER"] = &cmd_oper;
	_cmdMap["DIE"] = &cmd_die;
}

void	Server::_getAddrinfoStruct()
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, this->_port, &hints, &this->_server_info) != 0)
	{
		std::cerr << "Error, getaddrinfo() failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	this->_sockaddr_in = reinterpret_cast<struct sockaddr_in*>(this->_server_info->ai_addr);
}

void	Server::_prepareSocket()
{
    this->_fd = socket(this->_server_info->ai_family, this->_server_info->ai_socktype, this->_server_info->ai_protocol);
	if (!this->_fd)
	{
		std::cerr << "Error, socket() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error, fcntl() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		std::cerr << "Error, setsockopt() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(this->_fd, this->_server_info->ai_addr, this->_server_info->ai_addrlen) < 0)
	{
		std::cerr << "Error, bind() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Server::_listen()
{
	if (listen(this->_fd, MAXUSERS) < 0)
	{
		std::cerr << "Error, listen() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_pollfds[0].fd = this->_fd;
	this->_pollfds[0].events = POLLIN;
	this->_pollfds[1].fd = 0;
	this->_pollfds[1].events = POLLIN;
	std::cout << "Waiting for connections in " << inet_ntoa(this->_sockaddr_in->sin_addr) << ":" << htons(this->_sockaddr_in->sin_port) << std::endl;
}

void	Server::_checkConnection(void)
{
	int					new_fd;

	if (this->_pollfds[0].revents & POLLIN)
	{
		new_fd = accept(this->_fd, this->_server_info->ai_addr, &this->_server_info->ai_addrlen);
		if (new_fd < 0)
		{
			std::cerr << "Error, accept() failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		else if(this->_numPollfds >= MAXUSERS + 2)
		{
			User tmpUser(new_fd, *this);
			send_all(&tmpUser, "The server is full. Please, try again later\r\n");
			std::cerr << "User tried to connect but server is full" << std::endl;
			close(new_fd);
			return ;
		}
		this->_pollfds[this->_numPollfds].fd = new_fd;
		this->_pollfds[this->_numPollfds].events = POLLIN;
		this->_numPollfds++;
		this->_usersMap.insert(std::make_pair(new_fd, new User(new_fd, *this)));
		this->_usersMap.find(new_fd)->second->setHost(inet_ntoa(this->_sockaddr_in->sin_addr));
		std::cout << "Cliente conectado " << *(this->_usersMap.find(new_fd)->second) << std::endl;
	}
}

void	Server::_serverInput(void)
{
	std::string		buffer;

	std::getline(std::cin, buffer);
	if (std::cin && strToUpper(buffer) == "SHUTDOWN")
		this->shutdown();
}

void	Server::_checkInputs(void)
{
	User *		userTalking;
	Message*	message;

	for (int i = 2; i < this->_numPollfds; i++)
	{
		if (this->_pollfds[i].revents & POLLIN)
		{
			userTalking = _usersMap.find(_pollfds[i].fd)->second;
			if (userTalking->recv_line() > 0)
			{
				if (!userTalking->get_bufferLine().empty() && 
					userTalking->get_bufferLine().find("\r\n") != std::string::npos)
				{
					userTalking->limit_bufferLine();
					std::cout << *userTalking << userTalking->get_bufferLine() << std::endl;
					message = new Message(*this, userTalking);
					delete message;
				}
			}
			else
				this->quitUser(userTalking);
		}
	}
}

int	Server::_checkTime(User *user)
{
	std::vector<User *>	userVector;
	std::string			line;

	if (user->isRegistered())
	{
		if (!user->get_timeout() && (user->get_time() + PINGTIMEOUT < time(NULL)))
		{
			user->set_timeout(time(NULL) + TIMEOUT);
			send_all(user, "PING irc-serv\r\n");
		}
		else if (user->get_timeout() && user->get_timeout() < time(NULL))
		{
			line = "ERROR :Closing link: (" + user->get_username() + "@" + user->get_host() + ") [Ping timeout]\r\n";
			send_all(user, line.c_str());
			line = ":" + user->get_mask() + " QUIT :Ping timeout\r\n";
			userVector = user->get_users_from_channels();
			for (size_t i = 0; i < userVector.size(); i++)
				if (userVector[i] != user)
					send_all(userVector[i], line.c_str());
			this->quitUser(user);
			return 1;
		}
	}
	else
	{
		if ((user->get_registTime() + REGTIMEOUT) <= time(NULL))
		{
			send_all(user, "PONG ERROR [Registration timeout]\r\n");
			this->quitUser(user);
			return 1;
		}
	}
	return 0;
}

void	Server::_relocate_poll(int i)
{
	if (i < (this->_numPollfds - 1))
	{
		this->_pollfds[i].fd = this->_pollfds[i + 1].fd;
		this->_relocate_poll(++i);
	}
	else
		this->_pollfds[i].fd = -1;
}
