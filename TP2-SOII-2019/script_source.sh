#!/bin/bash

source /opt/intel/compilers_and_libraries_2018.5.274/linux/bin/compilervars_global.sh -arch intel64 -platform linux
source /opt/intel/compilers_and_libraries_2018.5.274/linux/bin/compilervars.sh -arch intel64 -platform linux
export C_INCLUDE_PATH=$C_INCLUDE_PATH:~/Development/SO2/hpc/libs/netcdf/include/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Development/SO2/hpc/libs/netcdf/lib/
