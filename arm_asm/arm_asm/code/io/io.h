#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

void ReadFromFile(std::string filename, std::vector<std::vector<std::string>>& data);
void ReadFromString(std::string& str, std::vector<std::vector<std::string>>& data);
void Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code);