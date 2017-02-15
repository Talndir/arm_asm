/*
	main.cpp

	Windows entry point and main source file.
*/

#include <stdint.h>

//#include "../sim/header/computer.h"

//#include "../io/io.h"

#include "../gui/header/app.h"

wxIMPLEMENT_APP(App);

// Entry point
/*
int main(int argc, char* argv[])
{
	std::vector<std::vector<std::string>> data;
	Read("colours.txt", data);

	std::vector<uint32_t> code;
	Parse(data, code);

	Computer<uint16_t> computer;
	computer.Read(code);
	computer.Run();

	std::cin.ignore();
	return 0;
}
*/