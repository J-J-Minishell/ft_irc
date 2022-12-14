#include "User.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

User::User(int fd, Server& server) : _fd(fd), _server(server), _nick("*"), _username("unknown")
{
	this->_registered = false;
	this->_password = false;
	this->_registTime = time(NULL);
	this->_timeout = 0;
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
	if (this->_bufferLine.size() > 5000)
		bytesRecieved = -1;
	if (bytesRecieved > 0)
	{
		buffer[bytesRecieved] = '\0';
		this->_bufferLine += buffer;
		leftTrim(this->_bufferLine);
		cleanSignalsFromStr(this->_bufferLine);
		if (this->_bufferLine == "\r\n")
			this->_bufferLine.clear();
		return bytesRecieved;
	}
	else if (bytesRecieved < 0)
	{
		std::cerr << "Error, function recv() failed" << std::endl;
		send_all(this->_fd, "Error, function recv() failed, leaving the server.");
		return -1;
	}
	return 0;
}

void	User::limit_bufferLine()
{
	std::string newLine;

	newLine = this->_bufferLine.substr(0, 510);
	if (newLine.size() == 510)
		newLine.append("\r\n");
	this->_bufferLine = newLine;
}

void	User::setHost(const std::string& value)
{
	this->_host = value;
	this->updateMask();
}

void	User::updateMask(void)
{
	this->_mask = this->_nick + "!" + this->_username + "@" + this->_host;
}

const std::string User::get_coloredMask(void)
{
	std::string	coloredMask;

	coloredMask = MASK_BLUE + this->_mask + RESET_COLOR + ":";
	return coloredMask;
}

std::ostream &operator<<(std::ostream &out, User &rhs)
{
    out << rhs.get_coloredMask();
    return (out);
}
