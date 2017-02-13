#ifndef HYBRID_MATRIX_H
#define HYBRID_MATRIX_H

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

//Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO

/* La classe hybrid_matrix prova a gestire in modo intermedio i casi sparsa e denso: visto che durante la lettura da terminale creiamo di fatto entrambe le strutture,
 * creiamo una classe contenente sia la matrice piena, che i due vettori blu e red contenenti le sole posizioni delle macchine
 * Questa classe quindi prova a muovere le macchine partendo dai vettori della sparsa, ma anzichè fare un binary_search all'interno dell'altro vettore, va direttamente dentro
 * la matrice densa a controllare se può muoversi, rendendo questo sistema rapido anche per le matrici di media densità.
 */

class hybrid_matrix
{

    std::vector < std::vector <int> > *matrice;
    std::vector<std::vector<int>> *blu;
    std::vector<std::vector<int>>  *red;

    int nrow;
    int ncol;

public:
    hybrid_matrix(std::vector < std::vector <int> > *matrice,std::vector<std::vector<int>> *blu,std::vector<std::vector<int>> *red,int nrow, int ncol); //costruttore
    ~hybrid_matrix();  //distruttore

    void muovi_riga(int row,int &cont); // se possibile, ,muovi le macchine rosse di row
    void muovi_colonna(int col, int &cont); // se possibile, ,muovi le macchine blu di col
    void muovi(int quale, std::vector<int> &cont);     // fai un'iterazione: muovi prima le blu(colonne) poi tutte le rosse(righe)
    void print();  // stampa su cout la matrice
    void stampa (int itera); // stampa la matrice su file itera.csv


};

#endif // HYBRID_MATRIX_H
