/** @file funciones_servidor_cc.h
 *  @brief Libreria principal del servidor
 *
 *  Libreria del servidor, con definiciones de mensajes, especificaciones de 
 *  usuarios y contraseñas válidos, constantes, estructuras y prototipos de
 *  funciones.
 *
 *  @author Facundo Maero
 */

#include <ctype.h>
#include <stddef.h>
#define errormsg "ERROR"
#define okmsg "OK"
#define arg_errorMsg "Comando incorrecto"
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

const char correct_user[20] = "facundo";
const char correct_pw[20] = "alfajor";
const int INICIO_ESTACIONES = 2;
const int NRO_ESTACIONES = 5;
const int NRO_SENSORES = 16;
const char * NOMBRE_FUNCIONES[] = {
    "ayuda",
    "desconectar",
    "listar",
    "mensual_precip",
    "diario_precip",
    "descargar",
    "promedio"
};
const int NRO_FUNCIONES = (sizeof (NOMBRE_FUNCIONES) / sizeof (const char *));

char welcome_message[TAM] = "Opciones disponibles:\n"
									"	* listar\n"
									"	* descargar <nro_estacion>\n"
									"	* diario_precip <nro_estacion>\n"
									"	* mensual_precip <nro_estacion>\n"
									"	* promedio <variable>\n"
									"	* desconectar\n"
									"	* ayuda\n";

/*!< Estructura de un sensor, con campo binario esta (disponible o no),
y campo nombre. */
struct sensor_disponible
{
    int esta;
    char nombreSensor[30];
};

/*!< Estructura principal de datos de las estaciones. Guarda el contenido de 
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

/*!< Estructura de una estacion, con un arreglo de datos (uno por cada 10 minutos),
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

/*!< Prototipos de funciones */
int initialize_udp_client_with_args(socklen_t *tamano_direccion , struct sockaddr_in* dest_addr, char address[]);
size_t get_variable_offset(char variable[], int* indiceSensor);
int promediar(struct Estacion stationArray[], char variable[], int newsockfd);
int check_estacion_existente(struct Estacion estaciones[], int *nro);
void mensual_precip(struct Estacion estaciones[], int nro, int newsockfd);
void listar(struct Estacion estaciones[], int newsockfd);
void show_help(int newsockfd);
int skip_lines(FILE* stream, int lines);
void check_sensores(struct Estacion stationArray[], int j, char* line2, struct sensor_disponible sensores_temp[] );
void send_udp(int sockfd, char buffer[], struct sockaddr_in* serv_addr, socklen_t tamano_direccion);
void recv_udp(int sockfd, char buffer[], struct sockaddr_in* serv_addr, socklen_t* tamano_direccion);
void descargar_estacion(int numero, struct Estacion stationArray[], int newsockfd, FILE* stream);
void procesar_input(int newsockfd, struct Estacion stationArray[], char buffer[], FILE* stream);
int send_to_socket(int sockfd, char cadena[]);
int read_from_socket(int sockfd, char buffer[]);
void start_server(int* sockfd, socklen_t* clilen, struct sockaddr_in* serv_addr,struct sockaddr_in* cli_addr);
int split_line(char *line, char** tokens);
void arg_error(int newsockfd);