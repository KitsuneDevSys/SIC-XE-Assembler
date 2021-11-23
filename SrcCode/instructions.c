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
		 strcmp( Test, "WD" ) 
		 ) ) ;
}
