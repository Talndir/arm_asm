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

	mainWindow->speedSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &App::UpdateTimer, this);

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
	int error = Parse(data, code);

	if (!error)
	{
		computer.Reset();
		computer.Read(code);
	}
	else
	{
		wxMessageDialog* dialog = new wxMessageDialog(mainWindow, "", "Compilation error", wxOK | wxICON_ERROR, wxDefaultPosition);

		switch (error)
		{
		case 1:	// Undefined opcode
			dialog->SetMessage("Undefined opcode");
			break;
		case 2:	// Incorrect nunber of operands
			dialog->SetMessage("Incorrect number of operands");
			break;
		case 3:	// Undefined operand
			dialog->SetMessage("Undefined operand");
			break;
		case 4:	// Operand out of bounds
			dialog->SetMessage("Operand out of bounds\nRegisters should be between R0-R15\nImmediate values should be between 0-4096");
			break;
		case 5:	// Branch destination label does not exist
			dialog->SetMessage("Branch destination label does not exist");
			break;
		}

		dialog->ShowModal();
	}

}

// Gets VDU data from Computer
void App::GetVDU(std::vector<std::vector<uint8_t>>& v, int page)
{
	computer.GetVDU(v, page);
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

	std::map<int, int>::iterator it = memoryToLine.find(v.at(5));

	if (it != memoryToLine.end())
		return it->second;
	else
		return -1;		// If no corresponding line is found, return -1 for no line
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

// Update timer from slider on GUI
void App::UpdateTimer(wxCommandEvent & event)
{
	float speed = (mainWindow->GetSpeed() - 1) / float(100 - 1);
	speed = (1.f - speed) * (1000 - 2) + 2;
	timer->Start(speed);
}

// Uses mainWindow->State to select which action to take per timer tick
void App::UpdateLogic()
{
	// Update timer speed
	UpdateTimer(wxScrollEvent());

	// Execute what needs to be executed
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
		mainWindow->pauseTimer.Stop();
		mainWindow->state = PROGRAM_PAUSE_STEP_MICROCODE;
		break;
	case PROGRAM_RUN_MICROCODE:
		RunMicro(mainWindow->cChanged, mainWindow->aChanged, mainWindow->dChanged);
		mainWindow->state = PROGRAM_PAUSE_MICROCODE;
		break;
	default:
		break;
	}
}