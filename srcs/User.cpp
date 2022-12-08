#include "User.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

User::User(int fd, Server& server) : _fd(fd), _server(server)
{}

User::~User()
{
	close(this->_fd);
}

int		User::recv_line(int fd)
{
	char	buffer[MAXBUFFER + 1];
	int		bytesRecieved;

	bytesRecieved = recv(fd, buffer, MAXBUFFER, 0);
	if (bytesRecieved > 0)
	{
		buffer[bytesRecieved] = '\0';
		this->_bufferLine = buffer;
		leftTrim(this->_bufferLine);
	}
	return bytesRecieved;
}

int	User::send_all(int fd, const void* buffer, size_t length)
{
	const char*	ptr;
	int			num_bytes;

	ptr = (const char*) buffer;
	while (length > 0)
	{
		num_bytes = send(fd, ptr, length, 0);
		if (num_bytes < 0)
		{
			std::cerr << "Error, function send() failed" << std::endl;
			return -1;
		}
		ptr += num_bytes;
		length -= num_bytes;
	}
	return 0;
}

void	User::send_line(UserMap &usersList)
{
	for (UserMapIterator it = usersList.begin(); it != usersList.end(); it++)
	{
		if (this->_fd != it->second->_fd)
			send_all(it->second->_fd, this->_bufferLine.c_str(), this->_bufferLine.length());
	}
	this->_bufferLine.clear();
}


// Que clase debería gestionar los IO, Server o User??? 
int		User::manage_IO()
{
	int	bytesRecieved;

	bytesRecieved = recv_line(this->_fd);
	if (bytesRecieved < 0)
	{
		std::cerr << "Error, function recv() failed" << std::endl;
		return -1;
	}
	else if (bytesRecieved > 0)
	{
		send_line(_server.getUserMap());
	//	return 0;
	}
	return 1;
}