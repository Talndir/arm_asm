#include "../../gui/header/mainwindow.h"

/*
	mainwindow.cpp

	CPP file for MainWindow class.
	Contains all function definitions, as well as some enums only used by MainWindow.
*/

// Margin IDs for text box
enum textMargins
{
	MARGIN_LINE_NUMBERS = 0
};

// Marker IDs for margin of text box
enum markers
{
	MARKER_CURRENT_LINE = 0
};

// Constructor - overrides base class constructor but also calls base class constructor as well
MainWindow::MainWindow(const wxString & title, const wxPoint & pos, const wxSize & size, AppProxy* parentApp) : parent(parentApp), wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Global sizer
	wxGridBagSizer* globalSizer = new wxGridBagSizer;

	/* Animation */
	wxGridBagSizer* animSizer = new wxGridBagSizer;

	// ALU
	wxPanel* aluPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(160, 200));
	wxGridBagSizer* aluSizer = new wxGridBagSizer;
	wxStaticText* alu_label = new wxStaticText(aluPanel, wxID_ANY, wxT("ALU"));
	aluSizer->Add(alu_label, wxGBPosition(0, 0));
	alu_texts.push_back(new wxTextCtrl(aluPanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	alu_texts.push_back(new wxTextCtrl(aluPanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	alu_texts.push_back(new wxTextCtrl(aluPanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	aluSizer->Add(alu_texts.at(0), wxGBPosition(1, 0));
	aluSizer->Add(alu_texts.at(1), wxGBPosition(1, 1));
	aluSizer->Add(alu_texts.at(2), wxGBPosition(2, 0));
	aluPanel->SetSizer(aluSizer);

	// Register File
	wxPanel* regfilePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(160, 280));
	wxGridBagSizer* regfileSizer = new wxGridBagSizer;
	wxStaticText* regfile_label = new wxStaticText(regfilePanel, wxID_ANY, wxT("Register File"));
	regfileSizer->Add(regfile_label, wxGBPosition(0, 0));
	for (unsigned int i = 0; i < 8; ++i)
	{
		for (unsigned int j = 0; j < 2; ++j)
		{
			regfile_texts.push_back(new wxTextCtrl(regfilePanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
			regfileSizer->Add(regfile_texts.at(i * 2 + j), wxGBPosition(i + 1, j));
		}
	}
	regfilePanel->SetSizer(regfileSizer);

	// Decoder
	wxPanel* decoderPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(160, 200));
	wxGridBagSizer* decoderSizer = new wxGridBagSizer;
	wxStaticText* decoder_label = new wxStaticText(decoderPanel, wxID_ANY, wxT("Decoder"));
	decoderSizer->Add(decoder_label, wxGBPosition(0, 0));
	decoder_texts.push_back(new wxTextCtrl(decoderPanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	decoder_texts.push_back(new wxTextCtrl(decoderPanel, wxID_ANY, "", wxPoint(), wxSize(160, 30), wxTE_READONLY));
	decoder_texts.push_back(new wxTextCtrl(decoderPanel, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_READONLY));
	decoderSizer->Add(decoder_texts.at(0), wxGBPosition(1, 0));
	decoderSizer->Add(decoder_texts.at(1), wxGBPosition(2, 0));
	decoderSizer->Add(decoder_texts.at(2), wxGBPosition(3, 0));
	decoderPanel->SetSizer(decoderSizer);

	// RAM
	wxPanel* ramPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(160, 200));
	wxGridBagSizer* ramSizer = new wxGridBagSizer;
	wxStaticText* ram_label = new wxStaticText(ramPanel, wxID_ANY, wxT("RAM"));
	ramSizer->Add(ram_label, wxGBPosition(0, 0));
	ramPanel->SetSizer(ramSizer);

	// Animation speed
	animSpeedSlider = new wxSlider(this, wxID_ANY, 5, 1, 60, wxPoint(0, 0), wxSize(160, 60), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);

	animSizer->Add(aluPanel, wxGBPosition(0, 0));
	animSizer->Add(regfilePanel, wxGBPosition(0, 2));
	animSizer->Add(decoderPanel, wxGBPosition(2, 0));
	animSizer->Add(ramPanel, wxGBPosition(2, 2));
	animSizer->Add(animSpeedSlider, wxGBPosition(3, 1));
	animSizer->Add(160, 200, wxGBPosition(0, 1));
	animSizer->Add(160, 100, wxGBPosition(1, 0));

	/* Text editor */
	text = new wxStyledTextCtrl(this, wxID_ANY, wxPoint(), wxSize(340, 600));
	text->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
	text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	text->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	text->SetWrapMode(wxSTC_WRAP_WORD);
	text->SetText("");
	text->StyleClearAll();

	// Markers
	text->MarkerDefine(MARKER_CURRENT_LINE, wxSTC_MARK_ARROW, wxColour(0, 255, 0), wxColour(0, 255, 0));
	currentLineMarker = text->MarkerAdd(0, MARKER_CURRENT_LINE);

	/* Data sizer */
	wxGridBagSizer* dataSizer = new wxGridBagSizer;

	/* VDU */
	vdu = new wxGrid(this, wxID_ANY, wxPoint(0, 0), wxSize(440, 440), wxWANTS_CHARS, "RAM Grid");
	vdu->CreateGrid(16, 16);
	vdu->SetDefaultRowSize(24);
	vdu->SetDefaultColSize(24);
	vdu->SetRowLabelSize(24);
	vdu->SetColLabelSize(24);
	vdu->DisableDragRowSize();
	vdu->DisableDragColSize();

	// Checkbox for enabling/disabling colours
	ramColourBox = new wxCheckBox(this, wxID_ANY, "RAM colours");

	// Text box for choosing VDU 'page'
	vduPage = new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(80, 30), wxTE_LEFT | wxFILTER_NUMERIC);
	vduPage->SetValue("0");

	/* Registers */
	/*
	// General purpose registers
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
	*/

	/* Speed slider */
	speedSlider = new wxSlider(this, wxID_ANY, 5, 1, 100, wxPoint(0, 0), wxSize(200, 60), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);

	/* Add elements to sizers */
	//dataSizer->Add(regSizer, wxGBPosition(0, 0));
	//dataSizer->Add(spregSizer, wxGBPosition(0, 1));
	dataSizer->Add(vdu, wxGBPosition(0, 2));
	dataSizer->Add(ramColourBox, wxGBPosition(1, 2));
	dataSizer->Add(vduPage, wxGBPosition(2, 2));
	dataSizer->Add(speedSlider, wxGBPosition(3, 2));
	
	globalSizer->Add(animSizer, wxGBPosition(0, 0));
	globalSizer->Add(text, wxGBPosition(0, 1));
	globalSizer->Add(dataSizer, wxGBPosition(0, 2));
	SetSizer(globalSizer);

	/* Menu */
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(MENU_LOAD, "Open\tCTRL-O");
	menuFile->Append(MENU_SAVE, "Save\tCTRL-S");
	menuFile->Append(wxID_EXIT, "Exit");

	// Program menu
	wxMenu* menuProgram = new wxMenu;
	menuProgram->Append(MENU_RUN, "Run\tF5");
	menuProgram->Append(MENU_STEP_INSTRUCTION, "Step Instruction\tF1");
	menuProgram->Append(MENU_STEP_MICROCODE, "Step Microcode\tF2");
	menuProgram->Append(MENU_COMPILE, "Compile\tF4");
	menuProgram->Append(MENU_HALT, "Halt\tF3");
	menuProgram->Append(MENU_RUN_MICROCODE, "Run Microcode\tF6");

	// Menu bar
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "File");
	menuBar->Append(menuProgram, "Program");
	SetMenuBar(menuBar);

	/* Animation */
	// Timers
	animTimer.SetOwner(this, wxID_ANY);
	this->Connect(animTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(MainWindow::OnAnimTimer), NULL, this);
	animTimer.Start(50);
	pauseTimer.SetOwner(this, wxID_ANY);
	this->Connect(pauseTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(MainWindow::OnPauseTimer), NULL, this);
	
	// Nodes for animation (interp points)
	cNodes.push_back(wxPoint(160, 450));
	cNodes.push_back(wxPoint(320, 450));
	cNodes.push_back(wxPoint(200, 450));
	cNodes.push_back(wxPoint(200, 150));
	cNodes.push_back(wxPoint(160, 150));
	cNodes.push_back(wxPoint(320, 150));

	aNodes.push_back(wxPoint(160, 500));
	aNodes.push_back(wxPoint(320, 500));
	aNodes.push_back(wxPoint(240, 500));
	aNodes.push_back(wxPoint(240, 100));
	aNodes.push_back(wxPoint(160, 100));
	aNodes.push_back(wxPoint(320, 100));

	dNodes.push_back(wxPoint(160, 550));
	dNodes.push_back(wxPoint(320, 550));
	dNodes.push_back(wxPoint(280, 550));
	dNodes.push_back(wxPoint(280, 50));
	dNodes.push_back(wxPoint(160, 50));
	dNodes.push_back(wxPoint(320, 50));

	// Animation speed
	cAnim.speed = aAnim.speed = dAnim.speed = animSpeedSlider->GetValue();

	// Bus data
	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(40, 20), wxTE_READONLY));
	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(40, 20), wxTE_READONLY));
	bus_texts.push_back(new wxTextCtrl(this, wxID_ANY, "", wxPoint(), wxSize(40, 20), wxTE_READONLY));

	bus_texts.at(0)->Hide();
	bus_texts.at(1)->Hide();
	bus_texts.at(2)->Hide();

	/* Update */
	state = PROGRAM_HALT;
	this->UpdateLogic();
}

