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
	void Execute();

private:
	Register<T> r0;
	Register<T> r1;
	Register<T> acc;
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
	}
}

// Executes operation specified by value on control bus
template<typename T>
inline void ALU<T>::Execute()
{
	if ((uint8_t)((controlBus->Get() & 0x0F00) >> 8) == 0b0001)
	{
		switch ((uint8_t)(controlBus->Get() & 0x003F))
		{
		case 0x00:
			acc.Set(r0.Get() + r1.Get());
			break;
		case 0x01:
			acc.Set(r0.Get() - r1.Get());
			break;
		case 0x02:
			acc.Set(r0.Get() & r1.Get());
			break;
		case 0x03:
			acc.Set(r0.Get() | r1.Get());
			break;
		case 0x04:
			acc.Set(!r0.Get());
			break;
		case 0x05:
			acc.Set(~r0.Get());
			break;
		case 0x06:
			acc.Set(r0.Get() ^ r1.Get());
			break;
		case 0x07:
			acc.Set(r0.Get() << r1.Get());
			break;
		case 0x08:
			acc.Set(r0.Get() >> r1.Get());
			break;
		case 0x09:
			acc.Set(r0.Get() % r1.Get());
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
		}

		switch ((uint8_t)((controlBus->Get() & 0x00C0) >> 6))
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
	}
}
