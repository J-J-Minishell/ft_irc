#include "User.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

User::User(int fd, Server& server) : _fd(fd), _server(server), _username("unknown")
{
	this->_message = new Message(_server, *this);
}

User::~User()
{
	close(this->_fd);
}

int		User::recv_line()
{
	char	buffer[MAXBUFFER + 1];
	int		bytesRecieved;

	bytesRecieved = recv(this->_fd, buffer, MAXBUFFER, 0);
	if (bytesRecieved > 0)
	{
		buffer[bytesRecieved] = '\0';
		this->_bufferLine += buffer;
		leftTrim(this->_bufferLine);
		if (this->_bufferLine == "\r\n")
			this->_bufferLine.clear();
		return bytesRecieved;
	}
	else if (bytesRecieved < 0)
	{
		std::cerr << "Error, function recv() failed" << std::endl;
		_send_all(this->_fd, "Error, function recv() failed, leaving the server.", 51);
		return -1;
	}
	return 0;
}

int	User::_send_all(int fd, const void* buffer, size_t length)
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
			_send_all(it->second->_fd, this->_bufferLine.c_str(), this->_bufferLine.length());
	}
	this->_bufferLine.clear();
}


// Que clase debería gestionar los IO, Server o User??? 
int		User::manage_IO()
{
	int	bytesRecieved;

	bytesRecieved = recv_line();
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

void	User::setHost(const std::string& value)
{
	this->_host = value;
	this->_updateMask();
}

void	User::_updateMask(void)
{
	this->_mask = MASK_BLUE + this->_nick + "!" + this->_username + "@" + this->_host + RESET_COLOR + ":";
}

std::ostream &operator<<(std::ostream &out, User &rhs)
{
    out << rhs.get_mask();
    return (out);
}
  