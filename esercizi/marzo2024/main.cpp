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
    -   Utilizzando il paradigma della programmazione ad oggetti ed il linguaggio C++ 11,
        progettare ed implementare, come classe, una struttura dati grafo che consenta
        di caricare i dati di un grafo orientato pesato G contenuti nel file GRP.txt. Il file
        testo contiene nel primo rigo due interi separati da uno spazio che indicano,
        rispettivamente, il numero di nodi ed il numero di archi. I successivi righi
        contengono ciascuno tre numeri, separati da uno spazio, per indicare il nodo
        sorgente, il nodo destinazione ed il peso di ogni arco. Dotare la classe di un
        metodo per scrivere nel file OUT.txt l’output dell'ordinamento topologico dei nodi
        del grafo G
    -   Implementare l'algoritmo di Bellman-Ford che, dati in input un nodo sorgente S
        ed un un nodo destinazione D. restituisca il cammino di peso minimo tra S e D.
    -   implementare un algoritmo ricorsivo che stampi a video i nodi di ogni ciclo presente in G (usare una nuova linea per ogni ciclo)
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <algorithm>

using namespace std;
enum Color { WHITE, GRAY, BLACK };
#define INF __INT_MAX__

class Edge;

class Node {
    public:
        int key;
        string label;
        vector<Edge*> adj;
        Node* p;
        Color color;
        int d, f;

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
                newlabel[0] = 97 + V.size();
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void addedge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
        }
    
        void DFSvisit(Node* u, stack<Node*> &s, vector<bool> &recStack) {
            u->color = GRAY;
            recStack[u->key] = true;
            u->d = ++t;

            for(auto& edge : u->adj) {
                Node* v = edge->v;
                if(v->color == WHITE) {
                    v->p = u;
                    DFSvisit(v, s, recStack);
                } else if (recStack[v->key]) {
                    cout << "Ciclo trovato: ";
                    Node* temp = u;
                    while (temp != v) { 
                        cout << temp->key << " ";
                        temp = temp->p;
                    }
                    cout << v->key << " "; 
                    cout << endl;
                }
            }

            u->color = BLACK;
            recStack[u->key] = false;
            u->f = ++t;
            s.push(u);
        }

        stack<Node*> DFS() {
            stack<Node*> s;
            vector<bool> recStack(V.size(), false);

            for(auto& u : V) {
                u->color = WHITE;
                u->p = nullptr;
            }

            for(auto& u : V)
                if(u->color == WHITE)
                    DFSvisit(u, s, recStack); 

            return s;
        }

        void ordtop(ofstream &out) {
            stack<Node*> s = DFS(); int size = s.size();

            for(int i = 0; i < size; i++) {
                out << "Elemento: " << s.top()->key << endl;
                s.pop();
            }
        }
    
        void initialize_single_source(Node* source) {
            for(auto& u : V) {
                u->d = INF;
                u->p = nullptr;
            }

            source->d = 0;
        }

        void relax(Node* u, Node* v, int weight) {
            if(v->d > u->d + weight) {
                v->p = u;
                v->d = u->d + weight;
            }
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            int size = (int)V.size();
            for(int i = 0; i < size; i++)
                for(auto& u : V)
                    for(auto& edge : u->adj)
                        relax(u, edge->v, edge->weight);
            for(auto& u : V)
                for(auto& edge : u->adj)
                    if(edge->v->d > u->d + edge->weight)
                        return false;
            return true;
        }
    public:
        vector<Node*> V;

        Graph() {
            ifstream IN("GRP.txt");
            if (!IN) {
                cerr << "Errore: impossibile aprire il file GRP.txt" << endl;
                return;
            }

            int numnodi, numarchi; char sep;
            IN >> numnodi >> sep >> numarchi;

            //cout << numnodi << " " << numarchi << endl;

            for(int i = 0; i < numnodi; i++)
                addnode();

            int source, dest, weight;
            while(IN >> sep >> source >> sep >> dest >> sep >> weight >> sep)
                addedge(source - 1, dest - 1, weight);

            IN.close();

            ofstream OUT("OUT.txt");
            if (!OUT) {
                cerr << "Errore: impossibile aprire il file GRP.txt" << endl;
                return;
            }

            ordtop(OUT);

            OUT.close();
        }

        vector<int> get_path(int source, int dest) {
            if(bellmanford(source)) {
                vector<int> path;
                Node* curr = V[dest];

                while(curr != nullptr) {
                    path.push_back(curr->key);
                    if(curr == V[source]) break;
                    curr = curr->p;
                }
                
                return path;
            } else {
                cerr << "Rilevato ciclo negativo" << endl;
                return {};
            }
        }

        void print(vector<int> path, int s, int d) {
            cout << "percorso minimo da " << s << " a " << d << endl;
            for(auto& el : path)
                cout << el << "\t";
            cout << endl;
        }
};

int main() {
    Graph g;

    vector<int> path = g.get_path(0, 3);
    if(path.empty()) cerr << "Nessun cammino minimo rilevato" << endl;
    else g.print(path, 0, 3);

    return 0;
}