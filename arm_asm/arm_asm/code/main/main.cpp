/*
	main.cpp

	Windows entry point and main source file.
*/

#include <stdint.h>

#include "../sim/header/computer.h"

#include "../io/io.h"

// Entry point
int main(int argc, char* argv[])
{
	//Computer<uint16_t> computer;
	//computer.Run();

	std::vector<std::vector<std::string>> data;
	Read("test.txt", data);

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (unsigned int j = 0; j < data.at(i).size(); ++j)
			std::cout << data.at(i).at(j) << std::endl;
	}

	std::cin.ignore();
	return 0;
}