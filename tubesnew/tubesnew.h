#ifndef TUBESNEW_H_INCLUDED
#define TUBESNEW_H_INCLUDED
#include <cmath> // pow()
#include <string> // string
#include <iostream>
#include <stdlib.h> // atof() fungsi ngubah dari string ke float
#include <vector> // vector

using namespace std;

struct tree{
    string isi; //nampung isi pertama kali saat parsing
    bool boolcarrier; //informasi apakah dia pembawa boolean atau angka
    bool nilai; //penampung nilai boolean hasil operasi
    float value; // penampung nilai angka hasil operasi
    tree* left;
    tree* right;
};

int precedence(string a);
tree* maketree(string isi);
tree* mengisi(tree* current, string ekspresi[],int& index);
//void ngetest(tree* current);
tree* hasil(tree* current );
tree* evaluasi(tree* current, float& temp1 , float& temp2, bool& tempbool1 , bool& tempbool2);
#endif // TUBESNEW_H_INCLUDED
