#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "nonna.h"

static int C, G, M;

struct Centrino 
{
    vector<int> gomitoli;
    float barycenter;
    float mediana;
};

/**
 * Fenwick Tree Implementation
*/
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
 * Calculate the number of cross
 * @param permutation: vector of permutation
 * @param centrini: vector of centrini
 * @return the number of cross
 * 
 * @note calculate the number of cross using the Fenwick Tree
*/
int calculateCross(const vector<int> permutation, const vector<Centrino>& centrini)
{
    int cross = 0;
    FenwickTree tree(G);
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < centrini[permutation[i]].gomitoli.size(); ++j) {
            cross += tree.rangeQuery(centrini[permutation[i]].gomitoli[j] - C + 1, G); // if there are greater gomitoli already visited add to cross
            tree.update(centrini[permutation[i]].gomitoli[j]- C + 1, 1); // save the visit of the gomitolo
        }
    }
    return cross;
}

/**
 * Set the barycenter of each centrino
 * @param centrini: vector of centrini
 * 
 * @note calculate the barycenter of each centrino
*/
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
            centrini[i].barycenter = 0; // if there are no gomitoli the barycenter is 0
        }
    }
}

/**
 * Order the permutation by barycenter
 * @param permutation: vector of permutation
 * @param centrini: vector of centrini
 * @param incroci: number of cross to compare
 * @return the number of cross
 * 
 * @note order the permutation by barycenter and calculate the number of cross
*/
int orderByBarycenter (vector<int> &permutation,vector<Centrino>& centrini, int incroci)
{
    int notImprovementeLimit = 100, notImprovementeCount = 0;

    int  minIncroci = incroci;
    vector<int> minPermutation = permutation;
    
    setBarycenter(centrini);

    while(notImprovementeCount < notImprovementeLimit) // if there are no improvement stop
    {
        sort(permutation.begin(), permutation.end(), [&centrini](int a, int b) {
            return centrini[a].barycenter < centrini[b].barycenter;
        });

        int currentIncroci = calculateCross(permutation, centrini);

        if (currentIncroci < minIncroci) {
            minIncroci = currentIncroci;
            minPermutation = permutation;
            notImprovementeCount = 0; 
        } else {
            notImprovementeCount++;
        }
    }
    permutation = minPermutation;
    return minIncroci;
}

/**
 * Set the mediana of each centrino
 * @param centrini: vector of centrini
 * 
 * @note calculate the mediana of each centrino
*/
void setMediana(vector<Centrino>& centrini)
{
    for (int i = 0; i < centrini.size(); i++) {
        if (centrini[i].gomitoli.size() == 0) {
            centrini[i].mediana = 0.0f; // if there are no gomitoli the mediana is 0
        } 
        else {
            if (centrini[i].gomitoli.size() % 2 == 0) {
                centrini[i].mediana = (centrini[i].gomitoli[centrini[i].gomitoli.size() / 2] + centrini[i].gomitoli[centrini[i].gomitoli.size() / 2 - 1]) / 2.0f;
            } else {
                centrini[i].mediana = static_cast<float>(centrini[i].gomitoli[centrini[i].gomitoli.size() / 2]);
            }
        }
    }
}

/**
 * Order the permutation by mediana
 * @param permutation: vector of permutation
 * @param centrini: vector of centrini
 * @param incroci: number of cross to compare
 * @return the number of cross
 * 
 * @note order the permutation by mediana and calculate the number of cross
*/
int orderByMediana (vector<int> &permutation,vector<Centrino>& centrini, int incroci)
{
    int notImprovementeLimit = 100, notImprovementeCount = 0;

    int  minIncroci = incroci;
    vector<int> minPermutation = permutation;
    
    setMediana(centrini);

    while(notImprovementeCount < notImprovementeLimit) // if there are no improvement stop
    {
        sort(permutation.begin(), permutation.end(), [&centrini](int a, int b) {
            return centrini[a].mediana < centrini[b].mediana;
        });

        int currentIncroci = calculateCross(permutation, centrini);

        if (currentIncroci < minIncroci) {
            minIncroci = currentIncroci;
            minPermutation = permutation;
            notImprovementeCount = 0; 
        } else {
            notImprovementeCount++;
        }
    }
    permutation = minPermutation;
    return minIncroci;	
}

/**
 * Read input from file
 * @param path: path to the input file
 * @param centrini: vector of centrini
 * 
 * @note read the input file, insert in centrini and gomitoli
*/
void readInput(const char path[], vector<Centrino>& centrini)
{
    fstream in(path, fstream::in);
    in >> C >> G >> M;
    centrini.resize(C);
    
    for (int i = 0; i < M; ++i) {
        int c, g;
        in >> c >> g;
        auto& gomitoli = centrini[c].gomitoli;
        auto pos = lower_bound(gomitoli.begin(), gomitoli.end(), g); // insert the gomitolo in the right position
        gomitoli.insert(pos, g);
    }
    in.close();
}

/**
 * Write output to file
 * @param incroci: number of cross
 * @param permutation: vector of permutation
 * 
 * @note write the output file
*/
void writeOutput(int incroci, const vector<int>& permutation)
{
    std::fstream out("output.txt", std::fstream::out);
    if (!out) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    out << incroci << std::endl;
    for (int i = 0; i < permutation.size(); i++) {
        out << permutation[i] << " ";
    }
    out << std::endl << "***";
    out.close();
}

int main()
{
    vector<Centrino> centrini; // vector of centrini 
    vector<int> start, barycenter, mediana; //permutation of centrini
    int startCross, barycenterCross, medianaCross; //number of cross

    char path[] = "input.txt"; // path to the input file
    
    readInput(path, centrini);

    start.resize(centrini.size());
    barycenter.resize(centrini.size());
    mediana.resize(centrini.size());

    // initialize the permutation
    for (size_t i = 0; i < start.size(); ++i) {
        start[i] = i;
    }
    barycenter = start;
    mediana = start;

    startCross = calculateCross(start, centrini);
    barycenterCross = orderByBarycenter(barycenter, centrini, startCross);
    medianaCross = orderByMediana(mediana, centrini, startCross);

    if (startCross < barycenterCross && startCross < medianaCross) {
        writeOutput(startCross, start);
    } else if (barycenterCross < startCross && barycenterCross < medianaCross) {
        writeOutput(barycenterCross, barycenter);
    } else {
        writeOutput(medianaCross, mediana);
    }

    return 0;
}

