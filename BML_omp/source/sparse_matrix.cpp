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


const int BLU = 1;
const int RED = 2;

using namespace std;
using vect = vector <int>;
using iter = vect::iterator;
using uint =unsigned int;

//       Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO


//costruttore
sparse_matrix::sparse_matrix(vector <vect> *blu,vector <vect> *red,int nrow, int ncol):blu(blu),red(red),nrow(nrow), ncol(ncol){};

//distruttore
sparse_matrix::~sparse_matrix(){
};

void sparse_matrix::print(){

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


void sparse_matrix::muovi_colonna(int i, int &cont){
//    cout <<"muovo colonna " << i<<endl;

    iter inizio= ((*blu)[i]).begin();
    iter fine  = ((*blu)[i]).end();
    int senza_macchine = (inizio == fine);
    int unica_macchina_mezzo = ( !senza_macchine && inizio+1==fine) && (*inizio+1!=nrow);
    int unica_macchina_fondo = ( !senza_macchine && inizio+1==fine) && (*inizio+1==nrow);
    int almeno_due_macchine_presenti = ( !senza_macchine && inizio+1!=fine && (*inizio+1)!=*(inizio+1));
    int prima_puo_muoversi_mezzo= (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( !binary_search(((*red)[*inizio+1]).begin(),((*red)[*inizio+1]).end(),i));
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( !binary_search(((*red)[0]).begin(),((*red)[0]).end(),i));
    //    int esattamente_due_macchine = (inizio!=fine && inizio+1!=fine &&inizio+2==fine);

    int movimento_interno=0;
//cout <<"primo controllo"<<endl;
    for (iter it=inizio+1; (it!=fine-1) && (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo);it++){
//cout <<"secondo controllo"<<endl;
        int puo_muoversi_blu = ((*it+1)!=*(it+1));
        int puo_muoversi_red = !binary_search(((*red)[*it+1]).begin(),((*red)[*it+1]).end(),i);
        *it =*it+ ( puo_muoversi_blu && puo_muoversi_red );
        movimento_interno = movimento_interno + (puo_muoversi_blu && puo_muoversi_red);
//cout <<"secondo controllo e mezzo"<<endl;

    }

//cout <<"terzo controllo"<<endl;
    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi=0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
    non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=nrow) &&  (!binary_search(((*red) [*(fine-1)+1]).begin(),((*red) [*(fine-1)+1]).end(),i));
    ultima_riga_puo_muoversi = (*(fine-1)+1 ==nrow) && (*inizio!=0)&&(!binary_search(((*red)[0]).begin(),((*red)[0]).end(),i));
//    cout <<"terzo mezzo controllo"<<endl;
    *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
    *(fine-1) = (*(fine-1))%nrow;
}
//  cout <<"quarto controllo"<<endl;
    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        (*inizio)++; *inizio = (*inizio)%nrow;}

    sort(inizio, fine);//rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);
    cont = cont +prima_puo_muoversi_fondo + prima_puo_muoversi_mezzo + (movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;
//cout <<"ultimo controllo"<<endl;
}





