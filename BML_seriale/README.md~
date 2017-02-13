#BIHAM-MIDDLETON-LEVINE TRAFFIC MODEL

##PROGETTO DI STEFANO BRAMBILLA E FRANCESCO FACCIO
Stefano Brambilla: <s.brambilla93@gmail.com>
Francesco Faccio: <francesco.faccio.93@gmail.com>

###Progetto svolto per l'esame "Algoritmi e Calcolo Parallelo" 
###Politecnico di Milano  a.a. 2015/2016

## IL MODELLO DI BIHAM-MIDDLETON-LEVINE
https://en.wikipedia.org/wiki/Biham%E2%80%93Middleton%E2%80%93Levine_traffic_model


## IL CODICE
### COMPILARE ED ESEGUIRE
Per compilare ed eseguire il codice, usare da terminale gli shell script:

VERSIONE SERIALE
%% 
cd BML_seriale
chmod +x build.sh
chmod +x run.sh

./build.sh
./run.sh
%%


VERSIONE IN PARALLELO
%% 
cd BML_omp
chmod +x build.sh
chmod +x run.sh

./build.sh
./run.sh
%%


i file contenenti gli step della simulazione si trovano nella cartella "output".

### FILE DI INPUT
l'input è il file problem.csv, contenente:
a) nella prima riga, quali sono le iterazioni che dovranno essere stampate
b) successivamente, la matrice degli elementi.

Nella cartella "problem" è possibile generare un file di input casuale, secondo dei parametri scelti dall'utente (dimensioni della matrice e sparsità della matrice). usare da terminale i seguenti comandi:

MODIFICARE L'INPUT DELLA VERSIONE SERIALE
%%
cd problem
chmod +x problem.sh
./problem.sh 
%%

MODIFICARE L'INPUT DELLA VERSIONE IN PARALLELO
%%
cd problem
chmod +x problem_omp.sh
./problem_omp.sh 
%%




