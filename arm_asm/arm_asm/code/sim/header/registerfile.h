#pragma once

/*
	registerfile.h

	Header file for RegisterFile class.
	Contains a set of registers.
	Inherits Component.
*/

#include <vector>

#include "../../sim/header/component.h"
#include "../../sim/header/register.h"

/* Class definition */
template<typename T>
class RegisterFile : public Component<T>
{
public:
	RegisterFile();
	RegisterFile(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d, int n);
	~RegisterFile();

	void Read();
	void Write();
	void Execute();

private:
	std::vector<T> registers;
};

/* Function definitions */

// Default constructor
template<typename T>
inline RegisterFile<T>::RegisterFile()
{
}

// Constructor - takes number of registers and creates vector of them
template<typename T>
inline RegisterFile<T>::RegisterFile(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d, int n) : Component<T>(c, a, d)
{
	registers = std::vector<T>(n, (T)0);
}

// Destructor
template<typename T>
inline RegisterFile<T>::~RegisterFile()
{
}

// Reads register specified on address bus and places value on data bus
template<typename T>
inline void RegisterFile<T>::Read()
{
	dataBus->Set(registers.at(addressBus->Get()));
}

// Writes value on data bus to register specified on address bus
template<typename T>
inline void RegisterFile<T>::Write()
{
	registers.at(addressBus->Get()) = dataBus->Get();
}

// Executes operation specified by value on control bus
template<typename T>
inline void RegisterFile<T>::Execute()
{
	if ((uint8_t)((controlBus->Get() & 0x0F00) >> 8) == 0b0010)
	{
		switch ((uint8_t)(controlBus->Get() & 0x00FF))
		{
		case 0x00:
			Read();
			break;
		case 0x01:
			Write();
			break;
		}
	}
}
