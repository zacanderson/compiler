-----------------
| PM/0 Compiler |
-----------------

Creators: 
  GM
  ZA

Program Details:
  This program will run a virtual machine that compiles PL/0 code and prints information to the console.

  Our first iteration focused mainly on the functions within the stack and arithmetic operators used to manipulate the input file's values.
  Our second iteration received input, tokenized, and returned a struct array of lexemes. This file was modified to fit the requirements of multiple file formatting.
  Our third iteration received the tokens and delivers them via a Symbol Table to the Intermediate Code Generator. This is used to convert the code to Assembly, which our virtual machine receives.
  
  The final version uses the VM to run all of the iterations over an input file, and displaying the results in the shell.

These files are included in the package:
	lex.c
	parser.c
	vm.c
	driver.c
	compiler.h
	Makefile

To compile: 	$ make
To run: 	$ ./a.out <input file name> 
