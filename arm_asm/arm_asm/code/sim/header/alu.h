#pragma once

/*
	alu.h

	Header file for ALU class.
	Arithmetic and logic unit responsible for all arithmetic and logical operations.
	Inherits Component.

	TODO: Implement Execute function.
*/

#include "../../sim/header/component.h"
#include "../../sim/header/register.h"

/* Class definition */
template<typename T>
class ALU : public Component<T>
{
public:
	ALU();
	ALU(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d);
	~ALU();

	void Read(int n);
	void Write(int n);

	void Tick();
	void Print();
	void Reset();
	void GetData(std::vector<T>& v);

private:
	Register<T> r0;
	Register<T> r1;
	Register<T> acc;
	Register<uint8_t> sr;
};

/* Function definitions */

// Default constructor
template<typename T>
inline ALU<T>::ALU()
{
}

// Constructor
template<typename T>
inline ALU<T>::ALU(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d) : Component<T>(c, a, d)
{
	r0.Set((T)0);
	r1.Set((T)0);
	acc.Set((T)0);
	sr.Set((T)0);
}

// Destructor
template<typename T>
inline ALU<T>::~ALU()
{
}

// Reads value on data bus into register specified by n
template<typename T>
inline void ALU<T>::Read(int n)
{
	if (!n)
		r0.Set(dataBus->Get());
	else
		r1.Set(dataBus->Get());
}

// Writes value of acc onto data bus or an internal register
template<typename T>
inline void ALU<T>::Write(int n)
{
	switch (n)
	{
	case 0:
		r0.Set(acc.Get());
		break;
	case 1:
		r1.Set(acc.Get());
		break;
	case 2:
		dataBus->Set(acc.Get());
		break;
	case 3:
		dataBus->Set(sr.Get());
		break;
	}
}

// Executes operation specified by value on control bus
template<typename T>
inline void ALU<T>::Tick()
{
	if ((uint8_t)((controlBus->Get() & 0x0F00) >> 8) == 0b0001)
	{
		bool z = 0, v = 0, c = 0, s = 0;
		T temp;

		// SWitch on instruction
		switch ((uint8_t)(controlBus->Get() & 0x00FF))
		{
		case 0x00:	// Add
			acc.Set(r0.Get() + r1.Get());
			c = ((acc.Get() & 0x8000) != (r0.Get() & 0x8000)) && (!(acc.Get() & 0x8000));
			v = ((r0.Get() & 0x8000) == (r1.Get() & 0x8000)) && ((acc.Get() & 0x8000) != (r0.Get() & 0x8000));
			break;
		case 0x01:	// Subtract
			acc.Set(r0.Get() - r1.Get());
			c = ((acc.Get() & 0x8000) != (r0.Get() & 0x8000)) && ((acc.Get() & 0x8000));
			v = ((r0.Get() & 0x8000) != (r1.Get() & 0x8000)) && ((acc.Get() & 0x8000) != (r0.Get() & 0x8000));
			break;
		case 0x02:	// Bitwise AND
			acc.Set(r0.Get() & r1.Get());
			break;
		case 0x03:	// Bitwise OR
			acc.Set(r0.Get() | r1.Get());
			break;
		case 0x04:	// Bitwise NOT
			acc.Set(!r0.Get());
			break;
		case 0x05:	// Bitwise complement
			acc.Set(~r0.Get());
			break;
		case 0x06:	// Bitwise XOR
			acc.Set(r0.Get() ^ r1.Get());
			break;
		case 0x07:	// Logical shift left
			acc.Set(r0.Get() << r1.Get());
			break;
		case 0x08:	// Logical shift right
			acc.Set(r0.Get() >> r1.Get());
			break;
		case 0x09:	// Modulo
			acc.Set(r0.Get() % r1.Get());
			break;

		case 0xA0:	// Compare (subtract, update SR, discard result)
			temp = acc.Get();
			acc.Set(r0.Get() - r1.Get());
			c = ((acc.Get() & 0x8000) != (r0.Get() & 0x8000)) && ((acc.Get() & 0x8000));
			v = ((r0.Get() & 0x8000) != (r1.Get() & 0x8000)) && ((acc.Get() & 0x8000) != (r0.Get() & 0x8000));
			break;

		case 0xF0:
			Read(0);
			break;
		case 0xF1:
			Read(1);
			break;
		case 0xF2:
			Write(0);
			break;
		case 0xF3:
			Write(1);
			break;
		case 0xF4:
			Write(2);
			break;
		case 0xF5:
			Write(3);
			break;
		}

		z = !acc.Get();
		s = acc.Get() & 0x8000;

		// Write values to databus if necessary
		switch ((uint8_t)((controlBus->Get() & 0xF000) >> 12))
		{
		case 0:
			break;
		case 1:
			Write(0);
			break;
		case 2:
			Write(1);
			break;
		case 3:
			Write(2);
			break;
		}

		if ((uint8_t)(controlBus->Get() & 0x00FF) < 0xF0)
			sr.Set((sr.Get() & 0xF0) + z + (c << 1) + (v << 2) + (s << 3));
		if ((uint8_t)(controlBus->Get() & 0x00FF) == 0xA0)
			acc.Set(temp);
	}
}

// Prints contents of ALU registers
template<typename T>
inline void ALU<T>::Print()
{
	std::cout << "ALU" << std::endl;
	std::cout << "R0: " << std::hex << r0.Get() << std::endl;
	std::cout << "R1: " << std::hex << r1.Get() << std::endl;
	std::cout << "ACC: " << std::hex << acc.Get() << std::endl;
	std::cout << "SR: " << std::hex << sr.Get() << std::endl;
}

// Resets ALU
template<typename T>
inline void ALU<T>::Reset()
{
	r0.Set((T)0);
	r1.Set((T)0);
	acc.Set((T)0);
	sr.Set((T)0);
}

// Supplies A0, A1, ACC and SR
template<typename T>
inline void ALU<T>::GetData(std::vector<T>& v)
{
	v.push_back(r0.Get());
	v.push_back(r1.Get());
	v.push_back(acc.Get());
	v.push_back(sr.Get());
}
