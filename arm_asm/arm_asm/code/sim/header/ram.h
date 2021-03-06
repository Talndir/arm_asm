#pragma once

/*
	ram.h

	Header file for RAM class.
	Contains a large set of registers.
	Inherits Component.
*/

#include <vector>
#include <Windows.h>
#include <iostream>

#include "../../sim/header/component.h"

/* Class definition */
template<typename T>
class RAM : public Component<T>
{
	friend Computer<T>;

public:
	RAM();
	RAM(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d, int n);
	~RAM();

	void Load();
	void Store();

	void Tick();
	void Print();
	void Reset();
	void GetData(std::vector<T>& v);
	
	void PrintVDU();

	void GetVDU(std::vector<std::vector<uint8_t>>& v, int page);

private:
	std::vector<uint8_t> memory;
};

/* Function definitions */

// Default constructor
template<typename T>
inline RAM<T>::RAM()
{
}

// Constructor - takes in size of memory
template<typename T>
inline RAM<T>::RAM(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d, int n) : Component<T>(c, a, d)
{
	memory = std::vector<uint8_t>(n, 0);
}

// Destructor
template<typename T>
inline RAM<T>::~RAM()
{
}

// Copy value of address specified by address bus to data bus
template<typename T>
inline void RAM<T>::Load()
{
	dataBus->Set(memory.at(addressBus->Get()));
}

// Copy value on data bus to address specified by address bus
template<typename T>
inline void RAM<T>::Store()
{
	memory.at(addressBus->Get()) = dataBus->Get();
}

// Executes operation specified by value on control bus
template<typename T>
inline void RAM<T>::Tick()
{
	if ((uint8_t)((controlBus->Get() & 0x0F00) >> 8) == 0b0100)
	{
		switch ((uint8_t)(controlBus->Get() & 0x00FF))
		{
		case 0x00:
			Load();
			break;
		case 0x01:
			Store();
			break;
		}
	}
}

// Prints contents of RAM
template<typename T>
inline void RAM<T>::Print()
{
	std::cout << "RAM" << std::endl;

	for (unsigned int i = 0; i < memory.size(); ++i)
	{
		std::cout << std::hex << memory.at(i);
		
		if ((i + 1) % 16)
			std::cout << std::endl;
		else
			std::cout << " " << std::endl;
	}
}

template<typename T>
inline void RAM<T>::Reset()
{
	std::fill(memory.begin(), memory.end(), (uint8_t)0);
}

template<typename T>
inline void RAM<T>::GetData(std::vector<T>& v)
{
	for (unsigned int i = 0; i < memory.size(); ++i)
		v.push_back(memory.at(i));
}

// Prints VDU to console (first 256 memory locations)
template<typename T>
inline void RAM<T>::PrintVDU()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (unsigned int i = 0; i < 16; ++i)
	{
		for (unsigned int j = 0; j < 16; ++j)
		{
			uint8_t value = memory.at(j + (i << 4));
			SetConsoleTextAttribute(hConsole, value << 4);
			std::cout << " ";
		}

		std::cout << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 15);
}

// Pass back contents of VDU, which is just memory locations 0x00 to 0xFF
template<typename T>
inline void RAM<T>::GetVDU(std::vector<std::vector<uint8_t>>& v, int page)
{
	v.clear();

	std::vector<uint8_t> r;

	for (unsigned int i = 0; i < 16; ++i)
	{
		for (unsigned int j = 0; j < 16; ++j)
			r.push_back(memory.at(page * 256 + i * 16 + j));

		v.push_back(r);
		r.clear();
	}
}
