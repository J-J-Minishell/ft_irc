#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include "User.hpp"

class User;

std::string		extractWord(std::string& data);
void			leftTrim(std::string& str);
std::string		to_string(long nb);
int				send_all(User *user, const char* buffer);
std::string		to_string(long nb);
std::string		strToUpper(std::string str);
void			cleanSignalsFromStr(std::string &str);
std::string		findAndReplace(std::string str, std::string findStr, std::string replaceStr);
std::string     vectorToString(std::vector<std::string> vector);

#endif