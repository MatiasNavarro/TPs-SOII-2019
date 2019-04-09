/** @file funciones_servidor.h
 *  @brief Libreria principal del servidor
 *
 *  Libreria del servidor, con definiciones de mensajes, especificaciones de 
 *  usuarios y contraseñas válidos, constantes, estructuras y prototipos de
 *  funciones.
 *
 *  @author Navarro, Matias Alejandro
 */

#include <ctype.h>
#include <stddef.h>
#define CANT 6

/*!< Estructura de usuarios, con campo uname (Nombre de Usuario)
y campo pass (Contrasena). */
struct user
{
	char uname[20];
	char pass[20];
} users[CANT];


/*!< Prototipos de funciones */

void setUsers(void);			//Carga los usuarios
int userLog(char[]); 			//Autenticacion de ingreso
void getpromp(char[]); 			//Generacion de promp
void updateFirmware(int);		//Actualizacion de Firmware
int startScanning(int);			//Inicia el escaneo de toda la tierra
int telemetria(int,char[]);			//Envia informacion a la estacion terrena
int setComando(int,char[]);	//Ingreso de comandos que luego son enviados al satelite
void getComandosValidos(void);	//Imprime los comandos validos