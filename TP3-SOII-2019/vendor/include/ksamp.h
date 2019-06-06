/**
 * @file ksamp.h
 * @brief Header con declaracion de variables y prototipos de funciones.
 *
 * Aqui se declaran las variables a usar por el programa, los prototipos de funciones
 * a utilizar, y las estructuras de datos que las mismas emplean para funcionar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <getopt.h>
#include <time.h>
#define MBYTE 1024

#define minute 60
#define hour 3600
#define day 86400

FILE *fp;
char * buff;
char buffer[256];

char path[32];
char before[32];
char after[32];

struct Datos {
	char  hostname[50];
	char  cpu[50];
	char  kernel[50];
	long  uptime;
	struct tm time;
};

struct Interval{
	int memDisp;
	int memTot;
	char load1Min[10];
	char load5Min[10];
	char load15Min[10];
};

void printData(struct Datos, struct Interval data);
int parseFile(FILE* fp, char path[], char before[], char after[], char** buff);
void calculateInterval(struct Interval*);
void getData(struct Datos*);
void printPage(struct Datos, struct Interval data);
