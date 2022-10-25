#!/usr/bin/csh

echo "Execute this script with source setup_JLab_omega.csh"
source /apps/root/6.18.04/setroot_CUE.csh
module load cmake/3.19.4
eval cmake --version

# source /site/12gev_phys/softenv.csh 2.4

if ( ! -d "build" ) then
    echo "Build directory does not exist..."
    mkdir build
    echo "Created build directory."
endif

echo "Building cpp enviroment..."
cd build/
cmake ../
make

