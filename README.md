# SIC-XE-Assembler
## V1.0: December 2, 2021 Changelog
- Format 4 fully implemented in Pass 2.
- T and M records fully implemented.
- Output to .obj file fully implemented.
## V0.4: November 29, 2021 Changelog
- Format 1,2 and 3 fully implemented in Pass 2.
 - Printing of T Records of such will come later.
- Fully implemented displacement calculations in format 3.
  - All bugs have been fixed.
- Implemented a LocTab for use in format 3.
  - Keeps track of address location for all lines in the given sic/xe file.
## V0.3: November 26, 2021 Changelog
- Implementation of formats 1,2,3 and 4 within pass 1 have been added.
- Pass 1 complete: proper symbol table output is achieved.
- Implementation of FormatSpecifier method within instructions.c. 
- Modification of makefile and header file to incorporate new cfile register.c and their related methods.
- Reworked feature to remove leading spaces again.
  - Fixed issue that arised with SIC/XE lines containing only an opcode (such as RSUB).
- Edited test2.sic file to display fixes.     
## V0.2: November 25, 2021 Changelog
- Implemented detection for the BASE directive.
  - Reverted testfile (test1.sic) to account for this change. 
- Implemented functionality to seperate the symbols (+@#) from the opcode if their is a defined symbol on that line.
- Reworked feature to remove leading whitespaces.
  - Now the line wont have duplicate operands.
  - Added test2.sic to display this change.
- Pass 1 now prints out a symbol table. However currently this symbol table is incorrect due to not accounting for format 1, format 2, and format 4.
  - This will be taken care of in the next latest version.  
## V0.1: November 24, 2021 Changelog
- Imported testfile present in SCOFF file.
  - <s>Modified BASE line as BASE isn't an instruction nor directive within SIC or SICXE.</s>
- Increased the max memory limit from 8000 (2^15) to 10000 (2^20).
- Added functionality to seperate the symbols (+@#) from the opcode token if their isn't a symbol declared on that line.
  - Will add same funcionality to all portions of opcode in next latest version.
- Temporarily disabled pass 2 to allow streamlined testing of pass 1 implementation.
  - Temporarily enabled symbol table printing.  
## V0.0: November 22, 2021 Changelog
- Added Source Files From Project 2.
- Removed Unnecessary Files.
