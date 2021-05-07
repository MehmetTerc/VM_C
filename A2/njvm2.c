#include <stdio.h>
#include "instructions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAXSIZE 100 // Größe des Stacks

int sp = 0; //Stackpointer
int pc = 0; //Programmcounter

unsigned int *program_memory;
int stack[MAXSIZE];
unsigned int counter;
bool halt = false;




// Push
void push(int i)
{
  stack[sp] = i;
  sp++;
}

// Pop
int pop(void)
{
  sp--;
  return stack[sp];
}

  void rdchr(void)
{
  char val;
  printf("Insert a Character please!\n");
  scanf("%c", &val);
  push((int)val);
}
    


void execute(unsigned int ins)
{
  int opCode = ins >> 24;
  int immediate = SIGN_EXTEND(IMMEDIATE(ins));
  int val1, val2, val3, tmp;


  switch (opCode)
  {
  case HALT:
    halt = true;
    break;

  case PUSHC:
    push(immediate);
    break;

  case ADD:
    val1 = pop();
    val2 = pop();
    val3 = val1 + val2;
    push(val3);
    break;

  case SUB:
    val2 = pop();
    val1 = pop();
    val3 = val1 - val2;
    push(val3);
    break;

  case MUL:
    val1 = pop();
    val2 = pop();
    val3 = val1 * val2;
    push(val3);
    break;

  case DIV:
    val2 = pop();
    val1 = pop();
    if (val2 == 0)
    {
      perror("Division by Zero!!!\n");
      exit(1);
    }
    else
    {
      val3 = val1 / val2;
      push(val3);
    }
    break;

  case MOD:
    val2 = pop();
    val1 = pop();
    if (val2 == 0)
    {
      perror("Division by Zero!\n");
      exit(1);
    }
    else
    {
      val3 = val1 % val2;
      push(val3);
    }
    break;

  case RDINT:
    printf("Insert a Number please!\n");
    scanf("%d", &tmp);
    push(tmp);
    break;

  case WRINT:
    tmp = pop();
    printf("%d", tmp);
    break;

  case RDCHR:
  
    rdchr();
    break;

  case WRCHR:
    tmp = pop();
    printf("%c", (char)tmp);
    break;

  default:
    printf("Unbekannte Eingabe!\n");
    break;
  }
}

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf("Give Arguments!\n");
    exit(1);
  }

  for (int i = 1; i < argc; i++)
  {
    
    if (strcmp(argv[i], "--help") == 0)
    {
      printf("usage: ./njvm [option] [option] ...\n --version        show version and exit\n --help           show this help and exit\n");
    }
    else if (strcmp(argv[i], "--version") == 0)
    {
      printf("Ninja Virtual Machine version 0 (compiled April 18 2021, 16:34:52)\n");
    }
    else
    {
      printf("unknown command line argument '%s', try './njvm --help'\n", argv[i]);
    }
  }
  
  printf("Ninja Virtual Machine started\n");
  while (!halt)
  {
    counter = program_memory[pc];
    pc++;
    execute(counter);
  }
  printf("Ninja Virtual Machine stopped\n");
  return 0;
}