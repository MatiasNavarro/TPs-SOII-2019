/** @file servidor_u_cc.c
 *  @brief Archivo principal del Servidor.
 *
 *  Contiene el programa principal que ejecutará el servidor, así como las
 *  funciones principales y aplicaciones que puede invocar el cliente.
 *
 *  @author Matias Navarro
 */

/**
 * @brief Funcion 
 * @author Navarro, Matias Alejandro
 * @param 
 * @date 05/04/2019
 * @return 
 */

#include "../includes/comunes.h"
#include "../includes/funciones_servidor.h"

/**
* @brief Función principal del Servidor.
*
* Crea un socket, espera hasta 5 conexiones
* y ejecuta las funciones que recibe del cliente.
*/
int main(int argc, char *argv[])
{
	int sockfd, newsockfd, servlen, n, pid;
	unsigned int clilen;
	struct sockaddr_un cli_addr, serv_addr;
	char buffer[TAM];
	char promp[TAM];
	setUsers();
	
	/* Se toma el nombre del socket de la línea de comandos */
	if (argc != 2)
	{
		printf("Uso: %s <nombre_de_socket>\n", argv[0]);
		exit(1);
	}

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		perror("creación de  socket");
		exit(1);
	}

	/* Remover el nombre de archivo si existe */
	unlink(argv[1]);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, argv[1]);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
	{
		perror("ligadura");
		exit(1);
	}

	printf("Proceso: %d - socket disponible: %s\n", getpid(), serv_addr.sun_path);

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	while (1)
	{
		int conect;
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
		{
			perror("accept");
			exit(1);
		}

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}

		if (pid == 0)
		{ // proceso hijo
			close(sockfd);

			conect = userLog(promp);
			if (conect == 1)
			{
				printf("Autenticacion CORRECTA\n");
				getpromp(promp);
				n = write(newsockfd, "Conectado", sizeof("Conectado"));
			}
			else
			{
				printf("Error de autenticacion \n");
				exit(1);
			}

			while (1)
			{
				setComando(newsockfd,promp);

				memset(buffer, 0, TAM);
				n = read(newsockfd, buffer, TAM - 1);
				if (n < 0)
				{
					perror("lectura de socket");
					exit(1);
				}

				printf("%s ", promp);
				printf("PROCESO: %d. ", getpid());
				printf("Recibí: %s \n", buffer);
				fflush(stdout);

				n = write(newsockfd, "Obtuve su mensaje", 18);
				if (n < 0)
				{
					perror("escritura en socket");
					exit(1);
				}

				// Verificación de si hay que terminar
				buffer[strlen(buffer) - 1] = '\0';
				if (!strcmp("fin", buffer))
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
 * @brief Funcion encargada de cargar los distintos usuarios
 * @author Navarro, Matias Alejandro
 * @param 
 * @date 05/04/2019
 * @return 
 */
void setUsers(){
	strcpy(users[0].uname, "Nicolas"), strcpy(users[0].pass, "nn26");
	strcpy(users[1].uname, "Federico"), strcpy(users[1].pass, "fn25");
	strcpy(users[2].uname, "Matias"), strcpy(users[2].pass, "mn24");
	strcpy(users[3].uname, "Ciro"), strcpy(users[3].pass, "cn5");
	strcpy(users[4].uname, "Bianca"), strcpy(users[4].pass, "bn4");
}

/**
 * @brief Funcion que comprueba la autenticacion de usuarios en el servidor. 
 * Reingresa el usuario, hasta que la contraseña y el usuario sean correctas.
 * @author Navarro, Matias Alejandro
 * @param promp: arreglo donde se guarda el user[i].uname@ para la construccion del promp si el usuario ingresado es correcto
 * @date 05/04/2019
 * @return 1
 */
int userLog(char promp[])
{
	char usuario[20], password[20];
	int error = 0;
	printf("Autenticacion de usuario\n");
	while (error < 3)
	{
		printf("Usuario: ");
		fgets(usuario, 20, stdin);
		fflush(stdout);
		strtok(usuario, "\n");

		printf("Password: ");
		fgets(password, 20, stdin);
		fflush(stdout);
		strtok(password, "\n");

		for (int i = 0; i < CANT; i++)
		{
			if (strcmp(users[i].uname, usuario) == 0)
			{
				if (strcmp(users[i].pass, password) == 0)
				{
					strcpy(promp, users[i].uname);
					return 1;
					break;
				}
			}
		}

		error++;
	}
	return -1;
}

/**
 * @brief Funcion que arma el promp completo user[i].uname@hostname: 
 * @author Navarro, Matias Alejandro
 * @param promp: arreglo donde se arma el promp
 * @date 05/04/2019
 * @return 
 */
void getpromp(char promp[])
{
	char hostname[TAM];

	strcat(promp, "@");
	gethostname(hostname, TAM);
	strcat(promp, hostname);
	// printf("%s\n", promp);
}


/**
 * @brief Funcion 
 * @author Navarro, Matias Alejandro
 * @param 
 * @date 05/04/2019
 * @return 
 */
void setComando(int newsockfd, char promp[]){
	char buffer[TAM];
	//int aux1, aux2;

	while(1){ 
		printf("%s: ", promp);
		memset(buffer,0,sizeof(buffer));
		fgets(buffer,sizeof(buffer)-1,stdin);	//Ingreso un comando
		strtok(buffer, "\n");

		printf("%s\n",buffer);

		//Comprueba que tipo de comando se ingreso
		if(strcmp(buffer,"update firmware")==0){
			printf("Actualizando Firmware\n");
			write(newsockfd, buffer, sizeof(buffer));
			sleep(5);
			updateFirmware(newsockfd);
			break;
		}
		else if(strcmp(buffer,"start scanning")==0){
			printf("Star Scannig\n");
			break;
		}
		if(strcmp(buffer,"get telemetria")==0){
			printf("Obteniendo telemetria\n");
			break;
		}
		else {
			printf("Comando invalido\n");
		}
	}
}

/**
 * @brief Funcion 
 * @author Navarro, Matias Alejandro
 * @param 
 * @date 05/04/2019
 * @return 
 */
void updateFirmware(int newsockfd){
	FILE *firmware;
	char buffer[TAM], send[TAM];
	int size, read_size, num_packet = 1, n;
	memset(buffer,0,sizeof(buffer));
	memset(send,0,sizeof(send));

	n = read(newsockfd,buffer,sizeof(buffer));
	if(n<0){
		printf("Error al leer el socket");
		return ;
	}

	//Compruebo que el satelite este listo para la actualizacion
	if(strcmp(buffer,"OK")==0){
		printf("Satelite Listo\n");
	}
	else
	{
		printf("Error en el update\n");
		return;
	}
	
	//Abre el binario
	firmware = fopen("../bin/firmwareUpdate.bin", "r");
	if (firmware == NULL){	//Comprueba que el archivo no este vacio
		printf("Error al cargar el binario\n");
		return;
	}

	
	fseek(firmware,0,SEEK_END);
	size = ftell(firmware);
	fseek(firmware,0,SEEK_SET);

	n = write(newsockfd,&size,sizeof(size));
	if(n<0){
		printf("Error en el update\n");
		return;
	}
	printf("Enviando binario\n");

	n= read(newsockfd,&buffer, sizeof(buffer));
	if(n<0){
		printf("Error en el update\n");
		return;
	}


	while(!feof(firmware)){
		//Lee del archivo y lo coloca en el buffer
		read_size = fread(send,1,sizeof(size)-1,firmware);

		//Envio el dato
		write(newsockfd,send,read_size);
		num_packet++;

		memset(send,0,sizeof(send));
	}


	fclose(firmware);
}