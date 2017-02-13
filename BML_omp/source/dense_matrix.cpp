#ifdef OPENMP
# include <omp.h>
#endif
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<istream>
#include<ostream>
#include<string>
#include<vector>
#include<algorithm>

#include "in_out_functions.h"
#include "dense_matrix.h"
#include "sparse_matrix.h"

using namespace std;
const int RED=2;
const int BLU=1;

//       Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

// costruttore
dense_matrix::dense_matrix( vector <vector <int> > *(matrice), int nrow, int ncol): matrice(matrice), nrow(nrow), ncol(ncol){

};

// distruttore
dense_matrix::~dense_matrix(){
};


// se possibile, ,muovi le macchine rosse di row
void dense_matrix::muoviriga(int row, int &cont){
    int ultima_puo_muoversi=0;

    if ((*matrice)[row][0]==0 && (*matrice)[row][ncol-1]==RED) {
        ultima_puo_muoversi=1;
        (*matrice)[row][0]=RED;
        (*matrice)[row][ncol-1]=0;
        cont++;
    }


    for (int i=ultima_puo_muoversi; i< ncol-1-ultima_puo_muoversi; i++){
        if ((*matrice)[row][i]==RED)
            if((*matrice)[row][i+1]==0){
                (*matrice)[row][i]=0;
                (*matrice)[row][i+1]=RED;
                cont++;
                i++;
            }


    }


//int ultima_puo_muoversi = ((*matrice)[row][0]==0 && (*matrice)[row][ncol-1]==RED);
//(*matrice) [row][0] = (*matrice) [row][0] +  RED*ultima_puo_muoversi;
//(*matrice) [row][ncol-1] = (*matrice) [row][ncol-1] -  RED*ultima_puo_muoversi;

//int movimento=0;
//for (int i=ultima_puo_muoversi; i< ncol-1-ultima_puo_muoversi; i++){

//    int puo_muoversi = ((*matrice)[row][i]==RED)&& ((*matrice)[row][i+1]==0);
//            (*matrice)[row][i]= (*matrice)[row][i] - RED*puo_muoversi;
//            (*matrice)[row][i+1]=(*matrice)[row][i+1] + RED* puo_muoversi;
//            i= i + puo_muoversi;
//    movimento = movimento + puo_muoversi;
//}
//cont = cont +ultima_puo_muoversi || movimento;

}


// se possibile, ,muovi le macchine blu di col

void dense_matrix::muovicolonna(int col, int &cont){

    int ultima_puo_muoversi=0;

    if ((*matrice)[0][col]==0 && (*matrice)[nrow-1][col]==BLU) {
        ultima_puo_muoversi=1;
        (*matrice)[0][col]=BLU;
        (*matrice)[nrow-1][col]=0;
        cont++;
    }

    for (int i=ultima_puo_muoversi; i< nrow-1-ultima_puo_muoversi; i++){
        if ((*matrice)[i][col]==BLU)
            if((*matrice)[i+1][col]==0){
                (*matrice)[i][col]=0;
                (*matrice)[i+1][col]=BLU;
                cont++;
                i++;
            }

    }



//int ultima_puo_muoversi = ((*matrice)[0][col]==0 && (*matrice)[nrow-1][col]==BLU);
//(*matrice) [0][col] = (*matrice) [0][col] +  BLU*ultima_puo_muoversi;
//(*matrice) [nrow-1][col] = (*matrice) [nrow-1][col] -  BLU*ultima_puo_muoversi;

//int movimento=0;
//for (int i=ultima_puo_muoversi; i< nrow-1-ultima_puo_muoversi; i++){

//int puo_muoversi = ((*matrice)[i][col]==BLU)&& ((*matrice)[i+1][col]==0);
//        (*matrice)[i][col]= (*matrice)[i][col] - BLU*puo_muoversi;
//        (*matrice)[i+1][col]=(*matrice)[i+1][col] + BLU* puo_muoversi;
//        i= i + puo_muoversi;
//movimento = movimento + puo_muoversi;
//}
//cont = cont + ultima_puo_muoversi || movimento;
};



// fai un'iterazione: muovi prima le blu(colonne) poi tutte le rosse(righe)

// Ricevo in argomento argv che contiene il numero di processori disponibili (ottenuto con nproc da terminale)
// faccio un pragma omp parallel for sui movimenti di colonne e righe

void dense_matrix::muovi(int quale, vector<int> &cont, char* argv[]){
    if(quale==1){
        cont[1]=0;

        int thread_count = strtol(argv[1],NULL,10);
#pragma omp parallel for num_threads(thread_count)\
    schedule (dynamic)
        for (int i=0; i<ncol; i++)muovicolonna(i, cont[1]);
    }
    else {
        cont[2]=0;
        int thread_count = strtol(argv[1],NULL,10);
#pragma omp parallel for num_threads(thread_count)\
    schedule (dynamic)
        for (int i=0; i<nrow; i++) muoviriga(i, cont[2]);
    }



};

void dense_matrix::print(){
    for (int i=0; i<nrow; i++){
        cout <<endl;
        for (int j=0; j<ncol; j++)
            cout << (*matrice) [i][j]<<", ";

    }



}


void dense_matrix::stampa (int itera){
    stringstream ss;
    ss<< itera;
    string number= ss.str();
    string csv= ".csv";
    string prefix= "./output/";
    string iter= prefix + number + csv;
    char const *file = iter.c_str();
    ofstream fout;
    fout.open(file);  // ios::app
    for (int i=0; i<nrow;++i){
        for (int j=0; j<ncol-1; ++j){
            fout << (*matrice)[i][j]<<",";
        }
        fout<<(*matrice)[i][ncol-1]<<endl;
    }
}
