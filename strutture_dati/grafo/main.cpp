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
        int d, f, prim_key;
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
        return a->prim_key > b->prim_key; 
    }
};

bool compareEdges(Edge* e1, Edge* e2) {
    return e1->weight < e2->weight;
}

class UnionFind {
    public:
        vector<int> parent;
        vector<int> rank;

        UnionFind(int n) {
            parent.resize(n);
            rank.resize(n);
            for(int i = 0; i < n; i++)
                parent[i] = i;
        }

        int find(int x) {
            if(parent[x] != x)
                parent[x] = find(parent[x]);
            return parent[x];
        }

        void unionset(int x, int y) {
            int findx = find(x);
            int findy = find(y);

            if(rank[findx] < rank[findy])
                parent[findy] = findx;
            else if(rank[findy] < rank[findx])
                parent[findx] = findy;
            else {
                parent[findy] = findx;
                findx++;
            }
        }
};

class Graph {
    private:
        int t = 0;

        vector<Edge*> E;

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

        void getedges() {
            for (auto& u : V) 
                for (auto& edge : u->adj) 
                    E.push_back(edge);
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

        void prim(int s) {
            Node* r = V[s];
            vector<Edge*> mst;
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
                    mst.push_back(new Edge(u->p, u, u->prim_key)); 
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
            print_mst(mst);
        }

        vector<Edge*> kruskal() {
            getedges();
            vector<Edge*> mst;
            sort(E.begin(), E.end(), compareEdges);

            UnionFind uf(V.size());

            for(Edge* e : E) {
                int x = e->u->key;
                int y = e->v->key;

                if(uf.find(x) != uf.find(y)) {
                    mst.push_back(e);
                    uf.unionset(x, y);
                }
            }

            return mst;
        }

        void print_mst(vector<Edge*> mst) {
            for (Edge* edge : mst) {
                cout << edge->u->label << " - " << edge->v->label << " : " << edge->weight << "\n";
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
    g.print_mst(g.kruskal());

    // Esecuzione di Prim
    cout << "\nEsecuzione di Prim per l'MST dal nodo " << startNode << ":" << endl;
    g.prim(startNode);

    return 0;
}