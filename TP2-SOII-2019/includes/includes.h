#include <stdlib.h>
#include <stdio.h>
#include "/home/matiasnavarro/Development/SO2/hpc/libs/netcdf/include2/netcdf.h"

#ifdef NAN
/* NAN is supported */
#endif

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

/* nombre del archivo a leer */
#define FILE_NAME "OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc"
#define SAVE_NAME "./OR_ABI_Convolution.nc"
/* Lectura de una matriz de 21696 x 21696 */
#define NX 21696
#define NY 21696

#define WX 3
#define WY 3

/*Parametros globales*/
static int status,ncid2,filas,columnas,id;
static int dims[2];

static size_t start[2]={0};
static size_t conteo[2]={0};

//Convolve (ImagenOriginal, Filtro, ImagenFiltrada(resultado))
void convolve(float *,float W[WX][WY], float *);    //Realiza la convolucion
void save_nc(int,int,float *);                      //Guarda la imagen
void setNAN(float *);
