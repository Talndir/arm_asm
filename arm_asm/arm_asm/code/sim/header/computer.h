#pragma once

/*
	computer.h

	Header file for Computer class.
	Contains entire computer including CPU and memory.
*/

#include "../../sim/header/controlbus.h"
#include "../../sim/header/addressbus.h"
#include "../../sim/header/databus.h"
#include "../../sim/header/alu.h"
#include "../../sim/header/decoder.h"
#include "../../sim/header/ram.h"
#include "../../sim/header/registerfile.h"

/* Class definition */
template<typename T>
class Computer
{
public:
	Computer();
	~Computer();

	void Read(std::vector<uint32_t>& as);
	void Run();
	void Tick();

private:
	ControlBus<T> controlBus;
	AddressBus<T> addressBus;
	DataBus<T> dataBus;
	ALU<T> alu;
	Decoder<T> decoder;
	RAM<T> ram;
	RegisterFile<T> registerFile;
};

/* Function definitions */

// Constructor - creates all members
template<typename T>
inline Computer<T>::Computer()
{
	controlBus = ControlBus<T>();
	addressBus = AddressBus<T>();
	dataBus = DataBus<T>();

	alu = ALU<T>(controlBus, addressBus, dataBus);
	decoder = Decoder<T>(controlBus, addressBus, dataBus);
	ram = RAM<T>(controlBus, addressBus, dataBus, 1<<16);
	registerFile = RegisterFile<T>(controlBus, addressBus, dataBus, 16);

	decoder.Link(*this);
}

// Destructor
template<typename T>
inline Computer<T>::~Computer()
{
}

template<typename T>
inline void Computer<T>::Read(std::vector<uint32_t>& as)
{
	for (unsigned int i = 0; i < as.size(); ++i)
	{
		ram.memory.at(i * 4 + 0x1000 + 0) = (as.at(i) >> 0) & 0xFF;
		ram.memory.at(i * 4 + 0x1000 + 1) = (as.at(i) >> 8) & 0xFF;
		ram.memory.at(i * 4 + 0x1000 + 2) = (as.at(i) >> 16) & 0xFF;
		ram.memory.at(i * 4 + 0x1000 + 3) = (as.at(i) >> 24) & 0xFF;
	}
}

// Run the program
template<typename T>
inline void Computer<T>::Run()
{
	decoder.Fetch();

	while (decoder.cir.Get())
	{
		decoder.Decode();
		decoder.Execute();
		decoder.Fetch();

		//ram.PrintVDU();
		//registerFile.Print();
	}

	ram.PrintVDU();
	registerFile.Print();
}

// Tick all systems in computer at once
template<typename T>
inline void Computer<T>::Tick()
{
	decoder.Tick();
	alu.Tick();
	ram.Tick();
	registerFile.Tick();
}
