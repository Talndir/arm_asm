#include "../io/io.h"

void Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code)
{
	std::string opcode, op1, op2, op3;

	uint8_t opcode_, op1_, op2_, flags_, a_, b_, c_;

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		opcode = data.at(i).at(0);
		op1 = data.at(i).at(1).substr(1);		// Strip initial R

		if (data.at(i).size() == 3)
			op3 = data.at(i).at(2);				// Contains only 2 operands
		else
		{
			op2 = data.at(i).at(2);				// Contains 3 operands
			op3 = data.at(i).at(3);
		}

		// Parse O3
		if (op3.at(0) == 'R')					// Set flags as register
		{
			flags_ = 0x03;
			a_ = std::stoi(op3.substr(1));
			b_ = 0x00;
			c_ = 0x00;
		}
		else if (op3.at(0) == '#')				// Set flags as immediate
		{
			flags_ = 0x01;
			int n = std::stoi(op3.substr(1));
			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}

		// Check each opcode individually
		if (opcode == "MOV")
		{
			opcode_ = 0x00;
			op1_ = std::stoi(op1);
			op2_ = 0x00;
		}
		else if (opcode == "STR")
		{
			opcode_ = 0x10;
			op1_ = std::stoi(op1);
			op2_ = 0x00;
		}
		else if (opcode == "ADD")
		{
			opcode_ = 0x20;
			op1_ = std::stoi(op1);
			op2_ = std::stoi(op2.substr(1));
		}

		uint32_t f;
		f = (opcode_ << 24) + (op1_ << 20) + (op2_ << 16) + (flags_ << 12) + (a_ << 8) + (b_ << 4) + c_;
		code.push_back(f);
	}
}