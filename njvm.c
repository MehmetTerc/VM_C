#include <stdio.h>
#include <string.h>
#include "instructions.h"
#define MAXSIZE 100 // Größe des Stacks

int sp = 0; //Stackpointer
int pc = 0; //Programmcounter

unsigned int *program_memory;
int stack[MAXSIZE];
unsigned int ins;

unsigned int program_1[] = {

    (PUSHC << 24) | IMMEDIATE(3),
    (PUSHC << 24) | IMMEDIATE(4),
    (ADD << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (PUSHC << 24) | IMMEDIATE(6),
    (SUB << 24),
    (MUL << 24),
    (PUSHC << 24) | IMMEDIATE(10),
    (WRCHR << 24),
    (HALT << 24)};

unsigned int program_2[] = {

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

unsigned int program_3[] = {
    (RDCHR << 24),
    (WRINT << 24),
    (PUSHC << 24) | IMMEDIATE('\n'),
    (WRCHR << 24),
    (HALT << 24)

};

// Push
void push(int i)
{
  stack[sp] = i;
  sp++;
}

// Pop
int pop(void)
{
  int val;
  sp--;
  val = stack[sp];
  return val;
}

void execute(ins)
{
  int opCode = ins >> 24;
  int immediate = SIGN_EXTEND(IMMEDIATE(ins));
  int val1, val2, val3, tmp;

  switch (opCode)
  {
  case HALT:
    break;

  case PUSHC:
    push(immediate);

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
      perror("Division by Zero!\n");
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
    printf("Insert a Numberr please!\n");
    scanf("%d", tmp);
    printf("Die Zahl, die Sie eingegeben haben, war %d" + tmp);
    push(tmp);
    break;

  case WRINT:
    tmp = pop();
    printf("%d" + (int)tmp);
    break;

  case RDCHR:

    printf("Insert a Numberr please!\n");
    scanf("%d", tmp);
    printf("Die Zahl, die Sie eingegeben haben, war %d" + tmp);
    push(tmp);
    break;

  case WRCHR:
    tmp = pop();
    printf("%c" + (char)tmp);
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
    printf("Ninja Virtual Machine started\n");
    printf("Ninja Virtual Machine stopped\n");
  }

  for (int i = 1; i < argc; i++)
  {
    printf("%d\n" + argc);
    if (strcmp(argv[i], "--help") == 0)
    {
      printf("usage: ./njvm [option] [option] ...\n --version        show version and exit\n --help           show this help and exit\n");
    }
    else if (strcmp(argv[i], "--version") == 0)
    {
      printf("Ninja Virtual Machine version 0 (compiled April 18 2021, 16:34:52)\n");
    }
    else if ((strcmp(argv[i], "1") == 0))
    {
      printf("Programm 1 strtet\n");
      program_memory=program_1;
    }
    else if ((strcmp(argv[i], "1") == 0))
    {
      program_memory = program_2;
    }
    else if ((strcmp(argv[i], "1") == 0))
    {
      program_memory = program_3;
    }
    else
    {
      printf("unknown command line argument '%s', try './njvm --help'\n" + (int)argv[i]);
    }
  }

  return 0;
}