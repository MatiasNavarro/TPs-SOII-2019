/** @file OpenMP.c
 *  @brief Archivo principal del OpenMP.c.
 *
 *  Contiene el programa principal que ejecutará el scrip para realizar
 *  la convolucion para aplicar el filtro a la imagen.
 * 
 *  @author Navarro, Matias Alejandro
 */
#include "../includes/includes.h"
#include <omp.h>
//Numero de hilos (Number Thread)
#define NT 2


//Usuario: Estudiante52 
//Password: XCBxZcF1

int
main()
{
    int ncid, varid, retval;
    float *data_in=malloc(NX*NY*sizeof(float));
    float *result=malloc(NX*NY*sizeof(float));

    /* Definicion de la matriz w */
    float W[WX][WY] = {{-1.0, -1.0, -1.0},
                       {-1.0,  8.0, -1.0},
                       {-1.0, -1.0, -1.0}};

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
    /* Sets de nan en la matriz */
    setNAN(data_in);

    /*realizar la convolucion */
    convolve(data_in, W, result);
    free(data_in);

    /*guardar la imagen filtrada */
    save_nc(result);
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
 * @brief Funcion que setea los valores -1 de la matriz como NAN (i.e (float)(0.0/0.0))
 * @param data_in: imagen del satelite cargada
 * @return void
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void setNAN(float *data_in){
    #pragma omp parallel for num_threads(NT)
    for(int i=0; i<NX*NY; i=i+1)
	if(data_in[i]==(-1))
    {
        data_in[i]=NAN;
    }
}

/**
 * @brief Funcion encargada de realizar la convolucion, el resultado de dicha convolucion es la imagen con el filtro de bordes aplicado
 * @param - image: imagen del satelite cargada a la cual se le va a realizar la convolucion 
 *        - W[WX][WY]: matriz "filtro" con la cual se realiza la convolucion 
 *        - result: matriz donde se va a cargar la imagen resultante, es decir, la imagen con el filtro de bordes aplicado
 * @return void
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void convolve(float *image, float W[WX][WY], float *result)
{
    struct timespec start, end;
    
    if( clock_gettime( CLOCK_MONOTONIC_RAW, &start) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }

    #pragma omp parallel for num_threads(NT)
    for(int x=1; x<NX-1; x=x+1)
    {
        for(int y=1; y<NY-1; y=y+1)
        {
            result[(x*NX)+y] = (image[((x-1)*NX)+(y-1)] * W[0][0] + image[((x-1)*NX)+(y)] * W[0][1] + image[((x-1)*NX)+(y+1)] * W[0][2] +
                                image[(x*NX)+(y-1)]     * W[1][0] + image[(x*NX)+y]       * W[1][1] + image[(x*NX)+(y+1)]     * W[1][2] +
                                image[((x+1)*NX)+(y-1)] * W[2][0] + image[((x+1)*NX)+(y)] * W[2][1] + image[((x+1)*NX)+(y+1)] * W[2][2] ) 
                                * (float)(0.00031746) ;
        }
    }

    if( clock_gettime( CLOCK_MONOTONIC_RAW, &end) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }

    u_int64_t tiempo = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("Tiempo en realizar la convolucion: %f \n", (double)(tiempo));
}

/**
 * @brief Funcion encargada de crear el archivo .nc donde se guardara la imagen resultante y escritura de dicho archivo
 * @param - result: imagen filtrada (Resultado de la convolucion)
 * @return void
 * @date 16/05/2019.
 * @author Navarro, Matias Alejandro.
 */
void save_nc(float *result){
    /**/
    int ncid, varid, retval, status, filas, columnas, id;
    int dims[2];
    size_t start[2] = {0};
    start[0] = 0;
    start[1] = 0;
    size_t conteo[2] = {0};
    conteo[0] = NX;
    conteo[1] = NX;

    //############## Creacuion del archivo .nc ############################################
    status = nc_create(SAVE_NAME, NC_CLOBBER, &ncid);
    if (status != NC_NOERR) 
        ERR(status);

    /*dimensiones de datos que voy a guardar en data.nc*/
    status = nc_def_dim(ncid, "filas", NX, &filas);
    if (status != NC_NOERR) 
        ERR(status);
    status = nc_def_dim(ncid, "columas", NY, &columnas);
    if (status != NC_NOERR) 
        ERR(status);

    /*asignar esas dimensiones y id*/
    dims[0] = filas;
    dims[1] = columnas;
    status = nc_def_var (ncid, "CMI", NC_FLOAT, 2, dims, &id);
    if (status != NC_NOERR) 
        ERR(status);

    status = nc_enddef(ncid);
    if (status != NC_NOERR) 
        ERR(status);

    if ((status = nc_put_vara_float(ncid,id,start,conteo,result)))
        ERR(status);
    if ((status = nc_close(ncid)))
        ERR(status);

    //####################################################################################

    //Escritura de la imagen resultante en el archivo creado anteriormente
    retval = nc_open(SAVE_NAME, NC_WRITE, &ncid);
    if (retval != NC_NOERR) 
        ERR(retval);

    retval = nc_inq_varid(ncid, "CMI", &varid);
    if (retval != NC_NOERR) 
        ERR(retval);

    if ((retval = nc_put_vara_float(ncid,varid,start,conteo,result)))
        ERR(retval);

    if ((retval = nc_close(ncid)))
        ERR(retval);
}