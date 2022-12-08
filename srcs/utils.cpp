#include <utils.hpp>

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
