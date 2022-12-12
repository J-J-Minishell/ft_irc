#include "Message.hpp"

Message::Message(Server& server, User& user) : _server(server), _user(user)
{
	set_message(_user.get_bufferLine());

	cmdMap::iterator it = this->_server.getCmdMap().find(this->_cmd);
	if (it != this->_server.getCmdMap().end())
		it->second(*this); //llama a la funcion especifica del comando _cmd
	else
		this->_lineToSend = MASK_BLUE + this->_user.get_nick() + RESET_COLOR + ": " + this->_user.get_bufferLine();

	//tmp
	std::vector<User *> userVector;
	for (UserMapIterator it = this->_server.getUserMap().begin(); it != this->_server.getUserMap().end(); it++)
		userVector.push_back(it->second);
	this->_send(userVector);
}

Message::~Message()
{}

void	Message::set_message(std::string line)
{
	std::string	param;

	this->_cmd = extractWord(line);

	while (!line.empty())
	{
		param = extractWord(line);
		param = param.substr(0, param.find("\n"));
		this->_params.push_back(param);
	}
}

void	Message::_send(std::vector<User *> userVector)
{
	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
	{
		if (this->_user.get_fd() != (*it)->get_fd())
			send_all((*it)->get_fd(), this->_lineToSend.c_str(), this->_lineToSend.size());
	}
	this->_user.clear_bufferLine();
}
