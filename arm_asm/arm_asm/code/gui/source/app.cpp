#include "../../gui/header/app.h"

bool App::OnInit()
{
	mainWindow = new MainWindow("ARM_ASM", wxPoint(50, 50), wxSize(1280, 720), this);
	mainWindow->Show(true);

	timer = new TickTimer(this);
	timer->Start(50);

	/*
	std::vector<std::vector<std::string>> data;
	ReadFromFile("colours.txt", data);

	std::vector<uint32_t> code;
	Parse(data, code);

	computer.Read(code);
	computer.Run();
	*/

	return true;
}

void App::Compile()
{
	std::vector<std::vector<std::string>> data, d2;
	std::string s;
	mainWindow->GetText(s);
	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
	memoryToLine.clear();
	MatchLines(s, memoryToLine);
	ReadFromString(s, data);

	std::vector<uint32_t> code;
	Parse(data, code);

	computer.Reset();
	computer.Read(code);

	//computer.Run();
}

void App::GetVDU(std::vector<std::vector<uint8_t>>& v)
{
	computer.GetVDU(v);
}

void App::GetRegisterFile(std::vector<uint16_t>& v)
{
	computer.GetRegisterFile(v);
}

void App::GetDecoder(std::vector<uint16_t>& v)
{
	computer.GetDecoder(v);
}

void App::GetRAM(std::vector<uint16_t>& v)
{
	computer.GetRAM(v);
}

void App::GetALU(std::vector<uint16_t>& v)
{
	computer.GetALU(v);
}

int App::GetLine()
{
	std::vector<uint16_t> v;
	computer.GetDecoder(v);

	std::map<int, int>::iterator it = memoryToLine.find(v.at(0));
	if (it != memoryToLine.end())
		return it->second;
	else
		return 0;
}

void App::RunSingle()
{
	computer.RunSingle();
	mainWindow->Update();
}

void App::RunMicro()
{
	computer.RunMicro();
	mainWindow->Update();
}

void App::Update()
{
	timer->Start(mainWindow->GetSpeed());

	switch (mainWindow->state)
	{
	case PROGRAM_RUNNING:
		RunSingle();
		break;
	case PROGRAM_RESET:
		Compile();
		RunSingle();
		mainWindow->state = PROGRAM_RUNNING;
		break;
	case PROGRAM_STEP_INSTRUCTION:
		RunSingle();
		mainWindow->state = PROGRAM_HALT;
		break;
	case PROGRAM_STEP_MICROCODE:
		RunMicro();
		mainWindow->state = PROGRAM_HALT;
		break;
	default:
		break;
	}
}