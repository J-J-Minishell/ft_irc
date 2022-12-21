#include "Server.hpp"

Server*	Server::_instance = NULL;

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

Server::Server(const char *port) : _port(port), _numPollfds(1)
{
	memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_getAddrinfoStruct();
	this->_prepareSocket();

	this->_fillCmdMap();
}

Server::~Server(void)
{}

// ------------------ CREATE / DELETE INSTANCE ------------------

Server*	Server::createInstance(const char *port)
{
	if (Server::_instance == NULL)
		Server::_instance = new Server(port);
	return Server::_instance;
}

void	Server::deleteInstance(void)
{
	if (Server::_instance != NULL)
		delete Server::_instance;
	Server::_instance = NULL;
}

// ---------------------- RUN (MAIN LOOP) ----------------------

void	Server::run(void)
{
	int	numEvents;

	if (listen(this->_fd, MAXUSERS) < 0)
	{
		std::cerr << "Error, listen() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_pollfds[0].fd = this->_fd;
	this->_pollfds[0].events = POLLIN;
	std::cout << "Waiting for connections in " << inet_ntoa(this->_sockaddr_in->sin_addr) << std::endl;
	while (1)
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

	}
}

/*  /////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////  PRIVATE METHODS  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/////////////////////////////////////////////// */

void	Server::_fillCmdMap()
{
	_cmdMap["NICK"] = &nick_cmd;
//	_cmdMap["EXIT"] = &server_cmd;
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
		else if(this->_numPollfds > MAXUSERS)
		{
			send(new_fd, "The server is full. Please, try again later.", 45, 0);
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

void	Server::_checkInputs(void)
{
	User *		userTalking;
	Message*	message;

	for (int i = 1; i < this->_numPollfds; i++)
	{
		if (this->_pollfds[i].revents & POLLIN)
		{
			userTalking = _usersMap.find(_pollfds[i].fd)->second;
			if (userTalking->recv_line() > 0)
			{
				if (!userTalking->get_bufferLine().empty() && 
					userTalking->get_bufferLine().find("\r\n") != std::string::npos) // para nc sólo "\n", resto (telnet) "\r\n"
				{
					std::cout << *userTalking << userTalking->get_bufferLine() << std::endl;
					message = new Message(*this, *userTalking);
					delete message;
				}
			}
			else
			{
				std::cout << *userTalking << " is leaving." << std::endl;
				this->_relocate_poll(i);
				this->_numPollfds--;
				_usersMap.erase(userTalking->get_fd());  
				delete userTalking;
			}
		}
	}
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