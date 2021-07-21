// Gilberto Medero
// Zachary Anderson

// importing libraries for use.
#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>


// // standard enum definition (filled in for easy checking).
// typedef enum
// {
// 	modsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
// 	multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
// 	gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
// 	periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, 
// 	whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
// 	readsym = 32 , elsesym = 33, returnsym = 34
// } token_type;

// used in array of structs to print tokens in final list.
typedef struct tokens
{
  int token_type;
  char str[11];
} tokens;

// changed to global (test)
tokens tokenlist[500];

// printing the lexeme format to display results.
void defaultPrint()
{
	printf("Lexeme Table:\n");
	printf("lexeme          token type\n");
}

// deletes all whitespace within the function.
char * deleteSpaces(char *str)
{
  int count = 0;
  int size = 11;

  for (int i = 0; i < 11; i++)
  {
    if (str[i] != ' ')
    {
      str[count] = str[i];
      count++;
    }
  }

  str[count] = '\0';
  return str;
}

// input: buffer and list of tokens / search for reserved words in buffer, print, store.
bool reserves(char buffer[], tokens tokenlist[], int *ptr, int print)
{
  int n = 5;
  int ret;
  
  // checking for reserved words and assigning the "ret" value based on type.
  if(strcmp(buffer, "var") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "var", n, ' ', varsym);
    ret = varsym;
  }
  else if(strcmp(buffer, "begin") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "begin", n, ' ', beginsym);
    ret = beginsym;
  }
  else if(strcmp(buffer, "end") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "end", n, ' ', endsym);
    ret = endsym;
  }
  else if(strcmp(buffer, "const") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "const", n, ' ', constsym);
    ret = constsym;
  }
  else if(strcmp(buffer, "odd") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "odd", n, ' ', oddsym);
    ret = oddsym;
  }
  else if(strcmp(buffer, "procedure") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "procedure", n, ' ', procsym);
    ret = procsym;
  }
  else if(strcmp(buffer, "call") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "call", n, ' ', callsym);
    ret = callsym;
  }
  else if(strcmp(buffer, "if") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "if", n, ' ', ifsym);
    ret = ifsym;
  }
  else if(strcmp(buffer, "then") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "then", n, ' ', thensym);
    ret = thensym;
  }
  else if(strcmp(buffer, "do") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "do", n, ' ', dosym);
    ret = dosym;
  }
  else if(strcmp(buffer, "while") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "while", n, ' ', whilesym);
    ret = whilesym;
  }
  else if(strcmp(buffer, "write") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "write", n, ' ', writesym);
    ret = writesym;
  }
  else if(strcmp(buffer, "read") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "read", n, ' ', readsym);
    ret = readsym;
  }
  else if(strcmp(buffer, "else") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "else", n, ' ', elsesym);
    ret = elsesym;
  }
  else if(strcmp(buffer, "return") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "return", n, ' ', returnsym);
    ret = returnsym;
  }
  else
    return false;

  // assigning token to tokenlist.
  tokenlist[*ptr].token_type = ret;
  strcpy(tokenlist[*ptr].str, "");

  // true = reserved word found.
  return true;
}

