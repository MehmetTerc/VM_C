#include <stdio.h>
#include "instructions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define VERSION 3
#define MAXSIZE 10000

int sp = 0; //Stackpointer
int pc = 0; //Programmcounter

unsigned int *program_memory;
int stack[MAXSIZE];
unsigned int counter;
bool halt = false;
char *file_Path;
FILE *filePointer;
char charNJBF[4];
unsigned int *SDA;
unsigned int FramePointer = 0;

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

    case PUSHG:
        push(SDA[immediate]);
        break;

    case POPG:
        tmp = pop();
        SDA[immediate] = tmp;
        break;

    case ASF:
        push(FramePointer);
        FramePointer = sp;
        sp = sp + immediate;
        break;

    case RSF:
        sp = FramePointer;
        FramePointer = pop();
        break;

    case PUSHL:
        push(stack[FramePointer + immediate]);
        break;

    case POPL:
        tmp = pop();
        stack[FramePointer + immediate] = tmp;
        break;
    case EQ:
        tmp = pop();
        val2 = pop();
        if (tmp == val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case NE:
        tmp = pop();
        val2 = pop();
        if (tmp = !val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case LT:
        tmp = pop();
        val2 = pop();
        if (tmp < val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case LE:
        tmp = pop();
        val2 = pop();
        if (tmp <= val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case GT:
        tmp = pop();
        val2 = pop();
        if (tmp > val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case GE:
        tmp = pop();
        val2 = pop();
        if (tmp >= val2)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;
    case JMP:
        counter = immediate;
        break;
    case BRF:

        break;
    case BRT:
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
        }else if(strcmp(argv[i], "--debug") == 0){
            printf("Debugger is started. Select an operation:\n[0] Show the Stack\n");
        }
        else
        {
            printf("unknown command line argument '%s', try './njvm --help'\n", argv[i]);
        }
    }

    file_Path = argv[argc - 1];
    if ((filePointer = fopen(file_Path, "r")) == NULL)
    {
        perror("The File is not there!");
        exit(1);
    }

    fread(charNJBF, sizeof(char), 4, filePointer);

    if (strncmp(charNJBF, "NJBF", 4) != 0)
    {
        perror("Wrong Directory! \n");
        exit(1);
    }
    unsigned int version;

    fread(&version, sizeof(unsigned int), 1, filePointer);

    if (version > VERSION)
    {
        perror("Wrong Version\n");
        exit(1);
    }

    unsigned int instructionsSize;

    fread(&instructionsSize, sizeof(unsigned int), 1, filePointer);

    program_memory = malloc(instructionsSize * sizeof(unsigned int));

    if (program_memory == NULL)
    {
        perror("program memory is empty\n");
        exit(1);
    }

    unsigned int SDASize;

    fread(&SDASize, sizeof(unsigned int), 1, filePointer);

    SDA = malloc(SDASize * sizeof(unsigned int));

    if (SDA == NULL)
    {
        perror("SDA is Empty!\n");
        exit(1);
    }

    fread(program_memory, sizeof(unsigned int), instructionsSize, filePointer);

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