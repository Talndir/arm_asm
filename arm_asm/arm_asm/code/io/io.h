#pragma once

/*
	io.h

	Header file for IO functions.
	Only need to include this for all the IO funcs.
*/

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

/* Function declarations */
void ReadFromFile(std::string filename, std::vector<std::vector<std::string>>& data);
void ReadFromString(std::string& str, std::vector<std::vector<std::string>>& data);
int Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code);
void MatchLines(std::string& str, std::map<int, int>& data);