#include "../../gui/header/ticktimer.h"

TickTimer::TickTimer(AppProxy * proxy) : parent(proxy)
{
}

void TickTimer::Notify()
{
	parent->Update();
}

void TickTimer::Start(int t)
{
	wxTimer::Start(t);
}