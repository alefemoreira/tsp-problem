#include "Data.h"
#include <iostream>
#include <vector>

using namespace std;
Data data;

typedef struct Solution {
    vector<int> sequence;
    double cost;
};

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

Solution Construcao() {
    Solution s;
    s.sequence = escolher3NósAleatórios();
    vector<int> CL = nosRestantes();

    while(!CL.empty()) {
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL);
        ordernarEmOrderCrescente(&custoInsercao);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, selecionado);
    }
}

void BuscaLocal(Solution *s) {
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;

    while (!NL.empty()) {
        int n = rand() % NL.size();
        switch (n) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
        }

        if (improved)
            NL = {1, 2, 3, 4, 5};
        else
        NL.erase(NL.begin() + n);
    }
    
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
