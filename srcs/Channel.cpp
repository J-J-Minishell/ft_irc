#include "Channel.hpp"

// ------------------ CONSTRUCTOR / DESTRUCTOR------------------

Channel::Channel(Server& server, User* creator, std::string name) : _server(server), _creator(creator), _channelName(name)
{
	this->_usersMap[_creator] = 2;

	(void)this->_server.getPassword(); // tmp para el flag -Wall unused variable
}

Channel::~Channel()
{}

bool	Channel::add_user(User *newUser)
{
	return this->_usersMap.insert(std::make_pair(newUser, 0)).second;
}

void	Channel::send(std::string str)
{
	for (channelUsersMap::iterator it = this->_usersMap.begin(); it != this->_usersMap.end(); it++)
	{
		send_all(it->first->get_fd(), str.c_str());
	}
}