/** @file funciones_estaciones.c
 *  @brief Archivo con funciones extras del servidor.
 *
 *  Contiene funciones extras que necesita el servidor.
 *
 *  @author Facundo Maero
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/**
* @brief Saltea lineas en un archivo.
*
* Dado un descriptor de archivo y un numero de lineas, las saltea leyéndolas
* y descartando el resultado.
*
* @args *stream Descriptor del archivo donde se quieren saltar lineas.
* @args lines Cantidad de lineas a saltar.
*/
int 
skip_lines(FILE* stream, int lines){
    
    for (int i = 0; i < lines; ++i)
    {
        fscanf(stream, "%*[^\n]\n");
    }
    return 0;
}