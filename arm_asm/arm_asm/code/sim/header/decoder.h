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

	void Read();
	void Decode();
	void Execute();

private:
	Register<uint32_t> cir;
	std::vector<Instruction> microcode;
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

		temp.control = 0x01C0;
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

	}
}

// Executes decoded instruction
template<typename T>
inline void Decoder<T>::Execute()
{
}
