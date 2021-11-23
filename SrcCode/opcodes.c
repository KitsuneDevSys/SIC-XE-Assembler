#include "headers.h"

int InstructionToOpcode(char *Test)
{
	int opcode = 100;//will return an invalid opcode if the intruction was invalid
	if((!(strcmp(Test,"LDA"))) == 1)
	{
		opcode = 0x00;
	}
	else if((!(strcmp(Test,"STCH"))) == 1)
	{
		opcode = 0x54;
	}
	else if((!(strcmp(Test,"ADD"))) == 1)
	{
		opcode = 0x18;
	}
	else if((!(strcmp(Test,"AND"))) == 1)
	{
		opcode = 0x40;
	}
	else if((!(strcmp(Test,"COMP"))) == 1)
	{
		opcode = 0x28;
	}
	else if((!(strcmp(Test,"DIV"))) == 1)
	{
		opcode = 0x24;
	}
	else if((!(strcmp(Test,"J"))) == 1)
	{
		opcode = 0x3C;
	}
	else if((!(strcmp(Test,"JEQ"))) == 1)
	{
		opcode = 0x30;
	}
	else if((!(strcmp(Test,"JGT"))) == 1)
	{
		opcode = 0x34;
	}
	else if((!(strcmp(Test,"JLT"))) == 1)
	{
		opcode = 0x38;
	}
	else if((!(strcmp(Test,"JSUB"))) == 1)
	{
		opcode = 0x48;
	}
	else if((!(strcmp(Test,"LDCH"))) == 1)
	{
		opcode = 0x50;
	}
	else if((!(strcmp(Test,"LDL"))) == 1)
	{
		opcode = 0x08;
	}
	else if((!(strcmp(Test,"LDX"))) == 1)
	{
		opcode = 0x04;
	}
	else if((!(strcmp(Test,"MUL"))) == 1)
	{
		opcode = 0x20;
	}
	else if((!(strcmp(Test,"OR"))) == 1)
	{
		opcode = 0x44;
	}
	else if((!(strcmp(Test,"RD"))) == 1)
	{
		opcode = 0xD8;
	}
	else if((!(strcmp(Test,"RSUB"))) == 1)
	{
		opcode = 0x4C;
	}
	else if((!(strcmp(Test,"STA"))) == 1)
	{
		opcode = 0x0C;
	}
	else if((!(strcmp(Test,"STL"))) == 1)
	{
		opcode = 0x14;
	}
	else if((!(strcmp(Test,"STSW"))) == 1)
	{
		opcode = 0xE8;
	}
	else if((!(strcmp(Test,"STX"))) == 1)
	{
		opcode = 0x10;
	}
	else if((!(strcmp(Test,"SUB"))) == 1)
	{
		opcode = 0x1C;
	}
	else if((!(strcmp(Test,"TD"))) == 1)
	{
		opcode = 0xE0;
	}
	else if((!(strcmp(Test,"TIX"))) == 1)
	{
		opcode = 0x2C;
	}
	else if((!(strcmp(Test,"WD"))) == 1)
	{
		opcode = 0xDC;
	}
	//printf("%d is the opcode\n",opcode); //test
	return opcode;
}

