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
#include <fstream>

using namespace std;

enum Color {WHITE, GRAY, BLACK};
#define INF __INT_MAX__

class Edge;

class Node {
    public:
        int key;
        string label;
        Color color;
        Node* p;
        int d, f, key_prim;
        vector<Edge*> adj;

        Node(int key, string label) : key(key), label(label), color(WHITE) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight = 0) : u(u), v(v), weight(weight) {}
};

struct Compare {
    bool operator() (const Node* a, const Node* b) const {
        return a->d > b->d;
    }
};

struct Compare_Prim {
    bool operator() (const Node* a, const Node* b) const {
        return a->key_prim > b->key_prim; 
    }
};

class UnionFind {
    private:
        vector<Node*> parent;
        vector<int> rank;
    public:
        UnionFind(int n) {
            parent.resize(n);
            rank.resize(n, 0);
            for (int i = 0; i < n; i++) {
                parent[i] = nullptr;
            }
        }

        Node* find(Node* u) {
            if (parent[u->key] != u) {
                parent[u->key] = find(parent[u->key]);
            }
            return parent[u->key];
        }

        void union_sets(Node* u, Node* v) {
            Node* root_u = find(u);
            Node* root_v = find(v);
            if (root_u != root_v) {
                if (rank[root_u->key] > rank[root_v->key]) {
                    parent[root_v->key] = root_u;
                } else {
                    parent[root_u->key] = root_v;
                    if (rank[root_u->key] == rank[root_v->key]) {
                        rank[root_v->key]++;
                    }
                }
            }
        }
};

class Graph {
    private:
        int t = 0;

        void add_node(int key, string label = "") {
            if (label.empty()) {
                label = string(1, 'a' + V.size());
            }
            V.push_back(new Node(key, label));
        }

        void add_edge(int u, int v, int weight) {
            V[u]->adj.push_back(new Edge(V[u], V[v], weight));
        }
    
        void DFS_visit(Node* u) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                Node* v = edge->v;
                if(v->color == WHITE) {
                    v->p = u;
                    DFS_visit(v);
                }
            }

            u->f = ++t;
            u->color = BLACK;
        }
    
        void initialize_single_source(int source) {
            Node* s = V[source];
            for(auto& u : V) {
                u->d = INF;
                u->p = nullptr;
            }

            s->d = 0;
        }

        void relax(Node* u, Node* v, int weight) {
            if(v->d > u->d + weight) {
                v->p = u;
                v->d = u->d + weight;
            }
        }
    public:
        vector<Node*> V;
         
        Graph() {
            ifstream input("GRP.txt");
            if (!input.is_open()) {
                cerr << "Errore: impossibile aprire il file 'GRP.txt'" << endl;
                exit(1);
            }

            int num_nodi, num_archi;
            input >> num_nodi >> num_archi;
            for (int i = 0; i < num_nodi; i++) {
                add_node(i);
            }

            int source, dest, weight;
            while (input >> source >> dest >> weight) {
                add_edge(source, dest, weight);
            }
            input.close();
        }

        void BFS(int source) {
            Node* s = V[source];
            for(auto& u : V) {
                u->d = INF;
                u->color = WHITE;
                u->p = nullptr;
            }
            s->d = 0;
            s->color = GRAY;

            queue<Node*> Q;
            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.front(); Q.pop();
                for(auto& edge : u->adj) {
                    Node* v = edge->v;

                    v->d = u->d + 1;
                    v->p = u;
                    v->color = GRAY;

                    Q.push(v);
                }

                u->color = BLACK;
            }
        }

        void DFS() {
            for(auto& u : V) {
                u->color = WHITE;
                u->d = INF;
                u->p = nullptr;
            }

            for(auto& u : V)
                if(u->color == WHITE)
                    DFS_visit(u);
        }

        bool bellman_ford(int source) {
            initialize_single_source(source);
            for(int i = 0; i < (int)V.size(); i++) 
                for(auto& u : V)
                    for(auto& edge : u->adj)
                        relax(u, edge->v, edge->weight);
            
            for(auto& u : V)
                for(auto& edge : u->adj)
                    if(edge->v->d > u->d  + edge->weight)
                        return false;
            return true;
        }

        void dijkstra(int source) {
            initialize_single_source(source);
            stack<Node*> s;
            priority_queue<Node*, vector<Node*>, Compare> Q;

            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                for(auto& edge : u->adj)
                    relax(u, edge->v, edge->weight);
            }
        }

        void kruskal() {
            vector<Edge> edges;
            
            // Raccoglie tutti gli archi
            for (auto& u : V) {
                for (auto& edge : u->adj) {
                    if (u->key < edge->v->key) { 
                        edges.push_back(*edge);
                    }
                }
            }

            // Ordina gli archi per peso
            sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
                return a.weight < b.weight;
            });

            UnionFind uf(V.size());
            vector<Edge> mst;  // Albero di copertura minimo

            // Aggiungi gli archi al MST
            for (auto& e : edges) {
                if (uf.find(e.u) != uf.find(e.v)) {
                    uf.union_sets(e.u, e.v);
                    mst.push_back(e);
                    if (mst.size() == V.size() - 1) {
                        break;  // trovato il MST
                    }
                }
            }

            // Stampa il MST
            cout << "Albero di copertura minimo (MST):" << endl;
            for (auto& e : mst) {
                cout << "(" << e.u->key << ", " << e.v->key << ") -> Peso: " << e.weight << endl;
            }
        }

        void prim(int r) {
            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            V[r]->key_prim = 0;
            priority_queue<Node*, vector<Node*>, Compare_Prim> Q;

            for(auto& u : V)
                Q.push(u);
            
            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                for(auto& edge : u->adj) {
                    if(edge->v->key_prim > edge->weight) {
                        edge->v->key_prim = edge->weight;
                        edge->v->p = u;
                    }

                    Q.push(edge->v);
                }
            }
        }

        void print() {
            for (auto& u : V) {
                cout << "Nodo {" << u->key << "} ->";
                for (auto& edge : u->adj) {
                    cout << " (" << edge->v->key << ", peso: " << edge->weight << ")";
                }
                cout << endl;
            }
        }
};

