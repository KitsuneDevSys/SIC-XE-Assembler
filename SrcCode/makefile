project4:	main.o symbols.o  directives.o instructions.o opcodes.o  headers.h
	gcc -o project4 -Wall  -O0 main.o symbols.o directives.o instructions.o opcodes.o

main.o:	main.c headers.h 
	gcc -c -Wall  -O0 main.c

symbols.o:	symbols.c headers.h
	gcc -c -Wall  -O0 symbols.c

directives.o:	directives.c headers.h
	gcc -c -Wall  -O0 directives.c

instructions.o: instructions.c headers.h
	gcc -c -Wall -O0 instructions.c

opcodes.o: opcodes.c headers.h
	gcc -c -Wall -O0 opcodes.c

clean:	
	rm *.o -f
	touch *.c
	rm project4 -f
