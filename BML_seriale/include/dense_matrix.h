#ifndef DENSE_MATRIX_H
#define DENSE_MATRIX_H

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
#include "sparse_matrix.h"

//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO
/*
La classe dense_matrix è per le matrici ad altà densità: è semplicemente una matrice fatta da un vettore di vettori, contenenti 0,1, e 2.
inoltre contiene le dimensioni della matrice nrow e ncol.

*/

class dense_matrix
{
    std::vector < std::vector <int> > *matrice;
    int nrow;
    int ncol;

public:
    dense_matrix(std::vector < std::vector <int> > *matrice, int nrow, int ncol);  //costruttore
    ~dense_matrix();  //distruttore
    void muoviriga(int row,int &cont); // se possibile, ,muovi le macchine rosse di row
    void muovicolonna(int col, int &cont); // se possibile, ,muovi le macchine blu di col
    void muovi(int quale, std::vector<int> &cont);     // fai un'iterazione: muovi prima le blu(colonne) poi tutte le rosse(righe)
    void print();  // stampa su cout la matrice
    void stampa (int itera); // stampa la matrice su file itera.csv
};


#endif // DENSE_MATRIX_H
