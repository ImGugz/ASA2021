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

void lerInput(int * a, int * b, int * c) {
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
    vector<Vertice> * listaAdj; // lista de adjacências da rede de fluxo
    int ** matrizCapRes; // capacidades de arcos na rede residual
    int s, t; // fonte e sumidouro
public:
    Grafo(int V) {
        numVertices = V;
        listaAdj = new vector<Vertice>[V];
        matrizCapRes = new int*[V];
        s = 0; 
        t = V-1;
        for (int i = 0; i < numVertices; ++i) { // inicialização a 0
            matrizCapRes[i] = new int[V];
            for (int j = 0; j < numVertices; ++j) {
                matrizCapRes[i][j] = 0;
            }
        }
    }

    void adicionarCustoProcessadores(Vertice u, int custox, int custoy) {
        listaAdj[s].push_back(u);
        listaAdj[u].push_back(t);
        // processador X é source e Y sink
        matrizCapRes[s][u] = custox;
        matrizCapRes[u][t] = custoy;
    }

    void adicionarCustoProcessos(Vertice u, Vertice v, int custo) {
        // arcos bidirecionais
        listaAdj[u].push_back(v);
        listaAdj[v].push_back(u);
        // simetria de custo inter-processos
        matrizCapRes[u][v] = custo;
        matrizCapRes[v][u] = custo;
    }

    ~Grafo() {
        // libertar espaço na heap
        for (int i = 0; i < numVertices; ++i) {
            delete[] matrizCapRes[i];
        }
        delete[] matrizCapRes;
        delete[] listaAdj;
    }
    
    bool BFS(Vertice precedentes[]) { // O(E) pois E >> V nestas redes
        bool visitados[numVertices]; // guarda vertices visitados no caminho de aumento
        for (int i = 0; i < numVertices; ++i) {
            visitados[i] = false;
        }
        queue<Vertice> fila;
        fila.push(s);
        visitados[s] = true;
        precedentes[s] = -1; // s é a source de tudo
        while (!fila.empty()) {
            Vertice u = fila.front(); fila.pop();
            for (size_t i = 0; i < listaAdj[u].size(); ++i) {
                Vertice v = listaAdj[u][i];
                if (visitados[v] == false && matrizCapRes[u][v] > 0) {
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

    int edmondsKarp() {
        Vertice precedentes[numVertices];
        int fluxoMax = 0;
        while (BFS(precedentes)) {
            int fluxoCaminho = numeric_limits<int>::max(); // +inf
            for (Vertice v = t; v != s; v = precedentes[v]) {
                Vertice u = precedentes[v];
                fluxoCaminho = min(fluxoCaminho, matrizCapRes[u][v]);
            }
            for (Vertice v = t; v != s; v = precedentes[v]) {
                Vertice u = precedentes[v];
                matrizCapRes[u][v] -= fluxoCaminho;
                matrizCapRes[v][u] += fluxoCaminho;
            }
            fluxoMax += fluxoCaminho;
        }
        return fluxoMax;
    }
};

int main() {
    int n, k;
    readInput(&n, &k, NULL);
    Grafo g(n+2); // n processos + 2 processadores

    for (int i = 0; i < n; ++i) { // O(n) = O(V)
        int pix, piy;
        readInput(&pix, &piy, NULL);
        g.adicionarCustoProcessadores(i+1, pix, piy);
    }

    for (int i = 0; i < k; ++i) { // O(k) = O(V^2)
        int pi, pj, cij;
        readInput(&pi, &pj, &cij);
        g.adicionarCustoProcessos(pi, pj, cij);
    }

    cout << g.edmondsKarp() << "\n";

}