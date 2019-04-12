/** @file funciones_clientes.h
 *  @brief Libreria principal del cliente
 *
 *  Libreria del servidor, con definiciones de mensajes, especificaciones de 
 *  usuarios y contraseñas válidos, constantes, estructuras y prototipos de
 *  funciones.
 *
 *  @author Navarro, Matias Alejandro
 */

#include <ctype.h>
#include <stddef.h>
#define MAX 50
/*!< Estructura de satelite, con campo ID (Identificador del satelite), 
campo uptime(Tiempo que lleva vivo), campo version(Version del Firmware), campo consumoCPU (consumo de CPU). */
struct satelite{
    char ID[20];
    char uptime[MAX];
    char version[MAX];
    char consumoCPU[MAX];
};

void setInfo(void);         //Carga inicial de los datos del satelite
void getInfo(void);         //Imprime la informacion del satelite
void updateFirmware(int);    //Actualizacion de Firmware
int startScanning(int);     //Inicia el escaneo de toda la tierra
int telemetria(void);        //Envia informacion a la estacion terrena UNIX
int telemetria_inet(char *[]);     //INET