/*
 * --------------------------------------------------------------------------------
 * Codice sorgente di proprietà riservata.
 * Tutti i diritti riservati © Checca.
 *
 * Questo codice è confidenziale e destinato esclusivamente all'uso autorizzato.
 * È vietata la riproduzione, la distribuzione o la divulgazione di qualsiasi
 * parte di questo codice senza il consenso esplicito di Checca.
 *
 * Autore: Checca
 * --------------------------------------------------------------------------------
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Node {
    public:
        int key;
        string label;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight) {}
};

// Funzione di confronto per ordinare gli archi per peso
bool compareEdges(Edge* e1, Edge* e2) {
    return e1->weight < e2->weight;
}

// Classe Union-Find per gestione degli insiemi disgiunti
class UnionFind {
    public:
        vector<int> parent;
        vector<int> rank;

        UnionFind(int n) {
            parent.resize(n);
            rank.resize(n, 0);
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }

        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }

        void unionSets(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX != rootY) {
                if (rank[rootX] > rank[rootY]) {
                    parent[rootY] = rootX;
                } else if (rank[rootX] < rank[rootY]) {
                    parent[rootX] = rootY;
                } else {
                    parent[rootY] = rootX;
                    rank[rootX]++;
                }
            }
        }
};

class Graph {
    public:
        vector<Node*> V;       // Lista di nodi
        vector<Edge*> E;       // Lista di archi

        // Aggiungi un nodo al grafo
        void addNode(int key, const string& label) {
            V.push_back(new Node(key, label));
        }

        // Aggiungi un arco al grafo
        void addEdge(int keyU, int keyV, int weight) {
            Node* u = V[keyU];
            Node* v = V[keyV];
            E.push_back(new Edge(u, v, weight));
        }

        // Algoritmo di Kruskal per trovare il Minimum Spanning Tree
        vector<Edge*> kruskalMST() {
            // Ordinare gli archi per peso
            sort(E.begin(), E.end(), compareEdges);

            // Inizializzare Union-Find con numero di nodi
            UnionFind uf(V.size());

            // Contenitore per il MST
            vector<Edge*> mst;

            // Iterare sugli archi ordinati
            for (Edge* edge : E) {
                int u = edge->u->key;
                int v = edge->v->key;

                // Se i nodi u e v non appartengono allo stesso insieme, aggiungi l'arco
                if (uf.find(u) != uf.find(v)) {
                    mst.push_back(edge);
                    uf.unionSets(u, v);
                }
            }

            return mst;
        }
};

int main() {
    Graph graph;

    // Aggiungere nodi al grafo
    graph.addNode(0, "A");
    graph.addNode(1, "B");
    graph.addNode(2, "C");
    graph.addNode(3, "D");

    // Aggiungere archi (u, v, peso)
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 3, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);

    // Eseguire l'algoritmo di Kruskal
    vector<Edge*> mst = graph.kruskalMST();

    // Stampare il Minimum Spanning Tree
    cout << "Minimum Spanning Tree:\n";
    for (Edge* edge : mst) {
        cout << edge->u->label << " - " << edge->v->label << " : " << edge->weight << "\n";
    }

    // Libera la memoria
    for (Node* node : graph.V) {
        delete node;
    }
    for (Edge* edge : graph.E) {
        delete edge;
    }

    return 0;
}
