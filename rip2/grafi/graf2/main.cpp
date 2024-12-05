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
#include <stack>
#include <queue>

#define INF __INT_MAX__

using namespace std;

enum Color {WHITE, GRAY, BLACK};
enum Type {UNDEFINED, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};

class Edge;

class Node {
    public:
        int key, prim_key;
        string label;
        Node* p;
        int d, f;
        vector<Edge*> adj;
        vector<Edge*> reverse_adj;
        Color color;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;
        Type type;

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight), type(UNDEFINED) {}
};

struct Compare {
    bool operator() (Node* a, Node* b) {
        return a->d > b->d;
    }
};

struct Compare_prim {
    bool operator() (Node* a, Node* b) {
        return a->prim_key > b->prim_key;
    }
};

class UnionSet {
    private:
        vector<int> parent;
        vector<int> rank;
    public:
        UnionSet(int n) {
            parent.resize(n);
            rank.resize(n);
            for(int i = 0; i < n; i++) {
                parent[i] = i;
                rank[i] = INF;
            }
        }

        int find_set(int u) {
            if(parent[u] != u)
                parent[u] = find_set(parent[u]);
            return parent[u];
        }

        void _union(int u, int v) {
            int find_u = find_set(u), find_v = find_set(v);

            if(rank[find_u] < rank[find_v])
                parent[find_v] = find_u;
            else if(rank[find_v] < rank[find_v])
                parent[find_u] = find_v;
            else {
                parent[find_v] = find_u;
                rank[find_u]++;
            } 
        }
};

class Grafo {
    private:
        int t, ncycle;
        vector<Node*> V;

        void add_node(string label = "") {
            string newlabel;
            if(label == "") newlabel[0] = V.size() + 97;
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
        }
    
        void dfs_visit(Node* u, stack<Node*> &s) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                Node* v = edge->v;

