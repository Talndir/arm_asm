#pragma once

/*
	register.h

	Header file for Register class.
	Simple single value container with get/set functions.
*/

/* Class definition */
template<typename T>
class Register
{
public:
	Register();
	~Register();

	typename T Get();
	void Set(typename T v);

	void Reset();

private:
	T value;
};

/* Function definitions */

// Constructor
template<typename T>
inline Register<T>::Register()
{
	value = (T)0;
}

// Destructor
template<typename T>
inline Register<T>::~Register()
{
}

// Get function - returns value of register
template<typename T>
inline typename T Register<T>::Get()
{
	return value;
}

// Set function - sets value of register
template<typename T>
inline void Register<T>::Set(typename T v)
{
	value = v;
}

// Reset to 0
template<typename T>
inline void Register<T>::Reset()
{
	this->Set((T)0);
}
