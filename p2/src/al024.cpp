/**
 * 
 * ASA 2020/2021 - IST
 * Projeto 2
 * 
 * 95535 - Antonio Coelho
 * 95587 - Gustavo Aguiar
 * 
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>

using namespace std;

typedef int Vertice;

void readInput(int * a, int * b, int * c) {
    if (c == NULL) {
        int temp = scanf("%d %d", a, b);
        assert(temp == 2);
    } else {
        int temp = scanf("%d %d %d", a, b, c);
        assert(temp == 3);
    }
}

class Grafo {
    int numVertices;
    Vertice ** matrizAdjRes; // representação em matriz de adjacências
    int s, t; // fonte e sumidouro
public:
    Grafo(int V) {
        this->numVertices = V;
        matrizAdjRes = new int*[V];
        s = 0; 
        t = V-1;
        for (int i = 0; i < numVertices; ++i) { // inicialização a 0
            matrizAdjRes[i] = new int[V];
            for (int j = 0; j < numVertices; ++j) {
                matrizAdjRes[i][j] = 0;
            }
        }
    }

    void adicionarCustoProcessos(Vertice u, Vertice v, int custo) {
        // simetria de custo inter-processos
        matrizAdjRes[u][v] = custo;
        matrizAdjRes[v][u] = custo;
    }

    void adicionarCustoProcessadores(Vertice u, int custox, int custoy) {
        // processador X é source e Y sink
        matrizAdjRes[s][u] = custox;
        matrizAdjRes[u][t] = custoy;
    }

    ~Grafo() {
        // libertar espaço na heap
        for (int i = 0; i < numVertices; ++i) {
            delete[] matrizAdjRes[i];
        }
        delete[] matrizAdjRes;
    }
    
    bool BFS(Vertice precedentes[]) {
        bool visitados[numVertices]; // guarda vertices visitados no caminho de aumento
        for (int i = 0; i < numVertices; ++i) {
            visitados[i] = false;
        }
        queue<Vertice> fila;
        fila.push(s);
        visitados[s] = true;
        precedentes[s] = -1;
        while (!fila.empty()) {
            Vertice u = fila.front(); fila.pop();
            for (Vertice v = 0; v < numVertices; ++v) {
                if (visitados[v] == false && matrizAdjRes[u][v] > 0) {
                    if (v == t) {
                        precedentes[v] = u;
                        return true;
                    }
                    fila.push(v);
                    precedentes[v] = u;
                    visitados[v] = true;
                }
            }
        }
        return false;
    }

    int fordFulkerson() {
        Vertice precedentes[numVertices];
        int fluxoMax = 0;
        while (BFS(precedentes)) {
            int fluxoCaminho = numeric_limits<int>::max(); // +inf
            for (Vertice v = t; v != s; v = precedentes[v]) {
                Vertice u = precedentes[v];
                fluxoCaminho = min(fluxoCaminho, matrizAdjRes[u][v]);
            }
            for (Vertice v = t; v != s; v = precedentes[v]) {
                Vertice u = precedentes[v];
                matrizAdjRes[u][v] -= fluxoCaminho;
                matrizAdjRes[v][u] += fluxoCaminho;
            }
            fluxoMax += fluxoCaminho;
        }
        return fluxoMax;
    }
};

int main() {
    int n, k;
    readInput(&n, &k, NULL);
    Grafo g(n+2); // adicionar fonte e sumidouro

    for (int i = 0; i < n; ++i) { // O(n)
        int pix, piy;
        readInput(&pix, &piy, NULL);
        g.adicionarCustoProcessadores(i+1, pix, piy);
    }

    for (int i = 0; i < k; ++i) { // O(k)
        int pi, pj, cij;
        readInput(&pi, &pj, &cij);
        g.adicionarCustoProcessos(pi, pj, cij);
    }

    cout << g.fordFulkerson() << "\n";

}

\begin{center}
\includegraphics[width=\linewidth]{reglinear.png}
\end{center}