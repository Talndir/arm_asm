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

#include "../../sim/header/component.h"
#include "../../sim/header/instruction.h"

/* Concatenate two vectors, result stored in a */
template<typename T>
void CombineVectors(std::vector<T>& a, std::vector<T>& b)
{
	a.reserve(a.size() + b.size());
	a.insert(a.end(), b.begin(), b.end());
}

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
	void SwapBuses();
	void SaveData(int b);
	void LoadData(int b);

	void Tick();
	void Print();

	Register<uint32_t> cir;

private:
	//Register<uint32_t> cir;
	Register<T> temp;
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
	microcode.clear();

	// OORRFABC

	uint8_t opcode = (cir.Get() >> 24) & 0xFF;
	uint8_t operand1 = (cir.Get() >> 20) & 0x0F;
	uint8_t operand2 = (cir.Get() >> 16) & 0x0F;
	uint8_t flags = (cir.Get() >> 12) & 0x0F;
	uint8_t a = (cir.Get() >> 8) & 0x0F;
	uint8_t b = (cir.Get() >> 4) & 0x0F;
	uint8_t c = cir.Get() & 0x0F;

	std::vector<Instruction> op3;

	switch (flags)
	{
	case 0x01:
		op3.push_back(Instruction(0, 0, (a << 8) + (b << 4) + c));
		break;
	case 0x02:
		op3.push_back(Instruction(0, 0, (c << b) + a));
		break;
	case 0x03:
		op3.push_back(Instruction(0x0200, a, 0));
		break;
	case 0x04:
		op3.push_back(Instruction(0x0200, a, 0));
		op3.push_back(Instruction(0x01F0, 0, 0xFFFF));
		op3.push_back(Instruction(0x01F1, 0, (b << 4) + c));
		op3.push_back(Instruction(0x3100, 0, 0));
		break;
	case 0x05:
		break;
	case 0x06:
		break;
	}

	switch (opcode)
	{
	case 0x00:	// MOV Rn, O3
		CombineVectors(microcode, op3);		// Get O3
		StoreRegister(operand1);			// Store in Rn
		break;
	case 0x10:	// STR Rn, O3
		CombineVectors(microcode, op3);		// Get O3
		SaveData(2);						// Save O3
		GetRegister(operand1);				// Get Rn
		LoadData(1);						// Put O3 onto address bus
		microcode.push_back(Instruction(0x0401, 0xFFFF, 0xFFFF));	// Store in memory
		break;
	case 0x20:	// ADD Rn, Rm, O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		CombineVectors(microcode, op3);		// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3100, 0x0000, 0x0000));	// Add A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	}
	
	/*
	microcode.push_back(Instruction(0x0201, 0x0000, 0x000A));
	microcode.push_back(Instruction(0x0201, 0x0001, 0x0014));
	microcode.push_back(Instruction(0x0200, 0x0000, 0x0000));
	microcode.push_back(Instruction(0x01F0, 0x0000, 0xFFFF));
	microcode.push_back(Instruction(0x0200, 0x0001, 0x0000));
	microcode.push_back(Instruction(0x01F1, 0x0000, 0xFFFF));
	microcode.push_back(Instruction(0x3100, 0x0000, 0x0000));
	microcode.push_back(Instruction(0x0401, 0x0005, 0xFFFF));
	*/
}

// Executes decoded instruction
template<typename T>
inline void Decoder<T>::Execute()
{
	for (unsigned int i = 0; i < microcode.size(); ++i)
	{
		if (microcode.at(i).control != 0xFFFF) controlBus->Set(microcode.at(i).control);
		if (microcode.at(i).address != 0xFFFF) addressBus->Set(microcode.at(i).address);
		if (microcode.at(i).data != 0xFFFF) dataBus->Set(microcode.at(i).data);

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
	i.control = 0x01F0 + n;
	i.address = 0xFFFF;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - adds microcode to swap contents of address and data buses
template<typename T>
inline void Decoder<T>::SwapBuses()
{
	Instruction i;
	i.control = 0x0800;
	i.address = 0xFFFF;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - saves contents of data bus to a temp register
template<typename T>
inline void Decoder<T>::SaveData(int b)
{
	Instruction i;
	i.control = 0x0810 + b;
	i.address = 0xFFFF;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

// Helper function - loads contents of temp register into data bus
template<typename T>
inline void Decoder<T>::LoadData(int b)
{
	Instruction i;
	i.control = 0x0820 + b;
	i.address = 0xFFFF;
	i.data = 0xFFFF;
	microcode.push_back(i);
}

template<typename T>
inline void Decoder<T>::Tick()
{
	if ((uint8_t)((controlBus->Get() & 0x0F00) >> 8) == 0b1000)
	{
		T t;

		switch ((uint8_t)(controlBus->Get() & 0x00FF))
		{
		case 0x00:
			t = addressBus->Get();
			addressBus->Set(dataBus->Get());
			dataBus->Set(t);
			break;
		case 0x10:
			temp.Set(controlBus->Get());
			break;
		case 0x11:
			temp.Set(addressBus->Get());
			break;
		case 0x12:
			temp.Set(dataBus->Get());
			break;
		case 0x20:
			controlBus->Set(temp.Get());
			break;
		case 0x21:
			addressBus->Set(temp.Get());
			break;
		case 0x22:
			dataBus->Set(temp.Get());
			break;
		}
	}
}

// Print internal registers
template<typename T>
inline void Decoder<T>::Print()
{
	std::cout << "Decoder" << std::endl;
	std::cout << "CIR: " << std::hex << cir.Get() << std::endl;
}
