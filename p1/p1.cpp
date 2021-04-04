/**
 * 
 * ASA 2020/2021 - IST
 * Projeto 1
 * 
 * 95535 - Antonio Coelho
 * 95587 - Gustavo Aguiar
 * 
 */

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

/* Notacao para um vertice em vez de usar o tipo primitivo int */
typedef int Vertice;

/* Funcao para ler o input */
void readInput(int * a, int * b) {
    int temp = scanf("%d %d", a, b);
    assert(temp == 2);
}

/* Classe do Grafo */
class Graph {
    int numVertices;
    vector<Vertice> * adjList;
    int * inDegree;

public:
    Graph(int V, int E);
    void addEdge(int u, int v); // Adiciona arco
    vector<Vertice> countInDegree();
    vector<Vertice> topologicalSort(); // Devolve um vetor de vertices com ordençao topologica
    void longestPathDAG();
    void freeMemHeap();
};

/* Construtor da classe Grafo lendo do input os varios arcos */
Graph::Graph(int V, int E) {
    numVertices = V;
    adjList = new vector<Vertice>[V];
    inDegree = new int[V]();
    for (int i = 0; i < E; ++i) {
        Vertice u, v;
        readInput(&u, &v);
        addEdge(u - 1, v - 1); // Para representacao fiel (n vertices -> [0,..,n-1])
    }
}

/* Adiciona um arco ao grafo, de u para v */
void Graph::addEdge(int u, int v) {
    adjList[u].push_back(v);
    inDegree[v]++;
}

/* Algoritmo de Kahn para ordenacao topologica */
/* O(V + E) */
vector<Vertice> Graph::topologicalSort() {
    int K = 0; // Variavel de output
    vector<Vertice> topSort;
    queue<int> queueZeroDegree;
    for (Vertice u = 0; u < numVertices; ++u) {
        if (inDegree[u] == 0) {
            K++;
            queueZeroDegree.push(u);
        }
    }
    /* Desbastar o grafo, removendo arestas de sources e juntando ah fila as novas sources */
    /* NOTA: Num DAG ha sempre uma source (e um sink) */
    while (!queueZeroDegree.empty()) {
        Vertice u = queueZeroDegree.front();
        queueZeroDegree.pop();
        topSort.push_back(u);
        for (size_t i = 0; i < adjList[u].size(); ++i) {
            Vertice adj = adjList[u][i];
            if (--inDegree[adj] == 0) {
                queueZeroDegree.push(adj);
            }
        }
    }
    printf("%d ", K); // Imprimir output do K
    return topSort;
}

/* Mumit Khan, CSE 221, April 10, 2011 */
/* Algoritmo para encontrar o caminho mais longo dado um DAG de input */
/* NOTA: Neste caso assume-se um peso de 1 para um DAG sem peso */
/* O(V + E) */
void Graph::longestPathDAG() {
    int L = 0; // Variavel de output
    vector<int> lengthTo(numVertices, 0);
    vector<Vertice> topSort = topologicalSort();
    for (Vertice u = 0; u < numVertices; ++u) {
        Vertice v = topSort[u];
        for (size_t i = 0; i < adjList[v].size(); ++i) {
            Vertice w = adjList[v][i];
            if (lengthTo[w] <= lengthTo[v] + 1) {
                lengthTo[w] = lengthTo[v] + 1;
            }
            if (lengthTo[w] > L) {
                L = lengthTo[w];
            }
        }
    }
    /* Seja p = <v1, v2, ..., vn> um caminho qualquer num grafo
    Existem n vertices e n-1 arcos, onde o vector distanceTo armazena
    apenas a distancia em numero de arcos pelo que eh necessario incrementar */
    L++; // lengthTo[u] guarda o numero de arcos do path (numero de vertices no path - 1)
    printf("%d\n", L); // Imprimir output do L
}

/* Libertar memoria alocada no heap */
void Graph::freeMemHeap() {
    delete[] adjList;
    delete[] inDegree;
}

int main() {
    int numVertices, numArcos;
    readInput(&numVertices, &numArcos);
    Graph graph(numVertices, numArcos);
    graph.longestPathDAG();
    /*Alteracoes printf a parte, em vez de estar repartido por funcoes*/
    graph.freeMemHeap();
    return 0;
}