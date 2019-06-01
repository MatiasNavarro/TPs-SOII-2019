/** @file funciones_estaciones.h
 *  @brief Libreria principal de los datos de las estaciones.
 *
 *  Libreria de las estaciones, con definiciones de constantes, 
 *  estructuras y prototipos de funciones.
 *
 *  @author Facundo Maero
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stddef.h>
#define TAM 512
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

const int INICIO_ESTACIONES = 2;
const int NRO_ESTACIONES = 5;
const int NRO_SENSORES = 16;
const char * NOMBRE_FUNCIONES[] = {
    "mensual_precip",
    "diario_precip",
    "descargar",
    "promedio"
};
const int NRO_FUNCIONES = (sizeof (NOMBRE_FUNCIONES) / sizeof (const char *));

/* Estructura de un sensor, con campo binario esta (disponible o no),
y campo nombre. */
struct sensor_disponible
{
    int esta;
    char nombreSensor[30];
};

/* Estructura principal de datos de las estaciones. Guarda el contenido de 
una línea del archivo .CSV */
struct dato_estacion 
{
   char    fecha[50];
   char    dia[20];
   float   temp;
   float   humedad;
   float   ptoRocio;
   float   precip;
   float   velocViento;
   char    direcViento[30];
   float   rafagaMax;
   float   presion;
   float   radiacion;
   float   tempSuelo1;
   float   tempSuelo2;
   float   tempSuelo3;
   float   humedadSuelo1;
   float   humedadSuelo2;
   float   humedadSuelo3;
   float   humedadHoja;
}; 

/* Estructura de una estacion, con un arreglo de datos (uno por cada 10 minutos),
la cantidad de elementos o datos que posee, los sensores instalados y sus nombres, 
su ID, nombre y ID de su localidad. */
struct Estacion 
{
    struct dato_estacion dato[4100];
    int cantElem;
    struct sensor_disponible sensores[16];
    int     numero;
    char    nombre[50];
    int     idLocalidad;
};

/* Prototipos de funciones */
size_t get_variable_offset(char variable[], int* indiceSensor);
int promediar(struct Estacion stationArray[], char variable[], float promedios[], int* indiceSensor);
int check_estacion_existente(struct Estacion estaciones[], int nro);
float mensual_precip(struct Estacion estaciones[], int nro);
int skip_lines(FILE* stream, int lines);
void check_sensores(struct Estacion stationArray[], int j, char* line2, struct sensor_disponible sensores_temp[] );
void descargar_estacion(int numero, struct Estacion stationArray[], FILE* stream, char filename[]);
void procesar_input(int newsockfd, struct Estacion stationArray[], char buffer[], FILE* stream);
int split_line(char *line, char** tokens);
void diarioPrecipitacion(struct Estacion estaciones[], int nro, float precipitaciones[], int index_dias[]);
void leer_archivo(struct Estacion stationArray[10], FILE* stream);
int parseString(char cadena[], char before[], char after[], char token[]);
void print_page_header(char titulo[]);
int traducir_nombre_sensor(char sensor[], char traduccion[], char unidades[]);
void print_tabla_promedios(struct Estacion stationArray[], float promedios[], char unidades[]);
void print_precipitaciones_diarias(struct Estacion stationArray[], int station_index, float datos[], int index_dias[]);
void print_precipitaciones_mensual(float precipitaciones);
void print_descarga(char filename[]);