// input: buffer and list of tokens / search for symbols within buffer, print, store in tokenlist.
bool specialsym (char buffer[], tokens tokenlist[], int *ptr, int print)
{
  int n = 5;
  int ret;

  //printf("hey %s\n", buffer);
  strcpy(&buffer[0], deleteSpaces(buffer));

  // checking for special characters and assigning the "ret" value based on type.
  if(strcmp(buffer, "+") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "+", n, ' ', plussym);
    ret = plussym;
  }
  else if(strcmp(buffer, "-") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "-", n, ' ', minussym);
    ret = minussym;
  }
  else if(strcmp(buffer, "*") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "*", n, ' ', multsym);
    ret = multsym;
  }
  else if(strcmp(buffer, "/") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "/", n, ' ', slashsym);
    ret = slashsym;
  }
  else if(strcmp(buffer, "(") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "(", n, ' ', lparentsym);
    ret = lparentsym;
  }
  else if(strcmp(buffer, ")") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", ")", n, ' ', rparentsym);
    ret = rparentsym;
  }
  else if(strcmp(buffer, "=") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "=", n, ' ', eqlsym);
    ret = eqlsym; 
  }
  else if(strcmp(buffer, ",") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", ",", n, ' ', commasym);
    ret = commasym;
  }
  else if(strcmp(buffer, ".") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", ".", n, ' ', periodsym);
    ret = periodsym;
  }
  else if(strcmp(buffer, "%") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", "%", n, ' ', modsym);
    ret = modsym; 
  }
  else if(strcmp(buffer, ";") == 0)
  {
    if (print)
      printf("%11s%*c%d\n", ";", n, ' ', semicolonsym);
    ret = semicolonsym;
  }
  else if(buffer[0] == '<')
  {
    if (buffer[1] == '=')
    {
      if (print)
        printf("%11s%*c%d\n", "<=", n, ' ', leqsym);
      ret = leqsym;
    }
    else if(buffer[1] == '>')
    {
      if (print)
        printf("%11s%*c%d\n", "<>", n, ' ', neqsym);
      ret = neqsym;
    }
    else if (ispunct(buffer[1]))
    {
      puts("Error : Invalid Symbol");
      exit(1);
      return false;
    }
    else
    {
      if (print)
        printf("%11s%*c%d\n", "<", n, ' ', lessym);
      ret = lessym;
    }
  }
  else if(buffer[0] == '>')
  {
    if (buffer[1] == '=')
    {
      if (print)
        printf("%11s%*c%d\n", ">=", n, ' ', geqsym);
      ret = geqsym;
    }
    else if(ispunct(buffer[1]))
    {
      puts("Error : Invalid Symbol");
      exit(1);
      return false;
    }
    else
    {
      if (print)
        printf("%11s%*c%d\n", ">", n, ' ', gtrsym);
      ret = gtrsym;
    }
  }
  else if(buffer[0] == ':')
  {
    if (buffer[1] == '=')
    {
      if (print)
        printf("%11s%*c%d\n", ":=", n, ' ', becomessym);
      ret = becomessym;
    }
    else
    {
      puts("Error : Invalid Symbol");
      exit(1);
      return false;
    }
  }
  else if(ispunct(buffer[0]))
  {
    puts("Error : Invalid Symbol");
    exit(1);
    return false;
  }
  else
    return false;

  // assigning token to tokenlist.
  tokenlist[*ptr].token_type = ret;
  strcpy(tokenlist[*ptr].str, "");
  
  // true = symbol found.
  return true;
}

void identifier(char buffer[], tokens tokenlist[], int *ptr, int print)
{
  int n = 5;
 
  strcpy(buffer, deleteSpaces(buffer));

  if(print)
    printf("%11s%*c%d\n", buffer, n, ' ', identsym);

  tokenlist[*ptr].token_type = identsym;
  strcpy(tokenlist[*ptr].str, buffer);
}

void number(char buffer[], tokens tokenlist[], int *ptr, int print)
{
  int n = 5;

  strcpy(buffer, deleteSpaces(buffer));
  
  if (print)
    printf("%11s%*c%d\n", buffer, n, ' ', numbersym);

  tokenlist[*ptr].token_type = numbersym;
  strcpy(tokenlist[*ptr].str, buffer);
}

// overwrites previous values in buffer with a single space character.
void reset(char *buffer)
{
  int i;

  for(i = 0; i < sizeof(buffer); i++)
    buffer[i] = ' ';
  
  buffer[i] = '\0';
}

