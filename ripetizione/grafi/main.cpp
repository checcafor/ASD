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
#include <queue>
#include <stack>

using namespace std;

#define INF __INT_MAX__
enum Color {WHITE, GRAY, BLACK};

class Edge;

class Node {
    public:
        int key, key_prim;
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

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight) {}
};

struct Compare {
    bool operator() (Node* u, Node* v) {
        return u->d > v->d;
    }
};

struct PrimCompare {
    bool operator() (Node* u, Node* v) {
        return u->key_prim > v->key_prim;
    }
};

class Grafo {
    private:
        int t;

        void add_node(string label = "") {
            string newlabel;
            if(label == "") newlabel = string(1, 'a' + (V.size() % 26));
            else newlabel = label;
            V.push_back(new Node(V.size(), newlabel));
        }

        void edd_edge(int u, int v, int weight) {
            V[u]->adj.push_back(new Edge(V[u], V[v], weight));
        }
    
        void dfs_visit(Node* u, stack<Node*> &s) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                if(edge->v->color == WHITE) {
                    edge->v->p = u;
                    dfs_visit(edge->v, s);
                }
            }

            u->color = BLACK;
            u->f = ++t;
            s.push(u);
        }
    
        void initialize_single_source(Node* s) {
            for(auto& u : V) {
                u->d = __INT_MAX__;
                u->p = nullptr;
            }

            s->d = 0;
        }

        bool relax(Node* u, Node* v, int weight) {
            if(v->d > u->d + weight) {
                v->p = u;
                v->d = u->d + weight;
                // cout << " u " << u->d << " v " << v->d << " weight " << weight << endl;
                return true;
            }
            return false;
        }
    
        vector<Node*> get_trasposed() {
            vector<Node*> trasp;
            for(auto& u : V) {
                for(auto& edge : u->adj) {
                    Node* v = edge->v;
                    trasp[v->key]->adj.push_back(new Edge(V[v->key], V[u->key], edge->weight));
                }
            }

            return trasp;
        }
    
        void DFS_scc(vector<Node*> order) {
            for(auto& u : order) {
                u->color = GRAY;
                
            }
        }
    public:
        vector<Node*> V;

        Grafo() {
            add_node("A"); // Nodo 0
            add_node("B"); // Nodo 1
            add_node("C"); // Nodo 2
            add_node("D"); // Nodo 3

            // Inizializza gli archi (grafo diretto pesato)
            edd_edge(0, 1, 1); // A -> B
            edd_edge(0, 2, 4); // A -> C
            edd_edge(1, 2, 2); // B -> C
            edd_edge(2, 3, 3); // C -> D
            edd_edge(3, 0, 5); // D -> A
        }

        void BFS(int source) {
            Node* s = V[source];
            for(auto& u : V) {
                u->d = INF;
                u->p = nullptr;
                u->color = WHITE;
            }

            queue<Node*> Q;
            s->d = 0;
            s->color = GRAY;
            Q.push(s);

            while(!Q.empty()) {
                Node* u = Q.front(); Q.pop();

                for(auto& edge : u->adj) {
                    Node* v = edge->v;
                    if(v->color == WHITE) {
                        v->d = u->d + 1;
                        v->p = u;
                        v->color = GRAY;

                        Q.push(v);
                    }
                }

                u->color = BLACK;
            }

            // stampa
            cout << "\nStampo grafo dopo esecuzione BFS" << endl;
            for(auto& u : V) {
                cout << "Nodo " << u->key << " -> distanza da s: " << u->d << endl;
            }
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
                    dfs_visit(u, s);

            cout << "\nStampo grafo dopo esecuzione della DFS" << endl;
            for(auto& u : V) {
                cout << "Nodo " << u->key << " -> tempo inizio visita [" << u->d << "] , tempo fine visita [" << u->f << "]" << endl;
            }
            print_toporder(s);
        }

        void print_toporder(stack<Node*> s) {
            cout << "\nOrdinamento topologico: " << endl;
            while(!s.empty()) {
                cout << "[" << s.top()->key << "] \t"; s.pop();
            }
            cout << endl;
        }

        void dijkstra(int source) {
            Node* s = V[source];
            vector<bool> visited (V.size(), false);
            initialize_single_source(s);

            priority_queue<Node*, vector<Node*>, Compare> Q;
            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();
                if(visited[u->key]) continue;
                visited[u->key] = true;
                
                for(auto& edge : u->adj) {
                    relax(u, edge->v, edge->weight);
                    Q.push(edge->v);
                }
            }

            for(int i = 0; i < (int)V.size(); i++) {
                if(i == source) continue;
                cout << "Shortest path to " << i << ": ";
                print_path(V[i]);
                cout << endl;
            }
        }

        void print_path(Node* node) {
            if(node == nullptr) return;
            if(node->p == nullptr) {
                cout << node->key;
                return;
            }

            print_path(node->p);
            cout << " -> " << node->key;
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            for(int i = 0; i < (int)V.size() - 1; i++) {
                for(auto& u : V) {
                    for(auto& edge : u->adj)
                        relax(u, edge->v, edge->weight);
                }
            }

            for(auto& u : V) {
                for(auto& edge : u->adj)
                    if(edge->v->d > u->d + edge->weight)
                        return false;
            }
            
            return true;
        }

        void prim(int r) {
            Node* source = V[r];
            vector<bool> inMST(V.size(), false);
            vector<Edge*> mst;

            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            source->key_prim = 0;
            priority_queue<Node*, vector<Node*>, PrimCompare> Q;
            for(auto& u : V)    
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                if(inMST[u->key]) continue;
                inMST[u->key] = true;

                if(u->p != nullptr) mst.push_back(new Edge(u->p, u, u->key_prim));

                for(auto& edge : u->adj) {
                    if(!inMST[edge->v->key] and edge->v->d > edge->weight) {
                        edge->v->p = u;
                        edge->v->d = edge->weight;

                        Q.push(edge->v);
                    }
                }
            }
        }

        void scc() {
            // 1. DSF che ritorna l'ordinamento topologico
            stack<Node*> s = DFS();

            vector<Node*> temp;
            while (!s.empty()) {
                temp.push_back(s.top());
                s.pop();
            }
            reverse(temp.begin(), temp.end());

            // 2. trasporre grafo
            vector<Node*> g = get_trasposed();

            // 3. DFS su grafo trasposto

        }
};

int main() {
    Grafo g;
    g.BFS(0);
    g.DFS();
    g.dijkstra(0);
    cout << ((g.bellmanford(0)) ? "Non ha trovato cicli negativi" : "ha trovato cicli negativi") << endl;
    g.prim(0);

    return 0;
}