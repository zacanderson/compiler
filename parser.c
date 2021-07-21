#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define MAX_SYMBOL_TABLE_SIZE 500
symbol table[MAX_SYMBOL_TABLE_SIZE];

// function signatures
instruction *parse(lexeme *tokenList, int print);

void PROGRAM();
void BLOCK();
int CONST_DECLARATION();
int VAR_DECLARATION();
int PROCEDURE_DECLARATION();
void STATEMENT();
void CONDITION();
void FACTOR();
void TERM();
void EXPRESSION();

instruction *code;
int codeidx = 0, jpcIdx = 0, loopIdx = 0, jmpidx = 0;
lexeme *tokenlist;
int currentTableSize = 0;
int lDex = 0;
int SymIndex = 0;
int SymIdx = 0;
int SymIdxV = 0;
int SymIdxC = 0;
int token;
int numVars;
int numConst;
int stIndex = 1;
int save;
char *ident;
int procedurecount = 0;
int count = 0;
//int numProc = 0;
int mark = 1;
int unmarked = 0;
int jmpIdx = 0;

char *errormsg[] = {
	"",
	"Error : program must end with period\n",
	"Error : const, var, and read keywords must be followed by identifier\n",
	"Error : competing symbol declarations at the same level\n",
	"Error : constants must be assigned with =\n",
	"Error : constants must be assigned an integer value\n",
	"Error : symbol declarations must be followed by a semicolon\n",
	"Error : undeclared variable or constant in equation\n",
	"Error : only variable values may be altered\n",
	"Error : assignment statements must use :=\n",
	"Error : begin must be followed by end\n",
	"Error : if must be followed by then\n",
	"Error : while must be followed by do\n",
	"Error : condition must contain comparison operator\n",
	"Error : right parenthesis must follow left parenthesis\n",
	"Error : arithmetic equations must contain operands, parentheses, numbers, or symbols\n",
  "Error: undeclared procedure for call\n",
  "Error: parameters may only be specified by an identifier\n",
  "Error: parameters must be declared\n",
  "Error: cannot return from main\n"
};

void emit(int opcode, char op[], int l, int m);

void codeGeneration(int print)
{
  if (print)
  {
    printf("\nGenerated Assembly:\n");
    printf("%s%6s%5s%5s\n", "Line", "OP", "L", "M");

    for (int i = 0; i < codeidx; i++)
    {
      printf("%3d%7s    %d   %2d\n", i, code[i].op, code[i].l, code[i].m);
    }
    
    printf("\n\n");
  }
}

instruction *parse(lexeme *tokenList, int print)
{
	code = malloc(500 * sizeof(instruction));
  tokenlist = tokenList;
  PROGRAM();
	
  codeGeneration(print);
	return code;
}
// iterate through symbol table, if kind is 3 and val is equal to index parameter that is passed in, // return the index
int FINDPROCEDURE (int index)
{
  int i;

  for (i = 0; i < SymIndex; i++)
  {
    if (table[i].kind == 3 && table[i].val == index)
      return i;
  }
  return -1; // if specifications are not found
}

void MARK(int count)
{
  int i;

  for (i = SymIndex-1; count > 0; i--)
  {
    if (table[i].mark == unmarked)
    {
      table[i].mark = mark;
      count--;
    }
    else
      continue;
  }
}

void emit(int opcode, char op[], int l, int m)
{
	code[codeidx].opcode = opcode;
	strcpy(code[codeidx].op, op);
	code[codeidx].l = l;
	code[codeidx].m = m;
	codeidx++;
}

int SYMBOLTABLECHECK (char *string, int level)
{
  int i;

	// linear search through symbol table looking at name and level
  // return index if found, -1 if not

  for (i = 0; i < SymIndex; i++)
  {
    if (strcmp(table[i].name, string) == 0 && table[i].level == level && table[i].mark == unmarked)
      return i;
  }
  return -1;
}

int SYMBOLTABLESEARCH(char *string, int lexlevel, int kind)
{
  int i;

  //printf("%s %d\n", string,lexlevel);
  for (i = SymIndex-1; i >= 0; i--)
  {
    if (strcmp(table[i].name, string) == 0 && table[i].kind == kind && table[i].mark == unmarked && table[i].level <= lexlevel)
      return i;
  }
  return -1;
}

