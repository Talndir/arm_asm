#pragma once

/*
	decoder.h

	Header file for Decoder class.
	Decoder decodes instructions and executes them. Acts as a combined instruction decoder and control unit.
	Inherits Component.

	TODO:
	Read function.
	Decode function: complete opcode 3 parsing and write opcode to microcode translator.
	Execute function.
	Helper functions for decoding.
*/

#include <vector>

//#include "../../sim/header/computer.h"
#include "../../sim/header/component.h"
#include "../../sim/header/instruction.h"

/* Class definition */
template<typename T>
class Decoder : public Component<T>
{
public:
	Decoder();
	Decoder(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d);
	~Decoder();

	void Link(Computer<T>& c);

	void Read();
	void Decode();
	void Execute();

	void GetRegister(int n);
	void GetMemory(int n);
	void StoreRegister(int n);
	void StoreMemory(int n);
	void SendToALU(int n);

private:
	Register<uint32_t> cir;
	std::vector<Instruction> microcode;
	Computer<T>* computer;
};

/* Function definitions */

// Default constructor
template<typename T>
inline Decoder<T>::Decoder()
{
}

// Constructor
template<typename T>
inline Decoder<T>::Decoder(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d) : Component<T>(c, a, d)
{
}

// Destructor
template<typename T>
inline Decoder<T>::~Decoder()
{
}

// Links containing computer for program execution
template<typename T>
inline void Decoder<T>::Link(Computer<T>& c)
{
	computer = &c;
}

// Reads next instruction
template<typename T>
inline void Decoder<T>::Read()
{
}

// Decodes current instruction
template<typename T>
inline void Decoder<T>::Decode()
{
	uint8_t opcode = (cir.Get() >> 24) & 0xFF;
	uint8_t operand1 = (cir.Get() >> 20) & 0x0F;
	uint8_t operand2 = (cir.Get() >> 16) & 0x0F;
	uint8_t flags = (cir.Get() >> 12) & 0x0F;
	uint8_t a = (cir.Get() >> 8) & 0x0F;
	uint8_t b = (cir.Get() >> 4) & 0x0F;
	uint8_t c = cir.Get() & 0x0F;

	Instruction temp;
	std::vector<Instruction> op3;

	switch (flags)
	{
	case 0x01:
		temp.control = 0;
		temp.address = 0;
		temp.data = (a << 8) + (b << 4) + c;
		op3.push_back(temp);
		break;
	case 0x02:
		temp.control = 0;
		temp.address = 0;
		temp.data = (c << b) + a;
		op3.push_back(temp);
		break;
	case 0x03:
		temp.control = 0x0200;
		temp.address = a;
		temp.data = 0;
		op3.push_back(temp);
		break;
	case 0x04:
		temp.control = 0x0200;
		temp.address = a;
		temp.data = 0;
		op3.push_back(temp);
		
		temp.control = 0x01F0;
		temp.address = 0;
		temp.data = 0xFFFF;
		op3.push_back(temp);

		temp.control = 0x01F1;
		temp.address = 0;
		temp.data = (b << 4) + c;
		op3.push_back(temp);

		temp.control = 0x3100;
		temp.address = 0;
		temp.data = 0;
		op3.push_back(temp);
		break;
	case 0x05:
		break;
	case 0x06:
		break;
	}

	switch (opcode)
	{
	default:
		break;
	}
}

// Executes decoded instruction
template<typename T>
inline void Decoder<T>::Execute()
{
	for (unsigned int i = 0; i < microcode.size(); ++i)
	{
		if (microcode.at(i).control != 0xFFFF) dataBus.Set(microcode.at(i).control);
		if (microcode.at(i).control != 0xFFFF) dataBus.Set(microcode.at(i).address);
		if (microcode.at(i).control != 0xFFFF) dataBus.Set(microcode.at(i).data);

		computer->Tick();
	}
}

// Helper function - adds microcode to retrieve contents of Rn from register file
template<typename T>
inline void Decoder<T>::GetRegister(int n)
{
	Instruction i;
	i.control = 0x0200;
	i.address = n;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - adds microcode to retrieve contents of address n from RAM
template<typename T>
inline void Decoder<T>::GetMemory(int n)
{
	Instruction i;
	i.control = 0x0400;
	i.address = n;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - adds microcode to store value in Rn in register file
template<typename T>
inline void Decoder<T>::StoreRegister(int n)
{
	Instruction i;
	i.control = 0x0201;
	i.address = n;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - adds microcode to store value in address n of RAM
template<typename T>
inline void Decoder<T>::StoreMemory(int n)
{
	Instruction i;
	i.control = 0x0401;
	i.address = n;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - adds microcode for ALU to read data bus into internal register n
template<typename T>
inline void Decoder<T>::SendToALU(int n)
{
	Instruction i;
	i.control = 0x08F0 + n;
	i.address = 0xFFFF;
	i.data = 0xFFFF;
	microcode.push_back(i);
}
