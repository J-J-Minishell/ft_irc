#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <sys/socket.h>

std::string		extractWord(std::string& data);
void			leftTrim(std::string& str);
std::string		to_string(long nb);
int				send_all(int fd, const void* buffer, size_t length);
std::string		to_string(long nb);
std::string		strToUpper(std::string str);
void			cleanSignalsFromStr(std::string &str);

#endif