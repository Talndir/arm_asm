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

	// Anim timer
	animTimer.SetOwner(this, wxID_ANY);
	this->Connect(animTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(MainWindow::OnAnimTimer), NULL, this);
	animTimer.Start(50);

	cNodes.push_back(wxPoint(100, 500));
	cNodes.push_back(wxPoint(400, 500));
	cNodes.push_back(wxPoint(200, 500));
	cNodes.push_back(wxPoint(200, 200));
	cNodes.push_back(wxPoint(100, 200));
	cNodes.push_back(wxPoint(400, 200));

	aNodes.push_back(wxPoint(100, 550));
	aNodes.push_back(wxPoint(400, 550));
	aNodes.push_back(wxPoint(250, 550));
	aNodes.push_back(wxPoint(250, 150));
	aNodes.push_back(wxPoint(100, 150));
	aNodes.push_back(wxPoint(400, 150));

	dNodes.push_back(wxPoint(100, 600));
	dNodes.push_back(wxPoint(400, 600));
	dNodes.push_back(wxPoint(300, 600));
	dNodes.push_back(wxPoint(300, 100));
	dNodes.push_back(wxPoint(100, 100));
	dNodes.push_back(wxPoint(400, 100));

	cAnim.speed = aAnim.speed = dAnim.speed = 5.0;

	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(100, 30), wxTE_READONLY));
	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(100, 30), wxTE_READONLY));
	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(100, 30), wxTE_READONLY));

	// Update
	this->UpdateLogic();
}

void MainWindow::OnExit(wxCommandEvent & event)
{
	Close(true);
}

void MainWindow::OnRun(wxCommandEvent & event)
{
	state = PROGRAM_RUNNING;
	parent->UpdateLogic();
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
	parent->UpdateLogic();
	//this->UpdateLogic();
}

void MainWindow::OnStepMicro(wxCommandEvent & event)
{
	state = PROGRAM_STEP_MICROCODE;
	parent->UpdateLogic();
	//this->UpdateLogic();
}

void MainWindow::OnCompile(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
	parent->Compile();
	this->UpdateLogic();
}

void MainWindow::OnHalt(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
}

void MainWindow::OnRunMicrocode(wxCommandEvent & event)
{
	state = PROGRAM_RUN_MICROCODE;
}

void MainWindow::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
	
	for (unsigned int i = 0; i < cNodes.size() - 1; ++i)
	{
		dc.DrawLine(cNodes.at(i), cNodes.at(i + 1));
		dc.DrawLine(aNodes.at(i), aNodes.at(i + 1));
		dc.DrawLine(dNodes.at(i), dNodes.at(i + 1));
	}

	wxRect r;
	wxPoint offset(60, 15);

	r.SetTopLeft(cAnim.Lerp() - offset);
	r.SetBottomRight(cAnim.Lerp() + offset);
	dc.DrawLabel(bus_texts.at(0)->GetValue(), r);
	cAnim.Update();

	r.SetTopLeft(aAnim.Lerp() - offset);
	r.SetBottomRight(aAnim.Lerp() + offset);
	dc.DrawLabel(bus_texts.at(1)->GetValue(), r);
	aAnim.Update();

	r.SetTopLeft(dAnim.Lerp() - offset);
	r.SetBottomRight(dAnim.Lerp() + offset);
	dc.DrawLabel(bus_texts.at(2)->GetValue(), r);
	dAnim.Update();
	
	//dc.DrawCircle(rand() % 200, rand() % 200, 20);
}

void MainWindow::OnAnimTimer(wxTimerEvent & event)
{
	Refresh();
}

void MainWindow::GetText(std::string& s)
{
	s = text->GetText();
}

int MainWindow::GetSpeed()
{
	return speedSlider->GetValue();
}

