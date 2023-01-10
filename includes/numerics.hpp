#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# include "main.hpp"

enum
{
	RPL_WELCOME				=	1,		// "Welcome to the Internet Relay Network <nick>!<user>@<host>"
	RPL_YOURHOST			=	2,		// "Your host is <servername>, running version <ver>"
	RPL_CREATED				=	3,		// "This server was created <date>"
	RPL_MYINFO				=	4,		// "<servername> <version> <available user modes> <available channel modes>"
//	RPL_ISUPPORT			=	5,		// "$ :are supported by this server"

	ERR_NOSUCHNICK			=	401,	// "<nickname> :No such nick/channel"
	ERR_NOMOTD				=	422,	// ":MOTD File is missing"
	ERR_NEEDMOREPARAMS		=	461,	// "<command> :Not enough parameters"
	ERR_ALREADYREGISTRED	=	462,	// ":Unauthorized command (already registered)"

};

#endif
