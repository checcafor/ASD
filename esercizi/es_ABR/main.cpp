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

using namespace std;

class Node {
    public:
        int key;
        int val;
        Node *parent, *left, *right;

        Node(int key) : key(key) {parent = left = right = nullptr;}
};

class ABR {
    private:
        Node* root;

        void transplant(Node* u, Node* v) {
            Node* up = u->parent;

            if(up == nullptr)
                root = v;
            else if(up->left == u)
                up->left = v;
            else up->right = v;

            if(v != nullptr) v->parent = up;
        }

        Node* min(Node* node) {
            Node* tmp = node;
            while(tmp->left != nullptr)
                tmp = tmp->left;
            
            return tmp;
        }

        void deletion(Node* z) {
            if(z->left == nullptr)
                transplant(z, z->right);
            else if(z->right == nullptr)
                transplant(z, z->left);
            else {
                Node* y = min(z->right);
                if(y != z->right) {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
            }
        }
    
        void mindiff_inorder(Node* curr, Node* prev, Node* &p, Node* &f, int& min) {
            if(curr == nullptr) return;

            mindiff_inorder(curr->left, curr, p, f, min);

            if (prev != nullptr) {
                int currmin = abs(curr->key - prev->key);
                if (currmin < min) {
                    min = currmin;
                    p = prev;
                    f = curr;
                }
            }

            mindiff_inorder(curr->right, curr, p, f, min);
        }
    public:
        ABR() : root(nullptr) {}

        void insert(int key) {
            Node* z = new Node(key);
            Node* x = root; Node* xp = nullptr;

            while(x != nullptr) {
                xp = x;
                x = (x->key > key) ? x->left : x->right;
            }

            z->parent = xp;
            if(xp == nullptr)
                root = z;
            else if(xp->key > key)
                xp->left = z;
            else 
                xp->right = z;
        }

        Node* search(Node* node, int key) {
            if(node == nullptr or node->key == key)
                return node;
            return (node->key > key) ? search(node->left, key) : search(node->right, key);
        }

        bool del(int key) {
            Node* z = search(root, key);

            if(z == nullptr) return false;
            deletion(z);
            delete(z);

            return true;
        }

        void mindiff() {
            Node* u = root; Node* up = nullptr;
            Node* f = nullptr; Node* p = nullptr;
            int min = __INT_MAX__;
            mindiff_inorder(u, up, p, f, min);

            cout << "La coppia di nodi padre figlio con la minima differenza tra le chiavi è la seguente: " << endl;
            cout << " {" << p->key << "} -> {" << f->key << "}" << endl;
        }
};

int main() {
    ABR abr;
    abr.insert(10);
    abr.insert(5);
    abr.insert(15);
    abr.insert(3);
    abr.insert(7);

    abr.mindiff(); 

    abr.del(5);
    abr.mindiff();

    return 0;
}
