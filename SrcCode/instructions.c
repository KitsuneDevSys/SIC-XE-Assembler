#include "headers.h"

//26 instructions included out of 32
int IsAnInstruction( char *Test ){

return		( 	! ( 
		 strcmp( Test, "LDA" )  &&
		 strcmp( Test, "STCH" ) &&
	     strcmp( Test, "ADD" ) &&	 
		 strcmp( Test, "AND" ) &&
		 strcmp( Test, "COMP" ) &&
		 strcmp( Test, "DIV" ) &&
		 strcmp( Test, "J" ) &&
		 strcmp( Test, "JEQ" ) &&
		 strcmp( Test, "JGT" ) &&
		 strcmp( Test, "JLT" ) &&
		 strcmp( Test, "JSUB" ) &&
		 strcmp( Test, "LDCH" ) &&
		 strcmp( Test, "LDL" ) &&
		 strcmp( Test, "LDX" ) &&
		 strcmp( Test, "MUL" ) &&
		 strcmp( Test, "OR" ) &&
		 strcmp( Test, "RD" ) &&
		 strcmp( Test, "RSUB" ) &&
		 strcmp( Test, "STA" ) &&
		 strcmp( Test, "STL" ) &&
		 strcmp( Test, "STSW" ) &&
		 strcmp( Test, "STX" ) &&
		 strcmp( Test, "SUB" ) &&
		 strcmp( Test, "TD" ) &&
		 strcmp( Test, "TIX" ) &&
		 strcmp( Test, "WD" )	&&
		 strcmp( Test, "ADDF" ) &&      //XE instructions start here. 
		 strcmp( Test, "ADDR" ) &&
		 strcmp( Test, "CLEAR" ) &&
		 strcmp( Test, "COMPF" ) &&
		 strcmp( Test, "COMPR" ) &&
		 strcmp( Test, "DIVF" ) &&
		 strcmp( Test, "DIVR" ) &&
		 strcmp( Test, "FIX" ) &&
		 strcmp( Test, "FLOAT" ) &&
		 strcmp( Test, "HIO" ) &&
		 strcmp( Test, "LDB" ) &&
		 strcmp( Test, "LDF" ) &&
		 strcmp( Test, "LDS" ) &&
		 strcmp( Test, "LDT" ) &&
		 strcmp( Test, "LPS" ) &&
		 strcmp( Test, "MULF" ) &&
		 strcmp( Test, "MULR" ) &&
		 strcmp( Test, "NORM" ) &&
		 strcmp( Test, "RMO" ) &&
		 strcmp( Test, "SHIFTL" ) &&
		 strcmp( Test, "SHIFTR" ) &&
		 strcmp( Test, "SIO" ) &&
		 strcmp( Test, "SSK" ) &&
		 strcmp( Test, "STB" ) &&
		 strcmp( Test, "STF" ) &&
		 strcmp( Test, "STI" ) &&
		 strcmp( Test, "STS" ) &&
		 strcmp( Test, "STT" ) &&
		 strcmp( Test, "SUBF" ) &&
		 strcmp( Test, "SUBR" ) &&
		 strcmp( Test, "SVC" ) &&
		 strcmp( Test, "TIO" ) &&
		 strcmp( Test, "TIXR" )
		 ) ) ;
}

int FormatSpecifier(char *instructs, char opSymb) {
	if((opSymb == '0') || (opSymb == '+')) {
		if(opSymb == '+') {
			return 4;
		}
		else if((strcmp(instructs, "FIX") == 0) || (strcmp(instructs, "FLOAT") == 0) || (strcmp(instructs, "HIO") == 0) || (strcmp(instructs, "NORM") == 0) || (strcmp(instructs, "SIO") == 0) || (strcmp(instructs, "TIO") == 0)) {
			return 1;
		}
		else if((strcmp(instructs, "ADDR") == 0) || (strcmp(instructs, "CLEAR") == 0) || (strcmp(instructs, "COMPR") == 0) || (strcmp(instructs, "DIVR") == 0) || (strcmp(instructs, "MULR") == 0) || (strcmp(instructs, "RMO") == 0) || (strcmp(instructs, "SHIFTL") == 0) || (strcmp(instructs, "SHIFTR") == 0) || (strcmp(instructs, "SUBR") == 0) || (strcmp(instructs, "SVC") == 0) || (strcmp(instructs, "TIXR") == 0)) {
			return 2;
		}
		else {
			return 3;
		}
	}
	else {
		return 0;
	}
}
