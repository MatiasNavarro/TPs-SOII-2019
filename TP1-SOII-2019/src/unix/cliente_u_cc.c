/** @file cliente_u_cc.c
 *  @brief Archivo principal del Cliente.
 *
 *  Contiene el programa principal que ejecutará el cliente.
 *
 *  @author Navarro, Matias Alejandro
 */

#include "../../includes/comunes.h"
#include "../../includes/funciones_cliente.h"

static struct satelite sat;

int main(int argc, char *argv[])
{
	int sockfd, servlen,v;
	struct sockaddr_un serv_addr;
	char buffer[TAM];

	if (argc < 2)
	{
		fprintf(stderr, "Uso %s archivo\n", argv[0]);
		exit(0);
	}

	memset((char *)&serv_addr, '\0', sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, argv[1]);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perror("creación de socket");
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
	{
		perror("conexión");
		exit(1);
	}

	//Iniciando satelite
	printf("\nIniciando el satelite...\n");
	//Obtengo la informacion del satelite
	setInfo();
	//Imprimo la informacion del satelite
	getInfo();

	while (1)
	{
		int n;
		memset(buffer, '\0', TAM);

		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			perror("escritura de socket");
			exit(1);
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
			printf("\nUpdate firmware\n");
			//Actualiza la version del firmware
			updateFirmware(sockfd,argv);
		}
		//Telemetria
		else if(strcmp(buffer,"get telemetria")==0){
			//Obteniendo telemetria
			printf("\nObteniendo telemetria\n");
			sleep(1);
			v = telemetria();
			if(v==0)
			{
				printf("Telemetria completada exitosamente\n");
			}
			else
			{
				printf("Error durante la telemetria\n");
			}
		}
		else if(strcmp(buffer,"start scanning")==0){
			//Comienza el escaneo de la tierra
			printf("\nComenzando el escaneo\n");
			v = startScanning(sockfd);
			if(v==0){
				printf("Escaneo realizado con exito\n");
			}
			else
			{
				printf("Error durante el escaneo\n");
			}
			
		}

		if (strcmp(buffer,"fin")==0)
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
void setInfo()
{
	FILE *versionFile;
	FILE *consumoFile;
	char buffer[TAM];
	char stateFile[TAM];
	char pid[TAM];
	//Obtiene los datos del tiempo (fecha y hora)
	time_t t;
	t=time(NULL);
	struct tm *tm =localtime(&t);
	char time[100];
	strftime(time,100, "%d/%m/%Y %H:%M:%S",tm);

	strcpy(sat.ID, "ARCOR SAT");
	strcpy(sat.uptime, time);

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
void updateFirmware(int sockfd, char *argv[])
{
	FILE *firmware;
	char buffer[TAM];
	int size, reciv_size = 0, packet_size = 0, n;
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
	//Verificacion del tamaño
	write(sockfd, &size, sizeof(size));

	//Abre el archivo donde escribira los datos de la actualizacion
	firmware = fopen("../../bin/firmwareCliente.bin", "w");

	while (reciv_size < size)
	{
		int read_size;
		memset(buffer, 0, sizeof(buffer));
		//Lee el tamaño del paquete
		packet_size = read(sockfd, buffer, sizeof(buffer));
		if (packet_size < 0)
		{
			printf("Error en el update");
			fclose(firmware);
			return;
		}

		read_size = fwrite(buffer, 1, packet_size, firmware);
		if (read_size != packet_size)
		{
			printf("Error de escritura\n");
			printf("Error en el update");
			fclose(firmware);
			return;
		}

		reciv_size += packet_size;
	}

	fclose(firmware);
	printf("Actualizando firmware ... \n");
	printf("Actualizacion exitosa\n");
	printf("Reiniciando ...\n");
	fflush(stdout);
	sleep(1);
	close(sockfd);
	execvp(argv[0],argv);
}

/**
 * @brief Funcion encargada de enviar los campos del satelite al servidor.
 * Los campos enviados son ID, Update, Version de Firmware y Consumo de CPU.
 * @param sockfd: socket por el cual se envian los datos al servidor. 
 * @return
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro 
 */
int telemetria()
{	
	int descriptor_socket, n;
	struct sockaddr_un struct_cliente;
	socklen_t tamano_direccion;
	char buffer[TAM];
	memset(buffer,0,sizeof(buffer));

	/* Creacion de socket */
	if(( descriptor_socket = socket(AF_UNIX, SOCK_DGRAM, 0) ) < 0 ) 
	{
		perror("socket" );
		return -1;
	}

	/* Inicialización y establecimiento de la estructura del cliente */
	memset(&struct_cliente, 0, sizeof( struct_cliente ));
	struct_cliente.sun_family = AF_UNIX;
	strncpy(struct_cliente.sun_path,"./socket", sizeof(struct_cliente.sun_path));
	tamano_direccion = sizeof(tamano_direccion);

	strcpy(buffer,"ID: ");
	strcat(buffer,sat.ID);
	strcat(buffer,"\nUpdate: ");
	strcat(buffer,sat.uptime);
	strcat(buffer,"\nVersion: ");
	strcat(buffer,sat.version);
	strcat(buffer,"\nConsumo: ");
	strcat(buffer,sat.consumoCPU);
	strcat(buffer,"\n");

	n = sendto(descriptor_socket,buffer,TAM,0, (struct sockaddr *)&struct_cliente, sizeof(struct_cliente));
	if(n<0){
		//Error de lectura
		return -1;
	}
	else
	{
		//Los datos se enviaron correctamente
		return 0;
	}
	
	return -1;
}

/**
 * @brief Funcion encargada de fragmentar una imagen del satelite(cliente) y enviarla a la estacion (servidor)
 * @param sockfd: socket por donde se envia y reciben los datos.
 * @return 0 si la comunicacion no tuvo errores.
 *        -1 si ocurrio algun error.
 * @date 05/04/2019
 * @author Navarro, Matias Alejandro
 */
int startScanning(int sockfd){
	FILE *picture;
	char buffer[TAM];
	char archivo[32000];
	int size, n;
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
	printf("Tamaño del archivo: %i bytes\n", size);

	n = write(sockfd, &size, sizeof(size));
	if (n < 0)
	{
		printf("Error en el update\n");
		fclose(picture);
		return -1;
	}

	n = read(sockfd, &buffer, sizeof(buffer));
	if (n < 0)
	{
		printf("Error en el update\n");
		fclose(picture);
		return -1;
	}

	while (!feof(picture))
	{
		int read_size;
		//Lee del archivo y lo coloca en el buffer
		read_size = fread(archivo, 1, sizeof(archivo) - 1, picture);

		//Envia el dato
		write(sockfd, archivo, read_size);

		//Limpia el buffer
		memset(archivo, 0, sizeof(archivo));
	}

	fclose(picture);
	sleep(1);
	return 0;
}