int getnexttoken()
{
  token = tokenlist[lDex].type;

  lDex++;
  return token;
}

void enter(int kind, char *name, int value, int level, int address, int mark, int param)
{   
  table[SymIndex].kind = kind;
  strcpy(table[SymIndex].name, name);
  table[SymIndex].val = value;
  table[SymIndex].level = level;
  table[SymIndex].addr = address;
  table[SymIndex].mark = mark;
  table[SymIndex].param = param;
  
  SymIndex++;
}

// im not entirely sure about return types or parametres on some of these functions
// emit is definitely a function (we have to determine what goes inside)
// ^^ cont: emit has two different signatures, both seem to run every time emit() is called? not sure on that one
void PROGRAM() 
{
  token = getnexttoken();
  int numProc = 1;
  emit(7, "JMP", 0, 0); // dummy jump?

  for (int i = 0; tokenlist[i].type != 19 ; i++)
  {
    if (tokenlist[i].type == procsym)
    {
      numProc++;
      emit(7, "JMP", 0, 0); // dummy jump??
    }
  }

  enter(3,"main", 0, 0 ,0, unmarked, 0);
  procedurecount++;
	BLOCK(0, 0, 0);

	if (token != periodsym)
  {
		printf("%s\n", errormsg[1]);
    exit(1);
  }

  for (int i = 0; i < numProc; i++)
  {
    code[i].m = table[FINDPROCEDURE(i)].addr;
  }

  for (int j = 0; j < codeidx; j++) 
  {
    if (code[j].opcode == 5) // CAL
      code[j].m = table[FINDPROCEDURE(code[j].m)].addr;
  }
  
  emit(9, "SYS", 0, 3);
	// emit(token, tokenlist[SymIndex].name, 0, 0); // define a constant for l & m
}

// not used very much in PROGRAM() except at the start (translation not complete)
void BLOCK(int lexlevel, int param, int prodecureIndex)
{
  int c, v, p;
  
  c = CONST_DECLARATION(lexlevel);
  v = VAR_DECLARATION(lexlevel, param);
  p = PROCEDURE_DECLARATION(lexlevel);

  // codeidx - 1, current code index
  table[prodecureIndex].addr = codeidx;

  // emit INC (M = 4 + numVars);
  emit(6, "INC", 0, 4 + v); // INC
  STATEMENT(lexlevel);

  MARK(c+v+p);
}

int CONST_DECLARATION(int lexlevel)
{
  numConst = 0;
  if (token == constsym)
  {
    do
    {
      numConst++;
      token = getnexttoken();

      if (token != identsym)
      {
        printf("%s\n", errormsg[2]); // identifier error
        exit(1);
      }
      
      if (SYMBOLTABLECHECK(tokenlist[lDex-1].name, lexlevel) != -1)
      {
        printf("%s\n", errormsg[3]); // symbol already declared error
        exit(1);
      }

      ident = tokenlist[lDex-1].name; 
      //enter(1, tokenlist[SymIndex].name, tokenlist[SymIndex].value, 0);
      
      token = getnexttoken();

      if (token != eqlsym)
      {
        printf("%s\n", errormsg[4]); // assigning constant error
        exit(1);
      }

      token = getnexttoken();

      if (token != numbersym)
      {
        printf("%s\n", errormsg[5]); // integer assigning error
        exit(1);
      }
      // add to symbol table (kind 1, saved name, number, 0, 0)
      enter(1, ident, tokenlist[lDex-1].value, lexlevel, 0, unmarked, 0);
      
      token = getnexttoken();
    }while (token == commasym);

    if (token != semicolonsym)
    {
        printf("%s\n", errormsg[6]); // declaration semicolon error
        exit(1);
    }
    token = getnexttoken();
  }
  return numConst;
}

// another do-while loop, less complicated than last one
int VAR_DECLARATION(int lexlevel, int param) // returns number of variables
{
  if (param == 1)
    numVars = 1;
  else
    numVars = 0;

  if (token == varsym)
  {
    do
    {
      numVars++;
      token = getnexttoken();

      if (token != identsym)
      {
        printf("%s\n", errormsg[2]); // identifier error
        exit(1);
      }

      if (SYMBOLTABLECHECK(tokenlist[lDex-1].name, lexlevel) != -1)
      {
        printf("%s\n", errormsg[3]); // symbol already declared error
        exit(1);
      }

      // add to symbol table (kind 2, ident, 0, 0, var# + 3)
      enter(2, tokenlist[lDex-1].name, 0, lexlevel, numVars + 3, unmarked, 0);

      token = getnexttoken();
      
    } while (token == commasym);

    if (token != semicolonsym)
    {
      printf("%s\n", errormsg[6]); // declaration semicolon error
      exit(1);
    }

    token = getnexttoken();
  }
  return numVars;
}

