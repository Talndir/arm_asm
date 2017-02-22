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

/* Class definition */
template<typename T>
class Decoder : public Component<T>
{
	friend Computer<T>;

public:
	Decoder();
	Decoder(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d);
	~Decoder();

	void Link(Computer<T>& c);

	void Fetch();
	void Decode();
	void Execute();

	void RunSingle(bool& c, bool& a, bool& d, bool& again);

	void GetRegister(int n);
	void GetMemory(int n);
	void StoreRegister(int n);
	void StoreMemory(int n);
	void SendToALU(int n);
	void SwapBuses();
	void SaveData(int b);
	void LoadData(int b);
	void GetOp3(std::vector<Instruction>& v);

	void Tick();
	void Print();
	void Reset();
	void GetData(std::vector<T>& v);

private:
	Register<uint32_t> cir;
	Register<T> pc;
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
	cir.Set(1);
	pc.Set(0x1000);
	temp.Set(0);
	microcode.clear();
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
inline void Decoder<T>::Fetch()
{
	microcode.clear();

	microcode.push_back(Instruction(0x0400, pc.Get(), 0xFFFF));
	microcode.push_back(Instruction(0x0812, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0830, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0400, pc.Get() + 1, 0xFFFF));
	microcode.push_back(Instruction(0x0812, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0831, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0400, pc.Get() + 2, 0xFFFF));
	microcode.push_back(Instruction(0x0812, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0832, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0400, pc.Get() + 3, 0xFFFF));
	microcode.push_back(Instruction(0x0812, 0xFFFF, 0xFFFF));
	microcode.push_back(Instruction(0x0833, 0xFFFF, 0xFFFF));

	this->Execute();

	pc.Set(pc.Get() + 4);
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
		op3.push_back(Instruction(0x08FF, 0, (a << 8) + (b << 4) + c));
		break;
	case 0x02:
		op3.push_back(Instruction(0x08FF, 0, (c << b) + a));
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
	case 0x00:	// 0x00:	MOV Rn, O3			Rn <- O3
		GetOp3(op3);						// Get O3
		StoreRegister(operand1);			// Store in Rn
		break;

	case 0x10:	// 0x10:	STR Rn, O3			[O3] <- Rn
		GetOp3(op3);						// Get O3
		SaveData(2);						// Save O3
		GetRegister(operand1);				// Get Rn
		LoadData(1);						// Put O3 onto address bus
		microcode.push_back(Instruction(0x0401, 0xFFFF, 0xFFFF));	// Store in memory
		break;
	case 0x11:	// 0x11 :	LDR Rn, O3			Rn <- [O3]
		GetOp3(op3);						// Get O3
		SwapBuses();						// Move O3 to address bus
		microcode.push_back(Instruction(0x0400, 0xFFFF, 0xFFFF));	// Get [O3]
		StoreRegister(operand1);			// Store in Rn
		break;

	case 0x20:	// 0X20:	ADD Rn, Rm, O3		Rn <- Rm + O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3100, 0x0000, 0x0000));	// Add A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x21:	// 0X21 : SUB Rn, Rm, O3		Rn <-Rm - O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3101, 0x0000, 0x0000));	// Sub A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x22:	// 0x22:	MOD Rn, Rm, O3		Rn <- Rm % O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3109, 0x0000, 0x0000));	// Calculate A0 % A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x23:	// 0x23 : LSL Rn, Rm, O3		Rn <-Rm << O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3107, 0x0000, 0x0000));	// Calculate A0 << A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x24:	// 0x24 : LSR Rn, Rm, O3		Rn <-Rm >> O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3108, 0x0000, 0x0000));	// Calculate A0 >> A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x25:	// 0x25 : AND	Rn, Rm, O3		Rn <-Rm & O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3102, 0x0000, 0x0000));	// AND A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x26:	// 0x26:	ORR Rn, Rm, O3		Rn <- Rm | O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3103, 0x0000, 0x0000));	// OR A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x27:	// 0x27 : NOT Rn, O3			Rn <-!O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3104, 0x0000, 0x0000));	// NOT A0 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x28:	// 0x28 : XOR	Rn, Rm, O3		Rn <-Rm ^ O3
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3106, 0x0000, 0x0000));	// XOR A0, A1 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;
	case 0x29:	// 0x29:	NEG Rn, Rm			Rn <- ~Rm
		GetRegister(operand2);				// Get Rm
		SendToALU(0);						// Read Rm into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x3105, 0x0000, 0x0000));	// Take complement of A0 and deposit onto data bus
		StoreRegister(operand1);			// Store result in Rn
		break;

	case 0x30:	// 0x30:	CMP Rn, O3			COMPARE Rn, O3
		GetRegister(operand1);				// Get Rn
		SendToALU(0);						// Read Rn into A0
		GetOp3(op3);						// Get O3
		SendToALU(1);						// Read O3 into A1
		microcode.push_back(Instruction(0x01A0, 0x0000, 0x0000));	// Perform comparison (A0 - A1, no result except SR update)
		break;

	case 0x31:	// 0x31:	JMP :LABEL			BRANCH UNCONDITIONAL	(BACKWARD)
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0840, 0x0000, 0xFFFF));	// Unconditional branch (PC <- PC - O3)
		break;
	case 0x32:	// 0x32:	BEQ :LABEL			BRANCH IF Rn == Rm
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0844, 0x0000, 0xFFFF));	// Branch if ZERO flag is not set
		break;
	case 0x33:	// 0x33:	BNE :LABEL			BRANCH IF Rn != Rm		(BACKWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0842, 0x0000, 0xFFFF));	// Branch if ZERO flag is set
		break;
	case 0x34:	// 0x34:	BGT :LABEL			BRANCH IF Rn > Rm		(BACKWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0846, 0x0000, 0xFFFF));	// Branch if CARRY flag is not set and ZERO flag is not set
		break;
	case 0x35:	// 0x35:	BLT :LABEL			BRANCH IF Rn < Rm		(BACKWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0848, 0x0000, 0xFFFF));	// Branch if CARRY flag is set
		break;

	case 0x41:	// 0x31:	JMP :LABEL			BRANCH UNCONDITIONAL	(FORWARD)
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0841, 0x0000, 0xFFFF));	// Unconditional branch (PC <- PC - O3)
		break;
	case 0x42:	// 0x32:	BEQ :LABEL			BRANCH IF Rn == Rm
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0845, 0x0000, 0xFFFF));	// Branch if ZERO flag is not set
		break;
	case 0x43:	// 0x33:	BNE :LABEL			BRANCH IF Rn != Rm		(FORWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0843, 0x0000, 0xFFFF));	// Branch if ZERO flag is set
		break;
	case 0x44:	// 0x34:	BGT :LABEL			BRANCH IF Rn > Rm		(FORWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0847, 0x0000, 0xFFFF));	// Branch if CARRY flag is not set and ZERO flag is not set
		break;
	case 0x45:	// 0x35:	BLT :LABEL			BRANCH IF Rn < Rm		(FORWARD)
		microcode.push_back(Instruction(0x01F5, 0x0000, 0x0000));	// Get SR
		SaveData(2);						// Save SR
		GetOp3(op3);						// Get O3
		microcode.push_back(Instruction(0x0849, 0x0000, 0xFFFF));	// Branch if CARRY flag is set
		break;
	}
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

