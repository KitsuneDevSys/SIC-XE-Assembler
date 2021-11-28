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
	else if((!(strcmp(Test,"ADDF"))) == 1) //XE instruction opcodes
	{
		opcode = 0x58;
	}
	else if((!(strcmp(Test,"ADDR"))) == 1)
	{
		opcode = 0x90;
	}
	else if((!(strcmp(Test,"CLEAR"))) == 1)
	{
		opcode = 0xB4;
	}
	else if((!(strcmp(Test,"COMPF"))) == 1)
	{
		opcode = 0x88;
	}
	else if((!(strcmp(Test,"COMPR"))) == 1)
	{
		opcode = 0xA0;
	}
	else if((!(strcmp(Test,"DIVF"))) == 1)
	{
		opcode = 0x64;
	}
	else if((!(strcmp(Test,"DIVR"))) == 1)
	{
		opcode = 0x9C;
	}
	else if((!(strcmp(Test,"FIX"))) == 1)
	{
		opcode = 0xC4;
	}
	else if((!(strcmp(Test,"FLOAT"))) == 1)
	{
		opcode = 0xC0;
	}
	else if((!(strcmp(Test,"HIO"))) == 1)
	{
		opcode = 0xF4;
	}
	else if((!(strcmp(Test,"FIX"))) == 1)
	{
		opcode = 0xC4;
	}
	else if((!(strcmp(Test,"LDB"))) == 1)
	{
		opcode = 0x68;
	}
	else if((!(strcmp(Test,"LDF"))) == 1)
	{
		opcode = 0x70;
	}
	else if((!(strcmp(Test,"LDS"))) == 1)
	{
		opcode = 0x6C;
	}
	else if((!(strcmp(Test,"LDT"))) == 1)
	{
		opcode = 0x74;
	}
	else if((!(strcmp(Test,"LPS"))) == 1)
	{
		opcode = 0xD0;
	}
	else if((!(strcmp(Test,"MULF"))) == 1)
	{
		opcode = 0x60;
	}
	else if((!(strcmp(Test,"MULR"))) == 1)
	{
		opcode = 0x98;
	}
	else if((!(strcmp(Test,"NORM"))) == 1)
	{
		opcode = 0xC8;
	}
	else if((!(strcmp(Test,"RMO"))) == 1)
	{
		opcode = 0xAC;
	}
	else if((!(strcmp(Test,"SHIFTL"))) == 1)
	{
		opcode = 0xA4;
	}
	else if((!(strcmp(Test,"SHIFTR"))) == 1)
	{
		opcode = 0xA8;
	}
	else if((!(strcmp(Test,"SIO"))) == 1)
	{
		opcode = 0xF0;
	}
	else if((!(strcmp(Test,"SSK"))) == 1)
	{
		opcode = 0xEC;
	}
	else if((!(strcmp(Test,"STB"))) == 1)
	{
		opcode = 0x78;
	}
	else if((!(strcmp(Test,"STF"))) == 1)
	{
		opcode = 0x80;
	}
	else if((!(strcmp(Test,"STI"))) == 1)
	{
		opcode = 0xD4;
	}
	else if((!(strcmp(Test,"STS"))) == 1)
	{
		opcode = 0x7C;
	}
	else if((!(strcmp(Test,"STT"))) == 1)
	{
		opcode = 0x84;
	}
	else if((!(strcmp(Test,"SUBF"))) == 1)
	{
		opcode = 0x5C;
	}
	else if((!(strcmp(Test,"SUBR"))) == 1)
	{
		opcode = 0x94;
	}
	else if((!(strcmp(Test,"SVC"))) == 1)
	{
		opcode = 0xB0;
	}
	else if((!(strcmp(Test,"TIO"))) == 1)
	{
		opcode = 0xF8;
	}
	else if((!(strcmp(Test,"TIXR"))) == 1)
	{
		opcode = 0xB8;
	}
	//printf("%d is the opcode\n",opcode); //test
	return opcode;
}

