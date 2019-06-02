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
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">"
    "<meta name=\"description\" content=\"TP3 - SOII - 2019\">"
    "<meta name=\"author\" content=\"Matias Navarro\">"
    "<title>TP3-SOII-2019 - Matias Navarro</title>");

    //<!-- Font Awesome Icons -->
    printf(""
    "<link href=\"../vendor/fontawesome-free/css/all.min.css\" rel=\"stylesheet\" type=\"text/css\">");

    //<!-- Google Fonts -->
    printf(""
    "<link href=\"https://fonts.googleapis.com/css?family=Merriweather+Sans:400,700\" rel=\"stylesheet\">"
    "<link href=\'https://fonts.googleapis.com/css?family=Merriweather:400,300,300italic,400italic,700,700italic\' rel='stylesheet' type='text/css'>");

    //<!-- Plugin CSS -->
    printf(""
    "<link href=\"../vendor/magnific-popup/magnific-popup.css\" rel=\"stylesheet\">");

    //<!-- Theme CSS - Includes Bootstrap -->
    printf(""
    "<link href=\"../css/creative.min.css\" rel=\"stylesheet\">"
    "</head>"
    "<body id=\"page-top\">");

    //<!-- Navigation -->
    printf(""
    "<nav class=\"navbar navbar-expand-lg navbar-light fixed-top py-3\" id=\"mainNav\">"
    "    <div class=\"container\">"
    "    <a class=\"navbar-brand js-scroll-trigger\" href=\"#page-top\">Sistemas Operativos II - 2019 (UNC)</a>"
    "    <button class=\"navbar-toggler navbar-toggler-right\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarResponsive\" aria-controls=\"navbarResponsive\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">"
    "        <span class=\"navbar-toggler-icon\"></span>"
    "    </button>"
    "    <div class=\"collapse navbar-collapse\" id=\"navbarResponsive\">"
    "        <ul class=\"navbar-nav ml-auto my-2 my-lg-0\">"
    "        <li class=\"nav-item\">"
    "            <a class=\"nav-link js-scroll-trigger\" href=\"../index.lighttpd.html\">Home</a>"
    "        </li>"
    "        </ul>"
    "    </div>"
    "    </div>"
    "</nav>");

    //<!-- Services Section -->
    printf(""
    "<section class=\"page-section\" id=\"services\">"
    "    <div class=\"container\">"
    "    <h2 class=\"text-center mt-0\">System Info</h2>"
    "    <hr class=\"divider my-4\">"
    "    <div class=\"row\">"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-address-card text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">Hostname</h3>"
    "            <p class=\"text-muted mb-0\">%s</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-microchip text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">CPU Model</h3>"
    "            <p class=\"text-muted mb-0\">%s</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-tachometer-alt text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">CPU Usage</h3>"
    "            <p class=\"text-muted mb-0\">%s %s %s</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-memory text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">Memoria RAM</h3>"
    "            <p class=\"text-muted mb-0\">%i MB/%i MB</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-calendar-alt text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">Hora y Fecha</h3>"
    "            <p class=\"text-muted mb-0\">%d:%d:%d %d-%d-%d</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-clock text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">Uptime</h3>"
    "            <p class=\"text-muted mb-0\">%ldD %ld:%02ld:%02ld</p>"
    "        </div>"
    "        </div>"
    "        <div class=\"col-lg-3 col-md-6 text-center\">"
    "        <div class=\"mt-5\">"
    "            <i class=\"fas fa-4x fa-cogs text-primary mb-4\"></i>"
    "            <h3 class=\"h4 mb-2\">Linux Kernel</h3>"
    "            <p class=\"text-muted mb-0\">%s</p>"
    "        </div>"
    "        </div>"
    "    </div>"
    "    </div>"
    "</section>",
    datos.hostname,
    datos.cpu, data.load1Min, data.load5Min, data.load15Min,
    data.memTot/MBYTE - data.memDisp/MBYTE,data.memTot/MBYTE,
    datos.time.tm_hour, datos.time.tm_min, datos.time.tm_sec, datos.time.tm_year + 1900, datos.time.tm_mon + 1, datos.time.tm_mday, 
    ut/day, (ut%day)/hour,(ut%hour)/minute,ut%minute,
    datos.kernel
    );

    //<!-- Footer -->
    printf(""
    "<footer class=\"bg-light py-5\">"
    "    <div class=\"container\">"
    "    <div class=\"small text-center text-muted\">Copyright &copy; 2019 - (UNC) Facultad de Ciencias Exactas Físicas y Naturales</div>"
    "    </div>"
    "</footer>"

    //<!-- Bootstrap core JavaScript -->
    "<script src=\"../vendor/jquery/jquery.min.js\"></script>"
    "<script src=\"../vendor/bootstrap/js/bootstrap.bundle.min.js\"></script>"

    //<!-- Plugin JavaScript -->
    "<script src=\"../vendor/jquery-easing/jquery.easing.min.js\"></script>"
    "<script src=\"../vendor/magnific-popup/jquery.magnific-popup.min.js\"></script>"

    //<!-- Custom scripts for this template -->
    "<script src=\"../js/creative.min.js\"></script>"

    "</body>"

    "</html>");
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
