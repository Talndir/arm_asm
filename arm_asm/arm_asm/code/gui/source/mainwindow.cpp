#include "../../gui/header/mainwindow.h"

enum textMargins
{
	MARGIN_LINE_NUMBERS = 0
};

enum markers
{
	MARKER_CURRENT_LINE = 0
};

MainWindow::MainWindow(const wxString & title, const wxPoint & pos, const wxSize & size, AppProxy* parentApp) : parent(parentApp), wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Global sizer
	wxGridBagSizer* globalSizer = new wxGridBagSizer;

	// Anim sizer
	wxGridBagSizer* animSizer = new wxGridBagSizer;

	wxGridBagSizer* aluSizer = new wxGridBagSizer;
	wxStaticText* alu_label = new wxStaticText(this, wxID_ANY, wxT("ALU"));
	aluSizer->Add(alu_label, wxGBPosition(0, 0));
	alu_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	alu_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	alu_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	aluSizer->Add(alu_texts.at(0), wxGBPosition(1, 0));
	aluSizer->Add(alu_texts.at(1), wxGBPosition(1, 1));
	aluSizer->Add(alu_texts.at(2), wxGBPosition(2, 0));

	wxGridBagSizer* regfileSizer = new wxGridBagSizer;
	wxStaticText* regfile_label = new wxStaticText(this, wxID_ANY, wxT("Register File"));
	regfileSizer->Add(regfile_label, wxGBPosition(0, 0));
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			regfile_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
			regfileSizer->Add(regfile_texts.at(i * 4 + j), wxGBPosition(i + 1, j));
		}
	}

	wxGridBagSizer* decoderSizer = new wxGridBagSizer;
	wxStaticText* decoder_label = new wxStaticText(this, wxID_ANY, wxT("Decoder"));
	decoderSizer->Add(decoder_label, wxGBPosition(0, 0));
	decoder_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	decoder_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(200, 30), wxTE_READONLY));
	decoderSizer->Add(decoder_texts.at(0), wxGBPosition(1, 0));
	decoderSizer->Add(decoder_texts.at(1), wxGBPosition(2, 0));

	wxGridBagSizer* ramSizer = new wxGridBagSizer;
	wxStaticText* ram_label = new wxStaticText(this, wxID_ANY, wxT("RAM"));
	ramSizer->Add(ram_label, wxGBPosition(0, 0));

	animSizer->Add(aluSizer, wxGBPosition(0, 0));
	animSizer->Add(regfileSizer, wxGBPosition(0, 2));
	animSizer->Add(decoderSizer, wxGBPosition(2, 0));
	animSizer->Add(ramSizer, wxGBPosition(2, 2));
	animSizer->Add(150, 150, wxGBPosition(0, 1));
	animSizer->Add(150, 150, wxGBPosition(1, 0));

	// Text editor
	text = new wxStyledTextCtrl(this, wxID_ANY, wxPoint(), wxSize(250, 720));
	text->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
	text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	text->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	text->SetWrapMode(wxSTC_WRAP_WORD);
	text->SetText("");
	text->StyleClearAll();

	text->MarkerDefine(MARKER_CURRENT_LINE, wxSTC_MARK_ARROW, wxColour(0, 255, 0), wxColour(0, 255, 0));
	currentLineMarker = text->MarkerAdd(0, MARKER_CURRENT_LINE);

	// Data sizer
	wxGridBagSizer* dataSizer = new wxGridBagSizer;

	// VDU
	vdu = new wxGrid(this, wxID_ANY, wxPoint(0, 0), wxSize(600, 600), wxWANTS_CHARS, "RAM Grid");
	vdu->CreateGrid(16, 16);
	vdu->SetDefaultRowSize(30);
	vdu->SetDefaultColSize(30);
	vdu->SetRowLabelSize(30);
	vdu->SetColLabelSize(30);
	vdu->DisableDragRowSize();
	vdu->DisableDragColSize();

	ramColourBox = new wxCheckBox(this, wxID_ANY, "RAM colours");

	// Registers
	wxBoxSizer* regSizer = new wxBoxSizer(wxVERTICAL);

	for (unsigned int i = 0; i < 16; ++i)
	{
		registerBoxes.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY | wxTE_LEFT));
		regSizer->Add(registerBoxes.at(i));
	}

	// Special registers
	wxBoxSizer* spregSizer = new wxBoxSizer(wxVERTICAL);

	pc = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY | wxTE_LEFT);
	cir = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(120, 30), wxTE_READONLY);

	spregSizer->Add(pc);
	spregSizer->Add(cir);

	// Speed slider
	speedSlider = new wxSlider(this, wxID_ANY, 50, 1, 1000, wxPoint(0, 0), wxSize(200, 60), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);

	// Add elements to sizers
	dataSizer->Add(regSizer, wxGBPosition(0, 0));
	dataSizer->Add(spregSizer, wxGBPosition(0, 1));
	dataSizer->Add(vdu, wxGBPosition(0, 2));
	dataSizer->Add(ramColourBox, wxGBPosition(1, 2));
	dataSizer->Add(speedSlider, wxGBPosition(2, 2));
	
	globalSizer->Add(animSizer, wxGBPosition(0, 0));
	globalSizer->Add(text, wxGBPosition(0, 1));
	globalSizer->Add(dataSizer, wxGBPosition(0, 2));
	SetSizer(globalSizer);

	// Menu bar
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(MENU_LOAD, "Load");
	menuFile->Append(MENU_SAVE, "Save");

	wxMenu* menuProgram = new wxMenu;
	menuProgram->Append(MENU_RUN, "Run");
	menuProgram->Append(MENU_STEP_INSTRUCTION, "Step Instruction");
	menuProgram->Append(MENU_STEP_MICROCODE, "Step Microcode");
	menuProgram->Append(MENU_COMPILE, "Compile");
	menuProgram->Append(MENU_HALT, "Halt");
	menuProgram->Append(MENU_RUN_MICROCODE, "Run Microcode");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "File");
	menuBar->Append(menuProgram, "Program");
	SetMenuBar(menuBar);

	// Update
	this->Update();
}

