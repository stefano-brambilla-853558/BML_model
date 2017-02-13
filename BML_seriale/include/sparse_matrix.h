#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

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


//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

/* La classe sparse_matrix è per gestire il caso a bassa densità: ci sono due strutture dati, entrambe vettori di vettori di interi, uno per le blu e uno per le rosse;
 * questi contengono le posizioni delle sole macchine presenti nella matrice, ripartite per riga (rosse) o per colonna (blu), basandosi innanzitutto sull'ipotesi che una
 * macchina rossa rimarrà sempre nella sua riga d'origine, così come una blu sarà sempre nella sua colonna.
 * Quindi, per esempio, data:
 * 1 0 0 1 2
 * 2 1 0 0 1
 * 0 2 2 1 0
 *
 * red =   4
 *         0
 *         1, 2
 *
 * blu =   0
 *         1
 *
 *         0, 2
 *         1
 *
 * Le righe/colonne senza macchine rosse/blu sono comunque presenti come vettori vuoti.
 * */


class sparse_matrix
{
    std::vector<std::vector<int>> *blu;
    std::vector<std::vector<int>>  *red;
    int nrow;
    int ncol;

public:
    sparse_matrix(std::vector<std::vector<int>> *blu,std::vector<std::vector<int>> *red,int nrow, int ncol);  // costruttore
    ~sparse_matrix (); // distruttore
    void muovi_colonna(int i, int &cont);  // muovi tutte le blu della colonna i
    void muovi_riga(int i, int &cont);     // muovi tutte le rosse della riga i
    void muovi(int quale, std::vector<int> &cont);     // fai un'iterazione: muovi prima le blu(colonne) poi tutte le rosse(righe)
    void print(); //stampa in cout
    void stampa (int itera);  // stampa su file
};

#endif // SPARSE_MATRIX_H
