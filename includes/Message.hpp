#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "main.hpp"

class Message
{
	public:
		Message(Server& server, User& user);
		~Message();

		void	set_message(std::string line);

		std::string					get_cmd() { return this->_cmd; };
		std::vector<std::string>	get_params() { return this->_params; };

	private:
		void		_recv();
		void		_send();

		Server &					_server;
		User &						_user;
		std::string					_cmd;
		std::vector<std::string>	_params;

};

#endif