// changed main to lex
// driver function for this program, reading in an input file for processing.
void lex(char *str, int print)
{
  if(print)
    defaultPrint();
  
  int i = 0, n = 0, m = 0, j = 0, cnt = 0, *ptr = &cnt, size = 0, temp = 0;
  int a, digs;
  char buffer[11] = {" "};

  i = strlen(str);

  //printf("%c %d %d\n", str[i-1], i-1, i);

  // while the position in str[] is valid.
  while(n <= i)
  {
    // If comments are detected.
    if(str[n] == '/' && str[n+1] == '*')
    {
      temp = n;
      temp+=2;

      while(str[temp] != '*' && str[temp+1] != '/')
        temp++;

      // This happens if the */ is not found.
      if(temp == i)
        break;
      
      temp+=2;
      n = temp;
    }
    
    // if the length exceeds 11, not valid.
    else if(m > 10)
    { 
      reset(buffer);

      puts("Error : Identifier names cannot exceed 11 characters");

      exit(1);

      // Loops until  it finds a punctuation. 
      while(!ispunct(str[n]))
        n++;

      // copying info into buffer and terminating sequence.
      buffer[0] = str[n];
      buffer[1] = '\0';
      
      // Checks for symbol at the end error identifier.
      if(specialsym(buffer, tokenlist, ptr, print))
      {
        *ptr = *ptr + 1;
        reset(buffer);
        n++;
        m = 0;
        continue;
      }

      j = n;
      
      // proceed to next space, then skip it, initializing n as new iterator.
      while(!isspace(str[j]))
        j++;
      
      n = j+1;
      m = 0;
      continue;
    }

    if(isalpha(str[n]))
    {
      if (ispunct(buffer[0]))
      {
        specialsym(buffer, tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
      }
      
      buffer[m] = str[n];
      buffer[m+1] = '\0';
      m++;
      n++; 
      continue;
    }

    // Checks for integers greater than 5 digits. 
    else if(isdigit(str[n]))
    {
      if(ispunct(buffer[0]))
      {
        specialsym(buffer, tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
      }

      digs = 0;
      while(isdigit(str[n]))
      {
        buffer[m] = str[n];
        digs++;
        m++;
        n++; 
      }
      
      // printing error message for invalid # of digits.
      if(digs > 5)
      {
        printf("Error : Numbers cannot exceed 5 digits\n");

        exit(1);

        while(!isspace(str[n]))
          n++;

        reset(buffer);
        m = 0;
        continue;
      }
      else if(isalpha(str[n]))
      {
        printf("Error : Identifiers cannot begin with a digit\n");

        exit(1);

        reset(buffer);
        m = 0;

        while(!ispunct(str[n]))
        {
          if(isspace(str[n]))
            break; 

          n++;
        }
        continue;
      }

      else if(isalpha(buffer[0]))
      {
        identifier(buffer, tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
        continue;
      }

      else
      {
        number(buffer,tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
        continue;
      }
    }

    // checking for spaces in str[], if buffer contains a space, then reset.
    else if(isspace(str[n]) || iscntrl(str[n]))
    { 
      if(buffer[0] == ' ')
      {
        reset(buffer);
        m = 0;
        n++;
        continue;
      }

      // checking if character stored in buffer is a reserved word.
      else if(reserves(buffer, tokenlist, ptr, print))
      {
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
        n++;
        continue;
      }

      // checking if character stored in buffer is a special symbol.
      else if(specialsym(buffer, tokenlist, ptr, print))
      {
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
        n++;
        continue;
      }

      else if(isalpha(buffer[0]))
      {
        identifier(buffer, tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
        n++;
        continue;
      }
      
      // if not a reserved word or special symbol, reset buffer.
      else
      {
        reset(buffer);
        m = 0;
        n++;
        continue;
      }
    }
    // checking for punctuation in str[], if found then buffer is reset.
    else if(ispunct(str[n]))
    {  
     // printf("hey %c %s\n", str[n], buffer);
      if (buffer[0] == ' ')
      {
        buffer[0] = str[n];
        m++;
        n++;
        
        continue;
      } 

      else if(reserves(buffer, tokenlist, ptr, print))
      {
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
      }

      else if(isalpha(buffer[0]))
      {
        (identifier(buffer, tokenlist, ptr, print));
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
      }

      else if(ispunct(buffer[0]) && str[n] == ';')
      {
        specialsym(buffer, tokenlist, ptr, print);
        *ptr = *ptr + 1;
        reset(buffer);
        m = 0;
      }

      // look ahead for another punctuation.
      else if(ispunct(str[n+1]))
      {
        buffer[m] = str[n];
        m++;
        n++;
        continue;
      }
    }
    
    buffer[m] = str[n];
    m++;
    n++;
  }
  
  if (print) // todo
  {
    printf("\nToken List:\n");

    // printing entire token list.
    for (int pos = 0; pos < *ptr; pos++)
    {
      printf("%d ", tokenlist[pos].token_type);

      if(tokenlist[pos].str[0] != '\0')
        printf("%s ", tokenlist[pos].str);
    }

    printf("\n\n");
  }

  // program completion.
	return; // tokenlist;
}


lexeme *lex_analyze(char *inputfile, int print)
{ 
  lexeme *list = malloc(500 * sizeof(lexeme));
  lex(inputfile, print);
  
  int i = 0;
  int flag = 1;
  char *temp;
  
  while (flag)
  {
    list[i].type = tokenlist[i].token_type;

    if (tokenlist[i].token_type == 3) // aka numbersym
      list[i].value = atoi(tokenlist[i].str);

    else if(tokenlist[i].str[0] != '\0')
    {
      temp = tokenlist[i].str;
      list[i].name = temp;
    }

    // printf("%d: %d %d %s\n", i,list[i].type, list[i].value, list[i].name);

   if(tokenlist[i].token_type == 19)
     flag = 0;

    i++;
  }
  
  //printf("lex is analyzed\n");
  return list;
}