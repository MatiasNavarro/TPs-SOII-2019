/** @file comunes.h
 *  @brief Libreria con mensajes e imports comunes al cliente y al servidor.
 *
 *
 *  @author Matias Navarro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <netdb.h> 
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <sys/un.h>
#define TAM 1024

#define endMsg "/END"
#define disconnectMsg "/BYE"
#define start_UDP_Msg "/START"
#define ack_msg "/ACK"
#define end_UDP_Msg "/FINISH"
#define udp_ready "/UDP_READY"