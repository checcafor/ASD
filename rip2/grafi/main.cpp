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
        Node* p;
        Color color;

        Node(int key, string label) : key(key), label(label) {}
};

class Edge {
    public:
        Node* u;
        Node* v;
        int weight;

        Edge(Node* u, Node* v, int weight) : u(u), v(v), weight(weight) {}
};

class UnionFind {
    private:
        vector<int> parent;
        vector<int> rank;
    public:
        UnionFind(int n) {
            parent.resize(n);
            rank.resize(n);
            for(int i = 0; i < n; i++)
                parent[i] = i;
        }

        int findset(int i) {
            if(parent[i] != i) 
                parent[i] = findset(parent[i]);
            return parent[i];
        }

        void union_(int u, int v) {
            int find_u = findset(u), find_v = findset(v);

            if(rank[find_u] < rank[find_v])
                parent[find_v] = find_u;
            else if(rank[find_v] < rank[find_u])
                parent[find_u] = find_v;
            else {
                parent[find_v] = find_u;
                rank[find_u]++;
            }
        }
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
        vector<Node*> V;
        int t;

        void add_node(string label = "") {
            string newlabel;
            if(label == "")
                newlabel = string(1, 'a' + V.size());
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int ukey, int vkey, int w) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], w));
        }

        void DFS_visit(Node* u, stack<Node*> &s) {
            u->color = GRAY;
            u->d = ++t;

            for(auto& edge : u->adj) {
                if(edge->v->color == WHITE) {
                    edge->v->p = u;
                    DFS_visit(edge->v, s);
                }
            }

            u->color = BLACK;
            s.push(u);
            u->f = ++t;
        }

        Grafo trasposto() {
            Grafo Gt;
            for (size_t i = 0; i < V.size(); ++i) {
                Gt.add_node(V[i]->label);
            }
            for (auto& u : V) {
                for (auto& edge : u->adj) {
                    Gt.add_edge(edge->v->key, edge->u->key, edge->weight);
                }
            }
            return Gt;
        }
    
        void initialize_single_source(Node* s) {
            for(auto& u : V) {
                u->color = WHITE;
                u->d = INF;
            }

            s->d = 0;
        }

        void relax(Node* u, Node* v, int weight) {
            if(v->d > u->d + weight) {
                v->d = u->d + weight;
                v->p = u;
            }
        }
    public:
        Grafo() {
            add_node("a");
            add_node("b");
            add_node("c");
            add_node("d");
            add_node("e");

            add_edge(0, 1, 1); // a -> b
            add_edge(1, 2, 1); // b -> c
            add_edge(2, 0, 1); // c -> a
            add_edge(1, 3, 1); // b -> d
            add_edge(3, 4, 1); // d -> e
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

        void trovaSCC() {
            // 1. Calcola l'ordine di finitura
            stack<Node*> ordine = DFS();

            // 2. Crea il grafo trasposto
            Grafo Gt = trasposto();

            // 3. Esegui DFS sul grafo trasposto nell'ordine inverso di finitura
            for (auto& u : Gt.V) {
                u->color = WHITE;
            }

            while (!ordine.empty()) {
                Node* u = ordine.top();
                ordine.pop();

                if (Gt.V[u->key]->color == WHITE) {
                    stack<Node*> componente;  // Per raccogliere i nodi della SCC
                    Gt.DFS_visit(Gt.V[u->key], componente);

                    // Stampa o salva la SCC
                    cout << "SCC: ";
                    while (!componente.empty()) {
                        cout << componente.top()->label << " ";
                        componente.pop();
                    }
                    cout << endl;
                }
            }
        }

        void BFS(int source) {
            Node* s = V[source];
            for(auto& u : V) {
                u->color = WHITE;
                u->d = INF;
                u->p = nullptr;
            }

            queue<Node*> Q;
            s->d = 0;
            s->color = GRAY;

            Q.push(s);

            while(!Q.empty()) {
                Node* u = Q.front(); Q.pop();

                for(auto& edge : u->adj) {
                    if(edge->v->color == WHITE) {
                        edge->v->p = u;
                        edge->v->d = u->d + 1;
                        Q.push(edge->v);
                    }
                }

                u->color = BLACK;
            }
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            for(int i = 0; i < (int)V.size(); i++) {
                for(auto& u : V) {
                    for(auto& edge : u->adj) {
                        relax(u, edge->v, edge->weight);
                    }
                }
            }

            for(auto& u : V) {
                for(auto& edge : u->adj) {
                    if(edge->v->d > u->d + edge->weight)
                        return false;
                }
            }

            return true;
        }

        void dijkstra (int source) {
            Node* s = V[source];
            initialize_single_source(s);
            stack<Node*> st; vector<bool> visited (V.size(), false);

            priority_queue<Node*, vector<Node*>, Compare> pq;
            for(auto& u : V)
                pq.push(u);

            while(!pq.empty()) {
                Node* u = pq.top(); pq.pop();

                if(visited[u->key]) continue;
                visited[u->key] = true;

                st.push(u);

                for(auto& edge : u->adj) {
                    relax(u, edge->v, edge->weight);

                    pq.push(edge->v);
                }
            }
        }

        void kruskal() {
            UnionFind uf(V.size());

            vector<Edge*> edges;
            for(auto& u : V) {
                for(auto& edge : u->adj) 
                    edges.push_back(edge);
            }

            sort(edges.begin(), edges.end());

            for(auto& edge : edges) {
                if(uf.findset(edge->u->key) != uf.findset(edge->v->key))
                    uf.union_(edge->u->key, edge->v->key);
            }
        }

        void prim(int source) {
            Node* s = V[source];
            vector<Edge*> mst;
            int n = V.size();
            vector<bool> inMST(n, false);

            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            s->key_prim = 0;
            priority_queue<Node*, vector<Node*>, PrimCompare> Q;

            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                if (inMST[u->key]) continue;
                inMST[u->key] = true;

                if (u->p != nullptr) {
                    mst.push_back(new Edge(u->p, u, u->key_prim)); 
                }
                
                for(auto& edge : u->adj) {
                    if (!inMST[edge->v->key] and edge->weight < edge->v->key_prim) {
                        edge->v->p = u;
                        edge->v->key_prim = edge->weight;
                        Q.push(edge->v);
                    }
                }
            }
        }
};

int main() {
    Grafo g;

    g.trovaSCC();
    g.BFS(0);
    g.bellmanford(0);
    g.dijkstra(0);
    g.kruskal();

    return 0;
}