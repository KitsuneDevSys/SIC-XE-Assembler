#include "headers.h"

int IsAValidRegister( char *Test )
{
    int registervalue = NULL; //Will return an invalid registervalue if the register was invalid
    if((!(strcmp(Test,"A"))) == 1)
	{
		registervalue = 0x00;
	}
	else if((!(strcmp(Test,"X"))) == 1)
	{
		registervalue = 0x10;
	}
    else if((!(strcmp(Test,"B"))) == 1)
	{
		registervalue = 0x30;
	}
    else if((!(strcmp(Test,"S"))) == 1)
	{
		registervalue = 0x40;
	}
    else if((!(strcmp(Test,"T"))) == 1)
	{
		registervalue = 0x50;
	}
    else if((!(strcmp(Test,"F"))) == 1)
	{
		registervalue = 0x60;
	}
    else if((!(strcmp(Test,"PC"))) == 1)
	{
		registervalue = 0x80;
	}
    else if((!(strcmp(Test,"SW"))) == 1)
	{
		registervalue = 0x90;
	}
    return registervalue;
}