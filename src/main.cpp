#include "Data.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
Data data;

typedef struct Solution {
    vector<int> sequence;
    double cost;
};

struct InsertionInfo {
    int noInserido;
    int arestaRemovida;
    double custo;
};

vector<int> slicing(vector<int>& arr, int X, int Y) {
    // Starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;
 
    // To store the sliced vector
    vector<int> result(Y - X + 1);
 
    // Copy vector using copy function()
    copy(start, end, result.begin());
 
    // Return the final sliced vector
    return result;
}

void exibirSolucao(Solution *s) {
    for (int i = 0; i < s->sequence.size(); i++)
        cout << s->sequence[i] << " -> ";
    cout << s->sequence.back() << endl;
}

void calcularValorObj(Solution *s) {
    s->cost = 0;
    for (int i = 0; i < s->sequence.size(); i++)
        s->cost += data.getDistance(i, i+1);
}

vector<InsertionInfo> * calcularCustoInsercao(Solution &s, vector<int> &CL) {
    int solutionSize = data.getDimension();
    vector<InsertionInfo> custoInsercao((solutionSize - 1) * CL.size());
    int l = 0;
    for (int a = 0, b = 1; a < solutionSize - 1; a++, b++) {
        int i = s.sequence[a];
        int j = s.sequence[b];
        for (auto k : CL) {
            custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    }

    return &custoInsercao;
}

void ordernarEmOrderCrescente(vector<InsertionInfo> * custoInsercao) {
    
}

void inserirNaSolucao(Solution *s, InsertionInfo insercao) {
    s->sequence[insercao.arestaRemovida + 2] = s->sequence[insercao.arestaRemovida + 1];
    s->sequence[insercao.arestaRemovida + 1] = insercao.noInserido;
}

vector<int> escolher3NosAleatorios() {
    int no1 = rand() % data.getDimension() + 1;
    int no2 = rand() % data.getDimension() + 1;
    int no3 = rand() % data.getDimension() + 1;

    while (no1 == 1)
        no1 = rand() % data.getDimension() + 1;

    while (no2 == no1)
        no2 = rand() % data.getDimension() + 1;

    while (no3 == no2 || no3 == no1)
        no3 = rand() % data.getDimension() + 1;
    
    vector<int> v =  {1, no1, no2, no3, 1};

    return v;
}

vector<int> nosRestantes(const Solution &s) {
    int size = data.getDimension() - 4;
    vector<int> v (size);

    for (int i = 2; i < size; i++) {
        if (find(s.sequence.begin(), s.sequence.end(), i) != s.sequence.end())
            v.push_back(i);
    }
    return v;
}

Solution Construcao() {
    Solution s;
    s.sequence = escolher3NosAleatorios();
    vector<int> CL = nosRestantes(s);

    while(!CL.empty()) {
        vector<InsertionInfo> * custoInsercao = calcularCustoInsercao(s, CL);
        ordernarEmOrderCrescente(custoInsercao);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao->size()));
        InsertionInfo inserir = custoInsercao->at(selecionado);
        inserirNaSolucao(&s, inserir);
        
        // remove nó inserido de CL
        CL.erase(remove(CL.begin(), CL.end(), inserir.noInserido), CL.end());       
    }
}

void BuscaLocal(Solution *s) {
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;

    while (!NL.empty()) {
        int n = rand() % NL.size();
        switch (n) {
            case 1:
                improved = bestImprovementSwap(s);
                break;
            case 2:
                improved = bestImprovement2Opt(s);
                break;
            case 3:
                improved = bestImprovementOrOpt(s, 1); // Re-insertion
                break;
            case 4:
                improved = bestImprovementOrOpt(s, 2); // or-opt2
                break;
            case 5:
                improved = bestImprovementOrOpt(s, 3); // or-opt3
                break;
        }

        if (improved)
            NL = {1, 2, 3, 4, 5};
        else
            NL.erase(NL.begin() + n);
    }
    
}

Solution Pertubacao(const Solution &best) {
    Solution s = best;
    int size = s.sequence.size();
    int maxLength = (int) ceil(size / 10);

    int startSeg1 = rand() % (size - 2) + 1; // o -2 serve para que o segmento não altere o inicio e o fim da sequencia
    int lengthSeg1 = maxLength < 2 ? 2 : rand() % (maxLength - 2) + 2;

    int startSeg2 = rand() % (size - 2) + 1;

    while (
        (
            startSeg2 >= startSeg1 - maxLength &&
             startSeg2 <= startSeg1 + maxLength
        ) || (
            startSeg2 + maxLength > size - 2
        )
    ) {
        startSeg2 = rand() % (size - 2) + 1;
    }
    

    int lengthSeg2 = maxLength < 2 ? 2 : rand() % (maxLength - 2) + 2;

    vector<int> segment1 = slicing(s.sequence, startSeg1, startSeg1 + lengthSeg1 + 1);
    vector<int> segment2 = slicing(s.sequence, startSeg2, startSeg2 + lengthSeg2 + 1);
    
    vector<int> newSequence {size};

    for (int i = 0; i < size; i++) {
        if (i >= startSeg1 && i <= startSeg1 + lengthSeg1) {
            newSequence[i] = segment1[startSeg1 - i];
        }
        if (i >= startSeg2 && i <= startSeg2 + lengthSeg2) {
            newSequence[i] = segment2[startSeg2 - i];
        }
        newSequence[i] = s.sequence[i];
    }

    double cost = 0.0;
    for (size_t i = 0; i < size - 1; i++) {
        cost += data.getDistance(newSequence[i], newSequence[i+1]);
    }

    Solution newSolution {newSequence, cost};

    return newSolution;
}

Solution ILS(int maxIter, int maxIterIls) {
    Solution bestOfAll;
    bestOfAll.cost = INFINITY;
    for (int i = 0; i < maxIter; i++) {
        Solution s = Construcao();
        Solution best = s;

        int iterIls = 0;

        while (iterIls <= maxIterIls) {
            BuscaLocal(&s);
            if (s.cost < best.cost) {
                best = s;
                iterIls = 0;
            }
            s = Pertubacao(best);
            iterIls++;
        }

        if (best.cost < bestOfAll.cost)
            bestOfAll = best;
    }

    return bestOfAll;
}

int main(int argc, char** argv) {

    data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;

    return 0;
}
