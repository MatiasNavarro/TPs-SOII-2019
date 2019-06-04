/** @file modules.c
 *  @brief Pagina del webserver dedicada a modulos del Kernel
 *
 * Archivo con la lógica y presentación de la página del servidor
 * dedicada a mostrar módulos del kernel instalados, subir un módulo propio,
 * instalarlo y desinstalarlo.
 *
 *  @author Facundo Maero
 */
#include "../include/modules.h"

/**
* @brief Función principal de la página de módulos
*
* Llama a las subrutinas necesarias para mostrar la página correctamente.
* Cuenta el número de módulos instalados, los guarda en una estructura
* de datos, y los muestra en la página.
*/
int main (int argc, char* argv[]){
    printf("Content-Type: text/html\n\n");
    int lines = count_modules();
    if(lines == -1){
        printf("Error count_modules\n");
        exit(EXIT_FAILURE);
    }

    struct Modulo modulos[lines];
    getData(modulos, lines);

    printPage(modulos, lines);

	return 0;
}

/**
* @brief Imprime la página para ser mostrada en el web browser.
*
* A partir de una lista de módulos instalados y su cantidad,
* muestra una simple lista con información básica sobre ellos,
* y muestra además los elementos necesarios para cargar y borrar
* un custon driver.
* @param modulos[] Estructura de datos con la información de los módulos instalados.
* @param cant Cantidad de módulos instalados en el sistema.
*/
void printPage(struct Modulo modulos[], int cant){
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
    "    <a class=\"navbar-brand js-scroll-trigger text-black-50\" href=\"#page-top\">Sistemas Operativos II - 2019 (UNC)</a>"
    "    <button class=\"navbar-toggler navbar-toggler-right\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarResponsive\" aria-controls=\"navbarResponsive\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">"
    "        <span class=\"navbar-toggler-icon\"></span>"
    "    </button>"
    "    <div class=\"collapse navbar-collapse\" id=\"navbarResponsive\">"
    "        <ul class=\"navbar-nav ml-auto my-2 my-lg-0\">"
    "        <li class=\"nav-item\">"
    "            <a class=\"nav-link js-scroll-trigger text-black-50\" href=\"../index.lighttpd.html\">Home</a>"
    "        </li>"
    "        <li class=\"nav-item\">"
    "            <a class=\"nav-link js-scroll-trigger text-black-50\" href=\"../index.lighttpd.html\">Home</a>"
    "        </li>"
    "        <li class=\"nav-item\">"
    "            <a class=\"nav-link w3-padding-large w3-hide-small text-black-50\" href=\"./ksamp.cgi\">System Info</a>"
    "         </li>"
    "         <li class=\"nav-item\">"
    "           <a class=\"nav-link js-scroll-trigger text-black-50\" href=\"../formulario.html\">Formulario</a>"
    "         </li>"
    "        </ul>"
    "    </div>"
    "    </div>"
    "</nav>");

    printf(""
    //<!-- Services Section -->
    "<section class=\"page-section\" id=\"services\">"
    "<div class=\"container\">"
    "    <h2 class=\"text-center mt-0\">Modulos</h2>"
    "    <hr class=\"divider my-4\">"
    "    <div class=\"row justify-content-center\">"
    "    <div class=\"col-lg-8 text-center\">"
    "        <p class=\"text-muted mb-0\">Página que liste los módulos instalados en el sistema, y que posea"
    "            un formulario que permita subir un archivo al servidor, controlar que este sea un archivo "
    "            válido (del tipo módulo), e instalarlo en el kernel del sistema operativo. También debe poseer"
    "           un botón para removerlo [2,3]."
    "        </p><br>"
    "        <div class=\'w3-half w3-container\'>"
    "        <form action=\"/cgi-bin/upload.cgi\" method=\"post\" enctype=\"multipart/form-data\"> "
    "        <p>Archivo a subir: <input class=\"btn js-scroll-trigger\" type=\"file\" name=\"modulo\" /></p>"
    "        <p><input class=\"btn btn-primary btn-xl js-scroll-trigger\" type=\"submit\" name=\"Submit\" value=\"Subir Módulo\" /></p>"
    "        </form>"
    "        <p><a class=\"w3-button w3-border w3-large\" href=\"remove.sh\">Eliminar modulo</a></p>"
    "        </div>"
    "    </div>"
    "    </div>"
    "</div>"
      );

    printf(
        "<div class=\"w3-half w3-container w3-padding row justify-content-center\">"
        "<table class=\"w3-table w3-striped w3-bordered\">"
        "<tr>"
        "<th>Nro</th>"
        "<th>Nombre</th>"
        "<th>Tamaño</th>"
        "<th>Nro Instancias</th>"
        "<th>Estado</th>"
        "</tr>"
        );

    for (int i = 0; i < cant; ++i)
    {
        printf(
            "<tr>"
            "<td>%d</td>"
            "<td>%s</td>"
            "<td>%d Kb</td>"
            "<td>%d</td>"
            "<td>%s</td>"
            "</tr>",
            i+1,modulos[i].name, modulos[i].size/1024,
            modulos[i].instances_loaded, modulos[i].status
            );
    }

    printf(
        "</table>"
        "</div>"
        "</section>");
        
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
* @brief Cuenta el número de módulos instalados en el sistema.
*
* Mediante la exploración del archivo /proc/modules,
* se cuenta la cantidad de drivers instalados.
*
* @return La cantidad de módulos del kernel instalados actualmente.
*/
int count_modules(){
    char buffer[256];
    int lines = 0;
    FILE* fp = fopen("/proc/modules","r");
    if (fp == NULL){
        return -1;
    }
    while(fgets(buffer, 256, fp) != NULL){
        lines++;
    }
    fclose(fp);
    return lines;
}

/**
* @brief Obtiene de el directorio /proc información sobre los drivers en el sistema.
*
* Explorando el archivo /proc/modules se va guardando en la estructura
* modulos[] la información de cada uno.
* Puntualmente se guarda su nombre, su tamaño, la cantida de veces que fue montado,
* y su estado actual.
*
* @param modulos[] Estructura de datos donde se guardará la información de los módulos instalados.
* @param lines Cantidad de módulos instalados en el sistema, usado para iterar sobre el archivo.
*/
void getData(struct Modulo modulos[], int lines){

    char buffer[256];

    FILE* fp = fopen("/proc/modules","r");
    if (fp == NULL){
        return;
    }
    for (int i = 0; i < lines; ++i)
    {
        fgets(buffer, 256, fp);
        sscanf( buffer, "%s %d %d %*s %s", modulos[i].name, &modulos[i].size, &modulos[i].instances_loaded, modulos[i].status );
    }
    fclose(fp);
}
