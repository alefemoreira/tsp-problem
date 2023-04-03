#include "Data.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;
Data *data;

typedef struct Solution {
  vector<int> *sequence;
  double cost;
} Solution;

struct InsertionInfo {
  int noInserido;
  int arestaRemovida;
  double custo;
};

vector<int> slicing(vector<int> &arr, int X, int Y) {
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
  cout << "Solução: \n";
  for (int i = 0; i < s->sequence->size() - 1; i++)
    cout << (*s->sequence)[i] << " -> ";
  cout << s->sequence->back() << endl;
}

void calcularValorObj(Solution *s) {
  s->cost = 0;
  for (int i = 0; i < s->sequence->size(); i++)
    s->cost += data->getDistance(i, i + 1);
}

vector<InsertionInfo> *calcularCustoInsercao(Solution *s, vector<int> *CL) {
  vector<InsertionInfo> *custoInsercao =
      new vector<InsertionInfo>((s->sequence->size() - 1) * CL->size());

  int l = 0;
  for (int a = 0, b = 1; a < s->sequence->size() - 1; a++, b++) {
    int i = (*s->sequence)[a];
    int j = (*s->sequence)[b];
    for (auto k : *(CL)) {
      custoInsercao->at(l).custo = data->getDistance(i, k) +
                                   data->getDistance(k, j) -
                                   data->getDistance(i, j);
      custoInsercao->at(l).noInserido = k;
      custoInsercao->at(l).arestaRemovida = a;
      l++;
    }
  }

  return custoInsercao;
}

void inserirNaSolucao(Solution *s, InsertionInfo insercao) {
  auto it = s->sequence->begin() + insercao.arestaRemovida + 1;
  s->sequence->insert(it, insercao.noInserido);
  s->cost += insercao.custo;
}

vector<int> *escolher3NosAleatorios() {
  int no1 = rand() % (data->getDimension() - 1) + 2;
  int no2 = rand() % (data->getDimension() - 1) + 2;
  int no3 = rand() % (data->getDimension() - 1) + 2;

  while (no1 == 1)
    no1 = rand() % data->getDimension() + 1;

  while (no2 == no1)
    no2 = rand() % data->getDimension() + 1;

  while (no3 == no2 || no3 == no1)
    no3 = rand() % data->getDimension() + 1;

  vector<int> *v = new vector<int>({1, no1, no2, no3, 1});

  return v;
}

bool findValueInSolution(Solution *s, int value) {
  for (int i = 0; i < s->sequence->size(); i++)
    if ((*s->sequence)[i] == value)
      return true;

  return false;
}

vector<int> nosRestantes(Solution *s) {
  int size = data->getDimension();
  vector<int> v;

  for (int i = 2; i <= size; i++) {
    if (!findValueInSolution(s, i)) {
      v.push_back(i);
    }
  }

  return v;
}

Solution Construcao() {
  Solution s;
  s.sequence = escolher3NosAleatorios();
  vector<int> CL = nosRestantes(&s);

  while (!CL.empty()) {
    vector<InsertionInfo> *custoInsercao = calcularCustoInsercao(&s, &CL);

    sort(custoInsercao->begin(), custoInsercao->end(),
         [](InsertionInfo &x, InsertionInfo &y) { return x.custo < y.custo; });

    double alpha = (double)rand() / RAND_MAX;
    int selecionado = rand() % ((int)ceil(alpha * custoInsercao->size()));
    InsertionInfo inserir = custoInsercao->at(selecionado);
    inserirNaSolucao(&s, inserir);

    // remove nó inserido de CL
    CL.erase(remove(CL.begin(), CL.end(), inserir.noInserido), CL.end());
  }

  return s;
}

