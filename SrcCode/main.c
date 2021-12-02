#include "headers.h"

int main( int argc, char* argv[]){
	FILE *fp; //Used to read a SIC assembly file
	FILE *wp; //Used to write to a Object assembly file
	char line[1024]; //Used to store the most recent line and store strtoks for the given line
	char* newsym; //Used to store the symbol from the given line
	char* nextoken; //Used to store the directive/instruction from the given line
	char* nexoperand; //Used to store the operand from the given line
	char* prefix; //Used to store the prefix from the operand (i.e X and C)
	char* symbolname; //Stores a copy of the symbol name
	char* fname; //Used to store the file name of the output file
	char* holdsymbol; //Stores a  temporary copy of the operand used to extract the operand symbol
	char* postfix = NULL; //Pointer to be used with strtok_r to extract the postfix
	//char* ByteHex; //TEST
	
	char hex[] = "0x"; //Used to take values from START
	char extension[] =".obj"; //Used to assign the file extension of the output file
	
	
	char operand[1024]; //Stores a copy of the operand
	char operandsymbol[1024]; //stores a copy of the operand with only the symbol (#@)
	char bwstring[1024]; //Stores a copy of the BYTE/WORD operand string
	char fullline[1024]; //Stores a copy of the recent read line
	char sName[7]; //Used to store a copy of a symbol name (TEST)
	
	struct symbols SymTab[1024]; //initilizes a structure array with a size of 1024 that stores the symbol table
	struct records RecTab[1024]; //initilizes a structure array with a size of 1024 that stores the object code

	int locpath=1; //Used to insure specific directives don't increment the locctr twice.
	int path=0; //Used as a true/false integer for the main if in the while loop
	int locctr=0; //Used to keep track of the address of the next instruction (Essentially PC)
	int linectr=1; //Used to indicate line number for the recent line.
	int index=0; //Used to keep track of the current index in the symbol table
	int uniques=0; //used to check if symbol is unique
	int Wn=0; int Bn=0; //Used to store number of words and bytes for RESW and RESB
	int hexc=1; //Used to check if the X' string is a valid hex (0=false 1=true)
	int SymIndex=0; //Used to print out the SymTab if no errors were found
	int oSymbol=0; //Used to check if an operand that is a referenced symbol is within the symbol table
	int pLength; //Used to hold the value of the program length
	int trecpath=1; //Used to insure that no faulty t-records are created
	int mrecpath=1; //Used to insure that all m-records are created
	int rindex=0; //Used to keep track of the current index in the RecTab
	int RecIndex=0; //Used to print out the Record structure
	int EAddress=0; //Used to store the address of the first SIC Instruction 
	int FirstSICInstruction=1; //Used to determine if the the given SIC instruction is the first one that appears, top to bottom, in the sic file
	int RecordOrder=1; //Used to print RecTab's contents in a specific order, 1=Header 2=Text 3=Modification 4=End
	
	long Ws; //Used to store the size from a WORD directive

	//New Variables for SICXE Assembler
	char opcode[1024]; //Stores a copy of the opcode
	char opcodesymbol[1024]; //Stores a copy of the opcode symbol (+)
	char operandsymboltwo[1024]; //stores a copy of the operand with only the symbol (,)
	int formatD[1024]; //Stores the formats of the instructions.
	int fiD = 0; //increments when each iteration of while loop is finished.
	char* holdoperand;
	char postfixstring[1024]; //test
	struct locationandline LocTab[1024]; //Used to store the locctr and its related linectr for use in format 3 and 4
	int holddis; //used to hold negitive displacements to shorten them.
	int FullDis; //used to determin if we use PC or BASE
	char sholddis[8]; //used to hold a string version of the integer string
	char shortholddis[4]; //used to shorten the negdisplacement.
	int BaseAddress; //Used to store the BASE Address.

	if ( argc != 2 ) //Checks if user inputed two arguments when running this program
    {
	printf("ERROR: Usage: %s filename\n", argv[0]);
	return 0;
	}
	fp = fopen( argv[1], "r"); //Checks if file is able to be opened
	fname = argv[1];
	//fname = strtok(fname, ".");
	strcat(fname, extension); //Holds the file name for the object code to be outputed if no errors were found
	
	
	if ( fp == NULL ) //Checks if file is able to be opened
    {
	printf("ERROR: %s could not be opened for reading,\n", argv[1] );
	return 0;
	}
	newsym = malloc(  1024 * sizeof(char) ); //Allocates 1024 memmory to newsym
	memset( newsym, '\0', 1024 * sizeof(char) ); //Sets all of the memory to have terminating char
	nextoken = malloc(  1024 * sizeof(char) ); //Allocates 1024 memmory to nextoken
	memset( nextoken, '\0', 1024 * sizeof(char) ); //Sets all of the memory to have terminating char
	nexoperand = malloc( 1024 * sizeof(char) ); //Allocates 1024 memmory to nexoperand
	memset( nexoperand,'\0', 1024 * sizeof(char) ); //Sets all of the memory to have terminating char
	prefix = malloc(  1024 * sizeof(char) );
	memset( prefix,'\0', 1024 * sizeof(char) );
	symbolname = malloc(  1024 * sizeof(char) ); //Allocates 1024 memmory to symbolname
	memset( symbolname,'\0', 1024 * sizeof(char) ); //Sets all of the memory to have terminating char
	
	
	while(  fgets( line , 1024 , fp ) != NULL )
       	{
		strcpy( fullline, line );
		//printf("%d is the number value", line[0]); //test
		if(locctr > 0xFFFFF) //checks if locctr is out of bounds for the set amount of memory our assemblier has (0xFFFFF is max) (2^20 = 1048576 -> 0x100000)
			{
				printf("ERROR:Program has ran out of memory on line %d\n",linectr);
				fclose(fp);
				return 0;
			}//end of if
		if ( line[0] == 35) //Used to tell program to ignore comments
	       	{  
			    linectr++;
			    continue;
		    }//end of if
		if( line[0] == 9 || line[0] == 32) //Used to remove leading whitespaces or horizontal tabs in front of SIC line and check for blank lines
		{
			 int start=0;
			 int next=0;
			 //printf("BEFORE: |%s| \n",line);
			 while(line[start] == 9 || line[start] == 32)
			 {
				if(start == 1024) //Executes if line is just a blank line
				{
					printf("ERROR: Blank line violates SIC specification on line %d\n",linectr);
					fclose(fp);
					return 0;
				}
				start++;
			 }
			 //printf("%d\n",start);
			 if(start != 0)
			 {
				 /*
				 while(line[next + start] !='\0')
				 {
					 line[next] = line[next+start];
					 //printf("DURING: %s \n",line);
					 next++;
				 }
				 line[next]='\r'; //Adds New carriage return to prevent program from overriding \r with the new \0 from the below line of code
				 line[next+2]='\0'; //Adds New Null Terminating Character to remove/ignore repeating Opcode
				 */
				 //TESTING NEW CODE
				 for(int current = start ; line[current] != '\0'; current++,next++)
				 {
					fullline[next] = line[current];
				 }
				 fullline[next] = '\0';
				 //fullline[next +1] = '\r';
				 strcpy(line,fullline);
			 }
			 //printf("AFTER: |%s| \n",line); //TEST
			 //printf("TEST");//TEST
			 path=1; //Used to split off program from s i/d o ; to i/d o
		} //end of if
		//printf("%d Test\n", line[0]); //TEST
		if (  ( (line[0] >= 65 ) && ( line[0] <= 90 ) ) || ( (line[0] == 43  &&  path == 1)  )  ) 
	       	{
			if(path==1) //If line doesnt have the symbol parameter
			{
			 nextoken = strtok(line, " \t\n");
			 strcpy(opcode, nextoken); //copys Intruction into a seperate char array for extracting the symbol
			 nexoperand = strtok(NULL," 	\t");
			 strcpy(operand, nexoperand); //copys operand into a seperate char array
			 if( (line[0] == 43) )
			 {
				strncpy(opcodesymbol,line,1); //copies symbol (+) into opcodesymbol
			 }
			 else
			 {
				strcpy(opcodesymbol,"0"); //Places a 0 if their is no symbol present
			 }
			 //printf("The opcode stored is %s\n",opcode); //TEST
			 holdsymbol = strtok_r(opcode, " +\t\n", &postfix); //removes + from opcode
			 strcpy(opcode, holdsymbol); //stores the modified opcode into the char array
			 formatD[fiD] = FormatSpecifier(opcode, opcodesymbol[0]); //stores the format number of each instruction into array.
			 //printf("Format: %d\n", formatD[fiD]);
			 strcpy(nextoken, opcode); //copys opcode into nextoken
			 //printf("|%s| is the opcode \n",opcode); //TEST
			 //printf("|%s| is the opcodesymbol extracted \n",opcodesymbol); //TEST
			 //printf("|%s| is the operand BEFORE \n",operand);
			 //printf("|%c| is the operandsymbol BEFORE\n", nexoperand[0]); //TEST
			 if( (nexoperand[0] == 35) || (nexoperand[0] == 64) )
			 {
				strncpy(operandsymbol,nexoperand,1); //Copies symbol (#@) into operandsymbol
			 }
			 else
			 {
				strcpy(operandsymbol,"0"); //Places a 0 if their is no symbol present
			 }
			 //printf("The operand stored is |%s|\n",operand); //TEST
			 holdsymbol = strtok(operand, " #@\t\n"); //extracts symbol from operand
			 //printf("The string stored in holdsymbol is |%s|\n",holdsymbol);
			 if(holdsymbol != NULL)
			 {
		     	strcpy(operand, holdsymbol); //stores the extracted symbol into the char array
			 	//printf("|%s| is the operand \n",operand);
			 	strcpy(nexoperand, operand); //copys operand into a seperate char array
			 	//printf("%s is the operand \n",operand); //TEST
			 	//printf("|%s| is the operandsymbol extracted \n",operandsymbol); //TEST
			 }
			//printf("%s is the operand \n",operand);
			//printf("%s is the operandsymbol extracted \n",operandsymbol);
			LocTab[linectr].line = linectr;
			LocTab[linectr].location = locctr;
			 if((!(strcmp(nextoken, "BASE"))) == 1) //Based on SCOFF document BASE doesn't increment locctr
			 {
				locpath = 0;
			 } //end of if BASE
			 else if(formatD[fiD] == 1) { // The four next else if's increment the locctr based on the format.
				 locctr += 0x01;
			 } 
			 else if(formatD[fiD] == 2) { 
				 locctr += 0x02;
			 }
			 else if(formatD[fiD] == 3) { 
				 locctr += 0x03;
			 } 
			 else if(formatD[fiD] == 4) { 
				 locctr += 0x04;
			 } 
			}
			else
			{
			newsym = strtok( line, " \t\n");
			strcpy(symbolname,newsym);
			nextoken = strtok( NULL, " \t\n");
			nexoperand = strtok( NULL, "\t\n"); //CHANGED
			strcpy(operand, nexoperand); //copys operand into a seperate char array
			strcpy(opcode,nextoken);//copys opcode into a seperate char array
			if( IsAValidSymbol(newsym) == 0 ) //Used to validate symbol names
		       	{
				printf("ERROR: INVALID SYMBOL on line %d\n",linectr);
				fclose(fp);
				return 0;
			} //end if
			if(index !=0) //doesnt check if this is the first symbol
			{
			while(SymTab[uniques].Name[1] != '\0') //used to check if symbol is already in the table
			{
				strcpy(sName, SymTab[uniques].Name);
				if(strcmp(sName, newsym) == 0)
				{
					printf("ERROR: Duplicate Symbol on line %d\n",linectr);
					fclose(fp);
					return 0;
				}
				uniques++;
			}uniques=0; //restarts uniques
			} //end of symbol duplication check
			}//end of else-path
			if( (path!=1) && ( (nextoken[0] == 43) ) ) //checks if their is a symbol (+) present within the opcode if their is a symbol defined 
			{
				strncpy(opcodesymbol,nextoken,1); //copies symbol (+) into opcode symbol
			}
			else if( (path!=1) && ( (nextoken[0] != 43) ) ) //executes if the above if is false
			{
				strcpy(opcodesymbol,"0"); //Places a 0 if their is no symbol present
			} //end of else-if
			if(path != 1) //removes symbol (+) from opcode and places it in nextoken if path isn't equal to 1
			{
				holdsymbol = strtok_r(opcode, " +@#\t\n", &postfix); //removes +#@ from opcode
			    strcpy(opcode, holdsymbol); //stores the modified opcode into the char array
			    strcpy(nextoken, opcode); //copys opcode into nextoken
				formatD[fiD] = FormatSpecifier(opcode, opcodesymbol[0]); //stores the format number of each instruction into array.
			} //end if
			if( (path!=1) && ( (nexoperand[0] == 35) || (nexoperand[0] == 64) ) ) //checks if their is a symbol (@#) present within the operand if their is a symbol defined
			{
				strncpy(operandsymbol,nexoperand,1); //Copies symbol (#@) into operandsymbol
			}
			else if( (path!=1) && ( (nexoperand[0] != 35) && (nexoperand[0] != 64) ) ) //executes if the above if false
			{
				strcpy(operandsymbol,"0"); //Places a 0 if their is no symbol present
			}
			if(path != 1) //removes symbol (@#) from the operand and places it in the nexoperand if path isn't equal to 1
			{
				holdsymbol = strtok_r(operand, " #@\t\n", &postfix); //extracts symbol from operand
			 	strcpy(operand, holdsymbol); //stores the modified operand into the char array
			 	strcpy(nexoperand, operand); //copys operand into a seperate char array
			}
			if( (IsADirective(nextoken) != 1) ) //Used to validate instructions/directives
		       	{
				 if( (IsAnInstruction(nextoken) != 1) )
				 {
					printf("ERROR: Invalid DIRECTIVE or INSTRUCTION on line %d\n",linectr);
					fclose(fp);
					return 0;
			     }
			} //end of instruction/directive check
			if(((!(strcmp(nextoken, "START"))) != 1) && index==0) //Checks if the symbol table has the START Directive
				{
					printf("ERROR: SIC Program missing START DIRECTIVE\n");
					fclose(fp);
					return 0;
				}
			else if((!(strcmp(nextoken, "START"))) == 1) //Sets the initial address for the locctr (in hex)
			{
				strcat(hex,nexoperand);
				locctr = (int) strtol(hex,NULL,0);
				if(locctr >= 0x100000)//checks to see if addresss starts at 100000 in hex (1048576 in decimal) or higher; thus exceeding memmory limit
				{
					printf("ERROR: SIC program starts at Hex 100000 or higher(no room in memory) on line %d\n",linectr);
					fclose(fp);
					return 0;
				}
				SymTab[index].Address = locctr; //Stores Address in SymTab structure array
				LocTab[linectr].line = linectr;
			    LocTab[linectr].location = locctr;
				locpath=0;
			}  //end of if START
			else if((!(strcmp(nextoken, "WORD"))) == 1)
			{
				Ws = strtol(nexoperand,NULL,10);
				if(Ws > 8388608 ) //Checks if WORD exceeds 24 bit limit (2^23 = 8388608)
				{
					printf("ERROR: Word constant exceeds 24 bit limitation on line %d\n",linectr);
					fclose(fp);
					return 0;
				}
			} //end of else-if WORD
			else if((!(strcmp(nextoken, "BYTE"))) == 1)
			{
				char* Bw;
				prefix = strtok(operand, "'");
				Bw = strtok(NULL,"'");
				//printf("|%s| is the string stored in bw \n",Bw); //TEST
				if((!(strcmp(prefix, "C"))) == 1)
				{
					SymTab[index].Address = locctr; //Stores Address in SymTab structure array
					LocTab[linectr].line = linectr;
			        LocTab[linectr].location = locctr;
					locctr += strlen(Bw);
					locpath=0;
				} //end of c-if path
				else if((!(strcmp(prefix, "X"))) == 1)
				{
					strcpy(bwstring,Bw);
					//printf("|%s| is the string stored in bw \n",bwstring); //TEST
					for(int cindex=0;bwstring[cindex]!='\0';cindex++) //Checks to see if the string in an X' is a valid hex
					{
							//printf("|%d| \n",isxdigit(bwstring[cindex]));
							if(isxdigit(bwstring[cindex]) ==0) //Executes if a character in the string isnt a valid hex
							{
								hexc=0;
							}
					}
					if(hexc == 0) //Checks if the string is a valid hex
					{
							//printf("%ld is the length of the operand\n", strlen(Bw)); //test
							printf("ERROR:Program has invalid hex constant on line %d\n",linectr);
							fclose(fp);
							return 0;
					}
					else
					{
						SymTab[index].Address = locctr; //Stores Address in SymTab structure array
						LocTab[linectr].line = linectr;
			            LocTab[linectr].location = locctr;
						locctr += (strlen(Bw))/2;
						locpath = 0;
					}
				} //end of x-if path
			} //end of else-if BYTE
			else if((!(strcmp(nextoken, "RESW"))) == 1)
			{
				Wn = strtol(operand,NULL,0); //Number of words to be stored in memory
				SymTab[index].Address = locctr; //Stores Address in SymTab structure array
				LocTab[linectr].line = linectr;
			    LocTab[linectr].location = locctr;
				locctr += 3 * Wn; //1 word is 3 bytes so (address = Wn * 3)
				locpath = 0;
			} //end of else-if RESW
			else if((!(strcmp(nextoken, "RESB"))) == 1)
			{
				Bn = (int) strtol(operand,NULL,10); //takes the number of Bytes as a decimal value
				SymTab[index].Address = locctr; //Stores Address in SymTab structure array
				LocTab[linectr].line = linectr;
			    LocTab[linectr].location = locctr;
				locctr += Bn; //accounds the number of bytes for memory storage by incrementing locctr to the given operand decimal number
				locpath = 0;
			} //end of else-if RESB
			else if((!(strcmp(nextoken, "BASE"))) == 1) //Based on SCOFF document BASE doesn't increment locctr
			{
				locpath = 0;
			} //end of else-if BASE
			strcpy(SymTab[index].Name, symbolname); //Stores symbol in SymTab structure array
			SymTab[index].DefinedOnSourceLine = linectr; //Stores source line in SymTab structure array
			if(locpath==1 && path!=1) //Insures specific dirrectives dont trigger this (i.e. START and BYTE) and path=1 path doesnt trigger this
			{
				SymTab[index].Address = locctr; //Stores Address in SymTab structure array
				LocTab[linectr].line = linectr;
			    LocTab[linectr].location = locctr;
				if(formatD[fiD] == 1) 
				{ // The four next else if's increment the locctr based on the format.
				 locctr += 0x01;
			 	} 
				else if(formatD[fiD] == 2) 
				{ 
				 locctr += 0x02;
			 	}
			 	else if(formatD[fiD] == 3) 
				{ 
				 locctr += 0x03;
			 	} 
			 	else if(formatD[fiD] == 4) 
				{ 
				 locctr += 0x04;
			 	} 
				//locctr+=3; //increments the locctr by three bytes NOTE THIS IS NO LONGER USED AS NOT ALL SICXE INSTRUCTIONS INCREMENT BY 3 BYTES
			}
			//printf("%X is the address stored in SymTab[%d]\n", SymTab[index].Address,index); //test
			if(path !=1) //Insures path=1 path doesnt trigger this
			{
				index++; //increments index to next position for next valid symbol
			}
			locpath=1;
			path=0;
			linectr++;
			continue;
		}//end of if 
		else //Any outstanding potential errors are checked here
		{
			newsym = strtok( line, " \t\n");
			if(line[0] == 13) //BLANK LINE
			{
				printf("ERROR: Blank line violates SIC specification on line %d\n",linectr);
				fclose(fp);
				return 0;
			}
			if ( IsAValidSymbol(newsym) == 0 ) //Used to validate symbol names (If the symbol starts with any non alphabetical chars)
			{
				printf("ERROR: INVALID SYMBOL on line %d\n",linectr);
				fclose(fp);
				return 0;
			}
		}
		fiD++;
	} //end of file read while
	//printf("TESTING\n"); //TEST
	while(SymIndex != index)//Prints out the symbol table 
	{
		printf("%s			%X\n",SymTab[SymIndex].Name, SymTab[SymIndex].Address);
		SymIndex++;
	}//end of SymTab print while
	//end of Pass 1
	/*
	int I = 1;
	while(I < linectr) //Prints out the LocTab table (test)
	{
		printf("%d			%X\n",LocTab[I].line, LocTab[I].location);
		I++;
	}
	*/



	//Start of Pass 2
	locctr -= 3; //last inctruction/directive, END, shouldn't progress locctr
	pLength = locctr - SymTab[0].Address;
	//printf("%X is the length of the program\n",pLength);
	rewind(fp); //reset the file pointer to the start of the file 
	locctr = 0; //resets locctr to 0
	linectr = 1; //resets linectr to 1
	index = 0; //resets index to 0
	path=0; //resets path to 0
	fiD=0;
	strcpy(hex,"0x"); //resets hex to 0x
	//printf("1 HELLO WORLD\n");
	while(  fgets( line , 1024 , fp ) != NULL ) //Reads through the file again, Pass 2, to write out the object code. Removed all Pass 1 error checks and SymTab table insertion in this loop, because this was done in Pass 1
       	{
		strcpy( fullline, line );
		//printf("2 HELLO WORLD\n");
		printf("linectr is %d\n",linectr);
		if ( line[0] == 35) //Used to tell program to ignore comments
	       	{  
				linectr++;
				continue;
			}//end of if
		if( line[0] == 9 || line[0] == 32) //Used to remove leading whitespaces or horizontal tabs in front of SIC line
		{
			 int start=0;
			 int next=0;
			 while(line[start] == 9 || line[start] == 32)
			 {
				start++;
			 }
			 if(start != 0)
			 {
				 /*
				 while(line[next + start] !='\0')
				 {
					 line[next] = line[next+start];
					 //printf("DURING: %s \n",line);
					 next++;
				 }
				 line[next]='\r'; //Adds New carriage return to prevent program from overriding \r with the new \0 from the below line of code
				 line[next+2]='\0'; //Adds New Null Terminating Character to remove/ignore repeating Opcode
				 */
				 //TESTING NEW CODE
				 for(int current = start ; line[current] != '\0'; current++,next++)
				 {
					fullline[next] = line[current];
				 }
				 fullline[next] = '\0';
				 //fullline[next +1] = '\r';
				 strcpy(line,fullline);
			 }
			 path=1; //Used to split off program from s i/d o ; to i/d o
		} //end of if
		if (  ( (line[0] >= 65 ) && ( line[0] <= 90 ) ) || ( (line[0] == 43  &&  path == 1)  )   ) 
	       	{
			if(path==1) //If line doesnt have the symbol parameter
			{
			 //NEW CODE STARTS HERE
			 //printf("Within pass == 1\n");
			 nextoken = strtok(line, " \t\n");
			 strcpy(opcode, nextoken); //copys Intruction into a seperate char array for extracting the symbol
			 nexoperand = strtok(NULL," 	\t");
			 strcpy(operand, nexoperand); //copys operand into a seperate char array for extracting the symbol
			 //printf("HELP\n");
		     if( (line[0] == 43) )
			 {
				strncpy(opcodesymbol,line,1); //copies symbol (+) into opcodesymbol
			 }
			 else
			 {
				strcpy(opcodesymbol,"0"); //Places a 0 if their is no symbol present
			 }
			 holdsymbol = strtok_r(opcode, " +\t\n", &postfix); //removes + from opcode
			 strcpy(opcode, holdsymbol); //stores the modified opcode into the char array
			 formatD[fiD] = FormatSpecifier(opcode, opcodesymbol[0]); //stores the format number of each instruction into array.
			 strcpy(nextoken, opcode); //copys opcode into nextoken
			 //printf("Testing 2 within pass=1 new code\n");
			 if( (nexoperand[0] == 35) || (nexoperand[0] == 64) )
			 {
				strncpy(operandsymbol,nexoperand,1); //Copies symbol (#@) into operandsymbol
			 }
			 else
			 {
				strcpy(operandsymbol,"0"); //Places a 0 if their is no symbol present
			 }
			 //printf("TESTING 1 after new code in path == 1 part 1\n");
			 holdsymbol = strtok(operand, " #@\t\n"); //extracts symbol from operand
			 if(holdsymbol != NULL)
			 {
		     	strcpy(operand, holdsymbol); //stores the extracted symbol into the char array
			 	//printf("|%s| is the operand \n",operand);
			 	strcpy(nexoperand, operand); //copys operand into a seperate char array
			 	//printf("%s is the operand \n",operand); //TEST
			 	//printf("|%s| is the operandsymbol extracted \n",operandsymbol); //TEST
			 }
			 //NEW CODE ENDS HERE
			 //printf("TESTING 1 after new code in path == 1 part 2\n");
			 holdoperand = strtok_r(operand, " ,\t\n\r", &postfix); //extracts symbol from operand
			 //printf("%s is stored in postfix\n",postfix); //test
			 //printf("%s is stored in holdoperand\n",holdoperand);
			 if(postfix!=NULL)
			 {
				strcpy(postfixstring,postfix); //test
			 }
			 //printf("After if statement in pass 1\n"); 
			 if(holdoperand != NULL)
			 {
				strcpy(operandsymboltwo, holdoperand); //stores the extracted symbol into the char array
			    strcpy(operand, nexoperand); //copys operand into a seperate char array
			 }
			 //NEEDS TO BE MODIFIED FOR SICXE
			 //printf("%d: Location Counter: %X\n", linectr, locctr);
			 strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
			 RecTab[rindex].Address = locctr; //Puts the adress from the locctr into the RecTab
			 //RecTab[rindex].Length = 0x03; //Stores the length of a SIC instruction (3 bytes) into the RecTab
			 RecTab[rindex].opcode = InstructionToOpcode(nextoken); //Stores the opcode value of the instruction into the RecTab (but not the address)
			 //printf("Before While loop within Pass 1\n");
			 while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    {
				  //printf("HELLO WORLD\n"); //test
				  strcpy(sName, SymTab[uniques].Name);
				  //printf("HEY YOU!!\n");
				  if(strcmp(sName, operandsymboltwo) == 0)
				  {
					  RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
					  if((!(strcmp(postfixstring, "X"))) == 1) //Used to prevent RSUB from creating Modification Records
						{
							RecTab[rindex].opaddress += 0x10000; //Offsets opaddress by 10000 if an X postfix is present in the operand
						}
					    break;
					 
				  }
				 uniques++;
			    }
				//Create T records based on the format of the instruction
				if(formatD[fiD] == 1) {
					RecTab[rindex].Length = 0x01;
				} else if(formatD[fiD] == 2) {
					RecTab[rindex].Length = 0x02;
					//printf("%s\n", nexoperand);

					//if format 2 instruction deals with 2 registers, split the string based on the comma.
					if((RecTab[rindex].opcode != 0xB4) && (RecTab[rindex].opcode != 0xB0) && (RecTab[rindex].opcode != 0xB8)) {
						//printf("%s\n", operand);
						nexoperand = strtok(operand, ",");
						RecTab[rindex].regAddress1 = RegisterValue(nexoperand);
						nexoperand = strtok(NULL, ",");
						RecTab[rindex].regAddress2 = RegisterValue(nexoperand);
					}
					else { //if format 2 instruction deals with 1 register
						RecTab[rindex].regAddress1 = RegisterValue(nexoperand);
						RecTab[rindex].regAddress2 = 0;
					}
				} else if(((formatD[fiD] == 3) && ((!(strcmp(nextoken, "BASE"))) != 1) && ((!(strcmp(nextoken, "END"))) != 1))) {
					RecTab[rindex].Length = 0x03;
					uniques=0;
					int cHeCkEr = 0;
					int lEnGtH = 0;
					while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    		{
							//printf("HELLO WORLD\n"); //test
							strcpy(sName, SymTab[uniques].Name);
							//printf("HEY YOU!!\n");
							if(strcmp(sName, operandsymboltwo) == 0)
							{
								RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
								break;
							}
							uniques++;
						}
					if(strcmp(operandsymbol, "@") == 0) {
						RecTab[rindex].opcode+=0x02;
					}
					else if(strcmp(operandsymbol, "#") == 0) {
						lEnGtH = strlen(operandsymboltwo);
						//printf("%s\n",operandsymboltwo);
						for(int L = 0; L < lEnGtH; L++) {
							if(isdigit(operandsymboltwo[L]) == 0) {
								break;
							}
							if(L != lEnGtH - 1) {
								continue;
							}
							//printf("Before %s\n",operandsymboltwo);
							RecTab[rindex].opaddress = atoi(operandsymboltwo);
							//printf("After %d\n",RecTab[rindex].opaddress);
							RecTab[rindex].disp = RecTab[rindex].opaddress;
							cHeCkEr = 1;
						}
						RecTab[rindex].opcode+=0x01;
					}
					else {
						RecTab[rindex].opcode+=0x03;
					}
					if(cHeCkEr == 0) {
						if(locctr >= RecTab[rindex].opaddress) {
							//RecTab[rindex].disp = locctr - RecTab[rindex].opaddress - 3; //- RecTab[rindex].Address;
							//RecTab[rindex].disp = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							holddis = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							FullDis = holddis;
							//printf("%X is the value stored in holddis\n",holddis);
							//printf("%d is the decimal value stored in holddis\n",holddis);
							//printf("%X is the opaddress and %X is the LocTab\n", RecTab[rindex].opaddress, LocTab[linectr+1].location);
							//itoa(holddis,sholddis,16);
							snprintf(sholddis,9,"%X",holddis);
							//printf("%s is the integer value as a string\n",sholddis);
							strncpy(shortholddis, &sholddis[5],3);
							//printf("%s is the integer value as a string after strncpy\n",shortholddis);
							//printf("Neg displacement\n");
							RecTab[rindex].disp = (int) strtol(shortholddis,NULL,16);

						}
						else {
							//RecTab[rindex].disp = RecTab[rindex].opaddress - 3 - locctr;
							holddis = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							RecTab[rindex].disp = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							FullDis = holddis;
							//printf("%d: %X, %X\n", linectr, RecTab[rindex].opaddress, locctr);
						}
						//printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
					}
					lEnGtH = strlen(nexoperand);
					//printf("TEST");
					int ctCom = 0;
					int regLi = 0;
					for(int L = 0; L < lEnGtH; L++) {
						if(nexoperand[L] == ',') {
							ctCom++;
							continue;
						}
						if((ctCom == 1) && (nexoperand[L] == 'X')) {
							regLi = 1;
							break;
						}
						else {
							ctCom = 0;
						}
					}
					//printf("%d is the value of regLi\n",regLi);
					//printf("%s is the string\n",nextoken);
					//printf("%d is the value of the strcmp\n",(!(strcmp(nextoken, "RSUB"))));
					//printf("TEST before\n");
					//printf("%d is the decimal value of displacement\n",RecTab[rindex].disp);
					if(((!(strcmp(nextoken, "RSUB"))) != 1)) {
						if((FullDis <= 2047) && (FullDis >= -2048)) {
							RecTab[rindex].pcOrB = 0x02;
							printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
							//printf("1\n");
						}
						else {
							FullDis = RecTab[rindex].opaddress - BaseAddress;
							if((FullDis >= 0) && (FullDis <= 4095)) { //replace FullDis with Base disp
								if(regLi == 1) {
									RecTab[rindex].pcOrB = 0x0C;
									//printf("2\n");
								}
								else {
									RecTab[rindex].pcOrB = 0x04;
									//printf("3\n");
								}
								RecTab[rindex].disp = FullDis;
								printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
							}
							else {
								printf("ERROR: Addresses out of bounds for PC and Base addressing on line %d.\n", linectr);
								fclose(fp);
								return 0;
							}
						}
					}
					else{
						RecTab[rindex].pcOrB = 0x00;
						RecTab[rindex].disp = 0x000;
						printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
					}
					//printf("TEST after\n");
				} else if(formatD[fiD] == 4) {
					RecTab[rindex].Length = 0x04;
					//printf("operandsymbol: %s\n", operandsymbol);
					//When fprinting the object code, hard code "10" in between the opcode and opaddress
					uniques=0;
						while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    		{
							//printf("HELLO WORLD\n"); //test
							strcpy(sName, SymTab[uniques].Name);
							//printf("HEY YOU!!\n");
							if(strcmp(sName, operandsymboltwo) == 0)
							{
								RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
								break;
							}
							uniques++;
						}
					if(strcmp(operandsymbol, "@") == 0) {
						RecTab[rindex].opcode+=0x02;
					}
					else if(strcmp(operandsymbol, "#") == 0) {
						int lEnGtH = strlen(operandsymboltwo);
						for(int L = 0; L < lEnGtH; L++) {
							if(isdigit(operandsymboltwo[L]) == 0) {
								break;
							}
							if(L != lEnGtH - 1) {
								continue;
							}
							RecTab[rindex].opaddress = atoi(operandsymboltwo);
						}
						RecTab[rindex].opcode+=0x01;
					}
					else {
						RecTab[rindex].opcode+=0x03;
					}
					printf("Opaddress: %X\n", RecTab[rindex].opaddress);
				} //else {
				
				//}
			 uniques=0; //restarts uniques
			 rindex++; //increments rindex to next position for modification record
			 //printf("Before String compare within pass == 1 \n");
			 if((!(strcmp(nextoken, "RSUB"))) == 1) //Used to prevent RSUB from creating Modification Records
			 {
				mrecpath=0;
			 }
			 //NEW IF STATEMENT
			 if((!(strcmp(nextoken, "BASE"))) == 1) //Based on SCOFF document BASE doesn't increment locctr
			 {
				locpath = 0;
				mrecpath= 0;
				//printf("%X is the value of the locctr at BASE\n",locctr);
				//printf("|%s| is the string in operandtwo\n",operandsymboltwo);
				while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    {
				  //printf("HELLO WORLD\n"); //test
				  strcpy(sName, SymTab[uniques].Name);
				  //printf("\n");
				  if(strcmp(sName, operandsymboltwo) == 0)
				  {
					BaseAddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
				  }
				 uniques++;
			    }
				uniques = 0;
				printf("%X is the value of the BASE Address at line %d\n",BaseAddress,linectr);

			 } //end of if BASE
			 if(mrecpath ==1) //Used to Prevent RSUB Modification Records Exclusively
			 {
			 strcpy(RecTab[rindex].RecordType,"M");
			 RecTab[rindex].Address = locctr+1;
			 RecTab[rindex].Length = 0x04;
			 RecTab[rindex].ModFlag = '+';
			 strcpy(RecTab[rindex].SymbolMod, SymTab[0].Name);
			 }
			 mrecpath=0; //Used to prevent the creation of duplicate Modification Records
			 trecpath=0; //Used to prevent the creation of duplicate Text Records
			 rindex++; //increments rindex to next position for next valid record
			 //NEW LOCCTR
			 //printf("TESTING 4\n");
			 if((!(strcmp(nextoken, "BASE"))) == 1) //Based on SCOFF document BASE doesn't increment locctr
			 {
				locpath = 0;
			 } //end of if BASE
			 else if(formatD[fiD] == 1) 
			 { // The four next else if's increment the locctr based on the format.
				 locctr += 0x01;
			 } 
			 else if(formatD[fiD] == 2) 
			 { 
				 locctr += 0x02;
			 }
			 else if(formatD[fiD] == 3) 
			 { 
				 locctr += 0x03;
			 } 
			 else if(formatD[fiD] == 4) 
			 { 
				 locctr += 0x04;
			 } 
			 //locctr+=3; //increments the locctr by three bytes
			 //printf("END OF PATH == 1\n"); //test
			}
			else
			{
			newsym = strtok( line, " \t\n");
			strcpy(symbolname,newsym);
			nextoken = strtok( NULL, " \t\n");
			nexoperand = strtok( NULL, "\t\n");
			strcpy(operand, nexoperand); //copys operand into a seperate char array for extracting the symbol
			strcpy(opcode,nextoken);//copys opcode into a seperate char array
			//printf("BEFORE HOLDOPERAND\n");
			holdoperand = strtok(operand, " ,@#\t\n"); //extracts symbol from operand
			//printf("%s is the string in holdoperand\n",holdoperand);
			strcpy(operandsymboltwo, holdoperand); //stores the extracted symbol into the char array
			strcpy(operand, nexoperand); //copys operand into a seperate char array
			//printf("%s is the string in operandsymboltwo\n",operandsymboltwo); //test
			if( (nexoperand[0] == 35) || (nexoperand[0] == 64) )
			{
				strncpy(operandsymbol,nexoperand,1); //Copies symbol (#@) into operandsymbol
			}
			 else
			{
				strcpy(operandsymbol,"0"); //Places a 0 if their is no symbol present
			}
			holdsymbol = strtok(operand, " #@\t\n"); //extracts symbol from operand
			if(holdsymbol != NULL)
			 {
		     	strcpy(operand, holdsymbol); //stores the extracted symbol into the char array
			 	strcpy(nexoperand, operand); //copys operand into a seperate char array
			 }
			}//end of else-path
			//printf("TESTING 3\n");
			if( (path!=1) && ( (nextoken[0] == 43) ) ) //checks if their is a symbol (+) present within the opcode if their is a symbol defined 
			{
				strncpy(opcodesymbol,nextoken,1); //copies symbol (+) into opcode symbol
			}
			else if( (path!=1) && ( (nextoken[0] != 43) ) ) //executes if the above if is false
			{
				strcpy(opcodesymbol,"0"); //Places a 0 if their is no symbol present
			} //end of else-if
			if(path != 1) //removes symbol (+) from opcode and places it in nextoken if path isn't equal to 1
			{
				holdsymbol = strtok_r(opcode, " +@#\t\n", &postfix); //removes +#@ from opcode
			    strcpy(opcode, holdsymbol); //stores the modified opcode into the char array
			    strcpy(nextoken, opcode); //copys opcode into nextoken
				formatD[fiD] = FormatSpecifier(opcode, opcodesymbol[0]); //stores the format number of each instruction into array.
			} //end if
			//printf("|%s| is the string stored in operandtwo in pass !=1 \n",operandsymboltwo);
			if( IsAValidSymbol(operandsymboltwo) != 0 ) //Used to check if operands that are symbols are within the symbol table
		    {
			   while(SymTab[uniques].Name[1] != '\0') //used to check if symbol is in table
			    {
				  strcpy(sName, SymTab[uniques].Name);
				  //printf("|%s| is the string stored within SymTab.Name\n",sName);
				  if(strcmp(sName, operandsymboltwo) == 0)
				  {
					 oSymbol=1;
					 break;
				  }
				 uniques++;
			    }uniques=0; //restarts uniques
			}
			else if(IsAValidSymbol(operand) == 0) //Executes if operand doesn't contain a symbol or is a register (to be added soon)
			{
				oSymbol=1;
			}
			if(oSymbol == 0) //Executes if operend symbol isn't within the symbol table
			{
				printf("ERROR: referenced operand symbol is not located within the symbol table on line %d\n",linectr);
				fclose(fp);
				return 0;
			}
			if((!(strcmp(nextoken, "START"))) == 1) //Sets the H record for the RecTab
			{
				strcat(hex,nexoperand);
				strcpy(RecTab[rindex].RecordType,"H"); //Sets the record type to a Header
				strcpy(RecTab[rindex].ProgramName,SymTab[index].Name); //Puts the program name from the SymTab into the RecTab
				RecTab[rindex].Address = SymTab[index].Address; //Puts the adress from the SymTab into the RecTab
				RecTab[rindex].Length = pLength; //stores the Length of the program into calculated from Pass1 into the H record within RecTab
				locctr = (int) strtol(hex,NULL,0);
				locpath=0;
				trecpath=0;
				mrecpath=0;
				rindex++; //increments rindex to next position for next valid record
			}  //end of if START
			else if((!(strcmp(nextoken, "WORD"))) == 1)
			{
				Ws = strtol(nexoperand,NULL,10);
				strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
				RecTab[rindex].Address = SymTab[index].Address; //Puts the adress from the SymTab into the RecTab
				RecTab[rindex].Length = 0x03; //Stores the length of Word (3 bytes) into the RecTab
				RecTab[rindex].opaddress = Ws; //Stores the value of the Ws as a decimal to later be converted into a hexadecimal
				trecpath=0;
				mrecpath=0;
				rindex++; //increments rindex to next position for next valid record
			} //end of else-if WORD
			else if((!(strcmp(nextoken, "BYTE"))) == 1)
			{
				char* Bw;
				prefix = strtok(operand, "'");
				Bw = strtok(NULL,"'");
				if((!(strcmp(prefix, "C"))) == 1)
				{
					char CharacterToHex;
					char StringBuffer[10000]; 
					strcpy(hex,""); //resets hex to empty string for use in getting the hex from a string within the C prefix
					int ByteTextLimit=30;
				    int ByteStringLength;
					int RemainingStringLength;
				    int SubStringLength=0;
				    int outer=0;
				    int inner=0;
					int offset=locctr;
					int firststringpass=1;
			
					strcpy(bwstring,Bw);
					ByteStringLength = strlen(Bw);
					RemainingStringLength = ByteStringLength;
					while(outer<ByteStringLength) //Sets Text records for the C prefix for Byte
					{
						inner=0;
						//printf("%d is the string length\n",ByteStringLength);//test
						while(inner<ByteTextLimit && inner<ByteStringLength && inner<RemainingStringLength) //converts each individual character in the Character string into a hexadecimal character, then stores that into a char array
						{
							char Temp[10];
							CharacterToHex = bwstring[outer];
							
							//TestSymbol[index] < 65 || TestSymbol[index] > 90
							//printf("%d is the value of this character in Decimal\n", CharacterToHex);
							if(CharacterToHex > 31)
							{
							sprintf(Temp, "%X", CharacterToHex); //Converts Character into Hexadecimal Character
							strcat(StringBuffer,Temp); //Stores Hexadecimal Character into Hexadecimal String
							}
							inner++;
							outer++;
							SubStringLength++;
						}
						if(inner>=ByteTextLimit) //Calculates the remaining string length outside of the while loop to prevent premature termination 
						{
							RemainingStringLength=ByteStringLength-30;
						}
						
						//ByteHex = strtok(hex," \n\t\r"); //test
						//printf("%s is the string in ByteHex",ByteHex); //test
						strcat(hex,StringBuffer);
						//printf("%s is the string in hex\n", hex);
					    //printf("%d is the offending character\n",hex[73]); 
						strcpy(StringBuffer,""); //resets StringBuffer to a empty Sring
						if(firststringpass==1) //Sets the Text record for the first pass of the BYTE string
						{
						strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
						RecTab[rindex].Address = locctr; //Sets the address of the current spot of the locctr
						RecTab[rindex].Length = SubStringLength;//Sets the length of the record to the length of the converted string (up to a max of 30 in decimal, 1E in hex)
						strcpy(RecTab[rindex].ByteOpcode,hex);
						RecTab[rindex].ByteRecord = 1; //Indicates this will be a Byte Text Record
						firststringpass=0; //Insures that future Text records don't execute thus if-path
						}
						else //Sets the Text record for the ongoing passes of the BYTE string
						{
						strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
						RecTab[rindex].Address = offset; //Sets the address of the current spot of the locctr
						RecTab[rindex].Length = SubStringLength;//Sets the length of the record to the length of the converted string (up to a max of 30 in decimal, 1E in hex)
						strcpy(RecTab[rindex].ByteOpcode,hex);
						RecTab[rindex].ByteRecord = 1; //Indicates this will be a Byte Text Record
						}
						strcpy(hex,""); //resets hex
						offset+=SubStringLength;
						SubStringLength=0; //resets SubStringLength
						rindex++;
					}
					locctr += strlen(Bw);
					locpath=0;
				} //end of c-if path
				else if((!(strcmp(prefix, "X"))) == 1)
				{
					strcpy(bwstring,Bw);
										
					strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
					RecTab[rindex].Address = locctr; //Sets address of the record to the value of the locctr
					strcpy(RecTab[rindex].ByteOpcode,bwstring); //sets the Opcode to the Hex string stored within the operand
					RecTab[rindex].Length = (strlen(Bw))/2; //the Hex string is a pair of characters that for each individual length, divide by 2 to get the length
					RecTab[rindex].ByteRecord = 1; //Indicates this will be a Byte Text Record
					
					locctr += (strlen(Bw))/2;
					locpath = 0;
					rindex++;
				} //end of x-if path
				trecpath=0;
				mrecpath=0;
			} //end of else-if BYTE
			else if((!(strcmp(nextoken, "RESW"))) == 1)
			{
				Wn = strtol(operand,NULL,0); //Number of words to be stored in memory
				locctr += 3 * Wn; //1 word is 3 bytes so (address = Wn * 3)
				locpath = 0;
				trecpath=0;
				mrecpath=0;
			} //end of else-if RESW
			else if((!(strcmp(nextoken, "RESB"))) == 1)
			{
				Bn = (int) strtol(operand,NULL,10); //takes the number of Bytes as a decimal value
				locctr += Bn; //accounts for the number of bytes for memory storage by incrementing locctr to the given operand decimal number
				locpath = 0;
				trecpath=0;
				mrecpath=0;
			}//end of else-if RESB
			else if((!(strcmp(nextoken, "END"))) == 1)
			{
				strcpy(RecTab[rindex].RecordType,"E"); //Stores Record Type as End Record
				RecTab[rindex].Address = EAddress; //Stores address of the first execcutible instruction in the RecTab
				trecpath=0;
				mrecpath=0;
				rindex++;
			}//end of else-if END
			else if((!(strcmp(nextoken, "BASE"))) == 1) //Based on SCOFF document BASE doesn't increment locctr
			{
				locpath = 0;
				trecpath = 0;
			} //end of else-if BASE
			else if(FirstSICInstruction==1) //If the nextoken is the first SIC Instruction, this will execute
			{
				EAddress = locctr; //stores the locctr in End Address to be stored in the E Record
				FirstSICInstruction=0;
			}
			if(trecpath==1) //Creates t-records for lines with a symbol defined (NEEDS TO BE MODIFIED FOR SICXE)
			{
				strcpy(RecTab[rindex].RecordType,"T"); //Sets the record type to a text
				while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the defined symbol from the SymTab
			    {
				  strcpy(sName, SymTab[uniques].Name);
				  //printf("%d is the value of the string compare\n",strcmp(sName, operand));
				  if(strcmp(sName, newsym) == 0)
				  {
					 RecTab[rindex].Address = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
					 break;
				  }
				 uniques++;
			    }uniques=0; //restarts uniques
				RecTab[rindex].Length = 0x03; //Stores the length of an instruction (3 bytes) into the RecTab
				RecTab[rindex].opcode = InstructionToOpcode(nextoken); //Stores the opcode value of the instruction into the RecTab (but not the address)
				if(formatD[fiD] == 1) {
					RecTab[rindex].Length = 0x01;
				} else if(formatD[fiD] == 2) {
					RecTab[rindex].Length = 0x02;
					//printf("%s\n", nexoperand);

					//if format 2 instruction deals with 2 registers, split the string based on the comma.
					if((RecTab[rindex].opcode != 0xB4) && (RecTab[rindex].opcode != 0xB0) && (RecTab[rindex].opcode != 0xB8)) {
						//printf("%s\n", operand);
						nexoperand = strtok(operand, ",");
						RecTab[rindex].regAddress1 = RegisterValue(nexoperand);
						nexoperand = strtok(NULL, ",");
						RecTab[rindex].regAddress2 = RegisterValue(nexoperand);
					}
					else { //if format 2 instruction deals with 1 register
						RecTab[rindex].regAddress1 = RegisterValue(nexoperand);
						RecTab[rindex].regAddress2 = 0;
					}
				} else if(((formatD[fiD] == 3) && ((!(strcmp(nextoken, "BASE"))) != 1) && ((!(strcmp(nextoken, "END"))) != 1))) {
					RecTab[rindex].Length = 0x03;
					uniques=0;
					int cHeCkEr = 0;
					int lEnGtH = 0;
					while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    		{
							//printf("HELLO WORLD\n"); //test
							strcpy(sName, SymTab[uniques].Name);
							//printf("HEY YOU!!\n");
							if(strcmp(sName, operandsymboltwo) == 0)
							{
								RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
								break;
							}
							uniques++;
						}
					if(strcmp(operandsymbol, "@") == 0) {
						RecTab[rindex].opcode+=0x02;
					}
					else if(strcmp(operandsymbol, "#") == 0) {
						lEnGtH = strlen(operandsymboltwo);
						//printf("%s\n",operandsymboltwo);
						for(int L = 0; L < lEnGtH; L++) {
							if(isdigit(operandsymboltwo[L]) == 0) {
								break;
							}
							if(L != lEnGtH - 1) {
								continue;
							}
							//printf("Before %s\n",operandsymboltwo);
							RecTab[rindex].opaddress = atoi(operandsymboltwo);
							//printf("After %d\n",RecTab[rindex].opaddress);
							RecTab[rindex].disp = RecTab[rindex].opaddress;
							cHeCkEr = 1;
						}
						RecTab[rindex].opcode+=0x01;
					}
					else {
						RecTab[rindex].opcode+=0x03;
					}
					if(cHeCkEr == 0) {
						if(locctr >= RecTab[rindex].opaddress) {
							//RecTab[rindex].disp = locctr - RecTab[rindex].opaddress - 3; //- RecTab[rindex].Address;
							//RecTab[rindex].disp = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							holddis = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							FullDis = holddis;
							//printf("%X is the value stored in holddis\n",holddis);
							//printf("%d is the decimal value stored in holddis\n",holddis);
							//printf("%X is the opaddress and %X is the LocTab\n", RecTab[rindex].opaddress, LocTab[linectr+1].location);
							//itoa(holddis,sholddis,16);
							snprintf(sholddis,9,"%X",holddis);
							//printf("%s is the integer value as a string\n",sholddis);
							strncpy(shortholddis, &sholddis[5],3);
							//printf("%s is the integer value as a string after strncpy\n",shortholddis);
							//printf("Neg displacement\n");
							RecTab[rindex].disp = (int) strtol(shortholddis,NULL,16);

						}
						else {
							//RecTab[rindex].disp = RecTab[rindex].opaddress - 3 - locctr;
							holddis = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							RecTab[rindex].disp = RecTab[rindex].opaddress - LocTab[linectr+1].location;
							FullDis = holddis;
							//printf("%d: %X, %X\n", linectr, RecTab[rindex].opaddress, locctr);
						}
						//printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
					}
					lEnGtH = strlen(nexoperand);
					//printf("TEST");
					int ctCom = 0;
					int regLi = 0;
					for(int L = 0; L < lEnGtH; L++) {
						if(nexoperand[L] == ',') {
							ctCom++;
							continue;
						}
						if((ctCom == 1) && (nexoperand[L] == 'X')) {
							regLi = 1;
							break;
						}
						else {
							ctCom = 0;
						}
					}
					//printf("%d is the value of regLi\n",regLi);
					//printf("%s is the string\n",nextoken);
					//printf("%d is the value of the strcmp\n",(!(strcmp(nextoken, "RSUB"))));
					//printf("TEST before\n");
					//printf("%d is the decimal value of displacement\n",RecTab[rindex].disp);
					if(((!(strcmp(nextoken, "RSUB"))) != 1)) {
						if((FullDis <= 2047) && (FullDis >= -2048)) {
							RecTab[rindex].pcOrB = 0x02;
							printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
							//printf("1\n");
						}
						else {
							FullDis = RecTab[rindex].opaddress - BaseAddress;
							if((FullDis >= 0) && (FullDis <= 4095)) { //replace FullDis with Base disp
								if(regLi == 1) {
									RecTab[rindex].pcOrB = 0x0C;
									//printf("2\n");
								}
								else {
									RecTab[rindex].pcOrB = 0x04;
									//printf("3\n");
								}
								RecTab[rindex].disp = FullDis;
								printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
							}
							else {
								printf("ERROR: Addresses out of bounds for PC and Base addressing on line %d.\n", linectr);
								fclose(fp);
								return 0;
							}
						}
					}
					else{
						RecTab[rindex].pcOrB = 0x00;
						RecTab[rindex].disp = 0x000;
						printf("%d: Displacement: %X\n",linectr, RecTab[rindex].disp);
					}
					//printf("TEST after\n");
				} else if(formatD[fiD] == 4) {
					RecTab[rindex].Length = 0x04;
					//printf("operandsymbol: %s\n", operandsymbol);
					//When fprinting the object code, hard code "10" in between the opcode and opaddress
					uniques=0;
						while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    		{
							//printf("HELLO WORLD\n"); //test
							strcpy(sName, SymTab[uniques].Name);
							//printf("HEY YOU!!\n");
							if(strcmp(sName, operandsymboltwo) == 0)
							{
								RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
								break;
							}
							uniques++;
						}
					if(strcmp(operandsymbol, "@") == 0) {
						RecTab[rindex].opcode+=0x02;
					}
					else if(strcmp(operandsymbol, "#") == 0) {
						int lEnGtH = strlen(operandsymboltwo);
						for(int L = 0; L < lEnGtH; L++) {
							if(isdigit(operandsymboltwo[L]) == 0) {
								break;
							}
							if(L != lEnGtH - 1) {
								continue;
							}
							RecTab[rindex].opaddress = atoi(operandsymboltwo);
						}
						RecTab[rindex].opcode+=0x01;
					}
					else {
						RecTab[rindex].opcode+=0x03;
					}
					printf("Opaddress: %X\n", RecTab[rindex].opaddress);
				} else {
					uniques=0; 
					while(SymTab[uniques].Name[1] != '\0') //used to get the address of the symbol from the operand from the SymTab
			    	{
				  		strcpy(sName, SymTab[uniques].Name);
				  		if(strcmp(sName, operandsymboltwo) == 0) //NEED TO TAKE INTO ACCOUNT THE X ADDRESSES FOR SYMBOL,X
				  		{
					 		RecTab[rindex].opaddress = SymTab[uniques].Address; //Puts the address from the SymTab into the RecTab
					 		break;
				  		}
				 		uniques++;
			    	}uniques=0; //restarts uniques
				}
			 //printf("Before String compare within pass == 1 \n");
				rindex++;
			}
			if(mrecpath == 1) //creates modification records for lines with a symbol defined 
			{
				strcpy(RecTab[rindex].RecordType,"M");
				RecTab[rindex].Address = locctr+1;
				RecTab[rindex].Length = 0x05;
				RecTab[rindex].ModFlag = '+';
				strcpy(RecTab[rindex].SymbolMod, SymTab[0].Name);
				rindex++;
			}
			if(locpath==1 && path!=1) //Insures specific dirrectives dont trigger this (i.e. START and BYTE) and path=1 path doesnt trigger this
			{
				if(formatD[fiD] == 1) 
				{ // The four next else if's increment the locctr based on the format.
				 locctr += 0x01;
			 	} 
				else if(formatD[fiD] == 2) 
				{ 
				 locctr += 0x02;
			 	}
			 	else if(formatD[fiD] == 3) 
				{ 
				 locctr += 0x03;
			 	} 
			 	else if(formatD[fiD] == 4) 
				{ 
				 locctr += 0x04;
			 	} 
				//locctr+=3; //increments the locctr by three bytes
			}
			if(path !=1) //Insures path=1 path doesnt trigger this
			{
				index++; //increments index to next position for next valid symbol
			}
			oSymbol=0;
			trecpath=1;
			mrecpath=1;
			locpath=1;
			path=0;
			linectr++;
			continue;
		}//end of if 
		fiD++;
		//printf("%d is the line\n",linectr); //TEST
	} //end of second file read while
	//Where I will open the obj file and start writing the RecTab to it
	printf("END HELLO WORLD\n");
	wp = fopen( fname, "w"); //Creates the Object file to write the RecTab's contents.
	if(wp == NULL) //Checks if file is able to be opened
	{
		printf("ERROR: Object File Creation Failed");
		fclose(fp);
		fclose(wp);
		return 0;
	}
	//printf("HELLO WORLD\n");
	
	//Writing the RecTab into the .obj file in order of Header, Text, Modification, and End Record
	while(RecIndex != rindex && RecordOrder < 5)//Prints out the RecTab table for testing purposes 
	{
		if((strcmp(RecTab[RecIndex].RecordType,"H") == 0) && RecordOrder == 1)
		{
			//printf("%s%s%06X%06X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].ProgramName, RecTab[RecIndex].Address, RecTab[RecIndex].Length);
			fprintf(wp,"%s%s%06X%06X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].ProgramName, RecTab[RecIndex].Address, RecTab[RecIndex].Length);
			RecIndex++;
		}
		else if((strcmp(RecTab[RecIndex].RecordType,"T") == 0) && RecordOrder == 2)
		{
			if(RecTab[RecIndex].ByteRecord != 1)
			{
				if(RecTab[RecIndex].Length == 1) {
					//printf("%s%06X%02X%02X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode);
					fprintf(wp,"%s%06X%02X%02X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode);
				}
				else if(RecTab[RecIndex].Length == 2) {
					//printf("%s%06X%02X%02X%01X%01X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].regAddress1, RecTab[RecIndex].regAddress2);
					fprintf(wp,"%s%06X%02X%02X%01X%01X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].regAddress1, RecTab[RecIndex].regAddress2);
				}	
				else if(RecTab[RecIndex].Length == 4) {
					//printf("%s%06X%02X%02X10%04X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].opaddress);
					fprintf(wp,"%s%06X%02X%02X10%04X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].opaddress);
				}
				else {
					//printf("%s%06X%02X%02X%01X%03X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].pcOrB, RecTab[RecIndex].disp);
					fprintf(wp,"%s%06X%02X%02X%01X%03X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].pcOrB, RecTab[RecIndex].disp);
				}
			//old sic pass 2//printf("%s%06X%02X%02X%04X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].opaddress);
			//old sic pass 2//fprintf(wp,"%s%06X%02X%02X%04X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].opcode, RecTab[RecIndex].opaddress);
			RecIndex++;
			}
			else if(RecTab[RecIndex].ByteRecord == 1)
			{
			//printf("%s%06X%02X%s\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].ByteOpcode);
			fprintf(wp,"%s%06X%02X%s\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].ByteOpcode);
			RecIndex++;
			}
		}
		else if((strcmp(RecTab[RecIndex].RecordType,"M") == 0) && RecordOrder == 3)
		{
			if((RecTab[RecIndex].Length == 4) && (strcmp(operandsymbol, "@") != 0) && (strcmp(operandsymbol, "#") != 0)) {
				//printf("%s%06X%02X%c%s\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].ModFlag, RecTab[RecIndex].SymbolMod);
				fprintf(wp,"%s%06X%02X%c%s\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address, RecTab[RecIndex].Length, RecTab[RecIndex].ModFlag, RecTab[RecIndex].SymbolMod);
			}
			RecIndex++;
		}
		else if((strcmp(RecTab[RecIndex].RecordType,"E") == 0) && RecordOrder == 4)
		{
			//printf("%s%06X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address);
			fprintf(wp,"%s%06X\n",RecTab[RecIndex].RecordType, RecTab[RecIndex].Address);
			RecIndex++;
		}
		else //Used to increment the RecIndex in case no above conditions match
		{
			if(RecIndex == rindex-1 && RecordOrder < 5) //Used to reset RecIndex in order to print out all types of records
			{
				RecIndex = 0;
				RecordOrder++;
			}
			else //Executes if none of the above conditions match
			{
			    RecIndex++;
			}
		}
	}//end of RecTab print while
	fclose(fp);
	fclose(wp);
}//end of program
