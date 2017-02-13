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




#include "in_out_functions.h"
#include "dense_matrix.h"
#include "sparse_matrix.h"
#include "hybrid_matrix.h"
#ifdef OPENMP
# include <omp.h>
#endif

/* main per il programma OpenMP del BML, corso di ACP.
   Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

   Per maggiori informazioni sulle funzioni e sulle classi usate, leggere gli altri file .cpp e .h
  */

/* NOTA ALLA VERSIONE IN OPENMP:
 * Prima di cominciare a scrivere il codice, abbiamo riflettuto su quali parti fossero parallelizzabili o meno.
 * abbiamo concluso che l'unico punto davvero parallelizzabile fosse il movimento della macchine, per evitare problemi di indeterminazione
 * con la lettura e scrittura del file.
 * Il modo più intelligente per parallelizzare il movimento della macchine ci è sembrato quello di assegnare alcune colonne a ciascun thread (per muovere le blu)
 * e alcune righe a ciascun thread (per muovere le rosse).
 * Partendo da questa ipotesi, abbiamo pensato già il seriale per parallelizzarlo in questo modo: di conseguenza il programma in OpenMP risulta di fatto identico al seriale
 * con l'unica differenza nella funzione dense_matrix::muovi e sparse_matrix::muovi, dove il ciclo for sulle righe e le colonne viene gestito da un pragma parallel for.
 * Di conseguenza, la maggior parte dei commenti al codice e alle classi sono state riportate nella sola versione seriale.
 */


const int BLU = 1;
const int RED = 2;

const char NESSUNA_MACCHINA= '0';
const char MACCHINA_BLU = '1';
const char MACCHINA_ROSSA= '2';

using namespace std;
using vect = vector <int>;
using iter = vect::iterator;
using uint =unsigned int;