                if(v->color == WHITE) {
                    v->p = u;
                    edge->type = TREE_EDGE;
                    dfs_visit(v, s);
                } else if(v->color == GRAY) {
                    ncycle++;
                    edge->type = BACK_EDGE;
                } else edge->type = (u->d < v->d) ? CROSS_EDGE : FORWARD_EDGE;
            }

            u->color = BLACK;
            u->f = ++t;

            s.push(u);
        }
    
        void initialize_single_source(Node* s) {
            for(auto& u : V) {
                u->d = INF;
                u->p = nullptr;
            }

            s->d = 0;
        }

        void relax(Node* u, Node* v, int weight) {
            if(v->d > u->d + weight) {
                v->d = u->d + weight;
                v->p = u;
            }
        }
    
        void transpose_graph() {
            // Svuota le reverse_adj di ogni nodo
            for (auto& node : V) {
                node->reverse_adj.clear();
            }

            // Costruisci le reverse_adj invertendo gli archi
            for (auto& node : V) {
                for (auto& edge : node->adj) {
                    edge->v->reverse_adj.push_back(new Edge(edge->v, edge->u, edge->weight));
                }
            }

            // Svuota le adj e popola con gli archi trasposti
            for (auto& node : V) {
                node->adj.clear();
                for (auto& edge : node->reverse_adj) {
                    node->adj.push_back(edge);
                }
            }

            cout << "Il grafo è stato trasposto con successo." << endl;
        }
    public:
        Grafo() {
            add_node("A");
            add_node("B");
            add_node("C");
            add_node("D");

            add_edge(0, 1, 1); // A -> B
            add_edge(0, 2, 1); // A -> C
            add_edge(1, 2, 1); // B -> C
            add_edge(2, 3, 1); // C -> D
            add_edge(3, 1, 1); // D -> B (ciclo)
        }

        stack<Node*> dfs() {
            stack<Node*> s;

            for(auto& u : V) {
                u->color = WHITE;
                u->p = nullptr;
            }

            t = 0; ncycle = 0;
            for(auto& u : V)
                if(u->color == WHITE)
                    dfs_visit(u, s);
                

            cout << "Sono stati rilevati " << ncycle << " cicli." << endl;
            print_stack(s);

            return s;
        }

        void print_stack(stack<Node*> s) {
            vector<int> nodes;
            while(!s.empty()) {
                nodes.push_back(s.top()->key); s.pop(); 
            }

            cout << "Stampa ordinamento topologico ... " << endl;
            for(int el : nodes) 
                cout << " - " << el << " - \t";
            cout << endl;
        }

        void BFS(int source) {
            Node* s = V[source];
            queue<Node*> Q;

            for(auto& u : V) {
                u->color = WHITE;
                u->d = INF;
                u->p = nullptr;
            }

            s->d = 0;
            s->color = GRAY;
            Q.push(s);

            while(!Q.empty()) {
                Node* u = Q.front(); Q.pop();

                for(auto& edge : u->adj) {
                    if(edge->v->color == WHITE) {
                        edge->v->d = u->d + 1;
                        edge->v->p = u;
                        edge->v->color = GRAY;

                        Q.push(edge->v);
                    }
                }

                u->color = BLACK;
            }

            for(auto& u : V) {
                if(u == s) continue;
                cout << "Nodo " << u->key << " distanza da s: " << u->d << endl;
            }
        }

        void dijkstra(int source) {
            Node* s = V[source];
            initialize_single_source(s);
            vector<bool> vis(V.size(), false);

            priority_queue<Node*, vector<Node*>, Compare> pq;

            for(auto& u : V)
                pq.push(u);

            while(!pq.empty()) {
                Node* u = pq.top(); pq.pop();

                if(vis[u->key]) continue;
                vis[u->key] = true;

                for(auto& edge : u->adj) {
                    relax(u, edge->v, edge->weight);
                    pq.push(edge->v);
                }
            }
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            for(int i = 0; i < V.size(); i++) {
                for(auto& u : V)
                    for(auto& edge : u->adj)
                        relax(u, edge->v, edge->weight);
            }

            for(auto& u : V)
                for(auto& edge : u->adj)
                    if(edge->v->d > u->d + edge->weight) return false;

            return true;
        }

        void prim(int source) {
            vector<bool> inMST(V.size(), false);

            for(auto& u : V) {
                u->prim_key = INF;
                u->p = nullptr;
            }

            Node* s = V[source];
            s->prim_key = 0;

            priority_queue<Node*, vector<Node*>, Compare_prim> pq;
            for(auto& u : V)
                pq.push(u);

            while(!pq.empty()) {
                Node* u = pq.top(); pq.pop();

                if(inMST[u->key]) continue;
                inMST[u->key] = true;

                for(auto& edge : u->adj) {
                    if(edge->v->prim_key > edge->weight) {
                        edge->v->prim_key = edge->weight;
                        edge->v->p = u;

                        pq.push(edge->v);
                    }
                }
            }
        }

        void kruskal() {
            vector<Edge*> E;
            stack<Edge*> mst;
            UnionSet us(V.size());

            for(auto& u : V) {
                for(auto& edge : u->adj)
                    E.push_back(edge);
            }

            sort(E.begin(), E.end());
            for(auto& edge : E) {
                if(us.find_set(edge->u->key) != us.find_set(edge->v->key)) {
                    us._union(edge->u->key, edge->v->key);
                    mst.push(edge);
                }
            }
        }

        void scc() {
            stack<Node*> s = dfs();
            transpose_graph();

            for (auto& u : V) {
                u->color = WHITE;
            }

            cout << "Eseguo la DFS sul grafo trasposto..." << endl;
            while (!s.empty()) {
                Node* u = s.top(); s.pop();
                if (u->color == WHITE) {
                    stack<Node*> component;
                    dfs_visit(u, component);
                    cout << "Nuova componente fortemente connessa trovata:" << endl;
                    while (!component.empty()) {
                        cout << component.top()->label << " ";
                        component.pop();
                    }
                    cout << endl;
                }
            }
        }
};

int main() {
    Grafo g;

    cout << "Eseguo DFS . . ." << endl;
    g.dfs();

    cout << "Eseguo BFS . . ." << endl;
    g.BFS(0);

    cout << "Eseguo Dijkstra . . ." << endl;
    g.dijkstra(0);

    cout << "Eseguo BellamnFord . . ." << endl;
    g.bellmanford(0);

    cout << "Eseguo Prim . . ." << endl;
    g.prim(0);

    cout << "Eseguo Kruskal . . ." << endl;
    g.kruskal();

    cout << "Calcolo componenti fortemente connesse . . ." << endl;
    g.scc();

    return 0;
}