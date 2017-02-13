
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


//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO
//costruttore
sparse_matrix::sparse_matrix(vector <vect>*blu,vector <vect> *red,int nrow, int ncol):blu(blu),red(red),nrow(nrow), ncol(ncol){};

//distruttore
sparse_matrix::~sparse_matrix(){
};


/* Le funzioni muovi_colonna e muovi_riga si basano essenzialmente su questo ragionamento:
 * se sto muovendo una macchina blu di una colonna, questa può essere bloccata:
 * - o da una macchina blu, che deve essere per forza nella stessa colonna e nella posizione successiva (per come costruisco la struttura le posizioni sono ordinate)
 * - o da una macchina rossa, che cercherò nella struttura red con un std::binary_search (NB anche le rosse devono essere ordinate!)
 * Lo spostamento di una macchina viene fatto semplicemente aggiungendo 1 al valore della posizione della macchina, senza riordinare il vettore.
 * Infatti non c'è rischio di "sorpassi"; l'unico caso in cui riordiniamo è quando una macchina dell'ultima riga/colonna si muove: in questo caso con la funzione std::rotate
 * portiamo all'inizio del vettore la macchina in questione.*/

/* NOTA BENE la sintassi: il valore &cont tiene conto dei movimenti fatti:il main gestisce il caso in cui questo sia zero sia per le rosse che per le blu;
 * in tal caso non ci possono più essere movimenti e non fa più controlli.
 */


void sparse_matrix::muovi_colonna(int i, int &cont){

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
    int prima_puo_muoversi_mezzo= (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( !binary_search(((*red)[*inizio+1]).begin(),((*red)[*inizio+1]).end(),i));
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( !binary_search(((*red)[0]).begin(),((*red)[0]).end(),i));

    int movimento_interno=0;   // gestisce quanti movimenti sono stati fatti.

    //SECONDO CONTROLLO:
    // ciclo su tutte le altre macchine, tranne l'ultima.
    for (iter it=inizio+1; (it!=fine-1) && (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo);it++){
        int puo_muoversi_blu = ((*it+1)!=*(it+1));
        int puo_muoversi_red = !binary_search(((*red)[*it+1]).begin(),((*red)[*it+1]).end(),i);
        *it =*it+ ( puo_muoversi_blu && puo_muoversi_red );
        movimento_interno = movimento_interno + (puo_muoversi_blu && puo_muoversi_red);

    }

    //TERZO CONTROLLO:
    // ultima macchina del vettore
    //NB: se c'è una sola macchina essa è considerata "prima"
    //    se ci sono solo due macchine sono considerate "prima" e "ultima"
    //    ci devono essere almeno 3 macchine per entrare nel ciclo del "secondo controllo"

    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi=0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
        non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=nrow) &&  (!binary_search(((*red) [*(fine-1)+1]).begin(),((*red) [*(fine-1)+1]).end(),i));
        ultima_riga_puo_muoversi = (*(fine-1)+1 ==nrow) && (*inizio!=0)&&(!binary_search(((*red)[0]).begin(),((*red)[0]).end(),i));

        *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        *(fine-1) = (*(fine-1))%nrow;
    }
    // sposto effettivamente la prima macchina
    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        (*inizio)++; *inizio = (*inizio)%nrow;}

    //sort(inizio, fine);
    std:: rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);

    cont = cont +prima_puo_muoversi_fondo + prima_puo_muoversi_mezzo + (movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;
}




// muovi_riga è esattamente speculare a muovi_colonna

void sparse_matrix::muovi_riga(int i, int &cont){

    iter inizio= ((*red)[i]).begin();
    iter fine  = ((*red)[i]).end();
    int senza_macchine = (inizio == fine);
    int unica_macchina_mezzo = ( !senza_macchine && inizio+1==fine) && (*inizio+1!=ncol);
    int unica_macchina_fondo = ( !senza_macchine && inizio+1==fine) && (*inizio+1==ncol);
    int almeno_due_macchine_presenti = ( !senza_macchine && inizio+1!=fine && (*inizio+1)!=*(inizio+1));
    int prima_puo_muoversi_mezzo= (unica_macchina_mezzo || almeno_due_macchine_presenti) && ( !binary_search(((*blu)[*inizio+1]).begin(),((*blu)[*inizio+1]).end(),i));
    int prima_puo_muoversi_fondo = unica_macchina_fondo && ( !binary_search(((*blu)[0]).begin(),((*blu)[0]).end(),i));

    int movimento_interno=0;

    for (iter it=inizio+1; (it!=fine-1) && (!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo);it++){

        int puo_muoversi_red = ((*it+1)!=*(it+1));
        int puo_muoversi_blu = !binary_search(((*blu)[*it+1]).begin(),((*blu)[*it+1]).end(),i);
        *it =*it+ ( puo_muoversi_blu && puo_muoversi_red);
        movimento_interno = movimento_interno + (puo_muoversi_blu && puo_muoversi_red);


    }

    int ultima_riga_puo_muoversi =0;
    int non_ultima_riga_puo_muoversi =0;
    if ((!senza_macchine && !unica_macchina_fondo && !unica_macchina_mezzo)){
        non_ultima_riga_puo_muoversi =  (*(fine-1)+1!=ncol) && ( !binary_search(((*blu) [*(fine-1)+1]).begin(),((*blu) [*(fine-1)+1]).end(),i));
        ultima_riga_puo_muoversi = (*(fine-1)+1 ==ncol) && (*inizio!=0)&&(!binary_search(((*blu)[0]).begin(),((*blu)[0]).end(),i));

        *(fine-1) = *(fine-1) +   (non_ultima_riga_puo_muoversi || ultima_riga_puo_muoversi);
        *(fine-1) = (*(fine-1))%ncol;
    }

    if (prima_puo_muoversi_mezzo||prima_puo_muoversi_fondo){
        (*inizio)++; *inizio = (*inizio)%ncol;}


    //sort(inizio, fine);
    rotate(inizio,fine-(ultima_riga_puo_muoversi|| prima_puo_muoversi_fondo),fine);
    cont = cont + prima_puo_muoversi_fondo+ prima_puo_muoversi_mezzo +(movimento_interno) + non_ultima_riga_puo_muoversi + ultima_riga_puo_muoversi;

}


// FAI UN'ITERAZIONE:
/* int quale:   tiene conto di che tipo di iterazione stiamo facendo: ovvero, le iterazioni dispari muovono le blu, le iterazioni pari muovono le rosse
 * vector<int> &cont:  tiene conto del numero di movimenti fatti (per non fare più controlli dopo che non ci muoviamo più)
 */

void sparse_matrix::muovi(int quale, vector<int> &cont){
    if(quale==1){
        cont[1]=0;
        for (int i=0; i<ncol; i++)muovi_colonna(i, cont[1]);
    }
    else {
        cont[2]=0;
        for (int i=0; i<nrow; i++) muovi_riga(i, cont[2]);
    }

};

// stampa in cout la matrice
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

// STAMPA SU FILE la matrice;
// file:   itera.csv

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
