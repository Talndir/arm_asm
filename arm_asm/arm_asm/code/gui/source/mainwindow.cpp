#include "../../gui/header/mainwindow.h"

enum textMargins
{
	MARGIN_LINE_NUMBERS = 0
};

MainWindow::MainWindow(const wxString & title, const wxPoint & pos, const wxSize & size, AppProxy* parentApp) : parent(parentApp), wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Create sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	// Text editor
	text->SetMarginWidth(MARGIN_LINE_NUMBERS, 50);
	text->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	text->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	text->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	text->SetWrapMode(wxSTC_WRAP_WORD);
	text->SetText("");
	text->StyleClearAll();

	// VDU
	vdu->CreateGrid(16, 16);
	vdu->SetDefaultRowSize(20);
	vdu->SetDefaultColSize(20);
	vdu->DisableDragRowSize();
	vdu->DisableDragColSize();

	// Run button
	wxMenu* menu = new wxMenu;
	menu->Append(ID_RUN, "RUN");
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menu, "RUN");
	SetMenuBar(menuBar);

	// Add elements to sizer
	sizer->Add(text, 1, wxEXPAND);
	sizer->Add(vdu, 1, wxEXPAND);
	SetSizer(sizer);
}

void MainWindow::OnExit(wxCommandEvent & event)
{
	Close(true);
}

void MainWindow::OnRun(wxCommandEvent & event)
{
	parent->RunProgram();

	std::vector<std::vector<uint8_t>> v;
	parent->GetVDU(v);

	for (unsigned int i = 0; i < 16; ++i)
	{
		for (unsigned int j = 0; j < 16; ++j)
			vdu->SetCellBackgroundColour(i, j, rgb2wx(hsv2rgb(hsv(v.at(i).at(j) / 16.0 * 360.0, 1.0, 1.0))));
	}
}

void MainWindow::GetText(std::string& s)
{
	s = text->GetText();
}

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(ID_RUN, MainWindow::OnRun)
wxEND_EVENT_TABLE()