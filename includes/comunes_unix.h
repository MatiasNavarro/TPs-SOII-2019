/** @file comunes.h
 *  @brief Libreria con mensajes e imports comunes al cliente y al servidor.
 *
 *
 *  @author Matias Navarro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#define TAM 1024

#define endMsg "/END"
#define disconnectMsg "/BYE"
#define start_UDP_Msg "/START"
#define ack_msg "/ACK"
#define end_UDP_Msg "/FINISH"
#define udp_ready "/UDP_READY"