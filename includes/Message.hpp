#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "main.hpp"

class Message
{
	public:
		Message(Server& server, User* user);
		~Message();

		static	std::map<int, std::string>	numericsMap;
		static	std::string					prefix;

		void	set_message(std::string line);

		Server &	get_server() { return this->_server; };
		User *		get_user() { return this->_user; };

		std::string					get_cmd() { return this->_cmd; };
		std::vector<std::string>&	get_params() { return this->_params; };

		void		welcome_user();
		int			send_numeric(std::string numeric, std::string numericStr);
		void		delete_user() { this->_user = NULL; };
		void		send_message(std::vector<User *> userVector, std::string line);

	private:
		void		_initStaticVars();

		Server &					_server;
		User *						_user;
		std::string					_cmd;
		std::vector<std::string>	_params;

};

#endif