#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<istream>
#include<ostream>
#include<string>
#include<vector>
#include<algorithm>
#include<time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <typeinfo>
#include <exception>
#include <new>
#include "hybrid_matrix.h"

//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

const int BLU = 1;
const int RED = 2;

using namespace std;
using vect = vector <int>;
using iter = vect::iterator;
using uint =unsigned int;


//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

/* le funzioni seguenti sono di fatto molto simili a quelle della classe sparse_matrix, solo al posto dei vary binary search vengono messi controlli su matrice.
   */


//costruttore
hybrid_matrix::hybrid_matrix(vector <vector <int> > (*matrice),vector <vect>*blu,vector <vect> *red,int nrow, int ncol):matrice(matrice),blu(blu),red(red),nrow(nrow), ncol(ncol){};

//distruttore
hybrid_matrix::~hybrid_matrix(){
};


/* Le funzioni muovi_colonna e muovi_riga si basano essenzialmente su questo ragionamento:
 * se sto muovendo una macchina blu di una colonna, questa può essere bloccata:
 * - o da una macchina blu, che deve essere per forza nella stessa colonna e nella posizione successiva (per come costruisco la struttura le posizioni sono ordinate)
 * - o da una macchina rossa, che cercherò nella matrice piena.
 * Lo spostamento di una macchina viene fatto semplicemente aggiungendo 1 al valore della posizione della macchina, senza riordinare il vettore.
 * Infatti non c'è rischio di "sorpassi"; l'unico caso in cui riordiniamo è quando una macchina dell'ultima riga/colonna si muove: in questo caso con la funzione std::rotate
 * portiamo all'inizio del vettore la macchina in questione.*/

/* NOTA BENE la sintassi: il valore &cont tiene conto dei movimenti fatti:il main gestisce il caso in cui questo sia zero sia per le rosse che per le blu;
 * in tal caso non ci possono più essere movimenti e non fa più controlli.
 */


void hybrid_matrix::muovi_colonna(int i, int &cont){

    // PRIMO CONTROLLO:
    // cerca di capire in quale dei seguenti casi siamo:
    //   - se non ci sono macchine
    //   - se c'è una sola macchina nella colonna; se sì, se è in mezzo o se è in ultima riga.
    //   - se ci sono almeno due macchine blu.

    iter inizio= ((*blu)[i]).begin();
    iter fine  = ((*blu)[i]).end();
    int senza_macchine = (inizio == fine);
    int unica_macchina_mezzo = ( !senza_macchine && inizio+1==fine) && (*inizio+1!=nrow);
    int unica_macchina_fondo = ( !senza_macchine && inizio+1==fine) && (*inizio+1==nrow);
    int almeno_due_macchine_presenti = ( !senza_macchine && inizio+1!=fine && (*inizio+1)!=*(inizio+1));
    int prima_puo_muoversi_mezzo= (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( (*matrice)[*inizio+1][i]==0);
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( (*matrice)[0][i]==0);

    int movimento_interno=0;   // gestisce quanti movimenti sono stati fatti.

    //SECONDO CONTROLLO:
    // ciclo su tutte le altre macchine, tranne l'ultima.
    int entra_nel_ciclo = (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo) ;
    for (iter it=inizio+1; (it!=fine-1) && entra_nel_ciclo;it++){

        int puo_muoversi_blu = ((*it+1)!=*(it+1)) ;
        int puo_muoversi_red = ( (*matrice)[*it+1][i]==0);
        *it =*it+ ( puo_muoversi_blu&&puo_muoversi_red );
        (*matrice)[*it-( puo_muoversi_blu&&puo_muoversi_red)][i] += - BLU* ( puo_muoversi_blu&&puo_muoversi_red);
        (*matrice)[*it+1-( puo_muoversi_blu&&puo_muoversi_red)][i] +=  + BLU* ( puo_muoversi_blu&&puo_muoversi_red );
        movimento_interno += (puo_muoversi_blu&&puo_muoversi_red);


    }

    //TERZO CONTROLLO:
    // ultima macchina del vettore
    //NB: se c'è una sola macchina essa è considerata "prima"
    //    se ci sono solo due macchine sono considerate "prima" e "ultima"
    //    ci devono essere almeno 3 macchine per entrare nel ciclo del "secondo controllo"

    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi=0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
        non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=nrow) && ( (*matrice)[*(fine-1)+1][i]==0);
        ultima_riga_puo_muoversi = (*(fine-1)+1 ==nrow) && (*inizio!=0)&&( (*matrice)[0][i]==0);
        int temp = *(fine-1);
        *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        *(fine-1) = (*(fine-1))%nrow;
        (*matrice)[temp][i] +=- BLU* (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        (*matrice)[(temp+1)%nrow][i] += + BLU* (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);

    }
    // sposto effettivamente la prima macchina
    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        int temp2 = *(inizio);
        (*inizio)++; *inizio = (*inizio)%nrow;
        (*matrice)[temp2][i] +=- BLU* (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo);
        (*matrice)[(temp2+1)%nrow][i] += + BLU* (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo);

    }

    std:: rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);

    cont = cont +prima_puo_muoversi_fondo + prima_puo_muoversi_mezzo + (movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;
}




