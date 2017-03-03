#pragma once

/*
appproxy.h

Header file for AppProxy class.
Inherits from wxApp.
Abstract class.
*/

#include "../../gui/gui_include.h"		// Inlcuding base gui includes

class AppProxy : public wxApp
{
public:
	virtual void Compile() = 0;
	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v, int page) = 0;
	virtual void GetRegisterFile(std::vector<uint16_t>& v) = 0;
	virtual void GetDecoder(std::vector<uint16_t>& v) = 0;
	virtual void GetRAM(std::vector<uint16_t>& v) = 0;
	virtual void GetALU(std::vector<uint16_t>& v) = 0;
	virtual void GetBuses(std::vector<uint16_t>& v) = 0;

	virtual int GetLine() = 0;

	virtual void UpdateTimer(wxCommandEvent& event) = 0;
	virtual void UpdateLogic() = 0;
};