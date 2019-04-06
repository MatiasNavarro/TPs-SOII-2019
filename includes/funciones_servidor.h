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
#define CANT 5

/*!< Estructura dbase de datos de usuarios, con campo uname (Nombre de Usuario)
y campo pass (Contrasena). */
struct user
{
	char uname[20];
	char pass[20];
} users[CANT];

/*!< Prototipos de funciones */
int userLog(char[]); 	//Autenticacion de ingreso
void getpromp(char[]); 	//Generacion de promp