bool bestImprovementSwap(Solution *s) {
  double bestDelta = 0;
  int best_i, best_j;

  for (int i = 1; i < s->sequence->size() - 1; i++) {
    int vi = (*s->sequence)[i];
    int vi_next = (*s->sequence)[i + 1];
    int vi_prev = (*s->sequence)[i - 1];

    for (int j = i + 1; j < s->sequence->size() - 1; j++) {
      int vj = (*s->sequence)[j];
      int vj_next = (*s->sequence)[j + 1];
      int vj_prev = (*s->sequence)[j - 1];
      double delta =
          data->getDistance(vi, vj_next) + data->getDistance(vi, vj_prev) +
          data->getDistance(vj, vi_prev) + data->getDistance(vj, vi_next) -
          data->getDistance(vi, vi_prev) - data->getDistance(vi, vi_next) -
          data->getDistance(vj, vj_next) - data->getDistance(vj, vj_prev);

      // double delta =
      //     -data->getDistance(vi_prev, vi) - data->getDistance(vi,
      //     vi_next) + data->getDistance(vi_prev, vj) +
      //     data->getDistance(vj, vi_next) - data->getDistance(vj_prev, vj)
      //     - data->getDistance(vj, vj_next) + data->getDistance(vj_prev,
      //     vi) + data->getDistance(vi, vj_next);

      if (delta < bestDelta) {
        bestDelta = delta, best_i = i;
        best_j = j;
      }
    }
  }

  if (bestDelta < 0) {
    swap((*s->sequence)[best_i], (*s->sequence)[best_j]);
    s->cost = s->cost + bestDelta;
    return true;
  }
  return false;
}

void TwoOptSwap(Solution **s, int i, int j) {
  int size = (*s)->sequence->size();
  vector<int> *sequence = new vector<int>;
  for (int c = 0; c < i; ++c) {
    int value = (*(*s)->sequence)[c];
    sequence->push_back(value);
  }
  for (int c = j; c >= i; --c) {
    int value = (*(*s)->sequence)[c];
    sequence->push_back(value);
  }
  for (int c = j + 1; c < size; ++c) {
    int value = (*(*s)->sequence)[c];
    sequence->push_back(value);
  }
  vector<int> *old_sequence = (*s)->sequence;

  (*s)->sequence = sequence;
  delete old_sequence;
}

bool bestImprovement2Opt(Solution *s) {
  double bestDelta = 0;
  int best_i, best_j;

  for (int i = 1; i < s->sequence->size() - 1; i++) {
    int vi = (*s->sequence)[i];
    int vi_next = (*s->sequence)[i + 1];
    for (int j = i + 1; j < s->sequence->size() - 1; j++) {
      int vj = (*s->sequence)[j];
      int vj_prev = (*s->sequence)[i - 1];

      double delta =
          data->getDistance(vi, vj_prev) + data->getDistance(vj, vi_next) -
          data->getDistance(vi, vi_next) - data->getDistance(vj_prev, vj);
      if (delta < bestDelta) {
        best_i = i;
        best_j = j;
        bestDelta = delta;
      }
    }
  }

  if (bestDelta < 0) {
    TwoOptSwap(&s, best_i, best_j);
    s->cost += bestDelta;
    return true;
  }

  return false;
}

void OrOpt(Solution **s, int i, int j, int length) {
  vector<int> *new_sequence = new vector<int>();

  for (int c = 0; c < (*s)->sequence->size(); c++) {
    if (c >= i && c < i + length)
      continue;

    new_sequence->push_back((*(*s)->sequence)[c]);

    if (c == j) {
      for (int d = 0; d < length; d++) {
        new_sequence->push_back((*(*s)->sequence)[i + d]);
      }
    }
  }
  auto old_sequence = (*s)->sequence;
  delete old_sequence;
  (*s)->sequence = new_sequence;
}

bool bestImprovementOrOpt(Solution *s, int length) {
  double bestDelta = 0;
  int best_i, best_j;
  for (int i = 1; i < s->sequence->size() - length; i++) {
    int vi_length = (*s->sequence)[i + length - 1];
    int vi = (*s->sequence)[i];
    int vi_length_next = (*s->sequence)[i + length];
    int vi_prev = (*s->sequence)[i - 1];

    for (int j = i + length; j < s->sequence->size() - 1; j++) {
      if (j == i) {
        j += length;
      }

      if (j >= s->sequence->size() - 1) {
        break;
      }

      int vj = (*s->sequence)[j];
      int vj_next = (*s->sequence)[j + 1];
      // exibirSolucao(s);
      double delta = data->getDistance(vi_prev, vi_length_next) +
                     data->getDistance(vj, vi) +
                     data->getDistance(vi_length, vj_next) -
                     data->getDistance(vi_prev, vi) -
                     data->getDistance(vi_length, vi_length_next);

      if (delta < bestDelta) {
        best_i = i;
        best_j = j;
        bestDelta = delta;
      }
    }
  }

  if (bestDelta < 0) {
    OrOpt(&s, best_i, best_j, length);
    s->cost += bestDelta;
    return true;
  }

  return false;
}

