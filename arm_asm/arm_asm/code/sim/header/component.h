#pragma once

/*
	component.h

	Header file for Component class.
	This class is abstract and used as a parent class for other classes.
*/

#include <iostream>

#include "../../sim/header/controlbus.h"
#include "../../sim/header/addressbus.h"
#include "../../sim/header/databus.h"

/* Class definition */
template<typename T>
class Component
{
public:
	Component();
	Component(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d);
	~Component();

	virtual void Tick() = 0;
	virtual void Print() = 0;
	virtual void Reset() = 0;

protected:
	typename ControlBus<T>* controlBus;
	typename AddressBus<T>* addressBus;
	typename DataBus<T>* dataBus;
};

/* Function definitions */

// Default constructor
template<typename T>
inline Component<T>::Component()
{
}

// Constructor
template<typename T>
inline Component<T>::Component(ControlBus<T>& c, AddressBus<T>& a, DataBus<T>& d)
{
	controlBus = &c;
	addressBus = &a;
	dataBus = &d;
}

// Destructor - needs implementation despite being pure virtual
template<typename T>
inline Component<T>::~Component()
{
}
