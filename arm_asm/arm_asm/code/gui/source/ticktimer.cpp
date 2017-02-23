#include "../../gui/header/ticktimer.h"

/*
	ticktimer.cpp

	CPP file for TickTimer class.
	Contains all function definitions.
*/

// Constructor - empty except for calling parent
TickTimer::TickTimer(AppProxy * proxy) : parent(proxy)
{
}

// Override base notify to directly call a function from App
void TickTimer::Notify()
{
	parent->UpdateLogic();
}

// Override base start, since otherwise you need to mess with wxTimer base class back in App which is kinda silly
void TickTimer::Start(int t)
{
	wxTimer::Start(t);
}