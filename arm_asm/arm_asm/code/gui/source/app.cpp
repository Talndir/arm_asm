#include "../../gui/header/app.h"

/*
	app.cpp

	CPP file for App class.
	Contins all function definitions.
*/


// Initialisation function - overrides base class function, essentially a constrctor
bool App::OnInit()
{
	mainWindow = new MainWindow("ARM_ASM", wxPoint(50, 50), wxSize(1280, 720), this);
	mainWindow->Show(true);

	timer = new TickTimer(this);
	timer->Start(50);

	return true;
}

// Compiles code in text window
void App::Compile()
{
	std::vector<std::vector<std::string>> data, d2;
	std::string s;
	mainWindow->GetText(s);
	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());	// Remove special characters that mess with the parser
	memoryToLine.clear();
	MatchLines(s, memoryToLine);
	ReadFromString(s, data);

	std::vector<uint32_t> code;
	Parse(data, code);

	computer.Reset();
	computer.Read(code);
}

// Gets VDU data from Computer
void App::GetVDU(std::vector<std::vector<uint8_t>>& v)
{
	computer.GetVDU(v);
}

// Gets register file data from Computer
void App::GetRegisterFile(std::vector<uint16_t>& v)
{
	computer.GetRegisterFile(v);
}

// Gets decoder data from Computer
void App::GetDecoder(std::vector<uint16_t>& v)
{
	computer.GetDecoder(v);
}

// Gets RAM contents from Computer
void App::GetRAM(std::vector<uint16_t>& v)
{
	computer.GetRAM(v);
}

// Gets ALU data from Computer
void App::GetALU(std::vector<uint16_t>& v)
{
	computer.GetALU(v);
}

// Gets contents of buses from Computer
void App::GetBuses(std::vector<uint16_t>& v)
{
	computer.GetBuses(v);
}

// Gets current line, to add a marker to text box
int App::GetLine()
{
	std::vector<uint16_t> v;
	computer.GetDecoder(v);

	std::map<int, int>::iterator it = memoryToLine.find(v.at(0) - 4);
	if (it != memoryToLine.end())
		return it->second;
	else
		return 0;		// If no corresponding line is found, use first line
}

// Run a single ASM instruction
void App::RunSingle()
{
	computer.RunSingle();
	mainWindow->UpdateLogic();
}

// Run a single microcode instruction
void App::RunMicro(bool& c, bool& a, bool &d)
{
	bool again;

	computer.RunMicro(c, a, d, again);
	mainWindow->UpdateLogic();

	if (again)	// Checks if another instruction needs to be executed, prevents watching zeroes move around
	{
		computer.RunMicro(c, a, d, again);
		mainWindow->UpdateLogic();
	}
}

// Uses mainWindow->State to select which action to take per timer tick
void App::UpdateLogic()
{
	timer->Start(mainWindow->GetSpeed());		// Update timer speed

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
		RunMicro(mainWindow->cChanged, mainWindow->aChanged, mainWindow->dChanged);
		mainWindow->state = PROGRAM_HALT;
		break;
	case PROGRAM_RUN_MICROCODE:
		RunMicro(mainWindow->cChanged, mainWindow->aChanged, mainWindow->dChanged);
		mainWindow->state = PROGRAM_PAUSE_MICROCODE;
		break;
	default:
		break;
	}
}