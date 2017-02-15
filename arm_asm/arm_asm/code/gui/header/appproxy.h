#pragma once

#include "../../gui/gui_include.h"

class AppProxy : public wxApp
{
public:
	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v) = 0;
	virtual void RunProgram() = 0 ;
};