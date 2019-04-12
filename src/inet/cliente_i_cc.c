/** @file cliente_i_cc.c
 *  @brief Archivo principal del Servidor.
 *
 *  Contiene el programa principal que ejecutará el servidor, así como las
 *  funciones principales y aplicaciones que puede invocar el cliente.
 *
 *  @author Matias Navarro
 */

#include "../../includes/comunes.h"
#include "../../includes/funciones_cliente.h"
//#define TAM 256

static struct satelite sat;

int main(int argc, char *argv[])
{
	int sockfd, puerto, n, v;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int terminar = 0;

	char buffer[TAM];
	if (argc < 3)
	{
		fprintf(stderr, "Uso %s host puerto\n", argv[0]);
		exit(0);
	}

	puerto = atoi("6020");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR apertura de socket");
		exit(1);
	}

	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "Error, no existe el host\n");
		exit(0);
	}
	memset((char *)&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(puerto);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("conexion");
		exit(1);
	}

	//Obtengo la informacion del satelite
	setInfo();
	//Imprimo la informacion del satelite
	getInfo();

	while (1)
	{
		// printf( "Ingrese el mensaje a transmitir: " );
		// memset( buffer, '\0', TAM );
		// fgets( buffer, TAM-1, stdin );

		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			perror("escritura de socket");
			exit(1);
		}

		// Verificando si se escribió: fin
		buffer[strlen(buffer) - 1] = '\0';
		if (!strcmp("fin", buffer))
		{
			terminar = 1;
		}

		memset(buffer, '\0', TAM);
		n = read(sockfd, buffer, TAM);
		if (n < 0)
		{
			perror("lectura de socket");
			exit(1);
		}
		printf("Respuesta: %s\n", buffer);

		//Update
		if (strcmp(buffer, "update firmware") == 0)
		{
			//Actualiza la version del firmware
			updateFirmware(sockfd);
			//Obtiene informacion del satelite
			setInfo();
			//Imprime informacion del satelite
			getInfo();
		}
		//Telemetria
		else if (strcmp(buffer, "get telemetria") == 0)
		{
			//Obteniendo telemetria
			printf("Obteniendo telemetria\n");
			sleep(2);
			v = telemetria(sockfd);
			if (v == 0)
			{
				printf("Telemetria completada exitosamente\n");
			}
			else
			{
				printf("Error durante la telemetria\n");
			}
		}
		else if (strcmp(buffer, "start scanning") == 0)
		{
			//Comienza el escaneo de la tierra
			printf("Comenzando el escaneo\n");
			v = startScanning(sockfd);
			if (v == 0)
			{
				printf("Escaneo realizado con exito\n");
			}
			else
			{
				printf("Error durante el escaneo\n");
			}
		}

		if (terminar)
		{
			printf("Finalizando ejecución\n");
			exit(0);
		}
	}
	return 0;
}

/**
 * @brief Funcion que actualiza los campos ID, uptime,version y consumoCPU
 * del satelite. 
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro.
 */

//Actualiza la informacion del satelite
void setInfo()
{
	FILE *versionFile;
	FILE *consumoFile;
	char buffer[TAM];
	char stateFile[TAM];
	char pid[TAM];

	strcpy(sat.ID, "ARCOR SAT");
	strcpy(sat.uptime, "11:20 a.m");

	//Lee la version del firmware del Archivo firmwareClient.bin para cargar la version
	versionFile = fopen("../../bin/firmwareCliente.bin", "r");
	bzero(buffer, sizeof(buffer));
	fread(buffer, 1, sizeof(buffer) - 1, versionFile);
	strcpy(sat.version, buffer);
	fclose(versionFile);
	memset(buffer, 0, sizeof(buffer));

	//Obtiene el pid del proceso para poder saber el consumo
	strcpy(stateFile, "ps -Ao vsize,pid,pcpu | grep ");
	sprintf(pid, "%ld", (long)getpid());
	strcat(stateFile, pid);
	strcat(stateFile, " >> ../../bin/stateFile.bin");
	system("rm ../../bin/stateFile.bin");
	system(stateFile);
	consumoFile = fopen("../../bin/stateFile.bin", "r");
	fread(buffer, 1, sizeof(buffer) - 1, consumoFile);
	strtok(buffer, "\n");
	strcpy(sat.consumoCPU, buffer);
	fclose(consumoFile);
}

/**
 * @brief Funcion que imprime el estado actual de todos los campos del satelite
 * ID, uptime, version y consumoCPU.
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro.
 */
//Imprime la informacion actual del satelite
void getInfo()
{
	printf("\n");
	printf("ID: %s\n", sat.ID);
	printf("Uptime: %s\n", sat.uptime);
	printf("Version: %s\n", sat.version);
	printf("Consumo: %s\n", sat.consumoCPU);
	printf("\n");
}

/**
 * @brief Funcion que actualiza el firmware del satelite, a partir de un archivo binario
 * enviado por el servidor.
 * @param sockfd: socket por donde se envian y reciben los datos.
 * @date 05/04/2019.
 * @author Navarro, Matias Alejandro.
 */
