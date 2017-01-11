#pragma once

/*
	bus.h

	Header file for Bus class.
	Templated, so also contains function definitions.
*/

/* Forward declarations for friending */
template<typename T> class Computer;

/* Bus class */
template <typename T>
class Bus
{
	friend class Computer<T>;

public:
	Bus();					// Constructor
	virtual ~Bus() = 0;		// Virtual destructor to make class abstract

	T Get();	// Public read, all components can read from a bus

protected:
	void Set(T v);	// Protected write, only friends can write to a bus

private:
	T value;			// Current value on the bus
};


/* Function definitions */

// Constructor
template<typename T>
inline Bus<T>::Bus()
{
	value = (T)0;	// Sets value to 0
}

// Destructor
template<typename T>
Bus<T>::~Bus()
{
}

// Read value on bus
template<typename T>
T Bus<T>::Get()
{
	return value;
}

// Write value to bus
template<typename T>
void Bus<T>::Set(T v)
{
	value = v;
}