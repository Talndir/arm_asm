#include "../io/io.h"

/*
	matchlines.cpp

	CPP file for MatchLines function.
*/

// Turns memory locations into line numbers - used later to look up ASM line numbers for a certain PC value
void MatchLines(std::string& str, std::map<int, int>& data)
{
	std::istringstream ss(str);
	std::string line;
	int linenumber = 0;
	int memloc = 0x1000;	// PC offset, code starts at 0x1000

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