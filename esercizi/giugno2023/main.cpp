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
#include <fstream>
#include <stack>

using namespace std;

#define INF __INT_MAX__
enum Color {WHITE, GRAY, BLACK};

class Edge;

class Node {
    public:
        int key;
        string label;
        vector<Edge*> adj;
        Color color;

        int d;
        int f;
        Node* p;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight) {}
};

class Graph {
    private:
        int t = 0;

        void addnode(string label = "") {
            string newlabel;
            if(label == "")
                newlabel = string(1, 'a' + V.size());
            else
                newlabel = label;
            
            V.push_back(new Node(V.size(), newlabel));
        }

        void addedge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
        }

        bool isHamiltonianUtil(int currentNode, vector<bool>& visited, int startNode, int count) {
            // Se tutti i nodi sono stati visitati
            if (count == V.size()) {
                // Controllo se esiste un arco dall'ultimo nodo al nodo iniziale
                for (auto& edge : V[currentNode]->adj) {
                    if (edge->v->key == startNode) {
                        return true; // Esiste un ciclo hamiltoniano
                    }
                }
                return false;
            }

            // Esplora i nodi adiacenti
            for (auto& edge : V[currentNode]->adj) {
                int nextNode = edge->v->key;
                if (!visited[nextNode]) {
                    visited[nextNode] = true;
                    if (isHamiltonianUtil(nextNode, visited, startNode, count + 1)) {
                        return true;
                    }
                    visited[nextNode] = false; // Backtracking
                }
            }

            return false;
        }

    public:
        vector<Node*> V;
 
        Graph() {
            ifstream input("GRP.txt");
            if (!input) {
                cerr << "Errore nell'apertura del file GRP.txt!" << endl;
                return;
            }

            int num_nodi, num_archi;
            input >> num_nodi >> num_archi;

            for(int i = 0; i < num_nodi; i++) {
                addnode();
            }

            int source, dest, weight;
            while(input >> source >> dest >> weight) {
                addedge(source, dest, weight);
            }

            input.close();
        }

        // Funzione per verificare l'esistenza di un ciclo hamiltoniano
        bool hasHamiltonianCycle() {
            for (auto& node : V) {
                vector<bool> visited(V.size(), false);
                visited[node->key] = true;

                // Verifichiamo partendo dal nodo corrente
                if (isHamiltonianUtil(node->key, visited, node->key, 1)) {
                    return true; // Ciclo hamiltoniano trovato
                }
            }

            return false; // Nessun ciclo hamiltoniano trovato
        }
};

int main() {
    Graph g;
    if (g.hasHamiltonianCycle()) {
        cout << "Il grafo contiene un ciclo hamiltoniano." << endl;
    } else {
        cout << "Il grafo NON contiene un ciclo hamiltoniano." << endl;
    }

    return 0;
}