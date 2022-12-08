#ifndef USER_HPP
# define USER_HPP

# include "main.hpp"

class User
{
	public:
		User(int fd, Server& server);
		~User();

		int			get_fd() { return this->_fd; };
		std::string	get_bufferLine() { return this->_bufferLine; };

		int		recv_line(int fd);
		void	send_line(UserMap &usersList);

		int		manage_IO();

	private:
		User(void);
		User(const User& src);
		User&	operator=(const User& rhs);

		int		send_all(int fd, const void* buffer, size_t length);

		int				_fd;
		Server &		_server;
		std::string		_nick;
		std::string		_bufferLine;
};

#endif