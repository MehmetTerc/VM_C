#include <stdio.h>
#include "Instruktionen.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAXSIZE 100 /* StackSize */

int sp = 0; /* StackPointer */
int pc = 0; /* ProgramCounter */
unsigned int inst;
bool halt = false; /* Checking, if Program is running*/

unsigned int *program_memory; /* ProgramMemory */
int stack[MAXSIZE];

unsigned int code1[] = {
    (PUSHC << 24) | IMMEDIATE(3),
    (PUSHC << 24) | IMMEDIATE(4),
    (ADD << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (PUSHC << 24) | IMMEDIATE(6),
    (SUB << 24),
    (MUL << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)
  };

unsigned int code2[] = {
    (PUSHC << 24) | IMMEDIATE(-2),
    (RDINT << 24),
    (MUL << 24),
    (PUSHC << 24) | IMMEDIATE(3),
    (ADD << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE('\n'),
    (WRCHR << 24),
    (HALT << 24)
  };

unsigned int code3[] = {
    (RDCHR << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE('\n'),
    (WRCHR << 24),
    (HALT << 24)
  };

/**************************  FUNKTIONEN  ***********************/

void push(int i)
{
  stack[sp] = i;
  sp++;
}

int pop(void)
{
  sp--;
  return int tmp = stack[sp];
}

void add(void)
{
  int val1 = pop();
  int val2 = pop();
  int val3 = val1 + val2;
  push(val3);
}

void sub(void)
{
  int val2 = pop();
  int val1 = pop();
  int val3 = val1 - val2;
  push(val3);
}

void mul(void)
{
  int val1 = pop();
  int val2 = pop();
  int val3 = val1 * val2;
  push(val3);
}

void divide(void)
{
  int val2 = pop();
  int val1 = pop();
  if (val2 == 0)
  {
    perror("Division by Zero!\n");
    exit(1);
  }
  else
  {
    int val3 = val1 / val2;
    push(val3);
  }
}

void mod(void)
{
  int val2 = pop();
  int val1 = pop();
  if (val2 == 0)
  {
    perror("Division by Zero!\n");
    exit(1);
  }
  else
  {
    int val3 = val1 % val2;
    push(val3);
  }
}

void rdint(void)
{
  int val; /* ein ganzzahliger Datentyp */
  printf("Insert a Number please!\n");
  scanf("%d", &val); /* Wartet auf die Eingabe.    */
  push(val);
}

void wrint(void)
{
  int val = pop();
  printf("%d", val);
}

void rdchr(void)
{
  char val;
  printf("Insert a Character please!\n");
  scanf("%c", &val);
  push((int)val);
}

void wrchr(void)
{
  int val = pop();
  printf("%c" , (char)val);
}


/********************** EXECUTION OF INSTRUCTIONS ************************/
void runStack(unsigned int instructions)
{
  int opCode = instructions >> 24;
  int immediate = SIGN_EXTEND(IMMEDIATE(instructions));

  switch (opCode)
  {
  case HALT:
    halt = true;
    break;

  case PUSHC:
    push(immediate);
    break;

  case ADD:
    add();
    break;

  case SUB:
    sub();
    break;

  case MUL:
    mul();
    break;

  case DIV:
    divide();
    break;

  case MOD:
    mod();
    break;

  case RDINT:
    rdint();
    break;

  case WRINT:
    wrint();
    break;

  case RDCHR:
    rdchr();
    break;

  case WRCHR:
    wrchr();
    break;

  default:
    break;
  }
}

/*************************** MAIN  ************************************************/
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
    else if (strcmp(argv[i], "code1") == 0)
    {
      program_memory = code1;
      printf("Test1 \n");
    }
    else if (strcmp(argv[i], "code2") == 0)
    {
      program_memory = code2;
      printf("Test2 \n");
    }
    else if (strcmp(argv[i], "code3") == 0)
    {
      program_memory = code3;
      printf("Test3 \n");
    }
    else
    {
      printf("unknown command line argument '%s', try './njvm --help'\n", argv[i]);
    }
  }

  /* RUNNING STACK */
  printf("Ninja Virtual Machine started\n");
  while (!halt)
  {
    inst = program_memory[pc];
    pc++;
    runStack(inst);
  }
  printf("Ninja Virtual Machine stopped\n");
  return 0;
}