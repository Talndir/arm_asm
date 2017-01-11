#pragma once

/*
	controlbus.h

	Header file for ControlBus class.
	Inherits from Bus.
*/

#include "../../sim/header/bus.h"		// Inlcuding base class

/* Forward declarations for friending */
template<typename T> class Decoder;

/* Class definition */
template<typename T>
class ControlBus : public Bus<T>
{
	friend class Decoder<T>;
};