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
    bool operator() (Node* a, Node* b) {
        return a->d > b->d;
    }
};

struct Compare_p {
    bool operator() (Node* a, Node* b) {
        return a->key_prim > b->key_prim;
    }
};

class Grafo {
    private:
        vector<Node*> V; 
        int t;

        void add_node(string label = "") {
            string newlabel = label.empty() ? string(1, 'a' + V.size()) : label;
            V.push_back(new Node(V.size(), newlabel));
        }

        void add_edge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
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
            s.push(u);
            u->f = ++t;
        }
    
        Grafo trasposto() {
            Grafo Gt;

            for (auto& node : V) {
                Gt.add_node(node->label);
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
    public:
        Grafo() {
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

        void bfs(int source) {
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
                        edge->v->color = GRAY;
                        edge->v->p = u;
                        edge->v->d = u->d + 1;

                        Q.push(edge->v);
                    }
                }

                u->color = BLACK;
            }

            cout << "Esecuzione BFS ... " << endl;
            for(auto& u : V) 
                if(u != s)
                    cout << "Nodo: " << u->key << " -> distanza dal nodo sorgente [" << u->d << "] " << endl;
        }

        stack<Node*> dfs() {
            stack<Node*> s;

            for(auto& u : V) {
                u->color = WHITE;
                u->p = nullptr;
            }

            t = 0;
            for(auto& u : V) {
                if(u->color == WHITE)
                    dfs_visit(u, s);
            }

            stack<Node*> temp = s;
            cout << "Ordinamento topologico:" << endl;
            while (!temp.empty()) {
                cout << "Nodo: " << temp.top()->key << " [" << temp.top()->label << "]\n";
                temp.pop();
            }

            return s;
        }

        void scc() {
            stack<Node*> nodes = dfs();
            Grafo gt = trasposto();

            for (auto& u : gt.V) {
                u->color = WHITE;
            }

            cout << "Componenti fortemente connesse (SCC):" << endl;

            while (!nodes.empty()) {
                Node* u = nodes.top();
                nodes.pop();

                if (gt.V[u->key]->color == WHITE) {
                    stack<Node*> componente;  
                    gt.dfs_visit(gt.V[u->key], componente);

                    // Stampa la SCC
                    cout << "{ ";
                    while (!componente.empty()) {
                        cout << componente.top()->label << " ";
                        componente.pop();
                    }
                    cout << "}" << endl;
                }
            }
        }

        bool bellman_ford(int source) {
            initialize_single_source(V[source]);

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

        void bellmanford_s_d(int source, int d) {
            if(!bellman_ford(source)) return;

            Node* tmp = V[d];
            vector<Node*> path;
            while(tmp != V[source]) {
                path.push_back(tmp);
                tmp = tmp->p;
            }

            cout << "Cammino minimo da " << V[source]->key << " a " << V[d]->key << endl;
            for(auto& node : path)
                cout << "[" << node->key << "]\t";
            cout << endl;
        }

        void dijkstra(int source) {
            initialize_single_source(V[source]);

            priority_queue<Node*, vector<Node*>, Compare> Q;
            vector<bool> visited(V.size(), false);
            stack<Node*> s;

            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();

                if(visited[u->key]) continue;
                visited[u->key] = true;
                s.push(u);

                for(auto& edge : u->adj) {
                    relax(u, edge->v, edge->weight);
                    Q.push(edge->v);
                }
            }
        }

        void prim(int source) {
            vector<bool> inMST(V.size(), false);
            vector<Edge*> mst;

            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            V[source]->key_prim = 0;

            priority_queue<Node*, vector<Node*>, Compare_p> Q;

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

                        mst.push_back(edge);
                        Q.push(edge->v);
                    }
                }
            }
        }
};

int main() {
    Grafo g;

    // g.dfs();
    g.bfs(0);
    g.scc();
    g.bellmanford_s_d(0, 4);
    g.dijkstra(0);

    return 0;
}