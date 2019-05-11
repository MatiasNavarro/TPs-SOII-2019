#include <stdlib.h>
#include <stdio.h>
#include "/home/matiasnavarro/Development/SO2/hpc/libs/netcdf/include2/netcdf.h"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

/* nombre del archivo a leer */
#define FILE_NAME "OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc"

/* Lectura de una matriz de 21696 x 21696 */
#define NX 21696
#define NY 21696

#define WX 3
#define WY 3

void convolve(int, int, float[NX][NY], float [WX][WY], float[NX][NY]);

int
main()
{
    int ncid, varid, x, y, retval;
    float data_in[NX][NY]={0};
    /* Definicion de la matriz w */
    float W[WX][WY] = {{-1, -1, -1},
                       {-1, 8, -1},
                       {-1, -1, -1}};
    float result[NX][NY]={0};

    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
        ERR(retval);
    
    /* Obtenemos elvarID de la variable CMI. */
    if ((retval = nc_inq_varid(ncid, "CMI", &varid)))
        ERR(retval);

    /* Leemos la matriz. */
    if ((retval = nc_get_var_float(ncid, varid, &data_in[0][0])))
        ERR(retval);
    
    /* el desarrollo acá */

    /* Se cierra el archivo y liberan los recursos*/
    if ((retval = nc_close(ncid)))
        ERR(retval);  

    retval = 1;
    printf ("%i",retval);
    return 0;
}

void convolve(int x, int y, float data_in[NX][NY], float W[WX][WY], float result[NX][NY])
{
    for(int i=-1; i<2; i=i+1)
    {
        for(int j=-1; j<2; j=j+1)
        {
            result[x][y] = result[x][y] + data_in[x+i][x+j]*W[i+1][j+1];
        }
    }
}