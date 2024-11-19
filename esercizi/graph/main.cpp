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
#include <sstream>

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
        void addnode(string label = "") {
            string newlabel;
            if(label == "")
                newlabel[0] = 97 + V.size();
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void addedge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
        }
    
        bool isHamiltonianUtil(Node* curr_node, vector<bool> &visitati, Node* start_node, int count) {
            if(count == (int)V.size()) {
                for(auto& edge : curr_node->adj) {
                    if(edge->v == start_node)
                        return true;
                }

                return false;
            }

            for(auto& edge : curr_node->adj) {
                Node* next_node = edge->v;
                if(!visitati[next_node->key]) {
                    visitati[next_node->key] = true;
                    if(isHamiltonianUtil(next_node, visitati, start_node, count + 1)) {
                        return true;
                    }
                    visitati[next_node->key] = false;
                }
            }

            return false;
        }
    public:
        vector<Node*> V;

        Grafo() {
            ifstream input("IN.txt");
            if (!input.is_open()) {
                cerr << "Errore: impossibile aprire il file IN.txt" << endl;
                return;
            }

            int numnodi, numarchi;
            char sep;
            input >> numnodi >> sep >> numarchi;

            for (int i = 0; i < numnodi; i++)
                addnode();

            int source, dest, weight;
            while (input >> sep >> source >> sep >> dest >> sep >> weight >> sep) {
                cout << "Aggiungi nodo ... " << source << " " << dest << endl;
                addedge(source - 1, dest - 1, weight);
            }

            input.close();
        }

        bool hasHamiltonianCycle() {
            for(auto& node : V) {
                vector<bool> visitati (V.size(), false);

                visitati[node->key] = true;
                if(isHamiltonianUtil(node, visitati, node, 1))
                    return true;
            }

            return false;
        }
};

int main() {
    Grafo g;

    cout << ((g.hasHamiltonianCycle()) ? "Ha un ciclo hamiltoniano" : "Non ha un ciclo hamiltoniano") << endl;

    return 0;
}