void updateFirmware(int sockfd)
{
	FILE *firmware;
	char buffer[TAM];
	int size, reciv_size = 0, read_size, packet_size = 0, num_packet = 0, n;
	memset(buffer, 0, sizeof(buffer));

	//Mensaje de confirmacion que se encuentra listo para el update
	n = write(sockfd, "OK", sizeof("OK"));
	if (n < 0)
	{
		printf("Error en el update\n");
		return;
	}

	//Recibe el tamaño del archivo
	packet_size = read(sockfd, &size, sizeof(size));
	if (packet_size < 0)
	{
		printf("Error en el update\n");
		return;
	}
	printf("Tamañano del archivo de update: %i\n", packet_size);
	//Verificacion del tamaño
	write(sockfd, &size, sizeof(size));

	//Abre el archivo donde escribira los datos de la actualizacion
	firmware = fopen("../../bin/firmwareCliente.bin", "w");
	if (firmware == NULL)
	{
		printf("Error al abrir el archivo\n");
		printf("Error en el update");
		return;
	}

	while (reciv_size < size)
	{
		memset(buffer, 0, sizeof(buffer));
		//Lee el tamaño del paquete
		packet_size = read(sockfd, buffer, sizeof(buffer));
		if (packet_size < 0)
		{
			printf("Error en el update");
			return;
		}

		printf("Paquete %i recibido correctamente\n", num_packet);
		printf("Tamaño del paquete: %i\n", packet_size);

		read_size = fwrite(buffer, 1, packet_size, firmware);
		if (read_size != packet_size)
		{
			printf("Error de escritura\n");
			printf("Error en el update");
			return;
		}

		reciv_size += packet_size;
		num_packet++;

		printf("Tamaño total del binario recibido: %i\n", reciv_size);
	}

	fclose(firmware);
	printf("Actualizando firmware ... \n");
}

/**
 * @brief Funcion encargada de enviar los campos del satelite al servidor.
 * Los campos enviados son ID, Update, Version de Firmware y Consumo de CPU.
 * @param sockfd: socket por el cual se envian los datos al servidor. 
 * @return
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro 
 */
int telemetria(int sockfd)
{
	// socklen_t size;
	// struct sockaddr_un envio;

	char buffer[TAM];
	int n;
	memset(buffer, 0, sizeof(buffer));

	n = read(sockfd, buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error en la escritura\n");
	}

	if (strcmp(buffer, "OK") == 0)
	{
		printf("Sincronizacion correcta\n");
	}
	else
	{
		return -1;
	}
	memset(buffer, 0, sizeof(buffer));

	strcpy(buffer, "ID: ");
	strcat(buffer, sat.ID);
	strcat(buffer, "\nUpdate: ");
	strcat(buffer, sat.uptime);
	strcat(buffer, "\nVersion: ");
	strcat(buffer, sat.version);
	strcat(buffer, "\nConsumo: ");
	strcat(buffer, sat.consumoCPU);
	strcat(buffer, "\n");

	// printf("%s\n",buffer);
	n = write(sockfd, buffer, sizeof(buffer));
	if (n < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}

	return -1;
}

/**
 * @brief Funcion 
 * @author Navarro, Matias Alejandro
 * @param 
 * @date 05/04/2019
 * @return 
 */
int startScanning(int sockfd)
{
	FILE *picture;
	char buffer[TAM];
	char archivo[32000];
	int size, read_size, num_packet = 1, n;
	//Limpia lo buffers
	memset(buffer, 0, sizeof(buffer));
	memset(archivo, 0, sizeof(archivo));

	n = read(sockfd, buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error al leer el socket");
		return -1;
	}
	//Comprueba la sincronizacion servidor-satelite
	if (strcmp(buffer, "OK") == 0)
	{
		printf("Sincronizacion Correcta\n");
	}
	else
	{
		printf("Error en la sincronizacion\n");
		return -1;
	}

	//Abre la foto
	picture = fopen("../../bin/scanning.jpg", "r");
	if (picture == NULL)
	{ //Comprueba que el archivo no este vacio
		printf("Error al cargar el binario\n");
		return -1;
	}

	fseek(picture, 0, SEEK_END);
	size = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	printf("Size %i\n", size);

	n = write(sockfd, &size, sizeof(size));
	if (n < 0)
	{
		printf("Error en el update\n");
		return -1;
	}

	n = read(sockfd, &buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error en el update\n");
		return -1;
	}

	while (!feof(picture))
	{
		//Lee del archivo y lo coloca en el buffer
		read_size = fread(archivo, 1, sizeof(archivo) - 1, picture);

		//Envia el dato
		write(sockfd, archivo, read_size);
		num_packet++;

		//Limpia el buffer
		memset(archivo, 0, sizeof(archivo));
	}

	printf("Cantidad de paquetes enviados: %i\n", num_packet);
	fclose(picture);
	sleep(2);
	printf("Actualizacion exitosa\n");
	return 0;
}