void BuscaLocal(Solution *s) {
  vector<int> NL{0, 1, 2, 3, 4};
  bool improved = false;
  int counter = 0;

  while (!NL.empty()) {
    counter++;
    int n = rand() % NL.size();
    if (s->cost < 2579) {
      cout << "\n----\n";
      exibirSolucao(s);
      cout << "\nCusto: " << s->cost << endl;
      cout << "Opção: " << n << endl;
      cout << "Dimen: " << s->sequence->size() << endl;
      cout << "\n----\n";
    }
    switch (NL[n]) {
    case 0:
      improved = bestImprovementSwap(s);
      break;
    case 1:
      improved = bestImprovement2Opt(s);
      break;
    case 2:
      improved = bestImprovementOrOpt(s, 1); // Re-insertion
      break;
    case 3:
      improved = bestImprovementOrOpt(s, 2); // or-opt2
      break;
    case 4:
      improved = bestImprovementOrOpt(s, 3); // or-opt3
      break;
    }

    if (improved) {
      NL = {0, 1, 2, 3, 4};
    } else {
      NL.erase(NL.begin() + n);
      // cout << "N: " << n << "\n";
      // for (auto it = NL.begin(); it != NL.end(); it++) {
      //   cout << *it << ", ";
      // }
      // cout << "\n";
    }
    if (counter > 20)
      break;
  }
}

Solution Pertubacao(const Solution &best) {
  Solution s = best;
  int size = s.sequence->size();
  int maxLength = (int)ceil(size / 10);

  int startSeg1 =
      rand() % (size - 2) + 1; // o -2 serve para que o segmento não altere o
                               // inicio e o fim da sequencia
  int lengthSeg1 = maxLength < 2 ? 2 : rand() % (maxLength - 2) + 2;

  int startSeg2 = rand() % (size - 2) + 1;

  while ((startSeg2 >= startSeg1 - maxLength &&
          startSeg2 <= startSeg1 + maxLength) ||
         (startSeg2 + maxLength > size - 2)) {
    startSeg2 = rand() % (size - 2) + 1;
  }

  int lengthSeg2 = maxLength < 2 ? 2 : rand() % (maxLength - 2) + 2;

  vector<int> segment1 =
      slicing(*s.sequence, startSeg1, startSeg1 + lengthSeg1 + 1);
  vector<int> segment2 =
      slicing(*s.sequence, startSeg2, startSeg2 + lengthSeg2 + 1);

  vector<int> *newSequence = new vector<int>(size);

  for (int i = 0; i < size; i++) {
    if (i >= startSeg1 && i <= startSeg1 + lengthSeg1) {
      (*newSequence)[i] = segment1[startSeg1 - i];
    }
    if (i >= startSeg2 && i <= startSeg2 + lengthSeg2) {
      (*newSequence)[i] = segment2[startSeg2 - i];
    }
    (*newSequence)[i] = (*s.sequence)[i];
  }

  double cost = 0.0;
  for (size_t i = 0; i < size - 1; i++) {
    cost += data->getDistance((*newSequence)[i], (*newSequence)[i + 1]);
  }

  Solution newSolution{newSequence, cost};

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

int main(int argc, char **argv) {
  // srand((unsigned)time(0));

  data = new Data(argc, argv[1]);
  data->read();
  size_t n = data->getDimension();

  cout << "Dimension: " << n << endl;
  int maxIterIls = (int)round(n / 2);
  printf("\n iterIls: ");
  Solution s = ILS(50, maxIterIls);
  exibirSolucao(&s);
  cout << "Custo de S: " << s.cost << endl;

  return 0;
}
