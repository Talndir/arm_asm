#include "../io/io.h"

void MatchLines(std::string& str, std::map<int, int>& data)
{
	std::istringstream ss(str);
	std::string line;
	int linenumber = 0;
	int memloc = 0x1000;

	while (std::getline(ss, line))
	{
		if ((line != "") && (line.at(0) != ':'))
		{
			data.insert(std::make_pair(memloc, linenumber));
			memloc += 4;
		}

		++linenumber;
	}
}