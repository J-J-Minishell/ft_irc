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

int	Message::error(std::string numeric, std::string numericStr)
{
	std::string	line;

	line = prefix + numeric + this->_user.get_nick() + " " + numericStr + "\n";
	send_all(this->_user.get_fd(), line.c_str(), line.size());

	return -1;
}

void Message::_send(std::vector<User *> userVector)
{
	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
	{
		if (this->_user.get_fd() != (*it)->get_fd())
			send_all((*it)->get_fd(), this->_lineToSend.c_str(), this->_lineToSend.size());
	}
}

void Message::_initStaticVars()
{
	prefix = ":" + _server.getServerName();

	numericsMap[RPL_WELCOME] = "Welcome to the Internet Relay Network <nick>!<user>@<host>";
	numericsMap[RPL_YOURHOST] = "Your host is <servername>, running version " SERVER_VERSION;
	numericsMap[RPL_CREATED] = "This server was created " __TIME__ " " __DATE__;
	numericsMap[RPL_MYINFO] = "<servername> <version> <available user modes> <available channel modes>";

	numericsMap[ERR_NEEDMOREPARAMS] = "<command> :Not enough parameters";
	numericsMap[ERR_ALREADYREGISTRED] = ":Unauthorized command (already registered)";
}