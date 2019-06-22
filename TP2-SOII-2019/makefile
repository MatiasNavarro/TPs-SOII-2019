all: icc

icc :
	icc -xCORE_AVX512 -std=gnu99 -Wall -Werror -pedantic -L/home/Estudiante52/Development/SO2/hpc/libs/netcdf/lib/ -lnetcdf -O3 -o openMP openMP.c -fopenmp

gcc :
	gcc -std=gnu99 -Wall -Werror -pedantic -L/home/Estudiante52/Development/SO2/hpc/libs/netcdf/lib/ -lnetcdf -O3 -o openMP openMP.c -fopenmp

clean : 
	rm openMP OR_ABI_Convolution.nc

