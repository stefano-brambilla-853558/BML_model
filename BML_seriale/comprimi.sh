#!/bin/bash

# questo script serve per comprimere tutti i file necessari in un 
#archivio.tgz pronto per essere sottomesso sul sit

tar czf archive.tgz include/ source/ run.sh build.sh CMakeLists.txt

