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

//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

// costruttore
dense_matrix::dense_matrix( vector <vector <int> > *(matrice), int nrow, int ncol): matrice(matrice), nrow(nrow), ncol(ncol){

};

// distruttore
dense_matrix::~dense_matrix(){
};

/* muoviriga muove tutte le macchine rosse di una riga:
 * prima controlla se l'ultima si può muovere, e tiene un contatore di riferimento.
 *  dopodichè muove tutte le macchine successive della fila.
 * muovicolonna è speculare, per le blu*/

/* NOTA BENE la sintassi: il valore &cont tiene conto dei movimenti fatti:il main gestisce il caso in cui questo sia zero sia per le rosse che per le blu;
 * in tal caso non ci possono più essere movimenti e non fa più controlli.
 */

// se possibile, ,muovi le macchine rosse di row

void dense_matrix::muoviriga(int row, int &cont){
    //    int ultima_puo_muoversi=0;

    //    if ((*matrice)[row][0]==0 && (*matrice)[row][ncol-1]==RED) {
    //        ultima_puo_muoversi=1;
    //        (*matrice)[row][0]=RED;
    //        (*matrice)[row][ncol-1]=0;
    //        cont++;
    //    }


    //    for (int i=ultima_puo_muoversi; i< ncol-1-ultima_puo_muoversi; i++){
    //        if ((*matrice)[row][i]==RED)
    //            if((*matrice)[row][i+1]==0){
    //                (*matrice)[row][i]=0;
    //                (*matrice)[row][i+1]=RED;
    //                cont++;
    //                i++;
    //            }


    //    }


    /* E' stata fatta anche una seconda versione del codice, che non usasse nessun if per fare i controlli.
     * è comunque risultato più lento, quindi scartato; decidiamo comunque di riportarlo come alternativa alla soluzione del problema.*/

    int ultima_puo_muoversi = ((*matrice)[row][0]==0 && (*matrice)[row][ncol-1]==RED);
    (*matrice) [row][0] +=  RED*ultima_puo_muoversi;
    (*matrice) [row][ncol-1] += -  RED*ultima_puo_muoversi;

    int movimento=0;
    for (int i=ultima_puo_muoversi; i< ncol-1-ultima_puo_muoversi; i++){

        int puo_muoversi = ((*matrice)[row][i]==RED)&& ((*matrice)[row][i+1]==0);
        (*matrice)[row][i]+= - RED*puo_muoversi;
        (*matrice)[row][i+1]+= + RED* puo_muoversi;
        i= i + puo_muoversi;
        movimento += puo_muoversi;
    }
    cont +=ultima_puo_muoversi || movimento;

}


// se possibile, ,muovi le macchine blu di col

void dense_matrix::muovicolonna(int col, int &cont){

    //    int ultima_puo_muoversi=0;

    //    if ((*matrice)[0][col]==0 && (*matrice)[nrow-1][col]==BLU) {
    //        ultima_puo_muoversi=1;
    //        (*matrice)[0][col]=BLU;
    //        (*matrice)[nrow-1][col]=0;
    //        cont++;
    //    }

    //    for (int i=ultima_puo_muoversi; i< nrow-1-ultima_puo_muoversi; i++){
    //        if ((*matrice)[i][col]==BLU)
    //            if((*matrice)[i+1][col]==0){
    //                (*matrice)[i][col]=0;
    //                (*matrice)[i+1][col]=BLU;
    //                cont++;
    //                i++;
    //            }

    //    }


    // VERSIONE ALTERNATIVA SENZA IF

    int ultima_puo_muoversi = ((*matrice)[0][col]==0 && (*matrice)[nrow-1][col]==BLU);
    (*matrice) [0][col] +=  BLU*ultima_puo_muoversi;
    (*matrice) [nrow-1][col] += -  BLU*ultima_puo_muoversi;

    int movimento=0;
    for (int i=ultima_puo_muoversi; i< nrow-1-ultima_puo_muoversi; i++){

        int puo_muoversi = ((*matrice)[i][col]==BLU)&& ((*matrice)[i+1][col]==0);
        (*matrice)[i][col]+= - BLU*puo_muoversi;
        (*matrice)[i+1][col]+= + BLU* puo_muoversi;
        i+= + puo_muoversi;
        movimento+= + puo_muoversi;
    }
    cont +=+ ultima_puo_muoversi || movimento;
};




// FAI UN'ITERAZIONE:
/* int quale:   tiene conto di che tipo di iterazione stiamo facendo: ovvero, le iterazioni dispari muovono le blu, le iterazioni pari muovono le rosse
 * vector<int> &cont:  tiene conto del numero di movimenti fatti (per non fare più controlli dopo che non ci muoviamo più)
 */
void dense_matrix::muovi(int quale, vector<int> &cont){
    if(quale==1){
        cont[1]=0;
        for (int i=0; i<ncol; i++)muovicolonna(i, cont[1]);
    }
    else {
        cont[2]=0;
        for (int i=0; i<nrow; i++) muoviriga(i, cont[2]);
    }



};

// stampa in cout la matrice
void dense_matrix::print(){
    for (int i=0; i<nrow; i++){
        cout <<endl;
        for (int j=0; j<ncol; j++)
            cout << (*matrice) [i][j]<<", ";

    }



}

// STAMPA SU FILE la matrice;
// file:   itera.csv

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
