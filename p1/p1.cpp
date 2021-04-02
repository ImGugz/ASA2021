/**
 * 
 * ASA 2020/2021 - IST
 * Projeto 1
 * 
 * 95535 - Antonio Coelho
 * 95587 - Gustavo Aguiar
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <queue>
using namespace std;

#define ler_numeros(a, b) scanf("%d %d", &a, &b);

typedef int Vertice;

/* Grafo */

class Grafo {
    int numVertices;
    list<Vertice>* listaAdj;
public:
    // Constructor
    Grafo(int V, int E);
    void adicionarArco(int u, int v);
    void mostrarLeK();
};


Grafo::Grafo(int V, int E) {
    numVertices = V;
    listaAdj = new list<Vertice>[V];
    for (int i = 0; i < E; ++i) {
        Vertice u, v;
        ler_numeros(u, v);
        adicionarArco(u-1, v-1);
    }
}


void Grafo::adicionarArco(int u, int v) {
    listaAdj[u].push_back(v);
}

// Algoritmo de Khan (O (V+E))
void Grafo::mostrarLeK() {
    int K = 0;
    int L = 0;
    vector<Vertice> grauEntrada(numVertices, 0);
    vector<Vertice> ordemTopologica;
    for (Vertice u = 0; u < numVertices; ++u) {
        list<int>::iterator it;
        for (it = listaAdj[u].begin(); it != listaAdj[u].end(); it++) {
            grauEntrada[*it]++;
        }
    }
    queue<int> fila;
    for (Vertice u = 0; u < numVertices; ++u) {
        if (grauEntrada[u] == 0) {
            K++;
            fila.push(u);
        }
    }
    while (!fila.empty()) {
        Vertice u = fila.front();
        fila.pop();
        ordemTopologica.push_back(u);
        list<int>::iterator it;
        for (it = listaAdj[u].begin(); it != listaAdj[u].end(); it++) {
            if (--grauEntrada[*it] == 0) {
                fila.push(*it);
            }
        }
    }

    vector<int> tamanhoAteVertice(numVertices, 0);

    for (Vertice u = 0; u < numVertices; ++u) {
        Vertice v = ordemTopologica[u];
        list<int>::iterator it;
        for (it = listaAdj[v].begin(); it != listaAdj[v].end(); ++it) {
            if (tamanhoAteVertice[*it] <= tamanhoAteVertice[v] + 1) {
                tamanhoAteVertice[*it] = tamanhoAteVertice[v] + 1;
            }
        }
    }
    for (Vertice u = 0; u < numVertices; ++u) {
        if (tamanhoAteVertice[u] > L) {
            L = tamanhoAteVertice[u];
        }
    }
    L++;
    printf("%d %d\n", K, L);
}


int main() {
    int numVertices, numArcos;
    ler_numeros(numVertices, numArcos);
    Grafo grafo(numVertices, numArcos);
    grafo.mostrarLeK();
    return 0;
}