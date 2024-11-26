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
#include <stack>

using namespace std;
#define INF __INT_MAX__

enum Color{WHITE, GRAY, BLACK};
enum Type{UNDEFINED, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};

class Edge;

class Node {
    public:
        int key;
        string label;
        Node* p;
        vector<Edge*> adj;
        Color color;
        int d, f;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;
        Type type;

        Edge(Node* u, Node* v, int w) : u(u), v(v), weight(w), type(UNDEFINED) {}
};

class Grafo {
    private:
        int t;

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
    
        void DFS_visit(Node* u, stack<Node*> &s) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                if(edge->v->color == WHITE) {
                    edge->type = TREE_EDGE;
                    //cout << "impostato arco : " << edge->type << endl;
                    DFS_visit(edge->v, s);
                } else if(edge->v->color == GRAY) {
                    edge->type = BACK_EDGE;
                } else if(edge->v->color == BLACK) {
                    edge->type = (u->d < edge->v->d) ? FORWARD_EDGE : CROSS_EDGE;
                }
            }

            u->f = ++t;
            u->color = BLACK;
            s.push(u);
        }

        string type_arc(int n) {
            //cout << "n " << n << endl;
            switch(n) {
                case 1: return "TREE EDGE"; break;
                case 2: return "BACK EDGE"; break;
                case 3: return "FORWARD EDGE"; break;
                case 4: return "CROSS EDGE"; break;
                default: return "UNDEFINED"; break;
            }
        }

        Grafo traspose() {
            Grafo Gt;
            Gt.V.resize(V.size());

            for (size_t i = 0; i < V.size(); ++i) {
                Gt.V[i] = new Node(V[i]->key, V[i]->label);
            }

            for (auto& u : V) {
                for (auto& edge : u->adj) {
                    Gt.V[edge->v->key]->adj.push_back(new Edge(Gt.V[edge->v->key], Gt.V[u->key], edge->weight));
                }
            }

            // Debug: stampa il grafo trasposto
            cout << "Grafo trasposto:" << endl;
            for (auto& node : Gt.V) {
                cout << "Node " << node->key << ": ";
                for (auto& edge : node->adj) {
                    cout << edge->v->key << " ";
                }
                cout << endl;
            }

            return Gt;
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
                // cout << "Aggiungi nodo ... " << source << " " << dest << endl;
                addedge(source - 1, dest - 1, weight);
            }

            input.close();
        }

        stack<Node*> DFS() {
            stack<Node*> s;
            for(auto& u : V) {
                u->p = nullptr;
                u->color = WHITE;
            }

            t = 0;

            for(auto& u : V)
                if(u->color == WHITE)
                    DFS_visit(u, s);

            return s;
        }

        void DFS_collect(Node* u, vector<Node*>& scc) {
            u->color = GRAY;
            scc.push_back(u);
            cout << "Visiting node " << u->key << " in trasposed graph." << endl; // Debug

            for (auto& edge : u->adj) {
                if (edge->v->color == WHITE) {
                    DFS_collect(edge->v, scc);
                }
            }

            u->color = BLACK;
        }

        void scc() {
            // Prima DFS: Ordina i nodi per tempi di fine decrescenti
            stack<Node*> S = DFS();

            // Crea il grafo trasposto
            Grafo gt = traspose();

            // Resetta i colori dei nodi nel grafo trasposto
            for (auto& u : gt.V) {
                u->color = WHITE;
            }

            // Seconda DFS sul grafo trasposto
            vector<vector<Node*>> SCCs; // Contenitore per le SCC
            while (!S.empty()) {
                Node* u = S.top();
                S.pop();

                if (gt.V[u->key]->color == WHITE) { // Lavora sul grafo trasposto
                    vector<Node*> scc; // Una singola SCC
                    gt.DFS_collect(gt.V[u->key], scc); // Raccogli i nodi della SCC
                    SCCs.push_back(scc); // Aggiungi la SCC al risultato
                }
            }

            // Stampa delle SCC
            cout << "Componenti fortemente connesse trovate:" << endl;
            for (size_t i = 0; i < SCCs.size(); ++i) {
                cout << "SCC #" << i + 1 << ": ";
                for (auto& node : SCCs[i]) {
                    cout << node->key << " ";
                }
                cout << endl;
            }
        }

        void print() {
            stack<Node*> S = DFS();
            for(auto& u : V) {
                cout << "Node " << u->key << endl;
                for(auto& edge : u->adj)
                    cout << "\t->" << edge->v->key << ", tipo arco: " << type_arc(edge->type) << endl;
            }
            int size = S.size();
            cout << "Ordinamento topologico" << endl;
            for(int i = size; i > 0; i--) {
                cout << "- " << S.top()->key << " -\t";
                S.pop();
            }

            cout << endl;

        }
};

int main() {
    Grafo g;
    g.scc();

    return 0;
}