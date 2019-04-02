/** @file servidor_u_cc.c
 *  @brief Archivo principal del Servidor.
 *
 *  Contiene el programa principal que ejecutará el servidor, así como las
 *  funciones principales y aplicaciones que puede invocar el cliente.
 *
 *  @author Matias Navarro
 */

#include "../includes/comunes.h"
#define CANT 5

//Estructura que guarda el nombre y el pass de los clientes
struct user
{
	char uname[20];
	char pass[20];
} users[CANT];

int userLog(int,char[]); //Autenticacion de ingreso

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, servlen, n, pid,status=1;
	unsigned int clilen;
	struct sockaddr_un cli_addr, serv_addr;
	char buffer[TAM];

	strcpy(users[0].uname, "Nicolas"), 	strcpy(users[0].pass, "nn26");
	strcpy(users[1].uname, "Federico"), strcpy(users[1].pass, "fn25");
	strcpy(users[2].uname, "Matias"), 	strcpy(users[2].pass, "mn24");
	strcpy(users[3].uname, "Ciro"), 	strcpy(users[3].pass, "cn5");
	strcpy(users[4].uname, "Bianca"), 	strcpy(users[4].pass, "bn4");

	// // users[0].name = "Federico", users[0].pass = "25";
	// users[0].name = "Matias", 	users[0].pass = "24";
	// users[0].name = "Ciro", 	users[0].pass = "5";
	// users[0].name = "Bianca",	users[0].pass = "4";
	printf("%s %s \n", users[0].uname,users[0].pass);

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
		int error = 0;
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

			while (status)
			{
				int us;
				memset(buffer, 0, TAM);
				n = read(newsockfd, buffer, TAM - 1);
				if (n < 0)
				{
					perror("lectura de socket");
					exit(1);
				}

				us = userLog(newsockfd, buffer);
				printf("\nEstoy aca %d \n",us);

				if(error == 3){
					error = 0;
					exit(0);
				}
				
				printf("PROCESO: %d. ", getpid());
				printf("Recibí: %s", buffer);
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

int userLog(int newsockfd,char buffer[]){
	int au, error = 0; 
	char a_user[20];
	char a_pass[20];
	
	memset(buffer,0,TAM);
	while(error < 3) //Cantidad de intentos erroneos 3
	{
		au = write(newsockfd,"User: ",6);
		au = read(newsockfd,buffer,TAM-1); //Ingreso del usuario 
		au = write(newsockfd,"Obtuve su mensaje", 18);
		if(au < 0)
		{
			perror("lectura de socket");
			exit(1);			
		}
		strcpy(a_user,buffer);
		memset(buffer,0,TAM);

		au = write(newsockfd,"Password: ",10);
		au = read(newsockfd,buffer, TAM-1); //Ingreso de la contraseña
		au = write(newsockfd,"Obtuve su mensaje", 18);
		if(au < 0)
		{
			perror("lectura de socket");
			exit(1);			
		}
		strcpy(a_pass,buffer);

		for(int i=0;i<CANT;i++)
		{
			if(strcmp(a_user,users[i].uname))
			{
				if(strcmp(a_pass,users[i].pass))
				{
					//Usuario correcto
					printf("Conexion Exitosa");
					fflush(stdout);
					au = write(newsockfd,"Conexion Exitosa \n",sizeof("Conexion Exitosa \n"));
					return 0; 
				}
			}
		}

		printf("Usuario incorrecto \n");
		fflush(stdout);
		error++;

		if(error == 3)
		{
			au = write(newsockfd,"Error en la autenticacion",sizeof("Error en la autenticacion"));
			exit(0);
		}				

	}
	return -1;
}

