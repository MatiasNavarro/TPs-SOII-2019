/** @file funciones_cliente_cc.h
 *  @brief Libreria principal del cliente
 *
 *
 *  @author Facundo Maero
 */


int initialize_udp_server_with_args(socklen_t *tamano_direccion , struct sockaddr_in* serv_addr);
void send_udp(int sockfd, char buffer[], struct sockaddr_in* serv_addr, socklen_t tamano_direccion);
void recv_udp(int sockfd, char buffer[], struct sockaddr_in* serv_addr, socklen_t* tamano_direccion);
void recibir_datos(int sockfd);
int parser(char* line, char before[], char after[], char** buff);
char *read_line(void);
int send_to_socket(int sockfd, char cadena[]);
int read_from_socket(int sockfd, char buffer[]);
void get_ip_address(char address[], int fd);