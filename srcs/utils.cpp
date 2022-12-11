#include <utils.hpp>

std::string	extractWord(std::string& data)
{
	std::size_t	pos;
	std::string	newStr;

	pos = data.find(' ');
	if (pos != std::string::npos)
	{
		newStr = data.substr(0, pos);
		data.erase(0, pos);
		return newStr;
	}
	newStr = data;
	data.clear();
	return newStr;
}

void	leftTrim(std::string& str)
{
	size_t i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	str.erase(0, i);
}

std::string	to_string(long nb)
{
	std::ostringstream	oss;

	oss << nb;
	return oss.str();
}