void MainWindow::OnClose(wxCloseEvent & event)
{
	if (event.CanVeto())
	{
		wxMessageDialog* msg = new wxMessageDialog(this, "Would you like to save before exiting?", "Exit", wxYES_NO | wxCANCEL, wxDefaultPosition);

		switch (msg->ShowModal())
		{
		case wxID_CANCEL:
			event.Veto();
			break;
		case wxID_NO:
			Destroy();
			break;
		case wxID_YES:
			this->OnLoad(wxCommandEvent());
			Destroy();
			break;
		}
	}
}

// Callback function for exiting window
void MainWindow::OnExit(wxCommandEvent & event)
{
	this->OnClose(wxCloseEvent());
}

// Callback function for Menu->Program->Run
void MainWindow::OnRun(wxCommandEvent & event)
{
	state = PROGRAM_RUNNING;
	parent->UpdateLogic();
}

// Callback function for Menu->File->Load
void MainWindow::OnLoad(wxCommandEvent & event)
{
	wxFileDialog* d = new wxFileDialog(this, "Open", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN);

	if (d->ShowModal() == wxID_OK)
	{
		text->LoadFile(d->GetPath());
		SetTitle(d->GetFilename());
	}
}

// Callback function for Menu->File->Save
void MainWindow::OnSave(wxCommandEvent & event)
{
	wxFileDialog* d = new wxFileDialog(this, "Save", "", "", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (d->ShowModal() == wxID_OK)
	{
		text->SaveFile(d->GetPath());
		SetTitle(d->GetFilename());
	}
}

// Callback function for Menu->Program->Step Instruction
void MainWindow::OnStepInstruction(wxCommandEvent & event)
{
	state = PROGRAM_STEP_INSTRUCTION;
	parent->UpdateLogic();
}

// Callback function for Menu->Program->Step Microcode
void MainWindow::OnStepMicro(wxCommandEvent & event)
{
	state = PROGRAM_STEP_MICROCODE;
	parent->UpdateLogic();
}

// Callback function for Menu->Program->Compile
void MainWindow::OnCompile(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
	parent->Compile();
	this->UpdateLogic();
}

// Callback function for Menu->Program->Halt
void MainWindow::OnHalt(wxCommandEvent & event)
{
	state = PROGRAM_HALT;
}

// Callback function for Menu->Program->Run Microcode
void MainWindow::OnRunMicrocode(wxCommandEvent & event)
{
	state = PROGRAM_RUN_MICROCODE;
}

// Callback function for paint events
void MainWindow::OnPaint(wxPaintEvent & event)
{
	// Construct paintDC
	wxPaintDC dc(this);
	
	// Draw bus lines
	for (unsigned int i = 0; i < cNodes.size() - 1; ++i)
	{
		dc.DrawLine(cNodes.at(i), cNodes.at(i + 1));
		dc.DrawLine(aNodes.at(i), aNodes.at(i + 1));
		dc.DrawLine(dNodes.at(i), dNodes.at(i + 1));
	}

	wxRect r;
	wxPoint offset = wxPoint(bus_texts.at(0)->GetSize().x, bus_texts.at(0)->GetSize().y) / 2;

	// Draw bus values
	if ((state == PROGRAM_PAUSE_MICROCODE) || (state == PROGRAM_RUN_MICROCODE) || (state == PROGRAM_PAUSE_STEP_MICROCODE))
	{
		r.SetTopLeft(cAnim.Lerp() - offset);
		r.SetBottomRight(cAnim.Lerp() + offset);
		dc.DrawLabel(bus_texts.at(0)->GetValue(), r);

		r.SetTopLeft(aAnim.Lerp() - offset);
		r.SetBottomRight(aAnim.Lerp() + offset);
		dc.DrawLabel(bus_texts.at(1)->GetValue(), r);

		r.SetTopLeft(dAnim.Lerp() - offset);
		r.SetBottomRight(dAnim.Lerp() + offset);
		dc.DrawLabel(bus_texts.at(2)->GetValue(), r);
	}

	// Only update animations if microcode is paused between instructions but still running
	if ((state == PROGRAM_PAUSE_MICROCODE) || (state == PROGRAM_PAUSE_STEP_MICROCODE))
	{
		cAnim.Update();
		aAnim.Update();
		dAnim.Update();
	}

	// Start next microcode after a timed delay
	if ((cAnim.index == cAnim.points.size() - 1) && (aAnim.index == aAnim.points.size() - 1) && (dAnim.index == dAnim.points.size() - 1) && (!pauseTimer.IsRunning()) && ((state == PROGRAM_PAUSE_MICROCODE) || (state == PROGRAM_PAUSE_STEP_MICROCODE)))
		pauseTimer.StartOnce(5000 / animSpeedSlider->GetValue());
}

// Callback function for animation tick timer
void MainWindow::OnAnimTimer(wxTimerEvent & event)
{
	cAnim.speed = aAnim.speed = dAnim.speed = animSpeedSlider->GetValue();
	Refresh();
}

// Callback function for animation pause timer
void MainWindow::OnPauseTimer(wxTimerEvent & event)
{
	if (state == PROGRAM_PAUSE_MICROCODE)
		state = PROGRAM_RUN_MICROCODE;
	else if (state == PROGRAM_PAUSE_STEP_MICROCODE)
		state = PROGRAM_HALT;
}

// Gets text from text box
void MainWindow::GetText(std::string& s)
{
	s = text->GetText();
}

// Gets speed of the general program speed slider
int MainWindow::GetSpeed()
{
	return speedSlider->GetValue();
}

// Updates all values of MainWindow to update GUI
void MainWindow::UpdateLogic()
{
	// Update VDU
	{
		std::string s = vduPage->GetValue();
		if (s != "")
			vduPage->SetValue(wxString() << (int)std::max(0, std::min(255, std::stoi(s))));

		std::vector<std::vector<uint8_t>> v;

		if (s != "")
			parent->GetVDU(v, std::stoi(std::string(vduPage->GetValue())));
		else
			parent->GetVDU(v, 0);

		for (unsigned int i = 0; i < 16; ++i)
		{
			for (unsigned int j = 0; j < 16; ++j)
			{
				if (ramColourBox->GetValue())	// Change background colour if box checked, else change text
				{
					vdu->SetCellValue(i, j, "");
					vdu->SetCellBackgroundColour(i, j, rgb2wx(hsv2rgb(hsv(v.at(i).at(j) / 256.0 * 360.0, 1.0, 1.0))));
				}
				else
				{
					std::stringstream s;
					s << std::setw(2) << std::setfill('0') << std::hex << (int)v.at(i).at(j);
					vdu->SetCellValue(i, j, s.str());
					vdu->SetCellBackgroundColour(i, j, wxColor("WHITE"));
				}
			}
		}
	}

	// Register file general purpose registers
	{
		std::vector<uint16_t> v;
		parent->GetRegisterFile(v);

		for (unsigned int i = 0; i < 16; ++i)
		{
			std::stringstream s;
			s << "R" << std::setw(2) << std::setfill('0') << i;
			s << ": " << std::setw(4) << std::setfill('0') << std::hex << v.at(i);
			
			//registerBoxes.at(i)->ChangeValue(s.str());
			regfile_texts.at(i)->ChangeValue(s.str());
		}
	}

	// Decoder values
	{
		std::vector<uint16_t> v;
		parent->GetDecoder(v);

		std::stringstream s;
		s << "PC: " << std::setw(4) << std::setfill('0') << std::hex << v.at(0);	// Program counter

		//pc->ChangeValue(s.str());
		decoder_texts.at(0)->ChangeValue(s.str());

		s.clear();
		s.str("");
		uint32_t r = v.at(1) + (v.at(2) << 8) + (v.at(3) << 16) + (v.at(4) << 24);	// Current instruction register, in two pieces
		s << "CIR: " << std::setw(8) << std::setfill('0') << std::hex << r;

		//cir->ChangeValue(s.str());
		decoder_texts.at(1)->ChangeValue(s.str());

		s.clear();
		s.str("");
		s << "LR: " << std::setw(4) << std::setfill('0') << std::hex << v.at(6);	// Link register

		decoder_texts.at(2)->ChangeValue(s.str());
	}

	// ALU contents
	{
		std::vector<uint16_t> v;
		parent->GetALU(v);

		std::stringstream a, b, c;
		a << "R0: " << std::setw(4) << std::setfill('0') << std::hex << v.at(0);	// A0
		b << "R1: " << std::setw(4) << std::setfill('0') << std::hex << v.at(1);	// A1
		c << "ACC: " << std::setw(4) << std::setfill('0') << std::hex << v.at(2);	// ACC

		alu_texts.at(0)->ChangeValue(a.str());
		alu_texts.at(1)->ChangeValue(b.str());
		alu_texts.at(2)->ChangeValue(c.str());
	}

	// Get bus values for animation
	{
		std::vector<uint16_t> v;
		parent->GetBuses(v);

		std::stringstream c, a, d;
		c << /*"CTRL: " <<*/ std::setw(4) << std::setfill('0') << std::hex << v.at(0);
		a << /*"ADDR: " <<*/ std::setw(4) << std::setfill('0') << std::hex << v.at(1);
		d << /*"DATA: " <<*/ std::setw(4) << std::setfill('0') << std::hex << v.at(2);

		// If bus has not been set a value by decoder (value explicitly left alone) then animate it from last destination
		// else animate it from the decoder
		if (!cChanged)
			cAnim.source = cAnim.destination;
		else
		{
			bus_texts.at(0)->ChangeValue(c.str());
			cAnim.source = COMPONENT_DECODER;
		}

		if (!aChanged)
			aAnim.source = aAnim.destination;
		else
		{
			bus_texts.at(1)->ChangeValue(a.str());
			aAnim.source = COMPONENT_DECODER;
		}

		if (!dChanged)
			dAnim.source = dAnim.destination;
		else
		{
			bus_texts.at(2)->ChangeValue(d.str());
			dAnim.source = COMPONENT_DECODER;
		}

		// Check what the destination is
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

		// Set the points to interpolate between based on the 4^2 combinations of source and destination
		// Crude but it's very fast (double jump table)
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

	// Get which line we're on
	{
		int line = parent->GetLine();

		text->MarkerDeleteHandle(currentLineMarker);							// Delete line marker from previous line

		if (line != -1)
			currentLineMarker = text->MarkerAdd(line, MARKER_CURRENT_LINE);		// Draw line marker on current line
	}
}

// This wxWidgets MACRO-MANIA sets up the callback functions for menus, timers, paint events etc.
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_CLOSE(MainWindow::OnClose)
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