int main() {
    // Crea il grafo leggendo dal file "GRP.txt"
    Graph g;
    
    // Stampa il grafo
    cout << "Rappresentazione del grafo:" << endl;
    g.print();

    // Esecuzione di BFS
    int startNode = 0;
    cout << "\nEsecuzione di BFS dal nodo " << startNode << ":" << endl;
    g.BFS(startNode);
    for (auto& node : g.V) {
        cout << "Nodo " << node->key << ", distanza da sorgente: " << node->d << endl;
    }

    // Esecuzione di DFS
    cout << "\nEsecuzione di DFS:" << endl;
    g.DFS();
    for (auto& node : g.V) {
        cout << "Nodo " << node->key << ", tempo di scoperta: " << node->d << ", tempo di completamento: " << node->f << endl;
    }

    // Esecuzione di Bellman-Ford
    cout << "\nEsecuzione di Bellman-Ford dal nodo " << startNode << ":" << endl;
    if (g.bellman_ford(startNode)) {
        cout << "Nessun ciclo negativo rilevato." << endl;
        for (auto& node : g.V) {
            cout << "Nodo " << node->key << ", distanza minima: " << node->d << endl;
        }
    } else {
        cout << "Ciclo negativo rilevato!" << endl;
    }

    // Esecuzione di Dijkstra
    cout << "\nEsecuzione di Dijkstra dal nodo " << startNode << ":" << endl;
    g.dijkstra(startNode);
    for (auto& node : g.V) {
        cout << "Nodo " << node->key << ", distanza minima: " << node->d << endl;
    }

    // Esecuzione di Kruskal
    cout << "\nEsecuzione di Kruskal per l'MST:" << endl;
    g.kruskal();

    // Esecuzione di Prim
    cout << "\nEsecuzione di Prim per l'MST dal nodo " << startNode << ":" << endl;
    g.prim(startNode);

    cout << "Risultato Prim:" << endl;
    for (auto& node : g.V) {
        if (node->p) {
            cout << "Nodo " << node->key << " connesso a " << node->p->key << " con peso " << node->key_prim << endl;
        } else {
            cout << "Nodo " << node->key << " è la radice MST." << endl;
        }
    }

    return 0;
}