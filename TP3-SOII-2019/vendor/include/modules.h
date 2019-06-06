#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <getopt.h>
#define MBYTE 1024
#define PALABRA 50

struct Modulo
{
    char name[PALABRA];
    int size;
    int instances_loaded;
    char status[PALABRA];
};

void getData(struct Modulo modulos[], int lines);
int parseFile(char before[], char after[], char* buff);
int count_modules();
void printPage(struct Modulo modulos[], int cant);