template<typename T>
inline void Decoder<T>::RunSingle(bool& c, bool& a, bool& d, bool& again)
{
	if (!microcode.size())
	{
		Fetch();
		Decode();
	}

	again = false;

	if (microcode.at(0).control == 0x08FF)
		again = true;

	c = a = d = false;

	if (microcode.at(0).control != 0xFFFF) { controlBus->Set(microcode.at(0).control); c = true; }
	if (microcode.at(0).address != 0xFFFF) { addressBus->Set(microcode.at(0).address); a = true; }
	if (microcode.at(0).data != 0xFFFF) { dataBus->Set(microcode.at(0).data); d = true; }

	computer->Tick();
	microcode.erase(microcode.begin());
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
inline void Decoder<T>::GetOp3(std::vector<Instruction>& v)
{
	microcode.reserve(microcode.size() + v.size());
	microcode.insert(microcode.end(), v.begin(), v.end());
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
		case 0x30:
			cir.Set(temp.Get());
			break;
		case 0x31:
			cir.Set(cir.Get() + (temp.Get() << 8));
			break;
		case 0x32:
			cir.Set(cir.Get() + (temp.Get() << 16));
			break;
		case 0x33:
			cir.Set(cir.Get() + (temp.Get() << 24));
			break;
		case 0x40:
			pc.Set(pc.Get() - (dataBus->Get() + 4));
			break;
		case 0x41:
			pc.Set(pc.Get() + (dataBus->Get() - 4));
			break;
		case 0x42:
			if (!(temp.Get() & 0x01))
				pc.Set(pc.Get() - (dataBus->Get() + 4));
			break;
		case 0x43:
			if (!(temp.Get() & 0x01))
				pc.Set(pc.Get() + (dataBus->Get() - 4));
			break;
		case 0x44:
			if (temp.Get() & 0x01)
				pc.Set(pc.Get() - (dataBus->Get() + 4));
			break;
		case 0x45:
			if (temp.Get() & 0x01)
				pc.Set(pc.Get() + (dataBus->Get() - 4));
			break;
		case 0x46:
			if (!(temp.Get() & 0x02) && !(temp.Get() & 0x01))
				pc.Set(pc.Get() - (dataBus->Get() + 4));
			break;
		case 0x47:
			if (!(temp.Get() & 0x02) && !(temp.Get() & 0x01))
				pc.Set(pc.Get() + (dataBus->Get() - 4));
			break;
		case 0x48:
			if (temp.Get() & 0x02)
				pc.Set(pc.Get() - (dataBus->Get() + 4));
			break;
		case 0x49:
			if (temp.Get() & 0x02)
				pc.Set(pc.Get() + (dataBus->Get() - 4));
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

template<typename T>
inline void Decoder<T>::Reset()
{
	cir.Set(1);
	pc.Set(0x1000);
	temp.Set(0);
	microcode.clear();
}

template<typename T>
inline void Decoder<T>::GetData(std::vector<T>& v)
{
	v.push_back(pc.Get());
	v.push_back((cir.Get() >> 0) & 0xFF);
	v.push_back((cir.Get() >> 8) & 0xFF);
	v.push_back((cir.Get() >> 16) & 0xFF);
	v.push_back((cir.Get() >> 24) & 0xFF);
}
