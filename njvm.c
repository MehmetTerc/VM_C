#include <stdio.h>
#include "instructions.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define VERSION 4
#define MAXSIZE 10000

int sp = 0; //Stackpointer
int pc = 0; //Programmcounter

unsigned int *program_memory;
int stack[MAXSIZE];
unsigned int ProgramCounter;
bool halt = false;
char *file_Path;
FILE *filePointer;
char charNJBF[4];
unsigned int *SDA;
unsigned int FramePointer = 0;
int opCode = 1;
unsigned int SDASize;

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

//Write the full Stack
void readStack()
{
    printf("\t------Stack------\n");
    for (int i = sp; i >= 0; i--)
    {
        if (sp == i)
        {
            printf("SP ->\n");
        }
        else if (FramePointer == i)
        {
            printf("FP ->");
            printf("\tPosition: %d  Element %d\n", i, stack[i]);
        }
        else
        {
            printf("\tPosition: %d  Element %d\n", i, stack[i]);
        }
    }
}

//Write the SDA
void readSDA()
{

    printf("\t---SDA---\n");
    for (int i = 0; i < SDASize; i++)
    {
        printf("SDA[%d] -> %d\n", i, SDA[i]);
    }
}

//read a file
void fileReader(int argc, char *argv[])
{
    file_Path = argv[1];
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

    fread(&SDASize, sizeof(unsigned int), 1, filePointer);

    SDA = malloc(SDASize * sizeof(unsigned int));

    if (SDA == NULL)
    {
        perror("SDA is Empty!\n");
        exit(1);
    }

    fread(program_memory, sizeof(unsigned int), instructionsSize, filePointer);
}

//The Execution of the Ninja Compiler
void execute()
{
    unsigned int ins = program_memory[pc];
    pc++;
    opCode = ins >> 24;
    int immediate = SIGN_EXTEND(IMMEDIATE(ins));
    int val1, val2, val3, tmp;
    char cval;
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
        printf("Insert a Character please!\n");
        scanf("%c", &cval);
        push((int)cval);
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
            push(true);
        }
        else
        {
            push(false);
        }
        break;
    case NE:
        tmp = pop();
        val2 = pop();
        if (tmp != val2)
        {
            push(true);
        }
        else
        {
            push(false);
        }
        break;
    case LT:
        tmp = pop();
        val2 = pop();
        if (val2 < tmp)
        {
            push(true);
        }
        else
        {
            push(true);
        }
        break;
    case LE:
        tmp = pop();
        val2 = pop();
        if (val2 <= tmp)
        {
            push(true);
        }
        else
        {
            push(false);
        }
        break;
    case GT:
        tmp = pop();
        val2 = pop();
        if (val2 > tmp)
        {
            push(true);
        }
        else
        {
            push(false);
        }
        break;
    case GE:
        tmp = pop();
        val2 = pop();
        if (val2 >= tmp)
        {
            push(true);
        }
        else
        {
            push(false);
        }
        break;
    case JMP:
        pc = immediate;
        break;
    case BRF:
        if (pop() == false)
        {
            pc = immediate;
        }
        break;
    case BRT:
        if (pop() == true)
        {
            pc = immediate;
        }
        break;
    case CALL:
        push(pc+1)
        pc = immediate
        break;
    case RET:
        break;
    case DROP:
        break;
    case PUSHR:
        break;
    case POPR:
        break;
    case DUP:
        break;
    default:
        printf("Unbekannte Eingabe!\n");
        break;
    }
}

//The Debugger
void debug()
{
    int a;
    printf("Debugger is started. Select an operation:\n");
    printf("[0] Show the Stack [1] Show the SDA [2] Only the next Instruction [3] Execute all Instructions [4] Exit\n");
    scanf("%d", &a);
    switch (a)
    {
    case 0:
        readStack();
        debug();
        break;
    case 1:
        readSDA();
        debug();
        break;
    case 2:
        execute();
        debug();
        break;
    case 3:
        printf("Ninja Virtual Machine started\n");
        while (opCode != HALT)
        {
            execute();
        }
        printf("Ninja Virtual Machine stoped\n");
        break;
    case 4:
        printf("Ninja Virtual Machine stopped\n");
        exit(0);
        break;
    default:
        printf("Please write a right number\n");
        debug();
    }
}

int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("usage: ./njvm [option] [option] ...\n --version        show version and exit\n --help           show this help and exit\n");
        }
        else if (strcmp(argv[1], "--version") == 0)
        {
            printf("Ninja Virtual Machine version 0 (compiled April 18 2021, 16:34:52)\n");
        }
        else if (strchr(argv[1], '.') != NULL)
        {
            char *tmp = strrchr(argv[1], '.');
            if (strcmp(tmp + 1, "bin") == 0)
            {

                fileReader(argc, argv);
                if (argc > 2)
                {
                    if (strcmp(argv[2], "--debug") == 0)
                    {
                        debug();
                    }
                    else
                    {
                        printf("Ninja Virtual Machine started\n");
                        while (opCode != HALT)
                        {
                            execute();
                        }
                        printf("Ninja Virtual Machine stopped\n");
                    }
                }
                else
                {
                    printf("teest\n");
                    printf("Ninja Virtual Machine started\n");
                    while (opCode != HALT)
                    {
                        execute();
                    }
                    printf("Ninja Virtual Machine stopped\n");
                }
            }
            else
            {
                printf("unknown command line argument '%s', try './njvm --help'\n", argv[1]);
            }
        }

        return 0;
    }
}