#include "../io/io.h"

/*
	rad.cpp

	CPP file for Read functions.
*/

// Checks if both inputs are spaces - needed later
bool BothSpaces(char a, char b)
{
	return (a == b) & (a == ' ');
}

// Reads code from file
// This used to be the code for the function below it, but the text box gives me a string so I wrote a string version, which this now calls.
// Pretty redundant actually, since I never use this function anymore after wxWidgets took over file loading. it should be stripped out by the compiler.
void ReadFromFile(std::string filename, std::vector<std::vector<std::string>>& data)
{
	std::ifstream file;
	file.open(filename);
	std::string s(static_cast<std::stringstream const&>(std::stringstream() << file.rdbuf()).str());

	/*
	Note on the file->string conversion:
	This is NOT efficient. Unfortunately there's no really fast way in C++, also it does a double copy which is bad.
	Can't use memcpy, it's not in RAM yet. So I'm stuck with this code.
	*/

	ReadFromString(s, data);
}

// Reads string, returns 2D vector of 'words'
void ReadFromString(std::string& str, std::vector<std::vector<std::string>>& data)
{
	std::istringstream ss(str);

	std::string line;
	std::vector<std::string> s;
	std::vector<std::string> d = { ", ", ",", " " };

	// Iterate over each line
	while (std::getline(ss, line))
	{
		if (line != "")		// Skip empty lines
		{
			size_t pos = 0;

			while ((pos = line.find(",", pos)) != std::string::npos)	// Replace commas with spaces
				line.replace(pos, 1, " ");

			// Reduces strings of consecutive spaces to a single space
			// This takes a function as argument, which is what the BothSpaces function earlier is for
			line.erase(std::unique(line.begin(), line.end(), BothSpaces), line.end());
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);

			std::string token;

			while ((pos = line.find(" ")) != std::string::npos)		// Split on spaces, now that they're out only delimiter
			{
				token = line.substr(0, pos);
				line.erase(0, pos + 1);

				if (token.length() > 0)
					s.push_back(token);
			}

			if (line.length() > 0)		// Don't push empty lines
				s.push_back(line);

			data.push_back(s);
			s.clear();
		}
	}
}
