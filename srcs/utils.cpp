#include <utils.hpp>

std::string	extractWord(std::string& data)
{
	std::size_t	pos;
	std::string	newStr;

	pos = data.find(' ');
	if (pos != std::string::npos)
	{
		newStr = pos == 0 ? data.substr(0, 1) : data.substr(0, pos);
		pos == 0 ? data.erase(0, 1) : data.erase(0, pos);
		return newStr;
	}
	newStr = data;
	data.clear();
	return newStr;
}

int		send_all(User *user, const char* buffer)
{
	const char*	ptr;
	int			num_bytes;
	size_t		length = 0;

	ptr = (const char*) buffer;
	while (buffer && buffer[length] != '\0')
		length++;
	while (length > 0)
	{
		num_bytes = send(user->get_fd(), ptr, length, 0);
		if (num_bytes < 0)
		{
			std::cerr << "Error, function send() failed" << std::endl;
			return -1;
		}
		if (num_bytes < (int)length)
		{
			user->get_server().setPOLLOUT(user);
			break;
		}
		ptr += num_bytes;
		length -= num_bytes;
	}
	return 0;
}

std::string	maxBuffer_trim(std::string &line, int maxBuffer)
{
	std::string	sortedLine;
	std::string nextWord;
	std::string	tmpLine = line;

	for (; !tmpLine.empty(); )
	{
		nextWord = extractWord(tmpLine);
		if ((int)(sortedLine.size() + nextWord.size()) < maxBuffer)
		{
			extractWord(line);
			sortedLine += nextWord;
		}
		else
			return sortedLine;
	}
	line.clear();
	return sortedLine;
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

std::string	vectorToString(std::vector<std::string> vector)
{
	std::vector<std::string>::iterator	it;
	std::string							line;

	for (it = vector.begin(); it != vector.end(); it++)
	{
		if (it == vector.begin())
			line = *it;
		else
			line += " " + *it;
	}
	return line;
}

std::vector<std::string> stringDelimiter(std::string str, std::string delimiter)
{
    std::vector<std::string> words;

    size_t pos = 0;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        words.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
	if (!str.empty())
        words.push_back(str);
	return words;
}

std::string	findAndReplace(std::string str, std::string findStr, std::string replaceStr)
{
	return str.replace(str.find(findStr), findStr.size(), replaceStr);
}

std::string	to_string(long nb)
{
	std::ostringstream	oss;

	oss << nb;
	return oss.str();
}
