/** @file cliente_u_cc.c
 *  @brief Archivo principal del Cliente.
 *
 *  Contiene el programa principal que ejecutará el cliente.
 *
 *  @author Navarro, Matias Alejandro
 */

#include "../includes/comunes.h"
#include "../includes/funciones_cliente.h"

static struct satelite sat;

int main( int argc, char *argv[] ) {
	int sockfd, servlen, n;
	struct sockaddr_un serv_addr;
	char buffer[TAM];
	int terminar = 0;

        if ( argc < 2 ) {
                fprintf( stderr, "Uso %s archivo\n", argv[0]);
                exit( 0 );
        }

	memset( (char *)&serv_addr, '\0', sizeof(serv_addr) );
	serv_addr.sun_family = AF_UNIX;
	strcpy( serv_addr.sun_path, argv[1] );
	servlen = strlen( serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0) ) < 0) {
		perror( "creación de socket" );
		exit( 1 );
	}


 	if ( connect( sockfd, (struct sockaddr *)&serv_addr, servlen ) < 0 ) {
		perror( "conexión" );
		exit( 1 );
	}

	setInfo();
	

	while(1) {

		memset( buffer, '\0', TAM );
		printf( "Ingrese el mensaje a enviar: " );
		fgets( buffer, TAM-1, stdin );
		strtok(buffer, "\n");

		n = write( sockfd, buffer, strlen(buffer) );
                if ( n < 0 ) {
                        perror( "escritura de socket" );
                        exit( 1 );
                }

                // Verificando si se escribió: fin
                buffer[strlen(buffer)-1] = '\0';
                if( !strcmp( "fin", buffer ) ) {
                        terminar = 1;
                }

                memset( buffer, '\0', TAM );
		n = read( sockfd, buffer, TAM );
                if ( n < 0 ) {
                        perror( "lectura de socket" );
                        exit( 1 );
                }

                printf( "Respuesta: %s\n", buffer );
                if( terminar ) {
                        printf( "Finalizando ejecución\n" );
                        exit(0);
                }
	}
	return 0;
}


//Actualiza la informacion del satelite
void setInfo(){
	FILE *file_version;
	FILE *consumo;
	char buffer2[1024];
	char stateFile[1024];
	char pid[1024];

	strcpy(sat.ID, "ARCOR SAT");
	strcpy(sat.uptime, "11:20 a.m");
	
	printf("Ack 1\n");
	//Lee la version del firmware del Archivo firmwareClient.bin para cargar la version
	file_version = fopen("./bin/firmwareClient.bin", "rb");
	printf("Ack 2\n");
	memset(buffer2,0,sizeof(buffer2));
	fread(buffer2,sizeof(buffer2),1,file_version);
	printf("Ack 3\n");
	strcpy(sat.version,buffer2);
	fclose(file_version);
	memset(buffer2,0,sizeof(buffer2));
	

	strcpy(stateFile,"ps -Ao vsize,pid,pcpu | greap ");
	sprintf(pid,"%ld", (long)getpid());
	strcat(stateFile, pid);
	strcat(stateFile, " >> ./bin/stateFile.txt");
	system("rm ./bin/stateFilw.txt");
	system(stateFile);
	consumo = fopen("./bin/stateFile.txt","rb");

	printf("Ack 3\n");
	fread(buffer2,1,TAM-1, consumo);
	strtok(buffer2, "\n");
	strcpy(sat.consumoCPU, buffer2);
	fclose(consumo);
}

void getInfo(){
	printf("ID = %s\n", sat.ID);
	printf("Uptime = %s\n", sat.uptime);
	printf("Version = %s\n", sat.version);
	printf("Consumo = %s\n", sat.consumoCPU);
}