void sparse_matrix::muovi_riga(int i, int &cont){
//    cout <<"muovo riga " << i<<endl;

    iter inizio= ((*red)[i]).begin();
    iter fine  = ((*red)[i]).end();
    int senza_macchine = (inizio == fine);
    int unica_macchina_mezzo = ( !senza_macchine && inizio+1==fine) && (*inizio+1!=ncol);
    int unica_macchina_fondo = ( !senza_macchine && inizio+1==fine) && (*inizio+1==ncol);
    int almeno_due_macchine_presenti = ( !senza_macchine && inizio+1!=fine && (*inizio+1)!=*(inizio+1));
    int prima_puo_muoversi_mezzo= (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( !binary_search(((*blu)[*inizio+1]).begin(),((*blu)[*inizio+1]).end(),i));
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( !binary_search(((*blu)[0]).begin(),((*blu)[0]).end(),i));
    //    int esattamente_due_macchine = (inizio!=fine && inizio+1!=fine &&inizio+2==fine);

    int movimento_interno=0;

//cout <<"primo controllo"<<end/*l*/;
    for (iter it=inizio+1; (it!=fine-1) && (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo);it++){
//cout <<"secondo controllo"<<endl;
        int puo_muoversi_red = ((*it+1)!=*(it+1));
        int puo_muoversi_blu = !binary_search(((*blu)[*it+1]).begin(),((*blu)[*it+1]).end(),i);
        *it =*it+ ( puo_muoversi_blu && puo_muoversi_red );
        movimento_interno = movimento_interno + (puo_muoversi_blu && puo_muoversi_red);

//cout <<"secondo controllo e mezzo"<<endl;

    }

//cout <<"terzo controllo"<<endl;
    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi =0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
    non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=ncol) && ( !binary_search(((*blu) [*(fine-1)+1]).begin(),((*blu) [*(fine-1)+1]).end(),i));
    ultima_riga_puo_muoversi = (*(fine-1)+1 ==ncol) && (*inizio!=0)&&(!binary_search(((*blu)[0]).begin(),((*blu)[0]).end(),i));
//    cout <<"terzo mezzo controllo"<<endl;
    *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
    *(fine-1) = (*(fine-1))%ncol;
}
//  cout <<"quarto controllo"<<endl;
    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        (*inizio)++; *inizio = (*inizio)%ncol;}


    sort(inizio, fine);//rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);
    cont = cont + prima_puo_muoversi_fondo+ prima_puo_muoversi_mezzo +(movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;

//cout <<"ultimo controllo"<<endl;
// cout <<"muovo riga " << i<<endl;
//    iter inizio= ((*red)[i]).begin();
//    iter fine  = ((*red)[i]).end();


//    int cont = (inizio!=fine && inizio+1==fine)||( inizio!=fine && inizio+1!=fine && (*inizio+1)!=*(inizio+1)) && ( !binary_search(((*blu)[*inizio+1]).begin(),((*blu)[*inizio+1]).end(),i));

//    for (iter it=inizio+1; it!=fine-1;it++){

//        *it =*it+ (((*it+1)!=*(it+1))  && ( !binary_search(((*blu)[*it+1]).begin(),((*blu)[*it+1]).end(),i)));

//    }

//    *(fine-1) = *(fine-1) +  (((fine-1)!=inizio) && (*(fine-1)+1!=ncol) && binary_search(((*blu) [*(fine-1)+1]).begin(),((*blu) [*(fine-1)+1]).end(),i)) || (((fine-1)!=inizio)&& (*(fine-1)+1==ncol)&&(*inizio!=0)&&binary_search(((*blu)[0]).begin(),((*blu)[0]).end(),i));
//    *inizio= *inizio + cont;
//    if (*(fine-1)==ncol||*inizio==ncol) {
//        *(fine-1)=0;
//        rotate(inizio,fine-1,fine);
//    }

}


// fai un'iterazione: muovi prima le (*blu)(colonne) poi tutte le rosse(righe)

// Ricevo in argomento argv che contiene il numero di processori disponibili (ottenuto con nproc da terminale)
// faccio un pragma omp parallel for sui movimenti di colonne e righe

void sparse_matrix::muovi(int quale, vector<int> &cont, char* argv[]){
    if(quale==1){
        cont[1]=0;
        int thread_count = strtol(argv[1],NULL,10);
#pragma omp parallel for num_threads(thread_count)\
    schedule (dynamic)
        for (int i=0; i<ncol; i++)muovi_colonna(i, cont[1]); //, cont[1]
    }
    else {
        cont[2]=0;
        int thread_count = strtol(argv[1],NULL,10);
#pragma omp parallel for num_threads(thread_count)\
    schedule (dynamic)
        for (int i=0; i<nrow; i++) muovi_riga(i, cont[2]);   //, cont[2]
    }



};


// stampa su file
void sparse_matrix::stampa (int itera){
    stringstream ss;
    ss<< itera;
    string number= ss.str();
    string csv= ".csv";
    string prefix= "./output/";
    string iter= prefix + number + csv;
    char const *file = iter.c_str();
    ofstream fout;
    fout.open(file);  // ios::app
    int cont=0;
    for (int i=0; i<nrow;++i){
        for (int j=0; j<ncol-1; ++j){
            cont = 0 + BLU* binary_search(((*blu)[j]).begin(),((*blu)[j]).end(),i) + RED* binary_search(((*red)[i]).begin(),((*red)[i]).end(),j);
            fout << cont<<",";
        }
        cont = 0 + BLU* binary_search(((*blu)[ncol-1]).begin(),((*blu)[ncol-1]).end(),i) + RED* binary_search(((*red)[i]).begin(),((*red)[i]).end(),ncol-1);
        fout << cont<<endl;
    }
}