// muovi_riga è esattamente speculare a muovi_colonna

void hybrid_matrix::muovi_riga(int i, int &cont){

    iter inizio= ((*red)[i]).begin();
    iter fine  = ((*red)[i]).end();
    int senza_macchine = (inizio == fine);
    int unica_macchina_mezzo = ( !senza_macchine && inizio+1==fine) && (*inizio+1!=ncol);
    int unica_macchina_fondo = ( !senza_macchine && inizio+1==fine) && (*inizio+1==ncol);
    int almeno_due_macchine_presenti = ( !senza_macchine && inizio+1!=fine && (*inizio+1)!=*(inizio+1));
    int prima_puo_muoversi_mezzo = (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( (*matrice)[i][*inizio+1]==0);
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( (*matrice)[i][0]==0);

    int movimento_interno=0;

    int entra_nel_ciclo = (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo) ;
    for (iter it=inizio+1; (it!=fine-1) && entra_nel_ciclo;it++){

        int puo_muoversi_red = ((*it+1)!=*(it+1));
        int puo_muoversi_blu = ( (*matrice)[i][*it+1]==0);
        *it =*it+ ( puo_muoversi_blu&&puo_muoversi_red);
        (*matrice)[i][*it-( puo_muoversi_blu&&puo_muoversi_red )]+= - RED* ( puo_muoversi_blu&&puo_muoversi_red );
        (*matrice)[i][*it+1-( puo_muoversi_blu&&puo_muoversi_red)]+=+ RED* ( puo_muoversi_blu&&puo_muoversi_red);

        movimento_interno += + (puo_muoversi_blu&&puo_muoversi_red);

    }

    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi =0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
        non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=ncol) && ( (*matrice)[i][*(fine-1)+1]==0);
        ultima_riga_puo_muoversi = (*(fine-1)+1 ==ncol) && (*inizio!=0)&& ( (*matrice)[i][0]==0);

        int temp = *(fine-1);
        *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        *(fine-1) = (*(fine-1))%ncol;

        (*matrice)[i][temp] += -RED* (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        (*matrice)[i][(temp+1)%ncol] += + RED* (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);


    }

    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        int temp2 = *(inizio);
        (*inizio)++; *inizio = (*inizio)%ncol;
        (*matrice)[i][temp2] +=- RED* (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo);
        (*matrice)[i][(temp2+1)%nrow] += + RED* (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo);

    }


    rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);
    cont = cont + prima_puo_muoversi_fondo+ prima_puo_muoversi_mezzo +(movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;

}


// FAI UN'ITERAZIONE:
/* int quale:   tiene conto di che tipo di iterazione stiamo facendo: ovvero, le iterazioni dispari muovono le blu, le iterazioni pari muovono le rosse
 * vector<int> &cont:  tiene conto del numero di movimenti fatti (per non fare più controlli dopo che non ci muoviamo più)
 */

// Ricevo in argomento argv che contiene il numero di processori disponibili (ottenuto con nproc da terminale)
// faccio un pragma omp parallel for sui movimenti di colonne e righe


void hybrid_matrix::muovi(int quale, vector<int> &cont,char* argv[]){
    if(quale==1){
        cont[1]=0;
        int thread_count = strtol(argv[1],NULL,10);
        #pragma omp parallel for num_threads(thread_count)
    /*schedule (runtime)*/                                        /* Dopo diversi tentativi di schedule, non abbiamo notato significativi miglioramenti tra static, dynamic e guided
                                                                   * invece, senza schedule guadagniamo un tempo non trascurabile visto che non c'è l'overhead dovuto alla chiamata. */

        for (int i=0; i<ncol; i++)muovi_colonna(i, cont[1]);
    }
    else {
        cont[2]=0;
        int thread_count = strtol(argv[1],NULL,10);
        #pragma omp parallel for num_threads(thread_count)
   // schedule (runtime)
        for (int i=0; i<nrow; i++) muovi_riga(i, cont[2]);
    }

};

// stampa in cout la matrice
void hybrid_matrix::print(){

    cout<< "il vettorone (*blu) è : "<<endl;
    for (vector<vect>::iterator it=(*blu).begin(); it!=(*blu).end();it++){
        for (iter it2= (*it).begin(); it2!=(*it).end();it2++){
            cout << *it2<<", ";

        }
        cout <<endl;
    }
    cout<<endl;
    cout<< "il vettorone (*red) è : "<<endl;
    for (vector<vect>::iterator it=(*red).begin(); it!=(*red).end();it++){
        for (iter it2= (*it).begin(); it2!=(*it).end();it2++){
            cout << *it2<<", ";

        }
        cout <<endl;
    }
    cout<<endl;

}

// STAMPA SU FILE la matrice (funzione dalla densa, più efficiente di quella della sparsa.);
// file:   itera.csv

void hybrid_matrix::stampa (int itera){

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
