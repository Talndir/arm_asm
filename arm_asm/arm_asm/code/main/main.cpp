/*
	main.cpp

	Windows entry point and main source file.
*/

#include <stdint.h>

#include "../sim/header/computer.h"

// Entry point
int main(int argc, char* argv[])
{
	Computer<uint16_t> computer;

	computer.Run();

	std::cin.ignore();
	return 0;
}