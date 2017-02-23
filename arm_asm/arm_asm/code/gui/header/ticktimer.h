#pragma once

/*
	timer.h

	Header file for Timer class.
	Inherits from wxTimer.
	Used purely for App tick timer.

	This class is pretty redundant, it's better not to override the Notify() function with
	a custom subclass and better to just set owner and connect to a callback function like
	with MainWindow.
*/

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"

/* Class definition */
class TickTimer : public wxTimer
{
public:
	TickTimer(AppProxy* proxy);

	void Start(int t);

private:
	void Notify();

	AppProxy* parent;
};