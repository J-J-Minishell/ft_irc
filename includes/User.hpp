#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

class User
{
	public:
		User(int fd, Server& server);
		~User();

		int				get_fd() const { return this->_fd; };
		std::string &			get_bufferLine() { return this->_bufferLine; };
		void					clear_bufferLine() { this->_bufferLine.clear(); };
		const std::string		get_nick() const { return this->_nick; };
		void					set_nick(std::string newNick) {	this->_nick = newNick; };
		const std::string		get_username() const { return this->_username; };
		void					set_username(std::string newUsername) { this->_username = newUsername; };
		const std::string		get_host() const { return this->_username; };

		void				updateMask(void);
		const std::string&	get_mask(void) const { return this->_mask; };

		void	setHost(const std::string& value);

		int		recv_line();

	private:
		User(void);
		User(const User& src);
		User&	operator=(const User& rhs);

		int				_fd;
		Server &		_server;
		std::string		_mask;
		std::string		_nick;
		std::string		_username;
		std::string		_host;
		std::string		_bufferLine;
};

std::ostream	&operator<<(std::ostream &out, User &rhs);

#endif