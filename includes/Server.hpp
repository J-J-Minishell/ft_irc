#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>

# define MAXUSERS			10

class Server
{
	public:
		static Server*		createInstance(const char *port);
		static void			deleteInstance(void);
		void				run(void);

	private:
		Server(const char *port);
		~Server(void);

		static Server *		_instance;
		int					_fd;
		const char *		_port;
		struct addrinfo *	_server_info;
		struct pollfd		_pollfds[MAXUSERS + 2];
		int					_numPollfds;

		void	_prepareSocket(void);
		void	_getAddrinfoStruct(void);
		void	_checkConnection(void);
		void	_checkInputs(void);
};

#endif