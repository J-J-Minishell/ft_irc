#include "Server.hpp"

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>

#include <cstdio>


Server*	Server::_instance = NULL;

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

Server::Server(const char *port) : _port(port), _numPollfds(1)
{
	memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_getAddrinfoStruct();
	this->_prepareSocket();
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
	if (listen(this->_fd, MAXUSERS) < 0)
	{
		std::cerr << "Error, listen() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_pollfds[0].fd = this->_fd;
	this->_pollfds[0].events = POLLIN;

	int	numEvents;
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
		std::cout << "Cliente conectado con fd " << new_fd << std::endl;
	}
}

int		tmpManageInputs(int i, pollfd *pollfds, int numPollfds)
{
	int bytes_read;
	char buffer[255 + 1];
	std::string text;
	std::string printLine;
	int exitCode = 0;

	printLine = "usuario con fd ";
	snprintf(buffer, sizeof(buffer), "%d", pollfds[i].fd);
	printLine += buffer;

	bytes_read = recv(pollfds[i].fd, buffer, sizeof(buffer), 0);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		text = buffer;
		std::cout << text << std::endl;

		if (text.substr(0, text.find("\r")).compare("exit") != 0)
		{
			printLine += " dice: ";
			printLine += text;
		}
	}
	if (bytes_read == 0 || (text.substr(0, text.find("\r")).compare("exit") == 0))
	{
		std::cout << "Cliente desconectado con fd " << pollfds[i].fd << std::endl;
		printLine += " ha cerrado la sesión.\n";
		exitCode = 1;
	}
	for (int j = 1; j < numPollfds; j++)
	{
		if (i != j)
			send(pollfds[j].fd, printLine.c_str(), printLine.size(), 0);
	}
	return exitCode;
}

void	Server::_checkInputs(void)
{
	for (int i = 1; i < this->_numPollfds; i++)
	{
		if (this->_pollfds[i].revents & POLLIN)
		{
			if (tmpManageInputs(i, this->_pollfds, _numPollfds))
			{
				close(this->_pollfds[i].fd);
				this->_pollfds[i].fd = this->_pollfds[i -1].fd;
				this->_numPollfds--;
			}
		}
	}
}
