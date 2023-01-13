#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class Channel
{
	public:
		Channel(Server& server, User* creator, std::string name);
		~Channel();

		std::string		get_name() { return this->_channelName; };
		bool			add_user(User *newUser);
		bool			is_user_in_chan(User *user) { return _usersMap.find(user) != _usersMap.end(); };
		void			send(std::string str, int userFd);
		channelUsersMap	get_usersMap() { return this->_usersMap; };

	private:
		Server &			_server;
		User *				_creator;
		std::string			_channelName;
		channelUsersMap		_usersMap;

};


#endif