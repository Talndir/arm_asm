#pragma once

/*
	databus.h

	Header file for DataBus class.
	Inherits from Bus.
*/

#include "../../sim/header/bus.h"		// Inlcuding base class

/* Forward declarations for friending */
template<typename T> class Decoder;
template<typename T> class ALU;
template<typename T> class RAM;
template<typename T> class RegisterFile;

/* Class definition */
template<typename T>
class DataBus : public Bus<T>
{
	friend class Decoder<T>;
	friend class ALU<T>;
	friend class RAM<T>;
	friend class RegisterFile<T>;
};