void MainWindow::UpdateLogic()
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
		std::vector<uint16_t> v;
		parent->GetBuses(v);

		std::stringstream c, a, d;
		c << "CTRL: " << std::setw(4) << std::setfill('0') << std::hex << v.at(0);
		a << "ADDR: " << std::setw(4) << std::setfill('0') << std::hex << v.at(1);
		d << "DATA: " << std::setw(4) << std::setfill('0') << std::hex << v.at(2);

		if (bus_texts.at(0)->GetValue() == c.str())
			cAnim.source = cAnim.destination;
		else
		{
			bus_texts.at(0)->ChangeValue(c.str());
			cAnim.source = COMPONENT_DECODER;
		}

		if (bus_texts.at(1)->GetValue() == a.str())
			aAnim.source = aAnim.destination;
		else
		{
			bus_texts.at(1)->ChangeValue(a.str());
			aAnim.source = COMPONENT_DECODER;
		}

		if (bus_texts.at(2)->GetValue() == d.str())
			dAnim.source = dAnim.destination;
		else
		{
			bus_texts.at(2)->ChangeValue(d.str());
			dAnim.source = COMPONENT_DECODER;
		}

		switch ((v.at(0) & 0x0F00) >> 8)
		{
		case 1:
			cAnim.destination = aAnim.destination = dAnim.destination = COMPONENT_ALU;
			break;
		case 2:
			cAnim.destination = aAnim.destination = dAnim.destination = COMPONENT_REGFILE;
			break;
		case 4:
			cAnim.destination = aAnim.destination = dAnim.destination = COMPONENT_RAM;
			break;
		case 8:
			cAnim.destination = aAnim.destination = dAnim.destination = COMPONENT_DECODER;
			break;
		}

		anim* anim = &cAnim;
		std::vector<wxPoint>* nodes = &cNodes;

		for (unsigned int i = 0; i < 3; ++i)
		{
			if (i == 0) { anim = &cAnim; nodes = &cNodes; }
			else if (i == 1) { anim = &aAnim; nodes = &aNodes; }
			else if (i == 2) { anim = &dAnim; nodes = &dNodes; }

			anim->points.clear();
			anim->index = 0;
			anim->x = 0.0;

			switch (anim->source)
			{
			case COMPONENT_DECODER:

				anim->points.push_back(nodes->at(0));

				switch (anim->destination)
				{
				case COMPONENT_DECODER:
					break;
				case COMPONENT_RAM:
					anim->points.push_back(nodes->at(1));
					break;
				case COMPONENT_ALU:
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(4));
					break;
				case COMPONENT_REGFILE:
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(5));
					break;
				}

				break;

			case COMPONENT_ALU:

				anim->points.push_back(nodes->at(4));

				switch (anim->destination)
				{
				case COMPONENT_ALU:
					break;
				case COMPONENT_REGFILE:
					anim->points.push_back(nodes->at(5));
					break;
				case COMPONENT_DECODER:
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(0));
					break;
				case COMPONENT_RAM:
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(1));
					break;
				}

				break;

			case COMPONENT_REGFILE:

				anim->points.push_back(nodes->at(5));

				switch (anim->destination)
				{
				case COMPONENT_REGFILE:
					break;
				case COMPONENT_ALU:
					anim->points.push_back(nodes->at(4));
					break;
				case COMPONENT_DECODER:
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(0));
					break;
				case COMPONENT_RAM:
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(1));
					break;
				}

				break;

			case COMPONENT_RAM:

				anim->points.push_back(nodes->at(1));

				switch (anim->destination)
				{
				case COMPONENT_RAM:
					break;
				case COMPONENT_DECODER:
					anim->points.push_back(nodes->at(0));
					break;
				case COMPONENT_ALU:
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(4));
					break;
				case COMPONENT_REGFILE:
					anim->points.push_back(nodes->at(2));
					anim->points.push_back(nodes->at(3));
					anim->points.push_back(nodes->at(5));
					break;
				}

				break;
			}
		}
	}

	{
		int line = parent->GetLine();

		text->MarkerDeleteHandle(currentLineMarker);
		currentLineMarker = text->MarkerAdd(line, MARKER_CURRENT_LINE);
	}

	//this->Refresh();
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
EVT_PAINT(MainWindow::OnPaint)
wxEND_EVENT_TABLE()