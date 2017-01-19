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

// Run the program
template<typename T>
inline void Computer<T>::Run()
{
}

// Tick all systems in computer at once
template<typename T>
inline void Computer<T>::Tick()
{
	alu.Tick();
	ram.Tick();
	registerFile.Tick();
}
