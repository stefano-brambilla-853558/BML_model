#!/bin/bash

# chmod +x build.sh
# per dargli l'autorizzazione

#per utilizzarlo, vai nella cartella project da terminale (con cd)
# poi fai     ./build.sh

rm -rf build/ 
mkdir build/
cd build/

cmake ..

make project 
cp project ..
