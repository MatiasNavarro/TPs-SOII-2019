/** @file readNetcdf.c
 *  @brief Archivo principal del readNetcdf.c.
 *
 *  Contiene el programa principal que ejecutará el scrip para realizar
 *  la convolucion para aplicar el filtro a la imagen.
 * 
 *  @author Navarro, Matias Alejandro
 */
#include "../includes/includes.h"


int
main()
{
    int ncid, varid, x, y, retval;
    float *data_in=malloc(NX*NY*sizeof(float));
    float *result=malloc(NX*NY*sizeof(float));
    size_t start[2], cont[2];
    /* Definicion de la matriz w */
    float W[WX][WY] = {{-1, -1, -1},
                       {-1,  8, -1},
                       {-1, -1, -1}};

    start[0]=0;
    start[1]=0;
    cont[0]=NX;
    cont[1]=NY;

    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
        ERR(retval);
    
    /* Obtenemos elvarID de la variable CMI. */
    if ((retval = nc_inq_varid(ncid, "CMI", &varid)))
        ERR(retval);

    /* Leemos la matriz. */
    if ((retval = nc_get_var_float(ncid, varid, data_in)))
        ERR(retval);
    
    /* el desarrollo acá */
    //####-------------------------------------------------###
    setNAN(data_in);

    convolve(data_in, W, result);
    free(data_in);
    
    save_nc(start[0],start[1], result);
    free(result);
    //####-------------------------------------------------###
    /* Se cierra el archivo y liberan los recursos*/
    if ((retval = nc_close(ncid)))
        ERR(retval);  

    retval = 1;
    printf ("%i",retval);
    return 0;
}

/**
 * @brief
 * @param
 * @return
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void setNAN(float *data_in){
    
    for(int i=0; i<NX; i=i+1)
    {
        for(int j=0; j<NY; j=j+1)
            {
                if(data_in[i*NX + j]==-1)
                {
                    data_in[i*NX + j]=(float)(0.0/0.0);
                }                       
            }   
    }
}

/**
 * @brief
 * @param
 * @return
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void convolve(float *image, float W[WX][WY], float *result)
{
    for(int x=1; x<NX-1; x=x+1)
    {
        for(int y=1; y<NY-1; y=y+1)
        {
            result[(x*NX)+y] = (result[((x-1)*NX)+(y-1)] * W[0][0] + result[((x-1)*NX)+(y)] * W[0][1] + result[((x-1)*NX)+(y+1)] * W[0][2] +
                                result[(x*NX)+(y-1)]     * W[1][0] + result[(x*NX)+y]       * W[1][1] + result[(x*NX)+(y+1)]     * W[1][2] +
                                result[((x+1)*NX)+(y-1)] * W[2][0] + result[((x+1)*NX)+(y)] * W[2][1] + result[((x+1)*NX)+(y+1)] * W[2][2]  ) ;
        }
    }
}

/**
 * @brief
 * @param
 * @return
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void save_nc(int r, int s,float *result){
    /**/

    //Path completo: /home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TPs-SOII-2019/TP2-SOII-2019/src/image.nc

    status = nc_create(SAVE_NAME, NC_CLOBBER, &ncid2);
    if (status != NC_NOERR) 
        ERR(status);

    /*dimensiones de datos que voy a guardar en data.nc*/
    status = nc_def_dim(ncid2, "filas", 21696, &filas);
    if (status != NC_NOERR) 
        ERR(status);
    status = nc_def_dim(ncid2, "columas", 21696, &columnas);
    if (status != NC_NOERR) 
        ERR(status);

    /*asignar esas dimensiones y id*/
    dims[0] = filas;
    dims[1] = columnas;
    status = nc_def_var (ncid2, "CMI", NC_FLOAT, 2, dims, &id);
    if (status != NC_NOERR) 
        ERR(status);

    status = nc_enddef(ncid2);
    if (status != NC_NOERR) 
        ERR(status);

    if ((status = nc_put_vara_float(ncid2,id,start,conteo,result)))
        ERR(status);
    if ((status = nc_close(ncid2)))
        ERR(status);
}