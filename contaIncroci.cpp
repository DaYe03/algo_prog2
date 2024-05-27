#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "nonna.h"

static int C, G, M;

struct Centrino 
{
    int id;
    vector<int> gomitoli;
    float barycenter;
};

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

int calcolaIncroci(const vector<Centrino>& centrini)
{
    int incroci = 0;
    FenwickTree tree(G);
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < centrini[i].gomitoli.size(); ++j) {
            incroci += tree.rangeQuery(centrini[i].gomitoli[j] - C + 1, G);
            tree.update(centrini[i].gomitoli[j]- C + 1, 1);
        }
    }
    return incroci;
}


void setBarycenter(vector<Centrino>& centrini)
{
    float sum = 0;
    for (int i = 0; i < centrini.size(); i++) {
        sum = 0;
        if (centrini[i].gomitoli.size() != 0) {
            for (int j = 0; j < centrini[i].gomitoli.size(); j++)
            {
                sum += centrini[i].gomitoli[j];
            }
            centrini[i].barycenter = sum / centrini[i].gomitoli.size();
        }
        else {
            centrini[i].barycenter = 0;
        }

    }
}

void orderByBarycenter (vector<Centrino>& centrini, int incroci)
{
    setBarycenter(centrini);
    int i = 0, minIncroci = incroci, notImprovementeCount = 0, notImprovementeLimit = 100;
    vector<Centrino> minCentrini = centrini;
    while(i < 100 && notImprovementeCount < notImprovementeLimit) 
    {
        sort(centrini.begin(), centrini.end(), [](const Centrino& a, const Centrino& b) {
        return a.barycenter < b.barycenter;
        });

        int currentIncroci = calcolaIncroci(centrini);

        if (currentIncroci < minIncroci) {
            minIncroci = currentIncroci;
            minCentrini = centrini;
            notImprovementeCount = 0; 
        } else {
            notImprovementeCount++;
        }

        i++;
    }
    centrini = minCentrini;
}

/**
 * Read input from file
 * @param path: path to the input file
 * @param centrini: vector of centrini
 * 
 * @note read the input file, insert in centrini and gomitoli
*/
void readInput(const char path[], vector<Centrino>& centrini, vector<Centrino>& tmp)
{
    fstream in(path, fstream::in);
    in >> C >> G >> M;
    centrini.resize(C);
    tmp.resize(C);

    for (int i = 0; i < C; i++)
    {
        centrini[i].id = i;
        tmp[i].id = i;
    }
    
    for (int i = 0; i < M; ++i) {
        int c, g;
        in >> c >> g;

        auto& gomitoli = centrini[c].gomitoli;
        auto pos = lower_bound(gomitoli.begin(), gomitoli.end(), g);
        gomitoli.insert(pos, g);

        auto& gomitoliTmp = tmp[c].gomitoli;
        auto posTmp = lower_bound(gomitoliTmp.begin(), gomitoliTmp.end(), g);
        gomitoliTmp.insert(posTmp, g);
    }
    in.close();
}


void writeOutput(int incroci, const std::vector<Centrino>& centrini)
{
    std::fstream out("output.txt", std::fstream::out);
    if (!out) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    out << incroci << std::endl;
    for (int i = 0; i < centrini.size(); i++) {
        out << centrini[i].id << " ";
    }
    out << std::endl << "***";
    out.close();
}

int main()
{
    // helpers::setup(); //delete: setup the timer and timeout

    int tmpIncroci=0, incroci=0;
    vector<Centrino> centrini;
    vector<Centrino> tmp;
    // char path[] = "input/input12.txt";
    char path[] = "input.txt";

    readInput(path, centrini, tmp);
    incroci = calcolaIncroci(centrini);
    
    orderByBarycenter(tmp,incroci);
    tmpIncroci =  calcolaIncroci(tmp);

    if (incroci < tmpIncroci) {
        writeOutput(incroci, centrini);
    } else {
        writeOutput(tmpIncroci, tmp);
    }

    // std::cout << "C: " << C << " G: " << G << " M: " << M << std::endl;
    // std::cout << "Incroci: " << incroci << std::endl;

    // std::cout << "Incroci: " << tmpIncroci << std::endl;

    
    return 0;
}

