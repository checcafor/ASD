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

using namespace std;

class Edge;

class Node {
    public:
        int key;
        string label;
        Node* p;
        vector<Edge*> adj;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int w) : u(u), v(v), weight(w) {}
};

class Grafo {
    private:
        void add_node(string label = "") {
            string newlabel = "";
            if(label == "") newlabel[0] = 97 + V.size();
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int u, int v, int weight) {
            V[u]->adj.push_back(new Edge(V[u], V[v], weight));
        }
    
        bool isHamiltonianUtil(Node* curr, vector<bool> &visited, Node* start_node, int count) {
            if(count == V.size()) {
                for(auto& edge : curr->adj) {
                    if(edge->v == start_node) return true;
                }
                return false;
            }

            for(auto& edge : curr->adj) {
                if(!visited[edge->v->key]) {
                    visited[edge->v->key] = true;
                    if(isHamiltonianUtil(edge->v, visited, start_node, count + 1)) return true;
                    visited[edge->v->key] = false;
                }
            }

            return false;
        }
    public:
        vector<Node*> V;

        Grafo() {
            ifstream input("IN.txt");
            if(!input) return;

            int nodi, archi; char sep;
            input >> nodi >> sep >> archi;
            for(int i = 0; i < nodi; i++)
                add_node();

            int source, dest, weight;
            while(input >> sep >> source >> sep >> dest >> sep >> weight >> sep) {
                add_edge(source - 1, dest - 1, weight);
            }

            input.close();
        }

        bool has_hamiltoniancycle() {
            for(auto& u : V) {
                vector<bool> visitati(V.size(), false);
                if(isHamiltonianUtil(u, visitati, u, 1)) return true;
            }

            return false;
        }
};

int main() {
    Grafo g;
    if(g.has_hamiltoniancycle()) cout << "La funzione ha rilevato un ciclo hamiltoniano" << endl;
    else cout << "La funzione non ha rilevato un ciclo hamiltoniano" << endl;

    return 0;
}