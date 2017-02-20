#pragma once

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"
#include "../../gui/header/mainwindow.h"
#include "../../gui/header/ticktimer.h"

#include "../../sim/header/computer.h"

#include "../../io/io.h"

class App : public AppProxy
{
public:
	virtual bool OnInit();

	virtual void Compile();

	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v);
	virtual void GetRegisterFile(std::vector<uint16_t>& v);
	virtual void GetDecoder(std::vector<uint16_t>& v);
	virtual void GetRAM(std::vector<uint16_t>& v);
	virtual void GetALU(std::vector<uint16_t>& v);

	virtual int GetLine();
	
	void RunSingle();
	void RunMicro();

	virtual void Update();

private:
	MainWindow* mainWindow;
	TickTimer* timer;
	Computer<uint16_t> computer;
	std::map<int, int> memoryToLine;
};