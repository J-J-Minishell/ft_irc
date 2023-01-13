#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class Channel
{
	public:
		typedef	std::map<User*, int>			channelUsersMap;

		Channel(Server& server, User* creator, std::string name);
		~Channel();

		std::string		get_name() { return this->_channelName; };
		bool			add_user(User *newUser);
		void			send(std::string str);

	private:
		Server &			_server;
		User *				_creator;
		std::string			_channelName;
		channelUsersMap		_usersMap;

};


#endif