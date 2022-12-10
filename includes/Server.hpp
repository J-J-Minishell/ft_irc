#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Server
{
	public:
		static Server*		createInstance(const char *port);
		static void			deleteInstance(void);
		void				run(void);

		UserMap	&			getUserMap() { return _usersMap; };

	private:
		Server(const char *port);
		~Server(void);

		static Server *			_instance;
		int						_fd;
		const char *			_port;
		struct addrinfo *		_server_info;
		struct sockaddr_in *	_sockaddr_in;
		struct pollfd			_pollfds[MAXUSERS + 2];
		int						_numPollfds;

		UserMap					_usersMap;

		void	_prepareSocket(void);
		void	_getAddrinfoStruct(void);
		void	_checkConnection(void);
		void	_checkInputs(void);
		void	_checkCommand(std::string line);
};

#endif