#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<istream>
#include<ostream>
#include<string>
#include<vector>

#include "in_out_functions.h"
#include "dense_matrix.h"
#include "sparse_matrix.h"


//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO
using namespace std;
using uint = unsigned int;



/// per le iterazioni da stampare
vector<int> iterazioni(ifstream *fin){
    string stringa;
    int cont22=0;
    while(cont22==0&& !fin->eof()){   /// questo while è per superare eventuali righe vuote
        if(!fin->eof() && !fin->fail())
            getline(*fin, stringa);
        for (uint i=0; i<stringa.size()&& cont22==0; ++i){
            if (stringa[i]=='0' || stringa[i]=='1' ||stringa[i]=='2' ||stringa[i]=='3' ||stringa[i]=='4' ||stringa[i]=='5' ||stringa[i]=='6' ||stringa[i]=='7' ||stringa[i]=='8' ||stringa[i]=='9' )
                cont22++;
        }
    }
    /// ho trovato una riga "buona" (con dei numeri)
    vector< int> N_itera; ///qua dentro ci metto il numero delle iterazioni
    vector<int> temp;   /// qua i numeri provvisori
    unsigned short  int a=0;
    for (uint i=0; i<stringa.size(); ++i){/// controlla tutta la stringa
        /// se non trovi una cifra
        if (stringa[i]!='0' && stringa[i]!='1' &&stringa[i]!='2' &&stringa[i]!='3' &&stringa[i]!='4' &&stringa[i]!='5' &&stringa[i]!='6' &&stringa[i]!='7' &&stringa[i]!='8' &&stringa[i]!='9' ){
            if (a !=0){ ///se hai già trovato una cifra
                ///compatta il vettore di cifre già trovate
                N_itera.push_back(compatta(&temp)); /// e inserisci il valore nel vettore del numero di iterazioni
                a=0; /// aspetta di trovare nuove cifre
            }
        }
        else{ /// se ho trovato una cifra
            int c=number(stringa[i]);  /// la trasformo in intero da char
            temp.push_back(c); /// e la aggiungo a temp
            a++;  /// memorizzo che ho trovato un cifra
        }
    }
    if (a !=0){ ///se hai già trovato una cifra
        ///compatta il vettore di cifre già trovate
        N_itera.push_back(compatta(&temp)); /// e inserisci il valore nel vettore del numero di iterazioni
    }

    ///quando finisco la stringa, ritorna il vettore delle iterazioni che hai formato
    return N_itera;
}







/// questa funzione è per superare eventuali righe vuote
int riga_nonbanale(ifstream *fin,string *stringa){
    string temp;
    int cont=0;
    while(cont==0&& !fin->eof()){ /// finchè non trovo una riga che contiene un 1, un 2, o uno 0, fai getline
        if(!fin->eof() && !fin->fail())
            getline(*fin, *stringa);
        temp=*stringa;
        for (uint i=0; i<temp.size()&& cont==0; ++i){
            if (temp[i]=='0' || temp[i]=='1' ||temp[i]=='2' ) /// se li trovi, cont++ ed esci da tutti i cicli
                cont++;
        }
    }

    return cont;
}




/// compatta un vettore v(1;2;8) nell'intero scritto: i=128
int compatta (vector<int> *temp){
    int risultato=0;
    vector<int>::iterator it22=temp->begin(); ///iteratore
    int i=temp->size()-1;
    while (it22!=temp->end()){   /// lungo tutto il vettore
        risultato= risultato +*it22* exp(10,i); /// moltiplica ciascuna cifra per 10^(...)
        it22 = temp->erase(it22); ///erase cancella l'elemento in it e restituisce l'iteratore all'elemento successivo
        i--;}
    /// con questa funzione, calcolo il numero "compattata" e cancello tutti i valori di temp;
    return risultato;
}





/// se c='1' allora return int=1
int number (char c){
    if (c=='0')
        return 0;
    else if (c=='1')
        return 1;
    else if (c=='2')
        return 2;
    else if (c=='3')
        return 3;
    else if (c=='4')
        return 4;
    else if (c=='5')
        return 5;
    else if (c=='6')
        return 6;
    else if (c=='7')
        return 7;
    else if (c=='8')
        return 8;
    else
        return 9;

}
/// calcola esponenziale
int exp(int base, int esponente){
    if (esponente==0)
        return 1;
    else
        return base*exp(base, esponente-1);
}



// minimo
float min(float a, float b){
    if (a<b)return a;
    else return b;




}

