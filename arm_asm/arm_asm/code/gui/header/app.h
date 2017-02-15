#pragma once

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"
#include "../../gui/header/mainwindow.h"

#include "../../sim/header/computer.h"

#include "../../io/io.h"

class App : public AppProxy
{
public:
	virtual bool OnInit();

	virtual void RunProgram();

	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v);

private:
	MainWindow* mainWindow;
	Computer<uint16_t> computer;
};