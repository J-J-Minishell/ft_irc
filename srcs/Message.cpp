#include "Message.hpp"

Message::Message(Server& server, User& user) : _server(server), _user(user)
{
//	set_message(_user.get_bufferLine());
//	this->_server.getCmdMap().at(this->_cmd)(this);
}

Message::~Message()
{}

void	Message::_recv()
{

}

void	Message::set_message(std::string line)
{
	this->_cmd = extractWord(line);

	while (!line.empty())
		this->_params.push_back(extractWord(line));
}

