#include "../io/io.h"

/*
	parse.cpp

	CPP file for Parse function.
*/

// Parses code (already read in correctly) into 32-bit instructions to be stored in memory
void Parse(std::vector<std::vector<std::string>>& data, std::vector<uint32_t>& code)
{
	std::string opcode, op1, op2, op3;
	uint8_t opcode_, op1_, op2_, flags_, a_, b_, c_;
	std::map<std::string, int> jumps;
	int jumpOffset = 0;

	/* Preprocessor - strips out labels and puts them in a jump table for later */
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

	/* Parse instructions, one at a time */
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		opcode = data.at(i).at(0);

		// op3 is always last operand. This is because ARM uses RISC architecture with a simple instruction set and format.
		// This only allows one operand to be an immediate value. So op3 is that operand. Can also be a register, but that's handled after
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

		// Parse O3 - differentiate between it being a register or immediate value
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

		/* Check each opcode individually */
		// This is in linear time, so despite the tower of IF-ELSEs it's actually really fast
		// A lot of instructions are the same in terms of number of operands or what you do with them.
		// The code is repeated for simplicity - it doesn't make it slower, only larger, and prevents confusion when reading it later.

		/* Register File */
		// 0x00:	MOV Rn, O3			Rn <- O3
		if (opcode == "MOV")
		{
			opcode_ = 0x00;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		
		/* RAM */
		// 0x10:	STR Rn, O3			[O3] <- Rn
		else if (opcode == "STR")
		{
			opcode_ = 0x10;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		// 0X11:	LDR Rn, O3			Rn <-[O3]
		else if (opcode == "LDR")
		{
			opcode_ = 0x11;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}

		/* ALU */
		// 0X20:	ADD Rn, Rm, O3		Rn <-Rm + O3
		else if (opcode == "ADD")
		{
			opcode_ = 0x20;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0X21:	SUB Rn, Rm, O3		Rn <- Rm - O3
		else if (opcode == "SUB")
		{
			opcode_ = 0x21;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x22:	MOD Rn, Rm, O3		Rn <- Rm % O3
		else if (opcode == "MOD")
		{
			opcode_ = 0x22;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x23:	LSL Rn, Rm, O3		Rn <- Rm << O3
		else if (opcode == "LSL")
		{
			opcode_ = 0x23;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x24:	LSR Rn, Rm, O3		Rn <- Rm >> O3
		else if (opcode == "LSR")
		{
			opcode_ = 0x24;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x25:	AND	Rn, Rm, O3		Rn <- Rm & O3
		else if (opcode == "AND")
		{
			opcode_ = 0x25;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x26:	ORR Rn, Rm, O3		Rn <- Rm | O3
		else if (opcode == "ORR")
		{
			opcode_ = 0x26;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x27:	NOT Rn, O3			Rn <- !O3
		else if (opcode == "NOT")
		{
			opcode_ = 0x27;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x28:	XOR	Rn, Rm, O3		Rn <- Rm ^ O3
		else if (opcode == "XOR")
		{
			opcode_ = 0x28;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}
		// 0x29:	NEG Rn, Rm			Rn <- ~Rm
		else if (opcode == "NEG")
		{
			opcode_ = 0x29;
			op1_ = std::stoi(op1.substr(1));
			op2_ = std::stoi(op2.substr(1));
		}

		/* Branching (Decoder) */
		// CMP is a regular instruction on the ALU, the rest are branches
		// Branches execute PC <- PC - X where X is the jump offset
		// All values are stored as unsigned, so jumping forwards (negative X) requires a modification
		// If X is negative, add 0x10 to the operand and negate X, which executes PC <- PC + X instead

		// 0x30:	CMP Rn, O3			COMPARE Rn, O3
		else if (opcode == "CMP")
		{
			opcode_ = 0x30;
			op1_ = std::stoi(op1.substr(1));
			op2_ = 0x00;
		}
		// 0x31:	JMP :LABEL			BRANCH UNCONDITIONAL
		else if (opcode == "JMP")
		{
			opcode_ = 0x31;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;
			
			if (n < 0)
			{
				opcode_ = 0x41;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}
		// 0x32:	BEQ :LABEL			BRANCH IF Rn == Rm
		else if (opcode == "BEQ")
		{
			opcode_ = 0x32;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;

			if (n < 0)
			{
				opcode_ = 0x42;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}
		// 0x33:	BNE :LABEL			BRANCH IF Rn != Rm
		else if (opcode == "BNE")
		{
			opcode_ = 0x33;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;

			if (n < 0)
			{
				opcode_ = 0x43;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}
		// 0x34:	BGT :LABEL			BRANCH IF Rn > Rm
		else if (opcode == "BGT")
		{
			opcode_ = 0x34;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;

			if (n < 0)
			{
				opcode_ = 0x44;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}
		// 0x35:	BLT :LABEL			BRANCH IF Rn < Rm
		else if (opcode == "BLT")
		{
			opcode_ = 0x35;
			op1_ = 0x00;
			op2_ = 0x00;
			flags_ = 0x01;
			int n = (i - jumps.find(op3)->second) * 4;

			if (n < 0)
			{
				opcode_ = 0x45;
				n = -n;
			}

			a_ = (n >> 8) & 0x0F;
			b_ = (n >> 4) & 0x0F;
			c_ = n & 0x0F;
		}

		// Construct instruction from the assorted pieces we now have
		uint32_t f;
		f = (opcode_ << 24) + (op1_ << 20) + (op2_ << 16) + (flags_ << 12) + (a_ << 8) + (b_ << 4) + c_;
		code.push_back(f);
	}
}