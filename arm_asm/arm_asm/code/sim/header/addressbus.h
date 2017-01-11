#pragma once

/*
	addressbus.h

	Header file for AddressBus class.
	Inherits from Bus.
*/

#include "../../sim/header/bus.h"		// Inlcuding base class

/* Forward declarations for friending */
template<typename T> class Decoder;

/* Class definition */
template<typename T>
class AddressBus : public Bus<T>
{
	friend class Decoder<T>;
};