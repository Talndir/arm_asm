#include "../io/io.h"

bool BothSpaces(char a, char b)
{
	return (a == b) & (a == ' ');
}

void Read(std::string filename, std::vector<std::vector<std::string>>& data)
{
	std::ifstream file;
	file.open(filename);

	std::string line;
	std::vector<std::string> s;
	std::vector<std::string> d = { ", ", ",", " " };

	while (std::getline(file, line))
	{
		size_t pos = 0;

		while ((pos = line.find(",", pos)) != std::string::npos)
			line.replace(pos, 1, " ");

		line.erase(std::unique(line.begin(), line.end(), BothSpaces), line.end());
		std::transform(line.begin(), line.end(), line.begin(), ::toupper);

		std::string token;

		while ((pos = line.find(" ")) != std::string::npos)
		{
			token = line.substr(0, pos);
			line.erase(0, pos + 1);

			if (token.length() > 0)
				s.push_back(token);
		}

		if (line.length() > 0)
			s.push_back(line);

		data.push_back(s);
		s.clear();
	}
}