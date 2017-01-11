#pragma once

/*
	decoder.h

	Header file for Decoder class.
	Decoder decodes instructions and executes them. Acts as a combined instruction decoder and control unit.
	Inherits Component.

	TODO: Implement registers; constructor; Read, Decode and Execute functions.
*/

#include "../../sim/header/component.h"

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
}

// Executes decoded instruction
template<typename T>
inline void Decoder<T>::Execute()
{
}
