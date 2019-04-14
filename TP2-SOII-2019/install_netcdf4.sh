#!/bin/bash

# Script to install hdf5 and netCDF4 libraries on a Linux Ubuntu system
# After: https://code.google.com/p/netcdf4-python/wiki/UbuntuInstall
# And http://unidata.github.io/netcdf4-python/ 

# You can check for newer version of the programs on 
# ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4/
# and other sources


mkdir source

BASHRC="~/.bashrc"
BASE_DIR=$HOME'/Development/SO2/hpc' # MODIFICAR
ZLIB_DIR=$BASE_DIR"/libs/zlib"
HDF5_DIR=$BASE_DIR"/libs/hdf5"
NETCDF4_DIR=$BASE_DIR"/libs/netcdf"

CC=gcc

mkdir -p $ZLIB_DIR
mkdir -p $HDF5_DIR
mkdir -p $NETCDF4_DIR

# Install zlib
cd source
v=1.2.8  
wget ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4/zlib-${v}.tar.gz
tar -xf zlib-${v}.tar.gz && cd zlib-${v}
./configure --prefix=$ZLIB_DIR
#sudo make check install
make install
cd ..

# Install HDF5
v=1.8.13
wget ftp://ftp.unidata.ucar.edu/pub/netcdf/netcdf-4/hdf5-${v}.tar.gz
tar -xf hdf5-${v}.tar.gz && cd hdf5-${v}
./configure --enable-shared --enable-hl --with-zlib=$ZLIB_DIR --prefix=$HDF5_DIR
make -j 2 # 2 for number of procs to be used
make install
cd ..

# Install Netcdf
v=4.1.3
wget http://www.unidata.ucar.edu/downloads/netcdf/ftp/netcdf-${v}.tar.gz
tar -xf netcdf-${v}.tar.gz && cd netcdf-${v}
CPPFLAGS="-I$HDF5_DIR/include -I$ZLIB_DIR/include": LDFLAGS="-L$HDF5_DIR/lib -L$ZLIB_DIR/lib" ./configure --enable-netcdf-4 --enable-shared  --prefix=$NETCDF4_DIR

#make check
make 
make install
cd ..

# install python's netCDF4
#sudo -E pip install netCDF4 --upgrade
