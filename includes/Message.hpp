#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "main.hpp"

class Message
{
	public:
		Message(Server& server, User& user);
		~Message();

		void	set_message(std::string line);

		Server &	get_server() { return this->_server; };
		User &		get_user() { return this->_user; };

		std::string					get_cmd() { return this->_cmd; };
		std::vector<std::string>	get_params() { return this->_params; };

		void		set_lineToSend(std::string line) { this->_lineToSend = line; };

	private:
		void		_send(std::vector<User *> userVector);

		Server &					_server;
		User &						_user;
		std::string					_lineToSend;
		std::string					_cmd;
		std::vector<std::string>	_params;

};

#endif