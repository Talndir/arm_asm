#pragma once

/*
	app.h

	Header file for App class.
	Inherits from AppProxy.
*/

#include "../../gui/gui_include.h"			// Inlcuding base gui includes

#include "../../gui/header/appproxy.h"		// Base class
#include "../../gui/header/mainwindow.h"	// MainWindow class
#include "../../gui/header/ticktimer.h"		// Timer class for time-based functions in App

#include "../../sim/header/computer.h"		// Computer class

#include "../../io/io.h"					// IO for reading files and parsing code

/* Class definition */
class App : public AppProxy
{
public:
	virtual bool OnInit();

	virtual void Compile();

	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v, int page);
	virtual void GetRegisterFile(std::vector<uint16_t>& v);
	virtual void GetDecoder(std::vector<uint16_t>& v);
	virtual void GetRAM(std::vector<uint16_t>& v);
	virtual void GetALU(std::vector<uint16_t>& v);
	virtual void GetBuses(std::vector<uint16_t>& v);

	virtual int GetLine();
	
	void RunSingle();
	void RunMicro(bool& c, bool& a, bool &d);

	virtual void UpdateTimer(wxCommandEvent& event);
	virtual void UpdateLogic();

private:
	MainWindow* mainWindow;
	TickTimer* timer;
	Computer<uint16_t> computer;
	std::map<int, int> memoryToLine;
};