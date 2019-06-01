/** @file ksamp.c
 *  @brief Pagina del webserver dedicada a mostrar información del sistema.
 *
 * Archivo con la lógica y presentación de la página del servidor
 * dedicada a mostrar módulos del kernel instalados, subir un módulo propio,
 * instalarlo y desinstalarlo.
 *
 *  @author Facundo Maero
 */
#include "../include/ksamp.h"

/**
* @brief Función principal de la página de información del sistema.
*
* Llama a las subrutinas necesarias para mostrar la página correctamente.
* Explora el directorio /proc para obtener datos relevantes del sistema
* y mostrarlos en la página web.
*/
int main (int argc, char* argv[]){
    printf("Content-Type: text/html\n\n");
    struct Datos datos;
    struct Interval interval;
    
    getData(&datos);
    calculateInterval(&interval);
    printPage(datos, interval);
	return 0;
}

/**
* @brief Imprime el código HTML con la información solicitada.
*
* Recibe estructuras con datos sobre el sistema, imprime el código
* HTML y el contenido de estas estructuras para que el usuario pueda
* monitorear el webserver.
*
* @param datos Contiene modelo de CPU, uptime, hora actual, hostname y versión de kernel.
* @param data Contiene la RAM total y disponible, y medidas de uso del CPU.
*/
void printPage(struct Datos datos, struct Interval data){
    long ut = datos.uptime;
    //Imports
    printf(""
    "<html>"
    "<title>System Information</title>"
    "<meta charset=\"UTF-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<link rel=\"stylesheet\" href=\"../style/w3.css\">"
    "<link rel=\"stylesheet\" href=\"../style/lato.css\">"
    "<link rel=\"stylesheet\" href=\"../style/montserrat.css\">"
    "<link rel=\"stylesheet\" href=\"../style/font_awesome.css\">"
    "<style>"
    "body,h1,h2,h3,h4,h5,h6 {font-family: \"Lato\", sans-serif}"
    ".w3-bar,h1,button {font-family: \"Montserrat\", sans-serif}"
    ".fa-anchor,.fa-coffee {font-size:200px}"
    "</style>"
    "<body>");

    //<!-- Navbar -->
    printf(""
    "<div>"
    "  <div class=\"w3-bar w3-red w3-card-2 w3-left-align w3-large\">"
    "    <a href=\"../index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-white\">Home</a>"
    "    <a href=\"ksamp.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">System Info</a>"
    "    <a href=\"../stations.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Stations</a>"
    "    <a href=\"modules.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Drivers and Modules</a>"
    "    <a href=\"../doc/html/index.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Documentation</a>"
    "  </div>"
    "</div>");

    // <!-- Header -->
    printf(""
    "<header class=\"w3-container w3-red w3-left-align\" style=\"padding:0px 16px\">"
    "    <h1 class=\"w3-padding-8 w3-xxlarge\">System Info</h1>"
    "</header>");

    //Lista
    printf(""
    "<ul class=\"w3-ul w3-card-4\">"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/host.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Hostname</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/cpu.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">CPU Model</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/cpu2.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">CPU Usage</span><br>"
    "    <span>%s %s %s</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/ram.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Memoria RAM</span><br>"
    "    <span>%i MB/%i MB</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/clock.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Hora y Fecha</span><br>"
    "    <span>%d:%d:%d %d-%d-%d</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/uptime.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Uptime</span><br>"
    "    <span>%ldD %ld:%02ld:%02ld</span>"
    "  </li>"
    "  <li class=\"w3-padding-16\">"
    "    <img src=\"../icons/kernel.png\" class=\"w3-left w3-circle w3-margin-right\" style=\"width:50px\">"
    "    <span class=\"w3-large\">Linux Kernel Version</span><br>"
    "    <span>%s</span>"
    "  </li>"
    "</ul>"

    "</body>"
    "</html>",
    datos.hostname,
    datos.cpu, data.load1Min, data.load5Min, data.load15Min,
    data.memTot/MBYTE - data.memDisp/MBYTE,data.memTot/MBYTE,
    datos.time.tm_hour, datos.time.tm_min, datos.time.tm_sec, datos.time.tm_year + 1900, datos.time.tm_mon + 1, datos.time.tm_mday, 
    ut/day, (ut%day)/hour,(ut%hour)/minute,ut%minute,
    datos.kernel
    );
}

/**
* @brief Obtiene datos necesarios para llenar la estructura Datos.
*
* Obtiene el hostname, modelo de CPU, kernel y uptime de /proc 
* y lo guarda en una estructura de tipo Datos.
*
* @param *datos Puntero a estructura para modificarla directamente con los datos obtenidos.
*/
void getData(struct Datos *datos){

    char buffer[256];

    fp = fopen("/proc/sys/kernel/hostname","r");
    if (fp == NULL){
        return;
    } 
    if(fgets(buffer, 256, fp) == NULL){
        printf("Error fgets\n");
    }
    fclose(fp);
    strcpy(datos->hostname,buffer);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    datos->time = tm;

    strcpy(path,"/proc/cpuinfo");
    strcpy(before,"model name");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->cpu,buff+2);

    strcpy(path,"/proc/version");
    strcpy(before,"version ");
    strcpy(after,"(");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->kernel,buff);

    strcpy(path,"/proc/uptime");
    strcpy(before,"");
    strcpy(after," ");
    parseFile(fp,path,before,after,&buff);
    datos->uptime = atol(buff);
}

/**
* @brief Obtiene datos necesarios para llenar la estructura Interval.
*
* Obtiene la memoria RAM total, disponible y el promedio de uso de la CPU
* en los últimos 1, 5 y 15 minutos.
*
* @param *interval Puntero a estructura para modificarla directamente con los datos obtenidos.
*/
void calculateInterval(struct Interval *interval){

    strcpy(path,"/proc/meminfo");
    strcpy(before,"MemTotal:");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    
    interval->memTot = atoi(buff);

    strcpy(before,"MemAvailable:");
    parseFile(fp,path,before,after,&buff);
    
    interval->memDisp = atoi(buff);

    strcpy(path,"/proc/loadavg");
    strcpy(before,"");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    if(sscanf( buff, "%s %s %s", interval->load1Min, interval->load5Min, interval->load15Min) != 3){
        printf("Error sscanf\n");
        exit(EXIT_FAILURE);
    }
}

    /**
    * @brief Parsea un archivo en busca de una cadena de caracteres.
    *
    * Recibido un path, una cadena a buscar y delimitadores, busca en el archivo
    * y guarda la cadena en buff. Si no puede abrir el archivo termina el programa.
    * Busca iterativamente hasta encontrar la primer ocurrencia del string buscado
    * y la recorta en base a los delimitadores.
    * @param fp File Descriptor, usado para abrir un archivo como lectura
    * @param path[] Cadena de caracteres con la ruta en la que se buscara el archivo
    * @param before[] String con el delimitador que se encuentra antes de la cadena deseada
    * @param after[] String con el delimitador que se encuentra despues de la cadena deseada
    * @param buff Puntero a char donde se almacena el resultado
    */
int parseFile(FILE* fp, char path[], char before[], char after[], char** buff){

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    fp = fopen(path,"r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line,before)!=NULL){
            *buff = strstr(line,before) + strlen(before);
            *buff = strtok(*buff,after);
            break;
        }
    }
    fclose(fp);
    return 0;
}