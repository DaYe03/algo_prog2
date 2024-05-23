#include "nonna.h"
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;


struct filo
{
  int centrino;
  int gomitolo;
};

int main() {

    int C;  //numero centrini
    int G;  //numero gomitoli
    int M;  //numero fili
    
    fstream in;
    in.open("input.txt");
    fstream out;
    out.open("output.txt");

    stack<int> centrini;    //stack contenente i centrini

    in>>C>>G>>M;

    for(int i=0; i<C; i++) {
        centrini.push(i);
    }

    vector<filo> fili;  //vettore/grafo che contiene i fili
    fili.resize(M);

    for(int i=0; i<M; i++) {

        int c;
        int g;
        in>>c>>g;
        fili[i].centrino=c;
        fili[i].gomitolo=g;

        out<<fili[i].centrino<<" "<<fili[i].gomitolo<<endl;
    }

    in.close();

    out.close();

    return 0;

}
