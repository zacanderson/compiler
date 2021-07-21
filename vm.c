// Gilberto Medero
// Zachary Anderson

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compiler.h"
#include <math.h>

#define MAX_STACK_HEIGHT 500
#define MAX_CODE_LENGTH 500

void vm(instruction *text, int print);

// typedef struct instruction
// {
// 	int opcode;
// 	int l;
// 	int m;
// } instruction;

void execute(instruction *code, int print)
{
  vm(code, print);
	return;
}


int base(int stack[], int level, int BP)
{
	int base = BP;
	int counter = level;

	while(counter > 0)
	{
		base = stack[base];
		counter--;
	}

	return base;
}
void vm(instruction *text, int print)
{
	int SP = -1;
	int BP = 0;
	int PC = 0;
	int halt = 1;
	int count = 0;
  int A = 0;
  int i = 0;
	instruction IR;

	// instruction text[MAX_STACK_HEIGHT];
	int stack[MAX_CODE_LENGTH] = {0};
  char bar[MAX_CODE_LENGTH];

	// char* file = argv[1];
	// FILE *fp = fopen(file, "r");

	// if (fp == NULL)
	// 	printf("Could not open file\n");

  // 	while (!feof(fp))
	// {	
  //   // Reading from file
	// 	fscanf(fp, "%d %d %d\n", &text[count].opcode, &text[count].l, &text[count].m);
	// 	count++;
	// }
 
    printf("%18s%5s%5s%8s\n", "PC", "BP", "SP", "stack");
    printf("Initial values: %d%5d%6d\n", PC, BP, SP);


	while (halt == 1)
	{
		// Fetch
    A = PC;
		IR = text[A];
    PC = PC + 1;
		
		// Execute
		switch(IR.opcode)
		{
			// LIT: Pushes a constant value (literal) M onto the stack
			case 1:
				SP = SP + 1;
				stack[SP] = IR.m;
        
          printf("%2d LIT %2d%3d%6d%5d%5d%*s", A, 0,IR.m, PC, BP, SP, 3, "");
        
        i = 0;
        while (i <= SP)
        {
          printf("%d ", stack[i]);
          i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);					
        }
        printf("\n");
				break;

			// OPR: Operation to be performed on the data at the top of the stack.
			case 2:
				if (0 <= IR.m && IR.m <= 13)
				{
					switch (IR.m)
					{
						// RTN
						case 0:
							stack[BP - 1] = stack[SP];
							SP = BP - 1;
							BP = stack[SP + 2];
							PC = stack[SP + 3];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++;

                if (bar[i] == '|' && i < SP)
                  printf("%c ", bar[i]);
                else
                  bar[i] = ' ';
              }

              printf("\n");
							break;

						// NEG
						case 1:
							stack[SP] = stack[SP] * -1;
              
              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// ADD
						case 2:
							SP = SP - 1;
							stack[SP] = stack[SP] + stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// SUB
						case 3:
							SP = SP - 1;
							stack[SP] = stack[SP] - stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l, IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// MUL
						case 4:
							SP = SP - 1;
							stack[SP] = stack[SP] * stack[SP + 1];

               printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, 0,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
					      i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// DIV
						case 5:
							SP = SP - 1;
							stack[SP] = stack[SP] / stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// ODD
						case 6:
							stack[SP] = stack[SP] % 2;

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// MOD
						case 7:
							SP = SP - 1;
							stack[SP] = stack[SP] % stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// EQL
						case 8:
							SP = SP - 1;
							stack[SP] = stack[SP] == stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// NEQ
						case 9:
							SP = SP - 1;
							stack[SP] = stack[SP] != stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// LSS
						case 10:
							SP = SP - 1;
							stack[SP] = stack[SP] < stack[SP + 1];
              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");
              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// LEQ
						case 11:
							SP = SP - 1;
							stack[SP] = stack[SP] <= stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// GTR
						case 12:
							SP = SP - 1;
              
              stack[SP] = stack[SP] > stack[SP+1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						// GEQ
						case 13:
							SP = SP - 1;
							stack[SP] = stack[SP] >= stack[SP + 1];

              printf("%2d OPR %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

              i = 0;
              while (i <= SP)
              {
                printf("%d ", stack[i]);
                i++; 

                if (bar[i] == '|')
                  printf("%c ", bar[i]);
              }
              printf("\n");
							break;

						default:
						break;
					}
				}
				break;

			// LOD: Load value to top of stack from the stack location at offset M from L lexicographical                levels down
			case 3:
				SP = SP + 1;
				stack[SP] = stack[base(stack, IR.l, BP) + IR.m];

        printf("%2d LOD %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

        i = 0;
        while (i <= SP)
        {
          printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
        }
        printf("\n");
				break;

			// STO: Store value at top of stack in the stack location at offset M from L lexicographical levels          down
			case 4:
				stack[base(stack, IR.l, BP) + IR.m] = stack[SP];
				SP = SP - 1;

        printf("%2d STO %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

        i = 0;
        while (i <= SP)
        {
          printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
        }
        printf("\n");
				break;

			// CAL: Call procedure at code index M (generates new Activation Record and PC <- M)
			case 5:
				// SL, DL, RA, and P
				stack[SP + 1] = base(stack, IR.l, BP);
				stack[SP + 2] = BP;
				stack[SP + 3] = PC;
				stack[SP + 4] = stack[SP];
				BP = SP + 1;
				PC = IR.m;
        
        printf("%2d CAL %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

        i = 0;
        while (i <= SP)
        {
          printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
        }

        bar[BP] = '|';
        printf("\n");
				break;

			// INC: Allocate  M  memory  words  (increment  SP  by  M).  First  four  are  reserved  to Static           Link  (SL),  Dynamic  Link  (DL),Return Address (RA), and Parameter (P)
			case 6:
				SP = SP + IR.m;

        printf("%2d INC %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

        i = 0;
				
				while (i <= SP)
				{
					printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
				}
        printf("\n");
				break;

			// JMP: Jump to instruction M (PC <- M)
			case 7:
        PC = IR.m;
        
        printf("%2d JMP %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");
			
        i = 0;
        while (i <= SP)
				{
					printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
				}
        printf("\n");
        if (A == 18)
        exit(1);
				break;

			// JPC: Jump to instruction M if  top stack element is 0
			case 8:
				if (stack[SP] == 0)
					PC = IR.m;
				SP = SP - 1;

        printf("%2d JPC %2d%3d%6d%5d%5d%*s", A, IR.l,IR.m, PC, BP, SP, 3, "");

        i = 0;
				while (i <= SP)
				{
					printf("%d ", stack[i]);
					i++; 

          if (bar[i] == '|')
            printf("%c ", bar[i]);
				}
        printf("\n");
				break;

			// SYS:  Write the top stack element to the screen
			case 9:
        if (IR.m == 1)
        {
          printf("Top of Stack Value: %d\n", stack[SP]);
          SP = SP - 1;

          printf("%2d SYS%3d%3d%6d%5d%5d%*s", A, IR.l, IR.m, PC, BP, SP, 3, "");
          
          i = 0;
          while (i <= SP)
          {
            printf("%d ", stack[i]);
					  i++; 

            if (bar[i] == '|')
            printf("%c ", bar[i]);
          }
          printf("\n");
        }

        else if (IR.m == 2)
        {
          SP = SP + 1;
			    printf("Please Enter an Integer:\n");
			    scanf("%d", &stack[SP]);

          printf("%2d SYS%3d%3d%6d%5d%5d%*s", A, IR.l, IR.m, PC, BP, SP, 3, "");

          i = 0;
          while (i <= SP)
          {
            printf("%d ", stack[i]);
					  i++; 
          }
          printf("\n");
        }

        else if (IR.m == 3)
        {
          printf("%2d SYS%3d%3d%6d%5d%5d%*s", A, IR.l, IR.m, PC, BP, SP, 3, "");

          i = 0;
          while (i <= SP)
          {
            printf("%d ", stack[i]);
					  i++; 
          }
          printf("\n");
          halt = 0;
        }
				break;

			default:
				break;
		}
	}

	// fclose(fp);
	return;
}