#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

void Read(std::string filename, std::vector<std::vector<std::string>>& data);
void Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code);