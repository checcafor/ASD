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
#include <fstream>

using namespace std;
enum Color {WHITE, GRAY, BLACK};
#define INF __INT_MAX__

class Edge;

class Node {
    public:
        int key, key_prim;
        string label;
        int d, f;
        vector<Edge*> adj;
        Color color;
        Node* p;

        Node(int key, string label) : key(key), label(label) {}
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

        int find_set(int ukey) {
            if(parent[ukey] != ukey) {
                parent[ukey] = find_set(parent[ukey]);
            }

            return parent[ukey];
        }

        void union_(int u, int v) {
            int ru = find_set(u), rv = find_set(v);

            if(rank[ru] < rank[rv])
                parent[rv] = ru;
            else if(rank[rv] < rank[ru])
                parent[ru] = rv;
            else {
                parent[rv] = ru;
                rank[ru]++;
            }
        }
};

struct Compare {
    bool operator() (Node* a, Node* b) {
        return a->d > b->d;
    }
};

struct Compare_Prim {
    bool operator() (Node* a, Node* b) {
        return a->key_prim > b->key_prim;
    }
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
        vector<Node*> V;
        int t;

        void add_node(string label = "") {
            string newlabel = (label == "") ? string(1, 'a' + V.size()) : label;
            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
        }

        void dfs_visit(Node* u) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                Node* v = edge->v;
                if(v->color == WHITE) {
                    v->p = u;
                    dfs_visit(v);
                }
            }

            u->color = BLACK;
            u->f = ++t;
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
                v->p = u;
                v->d = u->d + weight;
            }
        }
    
        Graph transpose() {
            Graph T;
            T.V.resize(V.size());

            for (size_t i = 0; i < V.size(); ++i) {
                T.V[i] = new Node(V[i]->key, V[i]->label);
            }

            for (auto& u : V) {
                for (auto& edge : u->adj) {
                    T.V[edge->v->key]->adj.push_back(new Edge(T.V[edge->v->key], T.V[u->key], edge->weight));
                }
            }
            return T;
        }
    public:
        Graph() {
            ifstream input("GRP.txt");
            if(!input) return;

            int num_nodi, num_archi;
            input >> num_nodi >> num_archi;

            for(int i = 0; i < num_nodi; i++)
                add_node();

            int source, dest, weight;
            while(input >> source >> dest >> weight)
                add_edge(source, dest, weight);

            input.close();
        }

        void dfs() {
            for(auto& u : V) {
                u->color = WHITE;
                u->p = nullptr;
            }

            t = 0;

            for(auto& u : V) {
                if(u->color == WHITE)
                    dfs_visit(u);
            }
        }

        void bfs(int source) {
            Node* s = V[source];
            queue<Node*> Q;

            for(auto& u : V) {
                u->p = nullptr;
                u->d = __INT_MAX__;
                u->color = WHITE;
            }

            s->d = 0;
            s->color = GRAY;

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
        }

        void dijkstra (int source) {
            Node* s = V[source];
            priority_queue<Node*, vector<Node*>, Compare> Q;
            vector<bool> visited(V.size(), false);

            initialize_single_source(s);
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
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            for(int i = 0; i < (int)V.size(); i++) {
                for(auto& u : V)
                    for(auto& edge : u->adj) 
                        relax(u, edge->v, edge->weight);
            }

            for(auto& u : V)
                for(auto& edge : u->adj) 
                    if(edge->v->d > u->d + edge->weight)
                        return false;

            return true;
        }

        void prim(int source) {
            Node* s = V[source];
            vector<bool> inMST(V.size(), false);

            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            s->key_prim = 0;
            priority_queue<Node*, vector<Node*>, Compare_Prim> Q;

            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                if(inMST[u->key]) continue;
                inMST[u->key] = true;

                for(auto& edge : u->adj) {
                    if(edge->v->key_prim > edge->weight) {
                        edge->v->key_prim = edge->weight;
                        edge->v->p = u;

                        Q.push(edge->v);
                    }
                }
            }
        }

        void kruskal() {
            UnionSet us(V.size());
            vector<Edge*> edges;

            for(auto& u : V) 
                for(auto& edge : u->adj)
                    edges.push_back(edge);
            
            sort(edges.begin(), edges.end());
            for(auto& edge : edges) {
                if(us.find_set(edge->u->key) != us.find_set(edge->v->key)) {
                    us.union_(edge->u->key, edge->v->key);
                }
            }
        }
};

int main() {
    Graph g;
    g.dfs();
    g.bfs(0);
    g.dijkstra(0);
    g.bellmanford(0);
    g.prim(0);
    g.kruskal();

    return 0;
}