#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>

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

// vector<pair<int,int>> heuristicPairSelection ( const )

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
    setBarycenter(centrini);

    sort(permutation.begin(), permutation.end(), [&centrini](int a, int b) {
        return centrini[a].barycenter < centrini[b].barycenter;
    });

    return calculateCross(permutation, centrini);;
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
    setMediana(centrini);
    sort(permutation.begin(), permutation.end(), [&centrini](int a, int b) {
        return centrini[a].mediana < centrini[b].mediana;
    });

    return calculateCross(permutation, centrini);
}

/**
 * Heuristic Pair Selection
 * @param permutation: vector of permutation
 * @param centrini: vector of centrini
 * @return the pair of nodes
 * 
 * @note select the pair of nodes with the highest number of gomitoli
*/
vector<pair<int, int>> heuristicPairSelection(const vector<int>& permutation, const vector<Centrino>& centrini)
{
    vector<pair<int, int>> sortedPermutation;
    for (int i = 0; i < permutation.size(); ++i) {
        sortedPermutation.emplace_back(i, permutation[i]);
    }
    sort(sortedPermutation.begin(), sortedPermutation.end(), [&centrini](const pair<int, int>& a, const pair<int, int>& b) {
        return centrini[a.second].gomitoli.size() > centrini[b.second].gomitoli.size();
    });

    size_t tenPercentSize = sortedPermutation.size();
    if (tenPercentSize >= 1300 && tenPercentSize < 3000){
        tenPercentSize = sortedPermutation.size() / 1.6;
    }
    
    vector<pair<int, int>> candidatePairs;

    for (size_t i = 0; i < tenPercentSize; ++i) {
        for (size_t j = i + 1; j < tenPercentSize; ++j) {
            if (abs(sortedPermutation[i].first - sortedPermutation[j].first) <= 2) {
                candidatePairs.emplace_back(sortedPermutation[i].first, sortedPermutation[j].first);
            }
        }
    }
    return candidatePairs;
}

/**
 * Pairwise Node Swapping with Heuristics
 * @param permutation: vector of permutation
 * @param centrini: vector of centrini
 * @param currentCrossings: number of cross to compare
 * @return the number of cross
 * 
 * @note swap the pair of nodes and calculate the number of cross
*/
int pairwiseNodeSwappingWithHeuristics(vector<int>& permutation, const vector<Centrino>& centrini, int currentCrossings)
{
    vector<pair<int, int>> candidatePairs = heuristicPairSelection(permutation, centrini);
    for (const auto& pair : candidatePairs) {
        int u = pair.first, v = pair.second;
        iter_swap(permutation.begin() + u, permutation.begin() + v);

        int newCrossings = calculateCross(permutation, centrini);
        if (newCrossings < currentCrossings) {
            currentCrossings = newCrossings;
        } else {
            iter_swap(permutation.begin() + u, permutation.begin() + v);
        }
    }
    return currentCrossings;
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
    
    // helpers::setup(); //delete: setup
    vector<Centrino> centrini; // vector of centrini 
    vector<int> start, barycenter, mediana, heuristicBaricenter; //permutation of centrini
    int startCross= INT_MAX, barycenterCross = INT_MAX, medianaCross = INT_MAX, heuristicBaricenterCross = INT_MAX; //number of cross

    char path[] = "input.txt"; // path to the input file
    // char path[] = "input/input19.txt";
    
    readInput(path, centrini);

    start.resize(centrini.size());
    barycenter.resize(centrini.size());
    mediana.resize(centrini.size());
    heuristicBaricenter.resize(centrini.size());

    // initialize the permutation
    for (size_t i = 0; i < start.size(); ++i) {
        start[i] = i;
    }
    barycenter = start;
    mediana = start;
    heuristicBaricenter = start;

    startCross = calculateCross(start, centrini);
    barycenterCross = orderByBarycenter(barycenter, centrini, startCross);
    medianaCross = orderByMediana(mediana, centrini, startCross);
    heuristicBaricenter = barycenter;
    heuristicBaricenterCross = pairwiseNodeSwappingWithHeuristics(heuristicBaricenter, centrini, barycenterCross);

    if (startCross <= barycenterCross && startCross <= medianaCross && startCross <= heuristicBaricenterCross) {
        writeOutput(startCross, start);
    } else if (barycenterCross <= startCross && barycenterCross <= medianaCross && barycenterCross <= heuristicBaricenterCross) {
        writeOutput(barycenterCross, barycenter);
    } else if (medianaCross <= startCross && medianaCross <= barycenterCross && medianaCross <= heuristicBaricenterCross) {
        writeOutput(medianaCross, mediana);
    } else {
        writeOutput(heuristicBaricenterCross, heuristicBaricenter);
    }

    //delete: print time
    // cout << "Time: " << helpers::get_elapsed_time() / 1000000.0 << " seconds" << endl;
    // cout << "Timeout: " << helpers::has_reached_timeout() << endl;

    return 0;
}