void MainWindow::OnExit(wxCommandEvent & event)
{
	Close(true);
}

void MainWindow::OnRun(wxCommandEvent & event)
{
	state = PROGRAM_RUNNING;
	parent->Update();
	this->Update();
}

void MainWindow::OnLoad(wxCommandEvent & event)
{
	wxFileDialog* d = new wxFileDialog(this, "Open", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN);

	if (d->ShowModal() == wxID_OK)
	{
		text->LoadFile(d->GetPath());
		SetTitle(d->GetFilename());
	}
}

void MainWindow::OnSave(wxCommandEvent & event)
{
	wxFileDialog* d = new wxFileDialog(this, "Save", "", "", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (d->ShowModal() == wxID_OK)
	{
		text->SaveFile(d->GetPath());
		SetTitle(d->GetFilename());
	}
}

void MainWindow::OnStepInstruction(wxCommandEvent & event)
{
	state = PROGRAM_STEP_INSTRUCTION;
	parent->Update();
	this->Update();
}

void MainWindow::OnStepMicro(wxCommandEvent & event)
{
	state = PROGRAM_STEP_MICROCODE;
	parent->Update();
	this->Update();
}

void MainWindow::OnCompile(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
	parent->Compile();
	this->Update();
}

void MainWindow::OnHalt(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
}

void MainWindow::OnRunMicrocode(wxCommandEvent & event)
{
	state = PROGRAM_RUN_MICROCODE;
}

void MainWindow::GetText(std::string& s)
{
	s = text->GetText();
}

int MainWindow::GetSpeed()
{
	return speedSlider->GetValue();
}

void MainWindow::Update()
{
	{
		std::vector<std::vector<uint8_t>> v;
		parent->GetVDU(v);

		for (unsigned int i = 0; i < 16; ++i)
		{
			for (unsigned int j = 0; j < 16; ++j)
			{
				if (ramColourBox->GetValue())
				{
					vdu->SetCellValue(i, j, "");
					vdu->SetCellBackgroundColour(i, j, rgb2wx(hsv2rgb(hsv(v.at(i).at(j) / 16.0 * 360.0, 1.0, 1.0))));
				}
				else
				{
					std::stringstream s;
					s << std::setw(2) << std::setfill('0') << (int)v.at(i).at(j);
					vdu->SetCellValue(i, j, s.str());
					vdu->SetCellBackgroundColour(i, j, wxColor("WHITE"));
				}
			}
		}
	}

	{
		std::vector<uint16_t> v;
		parent->GetRegisterFile(v);

		for (unsigned int i = 0; i < 16; ++i)
		{
			std::stringstream s;
			s << "R" << std::setw(2) << std::setfill('0') << i;
			s << ": " << std::setw(4) << std::setfill('0') << std::hex << v.at(i);
			
			registerBoxes.at(i)->ChangeValue(s.str());
			regfile_texts.at(i)->ChangeValue(s.str());
		}
	}

	{
		std::vector<uint16_t> v;
		parent->GetDecoder(v);

		std::stringstream s;
		s << "PC: " << std::setw(4) << std::setfill('0') << std::hex << v.at(0);

		pc->ChangeValue(s.str());
		decoder_texts.at(0)->ChangeValue(s.str());

		s.clear();
		s.str("");
		uint32_t r = v.at(1) + (v.at(2) << 8) + (v.at(3) << 16) + (v.at(4) << 24);
		s << "CIR: " << std::setw(8) << std::setfill('0') << std::hex << r;

		cir->ChangeValue(s.str());
		decoder_texts.at(1)->ChangeValue(s.str());
	}

	{
		std::vector<uint16_t> v;
		parent->GetALU(v);

		std::stringstream a, b, c;
		a << "R0: " << std::setw(4) << std::setfill('0') << std::hex << v.at(0);
		b << "R1: " << std::setw(4) << std::setfill('0') << std::hex << v.at(1);
		c << "ACC: " << std::setw(4) << std::setfill('0') << std::hex << v.at(2);

		alu_texts.at(0)->ChangeValue(a.str());
		alu_texts.at(1)->ChangeValue(b.str());
		alu_texts.at(2)->ChangeValue(c.str());
	}

	{
		int line = parent->GetLine();

		text->MarkerDeleteHandle(currentLineMarker);
		currentLineMarker = text->MarkerAdd(line, MARKER_CURRENT_LINE);
	}

	this->Refresh();
}

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(MENU_RUN, MainWindow::OnRun)
EVT_MENU(MENU_LOAD, MainWindow::OnLoad)
EVT_MENU(MENU_SAVE, MainWindow::OnSave)
EVT_MENU(MENU_STEP_INSTRUCTION, MainWindow::OnStepInstruction)
EVT_MENU(MENU_STEP_MICROCODE, MainWindow::OnStepMicro)
EVT_MENU(MENU_COMPILE, MainWindow::OnCompile)
EVT_MENU(MENU_HALT, MainWindow::OnHalt)
EVT_MENU(MENU_RUN_MICROCODE, MainWindow::OnRunMicrocode)
wxEND_EVENT_TABLE()