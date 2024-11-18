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

/*
    implementare una classe grafo che consenta di caricare dati di un
    grafo non orientato pesato G contenuti nel file GRP.txt II file testo contiene nel
    primo rigo due interi separati da uno spazio che indicano, rispettivamente, il
    numero di nodi ed il numero di archi.
    I successivi righi contengono ciascuno tre numeri, separati da uno spazio, per
    indicare il nodo sorgente, nodo destinazione ed il peso di ogni arco. Dotare la
    classe di un metodo BFS(s) che scrive nel file OUT.txt, per ogni nodo, la distanza
    dal nodo s ed il proprio predecessore. 

    Dotare la classe dei metodi PRIM() e PRINT_MST() per calcolare e stampare a video
    il minimum spanning tree. 

    Dotare la classe del metodo IS_BINARY(MST) che restituisca TRUE se MST è un
    albero binario e FALSE altrimenti. 
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>

using namespace std;

enum Color {WHITE, GRAY, BLACK};
#define INF __INT_MAX__

class Edge;

class Node {
    public:
        int key;
        string label;
        vector<Edge*> adj;

        int d;
        Node* p;
        Color color;

        int prim_key;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight) {}
};

struct Compare {
    bool operator() (Node* u, Node* v) {
        return u->prim_key > v->prim_key;
    }
};

class Graph {
    private:
        void addnode(string label = "") {
            string newlabel;
            if(label == "") 
                newlabel[0] = 97 + V.size();
            else
                newlabel = label;
            
            V .push_back(new Node(V.size(), newlabel));
        }

        void addedge(int ukey, int vkey, int weight) { // grafo non orientato pesato
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
            V[vkey]->adj.push_back(new Edge(V[vkey], V[ukey], weight));
        }
    
        void writeonfile(Node* s) {
            ofstream output("OUT.txt");
            if(!output) cerr << "file non aperto" << endl;

            for(auto& u : V) {
                if(u != s)
                    output << "Nodo{" << u->key << "}: " << "distanza dal nodo s: " << u->d << ", predecessore " << u->p->key << endl;
            }

            output.close();
        }
    
        void reload() {
            for(auto& u : V) 
                u->p = nullptr;
        }
    
        void printMST(const vector<Edge>& mst) {
            cout << "Archi nell'MST:\n";
            int totalWeight = 0;
            for (const auto& edge : mst) {
                cout << edge.u->prim_key << " -- " << edge.v->prim_key << " [peso: " << edge.weight << "]\n";
                totalWeight += edge.weight;
            }
            cout << "Peso totale dell'MST: " << totalWeight << endl;
        }
    public:
        vector<Node*> V;

        Graph() {
            ifstream input("GRP.txt");

            int num_nodi, num_archi;
            input >> num_nodi >> num_archi;

            for(int i = 0; i < num_nodi; i++)
                addnode();
            
            int source, dest, weight;
            while(input >> source >> dest >> weight)
                addedge(source, dest, weight);

            input.close();
        }

        void BFS(int source) {
            Node* s = V[source];
            for(auto& u : V) {
                u->d = INF;
                u->p = nullptr;
                u->color = WHITE;
            }

            s->d = 0;
            s->color = GRAY;
            queue<Node*> Q;
            Q.push(s);

            while(!Q.empty()) {
                Node* u = Q.front(); Q.pop();
                for(auto& edge : u->adj) {
                    Node* v = edge->v;
                    if(v->color == WHITE) {
                        v->d = u->d + 1;
                        v->p = u;
                        Q.push(v);
                    }
                }

                u->color = BLACK;
            }

            writeonfile(s);
        }

        void prim(int s) {
            Node* r = V[s];
            vector<Edge> mst;
            int n = V.size();
            vector<bool> inMST(n, false);
            priority_queue<Node*, vector<Node*>, Compare> Q;

            r->prim_key = 0;
            for (auto& u : V) {
                u->prim_key = INF;
                u->p = nullptr;
                Q.push(u);
            }
            r->prim_key = 0;

            while (!Q.empty()) {
                Node* u = Q.top(); Q.pop();
                if (inMST[u->key]) continue;

                inMST[u->key] = true;
                if (u->p != nullptr) {
                    mst.push_back(Edge(u->p, u, u->prim_key)); 
                }

                for (auto& edge : u->adj) {
                    Node* v = edge->v;
                    if (!inMST[v->key] and edge->weight < v->prim_key) {
                        v->p = u;
                        v->prim_key = edge->weight;
                        Q.push(v);
                    }
                }
            }
            printMST(mst);
        }

        bool IS_BINARY(vector<Edge>& mst) {
            // Creiamo un contatore per il numero di figli per ogni nodo
            vector<int> childCount(V.size(), 0);

            // Per ogni arco nel MST, incrementiamo il contatore dei figli per ciascun nodo
            for (const auto& edge : mst) {
                childCount[edge.u->key]++;
                childCount[edge.v->key]++;
            }

            // Verifichiamo che ogni nodo abbia al massimo due figli
            for (int i = 0; i < V.size(); i++) {
                if (childCount[i] > 2) {
                    return false;  // Se troviamo un nodo con più di due figli, non è un albero binario
                }
            }

            return true;  // Tutti i nodi hanno al massimo due figli
        }
};

int main() {
    Graph g;
    g.BFS(0);
    g.prim(0);

    return 0;
}