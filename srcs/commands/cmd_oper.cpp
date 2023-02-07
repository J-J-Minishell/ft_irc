#include "main.hpp"

void	cmd_oper(Message &message)
{
	std::vector<std::string>	&params = message.get_params();
	User						&user = *message.get_user();

	if (params.size() < 2)
		return message.send_numeric(" 461 ", findAndReplace(Message::numericsMap[ERR_NEEDMOREPARAMS], "<command>", "OPER")) ;

	if (params[0] != OPERLOGIN || params[1] != OPERPASSWORD)
		return message.send_numeric(" 464 ", Message::numericsMap[ERR_PASSWDMISMATCH]);

	if (!user.isOper())
	{
		message.get_user()->setOper();
		message.send_numeric(" 381 ", Message::numericsMap[RPL_YOUREOPER]);
		std::cout << user << " is now a server operator" << std::endl;
	}
}