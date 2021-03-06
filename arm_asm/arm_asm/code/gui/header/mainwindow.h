#pragma once

/*
	mainwindow.h

	Header file for MainWindow class.
	Inherits wxFrame.
	Contains main window class, as well as enums needed by MainWindow.
*/

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"
#include "../../gui/header/anim.h"

#include <wx\stc\stc.h>		// Styled text control
#include <wx\grid.h>		// Grid
#include <wx\gbsizer.h>		// Grid bag sizer

#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

// Program states
enum Program
{
	PROGRAM_HALT = 0,
	PROGRAM_RUNNING = 1,
	PROGRAM_RESET = 2,
	PROGRAM_STEP_INSTRUCTION = 3,
	PROGRAM_STEP_MICROCODE = 4,
	PROGRAM_RUN_MICROCODE = 5,
	PROGRAM_PAUSE_MICROCODE = 6,
	PROGRAM_PAUSE_STEP_MICROCODE = 7
};


/* Class definition */
class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, AppProxy* parentApp);

	void GetText(std::string& s);
	int GetSpeed();

	void UpdateLogic();

	wxSlider* speedSlider;
	wxTimer pauseTimer;
	int state;
	bool cChanged, aChanged, dChanged;

private:
	AppProxy* parent;

	void OnClose(wxCloseEvent& event);

	void OnExit(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnStepInstruction(wxCommandEvent& event);
	void OnStepMicro(wxCommandEvent& event);
	void OnCompile(wxCommandEvent& event);
	void OnHalt(wxCommandEvent& event);
	void OnRunMicrocode(wxCommandEvent& event);

	void OnPaint(wxPaintEvent& event);
	void OnAnimTimer(wxTimerEvent& event);
	void OnPauseTimer(wxTimerEvent& event);

	wxStyledTextCtrl* text;
	wxGrid* vdu;
	std::vector<wxTextCtrl*> registerBoxes;
	wxTextCtrl* pc;
	wxTextCtrl* cir;
	wxCheckBox* ramColourBox;
	wxSlider* animSpeedSlider;
	wxTextCtrl* vduPage;

	std::vector<wxTextCtrl*> alu_texts;
	std::vector<wxTextCtrl*> regfile_texts;
	std::vector<wxTextCtrl*> decoder_texts;
	std::vector<wxTextCtrl*> bus_texts;

	wxTimer animTimer;

	anim cAnim, aAnim, dAnim;
	std::vector<wxPoint> cNodes, aNodes, dNodes;

	int currentLineMarker;

	wxDECLARE_EVENT_TABLE();
};

// Menu buttons
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

// Computer components for animations
enum Components
{
	COMPONENT_DECODER = 0,
	COMPONENT_ALU = 1,
	COMPONENT_REGFILE = 2,
	COMPONENT_RAM = 3
};