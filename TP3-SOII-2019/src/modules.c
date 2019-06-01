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
    printf(
        "<html>"
        "<title>Drivers and Modules</title>"
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
        "<body>"

        // <!-- Navbar -->
        "<div>"
        "<div class=\"w3-bar w3-red w3-card-2 w3-left-align w3-large\">"
        "<a href=\"../index.html\" class=\"w3-bar-item w3-button w3-padding-large w3-white\">Home</a>"
        "<a href=\"ksamp.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">System Info</a>"
        "<a href=\"../stations.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Stations</a>"
        "<a href=\"modules.cgi\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Drivers and Modules</a>"
        "<a href=\"../doc/html/index.html\" class=\"w3-bar-item w3-button w3-hide-small w3-padding-large w3-hover-white\">Documentation</a>"
        "</div>"
        "</div>"
        // "<!-- Header -->"
        "<header class=\"w3-container w3-red w3-left-align\" style=\"padding:0px 16px\">"
        "<h1 class=\"w3-padding-8 w3-xxlarge w3-third\">Módulos del Kernel</h1>"
        "<h1 class=\"w3-padding-8 w3-xlarge w3-twothird\"></h1>"
        "</header>"
        );

    printf(
        "<div class=\"w3-half w3-container w3-padding\">"
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
        );

    printf(
      "<div class='w3-half w3-container'>"
      "<form action=\"/cgi-bin/upload.cgi\" method=\"post\" enctype=\"multipart/form-data\"> "
      "<p>Archivo a subir: <input type=\"file\" name=\"modulo\" /></p> "
      "<p><input class=\"w3-button w3-border w3-large\" type=\"submit\" name=\"Submit\" value=\"Subir Módulo\" /></p> "
      "</form>"
      "<p><a class=\"w3-button w3-border w3-large\" href=\"remove.sh\">Eliminar modulo</a></p>"
      "</div>"
      );
        
    printf(
        "</body>"
        "</html>"
        );
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