#ifndef IN_OUT
#define IN_OUT

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<istream>
#include<ostream>
#include<string>
#include<vector>

#include "dense_matrix.h"
#include "sparse_matrix.h"

//       Programma di STEFANO BRAMBILLA e FRANCESCO FACCIO



// creare il vettore con le iterazioni da fare da file
std::vector<int> iterazioni(std::ifstream *fin);

// scorri fin finchè non trovi una riga "non banale" ovvero con almeno un 1 , un 2 o uno 0 (return getline di quella riga)
int riga_nonbanale(std::ifstream *fin,std::string *stringa);

// presa un vettore di int, restituire l'int dalla lettura delle vettore: v(1;4;5)---> return 145;
int compatta (std::vector<int> *temp);

//trasforma c char numero nel giusto intero
int number (char c);

//calcolare l'esponenziale di base^esponenete
int exp(int base, int esponente);

// minimo
float min(float a, float b);


#endif //IN_OUT

