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
#include <queue>

using namespace std;
enum Color {WHITE, GRAY, BLACK};

#define INF __INT_MAX__

class Edge;

class Node {
    public:
        int key, key_prim;
        string label;
        vector<Edge*> adj;
        Node* p;
        int d, f;
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
        vector<int> parent; vector<int> rank;
    public:
        UnionFind(vector<Node*> nodes) {
            parent.resize(nodes.size());
            rank.resize(nodes.size());

            for(auto& u : nodes) {
                rank[u->key] = 0;
                parent[u->key] = u->key;
            }
        }

        int findset(int ukey) {
            if(parent[ukey] != ukey)
                parent[ukey] = findset(parent[ukey]);

            return parent[ukey];
        }

        void union_ (int ukey, int vkey) {
            int uparent = findset(ukey), vparent = findset(vkey);
            if(uparent != vparent) {
                if(rank[ukey] < rank[vkey])
                    parent[vkey] = ukey;
                else if(rank[vkey] < rank[ukey])
                    parent[ukey] = vkey;
                else {
                    parent[vkey] = ukey;
                    rank[ukey]++;
                }
            }
        }
};

struct Compare {
    bool operator() (Edge* u, Edge* v) {
        return u->weight < v->weight; // Ordinamento crescente
    }
};

struct D_Compare {
    bool operator() (Node* u, Node* v) {
        return u->d > v->d;
    }
};

struct P_Compare {
    bool operator() (Node* u, Node* v) {
        return u->key_prim > v->key_prim;
    }
};

class Grafo {
    private:
        int t;
        void addnode(string label = "") {
            string newlabel = "";
            if(label == "") newlabel[0] = 97 + V.size();
            else newlabel = label;

            V.push_back(new Node(V.size(), newlabel));
        }

        void addedge(int ukey, int vkey, int weight) {
            V[ukey]->adj.push_back(new Edge(V[ukey], V[vkey], weight));
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
                v->d = u->d + weight;
                v->p = u;
            }
        }
    public:
        vector<Node*> V;
        Grafo() {
            ifstream input("IN.txt");
            if(!input) return;

            int nodi, archi; char sep;
            input >> nodi >> sep >> archi;
            for(int i = 0; i < nodi; i++)
                addnode();

            int source, dest, weight;
            while(input >> sep >> source >> sep >> dest >> sep >> weight >> sep) {
                addedge(source - 1, dest - 1, weight);
            }

            input.close();
        }

        void DFS() {
            for(auto& u : V) {
                u->p = nullptr;
                u->color = WHITE;
            }

            t = 0;
            for(auto& u : V)
                if(u->color == WHITE)
                    DFS_visit(u);

            cout << "Eseguo DFS . . ." << endl;
            for(auto& u : V) {
                cout << "Nodo: " << u->key << "-> tempo inizio visita[" << u->d << "] - tempo fine visita[" << u->f << "]" << endl;
            }
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
                    if(edge->v->color == WHITE) {
                        edge->v->d = u->d + 1;
                        edge->v->p = u;
                        edge->v->color = GRAY;

                        Q.push(edge->v);
                    }
                }
            }

            cout << "Eseguo BFS . . ." << endl;
            for(auto& u : V) {
                cout << "Nodo: " << u->key << " -> distanza dal nodo sorgente [" << u->d << "]" << endl;
            }
        }

        void dijkstra(int source) {
            Node* s = V[source];
            initialize_single_source(s);

            priority_queue<Node*, vector<Node*>, D_Compare> Q;
            vector<Node*> path;

            for(auto& u : V)
                Q.push(u);

            while(!Q.empty()) {
                Node* u = Q.top(); Q.pop();
                path.push_back(u);
                for(auto& edge : u->adj)
                    relax(u, edge->v, edge->weight);
            }

            cout << "Eseguo dijkstra . . ." << endl;
            for(auto& el : path)
                cout << "[" << el->key << "]\t" << endl;
        }

        bool bellmanford(int source) {
            Node* s = V[source];
            initialize_single_source(s);
            int size = V.size();

            for(int i = 0; i < size - 1; i++) {
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
            vector<Edge*> mst;

            for(auto& u : V) {
                u->key_prim = INF;
                u->p = nullptr;
            }

            s->key_prim = 0;
            priority_queue<Node*, vector<Node*>, P_Compare> pq;

            for(auto& u : V)
                pq.push(u);

            while(!pq.empty()) {
                Node* u = pq.top(); pq.pop();

                if(inMST[u->key]) continue;

                for(auto& edge : u->adj) {
                    if(edge->v->key_prim > edge->weight) {
                        edge->v->p = u;
                        edge->v->key_prim = edge->weight;

                        pq.push(edge->v);
                        mst.push_back(edge);
                    }
                }
            }

            cout << "Eseguo prim . . ." << endl;
            for(auto& edge : mst)
                cout << "{" << edge->u->key << ", " << edge->v->key << "}" << endl;
        }

        void kruskal() {
            vector<Edge*> E;
            for(auto& u : V) {
                for(auto& edge : u->adj)
                    E.push_back(edge);
            }

            sort(E.begin(), E.end(), Compare());
            vector<Edge*> mst;

            UnionFind uf(V);

            for(auto& edge : E) {
                if(uf.findset(edge->u->key) != uf.findset(edge->v->key)) {
                    uf.union_(edge->u->key, edge->v->key);
                    mst.push_back(edge);
                }
            }

            cout << "Eseguo kruskal . . ." << endl;
            for(auto& edge : mst)
                cout << "{" << edge->u->key << ", " << edge->v->key << "}" << endl;
        }
};

int main() {
    Grafo g;

    g.DFS();
    g.BFS(0);
    g.dijkstra(0);
    cout << "Bellmanford " << ((g.bellmanford(0)) ? "non ha" : "ha") << " rilevato cicli negativi" << endl;
    g.prim(0);
    g.kruskal();

    return 0;
}