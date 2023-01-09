#include "Message.hpp"

std::map<int, std::string>	Message::numericsMap;
std::string					Message::prefix;

Message::Message(Server &server, User &user) : _server(server), _user(user)
{
	if (numericsMap.empty())
		this->_initStaticVars();
	set_message(_user.get_bufferLine());

	cmdMap::iterator it = this->_server.getCmdMap().find(strToUpper(this->_cmd));
	if (it != this->_server.getCmdMap().end())
	{
		if (it->second(*this) == -1) // llama a la funcion especifica del comando _cmd
			return;
	}
	else
		this->_lineToSend = MASK_BLUE + this->_user.get_nick() + RESET_COLOR + ": " + this->_user.get_bufferLine();

	// tmp
	std::vector<User *> userVector;
	for (UserMapIterator it = this->_server.getUserMap().begin(); it != this->_server.getUserMap().end(); it++)
		userVector.push_back(it->second);
	this->_send(userVector);
}

Message::~Message()
{
	this->_user.clear_bufferLine();
}

void Message::set_message(std::string line)
{
	std::string param;

	this->_cmd = extractWord(line);
	this->_cmd = this->_cmd.substr(0, this->_cmd.find("\r\n"));

	while (!line.empty())
	{
		param = extractWord(line);
		param = param.substr(0, param.find("\r\n")); // para nc sólo "\n", resto (telnet) "\r\n"
		this->_params.push_back(param);
	}
}

int	Message::send_numeric(std::string numeric, std::string numericStr)
{
	std::string	line;

	line = prefix + numeric + this->_user.get_nick() + " " + numericStr + "\n";
	send_all(this->_user.get_fd(), line.c_str());

	return -1;
}

void	Message::welcome_user()
{
	std::string	line;

	this->_user.set_registered(true);
	std::cout << this->_user << INFO_GREEN " is registered" RESET_COLOR << std::endl;

	line = this->numericsMap[RPL_WELCOME];
	line.replace(line.find("<nick>!<user>@<host>"), 20, _user.get_mask());
	send_numeric(" 001 ", line);
	send_numeric(" 002 ", this->numericsMap[RPL_YOURHOST]);
	send_numeric(" 003 ", this->numericsMap[RPL_CREATED]);
	send_numeric(" 004 ", this->numericsMap[RPL_MYINFO]);
}

void Message::_send(std::vector<User *> userVector)
{
	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
	{
		if (this->_user.get_fd() != (*it)->get_fd())
			send_all((*it)->get_fd(), this->_lineToSend.c_str());
	}
}

void Message::_initStaticVars()
{
	prefix = ":" + _server.getServerName();

	numericsMap[RPL_WELCOME] = "Welcome to the Internet Relay Network <nick>!<user>@<host>";
	numericsMap[RPL_YOURHOST] = "Your host is " + this->_server.getServerName() + ", running version " SERVER_VERSION;
	numericsMap[RPL_CREATED] = "This server was created " __TIME__ " " __DATE__;
	numericsMap[RPL_MYINFO] = this->_server.getServerName() + " " SERVER_VERSION " <available user modes> <available channel modes>";

	numericsMap[ERR_NOSUCHNICK] = "<nickname> :No such nick/channel";
	numericsMap[ERR_NEEDMOREPARAMS] = "<command> :Not enough parameters";
	numericsMap[ERR_ALREADYREGISTRED] = ":Unauthorized command (already registered)";
}