#pragma once

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"

#include <wx\stc\stc.h>
#include <wx\grid.h>
#include <wx\gbsizer.h>

#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

enum Program
{
	PROGRAM_HALT = 0,
	PROGRAM_RUNNING = 1,
	PROGRAM_RESET = 2,
	PROGRAM_STEP_INSTRUCTION = 3,
	PROGRAM_STEP_MICROCODE = 4,
	PROGRAM_RUN_MICROCODE = 5
};

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, AppProxy* parentApp);

	void GetText(std::string& s);
	int GetSpeed();

	void Update();

	int state = PROGRAM_HALT;

private:
	AppProxy* parent;

	void OnExit(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnStepInstruction(wxCommandEvent& event);
	void OnStepMicro(wxCommandEvent& event);
	void OnCompile(wxCommandEvent& event);
	void OnHalt(wxCommandEvent& event);
	void OnRunMicrocode(wxCommandEvent& event);

	wxStyledTextCtrl* text;
	wxGrid* vdu;
	std::vector<wxTextCtrl*> registerBoxes;
	wxTextCtrl* pc;
	wxTextCtrl* cir;
	wxSlider* speedSlider;
	wxCheckBox* ramColourBox;

	std::vector<wxTextCtrl*> alu_texts;
	std::vector<wxTextCtrl*> regfile_texts;
	std::vector<wxTextCtrl*> decoder_texts;

	int currentLineMarker;

	wxDECLARE_EVENT_TABLE();
};

enum Menu
{
	MENU_RUN = 0,
	MENU_LOAD = 1,
	MENU_SAVE = 2,
	MENU_STEP_INSTRUCTION = 3,
	MENU_STEP_MICROCODE = 4,
	MENU_COMPILE = 5,
	MENU_HALT = 6,
	MENU_RUN_MICROCODE = 7
};