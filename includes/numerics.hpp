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
	
	RPL_LISTEND				=	323,	// ":End of LIST"
	RPL_ENDOFNAMES			=	366,	// "<channel> :End of NAMES list"
	ERR_NOSUCHNICK			=	401,	// "<nickname> :No such nick/channel"
	ERR_NOSUCHCHANNEL		=	403,	// "<channel name> :No such channel"
	ERR_CANNOTSENDTOCHAN	=	404,	// "<channel name> :Cannot send to channel"
	ERR_TOOMANYCHANNELS		=	405,	// "<channel name> :You have joined too many channels"
	ERR_UNKNOWNCOMMAND		=	421,	// "<command> :Unknown command"
	ERR_NOMOTD				=	422,	// ":MOTD File is missing"
	ERR_NONICKNAMEGIVEN		=	431,	// ":No nickname given"
	ERR_ERRONEUSNICKNAME	=	432,	// "<nick> :Erroneous nickname"
	ERR_NICKNAMEINUSE		=	433,	// "<nick> :Nickname is already in use"
	ERR_NOTONCHANNEL		=	442,	// "<channel> :You're not on that channel"
	ERR_NEEDMOREPARAMS		=	461,	// "<command> :Not enough parameters"
	ERR_ALREADYREGISTRED	=	462,	// ":Unauthorized command (already registered)"
	ERR_BADCHANMASK			=	476,	// ":Bad Channel Mask"

};

#endif
