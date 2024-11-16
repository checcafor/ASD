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
        int d;
        vector<Edge*> adj;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight = 0) : u(u), v(v), weight(weight) {}
};

class Graph {
    private:
        void addnode(string label = "") {
            string newlabel;
            if(label == "")
                newlabel[0] = 97 + V.size();
            else
                newlabel = label;
            
            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int u, int v, int w) {
            V[u]->adj.push_back(new Edge(V[u], V[v], w));
        }
    public:
        vector<Node*> V;

        Graph() {
            ifstream input("IN.txt");

            int num_nodi, num_archi, se;
            input >> num_nodi >> se >> num_archi;
            for(int i = 0; i < num_nodi; i++) addnode();

            string line;
            while (getline(input, line)) {

                line = line.substr(1, line.size() - 2);

                stringstream ss(line);
                int source, dest, weight;
                char sep;
                ss >> source >> sep >> dest >> sep >> weight;

                add_edge(source, dest, weight);
            }

            input.close();
        }

        void print() {
            for(auto& u : V) {
                cout << " -> [" << u->key << "] -> ";
                for(auto& edge : u->adj)
                    cout << " - {" << edge->v->key << "} ";
                cout << endl;
            }
        }
};

int main() {
    Graph g;
    g.print();

    return 0;
}