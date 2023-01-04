#include "User.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

User::User(int fd, Server& server) : _fd(fd), _server(server), _nick("unknown"), _username("unknown")
{}

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
		cleanSignalsFromStr(this->_bufferLine);
		if (this->_bufferLine == "\r\n") // para nc solo "\n", resto (telnet) "\r\n"
			this->_bufferLine.clear();
		return bytesRecieved;
	}
	else if (bytesRecieved < 0)
	{
		std::cerr << "Error, function recv() failed" << std::endl;
		send_all(this->_fd, "Error, function recv() failed, leaving the server.", 51);
		return -1;
	}
	return 0;
}

void	User::setHost(const std::string& value)
{
	this->_host = value;
	this->updateMask();
}

void	User::updateMask(void)
{
	this->_mask = MASK_BLUE + this->_nick + "!" + this->_username + "@" + this->_host + RESET_COLOR + ":";
}

std::ostream &operator<<(std::ostream &out, User &rhs)
{
    out << rhs.get_mask();
    return (out);
}
  