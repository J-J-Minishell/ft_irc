#include "main.hpp"

void	cmd_oper(Message &message)
{
	std::vector<std::string>	&params = message.get_params();

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "OPER")) ;

	if (params[0] != OPERLOGIN || params[1] != OPERPASSWORD)
		return message.send_numeric(" 464 ", Message::numericsMap[ERR_PASSWDMISMATCH]);

	message.get_user()->setOper();
	message.send_numeric(" 381 ", Message::numericsMap[RPL_YOUREOPER]);
}