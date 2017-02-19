#include "../io/io.h"

void Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code)
{
	std::string opcode, op1, op2, op3;
	uint8_t opcode_, op1_, op2_, flags_, a_, b_, c_;
	std::map<std::string, int> jumps;
	bool label = 0;
	int jumpOffset = 0;

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		opcode = data.at(i).at(0);

		if (opcode.at(0) == ':')
		{
			jumps.insert(std::make_pair(opcode.substr(1), (int)i));
			data.erase(data.begin() + i);
			--i;
		}
	}

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		label = 0;

		opcode = data.at(i).at(0);

		switch (data.at(i).size())
		{
		case 2:
			op3 = data.at(i).at(1);
			break;
		case 3:
			op1 = data.at(i).at(1);
			op3 = data.at(i).at(2);
			break;
		case 4:
			op1 = data.at(i).at(1);
			op2 = data.at(i).at(2);
			op3 = data.at(i).at(3);
			break;
		}

		// Parse O3
		if (data.at(i).size() > 2)
		{
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
		}
		else
			flags_ = 0x00;

		// Check each opcode individually
		if (opcode == "MOV")
		{
			opcode_ = 0x00;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		else if (opcode == "STR")
		{
			opcode_ = 0x10;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		else if (opcode == "ADD")
		{
			opcode_ = 0x20;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		else if (opcode == "JMP")
		{
			opcode_ = 0x30;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;
			
			if (n < 0)
			{
				opcode_ = 0x31;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}
		else if (opcode == "CMP")
		{
			opcode_ = 0x40;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		else if (opcode == "BNE")
		{
			opcode_ = 0x50;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;

			if (n < 0)
			{
				opcode_ = 0x51;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}

		uint32_t f;
		f = (opcode_ << 24) + (op1_ << 20) + (op2_ << 16) + (flags_ << 12) + (a_ << 8) + (b_ << 4) + c_;
		if (!label)
			code.push_back(f);
	}
}