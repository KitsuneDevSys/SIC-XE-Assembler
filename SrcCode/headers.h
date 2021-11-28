#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct symbols {
	int	DefinedOnSourceLine;
	int	Address; 
	char	Name[7]; };

typedef struct symbols	SYMBOL;

struct opcodes {
	char	OpCode;	
	char	Name[8];

};

typedef struct opcodes OPCODES;

//OPCODES OpcodeTable[ 32 ];

struct records 
{
	char RecordType[1];
	int Address;
	char ProgramName[7];
	int Length;
	int opcode;
	int opaddress;
	char ModFlag;
	char SymbolMod[7];
	char ByteOpcode[70];
	int ByteRecord;
	int regAddress1;
	int regAddress2;
};

typedef struct records RECORDS;

int IsAValidSymbol( char *TestSymbol );
int IsADirective( char *Test );
int IsAnInstruction( char *Test);
int InstructionToOpcode(char *Test);
int RegisterValue( char *Test );
int IsAnRegister(char* Test);
int FormatSpecifier(char *instructs, char opSymb);