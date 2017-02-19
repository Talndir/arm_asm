#pragma once

#include "../../gui/gui_include.h"

#include "../../gui/header/appproxy.h"

class TickTimer : public wxTimer
{
public:
	TickTimer(AppProxy* proxy);

	void Start(int t);

private:
	void Notify();

	AppProxy* parent;
};