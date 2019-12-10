procedural : readNetcdf.c
	gcc  -o readNet readNetcdf.c `nc-config --cflags --libs`

openmp :
	gcc  -o openMP OpenMp.c -fopenmp `nc-config --cflags --libs`

clean : 
	rm readNet openMP OR_ABI_Convolution.nc
