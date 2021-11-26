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
		registervalue = 0x01;
	}
    else if((!(strcmp(Test,"B"))) == 1)
	{
		registervalue = 0x03;
	}
    else if((!(strcmp(Test,"S"))) == 1)
	{
		registervalue = 0x04;
	}
    else if((!(strcmp(Test,"T"))) == 1)
	{
		registervalue = 0x05;
	}
    else if((!(strcmp(Test,"F"))) == 1)
	{
		registervalue = 0x06;
	}
    else if((!(strcmp(Test,"PC"))) == 1)
	{
		registervalue = 0x08;
	}
    else if((!(strcmp(Test,"SW"))) == 1)
	{
		registervalue = 0x09;
	}
    return registervalue;
}