#pragma once

#include "../../gui/gui_include.h"

class AppProxy : public wxApp
{
public:
	virtual void RunProgram() = 0;
	virtual void GetVDU(std::vector<std::vector<uint8_t>>& v) = 0;
	virtual void GetRegisterFile(std::vector<uint16_t>& v) = 0;
	virtual void GetDecoder(std::vector<uint16_t>& v) = 0;
	virtual void GetRAM(std::vector<uint16_t>& v) = 0;
	virtual void GetALU(std::vector<uint16_t>& v) = 0;
	
	virtual int GetLine() = 0;

	virtual void RunSingle() = 0;

	virtual void Update() = 0;
};