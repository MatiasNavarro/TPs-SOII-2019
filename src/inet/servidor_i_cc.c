/** @file servidor_i_cc.c
 *  @brief Archivo principal del Servidor.
 *
 *  Contiene el programa principal que ejecutara el servidor, asi como las
 *  funciones principales y aplicaciones que puede invocar el cliente.
 *
 *  @author Matias Navarro
 */
#include "../../includes/colors.h"
#include "../../includes/comunes.h"
#include "../../includes/funciones_servidor.h"

/**
* @brief Función principal del Servidor.
*
* Crea un socket, espera la conexion del satelite (cliente)
* y para mandarle comandos de las distintas funciones
*/
int main(int argc, char *argv[])
{
	int sockfd, flag = 0;
	socklen_t clilen;
	char buffer[TAM], promp[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	memset(promp,0,sizeof(promp));

	//Carga los usuarios al sistema
	setUsers();
	/*!< Inicio el proceso servidor, levanto el socket en el puerto 6020 (TCP) */
	start_server(&sockfd, &clilen, &serv_addr, &cli_addr);

	while (1)
	{
		int n, conect, newsockfd, pid;
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
		{
			perror("accept");
			exit(1);
		}

		//Comprobacion de usuarios
		while(flag==0){
			conect = userLog(promp);
			if (conect == 1)
			{
				printf("\nAutenticacion CORRECTA\n\n");
				getpromp(promp);
				n = write(newsockfd, "Servidor Conectado", sizeof("Servidor Conectado"));
				flag = 1;
			}
			else
			{
				printf("Error de autenticacion \n");
				exit(1);
			}
		}

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}

		if (pid == 0)
		{ // Proceso hijo
			close(sockfd);

			while (1)
			{
				memset(buffer, 0, TAM);
				//Ingresa uno de los comando posibles para mandar al satelite
				if (conect == 1)
				{
					n = setComando(newsockfd, promp);
					if (n == -1)
					{
						conect = 0;
						write(newsockfd, "Conexion Cerrada", sizeof("Conexion Cerrada"));
						close(newsockfd);
						exit(0);
					}
				}

				if (strcmp("fin", buffer)==0)
				{
					printf("PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid());
					exit(0);
				}
			}
		}
		else
		{
			printf("SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid);
			close(newsockfd);
		}
	}
	return 0;
}

/**
 * @brief Funcion encargada de cargar los distintos usuarios en el servidor.
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro.
 */
void setUsers()
{
	strcpy(users[0].uname, "Nicolas"), strcpy(users[0].pass, "nn26");
	strcpy(users[1].uname, "Federico"), strcpy(users[1].pass, "fn25");
	strcpy(users[2].uname, "Matias"), strcpy(users[2].pass, "mn24");
	strcpy(users[3].uname, "Ciro"), strcpy(users[3].pass, "cn5");
	strcpy(users[4].uname, "Bianca"), strcpy(users[4].pass, "bn4");
	strcpy(users[5].uname, "Geronimo"), strcpy(users[5].pass, "gn0");
}

/**
* @brief Inicializa el servidor TCP.
*
* Crea un socket TCP con los argumentos que recibe. Utiliza el puerto 6020.
* @args *sockfd
* @args clilen
* @args *serv_addr
* @args *cli_addr
* @date 05/04/2019.
* @author Navarro, Matias Alejandro.
*/
void start_server(int *sockfd, socklen_t *clilen, struct sockaddr_in *serv_addr, struct sockaddr_in *cli_addr)
{
	int puerto;

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd < 0)
	{
		perror(" apertura de socket ");
		exit(1);
	}

	memset((char *)serv_addr, 0, sizeof(*serv_addr));
	puerto = 6020;
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = INADDR_ANY;
	serv_addr->sin_port = htons(puerto);

	if (bind(*sockfd, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0)
	{
		perror("ligadura");
		exit(1);
	}

	printf("Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr->sin_port));

	listen(*sockfd, 1);
	*clilen = sizeof(*cli_addr);
}

/**
 * @brief Funcion que comprueba la autenticacion de usuarios en el servidor. 
 * Reingresa el usuario, hasta que la contraseña y el usuario sean correctas.
 * @param promp: arreglo donde se guarda el user[i].uname@ para la construccion
 * del promp si el usuario ingresado es correcto.
 * @return 1: si el usuario es correcto.
 * 		  -1: si el usuario es incorrecto.	
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro.
 */
int userLog(char promp[])
{
	char usuario[20], password[20];
	int error = 0;
	printf("\nAutenticacion de usuario\n");

	//Comprueba que el usuario sea correcto. Solo permite 3 errores
	while (error < 3)
	{
		printf("Usuario: ");
		//Ingreso del usuario
		fgets(usuario, 20, stdin);
		fflush(stdout);
		strtok(usuario, "\n");

		printf("Password: ");
		//Ingreso de la contraseña
		fgets(password, 20, stdin);
		fflush(stdout);
		strtok(password, "\n");

		//Comprobacion de Usuario y Contraseña
		for (int i = 0; i < CANT; i++)
		{
			if (strcmp(users[i].uname, usuario) == 0)
			{
				if (strcmp(users[i].pass, password) == 0)
				{
					strcat(promp, BOLDRED);
					strcat(promp, users[i].uname);
					return 1;
				}
			}
		}

		error++;
	}
	return -1;
}

/**
 * @brief Funcion que arma el promp completo user[i].uname@hostname: .
 * @param promp: arreglo donde se arma el promp
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro.
 */
void getpromp(char promp[])
{
	char hostname[TAM];

	//Concatena "@" al promp
	strcat(promp, "@");
	//Obtiene el nombre del host
	gethostname(hostname, TAM);
	strcat(promp, hostname);
	strcat(promp,RESET);
}

/**
 * @brief Funcion que permite ingresar los distintos comandos disponibles. 
 * @param 	newsockfd: socket por donde se envia y reciben los datos.
 * 	 		promp: el prop del usuario del server (Ej: "Bianca@MatiasNavarro-Linux")..
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro.
 */
int setComando(int newsockfd, char promp[])
{
	char buffer[TAM];
	char infoSat[TAM];
	int v;

	//Bucle que espera los comandos
	while (1)
	{
		printf("%s: ", promp);
		memset(buffer, 0, sizeof(buffer));
		//Ingreso de comando
		fgets(buffer, sizeof(buffer) - 1, stdin);
		strtok(buffer, "\n");

		//Comprueba que tipo de comando se ingreso
		//Update
		if (strcmp(buffer, "update firmware") == 0)
		{
			printf("Actualizando Firmware\n");
			write(newsockfd, buffer, sizeof(buffer));
			sleep(1);
			updateFirmware(newsockfd);
			close(newsockfd);
			exit(0);
		}
		//Telemetria
		else if (strcmp(buffer, "get telemetria") == 0)
		{
			printf("Obteniendo telemetria\n \n");
			write(newsockfd, buffer, sizeof(buffer));
			sleep(1);
			v = telemetria(infoSat);
			if (v == 0)
			{
				printf("Telemetria completada exitosamente\n");
			}
			else
			{
				printf("Error durante la telemetria\n");
			}
		}
		//Star Scanning
		else if (strcmp(buffer, "start scanning") == 0)
		{
			printf("Comenzando el escaneo\n");
			write(newsockfd,buffer,sizeof(buffer));
			v = startScanning(newsockfd);
			sleep(2);
			if(v==0){
				printf("Escaneo completo\n");
			}
			else
			{
				printf("Error durante el escaneo\n");
			}
		}
		//Help
		else if (strcmp(buffer, "help") == 0)
		{
			//Imprime los comandos disponibles
			getComandosValidos();
		}
		//Exit
		else if (strcmp(buffer, "exit") == 0)
		{
			printf("Apagando Sistema\n");
			return -1;
		}
		//Comando invalido
		else
		{
			printf("Comando invalido. Ingrese 'help' para mas informacion\n");
		}
	}
	return -1;
}

/**
 * @brief Funcion que imprime los comandos validos que puede se pueden ingresar desde
 * el servidor.
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro
 */
void getComandosValidos()
{
	printf("\n");
	printf("- update firmware: actualiza el firmware del satelite\n");
	printf("- get telemetria: obtiene los datos del satelite\n");
	printf("- start scanning: comienza el escaneo de la tierra\n");
	printf("- exit: apagar el sistema\n");
	printf("\n");
	fflush(stdout);
}

/**
 * @brief Funcion que actualiza el firmware del satelite. Lee un archivo binario y lo envia.
 * @param newsockfd: socket por donde se envia y reciben los datos.
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro
 */
void updateFirmware(int newsockfd)
{
	FILE *firmware;
	char buffer[TAM], send[TAM];
	int size, n;
	//Limpia lo buffers
	memset(buffer, 0, sizeof(buffer));
	memset(send, 0, sizeof(send));

	n = read(newsockfd, buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error al leer el socket");
		return;
	}
	//Compruebo que el satelite este listo para la actualizacion
	if (strcmp(buffer, "OK") == 0)
	{
		printf("Satelite Listo\n");
	}
	else
	{
		printf("Error en el update OK\n");
		return;
	}

	//Abre el binario
	firmware = fopen("../../bin/firmwareUpdate.bin", "r");

	fseek(firmware, 0, SEEK_END);
	size = ftell(firmware);
	fseek(firmware, 0, SEEK_SET);

	n = write(newsockfd, &size, sizeof(size));
	if (n < 0)
	{
		printf("Error en el update write\n");
		fclose(firmware);
		return;
	}

	n = read(newsockfd, &buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error en el update read\n");
		fclose(firmware);
		return;
	}

	while (!feof(firmware))
	{
		int read_size;
		//Lee del archivo y lo coloca en el buffer
		read_size = fread(send, 1, sizeof(size) - 1, firmware);

		//Envia el dato
		write(newsockfd, send, read_size);

		//Limpia el buffer
		memset(send, 0, sizeof(send));
	}

	fclose(firmware);
	printf("Actualizacion exitosa\n");
	sleep(1);
}

/**
 * @brief Funcion que obtiene la informacion de los distintos campos del cliente (satelite),
 * como son el ID, Update, Version de Firmware y Consumo de CPU. 
 * @param infoStat: buffer donde se va a guardar los datos recibidos desde el satelite.
 * @return 0 si la comunicacion no tuvo errores.
 *        -1 si ocurrio algun error.
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro
 */
int telemetria(char infoSat[])
{
	int socket_server, puerto, n;
	struct sockaddr_in serv_addr;
	socklen_t tamano_direccion;
	char buffer[TAM];
	memset(buffer, 0, sizeof(buffer));

	/* Creacion del socket */
	if ((socket_server = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("ERROR en apertura de socket");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	puerto = atoi("6025");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(puerto);
	memset(&(serv_addr.sin_zero), '\0', 8);

	if (bind(socket_server, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR en binding");
		return -1;
	}

	tamano_direccion = sizeof(struct sockaddr);

	n = recvfrom(socket_server, buffer, TAM - 1, 0, (struct sockaddr *)&serv_addr, &tamano_direccion);
	if (n < 0)
	{
		//Error en la lectura
		return -1;
	}

	strcpy(infoSat, buffer);
	printf("%s\n", infoSat);
	fflush(stdout);
	close(socket_server);

	return 0;
}

/**
 * @brief Funcion encargada de recibir y construir la imagen enviada desde el satelite.
 * @param newsockfd: socket por donde se envia y reciben los datos.
 * @return 0 si la comunicacion no tuvo errores.
 *        -1 si ocurrio algun error.
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro
 */
int startScanning(int newsockfd)
{

	FILE *picture;
	char buffer[TAM];
	char archivo[1400];
	int size = 0, reciv_size = 0, packet_size, n;
	//Limpia lo buffers
	memset(buffer, 0, sizeof(buffer));
	memset(archivo, 0, sizeof(archivo));

	n = write(newsockfd, "OK", sizeof("OK"));
	if (n < 0)
	{
		printf("Error al leer el socket");
		return -1;
	}
	printf("Sincronizacion Correcta\n");

	packet_size = read(newsockfd, &size, sizeof(size));
	if (packet_size < 0)
	{
		printf("Error en el update\n");
		return -1;
	}

	printf("Tamaño del archivo: %i bytes\n", size);
	//Verificacion del tamaño
	write(newsockfd, &size, sizeof(size));

	//Abre el archivo a "escribir"
	picture = fopen("./earth.jpg", "w");

	while (reciv_size < size)
	{
		int read_size;
		memset(archivo, 0, sizeof(archivo));
		//Lee el tamaño del paquete
		packet_size = read(newsockfd, archivo, sizeof(archivo));
		if (packet_size < 0)
		{
			printf("Error en el update");
			fclose(picture);
			return -1;
		}

		read_size = fwrite(archivo, 1, packet_size, picture);
		if (read_size != packet_size)
		{
			printf("Error de escritura\n");
			printf("Error en el update");
			fclose(picture);
			return -1;
		}

		reciv_size += read_size;
	}

	fclose(picture);
	printf("......\n");
	printf("......\n");
	printf("......\n");
	printf("......\n");
	printf("......\n");

	return 0;
}
