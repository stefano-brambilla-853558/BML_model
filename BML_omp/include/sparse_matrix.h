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

//       Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO


class sparse_matrix
{
    std::vector<std::vector<int>> *blu;
    std::vector<std::vector<int>> *red;
    int nrow;
    int ncol;

public:
    sparse_matrix(std::vector<std::vector<int>> *blu,std::vector<std::vector<int>> *red,int nrow, int ncol);
    ~sparse_matrix ();
    void print(); //stampa in cout
    void muovi_colonna(int i, int &cont);
    void muovi_riga(int i, int &cont);
    void muovi(int quale, std::vector<int> &cont,char* argv[]);     // fai unìiterazione: muovi prima le blu(colonne) poi tutte le rosse(righe)
    void stampa (int itera);
};

#endif // SPARSE_MATRIX_H
