#include "../../gui/header/app.h"

bool App::OnInit()
{
	mainWindow = new MainWindow("ARM_ASM", wxPoint(50, 50), wxSize(640, 480), this);
	mainWindow->Show(true);

	std::vector<std::vector<std::string>> data;
	ReadFromFile("colours.txt", data);

	std::vector<uint32_t> code;
	Parse(data, code);

	computer.Read(code);
	computer.Run();

	return true;
}

void App::RunProgram()
{
	std::vector<std::vector<std::string>> data, d2;
	std::string s;
	mainWindow->GetText(s);
	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
	ReadFromString(s, data);
	ReadFromFile("colours.txt", d2);

	std::vector<uint32_t> code;
	Parse(data, code);

	computer.Reset();
	computer.Read(code);
	computer.Run();
}

void App::GetVDU(std::vector<std::vector<uint8_t>>& v)
{
	computer.GetVDU(v);
}
