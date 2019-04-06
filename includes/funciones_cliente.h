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

/*!< Estructura de satelite, con campo ID (Identificador del satelite), 
campo uptime(Tiempo que lleva vivo), campo version(Version del Firmware), campo consumoCPU (consumo de CPU). */
struct satelite{
    int ID;
    char uptime[20];
    char version[20];
    char consumoCPU[20];
}

void setInfo(void);             //Carga inicial de los datos del satelite
int updateFirmware(int);    //Actualizacion de Firmware
int startScanning(int);     //Inicia el escaneo de toda la tierra
int telemetria(int);        //Envia informacion a la estacion terrena