#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<istream>
#include<ostream>
#include<string>
#include<vector>
#include<cstdlib>
using namespace std;

/*
Crea un file problem.csv, che rappresenta lo stato iniziale del problema; 
da terminale, chiede all'utente di inserire le dimensioni e la sparsità della matrice.
*/

int main(){

ofstream fout("./../BML_seriale/problem.csv");

fout << "1, 2,10,1000"<<endl;
int row=0;
int col=0;
int density=0;
cout<<"row:  "; cin>>row; cout<<endl;
cout<<"col:  "; cin>>col; cout<<endl;
cout<<"density  (0 < density < 100):    "; cin>>density;cout<<endl;


for (int i=0; i<row;++i){
     for (int j=0; j<col; ++j){
     int x=rand()%100+1;
     if (x<density) x=x%2+1;
     else x=0;
     fout<<x<<",";
     }
     fout<<'\n';
     }









}
