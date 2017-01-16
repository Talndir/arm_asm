#pragma once

/*
	instruction.h

	Header file for Instruction struct.
	Holds control bus, address bus and data bus values for a single microcode instruction.
*/

/* Struct definition */
struct Instruction
{
	uint16_t control;
	uint16_t address;
	uint16_t data;
};