int PROCEDURE_DECLARATION (int lexlevel)
{
	int numProc = 0;
	if (token == procsym) // if the token is a procedure
  {
		do
    {
			numProc++;
			token = getnexttoken();

			if (token != identsym)
      {
				printf("%s\n", errormsg[2]); // identifier error
        exit(1);
      }
			if (SYMBOLTABLECHECK(tokenlist[lDex-1].name, lexlevel) != -1)
      {
				printf("%s\n", errormsg[3]); // symbol already declared error
        exit(1);
      }

      int procIdx = SymIndex; // end of table
      
      // add to symbol table (kind 3, ident, val = procedurecount, lexlevel, 0, unmarked, param 0)
      enter(3, tokenlist[lDex-1].name, procedurecount, lexlevel, 0, unmarked, 0); 

      procedurecount++;
      token = getnexttoken();

			if (token == lparentsym) 
      {
			  token = getnexttoken();
      
        if (token != identsym)
        {
          printf("%s\n", errormsg[2]); // identifier error
          exit(1);
        }

        // add to symbol table (kind 2, ident, val=0, lexlevel + 1, addr 3, unmarked, 0)
        enter(2, tokenlist[lDex-1].name, 0, lexlevel + 1, 3, unmarked, 0);

        table[procIdx].param = 1;
        token = getnexttoken();

        if (token != rparentsym)
        {
          printf("%s\n", errormsg[14]); // "r.paren" req. "l.paren" error
          exit(1);
        }

        token = getnexttoken();
        
        if (token != semicolonsym)
        {  
          printf("%s\n", errormsg[6]); // declaration semicolon error
          exit(1);
        }
        token = getnexttoken();
        BLOCK (lexlevel + 1, 1, procIdx);
      }

			else
      {
				if (token != semicolonsym)
        {
					printf("%s\n", errormsg[6]); // declaration semicolon error
          exit(1);
        }

				token = getnexttoken();
				BLOCK (lexlevel + 1, 0, procIdx);
      }

    if (code[codeidx-1].opcode != 2 && code[codeidx-1].m != 0)
    {
      emit(1, "LIT", 0, 0); //LIT (M=0)
      emit(2, "OPR", 0, 0);
    }

    if (token != semicolonsym)
    {
      printf("%s\n", errormsg[6]); // declaration semicolon error
        exit(1);
    }

    token = getnexttoken();

    } while (token == procsym);
  }
  return numProc;
}

