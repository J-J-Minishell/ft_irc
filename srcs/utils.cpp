#include <utils.hpp>

std::string	extractWord(std::string& data)
{
	std::size_t	pos;
	std::string	newStr;

	pos = data.find(' ');
	if (pos != std::string::npos)
	{
		newStr = data.substr(0, pos);
		data.erase(0, pos + 1);
		return newStr;
	}
	newStr = data;
	data.clear();
	return newStr;
}

int		send_all(int fd, const void* buffer, size_t length)
{
	const char*	ptr;
	int			num_bytes;

	ptr = (const char*) buffer;
	while (length > 0)
	{
		num_bytes = send(fd, ptr, length, 0);
		if (num_bytes < 0)
		{
			std::cerr << "Error, function send() failed" << std::endl;
			return -1;
		}
		ptr += num_bytes;
		length -= num_bytes;
	}
	return 0;
}

void	leftTrim(std::string& str)
{
	size_t i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	str.erase(0, i);
}

std::string	strToUpper(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] >= 'a' && str[i] <= '~')
			str[i] -= 32;
	return str;
}

void	cleanSignalsFromStr(std::string &str)
{
	size_t		pos;

	while ((pos = str.find(4)) != std::string::npos)
		str.erase(pos);
}

std::string	to_string(long nb)
{
	std::ostringstream	oss;

	oss << nb;
	return oss.str();
}
