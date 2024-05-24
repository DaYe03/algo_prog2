//#include "nonna.h"
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;


/*struct filo
{
  int n1;
  int n2;
};*/

int incroci(vector<vector<int>> fili, int C, int G);


int main() {

    long int C;  //numero centrini
    long int G;  //numero gomitoli
    long int M;  //numero fili
    
    fstream in;
    in.open("input0.txt");
    fstream out;
    out.open("output.txt");

    stack<long int> centrini;    //stack contenente i centrini

    in>>C>>G>>M;

    /*for(int i=0; i<C; i++) {
        centrini.push(i);
    }*/

    vector<vector<int>> fili;       //vettore/grafo che contiene i fili
    

    for(int i=0; i<M; i++) {

        int c;
        int g;

        in>>c>>g;

        //!qui va in seg fault ma non ho troppo capito perchè
        fili[c].push_back(g);
        //fili[g].push_back(c);
                
        cout<<"ok"<<endl;

    }

    cout<<incroci(fili,C,G);

    in.close();

    out.close();

    return 0;

}



int incroci(vector<vector<int>> fili, int C, int G) {
    int incroci=0;
    vector<vector<bool>> visited;
        cout<<"ok"<<endl;

    for(int i=0; i<C; i++) {    //inizializzazione vettore visited
        for (int j=0; j<G; j++){
            visited[i][j]=false;            
        }    
        cout<<"ok"<<endl;

       for(int c=0; c<(C-1); c++) {                         //per ogni centrino da 0 a C-2(l'ultimo centrino è inutile da controllare)
            for(int g : fili[c]){    
                cout<<"ok"<<endl;                           //per ogni collegamento che il centrino ha con un gomitolo
                if(visited[c][g]==false) {     
                cout<<"ok"<<endl;                           //(se il collegamento non + stato già visitato precedentemente)
                    if(c!=0 || g !=((C+G)-1)) {  
                    cout<<"ok"<<endl;                       //(se il collegamento non è tra il centrino 0 e il gomitolo C+G-1 (cioè primo centrino-ultimo gomitolo) )
                        for (int c2=c; c2<C; c2++) { 
                        cout<<"ok"<<endl;                   //verifico per ogni centrino maggiore di quello che sto considerando
                            for(int g2 : fili[c2]){         //se esiste un collegamento tra quel centrino c2>c con un gomitolo g2<g
                                if(g2<g){
                                    incroci++;              //se esiste ho trovato un incrocio
                                    visited[c2][g2]=true;   //per evitare conti duplicati segno il filo come visitato
                                                            //(per questo nell'ultimo centrino non avrò più incroci)       
                                }
                            }
                        }

                    }

                    visited[c][g]==true;                    //segno il filo come visitato
                }
            }
       }
    }

    return incroci;
    
}