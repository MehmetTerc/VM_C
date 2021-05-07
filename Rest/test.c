#include <stdio.h>

int main(int argc, char * argv[]) {
     for (int i = 1; i < argc; i++)
  {
    
    if (strcmp(argv[i], "--help") == 0)
    {
      printf("usage: ./njvm [option] [option] ...\n --version        show version and exit\n --help           show this help and exit\n");
    }
} }