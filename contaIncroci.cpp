#include <iostream>
#include <fstream>
#include <vector>

#include "nonna.h"

static int C, G, M;

struct Centrino 
{
    int id;
    vector<int> gomitoli;
};

int partition(vector<int>& arr, int low, int high) 
{
    int pivot = arr[high];
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) 
{
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

class FenwickTree {
    public:
        FenwickTree(int size): size(size) 
        {
            tree.resize(size + 1, 0);
        }

        void update(int idx, int delta)
        {
            while (idx <= size) {
                tree[idx] += delta;
                idx += idx & -idx;
            }            
        }

        int query(int idx)
        {
            int sum = 0;
            while (idx > 0) {
                sum += tree[idx];
                idx -= idx & -idx;
            }
            return sum;
        }

        int rangeQuery(int l, int r)
        {
            return query(r) - query(l );
        }

        void print()
        {
            for (int i = 1; i <= size; ++i) {
                cout << tree[i] << " ";
            }
            cout << endl;
        }

    private:
        int size;
        vector<int> tree;
};

/**
 * Read input from file
 * @param path: path to the input file
 * @param centrini: vector of centrini
 * 
 * @note read the input file, insert in centrini ad quickSort gomitoli 
*/
void readInput(char path[], vector<Centrino>& centrini)
{
    fstream in(path, fstream::in);
    in >> C >> G >> M;
    centrini.resize(C);
    for (int i = 0; i < M; ++i) {
        int c, g;
        in >> c >> g;
        centrini[c].id = c;
        centrini[c].gomitoli.push_back(g);
    }
    in.close();
    for (int i = 0; i < C; ++i) {
        quickSort(centrini[i].gomitoli, 0, centrini[i].gomitoli.size() - 1);
    }
}

int calcolaIncroci(const vector<Centrino>& centrini)
{
    int incroci = 0;
    FenwickTree tree(G);
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < centrini[i].gomitoli.size(); ++j) {
            tree.print();//delete: debug
            cout<< "Centrino: " << centrini[i].id << " Gomitolo: " << centrini[i].gomitoli[j] << endl; //delete: debug

            incroci += tree.rangeQuery(centrini[i].gomitoli[j] - C + 1, G);
            tree.update(centrini[i].gomitoli[j]- C + 1, 1);

            cout << "Incroci: " << incroci << endl; //delete: debug
        }
    }
    return incroci;
}

int main()
{
    helpers::setup(); //delete: setup the timer and timeout

    int incroci=0;
    vector<Centrino> centrini;
    char path[] = "input/input19.txt";

    readInput(path, centrini);

    incroci = calcolaIncroci(centrini);

    //delete: print status and elapsed time
    cout << "C: " << C << " G: " << G << " M: " << M << endl;
    for (int i = 0; i < C; ++i) {
        cout << "Centrino " << centrini[i].id << ": ";
        for (int j = 0; j < centrini[i].gomitoli.size(); ++j) {
            cout << centrini[i].gomitoli[j] << " ";
        }
        cout << endl;
    }
    cout << "Incroci: " << incroci << endl;
    cout << "Elapsed time: " << helpers::get_elapsed_time() << endl;
    cout << "Has reached timeout? (0:false, 1:true): " << helpers::has_reached_timeout() << endl;
    return 0;
}
