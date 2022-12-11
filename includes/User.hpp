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
		const std::string		get_nick() const { return this->_nick; };
		const std::string		get_username() const { return this->_username; };
		const std::string		get_host() const { return this->_username; };
		
		const std::string&	get_mask(void) const { return this->_mask; };

		void	setHost(const std::string& value);

		int		recv_line();
		void	send_line(UserMap &usersList);

		bool	make_message();

		int		manage_IO();

	private:
		User(void);
		User(const User& src);
		User&	operator=(const User& rhs);

		int		_send_all(int fd, const void* buffer, size_t length);

		void	_updateMask(void);

		int				_fd;
		Server &		_server;
		Message	*		_message;
		std::string		_mask;
		std::string		_nick;
		std::string		_username;
		std::string		_host;
		std::string		_bufferLine;
};

std::ostream	&operator<<(std::ostream &out, User &rhs);

#endif