// capital letters functions used a lot here - large sorting conditional statement, using emit and code[]
void STATEMENT(int lexlevel)
{
  if (token == identsym)
  {
    SymIdx = SYMBOLTABLESEARCH(tokenlist[lDex - 1].name, lexlevel, 2);

    save = SymIdx;

    // if symIdx failed symbol name search
    if (SymIdx == -1)
    {
      printf("%s\n", errormsg[7]); // undeclared symbol error
      exit(1);
    }

    token = getnexttoken();

    if (token != becomessym)
    {
      printf("%s\n", errormsg[9]); // assignments req. use of ":=" error
      exit(1);
    }

    token = getnexttoken();
    //printf("hey %s %d %d\n", tokenlist[lDex-1].name, tokenlist[lDex-1].type, lDex);
    EXPRESSION(lexlevel);
    //printf("%d %d\n", tokenlist[lDex-1].type, lDex);
    emit(4, "STO", lexlevel - table[save].level, table[save].addr); // STO
    return;
  }

  if (token == callsym)
  {
    token = getnexttoken();

    if (token != identsym)
    {
      printf("%s\n", errormsg[2]); // identifier error
      exit(1);
    }

    //printf("\n%s %d\n", tokenlist[lDex-1].name, lexlevel);
    //puts("");
    SymIdx = SYMBOLTABLESEARCH(tokenlist[lDex-1].name, lexlevel, 3);
    

    if (SymIdx == -1)
    {
      printf("hey %s\n", errormsg[7]); // undeclared symbol error
      exit(1);
    }

    
    token = getnexttoken();
    if (token == lparentsym)
    {
      token = getnexttoken();

      if (table[SymIdx].param != 1)
      {
        printf("%s\n", errormsg[18]); // procedure must be declared error
        exit(1);
      }

      EXPRESSION(lexlevel);
      if (token != rparentsym)
      {
        printf("%s\n", errormsg[14]); // "r.paren" req. "l.paren" error
      exit(1);
      }

      token = getnexttoken();
    }
    else
      emit(1, "LIT", 0, 0);
    
    emit(5,"CAL", lexlevel - table[SymIdx].level, table[SymIdx].val);
    return;
  }

  if (token == returnsym)
  {
    if (lexlevel == 0)
    {
      printf("%s\n", errormsg[19]);
      exit(1);
    }
    
    token = getnexttoken();
    if (token == lparentsym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 0);

      if (token != rparentsym)
      {
        printf("%s\n", errormsg[14]);
        exit(1);
      }

      token = getnexttoken();
    }
    else
    {
      emit(1, "LIT", 0, 0); // LIT
      emit(2, "OPR", 0, 0); // RTN
    }
    return;
  }
  
  if (token == beginsym)
  {
    do
    {
      token = getnexttoken();
      //printf("%d %d\n", token, lDex-1);
      STATEMENT(lexlevel);
      
    } while (token == semicolonsym);

    if (token != endsym)
    {
      printf("%s\n", errormsg[10]); // "begin" req. "end" error
      exit(1);
    }

    token = getnexttoken();
    //printf("%d %d\n", tokenlist[lDex-1].type, lDex);
    return;
  }

  if (token == ifsym)
  {
    token = getnexttoken();
    CONDITION(lexlevel);
    jpcIdx = codeidx; // current code index

    emit(8, "JPC", 0, jpcIdx); // JPC

    if (token != thensym)
    {
      printf("%s\n", errormsg[11]); // "if" req. "then" error
      exit(1);
    }

    token = getnexttoken();
    STATEMENT(lexlevel);
    
    if (token == elsesym)
    {
      token = getnexttoken();
      jmpIdx = codeidx;
      emit(7, "JMP", 0 ,jmpIdx);

      code[jpcIdx].m = codeidx;
      STATEMENT(lexlevel);
      code[jmpIdx].m = codeidx;
    }
    else
      code[jpcIdx].m = codeidx; // current code index
      
    return;
  }

  if (token == whilesym)
  {
    token = getnexttoken();
    loopIdx = codeidx; // current code index
    CONDITION(lexlevel);

    if (token != dosym)
    {
      printf("%s\n", errormsg[12]); // "while" req. "do" error
      exit(1);
    }

    token = getnexttoken();
    jpcIdx = codeidx; // current code index
    // emit JPC
    emit(8, "JPC", 0, jpcIdx); // JPC
    STATEMENT(lexlevel);
    // emit JMP (M = loopIdx)
    emit(7, "JMP", 0, loopIdx); // JMP
    code[jpcIdx].m = codeidx; // current code index
    return;
  }
  
  if (token == readsym)
  {
    token = getnexttoken();

    if (token != identsym)
    {
      printf("%s\n", errormsg[2]); // identifier error
      exit(1);
    }

    SymIdx = SYMBOLTABLESEARCH(tokenlist[lDex-1].name, lexlevel, 2);

    if (SymIdx == -1)
    {
      printf("%s\n", errormsg[7]); // undeclared symbol error
      exit(1);
    }
    // if symIdx does not have kind of "var"
    // if (table[SymIdx].kind != 2)
    // {
    //   printf("%s\n", errormsg[8]); // not variable value error
    //   exit(1);
    // }

    token = getnexttoken();
    emit(9, "SYS", 0, 2); // READ

    // emit STO (M = table[SymIndx].addr) (L = lexlevel - symboltable[symIdx].level)
    emit(4, "STO", lexlevel - table[SymIdx].level, table[SymIdx].addr); // STO
    return;
  }

  if (token == writesym)
  {
    token = getnexttoken();
    EXPRESSION(lexlevel);
    emit(9, "SYS", 0, 1); // WRITE
    return;
  }
}

