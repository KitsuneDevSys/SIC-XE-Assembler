#include "headers.h"



int IsAValidSymbol( char *TestSymbol ){

	int Result = 1;
	int index =0;
	int maxlength = 7;
	int done;
	done = 0 ;

	while( !done ) {
		if ( TestSymbol[index] == '\0') {
			break;
			}	
		if ( TestSymbol[index] < 65 || TestSymbol[index] > 90){
			done = 1;
			Result = 0;
			}


		if ( index == maxlength ) {
			done = 1;
			Result = 0;
		}
		index++;
		
	}
	if(IsADirective(TestSymbol) == 1 || IsAnInstruction(TestSymbol) ==1)
	{
		Result = 0;
	}
	return Result;
}
