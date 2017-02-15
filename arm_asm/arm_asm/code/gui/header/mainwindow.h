#pragma once

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"

#include <wx\stc\stc.h>
#include <wx\grid.h>

#include <fstream>

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, AppProxy* parentApp);

	void GetText(std::string& s);

private:
	AppProxy* parent;

	void OnExit(wxCommandEvent& event);
	void OnRun(wxCommandEvent& event);

	wxStyledTextCtrl* text = new wxStyledTextCtrl(this, wxID_ANY);
	wxGrid* vdu = new wxGrid(this, wxID_ANY, wxPoint(0, 0), wxSize(320, 320), wxWANTS_CHARS, "RAM Grid");

	wxDECLARE_EVENT_TABLE();
};

enum MainWindowEvents
{
	ID_RUN = 0
};