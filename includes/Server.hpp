#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

class Server
{
	public:
		static Server*		createInstance(const char *port, std::string passwd);
		static void			deleteInstance(void);
		void				run(void);

		UserMap	&			getUserMap() { return this->_usersMap; };
		cmdMap	&			getCmdMap() { return this->_cmdMap; };
		std::string			getServerName() const { return this->_serverName; };
		std::string			getPassword() const {return this->_password; };
		int					findPollindex(User &user);
		void				quitUser(User *user);
		pairAddChannel		addChannel(Channel *newChannel);
		channelsMap &		getChannelsMap() { return this->_channelsMap; };
		channelUsersMap *	getChannelUsersMap(std::string channelName);
		void				setPOLLOUT(User *user);
		short				getPollEvents(User *user) { return this->_pollfds[findPollindex(*user)].events; };

	private:
		Server(const char *port, std::string password);
		~Server(void);

		static Server *			_instance;
		int						_fd;
		const char *			_port;
		struct addrinfo *		_server_info;
		struct sockaddr_in *	_sockaddr_in;
		struct pollfd			_pollfds[MAXUSERS + 2];
		int						_numPollfds;
		std::string				_serverName;
		std::string				_password;
		bool					_run;

		UserMap					_usersMap;
		cmdMap					_cmdMap;
		channelsMap				_channelsMap;

		void	_prepareSocket(void);
		void	_getAddrinfoStruct(void);
		void	_listen(void);
		void	_fillCmdMap(void);
		void	_checkConnection(void);
		void	_checkInputs(void);
		void	_serverInput(void);
		void	_checkCommand(std::string line);
		void	_relocate_poll(int i);
		int		_checkTime(User *user);
};

#endif