// inside parts of conditionals aren't translated from pseudocode, all are really similar tho
void CONDITION(int lexlevel)
{
  if (token == oddsym)
  {
    token = getnexttoken();
    EXPRESSION(lexlevel);
    emit(2, "OPR", 0, 8); // ODD
  }
  else
  {
    EXPRESSION(lexlevel);
    if (token == eqlsym )
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 8); // EQL
    }
    else if (token == neqsym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 1); // NEQ
    }
    else if (token == lessym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 10); // LSS
    }
    else if (token == leqsym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 11); // LEQ
    }
    else if (token == gtrsym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 12); // GTR
    }
    else if (token == geqsym)
    {
      token = getnexttoken();
      EXPRESSION(lexlevel);
      emit(2, "OPR", 0, 13); // GEQ
    }
    else
    {
      printf("%s\n", errormsg[13]); // "condition" req. "comparison" error
      exit(1);
    }
  }
}

// kind of big, doing a lot with the conditionals to do token checks (also using while loops)
void EXPRESSION(int lexlevel)
{
  if (token == minussym)
  {
    token = getnexttoken();
    TERM(lexlevel);
    emit(2, "OPR", 0, 1); // NEG
    while (token == plussym || token == minussym)
    {
      if (token == plussym)
      {
        token = getnexttoken();
        TERM(lexlevel);
        emit(2, "OPR", 0, 2); // ADD
      }
      else
      { 
        token = getnexttoken();
        TERM(lexlevel);
        emit(2, "OPR", 0, 3); // SUB
      }
    }
  }
  else
  {
    if (token == plussym)
      token = getnexttoken();

    TERM(lexlevel);

    while (token == plussym || token == minussym)
    {
      if (token == plussym)
      {
        token = getnexttoken();
        TERM(lexlevel);
        emit(2, "OPR", 0, 2); // ADD
      }
      else
      {
        token = getnexttoken();
        TERM(lexlevel);
        emit(2, "OPR", 0, 3); // SUB
      }
    }
  }
}

// using FACTOR() to choose which conditional to emit (I think)
void TERM(int lexlevel)
{
  FACTOR(lexlevel);
  while (token == multsym || token == slashsym || token == modsym)
  {
    if (token == multsym)
    {
      token = getnexttoken();
      FACTOR(lexlevel);
      emit(2, "OPR", 0, 4); // MUL //used to be val
    }
    else if (token == slashsym)
    {
      token = getnexttoken();
      FACTOR(lexlevel);
      emit(2, "OPR", 0, 5); // DIV
    }
    else
    {
      token = getnexttoken();
      FACTOR(lexlevel);
      emit(2, "OPR", 0, 7); // MOD
    }
  }
}

// nearly complete, some parts still in pseudocode, doing some symbol checking & emitting 
void FACTOR(int lexlevel)
{
  if (token == identsym)
  {
    SymIdxV = SYMBOLTABLESEARCH(tokenlist[lDex-1].name, lexlevel, 2);
    SymIdxC = SYMBOLTABLESEARCH(tokenlist[lDex-1].name, lexlevel, 1);

    if (SymIdxV == -1 && SymIdxC == -1)
    {
      printf("%s\n", errormsg[7]); // undeclared symbol error
      exit(1);
    }
    else if (SymIdxC == -1 || (SymIdxV != -1 && table[SymIdxV].level > table[SymIdxC].level))
    {
      emit(3, "LOD", lexlevel - table[SymIdxV].level, table[SymIdxV].addr); // LOD
      token = getnexttoken();
    }
    else
    {
      emit(1, "LIT", 0, (table[SymIdxC].val)); // LIT
      token = getnexttoken();
    }
  }
  else if (token == numbersym)
  {
    emit(1, "LIT", 0, tokenlist[lDex-1].value); // LIT
    token = getnexttoken();
  }
  else if (token == lparentsym)
  {
    token = getnexttoken();
    EXPRESSION(lexlevel);

    if (token != rparentsym)
    {
      printf("%s\n", errormsg[14]); // "r.paren" req. "l.paren" error
      exit(1);
    }

    token = getnexttoken();
  }
  else if (token == callsym)
  {
    STATEMENT(lexlevel);
  }
  else
  {
    printf("%s\n", errormsg[15]); // operations req. arithmetics/symbols error
    exit(1);
  }
}