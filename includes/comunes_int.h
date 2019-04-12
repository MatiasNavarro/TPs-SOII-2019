/** @file comunes_int.h
 *  @brief Libreria con mensajes e imports comunes al cliente y al servidor.
 *
 *
 *  @author Matias Navarro
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <netdb.h> 
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/time.h>

#define TAM 512
#define endMsg "/END"
#define disconnectMsg "/BYE"
#define start_UDP_Msg "/START"
#define ack_msg "/ACK"
#define end_UDP_Msg "/FINISH"
#define udp_ready "/UDP_READY"