int main (int argc, char* argv[]) {   // tramite argv passo il numero di processori da usare, ovvero quello massimo disponibile (uso nproc da terminale, e chiamo ./project nproc)



    // INIZIO CON IL TEMPO
        struct timeval start, end;
        float delta;
        gettimeofday(&start, NULL);

// APRO IL FILE
    const char* nomefile= "problem.csv";
    ifstream fin(nomefile);


//    try {if (fin.fail())
//        throw runtime_error("IMPOSSIBILE APRIRE IL FILE problem.csv");
//    }catch(runtime_error err){
//        cerr<< err.what()<<endl;
//        return 0;
//    }
    if (fin.fail()) {
        cout<<"IMPOSSIBILE APRIRE IL FILE problem.csv"<<endl;
        return 0;
    }

    string stringa;

    int Nrow=0;
    int Ncol=0;
    int Nrosse=0;
    int Nblu=0;





// LETTURA PRIMA RIGA (ITERAZIONI)
    vect N_iterazioni;
    N_iterazioni= iterazioni(&fin);
    sort(N_iterazioni.begin(), N_iterazioni.end());


// LETTURA DELLA PRIMA RIGA DELLA MATRICE

    riga_nonbanale(&fin, &stringa);
    ++Nrow;

    for (uint i=0; i<stringa.size(); ++i){              /// conto le colonne (quindi il numero di elementi uguali a 1,2 o 0)
        if (stringa[i]==NESSUNA_MACCHINA || stringa[i]== MACCHINA_BLU ||stringa[i]==MACCHINA_ROSSA)
            Ncol++;
    }

    // ciò che mi serve per la sparsa
    vect v(0);
    vector<vect> red (1,v);
    vector<vect> blu (Ncol, v);

    // ciò che mi serve per la densa
    vect riga(Ncol,0);
    vector<vect> matrix (0);
    iter it_riga=riga.begin();

    int temp=0;




    for (uint i=0; i<stringa.size(); ++i) { ///Calcolo il numero di macchine blu e rosse totali nella prima riga

        if (stringa[i]==NESSUNA_MACCHINA || stringa[i]== MACCHINA_BLU ||stringa[i]==MACCHINA_ROSSA){
            temp++;

            if (stringa[i]==MACCHINA_BLU){

                ++Nblu;

                ((blu)[temp-1]).push_back(Nrow-1);

                *it_riga=1;

            }

            else if (stringa[i]==MACCHINA_ROSSA){

                ++Nrosse;

                ((red)[Nrow-1]).push_back(temp-1);

                *it_riga=2;
            }

            else *it_riga=0;


            it_riga = it_riga + (it_riga!=riga.end());
        }

    }
//    try{    if (temp!= Ncol)
//            throw runtime_error("NUMERO DI COLONNE NON COSTANTE!!");
//    } catch(runtime_error err){
//        cerr<<err.what()<<endl;
//        return 0;
//    }

    if (temp!= Ncol){
     cout<< "NUMERO DI COLONNE NON COSTANTE!!"<<endl;
        return 0;
    }

    (matrix).push_back(riga);

// LETTURA DEL RESTO DELLA MATRICE

    while (!fin.eof() && !fin.fail() && riga_nonbanale(&fin, &stringa)!=0){

        ++Nrow;
        (red).push_back(v);

        temp=0;

        it_riga=riga.begin();

        for (uint i=0; i<stringa.size(); ++i) {

            if (stringa[i]==NESSUNA_MACCHINA || stringa[i]== MACCHINA_BLU ||stringa[i]==MACCHINA_ROSSA){
                temp++;

                if (stringa[i]==MACCHINA_BLU){

                    ++Nblu;

                    ((blu)[temp-1]).push_back(Nrow-1);

                    *it_riga=1;
                }

                else if (stringa[i]== MACCHINA_ROSSA){

                    ++Nrosse;

                    ((red)[Nrow-1]).push_back(temp-1);

                    *it_riga=2;}

                else *it_riga=0;


                it_riga = it_riga + (it_riga!=riga.end());
            }
        }

//        try{    if (temp!= Ncol&& temp!=0)
//                throw runtime_error("NUMERO DI COLONNE NON COSTANTE!!");
//        } catch(runtime_error err){
//            cerr<<err.what()<<endl;
//            return 0;
//        }
        if (temp!= Ncol){
         cout<< "NUMERO DI COLONNE NON COSTANTE!!"<<endl;
            return 0;
        }
        (matrix).push_back(riga);

    }





// CALCOLO DENSITÀ
//    float density=0;
//    density =(float)(Nblu+Nrosse)/(Nrow*Ncol);

if (1==2){  //density > (0.1 +(min(8,(float)sqrt(Nrow*Ncol)/5000))/100)

// DENSA
cout << "MATRICE DENSA"<<endl<<endl;
    dense_matrix matrice(&matrix, Nrow,Ncol);

    //MUOVI LE MACCHINE

    iter itera= N_iterazioni.begin();
    vector<int>::reverse_iterator fine= N_iterazioni.rbegin();

    vect cont(2,1);
    int j=0;

    for (j=0; j<*fine && (cont[BLU]!=0 ||cont[RED]!=0);j++){

        matrice.muovi((j+1)%2, cont,argv);

        if (j+1 == *itera){
            matrice.stampa(j+1);

            if(j+1!=*fine)
                itera++;
        }
    }
    if (j!=*fine){
    cout <<"la matrice si è bloccata alla " <<j<<"  iterazione"<<endl;
    matrice.stampa(j);}

    for (; j<*fine ;j++){
        if (j+1 == *itera){
            matrice.stampa(j+1);
            cout <<"Sto saltando movimenti:"<<endl;

            if(j+1!=*fine)
                itera++;
        }
    }

}
else if(1==2){

// SPARSA
    cout << "MATRICE SPARSA"<<endl<<endl;

   sparse_matrix sparsa (&blu,&red,Nrow, Ncol);

   //MOVIMENTI
   iter itera= N_iterazioni.begin();
   vector<int>::reverse_iterator fine= N_iterazioni.rbegin();

   vect cont2(2,1);
   int j=0;

//   itera= N_iterazioni.begin();
//   fine= N_iterazioni.rbegin();

//   vect cont2(2,1);
//    j=0;
   for (j=0; j<*fine && (cont2[BLU]!=0 ||cont2[RED]!=0);j++){  //

       sparsa.muovi((j+1)%2, cont2,argv);

       if (j+1 == *itera){
           sparsa.stampa(j+1);


           if(j+1!=*fine)
               itera++;
       }
   }
   if (j!=*fine){
   cout <<"la matrice si è bloccata alla " <<j<<"   iterazione"<<endl;
   sparsa.stampa(j);
   }

   for (; j<*fine ;j++){
       if (j+1 == *itera){
           sparsa.stampa(j+1);
           cout <<"sto saltando movimenti: "<<endl;


           if(j+1!=*fine)
               itera++;
       }
   }


}else{
    // HYBRID

    cout << "MATRICE IBRIDA"<<endl<<endl;

    hybrid_matrix ibrida (&matrix, &blu,&red,Nrow, Ncol);

    //MOVIMENTI
    iter itera= N_iterazioni.begin();
    vector<int>::reverse_iterator fine= N_iterazioni.rbegin();

    vect cont2(2,1);
    int j=0;

    // il contatore cont usato permette di tenere conto della presenza o meno di movimenti effettivi a una certa iterazione.
    // se la matrice si fosse bloccata a una certa iterazione (nessuna blu nè nessuna rossa si può muovere) allora
    // esce dal ciclo e stampa direttamente tutte le iterazioni successive.

    for (j=0; j<*fine && (cont2[BLU]!=0 ||cont2[RED]!=0);j++){

        ibrida.muovi((j+1)%2, cont2,argv);

        if (j+1 == *itera){
            ibrida.stampa(j+1);
            if(j+1!=*fine)
                itera++;
        }
    }
    if (j!=*fine){
        cout <<"la matrice si è bloccata alla " <<j<<"   iterazione"<<endl;
        ibrida.stampa(j);

    }

    for (; j<*fine ;j++){
        if (j+1 == *itera){
            ibrida.stampa(j+1);
            cout <<"sto saltando movimenti: "<<endl;

            if(j+1!=*fine)
                itera++;
        }
    }




}

gettimeofday(&end, NULL);
delta = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
cout<<"TEMPO FINALE = "<< delta <<endl<<endl;
start.tv_sec = end.tv_sec;
start.tv_usec = end.tv_usec;
    return 0;
}
